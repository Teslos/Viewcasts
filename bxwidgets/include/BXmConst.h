/********************************************************************
 *
 * Public header file of BXwidgets for constants
 *
 ********************************************************************
 *
 * SccsId = "@(#)BXmConst.h	2.103  01/23/97  (Copyright 1993, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1992-1996 BREDEX GmbH, Braunschweig, Germany
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
#ifndef _BXmConst_h
#define _BXmConst_h

/* version constants
 */
#define BXmVERSION		(1)
#define BXmREVISION		(5)
#define BXmUPDATE_LEVEL		(4)

/******************************************************************** 
 * NOTE: for constants defined with motif constants/enums
 *       we might have no proper definition in D
 *	 NOTE: in TeleUSE3.0 we can include <teleuse/Xm_D.h>
 ********************************************************************/
#ifndef _Xm_h
#ifndef _Xm_D_
#define _BXmUSE_LITERALS
#endif
#endif

/* position constants
 */
#define BXmALL			(-1)
#define BXmLAST_POSITION	(-1)
#define BXmCURRENT_POSITION	(-2)

/* direction constants
 */
#define BXmCURRENT		(1)
#define BXmLEFT			(2)
#define BXmRIGHT		(3)
#define BXmUP			(4)
#define BXmDOWN			(5)

/* child constants
 */
#define BXmCHILD_NONE			(0)
#define BXmCHILD_HELP_BUTTON		(7)
#define BXmCHILD_TEXT			(13)
#define BXmCHILD_INPUT			(13)
#define BXmCHILD_HORIZONTAL_SCROLLBAR	(51)
#define BXmCHILD_VERTICAL_SCROLLBAR	(52)
#define BXmCHILD_HELP_TEXT		(5001)
#define BXmCHILD_BACK_BUTTON		(5002)
#define BXmCHILD_GOTO_BUTTON		(5003)
#define BXmCHILD_VIEW_BUTTON		(5004)
#define BXmCHILD_CLOSE_BUTTON		(5005)
#define BXmCHILD_FILE_BUTTON		(5006)
#define BXmCHILD_BACK_MENU_BUTTON	(5007)
#define BXmCHILD_GOTO_MENU_BUTTON	(5008)
#define BXmCHILD_VIEW_MENU_BUTTON	(5009)
#define BXmCHILD_CLOSE_MENU_BUTTON	(5010)
#define BXmCHILD_SEARCH_BUTTON		(5011)
#define BXmCHILD_TOPICS_BUTTON		(5012)
#define BXmCHILD_USAGE_BUTTON		(5014)
#define BXmCHILD_TOPICS_DIALOG		(5015)
#define BXmCHILD_PRINT_DIALOG		(5016)
#define BXmCHILD_PRINT_BUTTON		(5017)
#define BXmCHILD_PRINT_MENU_BUTTON	(5018)

/* alignment constants
 */
#ifndef _BXmUSE_LITERALS
#define BXmALIGN_LEFT		XmALIGNMENT_BEGINNING
#define BXmALIGN_CENTER		XmALIGNMENT_CENTER
#define BXmALIGN_RIGHT		XmALIGNMENT_END
#else
#define BXmALIGN_LEFT		(0)
#define BXmALIGN_CENTER		(1)
#define BXmALIGN_RIGHT		(2)
#endif

/* orientation constants
 */
#ifndef _BXmUSE_LITERALS
#define BXmVERTICAL		XmVERTICAL
#define BXmHORIZONTAL		XmHORIZONTAL
#else
#define BXmVERTICAL		(1)
#define BXmHORIZONTAL		(2)
#endif

/* selectionPolicy constants
 */
#define BXmNO_SELECT		(-1)
#ifndef _BXmUSE_LITERALS
#define BXmSINGLE_SELECT	XmSINGLE_SELECT
#define BXmMULTIPLE_SELECT	XmMULTIPLE_SELECT
#define BXmEXTENDED_SELECT	XmEXTENDED_SELECT
#else
#define BXmSINGLE_SELECT	(0)
#define BXmMULTIPLE_SELECT	(1)
#define BXmEXTENDED_SELECT	(2)
#endif

/* resizePolicy constants
 */
#ifndef _BXmUSE_LITERALS
#define BXmRESIZE_NONE		XmRESIZE_NONE
#define BXmRESIZE_GROW		XmRESIZE_GROW
#define BXmRESIZE_ANY		XmRESIZE_ANY
#else
#define BXmRESIZE_NONE		(0)
#define BXmRESIZE_GROW		(1)
#define BXmRESIZE_ANY		(2)
#endif

/* dropPolicy constants
 */
#define BXmDROP_NOOP		(0)
#define BXmDROP_REPLACE		(1)

/* dynamicPolicy constants
 */
#define BXmDYNAMIC_GROW		BXmRESIZE_NONE
#define BXmSTATIC_GROW		BXmRESIZE_GROW
#define BXmANY_SIZE		BXmRESIZE_ANY

/* gravity constants
 */
#define BXmDEFAULT_GRAVITY	(0)
#define BXmCENTER		(1)
#define BXmNORTH		(2)
#define BXmSOUTH		(3)
#define BXmWEST			(4)
#define BXmEAST			(5)
#define BXmNORTHWEST		(6)
#define BXmNORTHEAST		(7)
#define BXmSOUTHWEST		(8)
#define BXmSOUTHEAST		(9)
#define BXmFULLSIZE		(10)
#define BXmFULLNORTH		(11)
#define BXmFULLWIDTH		(12)
#define BXmFULLSOUTH		(13)
#define BXmFULLWEST		(14)
#define BXmFULLHEIGHT		(15)
#define BXmFULLEAST		(16)

/* shadowType constants
 */
#ifndef _BXmUSE_LITERALS
#define BXmSHADOW_IN		XmSHADOW_IN
#define BXmSHADOW_OUT		XmSHADOW_OUT
#define BXmSHADOW_ETCHED_IN	XmSHADOW_ETCHED_IN
#define BXmSHADOW_ETCHED_OUT	XmSHADOW_ETCHED_OUT
#else
#define BXmSHADOW_IN		(7)
#define BXmSHADOW_OUT		(8)
#define BXmSHADOW_ETCHED_IN	(5)
#define BXmSHADOW_ETCHED_OUT	(6)
#endif

/* unitType constants
 */
#ifndef _BXmUSE_LITERALS
#define BXmPIXELS		XmPIXELS
#else
#define BXmPIXELS		(0)
#endif
#define BXmINCHES		(5)
#define BXmCENTIMETERS		(6)
#define BXmMILLIMETERS		(7)
#define BXmPOINTS		(8)
#define BXmFONT_UNITS		(9)
#define BXmAV_CHARS		(5001)
#define BXmMAX_CHARS		(5002)

/* match state constants
 */
#define BXmREGEX_NO_MATCH       (1)
#define BXmREGEX_MIGHT_MATCH    (2)
#define BXmREGEX_MATCH          (3)
#define BXmREGEX_ERROR          (4)

/* autoTraversalPolicy constants
 */
#define BXmTRAVERSE_NONE	(1)
#define BXmTRAVERSE_MATCH	(2)
#define BXmTRAVERSE_ALWAYS	(3)

/* callback reasons
 */
#define BXmCR_CLOSE		(7001)
#define BXmCR_AUTO_ACTIVATE	(7002)
#define BXmCR_SELECT_REF	(7003)
#define BXmCR_NEW_TOPIC		(7004)
#define BXmCR_CREATE_VIEW	(7005)
#define BXmCR_CREATE_USAGE	(7006)
#define BXmCR_ACTION		(7007)
#define BXmCR_PRE_EDIT		(7008)
#define BXmCR_POST_EDIT		(7009)
#define BXmCR_SELECTION		(7010)
#define BXmCR_CHANGE_POSITION	(7011)
#define BXmCR_DROP_ACTIVATE	(7012)

/* other special BXmTextTable constants
 */
#define BXmLIKE_NUM_ROWS                (-1)
#define BXmLIKE_SHADOW_THICKNESS        (-1)
#define BXmPOS_LIST_END                 (-1)
#define BXmINT_LIST_END                 (32767)

/* other special BXmHelp* constants
 */
#define BXmNO_TOPIC_ID		(-1)

#endif      /* _BXmConst_h */

