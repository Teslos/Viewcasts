/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*  create_da   |                                                */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Xm/DrawingA.h>

#include <X11/keysym.h>

#include "color3d.h"
#include "motif.h"
#include "grafik_func.h"
#include "grafik.h"
#include "sovLayerUtil.h"
#include "gl_local.h"
#include "memory.h"

/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
static int dbl_directRGBA[] = {GLX_RGBA,
			       GLX_X_VISUAL_TYPE_EXT, GLX_DIRECT_COLOR_EXT,
			       None};
                            
static int dbl_bufRGBA[]={GLX_RGBA,
			  GLX_RED_SIZE, 1,
			  GLX_GREEN_SIZE, 1,
			  GLX_BLUE_SIZE, 1,
			  GLX_DEPTH_SIZE, 12,
			  GLX_STENCIL_SIZE, 8,
			  GLX_DOUBLEBUFFER,
			  NULL};
static int dbl_bufSampleRGBA[]={GLX_RGBA, GLX_SAMPLES_SGIS, 4,
              GLX_RED_SIZE, 1,
              GLX_GREEN_SIZE, 1,
              GLX_BLUE_SIZE, 1,
              GLX_DEPTH_SIZE, 12,
              GLX_STENCIL_SIZE, 8,
              GLX_DOUBLEBUFFER,
              NULL};
static int dblbuf_12[] =
{GLX_BUFFER_SIZE, 12, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 12,
 GLX_STENCIL_SIZE, 4, NULL};
static int dblbuf_8[]  =
{GLX_BUFFER_SIZE,  8, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 12, NULL};
static int sglbuf_12[] = {GLX_BUFFER_SIZE,12, GLX_DEPTH_SIZE, 12, NULL};
static int sglbuf_8[]  = {GLX_BUFFER_SIZE, 8, GLX_DEPTH_SIZE, 12, NULL};
static int cmap_12[]   = {GLX_BUFFER_SIZE,12, NULL};
static int cmap_8[]    = {GLX_BUFFER_SIZE, 8, NULL};

static Colormap overlayColormap;          /* colormap for overlay */
static int      overlayDepth;             /* depth of the overlay */
static Visual  *overlayVisual;            /* Visual informations  */

/* Routinen :                                                    */
/* ============================================================= */
static Colormap detectOverlaySupport(Display *dpy);

/* ************************************************************* */
int create_da(Widget top, t_bool *dpy_has_gl)
/* ************************************************************* */
{
  Widget w_frame_da = NULL;
  int glErrorBase, glEventBase;
  int glMajorVersion, glMinorVersion;
  int anz_rbit, anz_gbit, anz_bbit, anz_indexbit;
  GLboolean doublebuffer_on;
  int directcol;                              /* direct color suport */
  int multisample;                            /* multisample support */
  /*  HV Aenderungen fuer Colormap --------------------------------- */
  Bool needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  int dummy;
  unsigned long colors[MAX_COLORS];
  unsigned long plane_masks[1];
  int i,j;
  XVisualInfo *visualList;
  Colormap sovcmap; 
  int stensize;
  t_overlay *overlay_info;
  sovVisualInfo lv_info;

  /* Verbindung zum X-Server oeffnen: */
  dpy = XtDisplay(top);

  *dpy_has_gl = FALSE;

  if (!(w_frame_da = XtNameToWidget(top, "*Frame_da")))
    {
      fprintf(stderr, "XtNameToWidget(..., \"*Frame_da\") returns NULL\n");
      return(1);
    }

  /* OpenGL auf diesem Display verfuegbar ? */
  if ((*dpy_has_gl = glXQueryExtension(dpy, &glErrorBase, &glEventBase)))
    {
      if (glXQueryVersion(dpy, &glMajorVersion, &glMinorVersion))
	{
#ifdef DEBUG
	  if (applRessourcen.debug)
	    fprintf(stderr, "OpenGL-Version = >%d.%d<\n", glMajorVersion,
		    glMinorVersion);
#endif
	}

      /* ask for extension for direct color suport*/
      directcol = glxExtensionSupported(dpy, DefaultScreen(dpy), 
				       "GLX_EXT_visual_info");
      multisample = glxExtensionSupported(dpy, DefaultScreen(dpy),
					  "GLX_EXT_SGIS_multisample");
      
      /*  HV Aenderungen fuer Colormap ------------------------------- */
      C3D_win.achtbit = FALSE;
      C3D_win.doublebuffer = TRUE;

      if (directcol)
	{
	  visualList = glXChooseVisual(dpy, DefaultScreen(dpy), dbl_directRGBA);
	  if (visualList == NULL)
	    printf(
		   "Color3d: Can't allocate Direct Colormap\nTrying to allocate True Colormap");
	}

      if (multisample)
	{
	  visualList = glXChooseVisual(dpy, DefaultScreen(dpy), dbl_bufSampleRGBA);
	  if (visualList == NULL)
	    printf("Color3d: Can't find Multisample visuals\n");
	}
      
      /* Gesucht: Visual mit 12 Bit Colorindextiefe und Doublebuffer - */
      visualList = glXChooseVisual(dpy, DefaultScreen(dpy), dbl_bufRGBA);
      if ( visualList != NULL )
      {	
	C3D_win.display_mode = C3D_DOUBLE | C3D_RGBA | C3D_STENCIL;
      }

      if (visualList == NULL)     /* weiterer Versuch mit Singlebuffer */
	{
	  printf("Es wurde kein 12 Bit Doublebuffer-Visual gefunden\n");
	  /* Gesucht: Visual mit 12 Bit Colorindextiefe und Singlebuffer - */
	  visualList = glXChooseVisual(dpy, DefaultScreen(dpy), sglbuf_12);
	  C3D_win.doublebuffer = FALSE;
	}

      if (visualList == NULL)     /* weiterer Versuch mit weniger Farbtiefe */
	{
	  printf("Es wurde kein 12 Bit Singlebuffer-Visual gefunden\n");
	  /* Gesucht: Visual mit 8 Bit Colorindextiefe und Doublebuffer -- */
	  visualList = glXChooseVisual(dpy, DefaultScreen(dpy), dblbuf_8);
	  C3D_win.achtbit = TRUE;
	}

      if (visualList == NULL)     /* weiterer Versuch mit weniger Farbtiefe */
	{
	  printf("Es wurde kein 8 Bit Doublebuffer-Visual gefunden\n");
	  /* Gesucht: Visual mit 8 Bit Colorindextiefe und Singlebuffer -- */    
	  visualList = glXChooseVisual(dpy, DefaultScreen(dpy), sglbuf_8);
	  C3D_win.achtbit = TRUE;
	  C3D_win.doublebuffer = FALSE;
	}

      if (visualList == NULL)
	{
	  printf("Color3d cannot run on this terminal\n");
	  warning("Color3d cannot run on this terminal");
	  return(1);
	}
	
      /* get the stencil size */
      glXGetConfig(dpy, visualList, GLX_STENCIL_SIZE, &stensize);
      /* get the direct color */
      glXGetConfig(dpy, visualList, GLX_DIRECT_COLOR_EXT, &directcol);
      if (directcol)
	     printf("\nWe have direct color support\n");
      
      C3D_win.vi       = visualList; 
      C3D_win.dpy      = dpy;
      C3D_win.stensize = stensize;

      printf("\nVisual ID: %d, Visual-Depth: %d Doublebuffer: %d, Achtbit: %d, Stencil-Size: %d\n\n",
	     C3D_win.vi->visualid,C3D_win.vi->depth,C3D_win.doublebuffer,
 	     C3D_win.achtbit, C3D_win.stensize);


      /* X-Window kreieren: */
      /*
      cmap = XCreateColormap(dpy,RootWindow(dpy,C3D_win.vi->screen),
      C3D_win.vi->visual, AllocNone);*/
      cmap = getShareableColormap(dpy, C3D_win.vi);
      /*
      if (cmap == NULL) printf("\nColormap nicht allokierbar!\n");
      if (!C3D_win.achtbit)
	{ 
	  if (XAllocColorCells(dpy, cmap, GL_FALSE, plane_masks, 0, colors,
			       MAX_COLORS) == 0)
	    printf("\nFehler bei 24-Bit Allokierung!\n");
	  g_tabelle.startindex = STOFFARB_OFFSET;
	}
      else
	{
	  if (XAllocColorCells(dpy, cmap, GL_FALSE, plane_masks, 0, colors,
			       ACHT_BIT_MAX_COLORS) == 0)
	    printf("\nFehler bei 8-Bit Allokierung!\n");
	  g_tabelle.startindex = ACHT_BIT_OFFSET;
	}
      */

      /* Farben setzen: */
      if (C3D_win.achtbit)
	SetColorMap256();
      else
	/*true_cmp();*/
	SetColorMap2000();

#ifndef HAVE_LINUX
      /* oeffne OpenGl-Grafikfenster im Doublebuffer- und Colormap-Modus  */
      gl_widget = XtVaCreateManagedWidget("glwidget",
					  glwMDrawingAreaWidgetClass,
					  w_frame_da,
					  XmNtopAttachment, XmATTACH_FORM,
					  XmNleftAttachment, XmATTACH_FORM,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNbottomAttachment, XmATTACH_FORM,
					  GLwNvisualInfo, C3D_win.vi,
					  XtNcolormap, cmap,
					  NULL);
#else
      gl_widget = XtVaCreateManagedWidget("glwidget",
                                          glwMDrawingAreaWidgetClass,
                                          w_frame_da,
                                          XmNtopAttachment, XmATTACH_FORM,
					  XmNleftAttachment, XmATTACH_FORM,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNbottomAttachment, XmATTACH_FORM,
					  GLwNvisualInfo, C3D_win.vi,
                                          XtNcolormap, cmap,
                                          NULL);
#endif
    

      /* Try to find a good overlay visual for pull-down and pop-up menus.*/
      /* store colormap */
      /*XXX detect overlay sovcmap = detectOverlaySupport(dpy); */
     /* create popup menu */
      //popMenuCreate(w_frame_da);

      /*
	C3D_win.cx = glXCreateContext(dpy, C3D_win.vi, 0, GL_FALSE);
      */

      XtAddCallback(gl_widget, GLwNinputCallback, (XtCallbackProc)c3d_input,
		    (XtPointer)NULL);
      XtAddCallback(gl_widget, GLwNexposeCallback,(XtCallbackProc)c3d_expose,
		    (XtPointer)NULL);
      XtAddCallback(gl_widget, GLwNresizeCallback,(XtCallbackProc)c3d_resize,
		    (XtPointer)NULL);
      XtAddCallback(gl_widget, GLwNginitCallback, (XtCallbackProc)c3d_init,
		    (XtPointer)NULL);

      C3D_win.wid   = gl_widget;
      C3D_win.win   = XtWindow( gl_widget );
      C3D_win.cmap  = cmap;
      C3D_win.frame = w_frame_da;
      C3D_win.overlayColormap = sovcmap;
      C3D_win.overlayVisual   = overlayVisual;
      C3D_win.overlayDepth    = overlayDepth;
    }
  else 
    return(1);
  return(0);
  /*************************************************************************/
}

 
/*****************************************************************
 CreateGLXContext this function try to conect the context
 to hardware rendering (for local machine) or software 
 rendering.

 Parameters:
 ===========
             context - GLX rendering context
	     w       - widget get context
 Returns:
 ========
             function returns 0 on success and 1 on failure
*****************************************************************/

GLXContext CreateGLXContext(GLXContext context, Display* dpy, XVisualInfo* vi)
{
  /*destroy existing context*/
  if(context != NULL)
    glXDestroyContext(dpy, context);
  /*first try direct context*/
  context = glXCreateContext(dpy, vi, NULL, True);
  if(context == NULL)
    {
      /*try indirect context*/
      context = glXCreateContext(dpy, vi, NULL, False);
      if(context == NULL)
	{
	  printf("Error: glXCreateContext failed!\n");
	  exit(0);
	}
      else
	{
	  printf("Warning: Using indirect GLX context, may be slow!\n");
	}
    }
  return (context);
}

/*****************************************************************
 This function try find if extension is supported by GLX.

 Parameters:
 ===========
           Display *dpy - handle to a window.
	   int screen - screen in which GLX draws.
	   char *extension - name of extension.
 Returns:
 ========
           return 1 if extension exist
	   0 otherwise
******************************************************************/
int glxExtensionSupported( Display *dpy, int screen, char *extension)
{
  int major, minor;
  const char *extensions, *start;
  char *where, *terminator;

  if (glXQueryVersion(dpy, &major, &minor)) 
    {
      if (minor >= 1 || major > 1) 
	{
	  extensions = glXQueryExtensionsString(dpy, screen);
	  start = extensions;
	
	  while(1) 
	    {
	      where = strstr(start, extension);
	      if (!where)
		return 0;
	      terminator = where + strlen(extension);
	      if (where == start || *(where - 1) == ' ')
		if (*terminator == ' ' || *terminator == '0')
		  return 1;
	      start = terminator;
	    }
	}
    }
  return 0;
}
	
/*****************************************************************
 This function try to create overlay support for this program.

 Parameters:
 ===========
           Display *dpy - handle to a window
 Returns:
 ========
           none
******************************************************************/

static Colormap detectOverlaySupport(Display *dpy)
{
 
  sovVisualInfo template;
  sovVisualInfo *overlayVisuals;
  int layer;
  int nVisuals, i, entries;

  overlayVisual = NULL;
  /* Need more than two colormap entries for 
   * reasonable menus
   */
  entries = 2;
  for(layer = 1; layer <= 3; layer++)
    {
      template.layer = layer;
      template.vinfo.screen = DefaultScreen(dpy);
      overlayVisuals = sovGetVisualInfo(dpy,
					VisualScreenMask | VisualLayerMask, 
					&template, &nVisuals);
      if(overlayVisuals)
	{
	  for(i =0; i < nVisuals; i++)
	    {
	      if(overlayVisuals[i].vinfo.visual->map_entries > entries)
		{
		  overlayVisual = overlayVisuals[i].vinfo.visual;
		  overlayDepth = overlayVisuals[i].vinfo.depth;
		  entries = overlayVisual->map_entries;
		}
	    }
	  XFree(overlayVisuals);
	}
    }

  if(overlayVisual)
    {
      overlayColormap = XCreateColormap(dpy, DefaultRootWindow(dpy),
					overlayVisual, AllocNone);
      return overlayColormap;
    }
  else
    return (Colormap) 0;
}












