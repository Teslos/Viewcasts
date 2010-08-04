/*
 * Copyright 1991-94 Silicon Graphics, Inc.
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
 *	tomesh -
 *		convert an array of triangles to tmesh format.
 *
 *				Paul Haeberli - 1990
 */
#include "stdio.h"
#include "sgiobj.h"
#include "mesh.h"


/*
 *	tmeshobj
 *
 *
 */
static sgiobj *outobj;
static long *outptr;
static int np;
static int vcount;
static int lastcmd;
static long *countpos;

static long hashlongs(buf,n)
long *buf;
{
    long hash;

    hash = 0;
    while(n--)
	hash = (273*hash)+(*buf++);
    return hash&0x7fffffff;
}

/* 
 *	output stuff follows
 *
 */
void out_ambegin(nverts,ntris)
int nverts, ntris;
{
    int maxlongs;

    maxlongs = 2+(nverts*PNTLONGS)+(3*np*5)+1;
    outobj = (sgiobj *)newtmeshobj(maxlongs);
    outptr = outobj->data;
    *outptr++ = PNTLONGS*nverts;
    vcount = 0;
    lastcmd = 0;
}

void out_amend()
{
    outobj->nlongs = outptr-outobj->data;
}

int out_amhashvert(v)
long *v;
{
    return hashlongs(v,9);
}

int out_amvertsame(v1,v2)
float *v1, *v2;
{
    if(v1[0] != v2[0]) return 0;
    if(v1[1] != v2[1]) return 0;
    if(v1[2] != v2[2]) return 0;

    if(v1[3] != v2[3]) return 0;
    if(v1[4] != v2[4]) return 0;
    if(v1[5] != v2[5]) return 0;

    if(v1[6] != v2[6]) return 0;
    if(v1[7] != v2[7]) return 0;
    if(v1[8] != v2[8]) return 0;

    return 1;
}

void out_amvertdata(fptr)
float *fptr;
{
    bcopy(fptr,outptr,9*sizeof(long));
    outptr += PNTLONGS;
}

static newcommand(op)
long op;
{
    if(op == OP_BGNTMESH && lastcmd == OP_ENDTMESH && vcount == 0) {
	outptr[-2] = OP_ENDBGNTMESH;
	return;
    }
    if(vcount>0) {
	*countpos = vcount;
	vcount = 0;
    }
    lastcmd = op;
    *outptr++ = op;
    countpos = outptr;
    outptr++;
}

void out_ambgntmesh() 
{
    newcommand(OP_BGNTMESH);
}

void out_amendtmesh() 
{
    newcommand(OP_ENDTMESH);
}

void out_amswaptmesh() 
{
    newcommand(OP_SWAPTMESH);
}

void out_amvert(index)
{
    *outptr++ = PNTLONGS*index*sizeof(long);
    vcount++;
}

sgiobj *tmeshobj(obj)
sgiobj *obj;
{
    int poly, i;
    float *fptr;
    Meshobj *mc; 

    mc = newMeshobj(
		out_ambegin,
		out_amend,
		out_amhashvert,
		out_amvertsame,
		out_amvertdata,
		out_ambgntmesh,
		out_amendtmesh,
		out_amswaptmesh,
		out_amvert);
    in_ambegin(mc);
    np = (obj->nlongs/PNTLONGS)/3;
    fptr = (float*)obj->data;
    for(poly=0; poly<np; poly++) {
	in_amnewtri(mc);
	for(i=0; i<3; i++) {
	    in_amvert(mc,(long)fptr);
	    fptr += PNTLONGS;
	}
    }
    in_amend(mc);
    freeMeshobj(mc);
    return outobj;
}

