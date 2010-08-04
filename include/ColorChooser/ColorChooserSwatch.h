/* 
 * (c) Copyright 1993 Silcon Graphics, INC. 
 * ALL RIGHTS RESERVED 
*/ 
/*   $RCSfile: ColorChooserSwatch.h,v $ $Revision: 1.1 $ $Date: 1995/01/27 18:06:01 $ */
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _ColorChooserSwatch_h
#define _ColorChooserSwatch_h

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Class record constants */

typedef struct _ColorChooserSwatch *ColorChooserSwatch;


/********    Public Function Declarations    ********/
#ifdef _NO_PROTO

extern ColorChooserSwatch SgSwatchAllocColor();
extern void               SgSwatchSetColor();
extern void               SgSwatchGetColor();


#else

extern ColorChooserSwatch SgSwatchAllocColor(Widget w);
extern void               SgSwatchSetColor(ColorChooserSwatch swatch, 
					 short r, short g, short b);
extern void               SgSwatchGetColor(ColorChooserSwatch swatch, 
					 short *r, short *g, short *b);


#endif /* _NO_PROTO */
/********    End Public Function Declarations    ********/


#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _ColorChooserSwatch_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
