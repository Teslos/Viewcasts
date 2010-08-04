/********************************************************************
 *
 * Private header file of widget class  BXmHelpText
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpTextP.h	1.42  10/13/95  (Copyright 1993, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1993 BREDEX GmbH, Braunschweig, Germany
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
#ifndef _BXmHelpTextP_h
#define _BXmHelpTextP_h


/* private header file of parent widget class
 * (in motif1.1 it is part of XmP.h)
 * ATTENTION: we must include Xm.h to see what private header
 *            files we need
 */
#include <Xm/Xm.h>
#if XmVersion <= 1001
#include <Xm/XmP.h>
#else
#include <Xm/PrimitiveP.h>
#endif

/* public header file */
#include <BXm/HelpText.h>

/* general header for help system
 * (interface to help compiler)
 */
#include <BXm/HelpGen.h>


/* type of new class part
 */
typedef struct _BXmHelpTextClassPart {
	XtPointer extension;		/* makes compiler happier */
} BXmHelpTextClassPart;


/* whole class type
 */
typedef struct _BXmHelpTextClassRec {
	CoreClassPart		core_class;
	XmPrimitiveClassPart	primitive_class;
	BXmHelpTextClassPart	help_class;
} BXmHelpTextClassRec;


/* class variable
 */
extern BXmHelpTextClassRec bxmHelpTextClassRec;


/* constants for font and gc switches
 *  - 4 typos (roman, boldface, slanted, verbatim)
 *  - 3 sizes (small, normal, large)
 */
#define	SW_RM		(0)
#define SW_BF		(1)
#define SW_SL		(2)
#define SW_TT		(3)

#define SW_NORM		(0)
#define SW_SMALL	(1)
#define SW_LARGE	(2)

#define NUM_TYPOS		(4)
#define NUM_SIZES		(3)
#define SWITCH(typo,size)	((NUM_TYPOS*(size))+(typo))
#define NUM_SWITCHES		(NUM_TYPOS*NUM_SIZES)

typedef enum { BXmRefNone, BXmRefTopic, BXmRefNote,
	       BXmRefLabel, BXmRefAction } BXmRefMode;
#define BXmREF_NO_ID	(-1)
#define BXmREF_NO_DATA	((void*)NULL)

/* type for segments of actual help topic
 */
typedef struct _BXmHelpSegment {
    /* static values:
     */
    int idx;		/* index for font and gc switch */
    String text;	/* DYN: text or NULL */
    int textlen;	/* strlen(text) */
    int width;		/* width of segment */
    int ascent;		/* ascent of segment */
    int descent;	/* descent of segment */
    int height;		/* height of segment (ascent + descent) */
    int eskip;		/* additional width after segment */
    int vskip;		/* additional height after segment (forces linebreak) */
    BXmRefMode refMode;
    int        refID;
    Pixmap bitmap;	/* bitmap or BXmNO_BITMAP */
    Pixmap pixmap;	/* pixmap or BXmNO_PIXMAP */
    Pixmap mask;	/* mask (bitmap) for pixmap or BXmNO_BITMAP */
    int quoteLevel;	/* for horizontal offset */
    String filename;    /* bitmap/pixmap filename */

    /* dynamic values from processLines():
     */
    int x;		/* actual x coord */
    int y;		/* actual y coord */
    Boolean isSelected;
    int pr_vskip;  
} BXmHelpSegment;

/* type for lines of actual help topic with actual size
 */
typedef struct {
    int y;			/* y coord */
    int height;			/* height (max of segments height) */
    int ascent;			/* ascent (max of segments ascent) */
    BXmHelpSegment* segp;	/* entry in array of segments */
    int segCount;		/* number of segments in line */
} BXmHelpLine;


/* type of new instance part
 */
typedef struct _BXmHelpTextPart {
    /* resources
     */
    String helpfile;		/* DYN: opened helpfile (or NULL) */
    String topic;		/* pointer to actual topic name */
    String* topicsList;		/* DYN: external list of topics */
    int topicsCount;		/* count (and size) of topicsList */
    XtCallbackList selectRefCallback;	/* SelectRef-Callbacks */
    XtCallbackList newTopicCallback;	/* NewTopic-Callbacks */
    XtCallbackList actionCallback;	/* Action-Callbacks */
    XmFontList fontList;	/* DYN: motif font list */
    XmFontList fallbackFontList;/* fallback motif font list */
    Dimension quoteIndent;	/* size of quote indent */
    Dimension marginWidth;	/* left and right margin */
    Dimension marginHeight;	/* top and bottom margin */
    String printCommand;	/* DYN: command to print with (for popen()) */
    String printFile;		/* DYN: file to print into */

    /* private data
     */
    FILE* fp;			/* DYN: opened helpfile */
    Cursor busyCursor;		/* DYN: cursor while parsing topic */

    String stdFontName;		/* DYN: Standard-Font-Name */
    String ttFamily;		/* DYN: teletype font family */
    int    fontSize[NUM_SIZES];	/* font sizes */
    String* listOfFonts;	/* DYN: list of font names */
    int     listOfFontsCount;   /* number of font names */
    XFontStruct* stdFont;			/* standard font */
    XFontStruct* loadedFont[NUM_SWITCHES];	/* DYN: loaded fonts */
    XFontStruct* font[NUM_SWITCHES];		/* special fonts */
    GC gc[NUM_SWITCHES];	/* DYN: shared GCs for special fonts */
    GC stdGC;			/* DYN: shared GC for norm. lines and bitmaps */
    GC dashGC;			/* DYN: shared GC for dashed lines */
    GC invGC;			/* DYN: sharedGC for inverting */

    BXmHelpSegment* segs;	/* DYN: segments of actual topic */
    int segsSize;		/* size of segs */
    int segsCount;		/* number of segments */
    BXmHelpLine* lines;		/* DYN: lines of actual topic */
    int linesSize;		/* size of lines */
    int linesCount;		/* number of lines */
    BXmHelpRefText* topics;	/* DYN: internal list of topics */
    int topicsSize;		/* size of topics */
    int topicsNum;		/* number of internal topics */
    BXmHelpRefText* notes;	/* DYN: list of notes */
    int notesSize;		/* size of notes */
    int notesNum;		/* number of notes */
    String* actionList;		/* DYN: list of action names */
    int actionSize;		/* size of actionList */
    int actionNum;		/* number of action names */

    int topicID;		/* ID of actual topic (or BXmNO_TOPIC_ID) */
    BXmHelpSegment* selectedSegment;	/* pointer to selected segment */

    int preferredWidth;		/* preferred width */
    int preferredHeight;	/* preferred height */
    String path;		/* DYN: search path */

    Widget popupWidget;		/* XmLabel widget of note popup */
    Boolean popupVisible;	/* True if note popup is visible */
    String popupString;		/* DYN: string for note popup */
    int popupStringSize;	/* size of popupString */
    int popupTextID;		/* last popuped noteID */

    Widget helpbox;		/* BXmHelpBox if there in (helpbox sets it */
    Pixmap drawable;
} BXmHelpTextPart;


/* whole instance type
 */
typedef struct _BXmHelpTextRec {
	CorePart	core;
	XmPrimitivePart	primitive;
	BXmHelpTextPart	help;
} BXmHelpTextRec;

/* special internal for BXmHelpBox */
extern XmFontList _BXmHelpTextGetNewFontList _PROTOS(( Widget ));

/* PostScript header in HelpHead.c */
extern String _bxmHelpPostScriptHead[];

#endif	/* _BXmHelpTextP_h */

