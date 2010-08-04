/********************************************************************
 *
 * BXwidgets: HelpTextI.h (XPM-Header)
 *
 * SccsId = "@(#)HelpTextI.h	1.4  7/11/95  (Copyright 1994, BREDEX GmbH)"
 *
 ********************************************************************/
#ifndef _HelpTextI_h
#define _HelpTextI_h

#include <BXm/BXm.h>

/*
 * Copyright 1990-93 GROUPE BULL
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of GROUPE BULL not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  GROUPE BULL makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * GROUPE BULL disclaims all warranties with regard to this software,
 * including all implied warranties of merchantability and fitness,
 * in no event shall GROUPE BULL be liable for any special,
 * indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits,
 * whether in an action of contract, negligence or other tortious
 * action, arising out of or in connection with the use 
 * or performance of this software.
 *
 * Arnaud LE HORS  BULL Research FRANCE -- Koala Project 
 *                 (XPM - X PixMap format version 2 & 3)
 *     Internet:   lehors@sophia.inria.fr
 * Surface Mail:   Arnaud LE HORS, INRIA - Sophia Antipolis, 
 *                 2004, route des Lucioles, 06565 Valbonne Cedex -- FRANCE
 *  Voice phone:   (33) 93.65.77.71, Fax: (33) 93 65 77 66, Telex: 97 00 50 F
 *
 */

/*****************************************************************************\
* xpm.h:                                                                      *
*                                                                             *
*  XPM library                                                                *
*  Include file                                                               *
*                                                                             *
*  Developed by Arnaud Le Hors                                                *
\*****************************************************************************/

#ifndef BXPM_h
#define BXPM_h

/*
 * first some identification numbers:
 * the following revision numbers is determined with the following rule:
 * SO Major number = LIB minor version number.
 * SO Minor number = LIB sub-minor version number.
 * e.g: BXpm version 3.2f
 *      we forget the 3 which is the format number, 2 gives 2, and f gives 6.
 *      thus we have BXpmVersion = 2 and BXpmRevision = 6
 *      which gives  SOXPMLIBREV = 2.6
 */
#define BXpmFormat 3
#define BXpmVersion 3
#define BXpmRevision 0

#ifndef BXPM_NUMBERS

#ifdef VMS
#include "decw$include:Xlib.h"
#include "decw$include:Xutil.h"
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

/* let's define Pixel if it is not done yet */
#if ! defined(_XtIntrinsic_h) && ! defined(PIXEL_ALREADY_TYPEDEFED)
typedef unsigned long Pixel;		/* Index into colormap */
#define PIXEL_ALREADY_TYPEDEFED
#endif

/* Return ErrorStatus codes:
 * null     if full success
 * positive if partial success
 * negative if failure
 */

#define BXpmColorError    1
#define BXpmSuccess       0
#define BXpmOpenFailed   -1
#define BXpmFileInvalid  -2
#define BXpmNoMemory     -3
#define BXpmColorFailed  -4

/* the following should help people wanting to use their own functions */
/*#define BXpmFree(ptr) free(ptr)*/
#define BXpmFree(ptr) BXFREE(ptr)

typedef struct {
    char *name;				/* Symbolic color name */
    char *value;			/* Color value */
    Pixel pixel;			/* Color pixel */
} BXpmColorSymbol;

typedef struct {
    char *name;				/* name of the extension */
    unsigned int nlines;		/* number of lines in this extension */
    char **lines;			/* pointer to the extension array of
					 * strings */
} BXpmExtension;

typedef struct {
    unsigned long valuemask;		/* Specifies which attributes are
					 * defined */

    Visual *visual;			/* Specifies the visual to use */
    Colormap colormap;			/* Specifies the colormap to use */
    unsigned int depth;			/* Specifies the depth */
    unsigned int width;			/* Returns the width of the created
					 * pixmap */
    unsigned int height;		/* Returns the height of the created
					 * pixmap */
    unsigned int x_hotspot;		/* Returns the x hotspot's
					 * coordinate */
    unsigned int y_hotspot;		/* Returns the y hotspot's
					 * coordinate */
    unsigned int cpp;			/* Specifies the number of char per
					 * pixel */
    Pixel *pixels;			/* List of used color pixels */
    unsigned int npixels;		/* Number of pixels */
    BXpmColorSymbol *colorsymbols;	/* Array of color symbols to
					 * override */
    unsigned int numsymbols;		/* Number of symbols */
    char *rgb_fname;			/* RGB text file name */
    unsigned int nextensions;		/* number of extensions */
    BXpmExtension *extensions;		/* pointer to array of extensions */

    /* Color Allocation Directives */
    unsigned int exactColors;		/* Only use exact colors for visual */
    unsigned int closeness;		/* Allowable RGB deviation */
    unsigned int red_closeness;		/* Allowable red deviation */
    unsigned int green_closeness;	/* Allowable green deviation */
    unsigned int blue_closeness;	/* Allowable blue deviation */
    int color_key;			/* Use colors from this color set */
} BXpmAttributes;

/* BXpm attribute value masks bits */
#define BXpmVisual	   (1L<<0)
#define BXpmColormap	   (1L<<1)
#define BXpmDepth	   (1L<<2)
#define BXpmSize		   (1L<<3)	/* width & height */
#define BXpmHotspot	   (1L<<4)	/* x_hotspot & y_hotspot */
#define BXpmCharsPerPixel   (1L<<5)
#define BXpmColorSymbols	   (1L<<6)
#define BXpmRgbFilename	   (1L<<7)
/************************************ there was
#define BXpmInfos	   (1L<<8) all infos members
*/
#define BXpmExtensions      (1L<<10)

#define BXpmReturnPixels	   (1L<<9)
/************************************ there was
#define BXpmReturnInfos	   BXpmInfos
*/
#define BXpmReturnExtensions BXpmExtensions

#define BXpmExactColors     (1L<<11)
#define BXpmCloseness	   (1L<<12)
#define BXpmRGBCloseness	   (1L<<13)
#define BXpmColorKey	   (1L<<14)

/*
 * color keys for visual type, they must fit along with the number key of
 * each related element in xpmColorKeys[] defined in xpmP.h
 */
#define BXPM_MONO	2
#define BXPM_GREY4	3
#define BXPM_GRAY4	3
#define BXPM_GREY 	4
#define BXPM_GRAY 	4
#define BXPM_COLOR	5


typedef struct {
    char *string;		/* characters string */
    char *symbolic;		/* symbolic name */
    char *m_color;		/* monochrom default */
    char *g4_color;		/* 4 level grayscale default */
    char *g_color;		/* other level grayscale default */
    char *c_color;		/* color default */
} BXpmColor;

typedef struct {
    unsigned int width;		/* image width */
    unsigned int height;	/* image height */
    unsigned int cpp;		/* number of characters per pixel */
    unsigned int ncolors;	/* number of colors */
    BXpmColor *colorTable;	/* list of related colors */
    unsigned int *data;		/* image data */
} BXpmImage;

typedef struct {
    char *hints_cmt;		/* Comment of the hints section */
    char *colors_cmt;		/* Comment of the colors section */
    char *pixels_cmt;		/* Comment of the pixels section */
} BXpmInfos;


/*
 * minimal portability layer between ansi and KR C
 */

/* forward declaration of functions with prototypes */

#if __STDC__ || defined(__cplusplus) || defined(c_plusplus)
 /* ANSI || C++ */
#define FUNC(f, t, p) extern t f p
#define LFUNC(f, t, p) static t f p
#else					/* K&R */
#define FUNC(f, t, p) extern t f()
#define LFUNC(f, t, p) static t f()
#endif					/* end of K&R */


/*
 * functions declarations
 */

#ifdef __cplusplus
extern "C" {
#endif

    FUNC(BXpmReadFileToPixmap, int, (Display *display,
				    Drawable d,
				    char *filename,
				    Pixmap *pixmap_return,
				    Pixmap *shapemask_return,
				    BXpmAttributes *attributes));

    FUNC(BXpmReadFileToImage, int, (Display *display,
				   char *filename,
				   XImage **image_return,
				   XImage **shapeimage_return,
				   BXpmAttributes *attributes));

    FUNC(BXpmReadFileToBuffer, int, (char *filename, char **buffer_return));
    FUNC(BXpmWriteFileFromBuffer, int, (char *filename, char *buffer));

    FUNC(BXpmAttributesSize, int, (void));
    FUNC(BXpmFreeAttributes, void, (BXpmAttributes *attributes));
    FUNC(BXpmFreeExtensions, void, (BXpmExtension *extensions,
				   int nextensions));
    FUNC(BXpmFreeXpmImage, void, (BXpmImage *image));
    FUNC(BXpmFreeXpmInfos, void, (BXpmInfos *infos));
    FUNC(BXpmGetErrorString, char *, (int errcode));

    /* BXpmImage functions */
    FUNC(BXpmReadFileToXpmImage, int, (char *filename,
				      BXpmImage *image,
				      BXpmAttributes *attributes,
				      BXpmInfos *infos));

    FUNC(BXpmCreatePixmapFromXpmImage, int, (Display *display,
					    Drawable d,
					    BXpmImage *image,
					    Pixmap *pixmap_return,
					    Pixmap *shapemask_return,
					    BXpmAttributes *attributes));

    FUNC(BXpmCreateImageFromXpmImage, int, (Display *display,
					   BXpmImage *image,
					   XImage **image_return,
					   XImage **shapeimage_return,
					   BXpmAttributes *attributes));

#ifdef __cplusplus
}					/* for C++ V2.0 */
#endif

#endif /* BXPM_NUMBERS */
#endif

#endif	/* _HelpTextI_h */

