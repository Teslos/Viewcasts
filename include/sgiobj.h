 /**************************************************************************
  *                                                                        *
  * Copyright (c) 1996-98 Silicon Graphics, Inc.                           *
  *                                                                        *
  * Permission to use, copy, modify, distribute, and sell this software    *
  * and its documentation for any purpose is hereby granted without fee,   *
  * provided that the name of Silicon Graphics may not be used in any      *
  * advertising or publicity relating to the software without the          *
  * specific, prior written permission of Silicon Graphics.                *
  *                                                                        *
  * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,     *
  * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY       *
  * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.       *
  *                                                                        *
  * IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR ANY SPECIAL,          *
  * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY      *
  * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,        *
  * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY *
  * OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          *
  * PERFORMANCE OF THIS SOFTWARE.                                          *
  *                                                                        *
  **************************************************************************/

/*
	Author : Paul Haeberli
             paul@asd.sgi.com
*/

#ifndef SGIOBJDEF
#define SGIOBJDEF

#define SOMAGIC		0x5424

/* types of objects */
#define OBJ_QUADLIST	1
#define OBJ_TRILIST	2
#define OBJ_TRIMESH	3
#define OBJ_END		4

/* offsets to data */
#define OFFSET_NORMAL	0
#define OFFSET_UVS	3
#define OFFSET_COLOR	3
#define OFFSET_POINT	6
#define PNTLONGS	9

/* how to draw an object */
#define DRAW_POINTS	0
#define DRAW_NORMALS	1
#define DRAW_UVS	2
#define DRAW_COLORS	4
#define DRAW_LINES	8

/* tmesh opcodes */

#define OP_BGNTMESH	1
#define OP_SWAPTMESH	2
#define OP_ENDBGNTMESH	3
#define OP_ENDTMESH	4

typedef struct sgiobj {
    struct sgiobj *next;
    long objtype;
    long nlongs;
    long *data;
    long xnlongs;
    long *xdata;
} sgiobj;

sgiobj *readsgiobj(char* );
sgiobj *clonesgiobj();
sgiobj *newtriobj(int);
sgiobj *newquadobj(int);
sgiobj *catsgiobj();
sgiobj *tmeshobj(sgiobj*);
void writesgiobj(char*, sgiobj*);

/*
 * Copyright (C) 1992, Silicon Graphics, Inc.
 * All Rights Reserved.
 */
typedef struct attribs {
	int r, g, b;
	float nx, ny, nz;
} attribs;

typedef struct point {
	struct point 	*next;
	int 		type;
	attribs		*attr;
	float x, y, z;
} point;

typedef struct object {
   	struct object 	*next;
	int 		type;
	attribs		*attr;
	point 		*points;
} object;

#define FASTMAGIC	0x5423

typedef struct fastobj {
    int npoints;
    int colors;
    int type;
    int material;
    int display;
    int *data;
} fastobj;

fastobj *readfastobj();
fastobj *clonefastobj();

#endif
