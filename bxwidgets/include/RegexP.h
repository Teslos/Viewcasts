/********************************************************************
 *
 * Private header file of widget class  BXmRegex
 *
 ********************************************************************
 *
 * SccsId = "@(#)RegexP.h	2.14  09/24/96  (Copyright 1992, BREDEX GmbH)"
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
#ifndef _BXmRegexP_h
#define _BXmRegexP_h


/* private header file of parent widget class */
#include <Xm/TextFP.h>

/* public header file */
#include <BXm/Regex.h>

/* header file for regex processing */
#include <BXm/RegexI.h>

typedef struct BXmREG_re_pattern_buffer BXmRegexData;

/* forward declarations
 *  - BXmRegex regex management
 */
void    _BXmRegexInitRegexData _PROTOS(( BXmRegexData* ));
Boolean _BXmRegexCompileRegex _PROTOS(( String, BXmRegexData* ));
void    _BXmRegexFreeRegexData _PROTOS(( BXmRegexData* ));
void    _BXmRegexSetActRegexData _PROTOS(( BXmRegexWidget, BXmRegexData* ));
Boolean _BXmRegexCheckAndCopyNewRegex _PROTOS(( String*, String*, BXmRegexData*, Widget ));


/* type of new class part
 */
typedef struct _BXmRegexClassPart {
    XtPointer extension;		/* makes compiler happier */
} BXmRegexClassPart;


/* whole class type
 */
typedef struct _BXmRegexClassRec {
    CoreClassPart		core_class;
    XmPrimitiveClassPart	primitive_class;
    XmTextFieldClassPart	text_class;
    BXmRegexClassPart		regex_class;
} BXmRegexClassRec;


/* class variable
 */
extern BXmRegexClassRec bxmRegexClassRec;


/* type of new instance part
 */
typedef struct _BXmRegexPart {
   /* resources
    *  - "DYN": own dynamic storage
    */
   String regex;		/* DYN: regular expression */
   int	  matchState;           /* matching state */
   int	  matchToCursorState;	/* matching state until cursor */
   String nextWidgetName;	/* DYN: name of next travers. widget */
   String prevWidgetName;	/* DYN: name of previous trav. widget */
   int 	  autoTraversalPolicy;	/* policy for automatic traversal */
   BXmConversionPolicyProc convPolicyProc; /* policy for character conversion */

   /* private data
    *  - "DYN": own dynamic storage
    */
   BXmRegexData  regexData;	/* DYN: regex buffer data */
   BXmRegexData* actRegexData;	/* pointer to actual regex buffer data */
   int	   wouldMatchState;	/* for modify verify callbacks */
   int	   wouldMatchToCursorState;  /* for modify verify callbacks */
   String  regexCopy;		/* pointer copy to see a ressource change */
   String  valueCopy;		/* pointer copy to see a change
				    - BUGFIX for motif (see Initialize())
				    - DYN ifndef FAST_ACCESS */
   Widget  shell;		/* shell widget for traversal */
   XrmName nextWidgetQuark;	/* nextWidgetName as quark */
   XrmName prevWidgetQuark;	/* prevWidgetName as quark */
} BXmRegexPart;


/* whole instance type
 */
typedef struct _BXmRegexRec {
    CorePart	core;
    XmPrimitivePart	primitive;
    XmTextFieldPart	text;
    BXmRegexPart	regex;
} BXmRegexRec;


#endif	/* _BXmRegexP_h */

