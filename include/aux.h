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
#ifndef __aux_h__
#define __aux_h__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "auxDevice.h"
#include "auxToolkit.h"



/**************************************************************************
 *    Global Defines
 **************************************************************************/
/*
** ToolKit Window Types
*/

#define AUX_RGB          (1<<1)
#define AUX_RGBA         AUX_RGB
#define AUX_INDEX        (1<<2)
#define AUX_SINGLE       (1<<3)
#define AUX_DOUBLE       (1<<4)
#define AUX_DEPTH        (1<<5)
#define AUX_STENCIL      (1<<6)
#define AUX_ACCUM        (1<<7)
#define AUX_AUX          (1<<8)
#define AUX_ALPHA        (1<<9)
#define AUX_SAMPLES      (1<<10)

/*
** Prototypes
*/

extern void auxInitDisplayMode(GLenum mode);
extern void auxInitPosition(int x, int y, int width, int height);
extern void auxInitWindow(char *title);
extern void auxCloseWindow(void);

extern void auxQuit(void);
extern void auxSwapBuffers(void);

extern Display *auxXDisplay(GLvoid);
extern Window auxXWindow(GLvoid);

extern void auxMainLoop(void (*drawScene)());
extern void auxExposeFunc(void (*function)(GLsizei width, GLsizei height));
extern void auxReshapeFunc(void (*function)(GLsizei width, GLsizei height));
extern void auxIdleFunc(void (*function)());
extern void auxKeyFunc(int key, void (*function)());
extern void auxMouseFunc(int button, int mode, void (*function)(AUX_EVENTREC *));

extern int  auxGetColorMapSize(void);
extern void auxGetMouseLoc(int *x, int *y);
extern void auxSetOneColor(int index, float red, float green, float blue);
extern void auxSetFogRamp(int , int);
extern void auxSetGreyRamp(void);
extern void auxSetRGBMap(int, float *);

/* this function only works properly with images sized 2^m x 2^n */
extern AUX_RGBImageRec *auxRGBImageLoad(char *);

extern void auxCreateFont(void);
extern void auxDrawStr(char *);

extern void auxWireSphere(GLdouble);
extern void auxSolidSphere(GLdouble);
extern void auxWireCube(GLdouble);
extern void auxSolidCube(GLdouble);
extern void auxWireBox(GLdouble, GLdouble, GLdouble);
extern void auxSolidBox(GLdouble, GLdouble, GLdouble);
extern void auxWireTorus(GLdouble, GLdouble);
extern void auxSolidTorus(GLdouble, GLdouble);
extern void auxWireCylinder(GLdouble, GLdouble);
extern void auxSolidCylinder(GLdouble, GLdouble);
extern void auxWireIcosahedron(GLdouble);
extern void auxSolidIcosahedron(GLdouble);
extern void auxWireOctahedron(GLdouble);
extern void auxSolidOctahedron(GLdouble);
extern void auxWireTetrahedron(GLdouble);
extern void auxSolidTetrahedron(GLdouble);
extern void auxWireDodecahedron(GLdouble);
extern void auxSolidDodecahedron(GLdouble);
extern void auxWireCone(GLdouble, GLdouble);
extern void auxSolidCone(GLdouble, GLdouble);
extern void auxWireTeapot(GLdouble);
extern void auxSolidTeapot(GLdouble);

/*
 * OpenGL Programming Class Library extensions
 */

#define AUX_OFF          GL_FALSE
#define AUX_ON           GL_TRUE

/* --- Window Functions allow use of multiple windows --- */

extern GLvoid    auxWinSet( GLint );
extern GLint     auxWinGet( GLvoid );
extern GLvoid    auxWinClose( GLint );
extern void 	 auxKeepAspect(int width, int height);

/* --- Event Processing Functions --- */

extern GLboolean auxEventPending( GLvoid );
extern GLint     auxReadEvent( GLint *, GLint *, GLint * );

/* --- Toolkit Functions --- */

extern GLvoid    auxDeleteMouseFunc( GLint, GLint, GLvoid (*)(AUX_EVENTREC *) );
extern GLvoid    auxDeleteAllMouseFunc( GLint, GLint );
extern GLvoid    auxAnimation( GLboolean );
extern GLvoid    auxDispatchEvent( GLint );

/* --- Miscellanous Functions --- */

extern GLvoid    auxGetSize( GLsizei *, GLsizei * );
extern GLvoid    auxGetOrigin( GLint *, GLint * );
extern GLvoid    auxGetScreenSize( GLsizei *width, GLsizei *height );
extern GLvoid	 auxXYaxes( GLvoid );
extern GLvoid	 auxXYZaxes( GLvoid );
extern GLvoid	 auxWireGrid( GLvoid );
extern GLvoid	 auxSolidGrid( GLvoid );

/* --- Text Functions --- */

extern GLint     auxNewFont( char * );
extern GLvoid    auxRenderFontString( GLint, char * );
extern GLint     auxFontStringLength( GLint, char * );

#ifdef __cplusplus
}
#endif

#endif /* __aux_h__ */
