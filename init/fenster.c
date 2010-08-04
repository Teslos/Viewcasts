/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   27.07.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/* make_fenster_cmap |                                           */
/*     cmap_init     |                                           */
/*     cmap_expose   |                                           */
/*     cmap_resize   |                                           */
/* make_fenster_abk  |                                           */
/*     abk_init      |                                           */
/*     abk_expose    |                                           */
/*     abk_resize    |                                           */
/* schliesse_fenster |                                           */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>
#ifdef HAVE_LINUX
#  include <GL/MesaDrawingArea.h>
#endif
/*#include <GL/glut.h>*/

#include "color3d.h"
#include "motif.h"
#include "grafik.h"
#include "grafik_func.h"

/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
GLfloat light_diffuse[] = {1.0,0.0,0.0,1.0};
GLfloat light_position[] = {1.0,1.0,1.0,0.0};
GLUquadricObj *qObj;

static int dblbuf_12[] =
{GLX_BUFFER_SIZE, 12, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 12, NULL};
static int dblbuf_8[]  = 
{GLX_BUFFER_SIZE,  8, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 12, NULL};
static int sglbuf_12[] = {GLX_BUFFER_SIZE, 12, NULL};
static int sglbuf_8[]  = {GLX_BUFFER_SIZE,  8, NULL};
static int cmap_12[]   = {GLX_BUFFER_SIZE, 12, NULL};
static int cmap_8[]    = {GLX_BUFFER_SIZE,  8, NULL};

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* ************************************************************* */


/* ************************************************************* */
int make_fenster_cmap(Widget top, t_winstruct *fenster)
/* ************************************************************* */
{
  Atom         wm_delete_window;
  Widget       shell;
  Widget       w_frame_cmap = NULL;
  Widget       cmap_widget;
  XVisualInfo *vi;
  Display     *dpy;
  /*GLXContext   cx;  */
  /*XFontStruct *font;*/
  /*Font         fid; */
  /*t_bool       nofont; */   /* Flagge fuer geladenen Font -------- */
  /* ************************************************************* */

  /* Verbindung zum X-Server oeffnen: */
  dpy = XtDisplay(top);

  if (!(w_frame_cmap = XtNameToWidget(top, "*Frame_cmap")))
    {
      fprintf(stderr, "XtNameToWidget(..., \"*Frame_cmap\") returns NULL\n");
      return(1);
    }

  /* benutze Visual und Colormap des Hauptfensters fuer neues Fenster */
  vi   = C3D_win.vi;
  cmap = C3D_win.cmap;

  /* oeffne OpenGl-Grafikfenster im entsprechenden Modus  */
#ifndef HAVE_LINUX
  cmap_widget = XtVaCreateManagedWidget("glwidget",
                                        glwMDrawingAreaWidgetClass,
                                        w_frame_cmap,
                                        GLwNvisualInfo, vi,
                                        XtNcolormap, cmap,
                                        NULL);
#else
   cmap_widget = XtVaCreateManagedWidget("glwidget",
                                        glwMDrawingAreaWidgetClass,
                                        w_frame_cmap,
                                        GLwNvisualInfo, vi,
                                        XtNcolormap, cmap,
                                        NULL);
#endif

  XtAddCallback(cmap_widget, GLwNexposeCallback,
                cmap_expose, (XtPointer)NULL);
  XtAddCallback(cmap_widget, GLwNginitCallback,
                cmap_init, (XtPointer)NULL);
  XtAddCallback(cmap_widget, GLwNresizeCallback,
                cmap_resize, (XtPointer)NULL);
  

  /* window-manager delete-button --------------------------------------- */
  shell = cmap_widget;
  XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
  wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW",
                                  False);
  XmAddWMProtocolCallback(shell, wm_delete_window,
                          (XtCallbackProc)schliesse_fenster,
                          (XtPointer)COLORMAP_FENSTER);

  g_tabelle.ex   = TRUE;
  g_tabelle.on   = FALSE;
  fenster->wid  = cmap_widget;
  fenster->vi   = vi;
  fenster->cmap = cmap;
  fenster->dpy  = dpy;

  /* Schriftfont fuer Ausgabe waehlen */
  /*
    font = XLoadQueryFont(C3D_win.dpy,
    "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1");
  */

  /*
    if (font == NULL)
    printf ("kein Font gefunden\n");
    else
    {
    fid = font->fid;
    glXUseXFont(C3D_win.dpy, fid, 32, 96, 2000 + 32);
    }
  */
  return 0;
  /*************************************************************************/
}

/*************************************************************************/
void  cmap_init(Widget w, XtPointer client_data,
                         XtPointer call)
/*************************************************************************/
{
  XVisualInfo *vi;
  Display *dpy;
  GLwDrawingAreaCallbackStruct *call_data;

  call_data = (GLwDrawingAreaCallbackStruct *) call;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** Start cmap_init() ***\n");
#endif
  
  XtVaGetValues(w, GLwNvisualInfo, &vi, NULL);

  dpy = XtDisplay(w);
  glx_context = glXCreateContext(dpy , vi,  C3D_win.cx, True);
  if (glx_context == NULL)
    fprintf(stderr, "Can't create rendering context\n");
  glXMakeCurrent(dpy, XtWindow(w), glx_context);

  gl_width = call_data->width;
  gl_height = call_data->height;
    
  CMAP_win.gl_width  = gl_width;
  CMAP_win.gl_height = gl_height;
  CMAP_win.cx        = glx_context;
  CMAP_win.wid       = w;
  CMAP_win.dpy       = dpy;
  
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
  
}

/*************************************************************************/
void  cmap_expose(Widget w, XtPointer client_data,
                           XtPointer call)
/*************************************************************************/
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** cmap_expose() ***\n");
#endif
  cmap_Redraw();
}

/*************************************************************************/
void  cmap_resize(Widget w, XtPointer client_data,
                           XtPointer call)
/*************************************************************************/
{
  GLwDrawingAreaCallbackStruct *call_data;
  call_data = (GLwDrawingAreaCallbackStruct*) call;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** cmap_resize() ***\n");
#endif
  gl_width = call_data->width;
  gl_height = call_data->height;
  glXWaitX();
  glViewport(0, 0, call_data->width, call_data->height);
  /*cmap_Redraw();*/
}

/*****************************************************************/
void cmap_DrawWindow(int gl_width, int gl_height)
/*****************************************************************/
{
  /* draw the table again */
  zeichne_tabelle();
}

/*****************************************************************/
int cmap_Redraw(gl_width, gl_height)
/*****************************************************************/
{
  GLwDrawingAreaMakeCurrent(CMAP_win.wid, CMAP_win.cx);
  cmap_DrawWindow(gl_width, gl_height);
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);

  return 0;
/*************************************************************************/
}

/*************************************************************************/
void schliesse_fenster(int fenster)
/*************************************************************************/
{
  switch (fenster)
  {
    case COLORMAP_FENSTER:
      {
	g_tabelle.on = FALSE;
	XmToggleButtonSetState(widget_isolinien.TB_tabelle_nicht_zeichnen,
                             FALSE, FALSE);
	break;
      }
    case ABKUEHLKURVEN_FENSTER:
      {
	ABK_win.on = FALSE;
	/* now destroy the GLX context */
	glXDestroyContext(XtDisplay(ABK_win.wid), ABK_win.cx);
	break;
      }
  }

}

/*************************************************************************
 make_fenster_abk: this function creates the window for drawing cooling
 curves. 

 Parameters:
 ==========
            top -  toplevel widget which is parent of the "cooling"
	           window
            fenster - structure of window
 Returns:
 ========
            function returns 0 on success		   
**************************************************************************/
int make_fenster_abk(Widget top, t_winstruct *fenster)

{
  Atom         wm_delete_window;      /* atom  for delating window */
  Widget       shell;                 /* widget for abk. */ 
  Widget       w_frame_abk = NULL;    /* frame widget for OPENGL motif widget*/
  Widget       abk_widget;            /* OPENGL motif widget */
  Widget       w_abkuehl;             /* shell widget */
  XVisualInfo *vi;                    /* visual info for window */
  Display     *dpy;                   /* display for window */
  XFontStruct *font;                  /* X-font */
  Colormap     cmap;
 

  /* Verbindung zum X-Server oeffnen: */
  dpy = XtDisplay(top);

  if (!(w_abkuehl = CreateWin2d(top)))
    {
      fprintf(stderr, "Can't create w_abkueh widget!\n");
      return(1);
    }
  /* put in global structure */
  widgetMain.S_abkuehl = w_abkuehl; 

  /* get the frame from shell widget */
  w_frame_abk = XtNameToWidget(w_abkuehl,"*Frame_abk");

  /* suche Visual und creiere Context ----------------------------------- */
  vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblbuf_12);
  if (vi == NULL)
    {
      printf("No 12-bit Double buffer visual available\n");
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), sglbuf_12);
    }

  if (vi == NULL)
    {
      printf("No 12-bit Single buffer visual available\n");
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), sglbuf_8);
      warning("No 12-bit Single buffer visual available.");
    }

  if(vi == NULL)
    {
      printf("No 12-bit Single buffer visual available.\n");
      warning("No 12-bit Single buffer visual available.");
      return(1);
    }
  
  /* set the colormap */
  SetColorMap2000();

  /* benutze Visual und Colormap des Hauptfensters fuer neues Fenster */
  vi   = C3D_win.vi;
  cmap = C3D_win.cmap;
  
  /* oeffne OpenGl-Grafikfenster im entsprechenden Modus */
  abk_widget = XtVaCreateManagedWidget("glwidget",
				       glwMDrawingAreaWidgetClass,
				       w_frame_abk,
				       GLwNvisualInfo, vi,
				       XtNcolormap, cmap,
				       XmNwidth, 640,
				       XmNheight, 540,
				       NULL,0);

  /*>>>>> Callbacks  for OPENGL widget<<<<<<*/
  XtAddCallback(abk_widget, GLwNginitCallback,
                abk_init, (XtPointer)NULL);
  XtAddCallback(abk_widget, GLwNexposeCallback,
                abk_expose, (XtPointer)NULL);
  XtAddCallback(abk_widget, GLwNresizeCallback,
                abk_resize, (XtPointer)NULL);
  XtAddCallback(abk_widget, XtNdestroyCallback,
		abk_close, (XtPointer)NULL);
 
  /* window-manager delete-button --------------------------------------- */
  shell = abk_widget;
  XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
  wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW",
                                  False);
  XmAddWMProtocolCallback(shell, wm_delete_window,
                          (XtCallbackProc)schliesse_fenster,
                          (XtPointer)ABKUEHLKURVEN_FENSTER);

  fenster->ex   = TRUE;
  fenster->on   = FALSE;
  fenster->wid  = abk_widget;
  fenster->vi   = vi;
  fenster->cmap = cmap;
  fenster->dpy  = dpy;

  /* Schriftfont fuer Ausgabe waehlen */
  font = XLoadQueryFont(C3D_win.dpy,
			"-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1");
  return (0);
}


/*************************************************************************/
void  abk_init(Widget w, XtPointer client_data,
                         XtPointer call)
/*************************************************************************/
{
  XVisualInfo *vi;
  Display *dpy;
  GLwDrawingAreaCallbackStruct *call_data;

  call_data = (GLwDrawingAreaCallbackStruct *)call;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** Start abk_init() ***\n");
#endif

  dpy = XtDisplay(w);
  XtVaGetValues(w, GLwNvisualInfo, &vi,NULL);
  
  ABK_win.cx = glXCreateContext(XtDisplay(w), vi, 0, False);
  if (ABK_win.cx == NULL)
    fprintf(stderr, "Can't create rendering context\n"); 
  
  /* make current the widget for abkuehlkurven */
  glXMakeCurrent(dpy, XtWindow(w), ABK_win.cx);

  ABK_win.gl_width  = call_data->width;
  ABK_win.gl_height = call_data->height;
  ABK_win.vi        = vi;
  ABK_win.wid       = w;
  ABK_win.dpy       = dpy;

  /* make the current default window */
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
}

/*************************************************************************/
void abk_expose(Widget w, XtPointer client_data,
                           XtPointer call)
/*************************************************************************/
{
  glXMakeCurrent(XtDisplay(w), XtWindow(w), ABK_win.cx);
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** abk_expose() ***\n");
#endif
  abk_Redraw();
}

/*************************************************************************/
void  abk_resize(Widget w, XtPointer client_data,
                           XtPointer call)
/*************************************************************************/
{
  GLwDrawingAreaCallbackStruct *call_data;
  call_data = (GLwDrawingAreaCallbackStruct*) call;

  glXMakeCurrent(XtDisplay(w), XtWindow(w), ABK_win.cx);
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** abk_resize() ***\n");
#endif

  /* get the new size */
  ABK_win.gl_width = call_data->width;
  ABK_win.gl_height = call_data->height;
  glXWaitX();
  /*abk_Redraw();*/
  glViewport(0, 0, call_data->width, call_data->height);
}


/*****************************************************************/
void abk_DrawWindow(int gl_width, int gl_height)
/*****************************************************************/
{
  /*show the cooling curves */
  zeichne_abkuehlkurven();
}

/*****************************************************************/
void abk_Redraw(void)
/*****************************************************************/
{
  GLwDrawingAreaMakeCurrent(ABK_win.wid, ABK_win.cx);
  abk_DrawWindow(gl_width, gl_height);
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);

/*****************************************************************/
}
/******************************************************************
 This function cleans the window and destroys the GLX
 drawing context.
******************************************************************/
void abk_close(Widget w, XtPointer clientData, 
	       XtPointer call)
{
  /* destroy widget */
  /* destroy context and set new flags*/
  glXDestroyContext(XtDisplay(w), ABK_win.cx);
  ABK_win.on = False;
  ABK_win.ex = False;
}

/******************************************************************
 This function is taken from book "OpenGL Programming for 
 X Window System" Mark J. Kilgard
******************************************************************/  
Colormap getShareableColormap(Display *dpy, XVisualInfo *vi)
{
  Status status;
  XStandardColormap *standardCmaps;
  Colormap cmap;
  int i, numCmaps;
  
  /* Be lazy; using DirectColor too involved for this example */
  if (vi->class != TrueColor)
    fprintf(stderr,"No support for non-TrueColor visual.");

  status = XmuLookupStandardColormap(dpy, vi->screen, vi->visualid,
				     vi->depth, XA_RGB_DEFAULT_MAP,
				     /*replace*/ False, /*retain*/ True);
  if (status == 1)
    {
      status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
			      &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);
      if (status == 1)
	for(i = 0; i < numCmaps; i++)
	  if(standardCmaps[i].visualid == vi->visualid)
	    {
	      cmap = standardCmaps[i].colormap;
	      XFree(standardCmaps);
	      return cmap;
	    }
    }

  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
			 vi->visual, AllocNone);
  return cmap;
}
  












