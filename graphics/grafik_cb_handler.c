#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include "grafik_func.h"
#include "color3d.h"
#include "motif.h"
#include "gl_local.h"

#include <GL/GLwDrawA.h>
#include <GL/gl.h>

#ifdef DEBUG
  #define PRINT_DEBUG(str) (if(c3d_da_input_debug)\
                            (if(applRessourcen.debug) fprintf(stderr, str);)
#else
  #define PRINT_DEBUG(str) 
#endif

/* Routinen: ---------------------------------------------------------- */
/* c3d_init */
/* c3d_expose */
/* c3d_input */
/* da_expose */
/* da_resize */
/* da_input */

/* ******************************************************************** */
void  c3d_init(Widget w, XtPointer client_data,
                        XtPointer call)
/* ******************************************************************** */
{
  GLwDrawingAreaCallbackStruct *call_data;
  Arg args[1];
  XVisualInfo *vi;
  int start;

  call_data = (GLwDrawingAreaCallbackStruct *) call;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** Start c3d_init() ***\n");
#endif

  XtSetArg(args[0], GLwNvisualInfo, &vi);
  XtGetValues(w, args, 1);
 
  if (applRessourcen.debug)
    printf("\nPass 1.");

  /* create the graphic context */
  glx_context = CreateGLXContext(glx_context,XtDisplay(w), vi);
   
  GLwDrawingAreaMakeCurrent(w, glx_context);

  C3D_win.cx = glx_context;

  gl_width  = call_data->width;
  gl_height = call_data->height;

  C3D_win.gl_width  = gl_width;
  C3D_win.gl_height = gl_height;
  
  
  c3d_DrawWindow(gl_width, gl_height);

  //C3D_create_overlay( C3D_win.frame, &C3D_win );

#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** End c3d_init() ***\n");
#endif
}

/* ******************************************************************** */
void  c3d_expose(Widget w, XtPointer client_data,
                           XtPointer call)
/* ******************************************************************** */
{
  XmDrawingAreaCallbackStruct *call_data;
  XEvent *event, ahead;
  int    mode;
  call_data = (XmDrawingAreaCallbackStruct *) call;
  mode = QueuedAfterReading;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** c3d_expose() ***\n");
#endif
  
  /* Weitere expose-Events desselben Widgets aus der Queue loeschen */
  event = call_data->event; 
  if (event->type == Expose)  
    {
      while (XEventsQueued(XtDisplay(w), mode) > 0)
	{
	  XPeekEvent(XtDisplay(w), &ahead);
	  if ((ahead.type != Expose) ||
	      (ahead.xexpose.window != event->xexpose.window))
	    break;
	  XNextEvent(XtDisplay(w), &ahead);
	}
    }

  if (event->xexpose.count == 0)
        C3D_postredisplay();
}

/* ******************************************************************** */
void  c3d_resize(Widget w, XtPointer client_data,
                        XtPointer call)
/* ******************************************************************** */
{
  GLwDrawingAreaCallbackStruct *call_data;
#ifdef DEBUG  
  if (applRessourcen.debug)
    fprintf(stderr, "*** c3d_resize() ***\n");
#endif
  call_data = (GLwDrawingAreaCallbackStruct *) call;
  gl_width  = call_data->width;
  gl_height = call_data->height;

  C3D_win.gl_width  = gl_width;
  C3D_win.gl_height = gl_height;

  vid.width  = gl_width;
  vid.height = gl_height;

  glXWaitX();
  
  if (cutting_on)
  {
      reshapewindow();
  }

  GLwDrawingAreaMakeCurrent( C3D_win.wid, C3D_win.cx );
  C3D_postredisplay();
}

/* ******************************************************************** */
void  c3d_input(Widget w, XtPointer client_data,
                        XtPointer call)
/* ******************************************************************** */
{
  GLwDrawingAreaCallbackStruct *call_data;
  call_data = (GLwDrawingAreaCallbackStruct *) call;
  XmProcessTraversal(w, XmTRAVERSE_CURRENT);
  da_input(w,client_data,call_data);
}

/* ******************************************************************** */
XtCallbackProc da_expose(Widget w, XtPointer client_data, XtPointer cbs)
/* ******************************************************************** */
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** da_expose(...) ***\n");
#endif
  return(NULL);
} 

/* ******************************************************************** */
XtCallbackProc da_resize(Widget w, XtPointer client_data,
                        XmDrawingAreaCallbackStruct *call_data)
/* ******************************************************************** */
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** da_resize(...) ***\n");
#endif
  
  return(NULL);
} 

/* ******************************************************************** */
/* Process all Input callbacks*/
XtCallbackProc da_input(Widget w, XtPointer client_data, 
                       XmDrawingAreaCallbackStruct *call_data)
/* ******************************************************************** */
{
  char buffer[1];
  KeySym keysym;
  t_bool c3d_da_input_debug;
  XKeyEvent *key_event;

  c3d_da_input_debug = FALSE;

  PRINT_DEBUG ("*** c3d_input()/da_input() ***\n")

  switch(call_data->event->type)
  {
    case KeyPress:
    {
#ifdef DEBUG
      if (applRessourcen.debug)
        fprintf(stderr, "KeyPress | keycode = >%d<\n", call_data->event->xkey.keycode);

      if (c3d_da_input_debug)
        if (applRessourcen.debug)
          fprintf(stderr, "KeyPress\n\n");
#endif
      if (XLookupString((XKeyEvent *)call_data->event, buffer,
			1, &keysym, NULL) == 1)
      {
	
#ifdef DEBUG
        if (c3d_da_input_debug)
          if (applRessourcen.debug)
            fprintf(stderr, "KeyPress buffer = >%1.1s<\n", buffer);
#endif
	/* see if keyboard key is pressed */
	if (cutting_on) 
	  {
	    mouse_plane(w, call_data->event, NULL, NULL);
	  }

	/* see if mouse button is pressed */
	switch(call_data->event->xbutton.button) 
	  {
	  case Button1:
	    {
	      
	      set_cursor_handler(w, PICK_CURSOR);
	      if(g_picking.pick_on)
		{
		  mode = PICK;
		  start_pick(w, call_data->event, NULL, NULL);
		}
	      break;
	    }
	  case Button2:
	    {
	      break;
	    }
	  case Button3:
	    {
	      break;
	    }
	  }
      }
      break;
    }
    case KeyRelease:
    {
      PRINT_DEBUG("KeyRelease\n\n")

      if (XLookupString((XKeyEvent *)call_data->event, buffer, 1, &keysym, NULL) == 1)
      {
#ifdef DEBUG
        if (c3d_da_input_debug)
          if (applRessourcen.debug)
            fprintf(stderr, "KeyRelease buffer = >%1.1s<\n", buffer);
#endif
	
      }
      break;
    }
    case ButtonPress:
    {
      PRINT_DEBUG("ButtonPress\n\n")

      switch(call_data->event->xbutton.button)
      {
        case Button1:
        {
	  /* choose picking or translating */ 
          if(call_data->event->xbutton.state == 1) 
	    {
	      set_cursor_handler(w, PICK_CURSOR);
	      if (g_picking.pick_on)
		{
		  mode = PICK;
		  set_cursor_handler(w, PICK_CURSOR);
		  start_pick(w, call_data->event, NULL, NULL);
		} 
	    }
	  else if (cutting_on)
	    {
	      mouse_plane(w, call_data->event, NULL, NULL);
	    }
	  else
	    {
	      mode = TRANSLATE;
	      set_cursor_handler(w, TRANSLATE_CURSOR);
	      start_move(w, call_data->event, (String*) " ", (Cardinal*) 1);
	    }
	  break;
        }
        case Button2:
        {
          mode = ROTATE;
          set_cursor_handler(w, ROTATE_CURSOR);
	  /* see if spin is set */
	  if (b_spin == TRUE)
	    /*spin_rot(w, call_data->event, NULL, NULL);*/
	    /*rotation(w, call_data->event, NULL, NULL);*/
	    startRotation(w, call_data->event, NULL, NULL);
	  else if (cutting_on)
	    {
	      mouse_plane(w, call_data->event, NULL, NULL);
	    }
	  else
	    start_rot(w, call_data->event, NULL, NULL);
          break;
        }
        case Button3:
        {
          set_cursor_handler(w, PICK_CURSOR);
          if (g_picking.pick_on)
          {
            mode = PICK;
            set_cursor_handler(w, PICK_CURSOR);
            start_pick(w, call_data->event, NULL, NULL);
          }
          break;
        }
      }
      break;
    }
    case ButtonRelease:
    {
      PRINT_DEBUG("ButtonRelease\n\n")

      switch(call_data->event->xbutton.button)
      {
        case Button1:
        {
	  mode = TRANSLATE;
          if (cutting_on)
	    {
	      mouse_plane(w, call_data->event, NULL, NULL);
	    }
	  else
	    {
	      set_cursor_handler(w, TRANSLATE_CURSOR);
	      end_move(w, call_data->event, NULL, NULL);
	      C3D_postredisplay();
	    }
          break;
        }
        case Button2:
        {
          mode = ROTATE;
          set_cursor_handler(w, ROTATE_CURSOR);
	  if( b_spin == TRUE)
	    /*spin_rot(w, call_data->event, NULL, NULL);*/
	    /*rotation(w, call_data->event, NULL, NULL);*/
	    ;
	  else if (cutting_on)
	    {
	      mouse_plane(w, call_data->event, NULL, NULL);
	    }
	  else
	    end_rot(w, call_data->event, NULL, NULL);
          C3D_postredisplay();
          break;
        }
      }
      break;
    }
    case MotionNotify:
      {
	if (cutting_on)
	  {
	    mouse_plane(w, call_data->event, NULL, NULL);
	    break;
	  }

	if (mode == TRANSLATE) 
	  { 
	    move(w, call_data->event, NULL, NULL);
	    C3D_postredisplay();
	    break;
	  }

	PRINT_DEBUG("MotionNotify\n\n")

      /* Verschieben */
      if (mode == TRANSLATE && call_data->event->xmotion.state & Button1Mask)
      {
	PRINT_DEBUG("MotionNotify & Button1Mask | mode = TRANSLATE\n")

	printf("Mouse Verschieben\n");
        end_move(w, call_data->event, NULL, NULL);
        C3D_postredisplay();
      }
      else if (mode == TRANSLATE)
      {
	PRINT_DEBUG("MotionNotify | mode = TRANSLATE\n")

	if (cutting_on)
	  {
	    mouse_plane(w, call_data->event, NULL, NULL);
	  }
	else
	  {
	    end_move(w, call_data->event, NULL, NULL);
	    C3D_postredisplay();
	  }
      }
      /* Drehen */
      else if (mode==ROTATE && call_data->event->xmotion.state & Button2Mask)
      {
	PRINT_DEBUG("MotionNotify & Button2Mask | mode = ROTATE\n")

	if( b_spin == TRUE)
	  /*spin_rot(w, call_data->event, NULL, NULL);*/
	  rotation(w, call_data->event, NULL, NULL);
	else
	  end_rot(w, call_data->event, NULL, NULL);
        C3D_postredisplay();
      }
      else if (mode == ROTATE)
      {
#ifdef DEBUG
        if (c3d_da_input_debug)
          if (applRessourcen.debug)
            fprintf(stderr, "MotionNotify | mode = ROTATE\n");
#endif
	if( b_spin == TRUE )
	  /*spin_rot(w, call_data->event, NULL, NULL);*/
	  rotation(w, call_data->event, NULL, NULL);
	else if (cutting_on)
	  {
	    mouse_plane(w, call_data->event, NULL, NULL);
	  }
	else
	  {
	    end_rot(w,call_data->event, NULL, NULL);
	    C3D_postredisplay();
	  }
      }
      /* Zoomen */
      else if (mode == ZOOM && call_data->event->xmotion.state & Button2Mask)
      {
#ifdef DEBUG
        if (c3d_da_input_debug)
          if (applRessourcen.debug)
            fprintf(stderr, "MotionNotify & Button2Mask | mode = ZOOM\n");
#endif

        end_zoom(w, call_data->event, NULL, NULL);
        C3D_postredisplay();
      }
      else if (mode == ZOOM)
      {
#ifdef DEBUG
        if (c3d_da_input_debug)
          if (applRessourcen.debug)
            fprintf(stderr, "MotionNotify | mode = ZOOM\n");
#endif
        end_zoom(w, call_data->event, NULL, NULL);
        //update_scene();
        C3D_postredisplay();
      }
      break; 
    }
  case ConfigureNotify:
    if (cutting_on)
      {
	mouse_plane(w, call_data->event, NULL, NULL);
      }
    break;
  }
  return(NULL);
/* ******************************************************************** */
}
