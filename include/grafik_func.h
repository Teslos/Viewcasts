#include <GL/GLwMDrawA.h>
#include <GL/glx.h>
#include "strukturen.h"

/************************************************************************ */
/* Callbacks */
void c3d_init(Widget w, XtPointer client_data,
                        XtPointer call);
void c3d_expose(Widget w, XtPointer client_data,
                          XtPointer call);
void c3d_resize(Widget w, XtPointer client_data,
		XtPointer call);
void c3d_input(Widget w, XtPointer client_data,
	       XtPointer call);
GLXContext CreateGLXContext(GLXContext, Display*, XVisualInfo* );
/* ********************************************************************** */

XtCallbackProc da_input(Widget w, XtPointer client_data,
                        XmDrawingAreaCallbackStruct *call_data);
XtCallbackProc da_expose(Widget w, XtPointer client_data, XtPointer cbs);
XtCallbackProc da_resize(Widget w, XtPointer client_data,
                         XmDrawingAreaCallbackStruct *call_data);

/* Zeichenfunktionen */
t_bool C3D_redraw(XtPointer);
void c3d_DrawWindow(int width, int height);
void C3D_clearcolor();

/* ********************************************************************** */
/* Callbacks und Zeichnefunktionene fuer Colormap ----------------------- */
void  cmap_init(Widget w, XtPointer client_data,
		XtPointer call);
void  cmap_expose(Widget w, XtPointer client_data,
		  XtPointer call);
void  cmap_resize(Widget w, XtPointer client_data,
		  XtPointer call);
/* ********************************************************************** */
/* Zeichenfunktionen */
void abk_Redraw(void);
void abk_DrawWindow(int width, int height);

/* ********************************************************************** */
/* Callbacks und Zeichnefunktionene fuer Colormap ----------------------- */
void  abk_init(Widget w, XtPointer client_data,
	       XtPointer call);
void  abk_expose(Widget w, XtPointer client_data,
		 XtPointer call_data);
void  abk_resize(Widget w, XtPointer client_data,
		 XtPointer call_data);
void  abk_close(Widget w, XtPointer client_data,
		 XtPointer call_data);		

/* ********************************************************************** */
/* Zeichenfunktionen */
void abk_Redraw(void);
void abk_DrawWindow(int width, int height);
void schliesse_fenster(int fenster);
/* ********************************************************************** */
/* Modes the program can be in */
#define NOTHING   0
#define ROTATE    1
#define TRANSLATE 2
#define ZOOM      3
#define PICK      4
/* ********************************************************************** */







