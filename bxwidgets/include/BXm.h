/********************************************************************
 *
 * General public header file of BXwidgets
 *
 ********************************************************************
 *
 * SccsId = "@(#)BXm.h	2.45  02/19/97  (Copyright 1992, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1992, 1993 BREDEX GmbH, Braunschweig, Germany
 * ALL RIGHTS RESERVED
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND
 * COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
 * ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY BREDEX GmbH.
 * 
 * BREDEX GmbH ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY
 * TO USE ANY OF ITS SOFTWARE. BXwidgets IS PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, AND BREDEX EXPRESSLY DISCLAIMS ALL IMPLIED
 * WARRANTIES, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL BREDEX BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * BREDEX GmbH
 * Fallersleber-Tor-Wall 23
 * D-38100 Braunschweig
 * phone: +49-531-243300
 * mail:  bxwidgets@bredex.de
 *
 ********************************************************************/
#ifndef _BXm_h
#define _BXm_h

/* include header file for motif (if not happened yet) */
#ifndef _Xm_h
#include <Xm/Xm.h>
#endif


/********************************************************************
 * insert file with declarations for constants
 ********************************************************************/
#include <BXm/BXmConst.h>
#include <BXm/BXmRes.h>


/********************************************************************
 * general macro for prototypes
 ********************************************************************/

#ifndef _PROTOS
#ifdef _NO_PROTO
#define _PROTOS(args)	()
#else
#define _PROTOS(args)	args
#endif
#endif


/********************************************************************
 * macros for memory allocation
 ********************************************************************/

#define BXMALLOC(num,type) \
	    ((type*)XtMalloc((Cardinal)((num)*sizeof(type))))
#define BXCALLOC(num,type) \
	    ((type*)XtCalloc((Cardinal)(num),sizeof(type)))
#define BXREALLOC(ptr,num,type)  \
	    ((type*)XtRealloc((char*)(ptr),(Cardinal)((num)*sizeof(type))))
#define BXRESIZE(ptr,num,type)  \
	    ((ptr)=(type*)XtRealloc((char*)(ptr),(Cardinal)((num)*sizeof(type))))
#define BXFREE(ptr)	\
	    (XtFree((char*)(ptr)),(ptr)=NULL)
#define BXNEWSTRING(str) \
	    ((str) == (String)NULL ? (String)NULL : XtNewString(str))


/********************************************************************
 * little useful macros
 ********************************************************************/

#ifndef MAX
#define MAX(a,b)        (((a)>(b))?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b)        (((a)<(b))?(a):(b))
#endif

/* whole width and height */
#define WWIDTH(w)       ((w)->core.width + 2*(w)->core.border_width)
#define WHEIGHT(w)      ((w)->core.height + 2*(w)->core.border_width)

#define DISPLAY(w)      (XtDisplay((Widget)(w)))
#define WINDOW(w)       (XtWindow((Widget)(w)))
#define SCREEN(w)       (XtScreen((Widget)(w)))
#define SCREENNO(w)     (XScreenNumberOfScreen(SCREEN(w)))
#define ROOT(w)         (RootWindowOfScreen(SCREEN(w)))

#define BXmNO_PIXMAP	((Pixmap)0)


/********************************************************************
 * macros for Motif functions
 ********************************************************************/

#if XmVersion <= 1001

#define XMSTRING_CREATE(s) XmStringCreateSimple(s)
#define CONFIGURE(wid,x,y,w,h,bw) \
        	_XmConfigureObject((RectObj)(wid),(x),(y),(w),(h),(bw))
#define MOVE(wid,x,y) \
		 _XmMoveObject((RectObj)(wid),(x),(y))
#define REDISPLAY(wid,evp,reg)\
	  _XmRedisplayGadgets((CompositeWidget)(wid),(XExposeEvent*)(evp),(reg))
#define DRAWSHADOWS(dpy,d,topGC,botGC,x,y,w,h,th,type) \
	  {if ((type)==XmSHADOW_IN||(type)==XmSHADOW_ETCHED_IN) \
	       _XmDrawShadow((dpy),(d),(botGC),(topGC),(th),(x),(y),(w),(h)); \
	   else \
	       _XmDrawShadow((dpy),(d),(topGC),(botGC),(th),(x),(y),(w),(h)); \
	  }
#define DRAWHIGHLIGHT(wid,gc,x,y,w,h,th) \
	  _XmDrawBorder((Widget)(wid),(gc),(x),(y),(w),(h),(th))
#define DEFBELL(wid)	True
/* in motif1.1 they are not in motif header files: */
extern void _XmForegroundColorDefault _PROTOS(( Widget, int, XrmValue* ));
extern void _XmBackgroundColorDefault _PROTOS(( Widget, int, XrmValue* ));

#else

#include <Xm/DrawP.h>

#define XMSTRING_CREATE(s) XmStringCreateLocalized(s)
#define CONFIGURE(wid,x,y,w,h,bw) \
        	_XmConfigureObject((wid),(x),(y),(w),(h),(bw))
#define MOVE(wid,x,y) \
		_XmMoveObject((wid),(x),(y))
#define REDISPLAY(wid,evp,reg) \
		_XmRedisplayGadgets((wid),(evp),(reg))
#define DRAWSHADOWS(dpy,d,topGC,botGC,x,y,w,h,th,type) \
	  _XmDrawShadows((dpy),(d),(topGC),(botGC),(x),(y),(w),(h),(th),(type))
#define DRAWHIGHLIGHT(wid,gc,x,y,w,h,th) \
     _XmDrawSimpleHighlight(DISPLAY(wid),WINDOW(wid),(gc),(x),(y),(w),(h),(th))
#define DEFBELL(wid)	(_XmGetAudibleWarning((Widget)(wid))==XmBELL)

#endif

#define FG_COLOR_DEFAULT_FUNC_PTR _XmForegroundColorDefault
#define BG_COLOR_DEFAULT_FUNC_PTR _XmBackgroundColorDefault


/********************************************************************
 * macros for resource management
 ********************************************************************/

/* default for search pathes:
 *  - local directory with type (and language)
 *  - directory /usr/lib/X11 with type (and language)
 *  - directory /usr/include/X11 with type (and language)
 */
#define BXM_DEFAULT_PATH \
"%L/%T/%N%S:%l/%T/%N%S:%T/%N%S:%N%S:/usr/lib/X11/%L/%T/%N%S:/usr/lib/X11/%l/%T/%N%S:/usr/lib/X11/%T/%N%S:/usr/include/X11/%L/%T/%N%S:/usr/include/X11/%l/%T/%N%S:/usr/include/X11/%T/%N%S"


/********************************************************************
 * data types
 ********************************************************************/

/* data types for table values
 */
typedef struct {
    int	row;		/* Row in Table (BXmALL: all) */
    int	column;		/* Column in Table (BXmALL: all) */
    int value;		/* int value there */
} BXmTableInt;

typedef struct {
    int	  row;		/* Row in Table (BXmALL: all) */
    int	  column;	/* Column in Table (BXmALL: all) */
    Pixel value;	/* pixel value there */
} BXmTablePixel;

typedef struct {
    int row;		/* Row in Table (BXmALL: all) */
    int column;		/* Column in Table (BXmALL: all) */
    String value;	/* String value there */
} BXmTableString;

typedef struct {
    int row;		/* Row in Table (BXmALL: all) */
    int column;		/* Column in Table (BXmALL: all) */
    Pixmap pixmap;	/* Pixmap value there (or BXmNO_PIXMAP) */
    String name;	/* name of the Pixmap or NULL */
} BXmTablePixmap;


typedef struct {
    int          qname;         /* quark */
    XFontStruct* font;
} _BXmFontInfo, *_BXmFontList;

/* data types for conversionPolicies (BXmRegex)
 */
typedef int (*BXmConversionPolicyProc) _PROTOS(( int ));
typedef struct _BXmConversionPolicyRec {
    String                      name;
    BXmConversionPolicyProc     proc;
} BXmConversionPolicyRec;


/* version dependent resource type
 */
#ifdef XlibSpecificationRelease
#define RES_ADDR_TYPE       XPointer
#else
#define RES_ADDR_TYPE       caddr_t
#endif

/********************************************************************
 * general global variables
 ********************************************************************/

/* special variable for end of string list (NULL not possible)
 */
extern String BXmSTRING_LIST_END;

/* special variable for version checks
 */
extern char BXmLibraryVersions[300];
extern char BXmLibrarySizes[300];


/********************************************************************
 * general global functions
 ********************************************************************/

extern void    BXmInitLibraryData	_PROTOS(( void ));
extern void    BXmRegisterConverters	_PROTOS(( void ));
extern int     BXmToupperLatin1		_PROTOS(( int ));
extern int     BXmTolowerLatin1		_PROTOS(( int ));
extern Boolean BXmSetRecomputeLayout    _PROTOS(( Widget, Boolean ));
extern Widget  BXmFindManagedWidgetByName _PROTOS(( Widget, XrmName ));
extern XtGeometryResult _BXmWishNewSize _PROTOS(( Widget, Dimension, Dimension, int));
extern void    _BXmInitDynaIntList	_PROTOS(( int**, int*, int*, int ));
extern Boolean _BXmStringEqual		_PROTOS(( char*, char* ));
extern void    _BXmWarning		_PROTOS(( String ));
extern void    _BXmWarningWithInt	_PROTOS(( String, int ));
extern void    _BXmWarningWithString	_PROTOS(( String, String ));
extern void    _BXmWarningWith2Strings	_PROTOS(( String, String, String ));
extern void    _BXmWarningWithStringInt	_PROTOS(( String, String, int ));
extern Boolean _BXmCorrectResizePolicyValue _PROTOS(( int ));
extern void    _BXmSetVLinkWidths       _PROTOS(( Widget, int, int, int*, Widget ));
extern int     _BXmGetVLinkMinWidth     _PROTOS(( Widget, int ));
extern _BXmFontList _BXmParseFontList _PROTOS(( XmFontList ));

/* in vpixmap.c */
extern void    BXmReplacePixmapConverter _PROTOS(( void ));

#endif      /* _BXm_h */

