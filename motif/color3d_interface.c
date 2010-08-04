/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname                         | Funktion               */
/* ------------------------------------------------------------- */
/* Createcolor3d_interface              |                        */
/* XmForm1_helpCallback                 |                        */
/* PB_fenster20_activateCallback        |                        */
/* PB_allg_bedienung21_activateCallback |                        */
/* MenuBar2_createCallback              |                        */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>

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

#include "zoom.h"
#include "createmovie.h"
#include "toolbar_back_2.xpm"
#include "toolbar_back_1.xpm"
#include "toolbar_forw_2.xpm" 
#include "toolbar_forw_1.xpm"

#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ArrowB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/ScrolledW.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#ifdef __sgi
#include <Sgm/ThumbWheel.h>
#endif
#include <assert.h>
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
static Widget AS_color3d0, XmForm1, MenuBar2, datei_menu3,
  PB_neu4,
  PB_geop_lesen5, PB_unv_lesen6, PB_loes_lesen45,
  PB_spannung_lesen46, PB_stroemung_lesen47,
  Separator17,
  PB_bild_lesen8, PB_bild_schreiben9, PB_object_lesen, PB_object_schreiben, 
  PB_snapshot10, PB_createmovie,
  Separator011, PB_quit12,
  CB_datei13, CB_stoffe14, CB_transformieren15, CB_2dgrafik16,
  CB_isolinien17, CB_stroem_spann18, extra_menu, PB_shading, PB_colors,
  /*CB_extra,*/hilfe_menu19, PB_fenster20, PB_allg_bedienung21, CB_hilfe22,
  XmForm_b23, L_fusszeile_l24,
  XmForm_r25,
  L_animation26, PB_animation_start127, PB_animation_stop128, 
  L_zeitschritt129, 
  T_zeitschritt229, PB_zeitschritt329, L_zeitschritt_start, L_zeitschritt_end,
  T_zeitschritt_start, T_zeitschritt_end, T_every_frame, L_every_frame, RC_every_frame,
  PB_zeitschritt_vor130, PB_zeitschritt_zurueck131,
  Separator_027, Separator_0271, Separator_0272, 
  Separator_0273, Separator_0274, Separator_0275, 
  L_rot271, RC_view28, RC_zoom, RC_rot281,
  RX_rot_x282, RX_rot_y295, RX_rot_z296,
  RC_rot_pfeil295,
  PB_rot_minus_x283, PB_rot_minus_xx284,
  PB_rot_plus_x285, PB_rot_plus_xx286,
  PB_rot_minus_y287, PB_rot_minus_yy288,
  PB_rot_plus_y289, PB_rot_plus_yy290,
  PB_rot_minus_z291, PB_rot_minus_zz292,
  PB_rot_plus_z293, PB_rot_plus_zz294,
  /*
    PB_rot_plus_x29, PB_rot_minus_x30,
    PB_rot_plus_y31, PB_rot_minus_y32,
    PB_rot_plus_z33, PB_rot_minus_z34,
*/
  PB_move_plus_x35, PB_move_minus_x36,
  PB_move_plus_y37, PB_move_minus_y38, 
  PB_move_plus_z39, PB_move_minus_z40, 
  PB_zoom_14x41, PB_zoom_07x42,
  PB_reset43, Frame_da44,
  L_stoffe51,
  PB_alleStoffe_an52, PB_alleStoffe_aus53,
  SC_stoffe, RC_stoffe54, WB_zoom,
  S_colormap60, XmForm61, Frame_cmap62,
  Spin_TB;
/*Widget T_zeitschritt229;*/
/*****************************************************************/

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

typedef struct 
{
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
help_handler((int)1000000);
}


static void PB_fenster20_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
     widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
#endif
{
help_handler((int)1000000);
}


static void PB_allg_bedienung21_activateCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
     widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
#endif
{
help_handler((int)1000001);
}

static void MenuBar2_createCallback(
#if NeedFunctionPrototypes
Widget widget, XtPointer client_data, XtPointer call_data)
#else
     widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
#endif
{ 
FaceVariable _r0;
Arg _arg;
_r0.value = (int) CB_hilfe22;
XtSetArg(_arg, "menuHelpWidget", _r0.value);
XtSetValues((Widget)MenuBar2, &_arg, 1);
}

Widget Createcolor3d_interface(
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
String app_name, app_class;

XtGetApplicationNameAndClass(XtDisplay(parent), &app_name, &app_class);

AS_color3d0 = XtVaAppCreateShell(app_name,
  app_class, applicationShellWidgetClass, XtDisplay(parent),
  "minWidth", 1000,
  "minHeight", 840,
  "x", 80,
  "y", 120,
  (num_user_args > 0 ? user_args[0].name : (String)0), (num_user_args > 0 ? user_args[0].value : 0),
  (num_user_args > 1 ? user_args[1].name : (String)0), (num_user_args > 1 ? user_args[1].value : 0),
  (num_user_args > 2 ? user_args[2].name : (String)0), (num_user_args > 2 ? user_args[2].value : 0),
  (num_user_args > 3 ? user_args[3].name : (String)0), (num_user_args > 3 ? user_args[3].value : 0),
  (num_user_args > 4 ? user_args[4].name : (String)0), (num_user_args > 4 ? user_args[4].value : 0),
  0, 0);

if(num_user_args > 5)
     printf("Warning: too many arguments (max is 5: use -coption 6 or 7)\n");

     XmForm1 = XtVaCreateManagedWidget("XmForm", xmFormWidgetClass,
       AS_color3d0,
       0, 0);
     XtAddCallback(XmForm1, "helpCallback", 
       (XtCallbackProc)XmForm1_helpCallback, (XtPointer)0);

     MenuBar2 = XmVaCreateSimpleMenuBar(XmForm1, "MenuBar",
       XmNrightAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_FORM,
       XmNleftAttachment, XmATTACH_FORM,
       NULL, 0);

     datei_menu3 = XmVaCreateSimplePulldownMenu(MenuBar2,
       "datei_menu", -1, 0,
       NULL, 0);

     PB_neu4 = XtVaCreateManagedWidget("PB_neu",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_geop_lesen5 = XtVaCreateManagedWidget("PB_geop_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_unv_lesen6 = XtVaCreateManagedWidget("PB_unv_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_loes_lesen45 = XtVaCreateManagedWidget("PB_loes_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_spannung_lesen46 = XtVaCreateManagedWidget("PB_spannung_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_stroemung_lesen47 = XtVaCreateManagedWidget("PB_stroemung_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     Separator17 = XtVaCreateManagedWidget("Separator1",
       xmSeparatorWidgetClass, datei_menu3,
       NULL, 0);

     PB_bild_lesen8 = XtVaCreateManagedWidget("PB_bild_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_bild_schreiben9 = XtVaCreateManagedWidget("PB_bild_schreiben",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

	 PB_object_lesen = XtVaCreateManagedWidget("PB_object_lesen",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

	 PB_object_schreiben = XtVaCreateManagedWidget("PB_object_schreiben",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);


     PB_snapshot10 = XtVaCreateManagedWidget("PB_snapshot",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     PB_createmovie = XtVaCreateManagedWidget("PB_createmovie",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     Separator011 = XtVaCreateManagedWidget("Separator0",
       xmSeparatorWidgetClass, datei_menu3,
       XmNseparatorType, XmDOUBLE_LINE,
       NULL, 0);

     PB_quit12 = XtVaCreateManagedWidget("PB_quit",
       xmPushButtonWidgetClass, datei_menu3,
       NULL, 0);

     CB_datei13 = XtVaCreateManagedWidget("CB_datei",
       xmCascadeButtonWidgetClass, MenuBar2,
       XmNsubMenuId, datei_menu3,
       NULL, 0);

     CB_stoffe14 = XtVaCreateManagedWidget("CB_stoffe",
       xmCascadeButtonWidgetClass, MenuBar2,
       NULL, 0);

     CB_transformieren15 = XtVaCreateManagedWidget("CB_transformieren",
       xmCascadeButtonWidgetClass, MenuBar2,
       NULL, 0);

     CB_2dgrafik16 = XtVaCreateManagedWidget("CB_2dgrafik",
       xmCascadeButtonWidgetClass, MenuBar2,
       NULL, 0);

     CB_isolinien17 = XtVaCreateManagedWidget("CB_isolinien",
       xmCascadeButtonWidgetClass, MenuBar2,
       NULL, 0);

     CB_stroem_spann18 = XtVaCreateManagedWidget("CB_stroem_spann",
       xmCascadeButtonWidgetClass, MenuBar2,
       NULL, 0);
 
     extra_menu = XmVaCreateSimplePulldownMenu(MenuBar2, "extra_menu",
       -1, 0, NULL, 0);
     
     PB_shading = XtVaCreateManagedWidget("PB_shading",
       xmPushButtonWidgetClass, extra_menu,
       NULL, 0);

     PB_colors = XtVaCreateManagedWidget("PB_colors",
       xmPushButtonWidgetClass, extra_menu,
       NULL, 0);

     /*CB_extra = XtVaCreateManagedWidget("CB_extra",
       xmCascadeButtonWidgetClass, MenuBar2,
       XmNsubMenuId, extra_menu,
       NULL, 0);*/

     initWidgetTree(MenuBar2);

     hilfe_menu19 = XmVaCreateSimplePulldownMenu(MenuBar2,
       "hilfe_menu", -1, 0,
       NULL, 0);

     PB_fenster20 = XtVaCreateManagedWidget("PB_fenster",
       xmPushButtonWidgetClass, hilfe_menu19,
       NULL, 0);
     XtAddCallback(PB_fenster20, "activateCallback",
       (XtCallbackProc)PB_fenster20_activateCallback,
       (XtPointer)0);

     PB_allg_bedienung21 = XtVaCreateManagedWidget("PB_allg_bedienung",
       xmPushButtonWidgetClass, hilfe_menu19,
       NULL, 0);
     XtAddCallback(PB_allg_bedienung21, "activateCallback",
       (XtCallbackProc)PB_allg_bedienung21_activateCallback,
       (XtPointer)0);

     CB_hilfe22 = XtVaCreateManagedWidget("CB_hilfe",
       xmCascadeButtonWidgetClass, MenuBar2,
       XmNsubMenuId, hilfe_menu19,
       NULL, 0);

     XtManageChild(MenuBar2);
     /*NDEBUG*/
     MenuBar2_createCallback(MenuBar2, (XtPointer)0, 0);

     XmForm_b23 = XtVaCreateManagedWidget("XmForm_b",
       xmFormWidgetClass, XmForm1,
       XtVaTypedArg, XmNbackground, XmRString, "white", 6,
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNbottomAttachment, XmATTACH_FORM,
       XmNresizePolicy, XmRESIZE_NONE,
       XmNshadowThickness, 2,
       XmNshadowType, XmSHADOW_ETCHED_IN,
       NULL, 0);
/*
     L_fusszeile_l24 = XtVaCreateManagedWidget("L_fusszeile_l",
       xmLabelGadgetClass, XmForm_b23,
       XmNalignment, XmALIGNMENT_BEGINNING,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_POSITION,
       XmNrightPosition, 50,
       XmNbottomAttachment, XmATTACH_FORM,
       XmNmarginHeight, 6,
       XmNmarginLeft, 3,
       NULL, 0);
*/
     XmForm_r25 = XtVaCreateManagedWidget("XmForm_r",
       xmFormWidgetClass, XmForm1,
       XmNrightAttachment, XmATTACH_FORM,
       XmNbottomAttachment, XmATTACH_WIDGET,
       XmNbottomWidget, XmForm_b23,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, MenuBar2,
       NULL, 0);

     L_animation26 = XtVaCreateManagedWidget("L_animation",
       xmLabelGadgetClass, XmForm_r25,
       XmNalignment, XmALIGNMENT_BEGINNING,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_FORM,
       XmNtopOffset, 5,
       XmNmarginHeight, 6,
       XmNmarginLeft, 3,
       NULL, 0);

     PB_animation_start127 = XtVaCreateManagedWidget("PB_animation_start",
       xmPushButtonWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, L_animation26,
       XmNleftOffset, 10,
       XmNtopAttachment, XmATTACH_FORM,
       XmNtopOffset, 5,
       NULL, 0);

     PB_animation_stop128 = XtVaCreateManagedWidget("PB_animation_stop",
       xmPushButtonWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, PB_animation_start127,
       XmNleftOffset, 10,
       XmNtopAttachment, XmATTACH_FORM,
       XmNtopOffset, 5,
       NULL, 0);

     L_zeitschritt129 = XtVaCreateManagedWidget("L_zeitschritt",
       xmLabelGadgetClass, XmForm_r25,
       XmNalignment, XmALIGNMENT_BEGINNING,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, PB_animation_start127,
       XmNmarginHeight, 6,
       XmNmarginLeft, 3,
       NULL, 0);

     assert(XtIsWidget (XmForm_r25));
     /*T_zeitschritt229 = XtVaCreateManagedWidget("T_zeitschritt",
       xmPushButtonWidgetClass,
       XmForm_r25,
       XmNx, 200,
       XmNy, 240,
       NULL);
     */
   

     /*
       T_zeitschritt229 = XtVaCreateManagedWidget(
       "T_zeitschritt",
       xmLabelWidgetClass,XmForm_r25 ,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XmNcolumns, 8,
       XmNheight, 60,
       XmNwidth, 200,
       XmNleftAttachment, XmATTACH_FORM,
       XmNleftOffset, 5,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_zeitschritt129,
       XmNtopOffset, 5,
       NULL, 0);
     */
     
     L_zeitschritt_start = XtVaCreateManagedWidget("L_zeitschritt_start",
       xmLabelWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_zeitschritt129, 
       XmNtopOffset, 10,
       NULL, 0);
     
     T_zeitschritt_start = XmCreateTextField(XmForm_r25, "T_zeitschritt_start", NULL,0);
     XtVaSetValues(T_zeitschritt_start,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, L_zeitschritt_start, 
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_zeitschritt129,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2, 
       NULL, 0);
     XtManageChild(T_zeitschritt_start);

     L_zeitschritt_end = XtVaCreateManagedWidget("L_zeitschritt_end",
       xmLabelWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, T_zeitschritt_start,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_zeitschritt129, 
       XmNtopOffset, 10,
       NULL, 0);

     T_zeitschritt_end = XmCreateTextField( XmForm_r25, "T_zeitschritt_end", NULL,0);
      XtVaSetValues(T_zeitschritt_end,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, PB_animation_start127,
       XmNleftOffset, 10,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_zeitschritt129,  
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL, 0);
     XtManageChild(T_zeitschritt_end);

     RC_every_frame  = XtVaCreateManagedWidget( "RC_every_frame",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM ,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, T_zeitschritt_start,
       XmNorientation, XmHORIZONTAL,
       XmNpacking, XmPACK_TIGHT,
       NULL, 0);

     L_every_frame = XtVaCreateManagedWidget( "L_every_frame", 
       xmLabelWidgetClass, RC_every_frame,
       XmNleftAttachment, XmATTACH_FORM ,
       // XmNtopAttachment, XmATTACH_WIDGET,
       //XmNtopWidget, T_zeitschritt_start, 
       NULL, 0);

     T_every_frame = XmCreateTextField( RC_every_frame, "T_every_frame", NULL,0);
      XtVaSetValues(T_every_frame,
	//XmNleftAttachment, XmATTACH_WIDGET,
	//XmNleftWidget,PB_animation_start127, 
    //XmNleftOffset, 10,
	//XmNtopAttachment, XmATTACH_WIDGET,
	//XmNtopWidget, T_zeitschritt_start,  
       XmNcolumns, 4,
       XmNvalue, "1",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL, 0);
     XtManageChild(T_every_frame);
  
     T_zeitschritt229 = XmCreateTextField(XmForm_r25, "T_zeitschritt", NULL, 0);
     XtVaSetValues( T_zeitschritt229,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XmNleftAttachment, XmATTACH_FORM,
       XmNleftOffset, 5,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget,RC_every_frame,
       XmNtopOffset, 5,
       NULL,0);
     XtManageChild(T_zeitschritt229);

     PB_zeitschritt329 = XtVaCreateManagedWidget("PB_zeitschritt",
       xmPushButtonWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, T_zeitschritt229,
       XmNleftOffset, 5,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, RC_every_frame,
       XmNtopOffset, 5,
       NULL, 0);
   
     PB_zeitschritt_zurueck131 = XtVaCreateManagedWidget("PB_zeitschritt_zurueck",
       xmArrowButtonWidgetClass, XmForm_r25,
       XmNarrowDirection, XmARROW_LEFT,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, L_animation26,
       XmNleftOffset, 10,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, PB_animation_start127,
       XmNtopOffset, 5,
       NULL, 0);

     PB_zeitschritt_vor130 = XtVaCreateManagedWidget("PB_zeitschritt_vor",
       xmArrowButtonWidgetClass, XmForm_r25,
       XmNarrowDirection, XmARROW_RIGHT,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, PB_zeitschritt_zurueck131,
       XmNleftOffset, 10,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, PB_animation_start127,
       XmNtopOffset, 5,
       NULL, 0);

     Separator_027 = XtVaCreateManagedWidget("Separator_0",
       xmSeparatorWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, T_zeitschritt229,
       XmNtopOffset, 5,
       NULL, 0);


     /* neue Widgets fuer bessere Steuerung der Rotation ------------------ */
     L_rot271 = XtVaCreateManagedWidget("L_rot",
       xmLabelGadgetClass, XmForm_r25,
       XmNalignment, XmALIGNMENT_BEGINNING,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, Separator_027,
       NULL, 0);

     Spin_TB = XtVaCreateManagedWidget("Spin_rot", 
       xmToggleButtonWidgetClass,
       XmForm_r25,
       XmNleftAttachment,  XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_rot271,
       NULL, 0);

     RC_rot281 = XtVaCreateManagedWidget("RC_rot",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNorientation, XmHORIZONTAL,
       XmNpacking, XmPACK_COLUMN, 
       XmNnumColumns, 3,
       XmNleftAttachment, XmATTACH_FORM,
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, Spin_TB,
       NULL, 0);


#ifdef NDEBUG
     RC_rot281 = XtVaCreateManagedWidget("RC_rot",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNorientation, XmHORIZONTAL,
       XmNpacking, XmPACK_COLUMN,
       XmNnumColumns, 3,
       XmNleftAttachment, XmATTACH_FORM,
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, L_rot271,
       NULL, 0);
#endif
     /*
       RX_rot_x282 = XtCreateManagedWidget("RX_rot_x",
       xmLabelWidgetClass, RC_rot281,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL, 0);
     */



     RX_rot_x282 = XmCreateTextField(RC_rot281, "RX_rot_x", NULL, 0);
     XtVaSetValues(RX_rot_x282,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL,0);

     XtManageChild(RX_rot_x282);
     /*
       RX_rot_y295 = XtVaCreateManagedWidget("RX_rot_y",
       xmLabelWidgetClass, RC_rot281,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       0, 0);
     */

     RX_rot_y295 = XmCreateTextField(RC_rot281, "RX_rot_y", NULL, 0);
     XtVaSetValues(RX_rot_y295,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL,0);
     XtManageChild(RX_rot_y295);

     /*
       RX_rot_z296 = XtVaCreateManagedWidget("RX_rot_z",
       xmLabelWidgetClass, RC_rot281,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       0, 0);

     */

     RX_rot_z296 =  XmCreateTextField(RC_rot281, "RX_rot_z", NULL, 0);
     XtVaSetValues(RX_rot_z296,
       XmNcolumns, 8,
       XmNvalue, "0.0",
       XtVaTypedArg, "columns", XmRString, "4", 2,
       NULL,0);
     XtManageChild(RX_rot_z296);

     RC_rot_pfeil295 = XtVaCreateManagedWidget("RC_rot_pfeil",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNorientation, XmHORIZONTAL,
       XmNpacking, XmPACK_COLUMN,
       XmNnumColumns, 3,
       XmNmarginHeight, 0,
       XmNspacing, 0, 
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget,  Spin_TB,
       XmNrightAttachment, XmATTACH_FORM,
       NULL, 0);


     PB_rot_minus_xx284 = XtVaCreateManagedWidget("PB_rot_minus_xx",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
	 InstallPixmap(PB_rot_minus_xx284, toolbar_forw_2_xpm);

     PB_rot_minus_x283 = XtVaCreateManagedWidget("PB_rot_minus_x",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_minus_x283, toolbar_forw_1_xpm);

     PB_rot_plus_x285 = XtVaCreateManagedWidget("PB_rot_plus_x",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
	 InstallPixmap(PB_rot_plus_x285, toolbar_back_1_xpm);

     PB_rot_plus_xx286 = XtVaCreateManagedWidget("PB_rot_plus_xx",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_plus_xx286, toolbar_back_2_xpm);

     PB_rot_minus_yy288 = XtVaCreateManagedWidget("PB_rot_minus_yy",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_minus_yy288, toolbar_forw_2_xpm);

     PB_rot_minus_y287 = XtVaCreateManagedWidget("PB_rot_minus_y",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
	 InstallPixmap(PB_rot_minus_y287, toolbar_forw_1_xpm);

     PB_rot_plus_y289 = XtVaCreateManagedWidget("PB_rot_plus_y",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
	 InstallPixmap(PB_rot_plus_y289, toolbar_back_1_xpm);

     PB_rot_plus_yy290 = XtVaCreateManagedWidget("PB_rot_plus_yy",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
      InstallPixmap(PB_rot_plus_yy290, toolbar_back_2_xpm);

     PB_rot_minus_zz292 = XtVaCreateManagedWidget("PB_rot_minus_zz",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_minus_zz292, toolbar_forw_2_xpm);

     PB_rot_minus_z291 = XtVaCreateManagedWidget("PB_rot_minus_z",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
	 InstallPixmap(PB_rot_minus_z291, toolbar_forw_1_xpm);

     PB_rot_plus_z293 = XtVaCreateManagedWidget("PB_rot_plus_z",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_plus_z293, toolbar_back_1_xpm);

     PB_rot_plus_zz294 = XtVaCreateManagedWidget("PB_rot_plus_zz",
       xmPushButtonWidgetClass, RC_rot_pfeil295,
       NULL, 0);
     InstallPixmap(PB_rot_plus_zz294, toolbar_back_2_xpm);

     Separator_0271 = XtVaCreateManagedWidget("Separator_0",
       xmSeparatorWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, RC_rot281,
       XmNtopOffset, 5,
       NULL, 0);

     RC_view28 = XtVaCreateManagedWidget("RC_view",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNpacking, XmPACK_COLUMN,
       XmNnumColumns, 2,
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, RC_rot281, 
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, Separator_0271,
       XmNrightAttachment, XmATTACH_FORM,
       NULL, 0);

     PB_move_plus_x35 = XtVaCreateManagedWidget("PB_move_plus_x",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     PB_move_plus_y37 = XtVaCreateManagedWidget("PB_move_plus_y",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     PB_move_plus_z39 = XtVaCreateManagedWidget("PB_move_plus_z",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     PB_move_minus_x36 = XtVaCreateManagedWidget("PB_move_minus_x",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     PB_move_minus_y38 = XtVaCreateManagedWidget("PB_move_minus_y",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     PB_move_minus_z40 = XtVaCreateManagedWidget("PB_move_minus_z",
       xmPushButtonWidgetClass, RC_view28,
       NULL, 0);

     Separator_0273 = XtVaCreateManagedWidget("Separator_0",
       xmSeparatorWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, RC_view28,
       XmNtopOffset, 5,
       NULL, 0);
#ifdef __sgi
     WB_zoom  = SgCreateThumbWheel(XmForm_r25, "WB_zoom", NULL, 0);
     XtVaSetValues( WB_zoom,
               XmNleftAttachment, XmATTACH_FORM,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, Separator_0273,
               SgNangleRange, 900,
               SgNhomePosition, 500,
               XmNmaximum, 1000,
               XmNminimum, 0,
               NULL, 0);
     XtManageChild( WB_zoom );   
#endif 
	
     RC_zoom = XtVaCreateManagedWidget("RC_zoom",
       xmRowColumnWidgetClass, XmForm_r25,
       XmNpacking, XmPACK_COLUMN,
       XmNnumColumns, 2,
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, Separator_0273,
       XmNrightAttachment, XmATTACH_FORM,
       NULL, 0);

     PB_zoom_14x41 = XtVaCreateManagedWidget("PB_zoom_14x",
       xmPushButtonWidgetClass, RC_zoom,
       NULL, 0);

     InstallLabeledPixmap(PB_zoom_14x41, zoominf);

     PB_zoom_07x42 = XtVaCreateManagedWidget("PB_zoom_07x",
       xmPushButtonWidgetClass, RC_zoom,
       NULL, 0);
     
     InstallLabeledPixmap(PB_zoom_07x42, zoomoutf);

     Separator_0275 = XtVaCreateManagedWidget("Separator_0",
       xmSeparatorWidgetClass, XmForm_r25,
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, WB_zoom,
       XmNtopOffset, 5,
       NULL, 0);

     L_stoffe51 = XtVaCreateManagedWidget("L_stoffe",
       xmLabelGadgetClass, XmForm_r25,
       XmNalignment, XmALIGNMENT_BEGINNING,
       XmNleftAttachment, XmATTACH_FORM,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, Separator_0275,
       XmNtopOffset, 5,
       XmNmarginHeight, 6,
       XmNmarginLeft, 3,
       NULL, 0);

     PB_alleStoffe_an52 = XtVaCreateManagedWidget(
"PB_alleStoffe_an",
  xmPushButtonWidgetClass, XmForm_r25,
  XmNleftAttachment, XmATTACH_WIDGET,
  XmNleftWidget, L_stoffe51,
  XmNleftOffset, 10,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, Separator_0275,
  XmNtopOffset, 5,
  NULL, 0);

PB_alleStoffe_aus53 = XtVaCreateManagedWidget(
"PB_alleStoffe_aus",
  xmPushButtonWidgetClass, XmForm_r25,
  XmNleftAttachment, XmATTACH_WIDGET,
  XmNleftWidget, PB_alleStoffe_an52,
  XmNleftOffset, 10,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, Separator_0275,
  XmNtopOffset, 5,
  NULL, 0);

PB_reset43 = XtVaCreateManagedWidget("PB_reset",
  xmPushButtonWidgetClass, XmForm_r25,
  XmNrightAttachment, XmATTACH_FORM,
  XmNbottomAttachment, XmATTACH_WIDGET,
  XmNbottomWidget, XmForm_b23,
  NULL, 0);

SC_stoffe = XtVaCreateManagedWidget("SC_stoffe",
  xmScrolledWindowWidgetClass, XmForm_r25,
  XmNscrollingPolicy, XmAUTOMATIC,
  XmNvisualPolicy, XmVARIABLE,
  XmNscrollBarDisplayPolicy, XmAS_NEEDED,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, PB_alleStoffe_aus53,
  XmNbottomAttachment, XmATTACH_WIDGET,
  XmNbottomWidget, PB_reset43,
  XmNrightAttachment, XmATTACH_FORM,
  XmNleftAttachment, XmATTACH_FORM,
  NULL, 0);

RC_stoffe54 = XtVaCreateManagedWidget("RC_stoffe",
  xmRowColumnWidgetClass, SC_stoffe /*XmForm_r25*/,
  XmNorientation, XmVERTICAL,
  XmNpacking, XmPACK_COLUMN,
  XmNnumColumns, 4,
  XmNadjustLast, False,
  XmNentryAlignment, XmALIGNMENT_CENTER,
  XmNisAligned, False, 
  NULL, 0);

Frame_da44 = XtVaCreateManagedWidget("Frame_da",
  xmFrameWidgetClass, XmForm1,
  XmNtopAttachment, XmATTACH_WIDGET,
  XmNtopWidget, MenuBar2,
  XmNbottomAttachment, XmATTACH_WIDGET,
  XmNbottomWidget, XmForm_b23,
  XmNrightAttachment, XmATTACH_WIDGET,
  XmNrightWidget, XmForm_r25,
  XmNleftAttachment, XmATTACH_FORM,
  NULL, 0);

S_colormap60 = XtVaCreatePopupShell(
"S_colormap", transientShellWidgetClass, XmForm1,
  XmNminWidth, 160,
  XmNminHeight, 400,
  XmNx, 10,
  XmNy, 200,
  NULL, 0);
/*XtPopup(S_colormap60);
  XtPopdown(S_colormap60);*/

XmForm61 = XtVaCreateManagedWidget("XmForm",
  xmFormWidgetClass, S_colormap60,
  XmNhorizontalSpacing, 0,
  XmNverticalSpacing, 0,
  NULL, 0);

Frame_cmap62 = XtVaCreateManagedWidget("Frame_cmap",
  xmFrameWidgetClass, XmForm61,
  XmNtopAttachment, XmATTACH_FORM,
  XmNbottomAttachment, XmATTACH_FORM,
  XmNleftAttachment, XmATTACH_FORM,
  XmNrightAttachment, XmATTACH_FORM,
  NULL, 0);

/* create movie dialog box */
create_S_CreateMovie (AS_color3d0);

return(AS_color3d0);
}













