/* 
 * (c) Copyright 1993 Silicon Graphics, INC. 
 * ALL RIGHTS RESERVED 
*/ 
/*   $RCSfile: ColorCP.h,v $ $Revision: 1.1 $ $Date: 1995/01/27 18:06:01 $ */
#ifndef _SgColorChooserP_h
#define _SgColorChooserP_h

#include <Xm/BulletinBP.h>
#include "ColorC.h"
#include "ColorHexagon.h"
#include "ColorChooserSwatch.h"
#include "GLColorSliderGadgetC.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  New fields for the SelectionBox widget class record  */

typedef struct
{
	XtPointer	extension;      /* Pointer to extension record */
#ifdef __sgi
/* Allow future extensions without breaking shared library compatibility */
	caddr_t _SG_vendorExtension;
#endif /* __sgi */
} SgColorChooserClassPart;


/* Full class record declaration */

typedef struct _SgColorChooserClassRec
{
	CoreClassPart			core_class;
	CompositeClassPart		composite_class;
	ConstraintClassPart		constraint_class;
	XmManagerClassPart		manager_class;
	XmBulletinBoardClassPart	bulletin_board_class;
	SgColorChooserClassPart		color_chooser_class;
} SgColorChooserClassRec;

externalref SgColorChooserClassRec sgColorChooserClassRec;


/* New fields for the SelectionBox widget record */

typedef struct
{
  unsigned char           default_type;
  Boolean                 minimize_buttons;

  XmString                ok_label_string;
  XtCallbackList          ok_callback;
  Widget                  ok_button;

  XmString                apply_label_string;
  XtCallbackList          apply_callback;
  Widget                  apply_button;

  XmString                cancel_label_string;
  XtCallbackList          cancel_callback;

  XmString                help_label_string;
  Widget                  help_button;

  Widget                  separator;

  XmString                current_color_label_string;
  XmString                stored_color_label_string;
  XmString                hue_label_string;
  XmString                saturation_label_string;
  XmString                value_label_string;
  XmString                red_label_string;
  XmString                green_label_string;
  XmString                blue_label_string;

  XtCallbackList          drag_callback;
  XtCallbackList          value_changed_callback;

  ShowSliders             show_sliders; /* Controls which color slider to 
					  display */
  int                     rgb_base;	/* RGB Color number base */
  Boolean                 wysiwyg; /* controls WYSIWYG mode for the chooser */
  Boolean                 use_gl; /* Use GL if True otherwise use X only */
  Boolean                 double_buffer; /* In GLmode use double buffering 
					    or not */

  /* Private Variables */
  ColorHexagon color_hexagon_obj; /* A handle to the color hexagon object */
  Boolean      work_proc_run;
  XEvent      *work_event;	/* Event for use by work proc */
  XtWorkProcId work_proc_id;

  /* Chooser widgets */
  Widget   sep_widget;
  Widget   color_hexagon_widget;
  Widget   color_hexagon_widget_overlay;  /*****added for OpenGL******/
  Widget   gray_button_widget;
  Widget   hue_label_widget;
  Widget   hue_text_widget;
  Widget   sat_label_widget;
  Widget   sat_text_widget;
  Widget   value_label_widget;
  Widget   value_text_widget;
  Widget   red_label_widget;
  Widget   red_text_widget;
  Widget   green_label_widget;
  Widget   green_text_widget;
  Widget   blue_label_widget;
  Widget   blue_text_widget;
  Widget   current_color_draw;
  Widget   stored_color_draw;
  Widget   left_arrow_button;	/* Arrow to set current color with stored */
  Widget   right_arrow_button;	/* Arrow to set stored color with current */
  Widget   sliders_widget;
  Widget   current_color_label;
  Widget   stored_color_label;

  GLColorSliderGadgetP hue_slider;
  GLColorSliderGadgetP sat_slider;
  GLColorSliderGadgetP value_slider;
  GLColorSliderGadgetP red_slider;
  GLColorSliderGadgetP green_slider;
  GLColorSliderGadgetP blue_slider;
  
  Widget hue_slider_widget;
  Widget sat_slider_widget;
  Widget value_slider_widget;
  Widget red_slider_widget;
  Widget green_slider_widget;
  Widget blue_slider_widget;
  
  /* Color swatch object for current and stored widgets */
  ColorChooserSwatch current_color_swatch;
  ColorChooserSwatch stored_color_swatch;

  void     *has_focus;		/* Slider gadget or widget with input focus.*/

  Boolean   geo_created;	/* Make sure _SgColorChooserGeoMatrixCreate 
				   is invoked only once */
  Dimension workarea_height;	/* Original height of workarea */
  Boolean   rgb_sliders_managed;/* Flag that indicated when sliders 
				   are managed */
  Boolean   hs_sliders_managed;
#ifdef __sgi
  /* Allow future extensions without breaking shared library compatibility */
  caddr_t _SG_vendorExtension;
#endif				/* __sgi */
} SgColorChooserPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _SgColorChooserRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmBulletinBoardPart	bulletin_board;
	SgColorChooserPart	color_chooser;
} SgColorChooserRec;


#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _SgColorChooserP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
