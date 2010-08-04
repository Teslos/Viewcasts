/********************************************************************
 *
 * Private header file of widget class  BXmCenter
 *
 ********************************************************************
 *
 * SccsId = "@(#)CenterP.h	1.3  1/13/94  (Copyright 1993, BREDEX GmbH)"
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
#ifndef _BXmCenterP_h
#define _BXmCenterP_h


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
#include <BXm/Center.h>


/* type of new class part
 */
typedef struct _BXmCenterClassPart {
	XtPointer extension;		/* makes compiler happier */
} BXmCenterClassPart;


/* whole class type
 */
typedef struct _BXmCenterClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	BXmCenterClassPart	center_class;
} BXmCenterClassRec;


/* class variable
 */
extern BXmCenterClassRec bxmCenterClassRec;


/* type of new instance part
 */
typedef struct _BXmCenterPart {
	/* resources
	 */
	int resizePolicy;

	/* private data
	 */
	Widget managedChild;
	Dimension wishedChildWidth;
	Dimension wishedChildHeight;
	Dimension wishedChildBorderWidth;
} BXmCenterPart;


/* whole instance type
 */
typedef struct _BXmCenterRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	BXmCenterPart	center;
} BXmCenterRec;



/* type of new constraint part
 */
typedef struct _BXmCenterConstraintsPart {
	XtPointer extension;            /* makes compiler happier */
} BXmCenterConstraintsPart;


/* whole constraints type
 */
typedef struct _BXmCenterConstraintsRec {
	XmManagerConstraintPart		manager;
	BXmCenterConstraintsPart	center;
} BXmCenterConstraintsRec, *BXmCenterConstraints;


#endif	/* _BXmCenterP_h */

