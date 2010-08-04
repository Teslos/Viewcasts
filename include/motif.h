/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   15.02.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Includefile fuer Motif-Fenster :                              */
/* ==========                                                    */       
/*                                                               */
/* ************************************************************* */
#ifndef _motif_h
#define _motif_h

#include <stdio.h>
#include <Xm/Xm.h>

#include <X11/cursorfont.h>
#include <GL/gl.h>
#include <GL/GLwMDrawA.h>

#include "widgets.h"
#include "hilfs_func.h"

#ifndef BOOL_DEFINED
typedef int t_bool;
#define TRUE   1
#define FALSE  0
#define BOOL_DEFINED
#endif /* #ifndef BOOL_DEFINED */

/* ********************************************************************* */
/* Cursor Symbols ------------------------------------------------------ */
#define TRANSLATE_CURSOR             XC_fleur
#define ROTATE_CURSOR                XC_exchange
#define ZOOM_CURSOR                  XC_ul_angle
#define PICK_CURSOR                  XC_crosshair
#define WAIT_CURSOR                  XC_coffee_mug
/* ********************************************************************* */
/* forward declarations ------------------------------------------------ */
void help_handler(int nummer);
void warning(char *message_string);
void set_watch_cursor_handler(Widget widget);
void set_cursor_handler(Widget widget, int cursor);
void reset_cursor_handler(Widget widget);
extern int create_da(Widget top, t_bool *dpy_has_gl);
void popMenuCreate(Widget parent);
char *file_dir_auswahl(char *header, char *header1, char *dirmask, int dir_only,
                       int create_only, int create_list);
char *ask_user(char *text, char *ans1, char *ans2, char *ans3,
               int default_ans);
int text_formatieren(Widget w_text, int n_felder);
int test_function(int test_param);

int read_datafile(char *datei);

void InstallLabeledPixmap(Widget w, char **xpmDescription);
void InstallPixmap(Widget w, char **xpmDescription);
void C3D_setwindowtitle ( Display *, Window, const char* );

/* Erzeugung der Oberflaeche ------------------------------------------- */
Widget Createcolor3d_interface(String name, Widget parent, Arg* user_args,
                               Cardinal num_user_args);
int init_mainW(void);

/* stoffe -------------------------------------------------------------- */
Widget Createstoffe_interface(String name, Widget parent, Arg* user_args,
                              Cardinal num_user_args);
int init_stoffgruppen(void);

/* loesung ------------------------------------------------------------- */
Widget Createloesung_interface(String name, Widget parent, Arg* user_args,
                               Cardinal num_user_args);
int init_loesung(void);

/* transform ----------------------------------------------------------- */
Widget Createtransform_interface(String name, Widget parent, Arg* user_args,
                                 Cardinal num_user_args);

/* Isolinien ----------------------------------------------------------- */
Widget Createisolinien_interface(String name, Widget parent, Arg* user_args,
                                 Cardinal num_user_args);

/* Picking ------------------------------------------------------------- */
Widget Createpicking_interface(String name, Widget parent, Arg* user_args,
                               Cardinal num_user_args);
/* 2d grafik ----------------------------------------------------------- */
Widget Create2dgrafik_interface(String name, Widget parent, Arg* user_args, 
				Cardinal num_user_args);
/* Vektor interface----------------------------------------------------- */
Widget Createvektor_interface(String name, Widget parent, Arg* user_args, 
			      Cardinal num_user_args);
/* Color chooser ------------------------------------------------------- */
Widget CreateColorChooser(String name, Widget parent, Arg* user_args, 
			  Cardinal num_user_args);

int init_transform(void);
int init_cut(void);
/* Extra interface------------------------------------------------------ */
int init_extra(void);

void action_set_statusline(Widget widget, XEvent* eventAdr, String* params,
                           Cardinal* paramsAnzAdr);
void traverse_current(Widget widget, XEvent* eventAdr, String* params,
                      Cardinal* paramsAnzAdr);
void focus_out_TF_in_TT(Widget widget, XEvent* eventAdr, String* params,
                        Cardinal* paramsAnzAdr);
void start_move(Widget widget, XEvent* eventAdr, String* params,
                Cardinal* paramsAnzAdr);
void end_move(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr);
void move(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr);
void start_rot(Widget widget, XEvent* eventAdr, String* params,
               Cardinal* paramsAnzAdr);
void end_rot(Widget widget, XEvent* eventAdr, String* params,
             Cardinal* paramsAnzAdr);
void start_zoom(Widget widget, XEvent* eventAdr, String* params,
                Cardinal* paramsAnzAdr);
void end_zoom(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr);
void popup_menue_ausgeben(Widget widget, XEvent* eventAdr, String* params,
                          Cardinal* paramsAnzAdr);
void start_pick(Widget widget, XEvent* eventAdr, String* params,
                Cardinal* paramsAnzAdr);
void end_pick(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr);
/* 2dgrafik------------------------------------------------------------- */
Widget CreateWin2d(Widget parent);

/* ********************************************************************* */
/* Klassenname der Applikation ----------------------------------------- */
#define APPL_KLASSE "Viewcasts"
/* ********************************************************************* */
#define LEFT_STATUSLINE 1
/* ********************************************************************* */
extern XtAppContext kontext;      /* Applikations-Umgebung ------------- */
extern char help_dir_buffer[1000];
extern XtWorkProcId redisplayID;
extern int redisplaypending;
extern Widget toplevel;
extern Widget gl_widget;	  /* DrawingArea-Widget Main ----------- */
extern Widget cmap_widget;	  /* DrawingArea-Widget Colormap ------- */
extern Widget pick_widget;	  /* DrawingArea-Widget Picking-Modus -- */
extern GLXContext glx_context;
extern t_bool debug;
extern t_bool cutting_on;
extern int n_datasets;            /* Anzahl Universalfile-Datasets ----- */
extern int mode;		  /* mode fuer Grafikmanipulation ------ */
extern int omx, mx, omy, my;      /* old and new mouse position -------- */
extern Widget* stoffbuttons;	  /* Pointer fuer Stoffbuttons --------- */

/* translations_table fuer TextField in TextTable */
extern XtTranslations TF_in_TT_translations_table; 

/* Variablen fuer Grafikelemente */
extern t_widget_main      widgetMain;
extern t_widget_stoffe    widget_stoffe;
extern t_widget_transform widget_transform;
extern t_widget_loesung   widget_loesung;
extern t_widget_isolinien widget_isolinien;
extern t_widget_cmap      widget_cmap;
extern t_widget_2dgrafik  widget2DGrafik;
extern t_widget_vektor    widget_vektor;
extern Widget             cutShell;

/* Datentyp fuer Applikations-Ressourcen ------------------------------- */
typedef struct
{
  char    *version;
  Boolean debug;
  Boolean debug_gl;
  Boolean nosubmit;
  Boolean texture;
  char    *help_dir;
  char    *print_string;
  char    *unv_dir;
  char    *cst_dir;
  char    *casts_geo_dir;
  char    *casts_loes_dir;
  char    *homogen_color;
  char    *inhomogen_color;
}ApplResTyp;
extern ApplResTyp applRessourcen;
/* ********************************************************************* */
#endif   /* #ifndef _motif_h */





