/********************************************************************
 *
 * Public header file of widget class  BXmHelpText
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpText.h	1.22  02/10/97  (Copyright 1993, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1993-1997 BREDEX GmbH, Braunschweig, Germany
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
#ifndef _BXmHelpText_h
#define _BXmHelpText_h

/* public header file of parent widget class XmPrimitive
 * is part of Xm.h
 */
#include <Xm/Xm.h>

/* public header file of BXwidgets
 */
#include <BXm/BXm.h>


/* variable for widget class
 */
extern WidgetClass bxmHelpTextWidgetClass;

/* datatype for widget class
 */
typedef struct _BXmHelpTextClassRec *	BXmHelpTextWidgetClass;

/* datatype for widget instance
 */
typedef struct _BXmHelpTextRec *	BXmHelpTextWidget;


/* callback informations for SelectRef- and NewTopic-Callbacks
 */
typedef struct {
    int     reason;     /* reason for call */
    XEvent* event;	/* always NULL */
    String  topic;	/* topic name */
    int     topicID;	/* topic id to use with BXmHelpTextSetTopicByID() */
    String  label;      /* label in the topic or NULL */
    int     labelID;    /* label id or BXmNO_TOPIC_ID */
} BXmHelpTextCallbackStruct, *BXmHelpTextPtr;

/* callback informations for Action-Callbacks
 *  - note: also used from BXmHelpBox
 */
typedef struct {
    int     reason;     /* reason for call (BXmCR_ACTION) */
    XEvent* event;	/* last button event */
    String  actionname;	/* action name */
} BXmHelpActionCallbackStruct, *BXmHelpActionPtr;


/* public widget specific functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget  BXmCreateHelpText _PROTOS(( Widget, char*, Arg*, Cardinal ));
extern Boolean BXmHelpTextSetTopic _PROTOS(( Widget, String ));
extern Boolean BXmHelpTextSetTopicByID _PROTOS(( Widget, int ));
extern String  BXmHelpTextTopicToTitle _PROTOS(( Widget, String ));
extern int     BXmHelpTextGetPosFromLabelID _PROTOS(( Widget, int ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* _BXmHelpText_h */

