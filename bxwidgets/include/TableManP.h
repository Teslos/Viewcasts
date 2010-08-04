/********************************************************************
 *
 * Private header file of widget class  BXmTableManager
 *
 ********************************************************************
 *
 * SccsId = "@(#)TableManP.h	2.18  01/17/97  (Copyright 1992, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1992-1996 BREDEX GmbH, Braunschweig, Germany
 * ALL RIGHTS RESERVED
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND
 * COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE
 * OR ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY BREDEX GmbH.
 * 
 * BREDEX GmbH ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY
 * TO USE ANY OF ITS SOFTWARE. BXwidgets IS PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, AND BREDEX EXPRESSLY DISCLAIMS ALL IMPLIED
 * WARRANTIES, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
#ifndef _BXmTableManP_h
#define _BXmTableManP_h


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
#endif

/* public header file
 */
#include <BXm/TableMan.h>


/* type of new class part
 */
typedef struct _BXmTableManagerClassPart {
	XtPointer extension;		/* makes compiler happier */
} BXmTableManagerClassPart;


/* whole class type
 */
typedef struct _BXmTableManagerClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	BXmTableManagerClassPart	table_class;
} BXmTableManagerClassRec;


/* class variable
 */
extern BXmTableManagerClassRec bxmTableManagerClassRec;


/* type of new instance part
 */
typedef struct _BXmTableManagerPart {
	/* resources
	 */
	int numRows;			/* number of rows */
	int numColumns;			/* number of columns */
	int defaultNumColumns;
	int defaultGravity;
	Boolean recomputeLayout;
	int resizePolicy;
	Dimension marginWidth;
	Dimension marginHeight;
	Dimension horizSpacing;
	Dimension vertiSpacing;
	int* dynaRowList;		/* DYN */
	int* dynaColList;		/* DYN */
	int dynaRowCount;
	int dynaColCount;
	int dynamicPolicy;
	BXmTableManagerWidget hlink;
	BXmTableManagerWidget vlink;
	String hlinkName;		/* DYN */
	String vlinkName;		/* DYN */
	Boolean debug;

	/* private data
	 */
	Boolean didChangeManaged;
	int* rowHeight;			/* DYN */
	int rowHeightSize;
	int* colWidth;			/* DYN */
	int  colWidthSize;
	int* prefColWidth;		/* DYN */
	Widget tmLinkWid;
	int*   tmLinkWidth;			/* DYN */
	int    tmLinkFirstWidth;
	int    tmLinkNumWidth;
	Dimension preferredWidth;
	Dimension preferredHeight;
	int dynaWidth;
	int dynaHeight;
	int firstRow;
	int firstCol;
	int lastRow;
	int lastCol;
	int numDynaRows;		/* number of existing dynamic rows */
	int numDynaCols;		/* number of existing dynamic columns */
	BXmTableManagerWidget hlinkBack;
	BXmTableManagerWidget vlinkBack;
	int dynaLayoutWidth;
	int dynaLayoutHeight;
	int mustCorrectGravity;		/* 0, ROW and/or COLUMN */
} BXmTableManagerPart;


/* whole instance type
 */
typedef struct _BXmTableManagerRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	BXmTableManagerPart	table;
} BXmTableManagerRec;



/* type of new constraint part
 */
typedef struct _BXmTableManagerConstraintsPart {
	/* resources
	 */
	int row;		  /* row of child widget */
	int column;		  /* column of child widget */
	int numRows;		  /* number of rows of child widget */
	int numColumns;		  /* number of columns of child widget */
	int gravity;		  /* gravity of child widget */
	int leftOffset;		  /* additional space on the left */
	int rightOffset;	  /* additional space on the right */
	int topOffset;		  /* additional space on the top */
	int bottomOffset;	  /* additional space on the bottom */

	/* private data
	 */
	int processed_x;	    /* internal processed x coord */
	int processed_y;	    /* internal processed y coord */
	int processed_width;  	    /* internal processed width */
	int processed_height;	    /* internal processed height */
	int       wished_width;     /* saved wished width */
	int       wished_height;    /* saved wished height */
	Boolean	  newGeoFromCSV;    /* geometry set from ConstraintSetValues */
	int       prevDynaRows;	    /* previous dynamic rows */
	int       prevDynaCols;	    /* previous dynamic columns */
	int	  numDynaRows;	    /* number of dynamic rows */
	int	  numDynaCols;	    /* number of dynamic columns */
	int raw_x;	    	/* internal x coord before gravity correction */
	int raw_y;	    	/* internal y coord before gravity correction */
	int raw_width;		/* internal width before gravity correction */
	int raw_height;	    	/* internal height before gravity correction */
} BXmTableManagerConstraintsPart;


/* whole constraints type
 */
typedef struct _BXmTableManagerConstraintsRec {
	XmManagerConstraintPart		manager;
	BXmTableManagerConstraintsPart	table;
} BXmTableManagerConstraintsRec, *BXmTableManagerConstraints;


#endif	/* _BXmTableManP_h */

