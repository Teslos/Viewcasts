/********************************************************************
 *
 * Private header file of widget class  BXmHelpShell
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpShellP.h	1.9  12/6/94  (Copyright 1993, BREDEX GmbH)"
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
#ifndef _BXmHelpShellP_h
#define _BXmHelpShellP_h


/* private header file of parent widget class */
#include <X11/ShellP.h>

/* public header file */
#include <BXm/HelpShell.h>


/* type of new class part
 */
typedef struct _BXmHelpShellClassPart {
	XtPointer extension;		/* makes compiler happier */
} BXmHelpShellClassPart;


/* whole class type
 */
typedef struct _BXmHelpShellClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ShellClassPart		shell_class;
	WMShellClassPart	wm_shell_class;
	VendorShellClassPart	vendor_shell_class;
	TopLevelShellClassPart	top_level_shell_class;
	BXmHelpShellClassPart	help_shell_class;
} BXmHelpShellClassRec;


/* class variable
 */
extern BXmHelpShellClassRec bxmHelpShellClassRec;


/* type of new instance part
 */
typedef struct _BXmHelpShellPart {
	/* resources
	 */
	unsigned char toSeeIfDeleteResponseSet;

	/* private data
	 */
	Widget managedChild;
	Widget unmanageFromUnmapChild;
	int directWishedWidth;
	int directWishedHeight;
} BXmHelpShellPart;


/* whole instance type
 */
typedef struct _BXmHelpShellRec {
	CorePart	core;
	CompositePart   composite;
	ShellPart       shell;
	WMShellPart     wm;
	VendorShellPart vendor;
	TopLevelShellPart topLevel;
	BXmHelpShellPart help;
} BXmHelpShellRec;


#endif	/* _BXmHelpShellP_h */

