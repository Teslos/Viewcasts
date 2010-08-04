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
 *	misc_logl - 
 *		Miscellaneous stuff that does not rely on the gl.
 *
 *			    Paul Haeberli - 1988
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
#include "chunkarena.h"

strtolower(str)
char *str;
{
    while(*str) {
	*str = tolower(*str);
	str++;
    }
}

ctos(cptr,sptr,n)
unsigned char *cptr;
unsigned short *sptr;
int n;
{
    while(n--) {
	if(n>=8) {
	    sptr[0] = cptr[0];
	    sptr[1] = cptr[1];
	    sptr[2] = cptr[2];
	    sptr[3] = cptr[3];
	    sptr[4] = cptr[4];
	    sptr[5] = cptr[5];
	    sptr[6] = cptr[6];
	    sptr[7] = cptr[7];
	    sptr+=8; 
	    cptr+=8;
	    n -= 7;
	} else {
	    *sptr++ = *cptr++;
	}
    }
}

stoc(sptr,cptr,n)
unsigned short *sptr;
unsigned char *cptr;
int n;
{
    while(n--) {
	if(n>=8) {
	    cptr[0] = sptr[0];
	    cptr[1] = sptr[1];
	    cptr[2] = sptr[2];
	    cptr[3] = sptr[3];
	    cptr[4] = sptr[4];
	    cptr[5] = sptr[5];
	    cptr[6] = sptr[6];
	    cptr[7] = sptr[7];
	    sptr+=8; 
	    cptr+=8;
	    n -= 7;
	} else {
	    *cptr++ = *sptr++;
	}
    }
}

delay(secs) 
float secs;
{
    int ticks;

    if(secs>=0) {
        ticks = (secs*HZ)+0.5;
	sginap(ticks);
    }
}

unsigned long getltime()
{
    struct tms ct;

    times(&ct);
#ifdef ONSUN
    return ct.tms_utime;
#else
    return times(&ct);
#endif
}

unsigned long waittill(t)
unsigned long t;
{
    unsigned long curt;

    while(1) {
	curt = getltime();
	if(curt>t)
	    return curt;
    }
}

static long stime;
static float inittime; 
static int firsted;

float uptime()
{
    if(stime == 0)
	stime = getltime();
    return (getltime()-stime)/((float)HZ);
}

cleartime()
{
    stime = 0;
    inittime = uptime();
    firsted = 1;
}

float gettime()
{
    if(!firsted) {
	cleartime();
	firsted = 1;
    }
    return uptime()-inittime;
}

timefunc(f,n)
int (*f)();
int n;
{
    float t;
    int i;

    cleartime();
    for(i=0; i<n; i++)
	(f)();
    t = gettime();
    fprintf(stderr,"timefunc: %f seconds per call\n",t/n);
}

/*
 *	tpercent - 
 *		Make a row of dots that show percent done
 *
 */
static int started = 0;
static int pos;
static FILE *outf;

#define NDOTS	66

tpercentfile(f)
FILE *f;
{
    outf = f;
}

tpercentdone(p)
float p;
{
    int newpos;
    FILE *outfile;

    if(!outf)
	outfile = stderr;
    else
	outfile = outf;
    p = p/100.0;
    if(!started && p <= 0.01) {
	fprintf(outfile,"working: [");
	fflush(outfile);
	started = 1;
	pos = 0;
	return;
    }
    if(started) {
	if(p<0.999) 
	    newpos = NDOTS*p;
	else
	    newpos = NDOTS;
        if(newpos>pos) {
	    while(pos<newpos) {
	        fprintf(outfile,".");
		pos++;
	    }
	    fflush(outfile);
        }
	if(p>0.999) {
	    fprintf(outfile,"]\n");
	    fflush(outfile);
	    started = 0;
	}
    }
}

/*
 *	mymalloc follows
 *
 */
static int mnoise, maxmalloc;

mallocnoise(n)
int n;
{
    mnoise = n;
}

malloclimit(n)
int n;
{
    maxmalloc = n;
}

unsigned char *mymalloc(n)
int n;
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

myfree(ptr)
char *ptr;
{
    free(ptr);
}

/*
 *	buf alloc follows
 *
 */
unsigned char *bufalloc(ptr,nbytes)
unsigned char *ptr;
int nbytes;
{
    unsigned long *lptr;
    
    if(ptr) {
	lptr = (unsigned long *)ptr;
	if(lptr[-1] >= nbytes)
	    return (unsigned char *)lptr;
	else 
	    myfree(lptr-1);
    }
    lptr = (unsigned long *)mymalloc(nbytes+sizeof(long));
    *lptr++ = nbytes;
    return (unsigned char *)lptr;
}

buffree(ptr)
unsigned char *ptr;
{
    unsigned long *lptr;

    if(ptr) {
	lptr = (unsigned long *)ptr;
	myfree(lptr-1);
    }
}

/*
 *	chunk alloc follows
 *
 */
#define NINCHUNK	(100)

chunkarena *newchunkalloc(chunksize)
int chunksize;
{
    chunkarena *ca;

    ca = (chunkarena *)mymalloc(sizeof(chunkarena));
    ca->nlongs = (chunksize+3)>>2;
    ca->flist = 0;
    return ca;
}

void chunkfree(ca,p)
chunkarena *ca;
long *p;
{
    *p = (long)ca->flist;
    ca->flist = (long *)p;
}

long *chunkalloc(ca)
chunkarena *ca;
{
    long *ret; 
    int i;

    if(!ca->flist) {
	ret = (long *)mymalloc(ca->nlongs*NINCHUNK*sizeof(long));
	for(i=0; i<NINCHUNK; i++) {
	    chunkfree(ca,ret);
	    ret += ca->nlongs;
   	}
    }
    ret = ca->flist;
    ca->flist = (long *)(*ret);
    return ret;
}
