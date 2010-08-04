/********************************************************************
 *
 * Private header file of private GC caching module
 *
 ********************************************************************
 *
 * SccsId = "@(#)TextTableGC.h	1.4  7/4/95  (Copyright 1993, BREDEX GmbH)"
 *
 ********************************************************************
 *
 * (c) Copyright 1993, 1995 BREDEX GmbH, Braunschweig, Germany
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
#ifndef _BXmTextTableGC_h
#define _BXmTextTableGC_h

/* general header files
 */
#include <BXm/BXm.h>

/* constant for NO GC
 */
#define BXmNO_GC           ((GC)0)

/* position index of general gc entries
 */
#define GCIDX_TABLE	0
#define GCIDX_HHEAD	1
#define GCIDX_VHEAD	2

/* forward declarations
 */
extern void _BXmTTGCInitWidget _PROTOS ((Widget, Drawable));
extern GC _BXmTTGCGetGC _PROTOS ((Widget, Pixel, int));
extern void _BXmTTGCReleaseGC _PROTOS ((Widget, GC));
extern void _BXmTTGCFreeAllData _PROTOS ((Widget));
extern void _BXmTTGCSetStdClipMasks _PROTOS ((Widget, XRectangle*, XRectangle*, XRectangle*));
extern void _BXmTTGCSetForeground _PROTOS ((Widget, GC*, Pixel));
extern void _BXmTTGCSetFonts _PROTOS ((Widget, Font));

/********************************************************************
 *
 * Preface:
 * ========
 *
 *  We have a PerWidgetInfo cashe with the following informations:
 *   - widget:      the widget
 *   - drawable:    for gc creation (screen and depth)
 *   - clipMask[3]: clip masks for table, hhead, vhead
 *
 *  We have a GC cashe with the following informations:
 *   - gc:	   the GC
 *   - widget:     the widget
 *   - foreground: the actual foreground color
 *   - position:   the clip mask index (table, hhead, vhead)
 *   - refcount:   number of uses
 *
 * Interface:
 * ==========
 *
 *  _BXmTTGCInitWidget (widget, drawable)
 *	- registers widget in PWI cashe with drawable for
 *        gc creation (screen and depth)
 *	- standard clips are created but no clips are set
 *  
 *  _BXmTTGCFreeAllData (widget)
 *	- frees all data for widget in the PWI and GC cashes
 *
 *  GC _BXmTTGCGetGC (widget, foreground, position)
 *	- returns a (new) gc for the widget with given
 *	  foreground and position
 *  
 *  _BXmTTGCReleaseGC (widget, gc)
 *	- releases a gc returned with _BXmTTGCGetGC()
 *
 *  _BXmTTGCSetStdClipMasks (widget, table_mask, hhead_mask, vhead_mask)
 *	- sets the clip masks of the standard gcs
 *
 *  _BXmTTGCSetForeground (widget, gcPtr, foreground)
 *	- sets the foreground in a given gc
 *	- if the gc is used otherwise this function returns
 *	  a new created gc
 *
 *  _BXmTTGCSetFonts (widget, font)
 *	- sets the font for all gcs of widget
 *
 ********************************************************************/

#endif		 /* _BXmTextTableGC_h */

