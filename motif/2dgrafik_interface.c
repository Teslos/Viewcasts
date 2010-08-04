/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   08.05.98                                         */
/* Dateiname:   2d_grafik_interface.c                               */
/*                                                               */
/* Routinenname                 | Funktion                       */
/* ------------------------------------------------------------- */
/* Create2d_grafik_interface    |                                */
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
#include <Xm/Frame.h>
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

static Widget S_2dgrafik0, XmForm1, PB_beenden4, PB_hilfe6, Separator07,
  L_eingabe11, RC_eingabe12, TB_pick13, TB_manuell14, 
  L_ausgabe21, RC_ausgabe22, TB_abkuehlkurve23, TB_ausgabe24,
  Separator131, TT_picking32, TT_picking33, TT_picking34, TT_rowColumn,
  RC_zeichne33, PB_zeichne34, PB_loesche35, PB_calculate36,
  PB_addpoint, Separator140, RC_achsen41,
  L_ordinate42, L_omin43, T_omin44, L_omax45, T_omax46,
  L_abszisse47, L_amin48, T_amin49, L_amax50, T_amax51;

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
XtPopdown((Widget)S_2dgrafik0);
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
help_handler((int)1030000);
}

Widget
Create2dgrafik_interface(
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
/* local variables*/
XmString xmstr;


S_2dgrafik0 = XtVaCreatePopupShell(
                (name ? name : "S_2dgrafik"),
                transientShellWidgetClass, parent,
                XmNminWidth, 400, XmNminHeight, 550,
       	        XmNx, 10, XmNy, 590,
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
	        NULL, 0);

  if(num_user_args > 5)
    printf("Warning:too many arguments (max is 5: use -coption 6 or 7)\n");

  XmForm1 = XtVaCreateManagedWidget(
                           "XmForm", xmFormWidgetClass, 
                           S_2dgrafik0,
		           XmNhorizontalSpacing, 10,
		           XmNverticalSpacing, 10,
		           NULL, 0);

  XtAddCallback(XmForm1, "helpCallback",
                (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

  PB_beenden4 = XtVaCreateManagedWidget(
                           "PB_beenden", xmPushButtonGadgetClass,
                           XmForm1,
		           XmNleftAttachment, XmATTACH_FORM,
	        	   XmNbottomAttachment, XmATTACH_FORM,
		           NULL, 0);
  XtAddCallback(PB_beenden4, XmNactivateCallback,
      (XtCallbackProc)PB_beenden4_activateCallback, (XtPointer)0);

  PB_hilfe6 = XtVaCreateManagedWidget(
                           "PB_hilfe", xmPushButtonGadgetClass,
                           XmForm1,
		           XmNbottomAttachment, XmATTACH_FORM,
		           XmNrightAttachment, XmATTACH_FORM,
	               	   NULL, 0);
  XtAddCallback(PB_hilfe6, XmNactivateCallback,
      (XtCallbackProc)PB_hilfe6_activateCallback, (XtPointer)0);

  Separator07 = XtVaCreateManagedWidget(
                           "Separator0", xmSeparatorGadgetClass,
                           XmForm1,
		           XmNseparatorType, XmDOUBLE_LINE,
		           XmNleftAttachment, XmATTACH_FORM,
		           XmNrightAttachment, XmATTACH_FORM,
		           XmNbottomAttachment, XmATTACH_WIDGET,
		           XmNbottomWidget, PB_beenden4,
		           0, 0);

  L_eingabe11 = XtVaCreateManagedWidget(
                            "L_eingabe", xmLabelGadgetClass,
                            XmForm1,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNtopOffset, 15,
                            XmNleftAttachment, XmATTACH_FORM,
                            NULL, 0);


  RC_eingabe12 = XtVaCreateManagedWidget(
                           "RC_eingabe", xmRowColumnWidgetClass,
                           XmForm1,
                           XmNtopAttachment, XmATTACH_FORM,
                           XmNleftAttachment, XmATTACH_WIDGET,
                           XmNleftWidget, L_eingabe11,
                           XmNradioBehavior, True,
                           XmNorientation, XmVERTICAL,
                           XmNpacking, XmPACK_COLUMN,
                           XmNnumColumns, 1,
                           NULL, 0);

  TB_pick13 = XtVaCreateManagedWidget(
                           "TB_pick", xmToggleButtonWidgetClass,
                           RC_eingabe12,
		           NULL, 0);

  TB_manuell14 = XtVaCreateManagedWidget(
                           "TB_manuell", xmToggleButtonWidgetClass,
                           RC_eingabe12,
		           NULL, 0);

  L_ausgabe21 = XtVaCreateManagedWidget(
                           "L_ausgabe", xmLabelGadgetClass,
                           XmForm1,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, RC_eingabe12,
                           XmNtopOffset, 15,
                           XmNleftAttachment, XmATTACH_FORM,
                           NULL, 0);

  TB_abkuehlkurve23 = XtVaCreateManagedWidget(
                           "TB_abkuehlkurve", xmToggleButtonWidgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_WIDGET,
                           XmNleftWidget, L_ausgabe21,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, RC_eingabe12,
                           NULL, 0);

  TB_ausgabe24 = XtVaCreateManagedWidget(
                           "TB_ausgabe", xmToggleButtonWidgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_WIDGET,
                           XmNleftWidget, L_ausgabe21,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, TB_abkuehlkurve23,
                           NULL, 0);

  RC_ausgabe22 = XtVaCreateManagedWidget(
                           "RC_ausgabe", xmRowColumnWidgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_WIDGET,
                           XmNleftWidget, L_ausgabe21,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, TB_ausgabe24,
                           XmNradioBehavior, True,
                           XmNorientation, XmVERTICAL,
                           XmNpacking, XmPACK_COLUMN,
                           XmNnumColumns, 4,
                           NULL, 0);

  Separator131 = XtVaCreateManagedWidget(
                           "Separator1", xmSeparatorGadgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_FORM,
                           XmNrightAttachment, XmATTACH_FORM,
                           XmNtopAttachment, XmATTACH_WIDGET,
 
                          XmNtopWidget, RC_ausgabe22,
                           NULL, 0);

TT_picking32 = XtVaCreateManagedWidget("TT_picking", bxmTextTableWidgetClass,
  XmForm1, BXmNvisibleRows, 3,
  BXmNnumColumns, 5,
  XmNleftAttachment, XmATTACH_FORM,
  XmNrightAttachment, XmATTACH_FORM,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, Separator131,
  XmNshadowThickness, 1,
  XmNselectionPolicy, BXmSINGLE_SELECT,
  XmNselectionOrientation, BXmHORIZONTAL,
  XmNautoActivate, True,
  BXmNhasHorizontalHeader, True,
  BXmNheaderSelectable, False,
  BXmNautoActivate, True,
  NULL, 0);

TT_rowColumn = XtVaCreateManagedWidget("TT_pickingF", xmRowColumnWidgetClass,
  XmForm1, 
  XmNleftAttachment, XmATTACH_FORM,
  XmNrightAttachment, XmATTACH_FORM,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, Separator131,
  XmNorientation, XmHORIZONTAL,
  XmNentryAlignment, XmALIGNMENT_CENTER,
  NULL,0);

  RC_zeichne33 = XtVaCreateManagedWidget(
                           "RC_zeichne", xmRowColumnWidgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_FORM,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, TT_picking32,
                           XmNorientation, XmHORIZONTAL,
                           XmNpacking, XmPACK_COLUMN,
                           XmNnumColumns, 1,
                           NULL, 0);

  PB_zeichne34 = XtVaCreateManagedWidget(
                           "PB_zeichne", xmPushButtonWidgetClass,
                           RC_zeichne33,
		           NULL, 0);

  PB_loesche35 = XtVaCreateManagedWidget(
                           "PB_loesche", xmPushButtonWidgetClass,
                           RC_zeichne33,
		           NULL, 0);

  PB_calculate36 = XtVaCreateManagedWidget(
                           "PB_calculate", xmPushButtonWidgetClass,
                           RC_zeichne33,
		           NULL, 0);
  		          
  PB_addpoint  = XtVaCreateManagedWidget(
                           "PB_addpoint", xmPushButtonWidgetClass,
                           RC_zeichne33,
		           NULL, 0);

  Separator140 = XtVaCreateManagedWidget(
                           "Separator2", xmSeparatorGadgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_FORM,
                           XmNrightAttachment, XmATTACH_FORM,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, RC_zeichne33,
                           NULL, 0);

  RC_achsen41 = XtVaCreateManagedWidget(
                           "RC_achsen", xmRowColumnWidgetClass,
                           XmForm1,
                           XmNleftAttachment, XmATTACH_FORM,
                           XmNtopAttachment, XmATTACH_WIDGET,
                           XmNtopWidget, Separator140,
                           XmNorientation, XmHORIZONTAL,
                           XmNpacking, XmPACK_COLUMN,
                           XmNnumColumns, 2,
                           NULL, 0);

  L_ordinate42 = XtVaCreateManagedWidget(
                           "L_ordinate", xmLabelGadgetClass,
                           RC_achsen41,
                           NULL, 0);

  L_omin43 = XtVaCreateManagedWidget(
                           "L_omin", xmLabelGadgetClass,
                           RC_achsen41,
                           NULL, 0);

  T_omin44 = XtVaCreateManagedWidget(
                           "T_omin", xmTextFieldWidgetClass,
                           RC_achsen41,
                           XmNcolumns, 5,
			   NULL, 0);
/*
 T_omin44 = XmCreateTextField(RC_achsen41,"T_omin",NULL,0);
 XtVaSetValues(T_omin44,
   XmNcolumns, 5, NULL);
*/

  L_omax45 = XtVaCreateManagedWidget(
                           "L_omax", xmLabelGadgetClass,
                           RC_achsen41,
                           NULL, 0);

  T_omax46 = XtVaCreateManagedWidget(
                           "T_omax", xmTextFieldWidgetClass,
                           RC_achsen41,
                           XmNcolumns, 5,
                           0, 0);


/*
 T_omax46 = XmCreateTextField(RC_achsen41,"T_omax",NULL,0);
 XtVaSetValues(T_omax46,
   XmNcolumns, 5, NULL);
*/

  L_abszisse47 = XtVaCreateManagedWidget(
                           "L_abszisse", xmLabelGadgetClass,
                           RC_achsen41,
                           NULL, 0);

  L_amin48 = XtVaCreateManagedWidget(
                           "L_amin", xmLabelGadgetClass,
                           RC_achsen41,
                           NULL, 0);

  T_amin49 = XtVaCreateManagedWidget(
                           "T_amin", xmTextFieldWidgetClass,
                           RC_achsen41,
                           XmNcolumns, 5,
                           NULL, 0);

/*
 T_amin49 = XmCreateTextField(RC_achsen41,"T_amin",NULL,0);
 XtVaSetValues(T_amin49,
   XmNcolumns, 5, NULL);
*/


  L_amax50 = XtVaCreateManagedWidget(
                           "L_amax", xmLabelGadgetClass,
                           RC_achsen41,
                           0, 0);

  T_amax51 = XtVaCreateManagedWidget(
                           "T_amax", xmTextFieldWidgetClass,
                           RC_achsen41,
                           XmNcolumns, 5,
                           0, 0);

/*
 T_amax51 = XmCreateTextField(RC_achsen41,"T_amin",NULL,0);
 XtVaSetValues(T_amax51,
   XmNcolumns, 5, NULL);
*/


  return(S_2dgrafik0);
}

/**********************************************************
 This function creates the widget tree for display of
 2d 'cooling' curves.
***********************************************************/
Widget CreateWin2d(Widget parent)
{
  /* local widgets*/
  Widget S_abkuehl70,  XmForm71, Frame_abk72;
  
  S_abkuehl70  = XtVaCreatePopupShell("S_abkuehl",
				     transientShellWidgetClass, parent,
				     XmNminWidth, 200,
				     XmNminHeight, 200,
				     XmNx, 10,
				     XmNy, 20,
				     NULL, 0);

  XmForm71     = XtVaCreateManagedWidget("XmForm",
					 xmFormWidgetClass, S_abkuehl70,
					 XmNhorizontalSpacing, 0,
					 XmNverticalSpacing, 0,
					 NULL, 0);

  Frame_abk72  = XtVaCreateManagedWidget("Frame_abk",
					xmFrameWidgetClass, XmForm71,
					XmNtopAttachment, XmATTACH_FORM,
					XmNbottomAttachment, XmATTACH_FORM,
					XmNleftAttachment, XmATTACH_FORM,
					XmNrightAttachment, XmATTACH_FORM,
					NULL, 0);
  return(S_abkuehl70);
}
