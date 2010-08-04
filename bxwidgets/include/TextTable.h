/********************************************************************
 *
 * Public header file of widget class BXmTextTable
 *
 ********************************************************************
 *
 * SccsId = "@(#)TextTable.h	2.27  02/12/97  (Copyright 1992, BREDEX GmbH)"
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
#ifndef _BXmTextTable_h
#define _BXmTextTable_h

/* motif header files
 * (public header file of parent widget class is part of Xm.h
 *  and that gets included by Text.h)
 */

#include <Xm/Text.h>
#include <BXm/Regex.h>
#include <Xm/ScrollBar.h>

/* public header file of BXwidgets
 */
#include <BXm/BXm.h>


/* variable for widget class
 */
extern WidgetClass bxmTextTableWidgetClass;

/* datatype for widget class
 */
typedef struct _BXmTextTableClassRec *	BXmTextTableWidgetClass;

/* datatype for widget instance
 */
typedef struct _BXmTextTableRec *		BXmTextTableWidget;


/* callback informations for Activate-Callbacks
 */
typedef struct {
    /* BXmRegex fields */
    int		reason;		/* reason for call */
    XEvent*	event;		/* last X event or NULL */
    int 	matchState;	/* match status */
    /* BXmTextTable fields */
    int		currentRow;	/* row of current field */
    int		currentColumn;	/* column of current field */
    String	currentValue;	/* current field value */
    String	newValue;	/* new field value (copy) */
    Boolean	valueChanged;	/* field value changed ? */
    int         wishedRow;      /* wished row of next field */
    int         wishedColumn;   /* wished column of next field */
    Boolean	doit;		/* do the change ? (R/W) */
    int		nextRow;	/* row of next field (R/W) */
    int		nextColumn;	/* column of next field (R/W) */
    Boolean	editing;	/* editing current field (R/W) */
} BXmTextTableActivateCallbackStruct, *BXmTextTableActivatePtr;

/* callback informations for Modify-Verify-Callbacks
 */
typedef struct {
    /* BXmRegex fields */
    int			reason;		/* reason for call */
    XEvent*		event;		/* last X event or NULL */
    Boolean		doit;		/* see XmTextField */
    XmTextPosition	currInsert;	/* see XmTextField */
    XmTextPosition	newInsert;	/* see XmTextField */
    XmTextPosition	startPos;	/* see XmTextField */
    XmTextPosition	endPos;		/* see XmTextField */
    XmTextBlock		text;		/* see XmTextField */
    /* BXmTextTable-Fields */
    int			currentRow;	/* row of field */
    int			currentColumn;  /* column of field */
} BXmTextTableVerifyCallbackStruct, *BXmTextTableVerifyPtr;

/* callback informations for ChangePosition-Callbacks
 */
typedef struct {
    int	    reason;		/* reason for call */
    XEvent* event;		/* last X event or NULL */
    int	    previousRow;	/* row of old current field */
    int	    previousColumn;	/* column of old current field */
    Boolean previousEditing;	/* was current field edited ? */
    int	    nextRow;		/* row of next current field (R/W) */
    int	    nextColumn;		/* column of next current field (R/W) */
    Boolean nextEditing;	/* edit next current field ? (R/W) */
} BXmTextTableChangePositionCallbackStruct, *BXmTextTableChangePositionPtr;

/* callback informations for PreEdit-Callbacks
 */
typedef struct {
    int	    reason;		/* reason for call */
    XEvent* event;		/* last X event or NULL */
    int	    currentRow;		/* row of field */
    int	    currentColumn;	/* column of field */
    Boolean doit;		/* really edit ? (R/W) */
    Widget  inputWidget;	/* input widget */
} BXmTextTablePreEditCallbackStruct, *BXmTextTablePreEditPtr;

/* callback informations for PostEdit-Callbacks
 */
typedef struct {
    int	    reason;		/* reason for call */
    XEvent* event;		/* last X event or NULL */
    int	    currentRow;		/* row of field */
    int	    currentColumn;	/* column of field */
} BXmTextTablePostEditCallbackStruct, *BXmTextTablePostEditPtr;

/* callback informations for Selection-Callbacks
 */
typedef struct {
    int	    	reason;		  /* reason for call */
    XEvent* 	event;		  /* last X event or NULL */
    int		pos;		  /* selected position */
    Boolean	selected;	  /* is field now selected ? */
    Boolean	wasSelected;	  /* was field previously selected ? */
    int*	selectedPosList;  /* like resource (original) */
    int		selectedPosCount; /* like resource */
} BXmTextTableSelectionCallbackStruct, *BXmTextTableSelectionPtr;


/* public widget specific functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget  BXmCreateTextTable _PROTOS(( Widget, char*, Arg*, Cardinal ));
extern void    BXmTextTableSetNumRows _PROTOS(( Widget, int ));
extern void    BXmTextTableSetNumColumns _PROTOS(( Widget, int ));
extern Boolean BXmTextTableSetField _PROTOS(( Widget, int, int, String ));
extern Boolean BXmTextTableSetRow _PROTOS(( Widget, int, String, String* ));
extern Boolean BXmTextTableSetColumn _PROTOS(( Widget, int, String, String* ));
extern void    BXmTextTableSetTableValue _PROTOS(( Widget, String* ));
extern Boolean BXmTextTableGetField _PROTOS(( Widget, int, int, Boolean, String* ));
extern Boolean BXmTextTableGetRow _PROTOS(( Widget, int, Boolean, int*, String*, String** ));
extern Boolean BXmTextTableGetColumn _PROTOS(( Widget, int, Boolean, int*, String*, String** ));
extern void    BXmTextTableGetTableValue _PROTOS(( Widget, Boolean, int*, int*, String** ));
extern Boolean BXmTextTableInsertRow _PROTOS(( Widget, int , String, String*));
extern Boolean BXmTextTableDeleteRow _PROTOS(( Widget, int ));
extern Boolean BXmTextTableInsertColumn _PROTOS(( Widget, int, String, String*));
extern Boolean BXmTextTableDeleteColumn _PROTOS(( Widget, int ));
extern Boolean BXmTextTableChangeCurrentField _PROTOS(( Widget, int, int, Boolean ));
extern Widget  BXmTextTableGetChild _PROTOS(( Widget, int ));
extern Boolean BXmTextTableSetAlignment _PROTOS(( Widget, int, int, int ));
extern Boolean BXmTextTableSetEditable _PROTOS(( Widget, int, int, Boolean ));
extern Boolean BXmTextTableSetFontname _PROTOS(( Widget, int, int, String ));
extern Boolean BXmTextTableSetPixmap _PROTOS(( Widget, int, int, Pixmap, String ));
extern Boolean BXmTextTableSetRegex _PROTOS(( Widget, int, int, String ));
extern Boolean BXmTextTableSetShadow _PROTOS(( Widget, int, int, Boolean ));
extern Boolean BXmTextTableSetBackground _PROTOS(( Widget, int, int, Pixel ));
extern Boolean BXmTextTableSetForeground _PROTOS(( Widget, int, int, Pixel ));
extern Boolean BXmTextTableSetBackgroundColor _PROTOS(( Widget, int, int, String ));
extern Boolean BXmTextTableSetForegroundColor _PROTOS(( Widget, int, int, String ));
extern Boolean BXmTextTableSelect _PROTOS(( Widget, int, Boolean ));
extern Boolean BXmTextTableDeselect _PROTOS(( Widget, int, Boolean ));
extern Boolean BXmTextTableSelectAll _PROTOS(( Widget ));
extern void BXmTextTableDeselectAll _PROTOS(( Widget ));
extern Boolean BXmTextTablePosIsSelected _PROTOS(( Widget, int ));
extern Boolean BXmTextTableXYToField _PROTOS(( Widget, Position, Position, int*, int* ));
extern Boolean BXmTextTableFieldToBounds _PROTOS(( Widget, int, int, Position*, Position*, Dimension*, Dimension* ));
extern Boolean BXmTextTableSetColumnsMinWidth _PROTOS(( Widget, int, int));
extern Boolean BXmTextTableSetColumnsMaxWidth _PROTOS(( Widget, int, int));
extern Boolean BXmTextTableSetRecomputeLayout _PROTOS(( Widget, Boolean));
extern Boolean BXmTextTableScrollVisible _PROTOS(( Widget, int, int));

/* type for field informations
 */
typedef struct {
    String  value;		/* value of field (no copy) */
    String  regex;		/* format of field (no copy) */
    Boolean isSelected;		/* is field currently selected ? */
    Pixel   background;		/* actual background color */
    Pixel   foreground;		/* actual foreground color */
    Pixel   fieldBackground;	/* background color if not selected */
    Pixel   fieldForeground;	/* foreground color if not selected */
    Boolean editable;		/* is field currently editable ? */
    Boolean fieldEditable;	/* is field editable due to editableList ? */
    int     alignment;		/* field alignment */
    Boolean hasShadow;		/* does field has shadow ? */
    String  fontname;		/* symbolic font name or NULL */
    XFontStruct* font;		/* actual font */
    Pixmap  pixmap;		/* field pixmap or BXmNO_PIXMAP */
    String  pixmapname;		/* name of the pixmap or NULL */
} BXmTextTableFieldDataStruct, *BXmTextTableFieldDataPtr;

extern BXmTextTableFieldDataPtr BXmTextTableGetFieldData _PROTOS(( Widget, int, int ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/* for backward compatibility
 */
#define BXmTextTableChangeCurrentPosition BXmTextTableChangeCurrentField

#endif      /* _BXmTextTable_h */

