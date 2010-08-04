/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/* CreateColorChooser | create color chooser                     */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */  
#include <string.h>
#include <stdio.h>
#include <Xm/Xm.h>
#include <stdlib.h>
#include <X11/Shell.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/XmStrDefs.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef HAVE_SGI
#include "ColorChooser/ColorC.h"
#include "color3d.h"
#include "motif.h"
#include "grafik.h"

void okCallback(Widget w, XtPointer clientData, XtPointer callData);
void cancelCallback(Widget w, XtPointer clientData, XtPointer callData);
void applyCallback(Widget w, XtPointer clientData, XtPointer callData);
void helpCallback(Widget w, XtPointer clientData, XtPointer callData);

/* global variables */
t_color chcolor;

Widget CreateColorChooser(String name, Widget parent, Arg* user_args,
			  Cardinal num_user_args)
{
  Widget colorchooser, color_dialog;

  int col = (int) user_args[0].value;
  color_dialog = XtVaCreatePopupShell((name ? name : "DScolorChooser"),
				      xmDialogShellWidgetClass, parent,
				      XmNminHeight, 400,
				      XmNminWidth, 500,
				      XmNallowShellResize, TRUE,
				      (num_user_args > 1 ? user_args[1].name : (String)0),
				      (num_user_args > 1 ? user_args[1].value : 0),
				      (num_user_args > 2 ? user_args[2].name : (String)0),
				      (num_user_args > 2 ? user_args[2].value : 0),
				      (num_user_args > 3 ? user_args[3].name : (String)0),
				      (num_user_args > 3 ? user_args[3].value : 0),
				      (num_user_args > 4 ? user_args[4].name : (String)0),
				      (num_user_args > 4 ? user_args[4].value : 0),
				      NULL, 0);
				    


  colorchooser = XtVaCreateManagedWidget("colorchooser",
					 sgColorChooserWidgetClass, color_dialog,
					 NULL);
  
#ifdef DEBUG
  printf("col: %i\n", col);
#endif

  /* add callbacks */
  XtAddCallback (colorchooser, XmNokCallback,
		 (XtCallbackProc) okCallback, (XtPointer) col);
  XtAddCallback (colorchooser, XmNcancelCallback,
		 (XtCallbackProc) cancelCallback, NULL );
  XtAddCallback (colorchooser, XmNapplyCallback,
		 (XtCallbackProc) okCallback, (XtPointer) col);
  XtAddCallback (colorchooser, XmNhelpCallback,
		 (XtCallbackProc) helpCallback, NULL );
  return (color_dialog);
}

void okCallback(Widget w, XtPointer clientData, XtPointer callData)
{
  short int r,g,b;
  int pac;
  char color_string[20];
  String name;
  Widget parent;
  XmAnyCallbackStruct *call_data;
  int column = (int) clientData;

#ifdef DEBUG
  printf ("I'am in OK callback\n");
#endif

  call_data = (XmAnyCallbackStruct *) callData;

  SgColorChooserGetColor(w, &r, &g, &b);
  if (call_data->reason == XmCR_OK)
    pac = PACK_COLOR(b,g,r,0);
  else if (call_data->reason == XmCR_APPLY)
    pac = PACK_COLOR(b,g,r,0);

  chcolor.color[0] = r;
  chcolor.color[1] = g;
  chcolor.color[2] = b;

  sprintf(color_string, "#%06x", pac);

  /* release old color string */
  if (chcolor.color_string != NULL)
    free(chcolor.color_string);

  /* find out which widget called the callback */
  parent  = XtParent(w);
  name    = XtName(parent);
 
  chcolor.color_string = strdup(color_string);

  /* give callback to previous widget */
  if (strcmp(name,"S_color_chooser") == 0)
    change_isolinie_col(column);
  else if ( strcmp(name, "S_iso_color_chooser") == 0)
    change_iso_col( column ); 
  else if (strcmp(name,"S_trans_chooser") == 0)
    change_transp_col();
  else if (strcmp(name, "S_cool_chooser") == 0)
    change_cool_curve(column);
  else if (strcmp(name, "S_mat_color_chooser") == 0)
    change_mat_col(column); 
  else
    fprintf(stderr, "Error: color_chooser non valid widget\n");
}
  
void cancelCallback(Widget w, XtPointer clientData, XtPointer callData)
{
#ifdef DEBUG
  printf ("I'am in CANCEL callback\n");
#endif

}  

void applyCallback(Widget w, XtPointer clientData, XtPointer callData)
{
#ifdef DEBUG
  printf ("I'am in APPLY callback\n");
#endif

}

void helpCallback(Widget w, XtPointer clientData, XtPointer callData)
{
#ifdef DEBUG
  printf ("I'am in HELP callback\n");
#endif
  
}
#endif /* HAVE_SGI */
