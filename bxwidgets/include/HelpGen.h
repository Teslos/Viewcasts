/********************************************************************
 *
 * General header file of BXwidgets help system
 *
 ********************************************************************
 *
 * SccsId = "@(#)HelpGen.h	1.46  9/25/96  (Copyright 1993, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1993-1995 BREDEX GmbH, Braunschweig, Germany
 * ALL RIGHTS RESERVED
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND
 * COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE
 * OR ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY BREDEX GmbH.
 * 
 * BREDEX GmbH ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY
 * TO USE ANY OF ITS SOFTWARE. BXwidgets IS PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, AND BREDEX EXPRESSLY DISCLAIMS ALL IMPLIED
 * WARRANTIES, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
#ifndef _BXmHelpGen_h
#define _BXmHelpGen_h


/********************************************************************
 * Header Files
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/* bugfix for SUN machines 
 */
#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif


/********************************************************************
 * Constants
 ********************************************************************/

/* new major version breaks compatibility */
#define BXmHELP_VERSION		1
#define BXmHELP_REVISION	4

#define BXmHELP_POSTFIX		".bxo"

/* topic length for labels
 */
#define BXmTEXT_LEN_LABEL_OFFSET	20000

/* code constants
 * (nonprintable ascii char's to get best performance)
 *
 *  C_...:	users statements or fundamental codes
 *
 *  CX_...:	non-fundamental internal codes
 *		start with C_EXT
 */
#define C_NOP		 0
#define C_RESET		 1
#define C_BPARAM	 2
#define C_EPARAM	 3
#define C_TOPIC		 4
#define C_REF		 5
#define C_TEXTREF	 6
#define C_NOTEREF	 7
#define C_LABEL		 8
#define C_NEWWORD	 9
#define C_PAR		10
#define C_ROMAN		11
#define C_BOLD		12
#define C_SLANTED	13
#define C_TT		14
#define C_SMALL		15
#define C_NORMAL	16
#define C_LARGE		17
/*	18	*/
#define C_NEWLINE	19
#define C_TITLE		20
#define C_SUBTITLE	21
#define C_QUOTE		22
#define C_ENDQUOTE	23
#define C_VERB		24
#define C_ENDVERB	25
#define C_BITMAP	26
#define C_PIXMAP	27
#define C_ACTION	28
#define C_BREF		29
#define C_EREF		30
#define C_EXT		31
#define CX_IGNORE	32
#define CX_COMMENT	33
#define CX_VERSION	34
#define CX_TIME		35
#define CX_USER		36
#define CX_CHECKLEN	37
#define CX_CHECKSUM	38
#define CX_HEAD_START	39
#define CX_HEAD_END	40
#define CX_TOPICS_START	41
#define CX_TOPICS_END	42
#define CX_NUM_TOPICS	43
#define CX_TOPICS_POS	44
#define CX_NOTES_START	45
#define CX_NOTES_END	46
#define CX_NUM_NOTES	47
#define CX_NOTES_POS	48
#define CX_NOTE		49
#define CX_ITEM		50
#define CX_BACT		51
#define CX_EACT		52
#define CX_ENUM		53
#define CX_ENDENUM	54


#define MAX_NUM_PARAMS 2
#define MAX_PARAM_SIZE 256


/********************************************************************
 * Types, Global Variables, Functions
 ********************************************************************/

/* type for codes
 */
typedef struct _BXHelpCode {
    char*  name;
    int    code;
    int    params;
    int    endcode;
    char*  debug;
} BXHelpCode;


/* type for topics
 */
typedef struct _BXmHelpRefText {
    char*  name;	/* idName */
    int    nameLen;	/* strlen(idName) */
    int    checksum;	/* checksum to avoid strcmp() */
    long   filepos;	/* topic position in bxo file (0 if none) */
    int    orderpos;	/* logical order (for topic list) */
    char*  text;	/* name to display */
    int    textLen;	/* strlen(name-to-display) */
    int    isLabel;	/* true if label, false if topic */
} BXmHelpRefText;


#define NO_END_CODE	-1

/* global variables
 */
extern BXHelpCode*      bxHelpCodes;
extern int		bxHelpCodesSize;

/* global functions prototyping macro
 */
#ifndef _PROTOS
#ifdef _NO_PROTO
#define _PROTOS(args)   ()
#else
#define _PROTOS(args)   args
#endif
#endif

/* global functions
 */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int _BXmHelpProcessFileChecksum _PROTOS(( FILE*, long, long*, long* ));
int _BXmHelpProcessTopicChecksum _PROTOS(( char*, int* ));

/* some systems miss the following declarations
 */
extern int toupper _PROTOS(( int ));
extern int tolower _PROTOS(( int ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /*_BXmHelpGen_h*/

