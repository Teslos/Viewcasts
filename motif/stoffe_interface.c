/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   stoffe_interface.c                               */
/*                                                               */
/* Routinenname                 | Funktion                       */
/* ------------------------------------------------------------- */
/* Createstoffe_interface       |                                */
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

static Widget S_stoffe0, XmForm1, L_header2, PB_beenden4, 
              PB_hilfe6, Separator07, XmForm_o10, TB_alle_stoffe11,
              L_name12, RC_allestoffe13, TB_solid14, TB_gitter15,
              TB_transparent16, TB_result17, TB_netz18, TB_absenken19,
              TB_stoffgruppen20, TB_rahmen21, L_stoffe22, L_zeile23,
              Separator130,
              TT_stoffgebiete31, PB_darstellen32, PB_reset33,
              S_verschiebung40, XmForm41, PB_ok42;

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
  XtCallCallbacks((Widget)PB_hilfe6, (String)"activateCallback", (XtPointer)0);
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
	XtPopdown((Widget)S_stoffe0);
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
	help_handler((int)1010000);
}

Widget
Createstoffe_interface(
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
  S_stoffe0 = XtVaCreatePopupShell(
                (name ? name : "S_stoffe"),
                transientShellWidgetClass, parent,
                "minWidth", 650, "minHeight", 440,
       	        "x", 10, "y", 550,
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
                           S_stoffe0,
		           "horizontalSpacing", 10,
		           "verticalSpacing", 10,
		           0, 0);

  XtAddCallback(XmForm1, "helpCallback",
                (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

  L_header2 = XtVaCreateManagedWidget(
                           "L_header", xmLabelGadgetClass,
                           XmForm1,
		           "leftAttachment", XmATTACH_FORM,
		           "topAttachment", XmATTACH_FORM,
		           0, 0);

  PB_beenden4 = XtVaCreateManagedWidget(
                           "PB_beenden", xmPushButtonGadgetClass,
                           XmForm1,
		           "leftAttachment", XmATTACH_FORM,
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

  XmForm_o10 = XtVaCreateManagedWidget(
                           "XmForm_o", xmFormWidgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "rightAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_FORM,
	                   0, 0);

  TB_alle_stoffe11 = XtVaCreateManagedWidget(
                            "TB_alle_stoffe", xmToggleButtonWidgetClass,
                            XmForm_o10,
		            0, 0);

  L_name12 = XtVaCreateManagedWidget(
                            "L_name", xmLabelGadgetClass, 
                            XmForm_o10,
		            0, 0);

  RC_allestoffe13 = XtVaCreateManagedWidget(
                            "RC_allestoffe", xmRowColumnWidgetClass,
                            XmForm_o10,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_alle_stoffe11,
                            "radioBehavior", True,
                            "orientation", XmVERTICAL,
                            "packing", XmPACK_COLUMN,
                            XmNnumColumns, 3,
                            0, 0);

  TB_solid14 = XtVaCreateManagedWidget(
                            "TB_solid", xmToggleButtonGadgetClass,
                            RC_allestoffe13,
	  	            0, 0);

  TB_gitter15 = XtVaCreateManagedWidget(
                            "TB_gitter", xmToggleButtonGadgetClass,
                            RC_allestoffe13,
	  	            0, 0);

  TB_transparent16 = XtVaCreateManagedWidget(
                            "TB_transparent", xmToggleButtonGadgetClass,
                            RC_allestoffe13,
	  	            0, 0);

  TB_result17 = XtVaCreateManagedWidget(
                            "TB_result", xmToggleButtonGadgetClass,
                            RC_allestoffe13,
	  	            0, 0);

  TB_netz18 = XtVaCreateManagedWidget(
                            "TB_netz", xmToggleButtonWidgetClass,
                            XmForm_o10,
                            "rightAttachment", XmATTACH_FORM,
	  	            0, 0);

  TB_absenken19 = XtVaCreateManagedWidget(
                            "TB_absenken", xmToggleButtonWidgetClass,
                            XmForm_o10,
                            "rightAttachment", XmATTACH_FORM,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", TB_netz18,
		            0, 0);

  TB_stoffgruppen20 = XtVaCreateManagedWidget(
                            "TB_stoffgruppen", xmToggleButtonWidgetClass,
                            XmForm_o10,
                            "rightAttachment", XmATTACH_FORM,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", TB_absenken19,
		            0, 0);

  TB_rahmen21 = XtVaCreateManagedWidget(
                            "TB_rahmen", xmToggleButtonWidgetClass,
                            RC_allestoffe13,
		            0, 0);

  L_stoffe22 = XtVaCreateManagedWidget(
                           "L_stoffe", xmLabelGadgetClass,
                           XmForm_o10,
                           0, 0);

  L_zeile23 = XtVaCreateManagedWidget(
                            "L_zeile", xmLabelGadgetClass,
                            XmForm_o10,
                            0, 0);

  Separator130 = XtVaCreateManagedWidget(
                           "Separator1", xmSeparatorGadgetClass,
                           XmForm1,
		           "separatorType", XmSINGLE_LINE,
		           "leftAttachment", XmATTACH_FORM,
		           "rightAttachment", XmATTACH_FORM,
		           "topAttachment", XmATTACH_WIDGET,
		           "topWidget", RC_allestoffe13,
		           0, 0);

  TT_stoffgebiete31 = XtVaCreateManagedWidget(
                            "TT_stoffgebiete", bxmTextTableWidgetClass,
                            XmForm1,
                            "visibleRows", 5,
                            "numColumns", 11,
                            "leftAttachment", XmATTACH_FORM,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", Separator130,
                            "hasHorizontalHeader", True,
                            "headerSelectable", False,
                            "shadowThickness", 1,
                            "selectionPolicy", BXmMULTIPLE_SELECT,
                            "autoActivate", True,
                            0, 0);

  PB_darstellen32 = XtVaCreateManagedWidget(
                           "PB_darstellen", xmPushButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "bottomAttachment", XmATTACH_WIDGET,
                           "bottomWidget", Separator07,
                           0, 0);

  PB_reset33 = XtVaCreateManagedWidget(
                           "PB_reset", xmPushButtonGadgetClass,
                           XmForm1,
                           "rightAttachment", XmATTACH_FORM,
                           "bottomAttachment", XmATTACH_WIDGET,
                           "bottomWidget", Separator07,
                           0, 0);

/* Eingabemoeglichkeiten fuer Verschiebung einzelner Stoffe ------------ */
    S_verschiebung40 = XtVaCreatePopupShell(
                           "S_verschiebung",
                           transientShellWidgetClass, XmForm1,
                           "minWidth", 120,
                           "minHeight", 50,
                           "x", 980,
                           "y", 320,
                           0, 0);

    XmForm41 = XtVaCreateManagedWidget(
                           "XmForm", xmFormWidgetClass,
                           S_verschiebung40,
                           "horizontalSpacing", 10,
                           "verticalSpacing", 10,
                           0, 0);

    PB_ok42 = XtVaCreateManagedWidget(
                           "PB_ok", xmPushButtonWidgetClass, XmForm41,
                           "leftAttachment", XmATTACH_FORM,
                           "bottomAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_FORM,
                           "rightAttachment", XmATTACH_FORM,
                           0, 0);

  return(S_stoffe0);
}

