/********************************************************************
 *
 * Public header file of widget class  BXmScrollManager
 *
 ********************************************************************
 *
 * SccsId = "@(#)ScrollMan.h	1.5  01/17/97  (Copyright 1994, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1994 BREDEX GmbH, Braunschweig, Germany
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
#ifndef _BXmScrollMan_h
#define _BXmScrollMan_h

/* public header file of BXwidgets */
#include <BXm/BXm.h>

/* public header file of parent widget class */
#include <Xm/ScrolledW.h>


/* variable for widget class
 */
extern WidgetClass bxmScrollManagerWidgetClass;

/* datatype for widget class
 */
typedef struct _BXmScrollManagerClassRec *	BXmScrollManagerWidgetClass;

/* datatype for widget instance
 */
typedef struct _BXmScrollManagerRec *		BXmScrollManagerWidget;


/* public widget specific functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget BXmCreateScrollManager _PROTOS(( Widget, char*, Arg*, Cardinal ));
extern void BXmScrollManagerSetAreas _PROTOS(( Widget, Widget, Widget, Widget, Widget, Widget ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#ifndef BXmNhorizontalHeaderWindow
#define BXmNhorizontalHeaderWindow BXmNhorizontalHeaderWidget
#endif
#ifndef BXmNverticalHeaderWindow
#define BXmNverticalHeaderWindow BXmNverticalHeaderWidget
#endif
#ifndef BXmCHeaderWindow
#define BXmCHeaderWindow BXmCHeaderWidget
#endif

#endif      /* _BXmScrollMan_h */

