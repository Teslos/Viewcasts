/********************************************************************
 *
 * Private header file of widget class  BXmScrollManager
 *
 ********************************************************************
 *
 * SccsId = "@(#)ScrollManP.h	1.6  12/23/94  (Copyright 1994, BREDEX GmbH)"
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
#ifndef _BXmScrollManP_h
#define _BXmScrollManP_h


/* private header file of parent widget class */
#include <Xm/ScrolledWP.h>

/* public header file
 */
#include <BXm/ScrollMan.h>


/* type of new class part
 */
typedef struct _BXmScrollManagerClassPart {
	XtPointer extension;		/* makes compiler happier */
} BXmScrollManagerClassPart;


/* whole class type
 */
typedef struct _BXmScrollManagerClassRec {
	CoreClassPart		  core_class;
	CompositeClassPart	  composite_class;
	ConstraintClassPart	  constraint_class;
	XmManagerClassPart	  manager_class;
	XmScrolledWindowClassPart swindow_class;
	BXmScrollManagerClassPart sman_class;
} BXmScrollManagerClassRec;


/* class variable
 */
extern BXmScrollManagerClassRec bxmScrollManagerClassRec;


/* type of new instance part
 */
typedef struct _BXmScrollManagerPart {
	/* resources
	 */
	Widget hheader;
	Widget vheader;
	Boolean autoLink;
	Boolean recomputeLayout;
	Dimension headerSpacing;
	Dimension marginWidth;
	Dimension marginHeight;

	/* private data
	 */
	Widget work;
	Widget hhClip;
	Widget vhClip;
	Boolean inInit;
	int preferredWidth;
	int preferredHeight;
	Widget hlink;
	Widget vlink;

} BXmScrollManagerPart;


/* whole instance type
 */
typedef struct _BXmScrollManagerRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmScrolledWindowPart	swindow;
	BXmScrollManagerPart	sman;
} BXmScrollManagerRec;


#endif	/* _BXmScrollManP_h */

