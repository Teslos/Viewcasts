/********************************************************************
 *
 * Private header file of widget class BXmTextTable
 *
 ********************************************************************
 *
 * SccsId = "@(#)TextTableP.h	2.46  01/17/97  (Copyright 1992, BREDEX GmbH)"
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
#ifndef _BXmTextTableP_h
#define _BXmTextTableP_h


/* private header file of parent widget class
 * (in motif1.1 it is part of XmP.h)
 * ATTENTION: we must include Xm.h to see what private header
 *            files we need
 */
#include <Xm/Xm.h>
#if XmVersion <= 1001
#include <Xm/XmP.h>
#else
#include <Xm/ManagerP.h>
#include <Xm/GadgetP.h>
#endif

/* public header file
 */
#include <BXm/TextTable.h>

/* header file for GC cashing
 */
#include <BXm/TextTableGC.h>


/****************************************************************
 *
 * types, macros, functions for BXmTextTable's data management
 *
 ****************************************************************/

typedef struct {
    String**	data;		/* two dimensional array of strings */
    int		rows;
    int		columns;
    int		sizerows;
    int		sizecolumns;
    String	defaultString;
} BXmTTDataRec, *BXmTTData;

/* MACRO for direct and fast texttable data acces (no tests, no copy)
 */
#define BXmTT_FIELD(list,row,column)    ((list)->data[(row)][(column)])
#define BXmTT_DEFAULT(list)             ((list)->defaultString)

/* forward declarations
 *  - BXmTextTable data management
 */
extern BXmTTData _BXmTTDataCreate _PROTOS(( int, int, String ));
extern void _BXmTTDataDestroy _PROTOS(( BXmTTData ));
extern void _BXmTTDataSetNumRows _PROTOS(( BXmTTData, int ));
extern void _BXmTTDataSetNumColumns _PROTOS(( BXmTTData, int ));
extern void _BXmTTDataSetField _PROTOS(( BXmTTData, int, int, String ));
extern String _BXmTTDataGetField _PROTOS(( BXmTTData, int, int ));
extern void _BXmTTDataInsertRow _PROTOS(( BXmTTData, int ));
extern void _BXmTTDataDeleteRow _PROTOS(( BXmTTData, int ));
extern void _BXmTTDataInsertColumn _PROTOS(( BXmTTData, int ));
extern void _BXmTTDataDeleteColumn _PROTOS(( BXmTTData, int ));

/* forward declarations
 *  - BXmTextTable regex list management
 */
void _BXmTTRegListCreateAndInit _PROTOS((BXmTableString*, BXmTableString**, int*, int*));
void _BXmTTRegListSetRegex _PROTOS((int, int, String, BXmTableString**, int*, int*));
void _BXmTTRegListFree _PROTOS((BXmTableString**));
void _BXmTextTableSetActRegList _PROTOS((BXmTextTableWidget, BXmTableString**, int*));
BXmTableString* _BXmTTRegGetValues _PROTOS((BXmTableString*, int, BXmTableString**));


/****************************************************************
 *
 * other internal types 
 *
 ****************************************************************/

typedef struct {
    int     row;        /* Row in Table (BXmALL: all) */
    int     column;     /* Column in Table (BXmALL: all) */
    GC      gc;         /* Graphics Context there */
    Boolean fromList;	/* comes from entry in list */
} BXmTextTableGC;

typedef struct {
    int staticOffset;	/* static offset of column */
    int offset;		/* real/dynamic offset of column */
    int rawWidth;	/* total static width not clipped */
    int staticWidth;	/* total static width clipped */
    int	width;		/* total real/dynamic width */
    int maxrow;		/* row with maximum width */
} ColData;


/****************************************************************
 *
 * normal BXmTextTable stuff
 *
 ****************************************************************/

/* type of new class part
 */
typedef struct _BXmTextTableClassPart {
	XtInitProc      initBXmTTHook;    /* initialize hook for subclasses */
	XtSetValuesFunc setvalBXmTTHook;  /* setvalues hook for subclasses */
	XtPointer       extension;    	  /* makes compiler happier */
} BXmTextTableClassPart;

/* whole class type
 */
typedef struct _BXmTextTableClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	BXmTextTableClassPart	table_class;
} BXmTextTableClassRec;


/* class variable
 */
extern BXmTextTableClassRec bxmTextTableClassRec;

typedef struct {
    int row;		/* Row in Table (BXmALL: all) */
    int column;		/* Column in Table (BXmALL: all) */
    XFontStruct* value;	/* Font value there */
} BXmTableFont;

typedef struct {
    XrmQuark qname;
    Pixmap   id;
    int      width;
    int      height;
    int      depth;	/* 2: bitmap */
} _BXmTTPixmap;


/* don't use Boolean, cause non-prototype declarations use int as default */
#define BXmBOOL    	int

typedef struct {
    int      row;	/* BXmALL: all */
    int      col;	/* BXmALL: all */
    XtArgVal value;	/* the value */
    BXmBOOL  ext;	/* from outside (value for GetValues()) */
} _BXmRCTableData, *_BXmRCTableList;

typedef struct {
    _BXmRCTableList data;
    int             dataSize;
    int		    dataCount;
    XtPointer       gvData;
} _BXmRCTableRec, *_BXmRCTable;


/* type of new instance part
 */
typedef struct _BXmTextTablePart {

	/* resources for functionality
	 */
	int rows;		/* number of rows (res.: numRows) */
	int columns;		/* number of columns (res.: numColumns) */
	Boolean hasHorizHeader;
	Boolean hasVertiHeader;
	Boolean scrollHorizontal;
	Boolean scrollVertical;
	String* tableValue;
	String* horizHeaderValue;
	String* vertiHeaderValue;
	String  defaultValue;
	int currentRow;
	int currentColumn;
	String	currentValue;
	Boolean editable;
	Boolean headerEditable;
	Boolean editing;
	Boolean recomputeLayout;
	int resizePolicy;
	int processPolicy;
	Boolean autoWrap;
	Boolean autoActivate;
	int alignment;
	BXmTableInt* alignmentList;	/* external interface */
	BXmTableInt* shadowListRes;	/* external interface */
	BXmTableInt* editableListRes;	/* external interface */
	BXmTablePixel* backgroundList;	/* external interface */
	BXmTablePixel* foregroundList;	/* external interface */
	BXmTableString* regexList;
	BXmTableString* fontnameListRes;
	XmFontList fontList;		/* motif font list */
	Boolean verifyBell;
	int* colMinWidth;
	int* colMaxWidth;
	int  colUnitType;
	int* colMaxLength;
	Boolean allowColResize;
	int* dynaColList;
	int  dynaColCount;
	int dropPolicy;
	
        /* selection resources
	 */
	int selectionPolicy;
	int selectionOrientation;
	Pixel selectionBackground;
	Pixel selectionForeground;
	int* selectedPosList;
	int selectedPosCount;
	Boolean headerSelectable;
	XtCallbackList selectionCallback;

	/* callbacks
	 */
	XtCallbackList activateCallback;
	XtCallbackList modifyVerifyCallback;
	XtCallbackList changePositionCallback;
	XtCallbackList preEditCallback;
	XtCallbackList postEditCallback;

	/* resources for appearance
	 */
	int visibleRows;		/* number of visible rows */
	int firstVisibleRow;		/* value of vertical ScrollBar */
	Dimension marginWidth;
	Dimension marginHeight;
	Dimension innerMarginWidth;
	Dimension innerMarginHeight;
	Dimension horizSpacing;
	Dimension vertiSpacing;
	Dimension sbarSpacing;
	Dimension headSpacing;
	Dimension highlightThickness;
	Dimension frameThickness;
	Boolean showHHFrame;
	Boolean showSeparator;
	Dimension separatorThickness;
	int fieldShadowType;

	/* private data
	 */
	Widget		traversalGadget;  /* dummy for traversal */
	Widget		hSbar;		  /* horizontal ScrollBar */
	Widget		vSbar;		  /* vertical ScrollBar */
	Widget		inputWidget;	  /* input widget (BXmRegex or XmText)*/

	ColData*	coldata;	/* array of columns data */
	unsigned  	fieldHeight;
	int		horizHeaderHeight;
	int		staticVertiHeaderWidth;
	int		vertiHeaderWidth;
	unsigned  	tableWidth;
	unsigned  	tableHeight;
	unsigned  	visibleTableWidth;
	unsigned  	visibleTableHeight;
	Dimension 	preferredWidth;
	Dimension 	preferredHeight;
	int		wishedVisibleRows;

	int		hSbarValue;	  /* value of horizontal ScrollBar */
	int       	lastVisibleRow;
	int       	firstVisibleColumn;
	int		lastVisibleColumn;
	Boolean	  	internalTextModification;

	BXmTTData	data;

	_BXmRCTable	alignList;		/* internal interface */

	int 		regexListSize;
	int 		regexListCount;
	BXmTableString 	*gvRegexList;

	BXmTableString** actRegListP;
	int*		 actRegListCountP;
	String 		 currentRegex;
	
	_BXmRCTable	shadowList;	/* internal interface */
	_BXmRCTable	editableList;	/* internal interface */

	BXmTablePixel* bgList;		/* internal interface */
	int            bgListSize;
	int            bgListCount;
	BXmTablePixel* gvBgList;	/* synthetic value for XtGetValues() */
	BXmTablePixel* fgList;		/* internal interface */
	int            fgListSize;
	int            fgListCount;
	BXmTablePixel* gvFgList;	/* synthetic value for XtGetValues() */

	BXmTextTableGC* bgGCList;		/* background GCs */
	int          bgGCListSize;
	int          bgGCListCount;
	BXmTextTableGC* fgGCList;		/* foreground GCs */
	int          fgGCListSize;
	int          fgGCListCount;

	Pixmap       drawable;          /* drawable for GC creation */
	GC	     gc;
	GC	     tsGC[3];           /* topShadow GCs */
	GC	     bsGC[3];           /* bottomShadow GCs */
	GC	     hlGC[3];           /* highlight GCs */
	GC	     selBgGC[3];
	GC	     selFgGC[3];
	_BXmFontInfo* fonts;		/* font retrieved from fontList */
	int 	     fontsCount;	
	_BXmRCTable  fontnameList;	/* list of quarkified font names */
	_BXmRCTable  fontstructList;	/* list of XFontStruct's */
	int	     ascent;
	int	     descent;
	Pixmap	     stipple;		/* insensitive Bitmap */

	XRectangle   cliprects[3];
	GC	     tmpGC;		/* temporary GC for column clipping */
	Boolean      hasResizeCursor;
	Cursor	     resizeCursor;

	int          selectedPosSize;
	int	     copyIncrement;
        Dimension    oldWidth;
	Dimension    oldHeight;

	String*		gvTValue;	/* synthetic value for XtGetValues() */
	String*		gvHHValue;	/* synthetic value for XtGetValues() */
	String*		gvVHValue;	/* synthetic value for XtGetValues() */

	int		colUnit;

	Atom		textAtom;
	Atom		ctextAtom;
	int             dragRow, dragCol;
	int             dropRow, dropCol;

	Boolean		doSelect;
	int		doSelectFirstPos;
	int		doSelectPrevPos;
	int*		oldSelectedPosList;
	int 		oldSelectedPosCount;

	BXmTablePixmap* pixmapListRes;
	_BXmRCTable	pixmapList;
	_BXmTTPixmap*	pixmaps;
	int		pixmapsSize;

	/* HHeader as special widget
	 */
	Widget  hhwid;
	Widget  hhclip;
	Widget  vlinkWid;
	int*    vlinkWidths;
	Boolean autoLink;
	String  vlinkName;
} BXmTextTablePart;


/* whole instance type
 */
typedef struct _BXmTextTableRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	BXmTextTablePart	table;
} BXmTextTableRec;



/* type of new constraint part
 */
typedef struct _BXmTextTableConstraintsPart {
	XtPointer extension;    	/* makes compiler happier */
} BXmTextTableConstraintsPart;


/* whole constraints type
 */
typedef struct _BXmTextTableConstraintsRec {
	XmManagerConstraintPart	manager;
	BXmTextTableConstraintsPart	table;
} BXmTextTableConstraintsRec, *BXmTextTableConstraints;

#endif		 /* _BXmTextTableP_h */

