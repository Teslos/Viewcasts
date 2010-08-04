/********************************************************************
 *
 * Public header file of widget class  BXmHelpBox
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpBox.h	1.15  10/24/95  (Copyright 1993, BREDEX GmbH)"
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
#ifndef _BXmHelpBox_h
#define _BXmHelpBox_h

/* public header file of BXwidgets
 * but we need BXmHelpActionCallback definitions
 * so ...
 */
#ifndef _BXmHelpText_h
#include <BXm/HelpText.h>
#endif

/* public header file of parent widget class is part of Xm.h
 * (BXm.h includes it)
 */


/* variable for widget class
 */
extern WidgetClass bxmHelpBoxWidgetClass;

/* datatype for widget class
 */
typedef struct _BXmHelpBoxClassRec *	BXmHelpBoxWidgetClass;

/* datatype for widget instance
 */
typedef struct _BXmHelpBoxRec *		BXmHelpBoxWidget;


/* callback informations for CreateDialog-Callbacks
 */
typedef struct {
    int     reason;     /* reason for call
			   (BXmCR_CREATE_VIEW or BXmCR_CREATE_USAGE) */
    XEvent* event;      /* last event (always NULL) */
    Widget  helpDialog;	/* new help dialog widget */
} BXmHelpBoxCreateDialogCallbackStruct, *BXmHelpBoxCreateDialogPtr;


/* public widget specific functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget BXmCreateHelpBox	_PROTOS(( Widget, char*, Arg*, Cardinal ));
extern Widget BXmCreateHelpDialog _PROTOS(( Widget, char*, Arg*, Cardinal ));
extern Boolean BXmHelpBoxSetTopic _PROTOS(( Widget, String ));
extern Boolean BXmHelpBoxSetTopicByID _PROTOS(( Widget, int ));
extern Widget BXmHelpBoxGetChild _PROTOS(( Widget, int ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* _BXmHelpBox_h */

