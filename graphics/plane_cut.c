#include <stdio.h>
#include <math.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


#include "grafik.h"
#include "motif.h"
#include "gendlist.h"
#include "genstack.h"
#include "GraphicsGems.h"
#include "memory.h"

enum { LEFT_MOUSE = 1,
       MIDDLE_MOUSE = 2,
       RIGHT_MOOUSE = 3
};

enum { NCUBE_EDGES = 12 };

#define INITDEPTH -3.0
#define FOV 45

#define MIN( a, b ) (((a) < (b)) ? (a) : (b))
#define MAX( a, b ) (((a) > (b)) ? (a) : (b))

#define MAXPLANE     6
#define MAXTRANSFORM (MAXPLANE+2)
#define EDIT_VIEW    (MAXTRANSFORM-1)
#define EDIT_OBJECT  (MAXTRANSFORM-2)
#define MAXMODEL     4

#define OFFSET_FACTOR  0.8
#define OFFSET_BIAS    0.001

extern GLfloat norm_cut[3];

static float boink_xr;
static float boink_zr;
static float boink_depth;

float xr[MAXTRANSFORM] = {0,0,0,0,0,0,0,0};
float zr[MAXTRANSFORM] = {0,0,0,0,0,0,0,0};
float yr[MAXTRANSFORM] = {0,0,0,0,0,0,0,0};
float depth[MAXTRANSFORM] = {0,0,0,0,0,0,0,INITDEPTH};
t_bool y_rotation = FALSE;

static long penable[MAXPLANE] = {1,0,0,0,0,0};
static long pface[MAXPLANE] = {0,0,0,0,0,0};

static float boink_rot_scale;
static float boink_depth_scale;
static short boink_screen_x;
static short boink_screen_y;

extern int model;
extern int docap;
extern int dointersection;
extern int motion;
extern int t_select;
extern int tselect;
extern int mselect;
extern int twoside;
extern int ctrlkey;
extern int leftshiftkey;
extern int rightshiftkey;
extern int leftmouse;
extern int middlemouse;
extern int orangecaps;

void biggest_box(float vol[][3]);
void mbiggest_box(float vol[][3]);
static void draw_arrow();
static void draw_model(void (*func)());
static void draw_square(float xdim, float ydim);
static void draw_polygon(float (*inter_points)[3], int ninter, float *nplane);
static void draw_box(float vol[][3], GLenum type);
void setinvmat(int i);
void dnorm(double v[4]);
void norplane(double *eq);
void assignm(float *GLmat, Matrix4 *GGmat, int invert);
void drawp(struct dlistelem *p, void *arg);

/***********************************************************
 This function handle mouse invents which comes from X
 server. Then it calls appropriate functions.
************************************************************/
void mouse_plane (Widget w, XEvent *event, String* params,
		  Cardinal* paramsAnzAdr)
{  
  /* local variables */
  register int i;
  static init = 1;               /* initialize flag       */
  int ch;                        /* character             */
  int screen_x, screen_y;        /* screen coordinates    */
  char s[100];                   /* string for key events */
  KeySym key;
  XKeyEvent* key_event;          /* key event             */
  XButtonEvent* button_event;    /* button event          */
  XMotionEvent* motion_event;    /* motion event          */
  XConfigureEvent* config_event; /* configure event       */
  XExposeEvent* expose_event;    /* expose event          */
  
  if (init) 
    {
      initparams();
      init = 0;
    }

  /* prepare event loop */
  button_event = (XButtonEvent *) (event);
  key_event    = (XKeyEvent *) (event);
  motion_event = (XMotionEvent *) (event);
  expose_event = (XExposeEvent *) (event);
  config_event = (XConfigureEvent *) (event);
 
  
  switch (event->type) {
  
    /* key events */
  case KeyPress:
    {
      ch = XLookupString(key_event, s, 100, &key, NULL);
      if (ch) 
	{
	  switch(key) 
	    {
	    case '0':
	      if(!tracking()) 
		{
		  penable[0] = GL_TRUE;
		  tselect = 0;
		}
	      break;
	    case ')':  /* shifted zero */
	      penable[0] = GL_FALSE;
	      break;
	    case '1':
	      if(!tracking()) 
		{
		  penable[1] = GL_TRUE;
		  tselect = 1;
		}
		break;
	    case '!':  /* shifted one */
	      penable[1] = GL_FALSE;
	      break;
	    case '2':
	      if(!tracking()) 
		{
		  penable[2] = GL_TRUE;
		  tselect = 2;
		}
	      break;
	    case '@':  /* shifted one */
	      penable[2] = GL_FALSE;
	      break;
	    case '3':
	      if(!tracking()) 
		{
		  penable[3] = GL_TRUE;
		  tselect = 3;
		}
		break; 
	    case '#':  /* shifted one */
	      penable[3] = GL_FALSE;
	      break;
	      case '4':
		if(!tracking()) 
		  {
		    penable[4] = GL_TRUE;
		    tselect = 4;
		  }
		break; 
	    case '$':  /* shifted one */
	      penable[4] = GL_FALSE;
	      break;
	    case '5':
	      if(!tracking()) 
		{
		  penable[5] = GL_TRUE;
		  tselect = 5;
		}
	      break; 
	      case '%':  /* shifted one */
		penable[5] = GL_FALSE;
		break;
	    case '6':
	      if(!tracking()) 
		{
		  penable[6] = GL_TRUE;
		  tselect = 3;
		}
	      break; 
	    case '^':  /* shifted one */
	      penable[3] = GL_FALSE;
	      break;
	    case 'c':
	      docap = !docap;
	      break;
	    case 'd':
	      for(i = 0; i <= MAXTRANSFORM; i++)
		printf("xr=%g zr=%g depth=%g\n",
		       xr[i], zr[i], depth[i]);
	      break;
	    case 'i':
	      dointersection = !dointersection;
	      break;
	    case 'l':
	      twoside = !twoside;
	      if(twoside)
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	      else
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	      break;
	    case 'm':
	      motion = !motion;
	      mselect = tselect;
	      break;
	    case 'o':
	      if(!tracking())
		tselect = EDIT_OBJECT;
	      break;
	    case 'v':
	      if(!tracking())
		tselect = EDIT_VIEW;
	      break;
	    case XK_x:
	      xr[tselect] = 0.0;
	      zr[tselect] = 90.0;
	      break;
	    case XK_y:
	      xr[tselect] = 0.0;
	      zr[tselect] = 0.0;
	      break;
	    case XK_z:
	      xr[tselect] = 90.0;
	      zr[tselect] = 0.0;
	      break;
	    case XK_KP_Enter:
	      printf("Cutting done\n");
	      cutting_on = FALSE;
	      break;
	    }
	}
      break;
    }
    /* button events */
  case ButtonPress:
    {
      if(button_event->button == LEFT_MOUSE) 
	{
	  leftmouse = GL_TRUE;
	  boink_screen_x = button_event->x;
	  boink_screen_y = -button_event->y;
	  boink_depth = depth[tselect];
	  boink_depth_scale = 0.002;
	  if(shifted())
	    boink_depth_scale /= 10.0;
	  else if(ctrlkey)
	    boink_depth_scale /= 100.0;
	}
      else if(button_event->button == MIDDLE_MOUSE)
	{
	  middlemouse = GL_TRUE;
	  boink_screen_x = button_event->x;
	  boink_screen_y = -button_event->y;
	  boink_xr = xr[tselect];
	  boink_zr = zr[tselect];
	  boink_rot_scale = 0.333;
	  if(shifted())
	    boink_rot_scale /= 10.0;
	  else if(ctrlkey)
	    boink_rot_scale /= 100.0;
	}
      break;
    }
    /* when button is released calculate 
     * new depth of cutting plane.
     */
  case ButtonRelease:
    {
      screen_x = button_event->x;
      screen_y = -button_event->y;
      if(button_event->button == LEFT_MOUSE)
	{
	  leftmouse = GL_FALSE;
	  depth[tselect] = boink_depth - boink_depth_scale *
	    (float)(boink_screen_x - screen_x);
	}
      else if(button_event->button == MIDDLE_MOUSE)
	{
	  middlemouse = GL_FALSE;
	  zr[tselect] = boink_zr + boink_rot_scale *
	    (float)(boink_screen_x - screen_x);
	  xr[tselect] = boink_xr + boink_rot_scale *
	    (float)(boink_screen_y - screen_y);
	  limitrot();
	}
      break;
    }
    /* when mouse is moving calculate
     * new angle or depth of cutting plane.
     */  
  case MotionNotify:
    {
      screen_x = motion_event->x;
      screen_y = -motion_event->y;
      if(leftmouse)
	{
	  depth[tselect] = boink_depth - boink_depth_scale *
	    (float) (boink_screen_x - screen_x);
	}
      else if(middlemouse)
	{
	  zr[tselect] = boink_zr + boink_rot_scale *
	    (float) (boink_screen_x - screen_x);
	  xr[tselect] = boink_xr + boink_rot_scale *
	    (float) (boink_screen_y - screen_y);
	  limitrot();
	}
      break;
    }
  case Expose:
    break;
  case ConfigureNotify:
    {
      long xdim, ydim;
      xdim = config_event->width;
      ydim = config_event->height;
      reshapewindow();
      break;
    }
  default:
    printf("No events\n");
    break;
  }
  if (motion && (!middlemouse || (mselect != tselect)))
    {
      while(!XtAppPending(kontext))
	{
	  zr[tselect] += (float)screen_x / 100.0;
	  xr[tselect] -= (float)screen_y / 100.0;
	  limitrot();
	  drawcutted();
	}
    }
  else 
    {
      if(!XtAppPending(kontext))
	drawcutted();
    }
}

extern GLdouble cp[4];
extern GLfloat mat_view[16]; 

void drawcutted() 
{
  int i,j;
  float x[3], xinv[3];
  float rv[4], plv[4];  /* result vector, plane vector */
  float sign;           /* factor which deciside plane orientation */ 
  double v_view[]= {0.0,0.0,-1.0,1.0};      /* view vector */
  float v_view1[]= {0.0,0.0,-1.0,1.0};
  float v1[3], v2[3], v3[3], P[3];
  float size = 10;
  float ccp[4];
  float cut[16];
  float temp[4];
  double eq[4];
  float  rp[4];
  GLfloat invmat[16];
  char *dummy;
  const GLubyte *errString;
  float D;
  GLfloat vd[4], vinv1[4], p1[3], p2[3],
	    p3[3], p4[3];
  GLfloat mat_tran[16], mat0[16], cpn[4];
  float ip[3], inter_points[NCUBE_EDGES][3];

  void (*func)();

  /* print errors */
  while (i = glGetError()) {
    errString = gluErrorString(i);
    fprintf(stderr, "ERROR: %s\n", errString);
  }

  /* clear the screen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  track_project_matrix();

  glMatrixMode( GL_MODELVIEW );
  C3D_track_model_matrix();

  for (i = 0; i < MAXPLANE; i++)
    {
      if ((tselect == i) || (tselect == EDIT_VIEW) ||
	 (motion && (mselect == i)))
	{
	 
	  GLfloat mat[16], matp[16];
	  
	  glPushMatrix();
	  glGetFloatv(GL_MODELVIEW_MATRIX, matp);
	  setinvmat(i);
	  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	  pface[i] = (mat[14] < 0) ? 1 : 0;
	  glPopMatrix();
	 
	  
	  glGetFloatv(GL_MODELVIEW_MATRIX, mat_view);
	  v1[0] = mat_view[0]; v1[1] = mat_view[4];
	  v1[2] = mat_view[5];
	  v2[0] = mat_view[1]; v2[1] = mat_view[5];
	  v2[2] = mat_view[9];

	  size = 10.0;
	  Vecinv(vinv1, v1);
	  Vecdis(vd,v2,vinv1);
	  p1[0] = size * vd[0];
	  p1[1] = size * vd[1];
	  p1[2] = size * vd[2];
	  Vecdis(vd, v2, v1);
	  p2[0] = size * vd[0];
	  p2[1] = size * vd[1];
	  p2[2] = size * vd[2];
	  Vecadd(vd, v1, v2);
	  p3[0] = size * vd[0];
	  p3[1] = size * vd[1];
	  p3[2] = size * vd[2];
	  Vecadd(vd, vinv1, v2);
	  p4[0] = size * vd[0];
	  p4[1] = size * vd[1];
	  p4[2] = size * vd[2];

	  /* get plane normal */
	  crossprod(v_view, v1, v2);

	  glPushMatrix();
	  setmatrix(i);
	  
	  /* get the current matrix */
	  glGetFloatv(GL_MODELVIEW_MATRIX, cut);

	  /* get trans. of plane equation */
	  multMxV(cut, cp, ccp);
	  
	  /* decision, if we look at plane or not */
	  sign = dotproduct(ccp, v_view);
	    pface[i] = (sign >  0) ? 1 : 0;
	  glPopMatrix();
	  glPushMatrix();
	  setmatrix(i);
	  glGetFloatv(GL_MODELVIEW_MATRIX, mat0);
	  glClipPlane(GL_CLIP_PLANE0 + i, cp);
	  glPopMatrix();
	}
    }

 
  /*
   * Draw now the cut on the model
   */
  glPushMatrix();
  glLoadIdentity();
  setmatrix(tselect);
  glGetFloatv(GL_MODELVIEW_MATRIX, invmat);
  multMxV(invmat, cp, rp);
  glPopMatrix(); 

#ifdef DEBUG
 printf("Vector plane: %f, %f, %f\n", rp[0],rp[1],rp[2],rp[3]);
#endif
 
 g_transform.norm[0] = (float) rp[0];
 g_transform.norm[1] = (float) rp[1];
 g_transform.norm[2] = (float) rp[2];

 g_transform.d = (float)depth[tselect];
 rp[3] = (float)depth[tselect];
 func = zeichne_geometrie;
 draw_model((*func));
 
  /* draw current clipping plane */
  if ((tselect < MAXPLANE) && penable[tselect])
    {
      GLfloat mat[16], cpn[4];

      /* bigest volume, x-dim. y-dim. of cut. plane*/ 
      float vol[3][3], xdim, ydim, zdim;
      float ebox[3], center[3], axis[3][3]={{1.,0.,0.},{0.,1.,0.},{0.,0.,1.}};
      int ninter=0;

      glPushMatrix();
      setmatrix(tselect);
      glColor3f(1.0,1.0,1.0);
     
      		
      /* get modelview matrix of current transformation */
      glGetFloatv(GL_MODELVIEW_MATRIX, mat);

      /* multiply modelview matrix with starting cut plane vec. */
      multMxV(mat, cp, cpn);
      cpn[3] = (float) depth[tselect];
      mbiggest_box(vol);

      glMatrixMode(GL_MODELVIEW);
      if (pface[tselect])  
	glColor4f(0.0,0.9,0.0,0.2);
      else  
	glColor4f(0.0,0.9,0.9,0.2);

      /* xmax - xmin = xdim */
      xdim = vol[0][1] - vol[0][0];
      ydim = vol[1][1] - vol[1][0];
      zdim = vol[2][1] - vol[2][0];

      ebox[0]  = xdim/2;
      ebox[1]  = ydim/2;
      ebox[2]  = zdim/2;
      center[0] = vol[0][0] + vol[0][1] / 2.0;
      center[1] = vol[1][0] + vol[1][1] / 2.0;
      center[2] = vol[2][0] + vol[2][1] / 2.0;
      
      /* is plane cutting the box ? */
      if (box2plane(ebox, axis, rp, center))
	{
	  int i;
	  int edges[12][2] = {{0,4},{1,5},{2,6},{3,7},
			      {0,3},{1,2},{5,6},{4,7},
			      {0,1},{4,5},{6,7},{2,3}
	  };	     
	  float v[8][3]; 

	  v[0][0] = v[1][0] = v[2][0] = v[3][0] = vol[0][0];
	  v[4][0] = v[5][0] = v[6][0] = v[7][0] = vol[0][1];
          v[0][1] = v[1][1] = v[4][1] = v[5][1] = vol[1][0];
	  v[2][1] = v[3][1] = v[6][1] = v[7][1] = vol[1][1];
	  v[0][2] = v[3][2] = v[4][2] = v[7][2] = vol[2][0];
	  v[1][2] = v[2][2] = v[5][2] = v[6][2] = vol[2][1];

	  /* go thru all edges and find intersections */
	  for (i = 0; i < NCUBE_EDGES; i++)
	    {
	      if (edge_inter_plane(rp, v[edges[i][0]], v[edges[i][1]], ip) != 0)
		{
		  inter_points[ninter][0] = ip[0];
		  inter_points[ninter][1] = ip[1];
		  inter_points[ninter][2] = ip[2];
		  ninter++;
		}
	      else
		{
#ifdef DEBUG
		  printf("Tangent point %i: %f, %f, %f\n", i, ip[0],ip[1],ip[2]);
#endif
		}
	    }
	 
	}  

      glPopMatrix();
      draw_polygon(inter_points, ninter, rp);
    }

  if (C3D_win.doublebuffer)
    glXSwapBuffers(XtDisplay(gl_widget), XtWindow(gl_widget));
  else
    glFlush();
  
  C3D_finish_track_model_matrix( );
  
  glMatrixMode ( GL_PROJECTION );
  glPopMatrix();
}

void norplane(double *eq)
{
  double N = 1.0/sqrt(eq[0]*eq[0]+eq[1]*eq[1]+eq[2]*eq[2]);
  if (eq[3] < 0.0) {
    eq[0] *= N;
    eq[1] *= N;
    eq[2] *= N;
    eq[3] *= N;
  } else {
    eq[0] *= -N;
    eq[1] *= -N;
    eq[2] *= -N;
    eq[3] *= -N;
  }
}

void setinvmatmodel(void)
{
  int i,j;
  glRotatef(-g_transform.rotz, 0.0, 0.0, 1.0);
  glRotatef(-g_transform.roty, 0.0, 1.0, 0.0);
  glRotatef(-g_transform.rotx, 1.0, 0.0, 0.0);
  glTranslatef(-g_transform.move[0], -g_transform.move[1],
	       -g_transform.move[2]);
  glRotatef(90.0, 1.0, 0.0, 0.0);
}

void setmatrix(long i) {
  float rot_x, rot_y, rot_z;
  /* do viewing transformation */
  /*glTranslatef(0.0,0.0,INITDEPTH);*/
  /* multiply with viewing transformations */
  /*glMultMatrixf(Zeichen_matrix);*/

  /* do model transformation */
  glRotatef(-zr[i], 0, 0, 1);
  glRotatef( xr[i], 1, 0, 0);
  glRotatef( yr[i], 0, 1, 0);
  glTranslatef(0.0, depth[i], 0.0);

#ifdef DEBUG
  printf("Rotate y: %f\n", -zr[i]);
  printf("Rotate x: %f\n",  xr[i]);
  printf("Translate z: %f\n", depth[i]);
#endif

}

void setinvmat(int i) 
{
  /* Matrix4 m, invm;
     GLfloat Zeichne_inverse[16];*/

  /* compute inverse of model transformation */
  glTranslatef(0.0,-depth[i],0.0);
  glRotatef(-yr[i], 0, 1, 0);
  glRotatef(-xr[i], 1, 0, 0);
  glRotatef( zr[i], 0, 0, 1);
  /*assignm(Zeichen_matrix, &m, 0); 
  inverse(&m, &invm);
  assignm(Zeichne_inverse, &invm, 1);
  glMultMatrixf(Zeichne_inverse);*/
  /*glTranslatef(0.0,0.0,-INITDEPTH);*/
  /* compute inverse of viewing transformation */
  /*
  glRotatef(-g_transform.rotz, 0, 0, 1);
  glRotatef(-g_transform.roty, 0, 1, 0);
  glRotatef(-g_transform.rotx, 1, 0, 0);
  */
  
}

void assignm(float *GLmat, Matrix4 *GGmat, int invert)
{
  int i,j;
  j = 0;
  for (i = 0; i < 16; i++) {
    if ((i%4 == 0) && (i != 0)) j++;
    if (invert) 
      GLmat[i] = GGmat->element[j][i%4];
    else
      GGmat->element[j][i%4] = GLmat[i];
  }
}

void mbiggest_box(float vol[][3])
{
  int stoffnr;
  vol[0][0] = INFINITE; vol[0][1] = -INFINITE;
  vol[1][0] = INFINITE; vol[1][1] = -INFINITE;
  vol[2][0] = INFINITE; vol[2][1] = -INFINITE;
  for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
    {
     if (g_stoffgebiete[stoffnr-1].zeichne_on != 0)
       {
	 vol[0][0] = MIN(vol[0][0], g_stoffgebiete[stoffnr-1].min[0]);
	 vol[0][1] = MAX(vol[0][1], g_stoffgebiete[stoffnr-1].max[0]);
	 vol[1][0] = MIN(vol[1][0], g_stoffgebiete[stoffnr-1].min[1]);
	 vol[1][1] = MAX(vol[1][1], g_stoffgebiete[stoffnr-1].max[1]);
	 vol[2][0] = MIN(vol[2][0], g_stoffgebiete[stoffnr-1].min[2]);
	 vol[2][1] = MAX(vol[2][1], g_stoffgebiete[stoffnr-1].max[2]);
       }
    }
}

void biggest_box(float vol[][3])
{
  int stoffnr;

  vol[0][0] = INFINITE; vol[0][1] = -INFINITE;
  vol[1][0] = INFINITE; vol[1][1] = -INFINITE;
  vol[2][0] = INFINITE; vol[2][1] = -INFINITE;
  for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
    {
      vol[0][0] = MIN(vol[0][0], g_stoffgebiete[stoffnr-1].min[0]);
      vol[0][1] = MAX(vol[0][1], g_stoffgebiete[stoffnr-1].max[0]);
      vol[1][0] = MIN(vol[1][0], g_stoffgebiete[stoffnr-1].min[1]);
      vol[1][1] = MAX(vol[1][1], g_stoffgebiete[stoffnr-1].max[1]);
      vol[2][0] = MIN(vol[2][0], g_stoffgebiete[stoffnr-1].min[2]);
      vol[2][1] = MAX(vol[2][1], g_stoffgebiete[stoffnr-1].max[2]);
    }
}

void draw_model(void (*func)())
{
  GLfloat vol[3][3];
  int i;

  /* setup matrix for drawing the object */
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //glPushMatrix();
  

  //glMatrixMode( GL_PROJECTION );
  //glPushMatrix();
  //glLoadIdentity( );
  //track_project_matrix();
  //farclip(GL_TRUE);
  
  glMatrixMode( GL_MODELVIEW );
  //setmatrix(EDIT_OBJECT);
  //C3D_track_model_matrix();
  glEnable(GL_LIGHTING);
  (*func)();
  mbiggest_box(vol);
  draw_box(vol, GL_LINE_LOOP);
  glDisable(GL_LIGHTING);
  
  //C3D_finish_track_model_matrix();
  //farclip(GL_TRUE);
  //glPopMatrix();
  
  //glMatrixMode( GL_PROJECTION );
  //glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
}

static void draw_square(float xdim, float zdim) 
{
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  /* glNormal3fv(norm_cut); */
  glVertex4f(-xdim,0,-zdim,0.7);
  glVertex4f( xdim,0,-zdim,0.7);
  glVertex4f( xdim,0, zdim,0.7);
  glVertex4f(-xdim,0, zdim,0.7);
  glEnd();
  /*
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glRecti(-10, -10, 10, 10);**/
  glEnable(GL_LIGHTING);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  
}

static void drawarrow( float x, float y, float z, float *nplane )
{
  GLuint arrow;
  float angle;
  float zn[] = {0.0, 0.0, 1.0};  /* z normal vector */
  float zout[3];
  float pnormal[3];
  float vecrot[3];
  float degrees;
  
  arrow = glGenLists(1);
  if (arrow != 0)
    {
      glNewList(arrow, GL_COMPILE);
      glLineWidth(1.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,1.0);
      glVertex3f(0.0,0.0,1.0);
      glVertex3f(0.5,0.0,0.66);
      glVertex3f(0.0,0.0,1.0);
      glVertex3f(-0.5,0.0,0.66);
      glEnd();
      glEndList();
    }
  
  anglevec( &angle, nplane, zn );
  crossprod( nplane, zn, vecrot );
  degrees = RAD2DEG( angle );

  /* draw penpendicular to normal vector of the plane */
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();
  glTranslatef(x,y,z);
  glPushMatrix();
  glRotatef(degrees, vecrot[0], vecrot[1], vecrot[2]);
  glCallList(arrow);
  glPopMatrix();
  glPopMatrix();
}

static void draw_polygon(float (*inter_points)[3], int ninter, float *nplane)
{
  int i;
  t_point *dp;
  t_point **dpoints = NULL;
  
  /* there is nothing to draw */
  if (ninter <= 0)
    return; 
  if (dpoints == NULL) 
  	dpoints = (t_point **)ALLOC(ninter * sizeof(t_point *));

  /* find convex hull, so we can draw polygon correctly. */
  for (i = 0; i < ninter; i++)
    {
      NEW(dp);
      dp->vnum   = i;
      dp->v[0]   = inter_points[i][0];
      dp->v[1]   = inter_points[i][1];
      dp->v[2]   = inter_points[i][2];
      dp->delete = FALSE;

      /* save all points */
      dpoints[i] = dp;
    }

  onhull(ninter, dpoints, nplane);
 

  /* draw polygon */
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDisable(GL_CULL_FACE);
  //glColor3f(1.,0.,0.);
  glPointSize(2.0);
  glEnable(GL_POLYGON_OFFSET_FILL);
  //glColor4f(0.0,0.9,0.0,0.25); 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPolygonOffset(OFFSET_FACTOR,OFFSET_BIAS);

  /* draw transparent cuting plane */
  glBegin(GL_POLYGON);   
  for (i = 0; i < ninter; i++)
    {
      int vnum;
      vnum = dpoints[i]->vnum;
      glVertex4f(inter_points[vnum][0], inter_points[vnum][1],
		 inter_points[vnum][2], 1.0);
      
    }
  glEnd();

  /* draw arrows at corners */
  /*for (i = 0; i < ninter; i++)
    {
      int vnum;
      vnum = dpoints[i]->vnum;
      drawarrow(inter_points[vnum][0], inter_points[vnum][1],
		inter_points[vnum][2], nplane);
		}*/

  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_LIGHTING);
  
  glColor3f(1.0, 0.0, 0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* draw line around cuting plane */
  glBegin(GL_POLYGON);
  for (i = 0; i < ninter; i++)
    {
      int v = dpoints[i]->vnum;
      glVertex4f(inter_points[v][0], inter_points[v][1],
		 inter_points[v][2], 1.0);
    }
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glDepthMask(GL_TRUE);

  FREE(dpoints);
}

void drawp(struct dlistelem *p, void *arg)
{
    char *fmt;
    t_point *navp;
    int vnum;

    fmt = (char *) arg;
    navp   = (t_point *) p->data;
  
    printf(fmt, navp->vnum);
 
}

static void draw_box(float vol[][3], GLenum type)
{
  static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0} 
    };
  static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = vol[0][0];
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = vol[0][1];
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = vol[1][0];
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = vol[1][1];
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = vol[2][0];
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = vol[2][1];

  for (i = 0; i < 6; i++)
    {
      glBegin(type);
      glVertex3fv(&v[faces[i][0]][0]);
      glVertex3fv(&v[faces[i][1]][0]);
      glVertex3fv(&v[faces[i][2]][0]);
      glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
    }
}

static void draw_arrow()
{
  GLuint arrowList;
  GLUquadricObj *quadratic, *quadratic1;
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  arrowList = glGenLists(1);
  if (arrowList != 0)
    {
      glNewList(arrowList, GL_COMPILE);
      glPushMatrix();
      glRotatef(-90.0,1,0,0); 
      quadratic = gluNewQuadric();
      quadratic1 = gluNewQuadric();
      gluQuadricOrientation(quadratic, GLU_OUTSIDE);
      gluQuadricOrientation(quadratic1, GLU_OUTSIDE);
      gluQuadricDrawStyle(quadratic, GLU_FILL);
      gluQuadricDrawStyle(quadratic1, GLU_FILL);
      gluQuadricNormals(quadratic, GLU_SMOOTH);
      /* make cylinder */
      gluCylinder(quadratic,0.5f,0.5f,6.0f,32,32);
      glTranslatef(0.0,0.0,6.0);
      /* make cone */
      gluCylinder(quadratic1,1.0f,0.0f,3.0f,32,32);
      glPopMatrix();
      glEndList();
    }
  glCallList(arrowList);
  glDisable(GL_BLEND);
}

void dnorm(double v[4])
{
  double d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
  v[0] /= d;
  v[1] /= d;
  v[2] /= d;
}
