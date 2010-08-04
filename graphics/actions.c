/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   actions.c                                        */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*  popup_menue_ausgeben   |  */
/*  traverse_current       |  */
/*  focus_out_TF_in_TT     |  */
/*  start_move             |  */
/*  end_move               |  */
/*  start_rot              |  */
/*  end_rot                |  */
/*  start_zoom             |  */
/*  end_zoom               |  */
/*  start_pick             |  */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <Xm/Xm.h>

#include <Xm/RowColumn.h>
#include <Xm/TextF.h>

#include "grafik_func.h"
#include "grafik.h"
#include "color3d.h"
#include "trackball.h"
#include "motif.h"

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
static int momentum = 1;
static int spinning;
float lastquat[4];
float curquat[4];
XtWorkProcId animateID;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
static void animate(XtPointer closure, XtIntervalId *id);
void stopSpinning(void);
void startRotation(Widget w, XEvent *event, String *params,
		   Cardinal *num_params);
/* ******************************************************************** */
void popup_menue_ausgeben(Widget widget, XEvent* eventAdr, String* params,
                          Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
  Widget w = XtNameToWidget(XtParent(widget), params[0]);
  static Widget static_widget;

  static_widget = widget;

  XtVaSetValues(w, XmNuserData, static_widget, NULL);

/* nur durch Button-Event auszuloesen --------------------------------- */
  if (eventAdr->type == ButtonPress || eventAdr->type == ButtonRelease)
  {
    XmMenuPosition(w, &eventAdr->xbutton);
  }
  else
    return;

  XtManageChild(w);

  return;
}
/* ******************************************************************** */
void traverse_current(Widget widget, XEvent* eventAdr, String* params,
                      Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
  XmProcessTraversal(widget, XmTRAVERSE_CURRENT);
  return;
}
/* ******************************************************************** */
/* wenn das TextField den Focus verliert wird die Aktion "EndEdit" ---- */
/* des Eltern-Widgets (TextTable-Widget) ausgefuehrt ------------------ */
/* ******************************************************************** */
void focus_out_TF_in_TT(Widget widget, XEvent* eventAdr, String* params,
                        Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
/*
  if (BXmTextTableGetChild(XtParent(widget), BXmCHILD_INPUT) == widget)
  {
    XtCallActionProc(XtParent(widget), "EndEdit", (XEvent *)NULL,
                     (String *)NULL, 0);
  }
*/
  return;
}
/* ******************************************************************** */

/* ******************************************************************** */
void start_move(Widget widget, XEvent* eventAdr, String* params,
                Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("start_move XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = TRANSLATE\n");
  }
#endif

  if (mode != TRANSLATE)
  {
    mode = TRANSLATE;
    set_cursor_handler(widget, TRANSLATE_CURSOR);
  }

  switch(eventAdr->type)
  {
    case KeyPress:
    {
      omx = mx = eventAdr->xkey.x;
      omy = my = eventAdr->xkey.y;
      g_transform.rahmen_on = TRUE;
      break;
    }
    case ButtonPress:
    {
      omx = mx = eventAdr->xbutton.x;
      omy = my = eventAdr->xbutton.y;
      break;
    }
  }

  return;
/* ******************************************************************** */
}

/* ******************************************************************** */
void end_move(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{ 
  int dx, dy, dz;
  Dimension xsize, ysize;
  char wert[12];
  GLint view[4];

/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 

  XtVaGetValues(C3D_win.wid, XmNwidth, &xsize, 
		XmNheight, &ysize, NULL);
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("end_move XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    printf(stderr, "mode = NOTHING\n");
  }
#endif
  mode = NOTHING;

  switch(eventAdr->type)
  {
    case KeyRelease:
    {
#ifdef DEBUG
      printf("move: KeyRelease\n");
#endif
      if (eventAdr->xkey.x >= 0) dx = (int)eventAdr->xkey.x - (int)omx;
      if (eventAdr->xkey.y >= 0) dy = (int)eventAdr->xkey.y - (int)omy;
      dy = -dy;
      dz = 0.0;
      /*trans_vektor(&dx, &dy, &dz);*/
/*
      g_transform.move[0] = g_transform.move[0] +
                            (float)(dx)*g_projektion.geo_factor;
      g_transform.move[1] = g_transform.move[1] +
                            (float)(dy)*g_projektion.geo_factor;
      g_transform.move[2] = g_transform.move[2] +
                            (float)(dz)*g_projektion.geo_factor;
*/
      g_transform.move[0] = g_transform.move[0] +
                            (float)(dx)/g_transform.scale * 0.1;
      g_transform.move[1] = g_transform.move[1] +
                            (float)(dy)/g_transform.scale * 0.1;
      g_transform.move[2] = g_transform.move[2] +
                            (float)(dz)/g_transform.scale * 0.1;
      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case ButtonRelease:
    {
      double objX, objY, objZ, oobjX, oobjY, oobjZ;
      GLdouble modelm[16], projectm[16];
#ifdef DEBUG
      printf("move: ButtonRelease\n");
#endif
      if (eventAdr->xbutton.x >= 0)
                 dx = (int)eventAdr->xbutton.x - (int)omx;
      if (eventAdr->xbutton.y >= 0)
                 dy = (int)eventAdr->xbutton.y - (int)omy;
      dy = -dy;
      dz = 0.0;
      /*trans_vektor(&dx, &dy, &dz);*/
/*
      g_transform.move[0] = g_transform.move[0] +
                            (float)(dx)*g_projektion.geo_factor;
      g_transform.move[1] = g_transform.move[1] +
                            (float)(dy)*g_projektion.geo_factor;
      g_transform.move[2] = g_transform.move[2] +
                            (float)(dz)*g_projektion.geo_factor;
*/
      g_transform.move[0] += (float)(dx)/g_transform.scale * 0.1;
      g_transform.move[1] += (float)(dy)/g_transform.scale * 0.1;
      g_transform.move[2] += (float)(dz)/g_transform.scale * 0.1;

      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case MotionNotify:
    {
      t_point p, oldp;
      int odx, ody;
#ifdef DEBUG
      printf("move: MotionNotify\n");
#endif
      if (eventAdr->xbutton.x >= 0)
	{
	  dx  = (int)eventAdr->xbutton.x - xsize/2.0;
	  odx = omx - xsize/2.0;
	}
      if (eventAdr->xbutton.y >= 0)
	{
	  dy = (int)eventAdr->xbutton.y - ysize/2.0;
	  ody = omy - ysize/2.0;
	}
      dy = -dy;
      dz = 0.0;
      /*trans_vektor(&dx, &dy, &dz);*/
/*
      g_transform.move[0] = g_transform.move[0] +
                            (float)(dx)*g_projektion.geo_factor;
      g_transform.move[1] = g_transform.move[1] +
                            (float)(dy)*g_projektion.geo_factor;
      g_transform.move[2] = g_transform.move[2] +
                            (float)(dz)*g_projektion.geo_factor;
*/
      /*
      g_transform.move[0] = g_transform.move[0] +
                            (float)(dx)/g_transform.scale * 0.1;
      g_transform.move[1] = g_transform.move[1] +
                            (float)(dy)/g_transform.scale * 0.1;
      g_transform.move[2] = g_transform.move[2] +
                            (float)(dz)/g_transform.scale * 0.1;*/
 
      C3D_getworldpoint( dx, dy, p.v );
      C3D_getworldpoint( odx, ody, oldp.v );
      g_transform.move[0] += p.v[0] - oldp.v[0];
      g_transform.move[1] += p.v[1] - oldp.v[1];
      g_transform.move[2] += p.v[2] - oldp.v[2];
      break;
    }
  }
  omx = mx;
  omy = my;

  return;
/* ******************************************************************** */
}

/* ******************************************************************** */
void start_rot(Widget widget, XEvent* eventAdr, String* params,
               Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("start_rot XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = ROTATE\n");
  }
#endif

  if (mode != ROTATE)
  {
    mode = ROTATE;
    set_cursor_handler(widget, ROTATE_CURSOR);
  }

  switch(eventAdr->type)
  {
    case KeyPress:
    {
#ifdef DEBUG
      printf("rot: KeyPress\n");
#endif
      omx = eventAdr->xkey.x;
      omy = eventAdr->xkey.y;
      g_transform.rahmen_on = TRUE;
      break;
    }
    case ButtonPress:
    {

#ifdef DEBUG
      printf("rot: ButtonPress\n");
#endif
      omx = eventAdr->xbutton.x;
      omy = eventAdr->xbutton.y;
      break;
    }
  }
  return;
/* ******************************************************************** */
}

/* ******************************************************************** */
void end_rot(Widget widget, XEvent* eventAdr, String* params,
             Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
  int dx, dy;
  char wert[12];
/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("end_rot XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = NOTHING\n");
  }
#endif
  mode = NOTHING;

  switch(eventAdr->type)
  {
    case KeyRelease:
    {
#ifdef DEBUG
      printf("rot: KeyRelease\n");
#endif
      if (eventAdr->xkey.x >= 0) dx = eventAdr->xkey.x - omx;
      if (eventAdr->xkey.y >= 0) dy = eventAdr->xkey.y - omy;
      g_transform.rotx = g_transform.rotx + (float)(dy)*0.1;
      g_transform.rotz = g_transform.rotz + (float)(dx)*0.1;
      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case ButtonRelease:
    {
#ifdef DEBUG
      printf("rot: ButtonRelease\n");
#endif
      if (eventAdr->xbutton.x >= 0) dx = eventAdr->xbutton.x - omx;
      if (eventAdr->xbutton.y >= 0) dy = eventAdr->xbutton.y - omy;
      g_transform.rotx = g_transform.rotx + (float)(dy)*0.1;
      g_transform.rotz = g_transform.rotz + (float)(dx)*0.1;
      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case MotionNotify:
    {
#ifdef DEBUG
      printf("rot: MotionNotify\n");
#endif
      if (eventAdr->xbutton.x >= 0)
	dx = (int)eventAdr->xbutton.x - (int)omx;
      if (eventAdr->xbutton.y >= 0)
	dy = (int)eventAdr->xbutton.y - (int)omy;
      g_transform.rotx = g_transform.rotx + (float)(dy)*0.1;
      g_transform.rotz = g_transform.rotz + (float)(dx)*0.1;
      break;
    }
    default:
      break;
  }

  assert(XmIsTextField(widgetMain.RX_rot_x) || XmIsTextField(widgetMain.RX_rot_y) ||
	 XmIsTextField(widgetMain.RX_rot_z));

  
/* Gebe Rotationswerte im Hauptfenster aus ---------------------------- */
  sprintf(wert, "%i", (int) g_transform.rotx);
  XmTextFieldSetString(widgetMain.RX_rot_x, wert);
  sprintf(wert, "%i", (int) g_transform.roty);
  XmTextFieldSetString(widgetMain.RX_rot_y, wert);
  sprintf(wert, "%i", (int) g_transform.rotz);
  XmTextFieldSetString(widgetMain.RX_rot_z, wert);

}

/* ******************************************************************** */

/* ******************************************************************** */
void start_zoom (Widget widget, XEvent* eventAdr, String* params, Cardinal* paramAnzAdr)
/* ******************************************************************** */
{
/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("start_zoom XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = ZOOM\n");
  }
#endif

  if (mode != ZOOM)
  {
    mode = ZOOM;
    set_cursor_handler(widget, ZOOM_CURSOR);
  }

  switch(eventAdr->type)
  {
    case KeyPress:
    {
      omy = my = eventAdr->xkey.y;
      g_transform.rahmen_on = TRUE;
      break;
    }
    case ButtonPress:
    {
      omy = my = eventAdr->xbutton.y;
      break;
    }
  }

  return;
/* ******************************************************************** */
}

/* ******************************************************************** */
void end_zoom(Widget widget, XEvent* eventAdr, String* params,
              Cardinal* paramsAnzAdr)
/* *********************************************************************/
{
  int dx, dy;

/* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
/* 
  if (widget->core.window != eventAdr->xany.window) return;
*/ 
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("end_zoom XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = NOTHING\n");
  }
#endif

  mode = NOTHING;

  switch(eventAdr->type)
  {
    case KeyRelease:
    {
      if (eventAdr->xkey.y >= 0) dy = omy - eventAdr->xkey.y;
      g_transform.scale = g_transform.scale * (1.0 + (float)(dy)*0.01);
      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case ButtonRelease:
    {
      if (eventAdr->xbutton.y >= 0) dy = omy - eventAdr->xbutton.y;
      g_transform.scale = g_transform.scale * (1.0 + (float)(dy)*0.01);
      reset_cursor_handler(widget);
      g_transform.rahmen_on = FALSE;
      break;
    }
    case MotionNotify:
    {
      if (eventAdr->xbutton.y >= 0) dy = omy - eventAdr->xbutton.y;
      g_transform.scale = g_transform.scale * (1.0 + (float)(dy)*0.01);
      break;
    }
  }
  return;
/* ******************************************************************** */
}

/* ******************************************************************** */
void start_pick(Widget widget, XEvent* eventAdr, String* params,
                Cardinal* paramsAnzAdr)
/* ******************************************************************** */
{
  t_pickpunkt knoten;
  /* Nur ausfuehren falls XEvent aus Grafikfenster ---------------------- */
#ifdef DEBUG
  if (applRessourcen.debug)
  {
    printf("start_pick XtName(widget) = >%s<\n", XtName(widget));
    printf("eventtype = >%d<\n", eventAdr->type);
    fprintf(stderr, "mode = PICK\n");
  }
#endif
  /* choose the pick mode for mouse pointer */
  if (mode != PICK)
  {
    mode = PICK;
    set_cursor_handler(widget, PICK_CURSOR);
  }

  /* see which event happened */
  switch(eventAdr->type)
  {
    case ButtonPress:
    {
      omx = mx = eventAdr->xbutton.x;
      omy = my = eventAdr->xbutton.y;

      /* return numbers of points hits */
      knoten.punkt_nr = picke_knoten(omx, omy, 5, 5, 200);
      /*knoten.punkt_nr = pick_nodes(omx, omy, 5, 5, 200);*/
      if (0 < knoten.punkt_nr) 
      {
        if ( knoten.punkt_nr <= g_geodat.npunkt) 
        {
          knoten.koord[0] = g_knoten[knoten.punkt_nr - 1].koord[0]; 
          knoten.koord[1] = g_knoten[knoten.punkt_nr - 1].koord[1]; 
          knoten.koord[2] = g_knoten[knoten.punkt_nr - 1].koord[2]; 
        }
        else
        {
          knoten.koord[0] =
             g_schnittpunktliste[knoten.punkt_nr - g_geodat.npunkt - 1].x;
          knoten.koord[1] =
             g_schnittpunktliste[knoten.punkt_nr - g_geodat.npunkt - 1].y;
          knoten.koord[2] =
             g_schnittpunktliste[knoten.punkt_nr - g_geodat.npunkt - 1].z;
        }
          knoten.next = NULL;
	  
	  /* add punkt to list of points */
          if (add_pickpunkt(knoten)) 
            warning("no memory for node list");
          else
	    {
	      fill_tt_picking();  /*fill the text-table */
	    }
      }
      break;
    }
  }
  
/* ******************************************************************** */
}

/************************************************************************
 spin_rot - This routine spins the model object by taking first
 the position of the mouse when is first press and then adding
 the angle to rotation by calculation of movement of the mouse.
*************************************************************************/

void spin_rot( Widget w, XEvent *event, String *params, 
		Cardinal *paramsAnzAdr)
{
  /* local variables */
  int anglex=0;    /* rotation in x direction */
  int angley=0;    /* rotation in y direction */
  int anglez=0;    /* rotation in z direction */
  
  static lastX=0;       /* position of button press */
  static lastY=0;       /* position of button press */
  static GLboolean frecalModelView = GL_FALSE;

  /* process all events */
  switch(event->type)
    {
    case MotionNotify: /* motion of mouse */
      {
	/* calculate the motion of the mouse */
	anglex = (event->xmotion.x-lastX);
	angley = (event->xmotion.y-lastY);
#ifdef DEBUG
	printf("xmotion.y %d\n", event->xmotion.y);
	printf("angle y %d\n",angley);
#endif
	frecalModelView = GL_TRUE;
	g_transform.gitter_on = TRUE;
	g_transform.rotation_on = TRUE;
	break;
      } 
    case ButtonPress:  /* button press */
      {
	/* remeber the position of the mouse */
	lastX = event->xbutton.x;
	lastY = event->xbutton.y;
#ifdef DEBUG
	printf("omx %d\n", omx);
	printf("omy %d\n", omy);
#endif
	break;
      }
    case ButtonRelease: 
      {
	frecalModelView = GL_FALSE;
	g_transform.gitter_on = FALSE;
	g_transform.rotation_on = FALSE;
        break;
      }
    }
  if(frecalModelView == GL_TRUE)
    {
      g_transform.rotz += anglex;
      g_transform.rotx += angley;
#ifdef DEBUG
      printf("angle x %d\n", anglex);
      printf("angle y %d\n", angley);
#endif
      lastX = event->xbutton.x;
      lastY = event->xbutton.y;
      frecalModelView = GL_FALSE; 
    }
       
}

/*****************************************************************
 move - moves model by mouse.

 Parameters:
 ===========
           w - widget 
           event - mouse event
           params - not used
           paramsAnzAdr - not used
*****************************************************************/
void move(Widget w, XEvent *event, String *params, 
		Cardinal *paramsAnzAdr)
{
  Dimension xsize, ysize;
  int dx=0, dy=0, odx, ody;
  t_point p, oldp;
  float nmove[4];

  XtVaGetValues(w, XmNwidth, &xsize, 
		XmNheight, &ysize, NULL);

  if (event->xbutton.x >= 0) 
    {
      dx = (int)event->xbutton.x - xsize/2.0;
      odx = (int)omx - xsize/2.0;
    }
  if (event->xbutton.y >= 0)
    {
      dy = (int)event->xbutton.y - ysize/2.0;
      ody = omy - ysize/2.0;
    }
  
  C3D_getworldpoint( dx, dy, p.v );
  C3D_getworldpoint( odx, ody, oldp.v );

  /* we need to calculate panning in object coordinates 
   * current pos. - old pos.
   */ 
  g_transform.pan[0] += p.v[0] - oldp.v[0];
  g_transform.pan[1] += p.v[1] - oldp.v[1];
  g_transform.pan[2] += p.v[2] - oldp.v[2];

#ifdef DEBUG
  printf( "Move vector: (%f, %f, %f)\n",g_transform.pan[0], g_transform.pan[1], g_transform.pan[2]);
#endif  

  /* remeber old mouse position */
  omx = event->xbutton.x;
  omy = event->xbutton.y;
}

/*************************************************************
 Purpose:
 ========
        This routine rotates model with given momentum
        which is applied by accumalation of the rotation
	    done with trackball function.
 Parameters:
 ===========
        Widget w - widget in which rotation is happen
        XEvent *event - mouse event
	    String *params - parmeters
	    Cardinal *num_params - number of parameters
 Returns:
 ========
       none
**************************************************************/
void rotation (Widget w, XEvent *event, String *params,
	       Cardinal *num_params)
{
  int x, y;			/* coordinates */

  x = event->xbutton.x;
  y = event->xbutton.y;

  trackball(lastquat, 
	    (2.0*omx - C3D_win.gl_width) / C3D_win.gl_width,
	    (C3D_win.gl_height - 2.0*omy) / C3D_win.gl_height,
	    (2.0*x - C3D_win.gl_width) / C3D_win.gl_width,
	    (C3D_win.gl_height - 2.0*y) / C3D_win.gl_height
	    );
  omx = x;
  omy = y;
  if (momentum) 
    {
      if(!spinning) 
	{
	  spinning = 1;
	  animateID = XtAppAddTimeOut(kontext, 1, animate, 0);
	}
    }
  else
    {
      add_quats(lastquat, curquat, curquat);
      c3d(C3D_win.gl_width, C3D_win.gl_height);
    }
}

static void animate(XtPointer closure, XtIntervalId *id)
{
  add_quats(lastquat, curquat, curquat);
  c3d(C3D_win.gl_width,C3D_win.gl_height);
  animateID = XtAppAddTimeOut(kontext, 1, animate, 0);
}


void stopSpinning(void)
{
  if (spinning)
    {
      spinning = 0;
      XtRemoveTimeOut(animateID);
    }
}	

void
startRotation(Widget w, XEvent *event, String *params,
	      Cardinal *num_params)
{
  int x, y;

  x = event->xbutton.x;
  y = event->xbutton.y;
  omx = x;
  omy = y;
  stopSpinning();
}

/* copy float to double matrix */
static void 
copymatrix( GLfloat *mat, GLdouble *nmat )
{
  int  i;
  for (i = 0; i < 16; i++)
    nmat[i] = (GLdouble) mat[i];
}

/*****************************************************************
 C3D_getworldpoint - returns worl point.

 Parameters:
 ============
          x - x pos. of the mouse 
          y - y pos. of the mouse
          v - coordinates of world at z = 1
******************************************************************/
void
C3D_getworldpoint( int x, int y, float *v )
{
  GLint viewport[4];
  GLdouble mvmatrix[16], projmatrix[16];
  GLint realy;  /*  OpenGL y coordinate position  */
  GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */
  
  glGetIntegerv (GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
  //copymatrix( Rotation_matrix, mvmatrix );
  copymatrix( Projektions_matrix, projmatrix );
  
  /*  note viewport[3] is height of window in pixels  */
  realy = viewport[3] - (GLint) y - 1;
  gluUnProject ((GLdouble) x, (GLdouble) realy, 0.0,
		mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

  gluUnProject ((GLdouble) x, (GLdouble) realy, 0.0,
		mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
#ifdef DEBUG
  printf("Coordinates of world at z=%f (%f, %f, %f)\n", 1.0, wx, wy, wz);
#endif
  v[0] = wx; v[1] = wy; v[2] = wz;
}  








