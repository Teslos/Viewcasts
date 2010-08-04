/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   make_matrix.c                                    */
/*                                                               */
/* Routinenname       | Funktion                                 */
/* ------------------------------------------------------------- */
/*  model_matrix      : erzeugt Modelview-Matrix                 */
/*  project_matrix    : erzeugt Projection-Matrix                */
/*  save_matrix       : sichern der obersten Matrix vom Stack    */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include "grafik.h"
#include "trackball.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
GLfloat m[4][4];
static int firsted;
static void tinit();
/*                                                               */
/* Routinen :                                                    */
/* ==========                                                    */       

/* ********************************************************************* */
void model_matrix()
/* ********************************************************************* */
{ 
/* ********************************************************************* */
t_koord center;  
int i;
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
 
  if (get_center(center))
    {
/*
    glTranslatef(center[0], center[1], center[2]);
*/
    
      if (b_spin)
	{
	  build_rotmatrix(m, curquat);
	  glMultMatrixf(&m[0][0]);
	  glScalef(g_transform.scale, g_transform.scale, g_transform.scale);
	}
      else
	{
	  glRotatef(g_transform.rotx, 1.0, 0.0, 0.0);
	  glRotatef(g_transform.roty, 0.0, 1.0, 0.0);
	  glRotatef(g_transform.rotz, 0.0, 0.0, 1.0);
	  glScalef(g_transform.scale, g_transform.scale, g_transform.scale);
	}
      /*
	glTranslatef(-center[0], -center[1], -center[2]);
      */
    }
  else
    {
      glRotatef(g_transform.rotx, 1.0, 0.0, 0.0);
      glRotatef(g_transform.roty, 0.0, 1.0, 0.0);
      glRotatef(g_transform.rotz, 0.0, 0.0, 1.0);
      glScalef(g_transform.scale, g_transform.scale, g_transform.scale);
    }
  glTranslatef(g_transform.pan[0], g_transform.pan[1],
	       g_transform.pan[2]);
  glGetFloatv(GL_MODELVIEW_MATRIX, Zeichen_matrix);
  return;
/* ********************* ende model_matrix ***************************** */
}

void C3D_model_matrix()
{
  float mat[16], out[16];
  /* make translate matrix */
  M_identity(Translation_matrix);
  M_settranslation3f( g_transform.pan[0], g_transform.pan[1],
		     g_transform.pan[2], Translation_matrix );
  M_setscale( g_transform.scale, Translation_matrix );
   
  /* make rotation matrix */
  M_identity(Rotation_matrix);
  M_setrotationx( g_transform.rotx, Rotation_matrix );
  M_setrotationy( g_transform.roty, Rotation_matrix );
  M_setrotationz( g_transform.rotz, Rotation_matrix );
  build_rotmatrix(m, curquat);
  M_transfmat_to_opengl( m, mat );
  M_multmatrix(Rotation_matrix, mat, out);
  M_copymatrix(out, Rotation_matrix);
}
  
/* ********************************************************************* */
void project_matrix()
/* ********************************************************************* */
{
/* ********************************************************************* */
  glMatrixMode (GL_PROJECTION);
 
  glPushMatrix();
  glLoadIdentity();
  if (g_projektion.perspektive)
    glOrtho(g_projektion.left,g_projektion.right,g_projektion.bottom,
            g_projektion.top, g_projektion.near, g_projektion.far);
  else
    gluPerspective(20.0, g_projektion.right/g_projektion.top,
		   g_projektion.near,g_projektion.far);
  
  glTranslatef(g_transform.move[0], g_transform.move[1],
	       g_transform.move[2]);
   
  glGetFloatv(GL_PROJECTION_MATRIX, Projektions_matrix);
  glPopMatrix();
/* ********************* ende project_matrix ************************** */
  return;
}

/* initialize the matrix transformations */
static void tinit()
{
  init_transform();
}

/* track transformation of projection matrix */
void track_project_matrix()
{
  if (!firsted) {
    tinit();
    firsted = 1;
  }
      
  project_matrix();
  glMultMatrixf(Projektions_matrix);
}

/* track model matrix transformation */
void track_model_matrix()
{
  if(!firsted) {
    tinit();
    firsted = 1;
  }

  model_matrix();
  glMultMatrixf(Zeichen_matrix);
}

void C3D_track_model_matrix()
{
  int i;
  if(!firsted) {
    tinit();
    firsted = 1;
  }

  C3D_model_matrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMultMatrixf(Translation_matrix);
  glMultMatrixf(Rotation_matrix);
  C3D_save_matrix();
}

void C3D_finish_track_model_matrix()
{
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix(); /* pop up modelview matrix */ 
}

/* ********************************************************************* */
void C3D_save_matrix() /* sichern der aktuellen Transformationsmatrix -- */
/* ********************************************************************* */
{
  glGetFloatv(GL_PROJECTION_MATRIX, Save_pr_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, Save_mo_matrix);
  return;
}












