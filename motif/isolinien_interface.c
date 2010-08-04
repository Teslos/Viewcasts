/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   27.01.98                                         */
/* Dateiname:   isolinien_interface.c                            */
/*                                                               */
/* Routinenname                 | Funktion                       */
/* ------------------------------------------------------------- */
/* Createisolinien_interface    |                                */
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

static Widget S_isolinien0, XmForm1,
  TB_tabelle_zeichnen2, TB_tabelle_nicht_zeichnen3,
  TB_tabelle_auto_range, PB_beenden4, PB_ok5, PB_hilfe6, Separator07,
  L_tabelle11, RC_tabelle12,
  TB_default13, TB_grau14, TB_raster15, TB_datei16, 
  Separator120, L_isolinien21, TT_isolinien22,
  L_wert23, T_wert24, L_breit25, T_breit26, 
	SeparatorIso1, 
	RC_contrisosurface, PB_genisosurface, PB_delisosurface,
	SeparatorIso2, L_isosurface,TT_isosurfaces,L_wertisosurface,
  T_wertisosurface,  RC_contr27, 
	RC_contrisovolume, PB_genisovolume, PB_delisovolume,
	SeparatorIso3, L_isovolume,TT_isovolumes,L_wertisovolume,
	T_wertisovolume,
  PB_erzeugen27, PB_loeschen28,
  Separator130, L_intervall31, 
  L_max32, T_max33, L_min34, T_min35;

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
  XtCallCallbacks((Widget)PB_hilfe6,(String)"activateCallback",(XtPointer)0);
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
	XtPopdown((Widget)S_isolinien0);
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
	help_handler((int)1040000);
}

Widget
Createisolinien_interface(
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
	S_isolinien0 = XtVaCreatePopupShell(
			(name ? name : "S_isolinien"),
			transientShellWidgetClass, parent,
			"minWidth", 450, "minHeight", 400,
			"x", 15, "y", 590,
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
			S_isolinien0,
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



	L_tabelle11 = XtVaCreateManagedWidget(
			"L_tabelle", xmLabelGadgetClass, 
			XmForm1,
			"topAttachment", XmATTACH_FORM,
			"leftAttachment", XmATTACH_FORM,
			0, 0);

	TB_tabelle_zeichnen2 = XtVaCreateManagedWidget(
			"TB_tabelle_zeichnen",
			xmToggleButtonWidgetClass,
			XmForm1,
			"topAttachment", XmATTACH_FORM,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_tabelle11,
			0, 0);

	TB_tabelle_nicht_zeichnen3 = XtVaCreateManagedWidget(
			"TB_tabelle_nicht_zeichnen",
			xmToggleButtonWidgetClass,
			XmForm1,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", TB_tabelle_zeichnen2,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_tabelle11,
			0, 0);
	TB_tabelle_auto_range = XtVaCreateManagedWidget(
			"TB_tabelle_auto_range",
			xmToggleButtonWidgetClass,
			XmForm1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, TB_tabelle_nicht_zeichnen3,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_tabelle11,
			0, 0);

	RC_tabelle12 = XtVaCreateManagedWidget(
			"RC_tabelle", xmRowColumnWidgetClass,
			XmForm1,
			"topAttachment", XmATTACH_FORM,
			"rightAttachment", XmATTACH_FORM,
			"radioBehavior", True,
			"orientation", XmVERTICAL,
			"packing", XmPACK_COLUMN,
			0, 0);

	TB_default13 = XtVaCreateManagedWidget(
			"TB_default", xmToggleButtonGadgetClass,
			RC_tabelle12,
			0, 0);

	TB_grau14 = XtVaCreateManagedWidget(
			"TB_grau", xmToggleButtonGadgetClass,
			RC_tabelle12,
			0, 0);

	TB_raster15 = XtVaCreateManagedWidget(
			"TB_raster", xmToggleButtonGadgetClass,
			RC_tabelle12,
			0, 0);

	TB_datei16 = XtVaCreateManagedWidget(
			"TB_datei", xmToggleButtonGadgetClass,
			RC_tabelle12,
			0, 0);

	Separator120 = XtVaCreateManagedWidget(
			"Separator1", xmSeparatorGadgetClass,
			XmForm1,
			"separatorType", XmSINGLE_LINE,
			"leftAttachment", XmATTACH_FORM,
			"rightAttachment", XmATTACH_FORM,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", RC_tabelle12,
			0, 0);

	L_isolinien21 = XtVaCreateManagedWidget(
			"L_isolinien", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_FORM,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", Separator120,
			0, 0);

	TT_isolinien22 = XtVaCreateManagedWidget(
			"TT_isolinien", bxmTextTableWidgetClass,
			XmForm1,
			"visibleRows", 2,
			"numColumns", 4,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_isolinien21,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", Separator120,
			"hasHorizontalHeader", True,
			"headerSelectable", False,
			"shadowThickness", 1,
			"selectionPolicy", BXmSINGLE_SELECT,
			"selectionOrientation", BXmHORIZONTAL,
			"autoActivate", True,
			"scrollHorizontal", False,
			BXmNautoActivate, True,
			0, 0);

	L_wert23 = XtVaCreateManagedWidget(
			"L_wert", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_isolinien21,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", TT_isolinien22,
			0, 0);

	T_wert24 = XtVaCreateManagedWidget(
			"T_wert", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 10,
			XmNvalue, "1000.0",
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_wert23,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", TT_isolinien22,
			0, 0);

	L_breit25 = XtVaCreateManagedWidget(
			"L_breit", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", T_wert24,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", TT_isolinien22,
			0, 0);

	T_breit26 = XtVaCreateManagedWidget(
			"T_breit", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 5,
			XmNvalue, "10.0",
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_breit25,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", TT_isolinien22,
			0, 0);

	SeparatorIso1 = XtVaCreateManagedWidget(
			"SeparatorIso1", xmSeparatorGadgetClass,
			XmForm1,
			XmNseparatorType, XmSINGLE_LINE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, T_breit26,
			0, 0);


	L_isosurface = XtVaCreateManagedWidget(
			"L_isosurface", xmLabelGadgetClass,
			XmForm1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso1,
			0, 0);

	TT_isosurfaces = XtVaCreateManagedWidget(
			"TT_isosurfaces", bxmTextTableWidgetClass,
			XmForm1,
			BXmNvisibleRows, 2,
			BXmNnumColumns, 4,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_isosurface,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso1,
			BXmNhasHorizontalHeader, True,
			BXmNheaderSelectable, False,
			XmNshadowThickness, 1,
			BXmNselectionPolicy, BXmSINGLE_SELECT,
			BXmNselectionOrientation, BXmHORIZONTAL,
			BXmNautoActivate, True,
			BXmNscrollHorizontal, False,
			BXmNautoActivate, True,
			0, 0);

	L_wertisosurface = XtVaCreateManagedWidget(
			"L_wertisosurface", xmLabelGadgetClass,
			XmForm1,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_isosurface,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, TT_isosurfaces,
			0, 0);

	T_wertisosurface = XtVaCreateManagedWidget(
			"T_wertisosurface", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 10,
			XmNvalue, "1000.0",
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_wertisosurface,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, TT_isosurfaces,
			0, 0);

	RC_contrisosurface = XtVaCreateManagedWidget(
			"RC_controlisosurface", xmRowColumnWidgetClass,
			XmForm1,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso1,
			XmNorientation, XmVERTICAL,
			XmNisAligned, TRUE,
			0, 0);

	PB_genisosurface = XtVaCreateManagedWidget(
			"PB_genisosurface", xmPushButtonGadgetClass,
			RC_contrisosurface,
			0, 0);

	PB_delisosurface = XtVaCreateManagedWidget(
			"PB_delisosurface", xmPushButtonGadgetClass,
			RC_contrisosurface,
			0, 0);


	SeparatorIso2 = XtVaCreateManagedWidget(
			"SeparatorIso2", xmSeparatorGadgetClass,
			XmForm1,
			XmNseparatorType, XmSINGLE_LINE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, T_wertisosurface,
			0, 0);



	RC_contr27 = XtVaCreateManagedWidget(
			"RC_control", xmRowColumnWidgetClass,
			XmForm1,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, Separator120,
			XmNorientation, XmVERTICAL,
			XmNisAligned, TRUE,
			0, 0);

	PB_erzeugen27 = XtVaCreateManagedWidget(
			"PB_erzeugen", xmPushButtonGadgetClass,
			RC_contr27,
			0, 0);

	PB_loeschen28 = XtVaCreateManagedWidget(
			"PB_loeschen", xmPushButtonGadgetClass,
			RC_contr27,
			0, 0);

	Separator130 = XtVaCreateManagedWidget(
			"Separator2", xmSeparatorGadgetClass,
			XmForm1,
			"separatorType", XmSINGLE_LINE,
			"leftAttachment", XmATTACH_FORM,
			"rightAttachment", XmATTACH_FORM,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", T_breit26,
			0, 0);

	L_isovolume = XtVaCreateManagedWidget(
			"L_isovolume", xmLabelGadgetClass,
			XmForm1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso2,
			0, 0);

	TT_isovolumes = XtVaCreateManagedWidget(
			"TT_isovolumes", bxmTextTableWidgetClass,
			XmForm1,
			BXmNvisibleRows, 2,
			BXmNnumColumns, 4,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_isosurface,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso2,
			BXmNhasHorizontalHeader, True,
			BXmNheaderSelectable, False,
			XmNshadowThickness, 1,
			BXmNselectionPolicy, BXmSINGLE_SELECT,
			BXmNselectionOrientation, BXmHORIZONTAL,
			BXmNautoActivate, True,
			BXmNscrollHorizontal, False,
			BXmNautoActivate, True,
			0, 0);

	L_wertisovolume = XtVaCreateManagedWidget(
			"L_wertisovolume", xmLabelGadgetClass,
			XmForm1,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_isovolume,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, TT_isovolumes,
			0, 0);

	T_wertisovolume = XtVaCreateManagedWidget(
			"T_wertisovolume", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 10,
			XmNvalue, "1000.0",
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, L_wertisovolume,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, TT_isovolumes,
			0, 0);

	RC_contrisovolume = XtVaCreateManagedWidget(
			"RC_controlisovolume", xmRowColumnWidgetClass,
			XmForm1,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, SeparatorIso2,
			XmNorientation, XmVERTICAL,
			XmNisAligned, TRUE,
			0, 0);

	PB_genisovolume = XtVaCreateManagedWidget(
			"PB_genisovolume", xmPushButtonGadgetClass,
			RC_contrisovolume,
			0, 0);

	PB_delisovolume = XtVaCreateManagedWidget(
			"PB_delisovolume", xmPushButtonGadgetClass,
			RC_contrisovolume,
			0, 0);


	SeparatorIso3 = XtVaCreateManagedWidget(
			"SeparatorIso3", xmSeparatorGadgetClass,
			XmForm1,
			XmNseparatorType, XmSINGLE_LINE,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, T_wertisovolume,
			0, 0);


	L_intervall31 = XtVaCreateManagedWidget(
			"L_intervall", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_FORM,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", SeparatorIso3,
			0, 0);

	L_min34 = XtVaCreateManagedWidget(
			"L_min", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_intervall31,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", SeparatorIso3,
			0, 0);

	T_min35 = XtVaCreateManagedWidget(
			"T_min", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 12,
			XmNvalue, "0.0",
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_min34,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", SeparatorIso3,
			0, 0);
	L_max32 = XtVaCreateManagedWidget(
			"L_max", xmLabelGadgetClass,
			XmForm1,
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", T_min35,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", SeparatorIso3,
			0, 0);

	T_max33 = XtVaCreateManagedWidget(
			"T_max", xmTextFieldWidgetClass,
			XmForm1,
			XmNcolumns, 12,
			XmNvalue, "0.0",
			"leftAttachment", XmATTACH_WIDGET,
			"leftWidget", L_max32,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", SeparatorIso3,
			0, 0);

	Separator07 = XtVaCreateManagedWidget(
			"Separator0", xmSeparatorGadgetClass,
			XmForm1,
			"separatorType", XmDOUBLE_LINE,
			"leftAttachment", XmATTACH_FORM,
			"rightAttachment", XmATTACH_FORM,
			"topAttachment", XmATTACH_WIDGET,
			"topWidget", T_min35,
			"bottomAttachment", XmATTACH_WIDGET,
			"bottomWidget", PB_beenden4,
			0, 0);


	return(S_isolinien0);
}

