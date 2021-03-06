#ifndef __sovLayerUtil_h__
#define __sovLayerUtil_h__

/* Copyright (c) Mark J. Kilgard, 1996. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include "strukturen.h"

/* Transparent type values */
/*      None                  0 */
#define TransparentPixel      1
#define TransparentMask       2

/* layered visual info template flags */
#define VisualLayerMask		0x200
#define VisualTransparentType	0x400
#define VisualTransparentValue	0x800
#define VisualAllLayerMask	0xFFF

/* C3D_layer_get parameters */
#define C3D_OVERLAY_POSSIBLE    800
#define C3D_LAYER_IN_USE        801
#define C3D_HAS_OVERLAY         802
#define C3D_TRANSPARENT_INDEX   803
/*
typedef struct _Criterion {
  int capability;
  int comparison;
  int value;
} Criterion;
*/
/* layered visual info structure */
typedef struct _sovVisualInfo {
   XVisualInfo vinfo;
   int layer;
   int type;
   unsigned long value;
} sovVisualInfo;

/* SERVER_OVERLAY_VISUALS property element */
typedef struct _sovOverlayInfo {
   long  overlay_visual;
   long  transparent_type;
   long  value;
   long  layer;
} sovOverlayInfo;

extern sovVisualInfo *sovGetVisualInfo(
  Display *display,
  long lvinfo_mask,
  sovVisualInfo *lvinfo_template,
  int *nitems_return);
extern Status sovMatchVisualInfo(
  Display *display,
  int screen,
  int depth,
  int class,
  int layer,
  sovVisualInfo *lvinfo_return);
extern void C3D_use_layer( GLenum layer, t_winstruct *win );
extern int  C3D_layer_get( GLenum param, t_winstruct *win );
extern void C3D_init_display_mode( unsigned int mask );
#endif /* __sovLayerUtil_h__ */
