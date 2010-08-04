/************************************************************
 This routine is used for capping the solid model.
 It is based on simple example by Kurt Akeley.

 This code works correctly only with proper polygonal
 surfaces. Such surfaces separeate space into inside and
 outside volumes. They are built of polygons whose 
 adjacent edges share vertexes and do not overlap.

************************************************************/

#include <stdio.h>
#include <math.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


#include "grafik.h"
#include "motif.h"

enum { LEFT_MOUSE = 1,
       MIDDLE_MOUSE = 2,
       RIGHT_MOOUSE = 3
};

#define INITDEPTH 0.0
#define FOV 45

#define MAXPLANE     6
#define MAXTRANSFORM (MAXPLANE+2)
#define EDIT_VIEW    (MAXTRANSFORM-1)
#define EDIT_OBJECT  (MAXTRANSFORM-2)
#define MAXMODEL     4

#define STEN_MASK ((1 << C3D_win.stensize) - 1)

/* functions used in this file */

void drawmodel(void (*func)(), int mat);
void drawscene(void);
void fillsquare(void);
void drawsquare(void);
void bigcube(void);
void drawcube(long outside, float s);
void initgraphics(void);
void reshapewindow(void);


long xdim, ydim;
float aspect;
int stensize;
GLfloat norm_cut[3];

float yr[] = {0,0,0,0,0,0,0,0};

float boink_xr;
float boink_yr;
float boink_depth;

long penable[MAXPLANE] = {0,0,0,0,0,0};
long pface[MAXPLANE] = {0,0,0,0,0,0};

float boink_rot_scale;
float boink_depth_scale;
short boink_screen_x;
short boink_screen_y;

int model;
int docap;
int dointersection;
int motion;
int t_select;

int tselect;
int mselect;
int twoside;
int ctrlkey = GL_FALSE;
int leftshiftkey = GL_FALSE;
int rightshiftkey = GL_FALSE;
int leftmouse = GL_FALSE;
int middlemouse = GL_FALSE;
int orangecaps;

Display* dpy;
Window win;
GLXContext cx;
GLfloat mat_view[16];
GLfloat mat1[16];


void farclip(long b) {
  static int done = 1;
  if(done)
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      
      if (g_projektion.perspektive)
	glOrtho(g_projektion.left,g_projektion.right,g_projektion.bottom,
		g_projektion.top, g_projektion.near, g_projektion.far);
      else
	glFrustum(g_projektion.left,g_projektion.right,g_projektion.bottom,
		  g_projektion.top, g_projektion.near, g_projektion.far);
      
      /*
      glTranslatef(g_transform.move[0], g_transform.move[1],
      g_transform.move[2]);*/
          
      glMatrixMode(GL_MODELVIEW);
      //glLoadIdentity();
      //C3D_track_model_matrix();
      /*glRotatef(g_transform.rotx, 1.0, 0.0, 0.0);
      glRotatef(g_transform.roty, 0.0, 1.0, 0.0);
      glRotatef(g_transform.rotz, 0.0, 0.0, 1.0);
      glScalef(g_transform.scale, g_transform.scale, g_transform.scale);*/
      //glMultMatrixf( Save_mo_matrix );
      done = 1;
      glGetFloatv(GL_MODELVIEW_MATRIX, mat_view);
      glMatrixMode(GL_MODELVIEW);
    }
}

void reshapewindow(void) {
    glViewport(0, 0, C3D_win.gl_width, C3D_win.gl_height);
    /*
    aspect = (float)C3D_win.gl_width / (float)C3D_win.gl_height;
    farclip(GL_TRUE);
    */
    /* set perspective projection */
    /* ADDED FOR TIME BEING */
    /*
    glStencilMask(STEN_MASK);
    glClear(GL_STENCIL_BUFFER_BIT);*/
    
}

/***********************************************************
 This function handle mouse invents which comes from X
 server. Then it calls appropriate functions.
************************************************************/
void mouse (Widget w, XEvent *event, String* params,
                Cardinal* paramsAnzAdr)
{  
  /* local variables */
  register int i;
  int ch;                        /* character             */
  int screen_x, screen_y;        /* screen coordinates    */
  char s[100];                   /* string for key events */
  /*XEvent *event;*/                  /* x server event        */
  XKeyEvent* key_event;          /* key event             */
  XButtonEvent* button_event;    /* button event          */
  XMotionEvent* motion_event;    /* motion event          */
  XConfigureEvent* config_event; /* configure event       */
  XExposeEvent* expose_event;    /* expose event          */
   
 
  /* initialize parameters */
  initparams();
  
  /* prepare event loop */
  button_event = (XButtonEvent *) (event);
  key_event = (XKeyEvent *) (event);
  motion_event = (XMotionEvent *) (event);
  expose_event = (XExposeEvent *) (event);
  config_event = (XConfigureEvent *) (event);
  /*XSelectInput(C3D_win.dpy, XtWindow(C3D_win.wid), KeyPressMask |
	                 ButtonPressMask |
	                 ButtonReleaseMask |
	                 Button1MotionMask |
	                 Button2MotionMask |
                         ExposureMask      |
	                 StructureNotifyMask);*/ 
  
  /* loop thru handling events */
  /*while(1) {*/ 
    /*XNextEvent(C3D_win.dpy, &event);*/
    initgraphics();
    switch (event->type) {
    /* key events */
    case KeyPress:
      {
	ch = XLookupString(key_event, s, 100, NULL, NULL);
	if (ch) 
	  {
	    switch((int)(s[0])) 
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
		  printf("xr=%g yr=%g depth=%g\n",
			 xr[i], yr[i], depth[i]);
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
	    boink_yr = yr[tselect];
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
	    yr[tselect] = boink_yr + boink_rot_scale *
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
	    yr[tselect] = boink_yr + boink_rot_scale *
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
	    yr[tselect] += (float)screen_x / 100.0;
	    xr[tselect] -= (float)screen_y / 100.0;
	    limitrot();
	    drawscene();
	  }
      }
    else 
      {
	if(!XtAppPending(kontext))
	  drawscene();
      }
  }
/*}*/
	    
int tracking()
{
  return leftmouse || middlemouse;
}

int shifted()
{
  return leftshiftkey || rightshiftkey;
}

void limitrot()
{
  while (xr[tselect] > 360) xr[tselect] -= 360;
  while (xr[tselect] < 0)   xr[tselect] += 360;
  while (yr[tselect] > 360) yr[tselect] -= 360;
  while (yr[tselect] < 0)   yr[tselect] += 360;
  if (motion && (mselect != tselect))
    {
      while (xr[tselect] > 360) xr[tselect] -= 360;
      while (xr[tselect] < 0)   xr[tselect] += 360;
      while (yr[tselect] > 360) yr[tselect] -= 360;
      while (yr[tselect] < 0)   yr[tselect] += 360;
    }
}

GLdouble cp[4] = {0.0, 1.0, 0.0, 0.0};
GLdouble cpp[4];
/************************************************************************
 This function draws the complete scene.
************************************************************************/
void drawscene(void)
{
  int i,j;
  float x[3], xinv[3], y[3], a[3], b[3], c[3], d[3];
  float rv[4], plv[4];  /* result vector, plane vector */
  float sign;           /* factor which deciside plane orientation */ 
  double v_view[]= {0.0,0.0,-1.0,1.0};      /* view vector */
  float v_view1[]= {0.0,0.0,-1.0,1.0};
  float size = 10;
  float ccp[4];
  float cut[16];
  float temp[4];

  const GLubyte *errString;
  void (*func)();
  GLfloat mat[16];
  GLfloat mat_tran[16];

  /* print errors */
  while (i = glGetError()) {
    errString = gluErrorString(i);
    fprintf(stderr, "ERROR: %s\n", errString);
  }

  /* clear the screen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  farclip(GL_TRUE);
  /* put the clipplane in camera view */
  /*glPushMatrix();*/
  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
 

  rv[0]=rv[3]=1; rv[1]=rv[2]=0.0;
  /*reshapewindow();*/
  /* modify location of current clipplane */
  for (i = 0; i < MAXPLANE; i++)
    {
      if ((tselect == i) || (tselect == EDIT_VIEW) ||
	 (motion && (mselect == i)))
	{
	  /* set the view vector */
	  /*v_view[] = {0,0,-1,0};*/

	  /* get matrix of transformation */
	  /*glGetFloatv(GL_MODELVIEW_MATRIX, mat1);*/
	  
	  /* mult. plane-vector by trans. matrix to get
	   * current plane-vector.
	   */
	  /*multMxV(mat1, rv);
	    /*printf ("Curr. plane vec: [ %f  %f  %f  %f ] \n", 
	    rv[0],rv[1],rv[2],rv[3]);*/
	  
	  /* mult. current plane vector and view vector */
	  /*sign = v_view[0]*rv[0]+ v_view[1]*rv[1]+v_view[2]*rv[2]+
	    v_view[3]*rv[3];*/

	  /* if sign is sign < 0 we can see plane */
	  /*pface[i] = (sign < 0) ? 1:0; */

	  /* determine which way clipping plane faces */
	  /*glPushMatrix();
	  setinvmat(i);
	  setinvmatmodel();*/
	  /*
	  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	  
	  transpose(mat, mat_tran);
	  multMxV(mat_tran, v_view);

	  glBegin(GL_LINES);
	  glColor3f(1.0,1.0,0.0);
	  glVertex3f(v_view[0], v_view[1], v_view[2]);
	  glVertex3f(-v_view[0], -v_view[1], -v_view[2]);
	  glEnd();

	  transpose(mat1, mat_tran);
	  multMxV(mat_tran, v_view1);
	  glBegin(GL_LINES);
	  glColor3f(0.0,1.0,0.0);
	  glVertex3f(v_view1[0], v_view1[1], v_view1[2]);
	  glVertex3f(-v_view1[0], -v_view1[1], -v_view1[2]);
	  glEnd();*/
	  /* mult. plane-vector by trans. matrix to get
	   * current plane-vector.
	   */

	  /*
	  multMxV(mat, rv);

	  printf ("Curr. plane vec: [ %f  %f  %f  %f ] \n", 
	  rv[0],rv[1],rv[2],rv[3]);*/
	  
	  /* mult. current plane vector and view vector */
	  /*sign = v_view[0]*rv[0]+ v_view[1]*rv[1]+v_view[2]*rv[2];*/

	  /* if sign is sign < 0 we can see plane */
	  /*pface[i] = (sign < 0) ? 1:0;*/ 
	  
	  /*pface[i] = (mat[14]<0) ? 1:0;*/
	
	  /* get eye vector  NOT FINISHED */
	  

	  /*printf( "You do %s  a cut\n", ((pface[i] != 0)
	    ? "see" : "not see"));*/
          /*		  pface[i] = 1;*/
	  /*glPopMatrix();*/

	  /* relocate the clipping plane */
	  glPushMatrix();
	 
	  /**glRotatef(-g_transform.rotz, 0.0, 0.0, 1.0);
	     glRotatef(-g_transform.roty, 0.0, 1.0, 0.0);*/
	  
	  glRotatef(-90.0, 1, 0, 0);
	  setmatrix(i);
	  /* get the current matrix */
	  glGetFloatv(GL_MODELVIEW_MATRIX, cut);

	  /* transponse global transformation */
	  transpose(mat_view, mat_tran);
	  
	  /* get new view vector */
	  multMxV(mat_tran, v_view, temp);
	  
	  /* assign new value to global view vector ???*/ 
	  /*v_view[0] = temp[0]; v_view[1] = temp[1]; v_view[2] = temp[2];
	    v_view[2] = temp[2];*/

	  /*ccp[0] = cp[0]; ccp[1] = cp[1]; ccp[2] = cp[2]; ccp[3] = cp[3];*/
	  /* get trans. of plane vector */
	  multMxV(cut, cp, ccp);

	  /* draw plane vector */
	  glBegin(GL_LINES);
	  glColor3f(0.0,1.0,0.0);
	  glVertex3f(cp[0],cp[1],cp[2]);
	  glVertex3f(-cp[0]*10,-cp[1]*10,-cp[2]*10);
	  glEnd();
	  printf("View vector: x: %f y:%f z:%f\n",v_view[0], v_view[1], v_view[2]);
	  /* decision, if we look at plane or not */
	  sign = v_view[0]*ccp[0]+ v_view[1]*ccp[1]+v_view[2]*ccp[2];
	  printf("sign:%f\n", sign);
	  pface[i] = (sign >  0)? 1:0;
	  
	  glClipPlane(GL_CLIP_PLANE0 + i, cp);
	  glPopMatrix();
	}
    }
  
  glColor3f(1.0,0.0,0.0);
  /*glPushMatrix();
  glRotatef(42.0, 1.0, 0.0, 0.0);
  glRotatef(7.0, 0.0, 1.0, 0.0);
  glRotatef(42.0, 0.0, 0.0, 1.0);*/
  /*  bigcube();*/

  func = zeichne_geometrie;
  /*func = bigcube;*/
  drawmodel((*func), 0);

  /* glPopMatrix();*/

  /* draw model */
  /*func = zeichne_result;*/
    
  /*glRotatef(56.0,0,1,0);*/
  /*func = bigcube;*/

  /*func = zeichne_geometrie;*/
  /*for (i = 1; i <= g_geodat.nstoff; i++)
    drawmodel((*func), i);*/
  

  /* disable all clipping planes */
  for (i = 0; i < MAXPLANE; i++)
    glDisable(GL_CLIP_PLANE0 + i);
  
  /* draw intersections and clear stencil buffer */
  if (dointersection)
    {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_LESS, 2, STEN_MASK);
      glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
      glDepthFunc(GL_ALWAYS);
      glColor4f(1,1,0,1);
      winrect();
      glDepthFunc(GL_GEQUAL);
      glDisable(GL_STENCIL_TEST);
    }
  else 
    glClear(GL_STENCIL_BUFFER_BIT);
 
  /* draw current clipping plane */
  if ((tselect < MAXPLANE) && penable[tselect])
    {
      if (pface[tselect])
	glColor4f(1,1,1,1);
      else
	glColor4f(0,1,0,1);
      glPushMatrix();
      glRotatef(-90.0, 1, 0, 0);
      setmatrix(tselect);
      drawsquare();
      glPopMatrix();
    }
 
  if (C3D_win.doublebuffer)
    glXSwapBuffers(XtDisplay(gl_widget), XtWindow(gl_widget));
  else
    glFlush();

}


/************************************************************
 This function draws the model with capping the open
 surface.
************************************************************/
void drawmodel(void (*func)(), int mat)
{
  register int i;

  /* setup matrix for drawing the object */
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  setmatrix(EDIT_OBJECT);
  
  /* enable selected clipping planes */
  for (i = 0; i < MAXPLANE; i++)
    {
      if (penable[i])
	glEnable(GL_CLIP_PLANE0 + i);
      else
	glDisable(GL_CLIP_PLANE0 + i);
    }

  if (docap)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  glEnable(GL_LIGHTING);
	
  (*func)();
	
  glDisable(GL_LIGHTING);
	
  if(docap)
    glDisable(GL_CULL_FACE);
  
  /* done if capping is not enabled */
  if(!docap)
    {
      glPopMatrix();
      return;
    }
  
  /* enable only front clipping planes - draw into stencil bitplanes */
  for (i = 0; i < MAXPLANE; i++)
    {
      if (penable[i] && pface[i])
	glEnable(GL_CLIP_PLANE0+i);
      else
	glDisable(GL_CLIP_PLANE0+i);
    }
    
  farclip(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glColorMask(0,0,0,0);
  glStencilMask(0x01);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0x01);
  glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

  (*func)();
    
  farclip(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glColorMask(1,1,1,1);
  glStencilMask(STEN_MASK);
  glPopMatrix();
 
  /* draw caps */
  for (i = 0; i < MAXPLANE; i++)
    {
      if (penable[i])
	glEnable(GL_CLIP_PLANE0 + i);
      else
	glDisable(GL_CLIP_PLANE0 + i);
    }
  glDepthFunc(GL_ALWAYS);
  glStencilFunc(GL_EQUAL, 1, 0x1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
  
  for (i = 0; i < MAXPLANE; i++)
    {
      if (penable[i] && pface[i])
	{
	  glDisable(GL_CLIP_PLANE0 + i);
	  glPushMatrix();
	  setmatrix(i);
	  if(orangecaps)
	    {
	      glColor4f(1,0.5,0,1);
	      fillsquare();
	    }
	  else
	    {
	      glEnable(GL_LIGHTING);
	      glColor4f(0,0.0,1,1);
	      fillsquare();
	      glDisable(GL_LIGHTING);
	    }
	  glPopMatrix();
	  glEnable(GL_CLIP_PLANE0 + i);
	}
    }

  glDepthFunc(GL_GEQUAL);
  glDisable(GL_STENCIL_TEST);
  
}


/***************************************************************
 initparams - this function initialize the parameters, for
 capping functions.
****************************************************************/
void initparams(void)
{
  int i;
  /* put all cutring planes off */
  for(i = 0; i < MAXPLANE; i++)
    {
      penable[i] = GL_FALSE;
    }
  for(i = 0; i < MAXTRANSFORM; i++)
    {
      xr[i] = 0.0;
      yr[i] = 0.0;
      depth[i] = 0.0;
    }
  penable[0] = GL_TRUE;
  depth[EDIT_VIEW] = INITDEPTH;
  tselect = 0;
  mselect = EDIT_VIEW;
  motion  = GL_FALSE;
  docap   = GL_TRUE;
  dointersection = GL_TRUE;
  twoside = GL_TRUE;
  model = 0;
}


void drawsquare(void) {
  glEnable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glRecti(-10, -10, 10, 10);
  glDisable(GL_BLEND);
}


void fillsquare(void) {
  /*glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, C3D_win.gl_width, 0, 
		  C3D_win.gl_height);
  glMatrixMode(GL_MODELVIEW);*/
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glNormal3f(0, 0, 1);
  glRecti(-100, -100, 100, 100);
 
}

void transpose(float *mat, float *mat_tran)
{
  mat_tran[0] = mat[0]; mat_tran[1] = mat[4]; mat_tran[2] = mat[8];
  mat_tran[3] = mat[12]; mat_tran[4] = mat[1]; mat_tran[5] = mat[5];
  mat_tran[6] = mat[9]; mat_tran[7] = mat[13]; mat_tran[8] = mat[2];
  mat_tran[9] = mat[6]; mat_tran[10] = mat[10]; mat_tran[11] = mat[14];
  mat_tran[12] = mat[3]; mat_tran[13] = mat[7]; mat_tran[14] = mat[11];
  mat_tran[15] = mat[15];
}






void multMxV ( float *mat, double *plv, float *rv )
{
  /* multiply matrix with vector */
  rv[0] = mat[0]*plv[0]+mat[4]*plv[1]+mat[8]*plv[2]+mat[12]*plv[3];
  rv[1] = mat[1]*plv[0]+mat[5]*plv[1]+mat[9]*plv[2]+mat[13]*plv[3];
  rv[2] = mat[2]*plv[0]+mat[6]*plv[1]+mat[10]*plv[2]+mat[14]*plv[3];
  rv[3] = mat[3]*plv[0]+mat[7]*plv[1]+mat[11]*plv[2]+mat[15]*plv[3];
}


 
void bigcube(void) {
    static GLuint list = 0;
    glScalef(10, 10, 10);
    if (list)
    glCallList(list);
    else {
    list = glGenLists(1);
    glNewList(list, GL_COMPILE_AND_EXECUTE);
    drawcube(1, 0.5);
    drawcube(0, 0.45);
    glEndList();
    }
}

GLfloat cubevertex[8][4] = {
    {-1.0, -1.0, -1.0, 1.0},
    { 1.0, -1.0, -1.0, 1.0},
    {-1.0,  1.0, -1.0, 1.0},
    { 1.0,  1.0, -1.0, 1.0},
    {-1.0, -1.0,  1.0, 1.0},
    { 1.0, -1.0,  1.0, 1.0},
    {-1.0,  1.0,  1.0, 1.0},
    { 1.0,  1.0,  1.0, 1.0},
};

GLfloat cubenormal[6][4] = {
    { 0.0,  0.0, -1.0},
    { 1.0,  0.0,  0.0},
    { 0.0,  0.0,  1.0},
    {-1.0,  0.0,  0.0},
    { 0.0,  1.0,  0.0},
    { 0.0, -1.0,  0.0},
};

void drawcube(long outside, float s) {
    int i;
    GLfloat cv[8][4];

    for (i=0; i<8; i++) {
	cv[i][0] = s * cubevertex[i][0];
	cv[i][1] = s * cubevertex[i][1];
	cv[i][2] = s * cubevertex[i][2];
	cv[i][3] = 1;
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    if (outside) {
	glNormal3fv(cubenormal[0]);
	glVertex3fv(cv[2]);
	glVertex3fv(cv[3]);
	glVertex3fv(cv[1]);
	glVertex3fv(cv[0]);

	glNormal3fv(cubenormal[1]);
	glVertex3fv(cv[3]);
	glVertex3fv(cv[7]);
	glVertex3fv(cv[5]);
	glVertex3fv(cv[1]);

	glNormal3fv(cubenormal[2]);
	glVertex3fv(cv[7]);
	glVertex3fv(cv[6]);
	glVertex3fv(cv[4]);
	glVertex3fv(cv[5]);

	glNormal3fv(cubenormal[3]);
	glVertex3fv(cv[6]);
	glVertex3fv(cv[2]);
	glVertex3fv(cv[0]);
	glVertex3fv(cv[4]);

	glNormal3fv(cubenormal[4]);
	glVertex3fv(cv[6]);
	glVertex3fv(cv[7]);
	glVertex3fv(cv[3]);
	glVertex3fv(cv[2]);

	glNormal3fv(cubenormal[5]);
	glVertex3fv(cv[0]);
	glVertex3fv(cv[1]);
	glVertex3fv(cv[5]);
	glVertex3fv(cv[4]);
    }
    else {
	glNormal3fv(cubenormal[2]);
	glVertex3fv(cv[0]);
	glVertex3fv(cv[1]);
	glVertex3fv(cv[3]);
	glVertex3fv(cv[2]);

	glNormal3fv(cubenormal[3]);
	glVertex3fv(cv[1]);
	glVertex3fv(cv[5]);
	glVertex3fv(cv[7]);
	glVertex3fv(cv[3]);

	glNormal3fv(cubenormal[0]);
	glVertex3fv(cv[5]);
	glVertex3fv(cv[4]);
	glVertex3fv(cv[6]);
	glVertex3fv(cv[7]);

	glNormal3fv(cubenormal[1]);
	glVertex3fv(cv[4]);
	glVertex3fv(cv[0]);
	glVertex3fv(cv[2]);
	glVertex3fv(cv[6]);

	glNormal3fv(cubenormal[5]);
	glVertex3fv(cv[2]);
	glVertex3fv(cv[3]);
	glVertex3fv(cv[7]);
	glVertex3fv(cv[6]);

	glNormal3fv(cubenormal[4]);
	glVertex3fv(cv[4]);
	glVertex3fv(cv[5]);
	glVertex3fv(cv[1]);
	glVertex3fv(cv[0]);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static GLfloat* fvect4(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    static GLfloat f[4];
    f[0] = r;
    f[1] = g;
    f[2] = b;
    f[3] = a;
    return f;
}

void initgraphics(void) {
    static int done = GL_FALSE;
    if (!done) {
	done = GL_TRUE;
	glXMakeCurrent(C3D_win.dpy, XtWindow(C3D_win.wid), C3D_win.cx);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glDepthRange(1, 0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, fvect4(0, 0, 0, 1));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fvect4(1, 1, 1, 1));
	glLightfv(GL_LIGHT0, GL_SPECULAR, fvect4(1, 1, 1, 1));
	glLightfv(GL_LIGHT0, GL_POSITION, fvect4(0, 0, 1, 0));
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fvect4(.3,.3,.3,1));
	if (twoside)
	    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_LIGHT0);

	glClearColor(0, 0, 0, 0);
	glClearDepth(0);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void winrect(void) {
    /* draw a rectangle the size of the window */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRectf(0, 0, 1, 1);
    glPopMatrix();
    farclip(GL_TRUE);
}
