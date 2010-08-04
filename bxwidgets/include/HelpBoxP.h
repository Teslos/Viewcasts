/********************************************************************
 *
 * Private header file of widget class  BXmHelpBox
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpBoxP.h	1.23  01/21/97  (Copyright 1993, BREDEX GmbH)"
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
#ifndef _BXmHelpBoxP_h
#define _BXmHelpBoxP_h


/* private header file of parent widget class
 * (in motif1.1 it is part of XmP.h)
 * ATTENTION: we must include Xm.h to see what private header
 *            files we need
 */
#include <Xm/Xm.h>
#if XmVersion == 1001
#include <Xm/XmP.h>
#else
#include <Xm/ManagerP.h>
#endif

/* public header file
 */
#include <BXm/HelpBox.h>

/* widget constants
 *  - must begin with buttons
 */
#define BX_BACK		 0
#define BX_GOTO		 1
#define BX_VIEW		 2
#define BX_PRINT	 3
#define BX_CLOSE	 4

#define BX_FILE		 5
#define BX_SEARCH	 6
#define BX_TOPICS	 7
#define BX_HELP		 8
#define BX_USAGE	 9
#define BX_OK		10
#define BX_APPLY	11
#define BX_CANCEL	12
#define BX_PRINT_FILE	13
#define BX_PRINT_COMMAND 14

#define BX_BUTTONS	 5
#define BX_LABELS	15

/* child widget index in print dialog
 */
#define PR_FTB		0
#define PR_FI		1
#define PR_CTB		2
#define PR_CI		3

typedef void (*BXmHelpBoxNewHelpTextPrefSize) _PROTOS(( Widget ));

/* type of new class part
 */
typedef struct _BXmHelpBoxClassPart {
	BXmHelpBoxNewHelpTextPrefSize helptextPrefSize;
	XtPointer extension;		/* makes compiler happier */
} BXmHelpBoxClassPart;


/* whole class type
 */
typedef struct _BXmHelpBoxClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	BXmHelpBoxClassPart	helpbox_class;
} BXmHelpBoxClassRec;


/* class variable
 */
extern BXmHelpBoxClassRec bxmHelpBoxClassRec;


/* new typ for oldTopicsList
 */
typedef struct _BXmHelpBoxOldTopicsRec {
	int	topicID;	/* topic ID */
	int	scrollBarPos;	/* relative position of scroll bar value */
} BXmHelpBoxOldTopicsRec;

/* type of new instance part
 */
typedef struct _BXmHelpBoxPart {
    /* resources
     *  - DYN: dynamic resources which have to get freed()
     *  - SYN: interface to resources of child widgets
     */
    String helpfile;			/* SYN: helptext: helpfile */
    String topic;			/* SYN: helptext: topic */
    XmFontList helpFontList;		/* SYN: helptext: fontList */
    XtCallbackList closeCallback;	/* close callbacks */
    XmString label[BX_LABELS];		/* SYN: various labels: labelString */
    String dialogTitle;			/* SYN: helpshell: title */
    String topicsDialogTitle;		/* SYN: topicsDialog: title */
    int resizePolicy;			/* resize policy */
    String usageTopic;		/* DYN: topic for usage button or NULL */
    XtCallbackList createDialogCallback;  /* view or usage dialog created */
    XtCallbackList actionCallback;      /* hook to help text action callback */
    String printFile;
    String printCommand;
    String printDialogTitle;

    /* private data
     */
    Widget sw;				/* child[1]: scrolled window */
     Widget clip;			/*  - clip window */
     Widget vsb;			/*  - vertical scrollbar */
     Widget helptext;			/*  - help text */
    Widget bbox;			/* child[2]: buttons box */
     Widget buttons[BX_BUTTONS];	/*  - buttons */
    Widget topicsDialog;		/* child[3]: selection box */

    Widget mbar;			/* child[0]: menu bar */
     Widget file;			/*  - file menubutton */
      Widget fbuttons[BX_BUTTONS];	/*     - buutons therein */
     Widget search;			/*  - search menubutton */
      Widget topics;			/*     - topics button therein */
     Widget helpButton;			/*  - help menubutton */
      Widget usageW;			/*     - usage button therein */
       Widget usageDialog;		/*        - usage dialog therefore */

    Widget viewDialog;			/* dialog widget for view button */
    Widget helpShell;			/* parent if BXmHelpShell or NULL */
    Widget printDialog;			/* dialog for printing */
     Widget printDW[4];

    String* topicsList;		/* list of topics (helpbox: topicsList) */
    int topicsCount;		/* number of topics (helpbox: topicsCount) */
    XmString* topicsTitleList;	/* DYN: list of topic titles */
    int* topicsTitlePosList;	/* DYN: list of topic titles index */

    int actTopicID;		/* actual topic ID or BXmNO_TOPIC_ID */
    int selectedTopicID;	/* selected topic ID or BXmNO_TOPIC_ID */
    int selectedLabelID;	/* selected topic ID or BXmNO_TOPIC_ID */
    BXmHelpBoxOldTopicsRec* oldTopicsList;
				/* DYN: list of old previous topics */
    int oldTopicsListSize;	/* size of oldTopicsList */
    int oldTopicsListCount;	/* number of previous topics */
    Boolean newTopicFromBackButton;	/* flag for oldTopicsList handling */

    int preferredWidth;		/* preferred width */
    int preferredHeight;	/* preferred height */
    int wishedWidth;		/* user wished width */
    int wishedHeight;		/* user wished height */
    int mbarHeight;		/* total height of menu bar */
    int bboxHeight;		/* total height of buttons box */
    Boolean firstChangeManagedCall;	/* for first layout processing */
    Boolean inSetValuesOrInitialize;	/* we are in SetValues() */
    int newTopicLabelID;
} BXmHelpBoxPart;


/* whole instance type
 */
typedef struct _BXmHelpBoxRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	BXmHelpBoxPart		helpbox;
} BXmHelpBoxRec;


#endif	/* _BXmHelpBoxP_h */

