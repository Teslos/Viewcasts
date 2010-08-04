/* 
 * (c) Copyright 1993 Silcon Graphics, INC. 
 * ALL RIGHTS RESERVED 
*/ 
/*   $RCSfile: ColorC.h,v $ $Revision: 1.1 $ $Date: 1995/01/27 18:06:01 $ */
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _SgColorChooser_h
#define _SgColorChooser_h

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SgNshowSliders  "showSliders"
#define SgNrgbBase      "rgbBase"
#define SgNwysiwyg      "wysiwyg"
#define SgNuseGl        "useGl"
#define SgNdoubleBuffer "doubleBuffer"

#define SgNcurrentColorLabelString "currentColorLabelString"
#define SgNstoredColorLabelString  "storedColorLabelString"
#define SgNhueLabelString          "hueLabelString"
#define SgNsaturationLabelString   "saturationLabelString"
#define SgNvalueLabelString        "valueLabelString"
#define SgNredLabelString          "redLabelString"
#define SgNgreenLabelString        "greenLabelString"
#define SgNblueLabelString         "blueLabelString"

#define SgCShowSliders  "ShowSliders"
#define SgCRgbBase      "RgbBase"
#define SgCWysiwyg      "Wysiwyg"
#define SgCUseGl        "UseGl"
#define SgCDoubleBuffer "DoubleBuffer"

#define SgRShowSliders "ShowSliders"

typedef enum _ShowSliders {
  SgValue,
  SgRGB_and_Value,
  SgRGB_and_HSV
  } ShowSliders;

/* Class record constants */

externalref WidgetClass sgColorChooserWidgetClass;

typedef struct _SgColorChooserClassRec * SgColorChooserWidgetClass;
typedef struct _SgColorChooserRec      * SgColorChooserWidget;

typedef struct {
  int      reason;
  XEvent  *event;
  short    r, g, b;		/* Color Chooser RGB values */
} SgColorChooserCallbackStruct;

#ifndef SgIsColorChooser
#define SgIsColorChooser(w)  (XtIsSubclass (w, sgColorChooserWidgetClass))
#endif



/********    Public Function Declarations    ********/
#ifdef _NO_PROTO

extern Widget SgCreateColorChooser() ;
extern Widget SgCreateColorChooserDialog() ;
extern void   SgColorChooserSetColor();
extern void   SgColorChooserGetColor();
extern void   SgColorChooserSetCurrentColor();
extern void   SgColorChooserSetStoredColor();

#else

/* Create Color Chooser widget convenient routine */
extern Widget SgCreateColorChooser( 
                        Widget p,
                        String name,
                        ArgList args,
                        Cardinal n) ;
/* Create Color Chooser dialog convenient routine */
extern Widget SgCreateColorChooserDialog( 
                        Widget p,
                        String name,
                        ArgList args,
                        Cardinal n) ;
/* Set the color of the Color Chooser widget */
extern void   SgColorChooserSetColor(Widget w, short r, short g, short b);
/* Get the current color of the Color Chooser widget */
extern void   SgColorChooserGetColor(Widget w, short *r, short *g, short *b);
/* Set the color of the Color Chooser widget, but not the stored color */
extern void   SgColorChooserSetCurrentColor(Widget w, short r, short g, short b);
/* Set the stored color of the Color Chooser widget, not the main selected 
   color */
extern void   SgColorChooserSetStoredColor(Widget w, short r, short g, short b);

#endif /* _NO_PROTO */
/********    End Public Function Declarations    ********/

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _SgColorChooser_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
