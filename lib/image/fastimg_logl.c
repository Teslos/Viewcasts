/*
 * Copyright 1994 Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABLIITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * In no event shall Silicon Graphics be liable for any special, incidental,
 * indirect or consequential damages of any kind, or any damages whatsoever
 * resulting from loss of use, data or profits, whether or not advised of the
 * possibility of damage, and on any theory of liability, arising out of or
 * in connection with the possession, use or performance of this software.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND
 * Use, duplication, or disclosure by the Government is subject to restrictions
 * set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013 and/or in
 * similar or successor clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the United States.
 * Contractor/manufacturer is Silicon Graphics, Inc., 2011 N. Shoreline Blvd.,
 * Mountain View, CA 94039-7311.
 */
/*
 *   	fastimg_logl -
 *		Faster copies between long pixel data and image files.
 *	This could be done on top of the iris image library, but the
 *	performance is lower by a factor of two of so.
 *
 *      This code should work on machines with any byte order.
 *
 *	Could someone make this run real fast using multiple processors?
 *
 *				Paul Haeberli - 1991
 */

/*
 *	Memory mapped file routines
 *
 */
#include <stdio.h>
#include "resource.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct MFILE {
    unsigned char *paddr;
    unsigned char *fpos;
    int flen;
    FILE *file;
} MFILE;

static MFILE *mfopen(char *name, char *mode)
{
    int fd, i;
    struct stat sb;
    MFILE *mf;

    if(mode[0] == 'r') {
	if(name[0] == '#') {
	    mf = (MFILE *)malloc(sizeof(MFILE));
	    mf->file = res_fopen(name,"r");
	    if(mf->file)
    		return mf;
	    else
		return 0;
	} 
	fd = open(name,0);
	if(fd<0) 
	    return 0;
	fstat(fd,&sb);
	mf = (MFILE *)malloc(sizeof(MFILE));
	mf->flen = sb.st_size;
	mf->paddr = (unsigned char *)mmap(0,mf->flen,PROT_READ,MAP_SHARED,fd,0);
	mf->file = 0;
	close(fd);
	if(!mf->paddr) 
	    return 0;
	mf->fpos = mf->paddr;
	return mf;
    } else {
	fprintf(stderr,"not done yet\n");
	return 0;
    }
}

static unsigned char *rbuf;
static unsigned int rbuflen;

static unsigned char *mfread(MFILE *mf, int n)
{
    unsigned char *ret;

    if(mf->file) {
	if(!rbuf) {
    	    rbuf = (unsigned char *)malloc(n);
	    rbuflen = n;
	} else if (rbuflen<n) {
     	    rbuf = (unsigned char *)realloc(rbuf,n);
	    rbuflen = n;
	}
	fread(rbuf,n,1,mf->file);
	return rbuf;
    } else {
	ret = mf->fpos;
	mf->fpos += n;
	return ret;
    }
}

static mfseek(MFILE *mf, int pos)
{
    if(mf->file) {
	res_fseek(mf->file,pos,SEEK_SET);
    } else
	mf->fpos = mf->paddr+pos;
}

static mfclose(MFILE *mf)
{
    if(mf->file)
  	res_fclose(mf->file);
    else
	munmap(mf->paddr,mf->flen);
    free(mf);
}

#include "image.h"
#include "unistd.h"
#include "lum.h"

#define OFFSET_R	3	/* this is byte order dependent */
#define OFFSET_G	2
#define OFFSET_B	1
#define OFFSET_A	0

#define CHANOFFSET(z)	(3-(z))	/* this is byte order dependent */

static expandrow();
static setalpha();
static copybw();
static interleaverow();
static int compressrow();
static lumrow();

#define TAGLEN	(5)

/*
 *	addlongimgtag - 
 *		this is used to extract image data from core dumps.
 *
 */
addlongimgtag(unsigned long *dptr, int xsize, int ysize)
{
    dptr = dptr+(xsize*ysize);
    dptr[0] = 0x12345678;
    dptr[1] = 0x59493333;
    dptr[2] = 0x69434222;
    dptr[3] = xsize;
    dptr[4] = ysize;
}

/*
 *	byte order independent read/write of shorts and longs.
 *
 */
static unsigned short getshort(MFILE *inf)
{
    unsigned char *buf;

    buf = mfread(inf,2);
    return (buf[0]<<8)+(buf[1]<<0);
}

static unsigned long getlong(MFILE *inf)
{
    unsigned char *buf;

    buf = mfread(inf,4);
    return (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+(buf[3]<<0);
}

static putshort(FILE *outf, unsigned short val)
{
    unsigned char buf[2];

    buf[0] = (val>>8);
    buf[1] = (val>>0);
    fwrite(buf,2,1,outf);
}

static int putlong(FILE *outf, unsigned long val)
{
    unsigned char buf[4];

    buf[0] = (val>>24);
    buf[1] = (val>>16);
    buf[2] = (val>>8);
    buf[3] = (val>>0);
    return fwrite(buf,4,1,outf);
}

static readheader(MFILE *inf, IMAGE *image)
{
    bzero(image,sizeof(IMAGE));
    image->imagic = getshort(inf);
    image->type = getshort(inf);
    image->dim = getshort(inf);
    image->xsize = getshort(inf);
    image->ysize = getshort(inf);
    image->zsize = getshort(inf);
}

static int writeheader(FILE *outf, IMAGE *image)
{
    IMAGE t;

    bzero(&t,sizeof(IMAGE));
    fwrite(&t,sizeof(IMAGE),1,outf);
    fseek(outf,0,SEEK_SET);
    putshort(outf,image->imagic);
    putshort(outf,image->type);
    putshort(outf,image->dim);
    putshort(outf,image->xsize);
    putshort(outf,image->ysize);
    putshort(outf,image->zsize);
    putlong(outf,image->min);
    putlong(outf,image->max);
    putlong(outf,0);
    return fwrite("no name",8,1,outf);
}

static int writetab(FILE *outf, unsigned long *tab, int len)
{
    int r;

    while(len) {
	r = putlong(outf,*tab++);
	len -= 4;
    }
    return r;
}

static readtab(MFILE *inf, unsigned long *tab, int len)
{
    while(len) {
	*tab++ = getlong(inf);
	len -= 4;
    }
}

/*
 *	sizeofimage - 
 *		return the xsize and ysize of an iris image file.
 *
 */
sizeofimage(char *name, int *xsize, int *ysize)
{
    IMAGE image;
    MFILE *inf;

    inf = mfopen(name,"r");
    if(!inf) {
	fprintf(stderr,"sizeofimage: can't open image file %s\n",name);
	exit(1);
    }
    readheader(inf,&image);
    if(image.imagic != IMAGIC) {
	fprintf(stderr,"sizeofimage: bad magic number in image file %s\n",name);
	exit(1);
    }
    *xsize = image.xsize;
    *ysize = image.ysize;
    mfclose(inf);
}

/*
 *      sizeofimage3 -
 *              return xsize, ysize and zsize of an iris image file.
 *
 */
sizeofimage3(char *name, int *xsize, int *ysize, int *zsize)
{
    IMAGE image;
    MFILE *inf;

    inf = mfopen(name,"r");
    if(!inf) {
        fprintf(stderr,"sizeofimage3: can't open image file %s\n",name);
        exit(1);
    }
    readheader(inf,&image);
    if(image.imagic != IMAGIC) {
        fprintf(stderr,"sizeofimage3: bad magic number in image file\n");
    	exit(1);
    }
    *xsize = image.xsize;
    *ysize = image.ysize;
    *zsize = image.zsize;
    mfclose(inf);
}

/*
 *	longimagedata - 
 *		read in a B/W RGB or RGBA iris image file and return a 
 *	pointer to an array of longs.
 *
 */
unsigned long *longimagedata(char *name)
{
    unsigned long *base, *lptr;
    unsigned char *rledat, *verdat;
    long *starttab, *lengthtab;
    MFILE *inf;
    IMAGE *image;
    int y, z, pos, len, tablen;
    int xsize, ysize, zsize;
    int bpp, rle, cur, badorder;
    int rlebuflen;

    inf = mfopen(name,"r");
    if(!inf) {
	fprintf(stderr,"longimagedata: can't open image file %s\n",name);
	exit(1);
    }
    image = (IMAGE *)mymalloc(sizeof(IMAGE));
    readheader(inf,image);
    if(image->imagic != IMAGIC) {
	fprintf(stderr,"longimagedata: bad magic number in image file %s\n",name);
	exit(1);
    }
    rle = ISRLE(image->type);
    bpp = BPP(image->type);
    if(bpp != 1 ) {
	fprintf(stderr,"longimagedata: image must have 1 byte per pix chan\n");
	exit(1);
    }
    xsize = image->xsize;
    ysize = image->ysize;
    zsize = image->zsize;
    if(rle) {
	tablen = ysize*zsize*sizeof(long);
	starttab = (long *)mymalloc(tablen);
	lengthtab = (long *)mymalloc(tablen);
	rlebuflen = 1.05*xsize+10;
	mfseek(inf,512);
 	readtab(inf,(unsigned long *)starttab,tablen);
	readtab(inf,(unsigned long *)lengthtab,tablen);

/* check data order */
	cur = 0;
	badorder = 0;
	for(y=0; y<ysize; y++) {
	    for(z=0; z<zsize; z++) {
		if(starttab[y+z*ysize]<cur) {
		    badorder = 1;
		    break;
		}
		cur = starttab[y+z*ysize];
	    }
	    if(badorder) 
		break;
	}

	mfseek(inf,512+2*tablen);
	cur = 512+2*tablen;
	base = (unsigned long *)
		mymalloc((xsize*ysize+TAGLEN)*sizeof(long));
	addlongimgtag(base,xsize,ysize);
  	if(badorder) {
	    for(z=0; z<zsize; z++) {
		lptr = base;
		for(y=0; y<ysize; y++) {
		    if(cur != starttab[y+z*ysize]) {
			mfseek(inf,starttab[y+z*ysize]);
			cur = starttab[y+z*ysize];
		    }
		    if(lengthtab[y+z*ysize]>rlebuflen) {
			fprintf(stderr,"longimagedata: rlebuf is too small - bad poop\n");
			exit(1);
		    }
		    rledat = mfread(inf,lengthtab[y+z*ysize]);
		    cur += lengthtab[y+z*ysize];
		    expandrow(lptr,rledat,3-z);
		    lptr += xsize;
		}
	    }
	} else {
	    lptr = base;
	    for(y=0; y<ysize; y++) {
		for(z=0; z<zsize; z++) {
		    if(cur != starttab[y+z*ysize]) {
			mfseek(inf,starttab[y+z*ysize]);
			cur = starttab[y+z*ysize];
		    }
		    rledat = mfread(inf,lengthtab[y+z*ysize]);
		    cur += lengthtab[y+z*ysize];
		    expandrow(lptr,rledat,3-z);
		}
		lptr += xsize;
	    }
    	}
	if(zsize == 3) 
	    setalpha(base,xsize*ysize);
	else if(zsize<3) 
	    copybw(base,xsize*ysize);
	mfclose(inf);
	free(starttab);
	free(lengthtab);
	free(image);
	return base;
    } else {
	base = (unsigned long *)
		mymalloc((xsize*ysize+TAGLEN)*sizeof(long));
	addlongimgtag(base,xsize,ysize);
	mfseek(inf,512);
	for(z=0; z<zsize; z++) {
	    lptr = base;
	    for(y=0; y<ysize; y++) {
		verdat = mfread(inf,xsize);
		interleaverow(lptr,verdat,3-z,xsize);
		lptr += xsize;
	    }
	}
	if(zsize == 3) 
	    setalpha(base,xsize*ysize);
	else if(zsize<3) 
	    copybw(base,xsize*ysize);
	mfclose(inf);
	free(image);
	return base;
    }
}

/* static utility functions for longimagedata */

static interleaverow(unsigned char *lptr, int *cptr, int z, int n)
{
    lptr += z;
    while(n--) {
	*lptr = *cptr++;
	lptr += 4;
    }
}

static copybw(long *lptr, int n)
{
    while(n>=8) {
	lptr[0] = 0xff000000+(0x010101*(lptr[0]&0xff));
	lptr[1] = 0xff000000+(0x010101*(lptr[1]&0xff));
	lptr[2] = 0xff000000+(0x010101*(lptr[2]&0xff));
	lptr[3] = 0xff000000+(0x010101*(lptr[3]&0xff));
	lptr[4] = 0xff000000+(0x010101*(lptr[4]&0xff));
	lptr[5] = 0xff000000+(0x010101*(lptr[5]&0xff));
	lptr[6] = 0xff000000+(0x010101*(lptr[6]&0xff));
	lptr[7] = 0xff000000+(0x010101*(lptr[7]&0xff));
	lptr += 8;
	n-=8;
    }
    while(n--) {
	*lptr = 0xff000000+(0x010101*(*lptr&0xff));
	lptr++;
    }
}

static setalpha(unsigned char *lptr, int n)
{
    while(n>=8) {
	lptr[0*4] = 0xff;
	lptr[1*4] = 0xff;
	lptr[2*4] = 0xff;
	lptr[3*4] = 0xff;
	lptr[4*4] = 0xff;
	lptr[5*4] = 0xff;
	lptr[6*4] = 0xff;
	lptr[7*4] = 0xff;
	lptr += 4*8;
	n -= 8;
    }
    while(n--) {
	*lptr = 0xff;
	lptr += 4;
    }
}

static expandrow(unsigned char *optr, unsigned char *iptr, int z)
{
    unsigned char pixel, count;

    optr += z;
    while(1) {
	pixel = *iptr++;
	if ( !(count = (pixel & 0x7f)) )
	    return;
	if(pixel & 0x80) {
	    while(count>=8) {
		optr[0*4] = iptr[0];
		optr[1*4] = iptr[1];
		optr[2*4] = iptr[2];
		optr[3*4] = iptr[3];
		optr[4*4] = iptr[4];
		optr[5*4] = iptr[5];
		optr[6*4] = iptr[6];
		optr[7*4] = iptr[7];
		optr += 8*4;
		iptr += 8;
		count -= 8;
	    }
	    while(count--) {
		*optr = *iptr++;
		optr+=4;
	    }
	} else {
	    pixel = *iptr++;
	    while(count>=8) {
		optr[0*4] = pixel;
		optr[1*4] = pixel;
		optr[2*4] = pixel;
		optr[3*4] = pixel;
		optr[4*4] = pixel;
		optr[5*4] = pixel;
		optr[6*4] = pixel;
		optr[7*4] = pixel;
		optr += 8*4;
		count -= 8;
	    }
	    while(count--) {
		*optr = pixel;
		optr+=4;
	    }
	}
    }
}

/*
 *	longstoimage -
 *		copy an array of longs to an iris image file.  Each long
 *	represents one pixel.  xsize and ysize specify the dimensions of
 *	the pixel array.  zsize specifies what kind of image file to
 *	write out.  if zsize is 1, the luminance of the pixels are
 *	calculated, and a sinlge channel black and white image is saved.
 *	If zsize is 3, an RGB image file is saved.  If zsize is 4, an
 *	RGBA image file is saved.
 *
 */
int longstoimage(unsigned long *lptr, 
		 int xsize, 
		 int ysize, 
		 int zsize, 
		 char *name)
{
    FILE *outf;
    IMAGE *image;
    int tablen, y, z, pos, len;
    long *starttab, *lengthtab;
    unsigned char *rlebuf;
    unsigned long *lumbuf;
    int rlebuflen, goodwrite;

    goodwrite = 1;
    outf = fopen(name,"w");
    if(!outf) {
	fprintf(stderr,"longstoimage: can't open output file\n");
	return 0;
    }
    tablen = ysize*zsize*sizeof(long);

    image = (IMAGE *)mymalloc(sizeof(IMAGE));
    starttab = (long *)mymalloc(tablen);
    lengthtab = (long *)mymalloc(tablen);
    rlebuflen = 1.05*xsize+10;
    rlebuf = (unsigned char *)mymalloc(rlebuflen);
    lumbuf = (unsigned long *)mymalloc(xsize*sizeof(long));

    bzero(image,sizeof(IMAGE));
    image->imagic = IMAGIC; 
    image->type = RLE(1);
    if(zsize>1)
	image->dim = 3;
    else
	image->dim = 2;
    image->xsize = xsize;
    image->ysize = ysize;
    image->zsize = zsize;
    image->min = 0;
    image->max = 255;
    goodwrite *= writeheader(outf,image);
    fseek(outf,512+2*tablen,SEEK_SET);
    pos = 512+2*tablen;
    for(y=0; y<ysize; y++) {
	for(z=0; z<zsize; z++) {
	    if(zsize == 1) {
		lumrow(lptr,lumbuf,xsize);
		len = compressrow(lumbuf,rlebuf,CHANOFFSET(z),xsize);
	    } else {
		len = compressrow(lptr,rlebuf,CHANOFFSET(z),xsize);
	    }
	    if(len>rlebuflen) {
		fprintf(stderr,"longstoimage: rlebuf is too small - bad poop\n");
		exit(1);
	    }
	    goodwrite *= fwrite(rlebuf,len,1,outf);
	    starttab[y+z*ysize] = pos;
	    lengthtab[y+z*ysize] = len;
	    pos += len;
	}
	lptr += xsize;
    }

    fseek(outf,512,SEEK_SET);
    goodwrite *= writetab(outf,(unsigned long *)starttab,tablen);
    goodwrite *= writetab(outf,(unsigned long *)lengthtab,tablen);
    free(image);
    free(starttab);
    free(lengthtab);
    free(rlebuf);
    free(lumbuf);
    fclose(outf);
    if(goodwrite)
	return 1;
    else {
	fprintf(stderr,"longstoimage: not enough space for image!!\n");
	return 0;
    }
}

/* static utility functions for longstoimage */

static lumrow(unsigned char *rgbptr, unsigned char *lumptr, int n) 
{
    lumptr += CHANOFFSET(0);
    while(n--) {
	*lumptr = ILUM(rgbptr[OFFSET_R],rgbptr[OFFSET_G],rgbptr[OFFSET_B]);
	lumptr += 4;
	rgbptr += 4;
    }
}

static int compressrow(unsigned char *lbuf, 
                       unsigned char *rlebuf, 
		       int z, 
		       int cnt)
{
    unsigned char *iptr, *ibufend, *sptr, *optr;
    short todo, cc;							
    long count;

    lbuf += z;
    iptr = lbuf;
    ibufend = iptr+cnt*4;
    optr = rlebuf;

    while(iptr<ibufend) {
	sptr = iptr;
	iptr += 8;
	while((iptr<ibufend)&& ((iptr[-8]!=iptr[-4])||(iptr[-4]!=iptr[0])))
	    iptr+=4;
	iptr -= 8;
	count = (iptr-sptr)/4;
	while(count) {
	    todo = count>126 ? 126:count;
	    count -= todo;
	    *optr++ = 0x80|todo;
	    while(todo>8) {
		optr[0] = sptr[0*4];
		optr[1] = sptr[1*4];
		optr[2] = sptr[2*4];
		optr[3] = sptr[3*4];
		optr[4] = sptr[4*4];
		optr[5] = sptr[5*4];
		optr[6] = sptr[6*4];
		optr[7] = sptr[7*4];
		optr += 8;
		sptr += 8*4;
		todo -= 8;
	    }
	    while(todo--) {
		*optr++ = *sptr;
		sptr += 4;
	    }
	}
	sptr = iptr;
	cc = *iptr;
	iptr += 4;
	while( (iptr<ibufend) && (*iptr == cc) )
	    iptr += 4;
	count = (iptr-sptr)/4;
	while(count) {
	    todo = count>126 ? 126:count;
	    count -= todo;
	    *optr++ = todo;
	    *optr++ = cc;
	}
    }
    *optr++ = 0;
    return optr - (unsigned char *)rlebuf;
}

#ifdef TESTMAIN

#define NTIMES	(1)
/*
 *	this reads in an image and writes it out, and then
 *	uses ipaste to display the converted image file.
 *
 */
main(argc,argv)
int argc;
char **argv;
{
    int xsize, ysize;
    unsigned long *lbuf;
int i;

    if(argc<2) {
	fprintf(stderr,"usage: fastimg: inimage.rgb\n");
	exit(1);
    }
    sizeofimage(argv[1],&xsize,&ysize);

    lbuf = 0;
system("date");
printf("NEW\n");

    for(i=0; i<NTIMES; i++) {
     	if(lbuf)
	    free(lbuf);
	lbuf = mylongimagedata(argv[1]);
    }
printf("DONE\n");
system("date");
    longstoimage(lbuf,xsize,ysize,3,"/usr/tmp/a.rgb");
system("date");
printf("OLD\n");
    for(i=0; i<NTIMES; i++) {
     	if(lbuf)
	    free(lbuf);
	lbuf = (unsigned long*)longimagedata(argv[1]);
    }
printf("DONE\n");
system("date");
    longstoimage(lbuf,xsize,ysize,3,"/usr/tmp/t.rgb");
    system("ipaste /usr/tmp/t.rgb");
    system("ipaste /usr/tmp/a.rgb");
}
#endif
