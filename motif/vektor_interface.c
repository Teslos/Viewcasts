/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   25.05.98                                         */
/* Dateiname:   vektor_interface.c                               */
/*                                                               */
/* Routinenname                 | Funktion                       */
/* ------------------------------------------------------------- */
/* Createvektor_interface       |                                */
/* XmForm1_helpCallback         |                                */
/* PB_beenden4_activateCallback |                                */
/* PB_hilfe6_activateCallback   |                                */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>

#ifndef _MOTIF_H_
#include "motif.h"
#define _MOTIF_H_
#endif /* _MOTIF_H_ */

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define	true	True
#define	false	False

#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>
#include <TableMan.h>
#include <TextTable.h>
#include <Xm/ArrowB.h>

#if !defined(XtSpecificationRelease) || XtSpecificationRelease <= 4
#include <X11/Quarks.h>
#else
extern	XrmQuark  XtQBool;
extern	XrmQuark  XtQBoolean;
extern	XrmQuark  XtQColor;
extern	XrmQuark  XtQCursor;
extern	XrmQuark  XtQDimension;
extern	XrmQuark  XtQDisplay;
extern	XrmQuark  XtQFile;
extern	XrmQuark  XtQFont;
extern	XrmQuark  XtQFontStruct;
extern	XrmQuark  XtQInt;
extern	XrmQuark  XtQPixel;
extern	XrmQuark  XtQPixmap;
extern	XrmQuark  XtQPointer;
extern  XrmQuark  XtQPosition;
extern  XrmQuark  XtQShort;
extern	XrmQuark  XtQString;
extern  XrmQuark  XtQUnsignedChar;
extern	XrmQuark  XtQWindow;
#endif

#undef  XtMapWidget
#undef  XtUnmapWidget
#define XtMapWidget(widget)     XMapWindow(XtDisplay((Widget)(widget)), XtWindow((Widget)(widget)))
#define XtUnmapWidget(widget)   XUnmapWindow(XtDisplay((Widget)(widget)), XtWindow((Widget)(widget)))

static Widget S_vektor0, XmForm1,
              PB_beenden4, PB_ok5, PB_hilfe6, Separator07,
              L_animation11, RC_animation12,
              L_start13, T_start14, PB_start15,
              L_stop16, T_stop17, PB_stop18, Separator119,
              TB_temperatur53, Separator152,
              TB_spannung20, PB_sp_zeit31,
              L_sp_faktor21, T_sp_faktor22,
              AB_sp_faktor_up23, AB_sp_faktor_down24,
              TB_huelle_on25, L_deform26,
              RC_deform27, TB_huelle28, TB_modell29, Separator130, 
              TB_stroemung40, PB_st_zeit51,
              L_st_faktor41, T_st_faktor42,
              AB_st_faktor_up43, AB_st_faktor_down44,
              L_darstellung45, RC_stroem46, TB_pfeile47, TB_tracer48,
              TB_red_pfeile49, TB_rollen50;

static float _vtof_(
#if NeedFunctionPrototypes
XtArgVal v)
#else
v)
XtArgVal v;
#endif
{
	union { XtArgVal v; float f; } u;
	u.v = v;
	return(u.f);
}

static XtArgVal _ftov_(
#if NeedFunctionPrototypes
float f)
#else
f)
float f;
#endif
{
  union { XtArgVal v; float f; } u;
  u.f = f;
  return(u.v);
}

#include <Xm/MenuShell.h>
#include <Xm/ScrolledW.h>

static void XmForm1_helpCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#endif
{
  XtCallCallbacks((Widget)PB_hilfe6,(String)"activateCallback",
                  (XtPointer)0);
}

static void PB_beenden4_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#endif
{
	XtPopdown((Widget)S_vektor0);
}

static void PB_hilfe6_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#endif
{
	help_handler((int)1050000);
}

Widget
Createvektor_interface(
#if NeedFunctionPrototypes
String name, Widget parent, Arg* user_args, Cardinal num_user_args)
#else
name, parent, user_args, num_user_args)
String name;
Widget parent;
Arg* user_args;
Cardinal num_user_args;
#endif
{
  S_vektor0 = XtVaCreatePopupShell(
                (name ? name : "S_vektor"),
                transientShellWidgetClass, parent,
                "minWidth", 230, "minHeight", 700,
       	        "x", 45, "y", 190,
		(num_user_args > 0 ? user_args[0].name : (String)0),
                (num_user_args > 0 ? user_args[0].value : 0),
		(num_user_args > 1 ? user_args[1].name : (String)0),
                (num_user_args > 1 ? user_args[1].value : 0),
		(num_user_args > 2 ? user_args[2].name : (String)0),
                (num_user_args > 2 ? user_args[2].value : 0),
		(num_user_args > 3 ? user_args[3].name : (String)0),
                (num_user_args > 3 ? user_args[3].value : 0),
		(num_user_args > 4 ? user_args[4].name : (String)0),
                (num_user_args > 4 ? user_args[4].value : 0),
	        0, 0);

  if(num_user_args > 5)
    printf("Warning:too many arguments (max is 5: use -coption 6 or 7)\n");

  XmForm1 = XtVaCreateManagedWidget(
                "XmForm", xmFormWidgetClass, 
                S_vektor0,
	        "horizontalSpacing", 10,
	        "verticalSpacing", 10,
	        0, 0);

  XtAddCallback(XmForm1, "helpCallback",
                (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

  PB_beenden4 = XtVaCreateManagedWidget(
                "PB_beenden", xmPushButtonGadgetClass,
                XmForm1,
	        "leftAttachment", XmATTACH_FORM,
	        "bottomAttachment", XmATTACH_FORM,
	        0, 0);
  XtAddCallback(PB_beenden4, "activateCallback",
      (XtCallbackProc)PB_beenden4_activateCallback, (XtPointer)0);

  PB_ok5 = XtVaCreateManagedWidget(
                "PB_ok", xmPushButtonGadgetClass,
                XmForm1,
	        "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", PB_beenden4,
	        "bottomAttachment", XmATTACH_FORM,
	        0, 0);
  XtAddCallback(PB_beenden4, "activateCallback",
      (XtCallbackProc)PB_beenden4_activateCallback, (XtPointer)0);

  PB_hilfe6 = XtVaCreateManagedWidget(
                "PB_hilfe", xmPushButtonGadgetClass,
                XmForm1,
	        "bottomAttachment", XmATTACH_FORM,
	        "rightAttachment", XmATTACH_FORM,
	        0, 0);
  XtAddCallback(PB_hilfe6, "activateCallback",
      (XtCallbackProc)PB_hilfe6_activateCallback, (XtPointer)0);

  Separator07 = XtVaCreateManagedWidget(
                "Separator0", xmSeparatorGadgetClass,
                XmForm1,
	        "separatorType", XmDOUBLE_LINE,
	        "leftAttachment", XmATTACH_FORM,
	        "rightAttachment", XmATTACH_FORM,
	        "bottomAttachment", XmATTACH_WIDGET,
	        "bottomWidget", PB_beenden4,
	        0, 0);

  L_animation11 = XtVaCreateManagedWidget(
                "L_animation", xmLabelWidgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_FORM,
                0, 0);

  RC_animation12 = XtVaCreateManagedWidget(
                "RC_animation", xmRowColumnWidgetClass,
                XmForm1,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", L_animation11,
                "leftAttachment", XmATTACH_FORM,
                "borderWidth", 1,
                "packing", XmPACK_COLUMN,
                XmNnumColumns, 2,
                0, 0);

  L_start13 = XtVaCreateManagedWidget(
                "L_start", xmLabelGadgetClass,
                RC_animation12,
                0, 0);

  T_start14 = XtVaCreateManagedWidget(
                "T_start", xmTextFieldWidgetClass,
                RC_animation12,
                XmNcolumns, 10,
                XmNvalue, "0.0",
                0, 0);

  PB_start15 = XtVaCreateManagedWidget(
                "PB_start", xmPushButtonWidgetClass,
                RC_animation12,
                0, 0);

  L_stop16 = XtVaCreateManagedWidget(
                "L_stop", xmLabelGadgetClass,
                RC_animation12,
                0, 0);

  T_stop17 = XtVaCreateManagedWidget(
                "T_stop", xmTextFieldWidgetClass,
                RC_animation12,
                XmNcolumns, 10,
                XmNvalue, "0.0",
                0, 0);

  PB_stop18 = XtVaCreateManagedWidget(
                "PB_stop", xmPushButtonWidgetClass,
                RC_animation12,
                0, 0);

  Separator152 = XtVaCreateManagedWidget(
                "Separator1", xmSeparatorGadgetClass,
                XmForm1,
                "separatorType", XmSINGLE_LINE,
                "leftAttachment", XmATTACH_FORM,
                "rightAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", RC_animation12,
                0, 0);

  TB_temperatur53 = XtVaCreateManagedWidget(
                "TB_temperatur", xmToggleButtonWidgetClass, 
                XmForm1,
                XtVaTypedArg, "fontList", XmRString,
                "*helvetica-bold-r-*-18-*", 25,
	        "topAttachment", XmATTACH_WIDGET,
                "topWidget", Separator152,
	        "leftAttachment", XmATTACH_FORM,
                "borderWidth", 1,
	        0, 0);

  Separator119 = XtVaCreateManagedWidget(
                "Separator1", xmSeparatorGadgetClass,
                XmForm1,
                "separatorType", XmSINGLE_LINE,
                "leftAttachment", XmATTACH_FORM,
                "rightAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_temperatur53,
                0, 0);

  TB_spannung20 = XtVaCreateManagedWidget(
                "TB_spannung", xmToggleButtonWidgetClass, 
                XmForm1,
                XtVaTypedArg, "fontList", XmRString,
                "*helvetica-bold-r-*-18-*", 25,
	        "topAttachment", XmATTACH_WIDGET,
                "topWidget", Separator119,
	        "leftAttachment", XmATTACH_FORM,
                "borderWidth", 1,
	        0, 0);

  L_sp_faktor21 = XtVaCreateManagedWidget(
                "L_sp_faktor", xmLabelGadgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_spannung20,
                0, 0);

  T_sp_faktor22 = XtVaCreateManagedWidget(
                "T_sp_faktor", xmTextFieldWidgetClass,
                XmForm1,
                XmNcolumns, 10,
                XmNvalue, "100.0",
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", L_sp_faktor21,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_spannung20,
                "topOffset", 10,
                0, 0);

  AB_sp_faktor_up23 = XtVaCreateManagedWidget(
                "AB_sp_faktor_up",
                xmArrowButtonWidgetClass, XmForm1,
                XmNarrowDirection, XmARROW_UP,
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", T_sp_faktor22,
                "leftOffset", 10,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_spannung20,
                "topOffset", 5,
                0, 0);

  AB_sp_faktor_down24 = XtVaCreateManagedWidget(
                "AB_sp_faktor_down",
                xmArrowButtonWidgetClass, XmForm1,
                XmNarrowDirection, XmARROW_DOWN,
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", T_sp_faktor22,
                "leftOffset", 10,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", AB_sp_faktor_up23,
                "topOffset", 5,
                0, 0);

  TB_huelle_on25 = XtVaCreateManagedWidget(
                "TB_huelle_on", xmToggleButtonGadgetClass,
                XmForm1,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", T_sp_faktor22,
                "leftAttachment", XmATTACH_FORM,
                0, 0);

  L_deform26 = XtVaCreateManagedWidget(
                "L_deform", xmLabelWidgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_huelle_on25,
                "topOffset", 25,
                0, 0);

  RC_deform27 = XtVaCreateManagedWidget(
                "RC_deform", xmRowColumnWidgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", L_deform26,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_huelle_on25,
                "radioBehavior", True,
                "orientation", XmVERTICAL,
                "borderWidth", 2,
                0, 0);

  TB_huelle28 = XtVaCreateManagedWidget(
                "TB_huelle", xmToggleButtonGadgetClass,
                RC_deform27,
                0, 0);

  TB_modell29 = XtVaCreateManagedWidget(
                "TB_modell", xmToggleButtonGadgetClass,
                RC_deform27,
                0, 0);

  Separator130 = XtVaCreateManagedWidget(
                "Separator1", xmSeparatorGadgetClass,
                XmForm1,
                "separatorType", XmSINGLE_LINE,
                "leftAttachment", XmATTACH_FORM,
                "rightAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", RC_deform27,
                0, 0);

  TB_stroemung40 = XtVaCreateManagedWidget(
                "TB_stroemung", xmToggleButtonWidgetClass, 
                XmForm1,
                XtVaTypedArg, "fontList", XmRString,
                "*helvetica-bold-r-*-18-*", 25,
	        "topAttachment", XmATTACH_WIDGET,
                "topWidget", Separator130,
	        "leftAttachment", XmATTACH_FORM,
                "borderWidth", 1,
	        0, 0);

  L_st_faktor41 = XtVaCreateManagedWidget(
                "L_st_faktor", xmLabelGadgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_stroemung40,
                0, 0);

  T_st_faktor42 = XtVaCreateManagedWidget(
                "T_st_faktor", xmTextFieldWidgetClass,
                XmForm1,
                XmNcolumns, 10,
                XmNvalue, "100.0",
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", L_st_faktor41,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_stroemung40,
                0, 0);

  AB_st_faktor_up43 = XtVaCreateManagedWidget(
                "AB_st_faktor_up",
                xmArrowButtonWidgetClass, XmForm1,
                XmNarrowDirection, XmARROW_UP,
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", T_st_faktor42,
                "leftOffset", 10,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", TB_stroemung40,
                "topOffset", 5,
                0, 0);

  AB_st_faktor_down44 = XtVaCreateManagedWidget(
                "AB_st_faktor_down",
                xmArrowButtonWidgetClass, XmForm1,
                XmNarrowDirection, XmARROW_DOWN,
                "leftAttachment", XmATTACH_WIDGET,
                "leftWidget", T_st_faktor42,
                "leftOffset", 10,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", AB_st_faktor_up43,
                "topOffset", 5,
                0, 0);

  L_darstellung45 = XtVaCreateManagedWidget(
                "L_darstellung", xmLabelGadgetClass,
                XmForm1,
                "leftAttachment", XmATTACH_FORM,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", T_st_faktor42,
                0, 0);

  RC_stroem46 = XtVaCreateManagedWidget(
                "RC_stroem", xmRowColumnWidgetClass,
                XmForm1,
                "topAttachment", XmATTACH_WIDGET,
                "topWidget", L_darstellung45,
                "leftAttachment", XmATTACH_FORM,
                "radioBehavior", True,
                "orientation", XmHORIZONTAL,
                "borderWidth", 2,
                "packing", XmPACK_COLUMN,
                XmNnumColumns, 2,
                0, 0);

  TB_pfeile47 = XtVaCreateManagedWidget(
                "TB_pfeile", xmToggleButtonGadgetClass,
                RC_stroem46,
                0, 0);

  TB_tracer48 = XtVaCreateManagedWidget(
                "TB_tracer", xmToggleButtonGadgetClass,
                RC_stroem46,
                0, 0);

  TB_red_pfeile49 = XtVaCreateManagedWidget(
                "TB_red_pfeile", xmToggleButtonGadgetClass,
                RC_stroem46,
                0, 0);

  TB_rollen50 = XtVaCreateManagedWidget(
                "TB_rollen", xmToggleButtonGadgetClass,
                RC_stroem46,
                0, 0);

  return(S_vektor0);
}

