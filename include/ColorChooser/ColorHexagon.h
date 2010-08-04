/* 
 * (c) Copyright 1993 Silcon Graphics, INC. 
 * ALL RIGHTS RESERVED 
*/ 
/*   $RCSfile: ColorHexagon.h,v $ $Revision: 1.1 $ $Date: 1995/01/27 18:06:01 $ */
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _ColorHexagon_h
#define _ColorHexagon_h

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Class record constants */

typedef struct _ColorHexagonObj *ColorHexagon;


/********    Public Function Declarations    ********/
#ifdef _NO_PROTO

extern ColorHexagon SgColorHexagonCreate();
extern void         SgColorHexagonDestroy();
extern void         SgColorHexagonSetColor();
extern void         SgColorHexagonGetColor();
extern void         SgColorHexagonSetHSV();
extern void         SgColorHexagonSetWysiwyg();

#else

extern ColorHexagon SgColorHexagonCreate(Widget w, Boolean use_popup, Widget overlay_w);

extern void         SgColorHexagonDestroy(ColorHexagon hexagon);
extern void         SgColorHexagonSetColor(ColorHexagon hexagon, short r, 
					   short g, short b);
extern void         SgColorHexagonGetColor(ColorHexagon hexagon, short *r, 
					   short *g, short *b);
extern void         SgColorHexagonSetHSV(ColorHexagon hexagon, 
					 float h, float s, float v); 
extern void         SgColorHexagonSetWysiwyg(ColorHexagon hexagon, 
					     Boolean wysiwyg);

#endif /* _NO_PROTO */
/********    End Public Function Declarations    ********/


#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _ColorHexagon_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
