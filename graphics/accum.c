/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*  accum.c
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include "grafik.h"
#include "mem.h"
#include "errs.h"

#define ACSIZE  16  /* default value of accumulation buffer */

/* functions */
static void loaddxdy(int accsize);

GLfloat *dx, *dy;

GLfloat jitter2[2][2] = {
	{0.25, 0.75}, {0.75, 0.25}
};

GLfloat jitter3[3][2] = {
    {0.5, 0.5}, 
    {1.35899e-05, 0.230369}, 
    {0.000189185, 0.766878}, 
};

GLfloat jitter4[4][2] = {
	{0.375, 0.25}, {0.125, 0.75}, 
	{0.875, 0.25}, {0.625, 0.75},
};

GLfloat jitter5[5][2] = {
	{0.5, 0.5}, {0.3, 0.1}, 
	{0.7, 0.9}, {0.9, 0.3},
	{0.1, 0.7},
};
	
GLfloat jitter6[6][2] = {
	{0.4646464646, 0.4646464646}, {0.1313131313, 0.7979797979},
	{0.5353535353, 0.8686868686}, {0.8686868686, 0.5353535353},
	{0.7979797979, 0.1313131313}, {0.2020202020, 0.2020202020},
};

GLfloat jitter8[8][2] = {
	{0.5625, 0.4375}, {0.0625, 0.9375},
  {0.3125, 0.6875}, {0.6875, 0.8125},
	{0.8125, 0.1875}, {0.9375, 0.5625}, 
	{0.4375, 0.0625}, {0.1875, 0.3125},
};

GLfloat jitter9[9][2] = {
	{0.5, 0.5}, {0.1666666666, 0.9444444444},
	{0.5, 0.16666666666}, {0.5, 0.8333333333},
	{0.1666666666, 0.6111111111}, {0.8333333333, 0.7222222222},
	{0.8333333333, 0.0555555555},
};
			
GLfloat jitter11[11][2] = {
    {0.5, 0.5}, {0.406537, 0.135858}, 
    {0.860325, 0.968558}, {0.680141, 0.232877}, 
    {0.775694, 0.584871}, {0.963354, 0.309056}, 
    {0.593493, 0.864072}, {0.224334, 0.415055}, 
    {0.0366643, 0.690884}, {0.139685, 0.0313988}, 
    {0.319861, 0.767097}, 
};

GLfloat jitter12[12][2] = {
	{0.4166666666, 0.625}, {0.9166666666, 0.875},
	{0.25, 0.375}, {0.4166666666, 0.125}, 
	{0.75, 0.125}, {0.0833333333, 0.125},
	{0.75, 0.625}, {0.25, 0.875}, {0.5833333333, 0.375},
	{0.9166666666, 0.375}, {0.0833333333, 0.625}, {0.5833333333, 0.875},
};
	
GLfloat jitter16[16][2] = {
    {0.4375, 0.4375}, {0.1875, 0.5625}, 
    {0.9375, 1.1875}, {0.4375, 0.9375-1}, 
    {0.6875, 0.5625}, {0.1875, 0.0625}, 
    {0.6875, 0.3125}, {0.1875, 0.3125}, 
    {0.4375, 0.1875}, {0.9375-1, 0.4375}, 
    {0.6875, 0.8125}, {0.4375, 0.6875}, 
    {0.6875, 0.0625}, {0.9375, 0.9375}, 
    {1.1875, 0.8125}, {0.9375, 0.6875}, 
};

GLfloat jitter29[29][2] = {
    {0.5, 0.5}, {0.498126, 0.141363}, 
    {0.217276, 0.651732}, {0.439503, 0.954859}, 
    {0.734171, 0.836294}, {0.912454, 0.79952}, 
    {0.406153, 0.671156}, {0.0163892, 0.631994}, 
    {0.298064, 0.843476}, {0.312025, 0.0990405}, 
    {0.98135, 0.965697}, {0.841999, 0.272378}, 
    {0.559348, 0.32727}, {0.809331, 0.638901}, 
    {0.632583, 0.994471}, {0.00588314, 0.146344}, 
    {0.713365, 0.437896}, {0.185173, 0.246584}, 
    {0.901735, 0.474544}, {0.366423, 0.296698}, 
    {0.687032, 0.188184}, {0.313256, 0.472999}, 
    {0.543195, 0.800044}, {0.629329, 0.631599}, 
    {0.818263, 0.0439354}, {0.163978, 0.00621497}, 
    {0.109533, 0.812811}, {0.131325, 0.471624}, 
    {0.0196755, 0.331813}, 
};

GLfloat jitter90[90][2] = {
    {0.5, 0.5}, {0.784289, 0.417355}, 
    {0.608691, 0.678948}, {0.546538, 0.976002}, 
    {0.972245, 0.270498}, {0.765121, 0.189392}, 
    {0.513193, 0.743827}, {0.123709, 0.874866}, 
    {0.991334, 0.745136}, {0.56342, 0.0925047}, 
    {0.662226, 0.143317}, {0.444563, 0.928535}, 
    {0.248017, 0.981655}, {0.100115, 0.771923}, 
    {0.593937, 0.559383}, {0.392095, 0.225932}, 
    {0.428776, 0.812094}, {0.510615, 0.633584}, 
    {0.836431, 0.00343328}, {0.494037, 0.391771}, 
    {0.617448, 0.792324}, {0.688599, 0.48914}, 
    {0.530421, 0.859206}, {0.0742278, 0.665344}, 
    {0.979388, 0.626835}, {0.183806, 0.479216}, 
    {0.151222, 0.0803998}, {0.476489, 0.157863}, 
    {0.792675, 0.653531}, {0.0990416, 0.267284}, 
    {0.776667, 0.303894}, {0.312904, 0.296018}, 
    {0.288777, 0.691008}, {0.460097, 0.0436075}, 
    {0.594323, 0.440751}, {0.876296, 0.472043}, 
    {0.0442623, 0.0693901}, {0.355476, 0.00442787}, 
    {0.391763, 0.361327}, {0.406994, 0.696053}, 
    {0.708393, 0.724992}, {0.925807, 0.933103}, 
    {0.850618, 0.11774}, {0.867486, 0.233677}, 
    {0.208805, 0.285484}, {0.572129, 0.211505}, 
    {0.172931, 0.180455}, {0.327574, 0.598031}, 
    {0.685187, 0.372379}, {0.23375, 0.878555}, 
    {0.960657, 0.409561}, {0.371005, 0.113866}, 
    {0.29471, 0.496941}, {0.748611, 0.0735321}, 
    {0.878643, 0.34504}, {0.210987, 0.778228}, 
    {0.692961, 0.606194}, {0.82152, 0.8893}, 
    {0.0982095, 0.563104}, {0.214514, 0.581197}, 
    {0.734262, 0.956545}, {0.881377, 0.583548}, 
    {0.0560485, 0.174277}, {0.0729515, 0.458003}, 
    {0.719604, 0.840564}, {0.325388, 0.7883}, 
    {0.26136, 0.0848927}, {0.393754, 0.467505}, 
    {0.425361, 0.577672}, {0.648594, 0.0248658}, 
    {0.983843, 0.521048}, {0.272936, 0.395127}, 
    {0.177695, 0.675733}, {0.89175, 0.700901}, 
    {0.632301, 0.908259}, {0.782859, 0.53611}, 
    {0.0141421, 0.855548}, {0.0437116, 0.351866}, 
    {0.939604, 0.0450863}, {0.0320883, 0.962943}, 
    {0.341155, 0.895317}, {0.952087, 0.158387}, 
    {0.908415, 0.820054}, {0.481435, 0.281195}, 
    {0.675525, 0.25699}, {0.585273, 0.324454}, 
    {0.156488, 0.376783}, {0.140434, 0.977416}, 
    {0.808155, 0.77305}, {0.282973, 0.188937}, 
};

static void loaddxdy(int accsize)
{
    long i;
		GLfloat *jitter;
		
		/* allocate memory */
		dx = (GLfloat *) ALLOC(sizeof(GLfloat) * accsize);
		dy = (GLfloat *) ALLOC(sizeof(GLfloat) * accsize);
		if (dy == NULL || dx == NULL)
				error(Enomem);
		 
		/* chose jitter field */
		switch(accsize) {
				case 2:
					jitter = &(jitter2[0][0]);
					break;
				case 3:
					jitter = &(jitter3[0][0]);
					break;
				case 4:
					jitter = &(jitter4[0][0]);
					break;
				case 5:
					jitter = &(jitter5[0][0]);
					break;
				case 6:
					jitter = &(jitter6[0][0]);
					break;
				case 8:
					jitter = &(jitter8[0][0]);
					break;
				case 9:
					jitter = &(jitter9[0][0]);
					break;
				case 11:
					jitter = &(jitter11[0][0]);
					break;
				case 12:
					jitter = &(jitter12[0][0]);
					break;
				case 16:
					jitter = &(jitter16[0][0]);
					break;
				case 29:
					jitter = &(jitter29[0][0]);
					break;
				case 90:
					jitter = &(jitter90[0][0]);
					break;
				
		}
	
    for (i = 0; i < accsize; i++) {
			dx[i] = (GLfloat) jitter[i * 2    ] * 100 / gl_width;
			dy[i] = (GLfloat) jitter[i * 2 + 1] * 100 / gl_height;
    }
}

void displayAccum(int accsize)
{
    int i;

    glClear(GL_ACCUM_BUFFER_BIT);
    loaddxdy(accsize);
    for (i = 0; i < accsize; i++) {
#ifdef DEBUG
	printf("Pass %d\n", i);
#endif
			glPushMatrix();
			glTranslatef(dx[i], dy[i], 0.0);
		
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			/* draw the scene */
			zeichne_geometrie();
			glPopMatrix();
			glAccum(GL_ACCUM, 1.0/accsize);
    }
#ifdef DEBUG
    printf("final job\n");
#endif
    glAccum(GL_RETURN, 1.0); /* return accum. picture to screen */
    glFlush();
}



