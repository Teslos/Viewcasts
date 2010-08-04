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
 *	file_logl -
 *		Miscellaneous support for files.
 *
 *				Paul Haeberli - 1985
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "image.h"
#include "resource.h"

int sizeoffile(FILE *f)
{
    int pos, ret;

    pos = ftell(f);
    if ((ret = fseek(f,0,2)) < 0) {
	fprintf(stderr,"sizeoffile: seek error\n");
	exit(1);
    }
    ret = ftell(f);
    if (fseek(f,pos,0) < 0) {
	fprintf(stderr,"sizeoffile: seek error\n");
	exit(1);
    }
    return ret;
}

unsigned char *charfiledata(FILE *f)
{
    unsigned char *dat;
    int nbytes;

    nbytes = sizeoffile(f);
    if (fseek(f,0,0) < 0) {
	fprintf(stderr,"charfiledata: seek error\n");
	exit(1);
    }
    dat = (unsigned char *)mymalloc(nbytes);
    fread(dat,1,nbytes,f);
    if (fseek(f,0,0) < 0) {
	fprintf(stderr,"charfiledata: seek error\n");
	exit(1);
    }
    return dat;
}

int fcopy(FILE *inf,FILE *outf)
{
    char tmpbuf[10240];
    int nbytes, totbytes;

    totbytes = 0;
    while(1) {
	nbytes = fread(tmpbuf,1,10240,inf);
	if(nbytes<=0) 
	    break;
	fwrite(tmpbuf,1,nbytes,outf);
	totbytes += nbytes;
    }
    return totbytes;
}

char *basename(char *str)
{
    char *cptr;

    cptr = str;
    while(*cptr)
	cptr++;
    while(cptr != str) {
	cptr--;
	if(*cptr == '/')
	    return (cptr+1);
    }
    return str;
}

int readfile(FILE *inf, char *buf)
{
    int nbytes;
    int count;

    count = 0;
    while(1) {
	nbytes = fread(buf,1,10240,inf);
	if(nbytes<=0) 
	    break;
	buf += nbytes;
	count += nbytes;
    }
    *buf = 0;
    return count;
}

int isfile(char *name)
{
    if(access(name,04) == 0)
	return 1;
    else
	return 0;
}

static goodfile(char *name,int exec)
{
    FILE *inf;
    short magic;

    inf = fopen(name,"r");
    if(!inf)
	return 0;
    if(exec) {
	magic = 0;
	fread(&magic,2,1,inf);
	fclose(inf);
	if((magic == 0x0160) || (magic == 0x7f45))
	    return 1;
	else
	    return 0;
    } else {
	fclose(inf);
	return 1;
    }
}

static void locatefile(char *name,char *expname,char *domain,int exec)
{
    char *path;
    char *start, *finish;
    char save;
    char template[256];

    if(name[0] == '/' || name[0] == '.') {
	strcpy(expname,name);
	return;
    } 
    if(goodfile(name,exec)) {
	strcpy(expname,name);
	return;
    }
    path = (char *)getenv(domain);
    if(!path) {
	strcpy(expname,name);
	return;
    }
    start = path;
    finish = path;
    while(1) {
	if(*path == 0 || *path == ':') {
	    if(start != finish ) {
		save = *finish;
		*finish = 0;
		strcpy(template,start);
		*finish = save;
		strcat(template,"/");
		strcat(template,name);
		if(goodfile(template,exec)) {
		    strcpy(expname,template);
		    return;
		}
	    }
	    if(*path == 0) {
		strcpy(expname,name);
		return;
	    }
	    path++;
	    start = path;
	    finish = path;
	} else {
	    path++;
	    finish = path;
	}
    }
}

findname(char *name,char *expname,char *domain)
{
    locatefile(name,expname,domain,0);
}

findexec(char *name,char *expname,char *domain)
{
    locatefile(name,expname,domain,1);
}

FILE *gfxopen(char *name,char *mode)
{
    char expname[256];

    findname(name,expname,"GFXPATH");
    return fopen(expname,mode);
}

findfile(char *name)
{
    char expname[256];

    findname(name,expname,"GFXPATH");
    return isfile(expname);
}

int isimagefile(char *name)
{
    FILE *inf;
    unsigned short magic;

    inf = fopen(name,"r");
    if(!inf)
	return 0;
    magic = 0;
    fread(&magic,1,sizeof(short),inf);
    fclose(inf);
    if(magic == IMAGIC)
	return 1;
    magic = (magic>>8)&0xff + (magic<<8)&0xff00;
    if(magic == IMAGIC)
	return 1;
    else
	return 0;
}

/*
 *	resource reading stuff follows
 *
 *
 */
#define BACKMAGIC	(('P'<<24)+('A'<<16)+('U'<<8)+('L'<<8))
#define RESSIZE	 	(4+4+32)

extern int __Argc;
extern char **__Argv;

static char nametab[40][32];
static int offsettab[40];
static int lengthtab[40];
static int nres, curbase, curlength;
static FILE *resf;

static int readrestab()
{
    int i, s;
    long magic;

    fseek(resf,-(4+3*4+4+4+32),SEEK_END);
    for(i=0; i<3; i++) {
	s = fseek(resf,-(4+i*4),SEEK_END);
	magic = 0;
	s = fread(&magic,sizeof(long),1,resf);
	if(magic != BACKMAGIC) 
	    return 0;
    }
    fseek(resf,-(4+3*4),SEEK_END);
    fread(&nres,sizeof(long),1,resf);
    for(i=0; i<nres; i++) {
	fseek(resf,-(4+3*4+4)-i*RESSIZE,SEEK_END);
	fread(&lengthtab[i],sizeof(long),1,resf);
	fseek(resf,-(4+3*4+4+4)-i*RESSIZE,SEEK_END);
	fread(&offsettab[i],sizeof(long),1,resf);
	fseek(resf,-(4+3*4+4+4+32)-i*RESSIZE,SEEK_END);
	fread(nametab[i],32,1,resf);
    }
    return 1;
}

static findres(char *name)
{
    int i;

    for(i=0; i<nres; i++) {
	if(strcmp(nametab[i],name) == 0) {
	    fseek(resf,offsettab[i],SEEK_SET);
	    curbase = offsettab[i];
	    curlength = lengthtab[i];
   	    return 1;
	}
    }
    return 0;
}

FILE *res_fopen(char *name,char *mode)
{
    char fname[512];

    if(name[0] == '#') {
	if(!resf) {
	    findexec(__Argv[0],fname,"PATH");
	    resf = fopen(fname,"r");
	    if(!resf) {
		fprintf(stderr,"res_fopen: can't open input file %s\n",__Argv[0]);
		exit(1);
	    }
	    if(!readrestab())
		return 0;
	}
	if(findres(name+1))
	    return resf;
	fprintf(stderr,"res_fopen: can't find resource %s\n",name+1);
	exit(1);
    } else {
	curbase = 0;
	return fopen(name,mode);
    }
}

char *res_fgets(char *buf,int size,FILE *inf)
{
    return fgets(buf,size,inf);
}

int res_fclose(FILE *inf)
{
    if(!curbase)
	return fclose(inf);
    else
	return 0;
}

int res_fseek(FILE *f,int offset,int whence)
{
    return fseek(f,curbase+offset,whence);
}

int res_ftell(FILE *f)
{
    return ftell(f)-curbase;
}
