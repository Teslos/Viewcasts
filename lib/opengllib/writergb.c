/*
 *	frombin -
 *		Convert a binary RGB dump image to an Iris image.
 *
 *				Paul Haeberli - 1986
 */
#include "image.h"
#include "memory.h"
#include "lum.h"
#define OFFSET_R	3	/* this is byte order dependent */
#define OFFSET_G	2
#define OFFSET_B	1
#define OFFSET_A	0
#define CHANOFFSET(z)	(3-(z))	/* this is byte order dependent */

void uninterleave(char *cptr,short *sptr,int n,int nslices,int slice);
static int compressrow();
static lumrow();
static int maxmalloc, mnoise;

void mallocnoise(int n)
{
    mnoise = n;
}

void malloclimit(int n)
{
    maxmalloc = n;
}

void myfree(char *ptr)
{
    free(ptr);
}

unsigned char *mymalloc(int n)
{
    unsigned char *cptr;

    if(maxmalloc && n>maxmalloc) {
     fprintf(stderr,"attemt to malloc %d bytes max is %d\n",n,maxmalloc);
     exit(1);
    }
    cptr = (unsigned char *)malloc(n);
    if(mnoise)
    fprintf(stderr,"malloc of %d bytes\n",n);
    if(!cptr) {
     fprintf(stderr,"malloc can't get %d bytes\n",n);
     exit(1);
    }
    return cptr;
}
static unsigned char *position = NULL;
static unsigned char *memread(char *buf, int n)
{
  unsigned char *ret;
  if (position == NULL)
    position = buf;
  ret = position;
  position += n;
  return ret;
}

static unsigned long getlong(char *image)
{
    unsigned char *buf;

    buf = memread(image,4);
    return (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+(buf[3]<<0);
}

void rgbtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *l, int n)
{
  while(n--)
    {
      r[0] = l[0];
      g[0] = l[1];
      b[0] = l[2];
      l +=3; r++; g++; b++;
    }
}
	

void rgbtocpack(unsigned char *r,unsigned char *g,unsigned char *b,unsigned long *l,int n)
{
    while(n>=8) {
    l[0] = r[0] | (g[0]<<8) | (b[0]<<16) | (0xff<<24);
    l[1] = r[1] | (g[1]<<8) | (b[1]<<16) | (0xff<<24);
    l[2] = r[2] | (g[2]<<8) | (b[2]<<16) | (0xff<<24);
    l[3] = r[3] | (g[3]<<8) | (b[3]<<16) | (0xff<<24);
    l[4] = r[4] | (g[4]<<8) | (b[4]<<16) | (0xff<<24);
    l[5] = r[5] | (g[5]<<8) | (b[5]<<16) | (0xff<<24);
    l[6] = r[6] | (g[6]<<8) | (b[6]<<16) | (0xff<<24);
    l[7] = r[7] | (g[7]<<8) | (b[7]<<16) | (0xff<<24);
    l += 8;
    r += 8;
    g += 8;
    b += 8;
    n -= 8;
    }
    while(n--)
        *l++ = *r++ | ((*g++)<<8) | ((*b++)<<16) | (0xff<<24);
}

static unsigned long *translong(unsigned *buf, int n)
{
  unsigned char  b1, b2, b3, b4;
  unsigned char *ptr;

  ptr = (unsigned char *)buf;
  while(n--)
    {
      b1 = *ptr++;
      b2 = *ptr++;
      b3 = *ptr++;
      b4 = *ptr++;
      *buf++ = (b1<<24)|(b2<<16)|(b3<<8)|(b4);
    }
  return (unsigned long *)ptr;
}

void writergb(char *name,int xsizeimg,int ysizeimg,int zsizeimg,char *buf)
{
  IMAGE *image;
  int y, z;
  FILE *inf;
  int interleave=1;
  short *sbuf;
  unsigned char *rbuf, *gbuf, *bbuf;
  unsigned long *lptr;

/*   sbuf = (short *) ALLOC((long)(xsizeimg * sizeof(sbuf[0]))); */
/*   cbuf = (char *) ALLOC(sizeof(char) * xsizeimg * zsizeimg); */
/*   if(sbuf==NULL || cbuf==NULL) */
/* 	printf("Error: not enough memory!\n"); */
   
/*   image = iopen(name,"w",RLE(1),3,xsizeimg,ysizeimg,zsizeimg); */
/*   if(interleave) { */
/*     for(y=0; y<ysizeimg; y++) { */
/*       for(z=0; z<zsizeimg; z++) { */
/* 	uninterleave(buf,sbuf,xsizeimg,zsizeimg,z); */
/* 	putrow(image,sbuf,y,z); */
/*       } */
/*     } */
/*   } else { */
/*     for(z=0; z<zsizeimg; z++) { */
/*       for(y=0; y<ysizeimg; y++) { */
/* 	if(sizeof(cbuf)==xsizeimg) ; */
/* 	  //ctos(cbuf,sbuf,xsize); */
/* 	putrow(image,sbuf,y,z); */
/*       } */
/*     } */
/*   } */
/*   FREE(cbuf); */
/*   FREE(sbuf); */
/*   iclose(image); */
  rbuf = (unsigned char *) mymalloc(sizeof(unsigned char) * xsizeimg * ysizeimg);
  gbuf = (unsigned char *) mymalloc(sizeof(unsigned char) * xsizeimg * ysizeimg);
  bbuf = (unsigned char *) mymalloc(sizeof(unsigned char) * xsizeimg * ysizeimg);
  lptr = (unsigned long *) mymalloc(sizeof(unsigned long) * xsizeimg * ysizeimg);
  
  rgbtorgb(rbuf,gbuf,bbuf,buf,xsizeimg*ysizeimg);
  rgbtocpack(rbuf,gbuf,bbuf,lptr,xsizeimg*ysizeimg);
    
  longstoimage(lptr, xsizeimg, ysizeimg, zsizeimg, name); 
  myfree(rbuf);
  myfree(gbuf);
  myfree(bbuf);
  myfree(lptr);
}

void uninterleave(char *cptr,short *sptr,int n,int nslices,int slice)
{
    cptr += slice;
    while(n--) {
	*sptr++ = *cptr;
	cptr += nslices;
    }
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
