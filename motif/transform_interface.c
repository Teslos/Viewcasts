/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   transform_interface.c                            */
/*                                                               */
/* Routinenname                 | Funktion                       */
/* ------------------------------------------------------------- */
/* Createtransform_interface    |                                */
/* XmForm1_helpCallback         |                                */
/* PB_beenden4_activateCallback |                                */
/* PB_hilfe5_activateCallback   |                                */
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
#include "color3d.h" 
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define	true	True
#define	false	False

#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/ArrowB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>

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

static Widget S_transform0, XmForm1, L_header2, PB_beenden4, PB_ok41,
              PB_hilfe5, Separator06,
              TB_zyklus7,
              L_anz_zyk8, T_anz_zyk9, AB_anzahl_plus10, AB_anzahl_minus11,
              L_winkel12, T_winkel13,
              L_achse14, RC_zyklus15,
              TB_achsex16, TB_achsey17, TB_achsez18,
              Separator120,
              TB_spiegeln21, L_spiegeln22,
              RC_spiegeln23, TB_ebenexy24, TB_ebenexz25, TB_ebeneyz26,
              Separator130,
              TB_schneiden31, TB_schneiden_aus29, TB_def_plane,
              TB_def_koord32, RC_punkte33, 
              Separator140, Separator27,
              T_punkt1x41, T_punkt1y42, T_punkt1z43,
              T_punkt2x44, T_punkt2y45, T_punkt2z46,
              T_punkt3x47, T_punkt3y48, T_punkt3z49,
              TB_def_nummer32, RC_nummer50, L_koord54,
              T_punkt1nr51, T_punkt2nr52, T_punkt3nr53,
              L_punkt161, L_punkt262, L_punkt363, L_nummer64, L_discutting,
              LS_discutting, LS_discuttingSW, Separator170, TB_zentrieren71;

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
  XtCallCallbacks((Widget)PB_hilfe5, (String)"activateCallback", (XtPointer)0);
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
	XtPopdown((Widget)S_transform0);
}

static void PB_hilfe5_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#endif
{
	help_handler((int)1020000);
}

Widget
Createtransform_interface(
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
  int i;
  char temstr[10];
  XmString *materials;
 
  materials = (XmString*) malloc(g_geodat.nstoff*(sizeof(char)*10));
  if(materials == NULL)
     fprintf(stderr, "Error in allocation of cutting list\n");

  S_transform0 = XtVaCreatePopupShell(
                (name ? name : "S_transform"),
                transientShellWidgetClass, parent,
                "minWidth", 400, "minHeight", 450,
       	        "x", 20, "y", 150,
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
				    S_transform0,
				    "horizontalSpacing", 10,
				    "verticalSpacing", 10,
				    0, 0);
  
  PB_beenden4 = XtVaCreateManagedWidget(
					"PB_beenden", xmPushButtonGadgetClass,
					XmForm1,
					XmNleftAttachment, XmATTACH_FORM,
					XmNbottomAttachment, XmATTACH_FORM,
					0, 0);
  PB_ok41 = XtVaCreateManagedWidget(
				    "PB_ok", xmPushButtonGadgetClass,
				    XmForm1,
				    "leftAttachment", XmATTACH_WIDGET,
				    "leftWidget", PB_beenden4,
				    "bottomAttachment", XmATTACH_FORM,
				    0, 0);
  PB_hilfe5 = XtVaCreateManagedWidget(
				      "PB_hilfe", xmPushButtonGadgetClass,
				      XmForm1,
				      "bottomAttachment", XmATTACH_FORM,
				      "rightAttachment", XmATTACH_FORM,
				      0, 0);
  
/* Widgets fuer zyklische Darstellung --------------------------------- */

  TB_zyklus7 = XtVaCreateManagedWidget(
                           "TB_zyklus", xmToggleButtonGadgetClass, 
                           XmForm1,
		           "leftAttachment", XmATTACH_FORM,
		           "topAttachment", XmATTACH_FORM,
                           XtVaTypedArg, "fontList",
                           XmRString, "*helvetica-bold-r-*-18-*", 25,
		           0, 0);

  L_anz_zyk8 = XtVaCreateManagedWidget(
                           "L_anz_zyk", xmLabelGadgetClass, 
                           XmForm1,
		           "topAttachment", XmATTACH_FORM,
                           "leftAttachment", XmATTACH_WIDGET,
                           "leftWidget", TB_zyklus7,
		           0, 0);

  T_anz_zyk9 = XtVaCreateManagedWidget(
                            "T_anz_zyk", xmTextFieldWidgetClass, 
                            XmForm1,
                            XmNcolumns, 2,
                            XmNvalue, "2",
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", L_anz_zyk8,
                            "topAttachment", XmATTACH_FORM,
		            0, 0);

  AB_anzahl_plus10 = XtVaCreateManagedWidget(
                            "AB_anzahl_plus", xmArrowButtonWidgetClass,
                            XmForm1,
                            XmNarrowDirection, XmARROW_UP,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", T_anz_zyk9,
                            "topAttachment", XmATTACH_FORM,
                            XmNheight, 20, 
                            0, 0);

  AB_anzahl_minus11 = XtVaCreateManagedWidget(
                            "AB_anzahl_minus", xmArrowButtonWidgetClass,
                            XmForm1,
                            XmNarrowDirection, XmARROW_DOWN,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", T_anz_zyk9,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", AB_anzahl_plus10,
                            "topOffset", 0,
                            XmNheight, 20, 
                            0, 0);

  L_winkel12 = XtVaCreateManagedWidget(
                            "L_winkel", xmLabelGadgetClass, 
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_zyklus7,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", T_anz_zyk9,
		            0, 0);

  T_winkel13 = XtVaCreateManagedWidget(
                            "T_winkel", xmTextFieldWidgetClass, 
                            XmForm1,
                            XmNcolumns, 5,
                            XmNvalue, "45.0",
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", L_anz_zyk8,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", T_anz_zyk9,
		            0, 0);

  L_achse14 = XtVaCreateManagedWidget(
                            "L_achse", xmLabelGadgetClass, 
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_zyklus7,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", T_winkel13,
		            0, 0);

  RC_zyklus15 = XtVaCreateManagedWidget(
                            "RC_zyklus", xmRowColumnWidgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", L_achse14,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", T_winkel13,
                            "radioBehavior", True,
                            "orientation", XmHORIZONTAL,
                            "packing", XmPACK_TIGHT,
                            "borderWidth", 2,
                            0, 0);

  TB_achsex16 = XtVaCreateManagedWidget(
                            "TB_achsex", xmToggleButtonGadgetClass,
                            RC_zyklus15,
	  	            0, 0);

  TB_achsey17 = XtVaCreateManagedWidget(
                            "TB_achsey", xmToggleButtonGadgetClass,
                            RC_zyklus15,
	  	            0, 0);

  TB_achsez18 = XtVaCreateManagedWidget(
                            "TB_achsez", xmToggleButtonGadgetClass,
                            RC_zyklus15,
	  	            0, 0);

  Separator120 = XtVaCreateManagedWidget(
                           "Separator1", xmSeparatorGadgetClass,
                           XmForm1,
		           "separatorType", XmSINGLE_LINE,
		           "leftAttachment", XmATTACH_FORM,
		           "rightAttachment", XmATTACH_FORM,
		           "topAttachment", XmATTACH_WIDGET,
		           "topWidget", RC_zyklus15,
		           0, 0);

/* Widgets fuer Spiegelung -------------------------------------- */

  TB_spiegeln21 = XtVaCreateManagedWidget(
                           "TB_spiegeln", xmToggleButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                            "topWidget", Separator120,
                           XtVaTypedArg, "fontList",
                           XmRString, "*helvetica-bold-r-*-18-*", 25,
                           0, 0);

  L_spiegeln22 = XtVaCreateManagedWidget(
                            "L_spiegeln", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_spiegeln21,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", Separator120,
                            "topOffset", 15,
                            0, 0);

  RC_spiegeln23 = XtVaCreateManagedWidget(
                            "RC_spiegeln", xmRowColumnWidgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_spiegeln21,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_spiegeln22,
                            "radioBehavior", True,
                            "orientation", XmHORIZONTAL,
                            "packing", XmPACK_TIGHT,
                            "borderWidth", 2,
                            0, 0);

  TB_ebenexy24 = XtVaCreateManagedWidget(
                            "TB_ebenexy", xmToggleButtonGadgetClass,
                            RC_spiegeln23,
                            0, 0);

  TB_ebenexz25 = XtVaCreateManagedWidget(
                            "TB_ebenexz", xmToggleButtonGadgetClass,
                            RC_spiegeln23,
                            "leftOffset", 15,
                            0, 0);

  TB_ebeneyz26 = XtVaCreateManagedWidget(
                            "TB_ebeneyz", xmToggleButtonGadgetClass,
                            RC_spiegeln23,
                            0, 0);

   Separator27 = XtVaCreateManagedWidget(
                           "Separator1", xmSeparatorGadgetClass,
                           XmForm1,
                           "separatorType", XmSINGLE_LINE,
                           "leftAttachment", XmATTACH_FORM,
                           "rightAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                           "topWidget", TB_ebeneyz26,
                           0, 0);

/* Widgets fuer Schnitt -------------------------------------- */
/*
  TB_schneiden31 = XtVaCreateManagedWidget(
                           "TB_schneiden", xmToggleButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                           "topWidget", Separator130,
                           XtVaTypedArg, "fontList",
                           XmRString, "*helvetica-bold-r-*-18-*", 25,
                           0, 0);

  TB_schneiden_aus29 = XtVaCreateManagedWidget(
                           "TB_schneiden_aus", xmToggleButtonGadgetClass,
                           XmForm1,
                           "rightAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                           "topWidget", Separator130,
                           XtVaTypedArg, "fontList",
                           XmRString, "*helvetica-bold-r-*-18-*", 25,
                           0, 0);
  TB_def_plane   = XtVaCreateManagedWidget(
                           "TB_def_plane", xmToggleButtonGadgetClass,
			   XmForm1,
                           XmNleftAttachment, XmATTACH_FORM,
                           XmNleftOffset, 50,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, TB_schneiden31,
                           0, 0);  

  TB_def_koord32 = XtVaCreateManagedWidget(
                           "TB_def_koord", xmToggleButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "leftOffset", 50,
                           "topAttachment", XmATTACH_WIDGET,
                            "topWidget", TB_def_plane,
                           0, 0);

  L_koord54 = XtVaCreateManagedWidget(
                            "L_koord", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "leftOffset", 100,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", TB_def_koord32,
                            0, 0);

  L_punkt161 = XtVaCreateManagedWidget(
                            "L_punkt1", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_koord54,
                            "topOffset", 10,
                            0, 0);

  L_punkt262 = XtVaCreateManagedWidget(
                            "L_punkt2", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_punkt161,
                            "topOffset", 10,
                            0, 0);

  L_punkt363 = XtVaCreateManagedWidget(
                            "L_punkt3", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_punkt262,
                            "topOffset", 10,
                            0, 0);

  RC_punkte33 = XtVaCreateManagedWidget(
                            "RC_punkte", xmRowColumnWidgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", L_punkt161,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_koord54,
                            "orientation", XmHORIZONTAL,
                            XmNpacking, XmPACK_COLUMN,
                            XmNnumColumns, 3,
                            "borderWidth", 2,
                            0, 0);

  T_punkt1x41 = XtVaCreateManagedWidget(
                            "T_punkt1x", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt1y42 = XtVaCreateManagedWidget(
                            "T_punkt1y", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt1z43 = XtVaCreateManagedWidget(
                            "T_punkt1z", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt2x44 = XtVaCreateManagedWidget(
                            "T_punkt2x", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "1.0",
                            0, 0);

  T_punkt2y45 = XtVaCreateManagedWidget(
                            "T_punkt2y", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt2z46 = XtVaCreateManagedWidget(
                            "T_punkt2z", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt3x47 = XtVaCreateManagedWidget(
                            "T_punkt3x", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt3y48 = XtVaCreateManagedWidget(
                            "T_punkt3y", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "0.0",
                            0, 0);

  T_punkt3z49 = XtVaCreateManagedWidget(
                            "T_punkt3z", xmTextFieldWidgetClass,
                            RC_punkte33,
                            XmNcolumns, 5,
                            XmNvalue, "1.0",
                            0, 0);

  TB_def_nummer32 = XtVaCreateManagedWidget(
                           "TB_def_nummer", xmToggleButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "leftOffset", 50,
                           "topAttachment", XmATTACH_WIDGET,
                            "topWidget", T_punkt3x47,
                           0, 0);

  L_nummer64 = XtVaCreateManagedWidget(
                            "L_nummer", xmLabelGadgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", TB_def_nummer32,
                            0, 0);

  RC_nummer50 = XtVaCreateManagedWidget(
                            "RC_nummer", xmRowColumnWidgetClass,
                            XmForm1,
                            "leftAttachment", XmATTACH_WIDGET,
                            "leftWidget", TB_schneiden31,
                            "topAttachment", XmATTACH_WIDGET,
                            "topWidget", L_nummer64,
                            "orientation", XmHORIZONTAL,
                            XmNpacking, XmPACK_COLUMN,
                            "borderWidth", 2,
                            0, 0);

  T_punkt1nr51 = XtVaCreateManagedWidget(
                            "T_punkt1nr", xmTextFieldWidgetClass,
                            RC_nummer50,
                            XmNcolumns, 6,
                            0, 0);

  T_punkt2nr52 = XtVaCreateManagedWidget(
                            "T_punkt2nr", xmTextFieldWidgetClass,
                            RC_nummer50,
                            XmNcolumns, 6,
                            0, 0);

  T_punkt3nr53 = XtVaCreateManagedWidget(
                            "T_punkt3nr", xmTextFieldWidgetClass,
                            RC_nummer50,
                            XmNcolumns, 6,
                            0, 0);

  L_discutting = XtVaCreateManagedWidget(
                              "L_dis_cutting", xmLabelGadgetClass,
			      XmForm1,
			      XmNleftAttachment, XmATTACH_WIDGET, 
			      XmNleftWidget, TB_schneiden31,
			      XmNtopAttachment, XmATTACH_WIDGET,
			      XmNtopWidget, RC_nummer50,
			      NULL, 0);

  for ( i = 1; i <= g_geodat.nstoff; i++)
  {
     sprintf(temstr, "%i", i);
     materials[i-1] = XmStringCreateLtoR(temstr, XmFONTLIST_DEFAULT_TAG);
  }
  LS_discutting = XmCreateScrolledList(XmForm1,"LS_dis_cutting",NULL, 0); 

  XtVaSetValues(LS_discutting,
    XmNselectionPolicy, XmBROWSE_SELECT,
    XmNlistSizePolicy, XmVARIABLE,
    XmNitems, materials,
    XmNitemCount, g_geodat.nstoff,
    XmNvisibleItemCount, 3,
    NULL, 0);
*/
  /* get resource from scrolled window */
/*
  LS_discuttingSW = XtParent(LS_discutting);

  XtVaSetValues(LS_discuttingSW,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, L_discutting,
    XmNcolumns, 6,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, RC_nummer50,
    NULL, 0);

  XtManageChild(LS_discutting);

  XtManageChild(LS_discuttingSW);
  */
/*Separator170 = XtVaCreateManagedWidget(
                           "Separator1", xmSeparatorGadgetClass,
                           XmForm1,
                           "separatorType", XmSINGLE_LINE,
                           "leftAttachment", XmATTACH_FORM,
                           "rightAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                           "topWidget",LS_discutting,
                           0, 0);*/

 

  Separator06 = XtVaCreateManagedWidget(
                           "Separator0", xmSeparatorGadgetClass,
                           XmForm1,
		           XmNseparatorType, XmDOUBLE_LINE,
		           XmNleftAttachment, XmATTACH_FORM,
		           XmNrightAttachment, XmATTACH_FORM,
		           XmNbottomAttachment, XmATTACH_WIDGET,
		           XmNbottomWidget, PB_beenden4,
                           0, 0);
  TB_zentrieren71 = XtVaCreateManagedWidget(
                           "TB_zentrieren", xmToggleButtonGadgetClass,
                           XmForm1,
                           "leftAttachment", XmATTACH_FORM,
                           "topAttachment", XmATTACH_WIDGET,
                           "topWidget",Separator27,
			   "bottomAttachment", XmATTACH_WIDGET,
			   "bottomWidget", Separator06,
                           0,0); 
  

  XtAddCallback(XmForm1, "helpCallback",
                (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

 
  XtAddCallback(PB_beenden4, "activateCallback",
      (XtCallbackProc)PB_beenden4_activateCallback, (XtPointer)0);

  
  XtAddCallback(PB_beenden4, "activateCallback",
      (XtCallbackProc)PB_beenden4_activateCallback, (XtPointer)0);

  
  XtAddCallback(PB_hilfe5, "activateCallback",
      (XtCallbackProc)PB_hilfe5_activateCallback, (XtPointer)0);		         
  
  return(S_transform0);
}
