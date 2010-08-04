/*
/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   grafik_func.c                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*  c3d_DrawWindow - Aufruf der Zeichenfunktion fuer Ogl-Terminal*/
/*  C3D_postredisplay     - Neuzeichnen auf Ogl-Terminal -------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>

#include "grafik_func.h"
#include "grafik.h"
#include "color3d.h"
#include "motif.h"

#include <GL/gl.h>

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/*****************************************************************/
void c3d_DrawWindow(int gl_width, int gl_height)
/*****************************************************************/
{
/*
  glViewport(0, 0, gl_width, gl_height);
*/
  glViewport(0, 0, C3D_win.gl_width, C3D_win.gl_height);

  C3D_clearcolor();
	
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  c3d(gl_width, gl_height);

  return;
}

/*****************************************************************/
t_bool C3D_redraw(XtPointer closure)
/*****************************************************************/
{
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
  c3d_DrawWindow(C3D_win.gl_width, C3D_win.gl_height);
  redisplaypending = 0;
  return TRUE;
}

void C3D_postredisplay()
{
  if (!redisplaypending) 
    {
      redisplayID = XtAppAddWorkProc( kontext, C3D_redraw, 0 );
      redisplaypending = 1;
    }
}

/*****************************************************************/

/*****************************************************************/
void C3D_clearcolor()
/*****************************************************************/		
{
  int i;
  
  switch (g_ClearColor) 
    {
    case 1:
      {
	glClearColor(1.0, 1.0, 1.0, 0.); /* white background color */
	COPYCOLOR3(c3d_foreground_color,RGB_Schwarz); /* foreground color */
	break;
      }
    case 2:
      {
	glClearColor(0.25,0.25,0.5,0.0); 
	COPYCOLOR3(c3d_foreground_color,RGB_Weiss);
	break;
      }
    case 3:
      {
	GLint viewport[4];

	/* paint blue to white */
	glClear( GL_DEPTH_BUFFER_BIT );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glShadeModel( GL_SMOOTH );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glTranslatef(-g_transform.move[0], -g_transform.move[1],
		     -g_transform.move[2]);
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glColor3f( 0.0, 0.0, 1.0 );
	glVertex3f( g_projektion.left, g_projektion.top, -300);
	glVertex3f( g_projektion.right, g_projektion.top, -300 );

	glColor3f( 1.0, 1.0, 1.0 );
	glVertex3f( g_projektion.right, g_projektion.bottom, -300 );
	glVertex3f( g_projektion.left, g_projektion.bottom, -300 );

	glEnd();

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	/* enable depth test */
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_LIGHTING );
	break;
      }
    default:
      {
	glClearColor (0.0, 0.0, 0.0, 0.0);
	COPYCOLOR3(c3d_foreground_color,RGB_Weiss);
	break;
      }
    }
}
