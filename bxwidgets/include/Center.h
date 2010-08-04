/********************************************************************
 *
 * Public header file of widget class  BXmCenter
 *
 ********************************************************************
 *
 * SccsId = "@(#)Center.h	1.3  1/13/94  (Copyright 1993, BREDEX GmbH)"
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
#ifndef _BXmCenter_h
#define _BXmCenter_h

/* public header file of BXwidgets */
#include <BXm/BXm.h>

/* public header file of parent widget class is part of Xm.h
 * (BXm.h includes it)
 */


/* variable for widget class
 */
extern WidgetClass bxmCenterWidgetClass;

/* datatype for widget class
 */
typedef struct _BXmCenterClassRec *	BXmCenterWidgetClass;

/* datatype for widget instance
 */
typedef struct _BXmCenterRec *		BXmCenterWidget;


/* public widget specific functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget BXmCreateCenter _PROTOS ((Widget, char*, Arg*, Cardinal));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif      /* _BXmCenter_h */

