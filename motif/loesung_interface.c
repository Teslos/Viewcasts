/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   06.01.97                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*              |                                                */
/*              |                                                */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/*****************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
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
#include <X11/Shell.h>

#include <Xm/Xm.h>

#define	true	True
#define	false	False

#undef  XtMapWidget
#undef  XtUnmapWidget
#define XtMapWidget(widget)     XMapWindow(XtDisplay((Widget)(widget)), XtWindow((Widget)(widget)))
#define XtUnmapWidget(widget)   XUnmapWindow(XtDisplay((Widget)(widget)), XtWindow((Widget)(widget)))

#ifndef _MOTIF_H_
#include "motif.h"
#define _MOTIF_H_
#endif /* _MOTIF_H_ */

#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/SelectioB.h>

static Widget S_loesung0, XmForm1, PB_ok2, PB_schliessen3, PB_hilfe4,
   Separator05, Separator16,
   L_animation21, L_ani_anfang22,T_ani_anfang23, L_ani_ende24,
   T_ani_ende25, 
   PB_ani_anfang26, PB_ani_ende27,
   L_text41, L_parameter42, L_max43, L_min44, L_dimens46,
   T_text51, T_parameter52, T_max53, T_min54, T_dimens56;

static char text[256];
static char answer[256];

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

typedef struct {
	int value;
} FaceVariable;

#include <Xm/MenuShell.h>
#include <Xm/ScrolledW.h>

static Widget FaceParent(
#if NeedFunctionPrototypes
Widget w)
#else
w)
Widget w;
#endif
{
	Widget parent = XtParent(w);
	Widget grandpa;
	if(parent
	&& (grandpa = XtParent(parent))
	&& (XmIsMenuShell(parent)
	 || (XmIsScrolledWindow(grandpa)
	  && strcmp(XtName(parent), "ScrolledWindowClipWindow") == 0)))
		parent = grandpa;
	return(parent);
}


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
	XtCallCallbacks((Widget)PB_hilfe4, (String)"activateCallback", (XtPointer)0);
}

static void PB_schliessen3_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#endif
{
        XtPopdown((Widget)S_loesung0);
}

static void PB_hilfe4_activateCallback(
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

static void
activeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
        int i;
        char *Textfeld;
        XmAnyCallbackStruct *cbs =
                        (XmAnyCallbackStruct *) call_data;
        Textfeld = XmTextFieldGetString(w);
        XtUnmanageChild(w);
}

Widget
Createloesung_interface(
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
  S_loesung0 = XtVaCreatePopupShell((name ? name : "S_loesung"),
                        transientShellWidgetClass, parent,
                        "minWidth", 400,
                        "minHeight", 350,
                        "x", 146,
                        "y", 60,
  	(num_user_args > 0 ? user_args[0].name : (String)0), (num_user_args > 0 ? user_args[0].value : 0),
  	(num_user_args > 1 ? user_args[1].name : (String)0), (num_user_args > 1 ? user_args[1].value : 0),
  	(num_user_args > 2 ? user_args[2].name : (String)0), (num_user_args > 2 ? user_args[2].value : 0),
  	(num_user_args > 3 ? user_args[3].name : (String)0), (num_user_args > 3 ? user_args[3].value : 0),
  	(num_user_args > 4 ? user_args[4].name : (String)0), (num_user_args > 4 ? user_args[4].value : 0),
  	0, 0);
  if(num_user_args > 5)
    printf("Warning: too many arguments (max is 5: use -coption 6 or 7)\n");

  XmForm1 = XtVaCreateManagedWidget(
                       "XmForm", xmFormWidgetClass,
                       S_loesung0,
                       "horizontalSpacing", 10,
                       "verticalSpacing", 10,
                       0, 0);
  XtAddCallback(XmForm1, "helpCallback", 
                      (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

  PB_ok2 = XtVaCreateManagedWidget(
                       "PB_ok",xmPushButtonWidgetClass,
                       XmForm1,
                       "leftAttachment", XmATTACH_FORM,
                       "bottomAttachment", XmATTACH_FORM,
                       0, 0);
  XtAddCallback(PB_ok2, "activateCallback", 
                (XtCallbackProc)PB_schliessen3_activateCallback, 
                (XtPointer)0);

  PB_schliessen3 = XtVaCreateManagedWidget(
                       "PB_schliessen", xmPushButtonGadgetClass,
                       XmForm1,
                       "leftAttachment", XmATTACH_WIDGET,
                       "leftWidget", PB_ok2,
                       "bottomAttachment", XmATTACH_FORM,
                       0, 0);
  XtAddCallback(PB_schliessen3, "activateCallback",
                (XtCallbackProc)PB_schliessen3_activateCallback,
                (XtPointer)0);


  PB_hilfe4 = XtVaCreateManagedWidget(
                        "PB_hilfe", xmPushButtonGadgetClass,
                        XmForm1,
                 	"bottomAttachment", XmATTACH_FORM,
                 	"rightAttachment", XmATTACH_FORM,
                 	0, 0);
  XtAddCallback(PB_hilfe4, "activateCallback", 
                (XtCallbackProc)PB_hilfe4_activateCallback, (XtPointer)0);

  Separator05 = XtVaCreateManagedWidget(
                        "Separator0",xmSeparatorGadgetClass,
                        XmForm1,
	                "separatorType", XmDOUBLE_LINE,
		        "leftAttachment", XmATTACH_FORM,
		        "rightAttachment", XmATTACH_FORM,
		        "bottomAttachment", XmATTACH_WIDGET,
		        "bottomWidget", PB_ok2,
		        0, 0);

  L_animation21 = XtVaCreateManagedWidget(
                        "L_animation", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_FORM,
                        "leftAttachment", XmATTACH_FORM,
		        0, 0);

  L_ani_anfang22 = XtVaCreateManagedWidget(
                        "L_ani_anfang", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", L_animation21,
                        "leftAttachment", XmATTACH_FORM,
		        0, 0);

  T_ani_anfang23 = XtVaCreateManagedWidget(
                        "T_ani_anfang", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", L_animation21,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", L_ani_anfang22,
		        0, 0);

  PB_ani_anfang26 = XtVaCreateManagedWidget(
                        "PB_ani_anfang", xmPushButtonWidgetClass, 
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", L_animation21,
                        "leftAttachment", XmATTACH_WIDGET,
                        "leftWidget", T_ani_anfang23,
		        0, 0);

  L_ani_ende24 = XtVaCreateManagedWidget(
                        "L_ani_ende", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_ani_anfang23,
                        "leftAttachment", XmATTACH_FORM,
                        0, 0);

  T_ani_ende25 = XtVaCreateManagedWidget(
                        "T_ani_ende", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_ani_anfang23,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", L_ani_anfang22,
                        0, 0);

  PB_ani_ende27 = XtVaCreateManagedWidget(
                        "PB_ani_ende", xmPushButtonWidgetClass, 
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", T_ani_anfang23,
                        "leftAttachment", XmATTACH_WIDGET,
                        "leftWidget", T_ani_ende25,
		        0, 0);

  Separator16 = XtVaCreateManagedWidget(
                        "Separator1",xmSeparatorGadgetClass,
                        XmForm1,
                        "orientation", XmVERTICAL,
                        "separatorType", XmDOUBLE_LINE,
                        "topAttachment", XmATTACH_FORM,
                        "leftAttachment", XmATTACH_WIDGET,
                        "leftWidget", PB_ani_anfang26,
                        "bottomAttachment", XmATTACH_WIDGET,
                        "bottomWidget", Separator05,
                        0, 0);

  L_text41 = XtVaCreateManagedWidget(
                        "L_text", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_FORM,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", Separator16,
                        0, 0);

  T_text51 = XtVaCreateManagedWidget(
                        "T_text", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_FORM,
                        "rightAttachment", XmATTACH_FORM,
                        0, 0);

  L_parameter42 = XtVaCreateManagedWidget(
                        "L_parameter", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_text51,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", Separator16,
                        0, 0);

  T_parameter52 = XtVaCreateManagedWidget(
                        "T_parameter", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", T_text51,
                        "rightAttachment", XmATTACH_FORM,
                        0, 0);

  L_max43 = XtVaCreateManagedWidget(
                        "L_max", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_parameter52,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", Separator16,
                        0, 0);

  T_max53 = XtVaCreateManagedWidget(
                        "T_max", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", T_parameter52,
                        "rightAttachment", XmATTACH_FORM,
                        0, 0);

  L_min44 = XtVaCreateManagedWidget(
                        "L_min", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_max53,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", Separator16,
                        0, 0);

  T_min54 = XtVaCreateManagedWidget(
                        "T_min", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", T_max53,
                        "rightAttachment", XmATTACH_FORM,
                        0, 0);

  L_dimens46 = XtVaCreateManagedWidget(
                        "L_dimens", xmLabelWidgetClass,
                        XmForm1,
                        "topAttachment", XmATTACH_WIDGET,
		        "topWidget", T_min54,
                        "leftAttachment", XmATTACH_WIDGET,
		        "leftWidget", Separator16,
                        0, 0);

  T_dimens56 = XtVaCreateManagedWidget(
                        "T_dimens", xmTextFieldWidgetClass,
                        XmForm1,
                        XmNcolumns, 10,
                        "topAttachment", XmATTACH_WIDGET,
                        "topWidget", T_min54,
                        "rightAttachment", XmATTACH_FORM,
                        0, 0);

	return(S_loesung0);
}
