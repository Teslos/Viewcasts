/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   motif_var.c                                      */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*              | Definition globale Variablen fuer Motiffenster */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <Xm/Xm.h>
#include <GL/GLwMDrawA.h>
#include "motif.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*************************************************************************/
XtAppContext kontext;   	/* Applikations-Umgebung ------- */
Widget toplevel;		/* Toplevel-Widget ------------- */
Widget gl_widget;		/* DrawingArea-Widget Main ----- */
Widget cmap_widget;             /* DrawingArea-Widget Colormap - */
Widget pick_widget;             /* DrawingArea-Widget Pick-Modus */

XtWorkProcId redisplayID;
int redisplaypending = 0;
GLXContext glx_context;
/*int gl_width = 0, gl_height = 0;*/

ApplResTyp applRessourcen;	/* */
int mode = 0;			/* mode fuer Grafikmanipulation  */
int omx, mx, omy, my;      	/* old and new mouse position -- */
Widget* stoffbuttons;           /* Pointer fuer Stoffbutton ---- */

/* translations_table fuer TextField in TextTable -------------- */
XtTranslations TF_in_TT_translations_table;

/* Variablen fuer Grafikelemente */
t_widget_main widgetMain;
t_widget_stoffe widget_stoffe             = {(Widget)NULL};
t_widget_transform widget_transform       = {(Widget)NULL};
t_widget_2dgrafik widget2DGrafik          = {(Widget)NULL};
t_widget_isolinien widget_isolinien       = {(Widget)NULL};
t_widget_vektor widget_vektor             = {(Widget)NULL};
t_widget_xy_diagramm widget_xy_diagramm   = {(Widget)NULL};
t_widget_loesung widget_loesung           = {(Widget)NULL};
t_widget_cmap widget_cmap                 = {(Widget)NULL};
t_widget_pick widget_pick                 = {(Widget)NULL};
/*************************************************************************/
