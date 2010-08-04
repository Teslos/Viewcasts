/*******************************************
 This routine makes shading and lighting 
 for color3d.

 Author: Toni Ivas & Helmut Vor
 =======
 
 Warnings:
 ========
*******************************************/
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "shade.h"
#include "memory.h"
#include "gl_local.h"

#define COPYV(a,v) ((a)[0] = (v).x, \
                    (a)[1] = (v).y, \
                    (a)[2] = (v).z, \
                    (a)[3] = (v).w)  

static void initmaterials();
static void initlights();
void setlights(lights *li);
void setmaterial(material *mat);
void mattof(FILE *f, material *mat);

/*******************************************
 Purpose:
 ========
         Setup default lighting parameters
         for lighting results..
 Parameters:
 ==========
          -
 Returns:
 ========
         function returns 0 if succesfull.
*******************************************/
int lighting ()
{
  int i;
  /* Setup lighting parameters */

  static GLfloat light0_pos[] = { 0.0, 0.0, 1000.0, 0.0 };
  static GLfloat light1_pos[] = { 0.0, 0.0, -1000.0, 0.0 };
  static GLfloat light2_pos[] = { 10.0, 10.0, 10.0, 0.0 };
  static GLfloat light2_ambient[] = { 0.15, 0.15, 0.15, 1.0 };
  static GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat light_ambient[] = { 0.15, 0.15, 0.15, 1.0 };
  static GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
  static GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
  static GLfloat mat_amb_diff[] = { 0.5, 0.5, 0.5, 1.0 };
  static GLfloat mat_ambient[] = { 0.3, 0.3, 0.3, 1.0};
  static GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
  static GLint   mat_shininess[] = {16};
  static GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };
  glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
  /*glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );*/
  glLightfv( GL_LIGHT0, GL_POSITION, light0_pos );

  glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient );
  glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse );
  /*glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular );*/
  glLightfv( GL_LIGHT1, GL_POSITION, light1_pos );

/*   glLightfv( GL_LIGHT2, GL_AMBIENT, light2_ambient ); */
/*   glLightfv( GL_LIGHT2, GL_DIFFUSE, light2_diffuse ); */
/*   /\*glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular );*\/ */
/*   glLightfv( GL_LIGHT2, GL_POSITION, light2_pos ); */
  
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, mat_ambient );
  glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, 0 );
  glLightModeli ( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );
  
  
 /*  glMaterialiv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess ); */ 
 /* glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );*/
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
     mat_amb_diff);

  glEnable(GL_LIGHTING);
 
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_COLOR_MATERIAL);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_RESCALE_NORMAL);
  
  return 0;
}

/* calculate normalized vector */
void normalize(float v[3])
{
  GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  if ( d == 0 )
    {
#ifdef DEBUG
      fprintf(stderr, "Zero lenght vector\n");
#endif
      return;
    }

  v[0] /= d;  v[1] /= d; v[2] /= d;
}

void grafik_zoom(void);

/*
 * Make cross vector product of 
 * vectors v1 and v2. Returns
 * normal vector out.
 */
/*
void crossprod(float v1[3], float v2[3], float out[3])
{
  out[0] = v1[1]*v2[2] - v1[2]*v2[1];
  out[1] = v1[2]*v2[0] - v1[0]*v2[2];
  out[2] = v1[0]*v2[1] - v1[1]*v2[0];
  normalize(out);
}
*/

void initlights()
{
  GLfloat ambient[] = {0.1,0.1,0.1,1.0};
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient );
  glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, 1 );
  glLightModeli ( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  makelight(0,0.0,-2.0,0.0);
  makelight(1,-20.0,20.0,10.0);
  makelight(2, 20.0,20.0,10.0); 
}

void setdiffuse(float r, float g, float b)
{
  GLfloat diffuse[4];
  diffuse[0] = r;
  diffuse[1] = g;
  diffuse[2] = g;
  diffuse[3] = 1.0;
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,
		diffuse );
}

void setspecular(float r, float g, float b)
{
  GLfloat specular[4];
  specular[0] = r;
  specular[1] = g;
  specular[2] = g;
  specular[3] = 1.0;
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,
		specular );
}

void setshininess(float s)
{
  GLfloat shininess;
  shininess = s;
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS,
		shininess );
}

void setambient( float r, float g, float b)
{
  GLfloat ambient[4];
  ambient[0] = r;
  ambient[1] = g;
  ambient[2] = b;
  ambient[3] = 1.0;
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient );
}

void initmaterials()
{
  GLfloat emission[]  = {0.0,0.0,0.0};
  GLfloat ambient[]   = {0.0,0.0,0.0,1.0};
  GLfloat diffuse[]   = {0.7,0.7,0.7,1.0};
  GLfloat specular[]  = {0.9,0.9,0.9,1.0};
  GLfloat shininess[] = {40.0};

  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,
		ambient );
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,
		diffuse );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,
		specular );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS,
		shininess );
  glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,
		emission );
}

void shadeinit()
{
  initmaterials();
  initlights();
  shadeoff();
}

void shadeon()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_COLOR_MATERIAL);

  if ( gl_config.renderer & (GL_RENDERER_O2 | GL_RENDERER_IMPACT) )
    cglNormalize( GL_NORMALIZE );
  else
    cglNormalize( GL_RESCALE_NORMAL );
}

void shadeoff()
{
  glDisable(GL_LIGHTING);
}

static void makelight(int i, float x, float y, float z)
{
  GLfloat ambient[] = {0.1,0.1,0.1,1.0};
  GLfloat light[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat position[4]; 
  position[0]  = x;
  position[1]  = y;
  position[2]  = z;
  position[3]  = 0.0; /* infinite light */
  
  glLightfv(GL_LIGHT0+i, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0+i, GL_POSITION, position);
  glLightfv(GL_LIGHT0+i, GL_DIFFUSE, light);
}

/*****************************************************
 Purpose:
 ========
 	disable the lights and lighting model.
	
 Parameters:
 ===========
        -none
 Returns:
 =========
 	-none
*******************************************************/
void disableLighting()
{
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
}
	
/******************************************************
 This are routines taken from Paul's Haberli libgutil
 library for reading the material and lights from file.
*******************************************************/ 
material *
newmat()
{
  return (material *) ALLOC(sizeof(material));
}

void
freemat(material *mat)
{
  FREE(mat);
}

material *
matfromfile(char *name)
{
  char fullname[256];
  material *mat;
  FILE *f;
  
  f = fopen(fullname, "r");
  if (!f) 
    {
      fprintf(stderr, "matfromfile: can't open file %s\n", fullname);
      return NULL;
    }
  mat = matfromf(f);
  fclose(f);
  return mat;
}

material *
matfromf(FILE *f)
{
  material *mat;
  char token[256];
  
  float r,g,b;
  mat = newmat();
  if (fscanf(f, "%s %f %f %f", token, &r, &g, &b) != 4)
    goto materror;
  if (strcmp(token,"emission") != 0)
    goto materror;
  mat->emission.x = r;
  mat->emission.y = g;
  mat->emission.z = b;
  
  if (fscanf(f, "%s %f %f %f", token, &r,&g,&b) != 4)
    goto materror;
  if (strcmp(token,"ambient") != 0)
    goto materror;
  mat->ambient.x = r;
  mat->ambient.y = g;
  mat->ambient.z = b;
  
  if (fscanf(f, "%s %f %f %f", token, &r, &g, &b) != 4)
    goto materror;
  if (strcmp(token, "diffuse") != 0)
    goto materror;
  mat->diffuse.x = r;
  mat->diffuse.y = g;
  mat->diffuse.z = b;
  
  if (fscanf(f, "%s %f %f %f", token, &r, &g, &b) != 4)
    goto materror;
  if (strcmp(token,"specular") != 0)
    goto materror;
  mat->specular.x = r;
  mat->specular.y = g;
  mat->specular.z = b;
  
  if (fscanf(f,"%s %f", token, &r) != 2)
    goto materror;
  if (strcmp(token, "shininess") != 0)
    goto materror;
  mat->shininess = r;
  
  if (fscanf(f,"%s %f",token, &r) != 2)
    goto materror;
  if (strcmp(token, "alpha") != 0)
    goto materror;
  mat->alpha = r;
  return mat;

 materror:
  freemat(mat);
  return 0;
}
  
void 
mattofile(char *name, material *mat)
{
  FILE *f;
  f = fopen(name, "w");
  if (!f)
    {
      fprintf(stderr, "mattofile: can't open file %s\n", name);
      return;
    }
  mattof(f,mat);
  fclose(f);
}


void 
mattof(FILE *f, material *mat)
{
  fprintf(f, "emission %f %f %f\n",
	  mat->emission.x, mat->emission.y, mat->emission.z);
  fprintf(f, "ambient %f %f %f\n",
	  mat->ambient.x, mat->ambient.y, mat->ambient.z);
  fprintf(f, "diffuse %f %f %f\n",
	  mat->diffuse.x, mat->diffuse.y, mat->diffuse.z);
  fprintf(f, "specular %f %f %f\n",
	  mat->specular.x, mat->specular.y, mat->specular.z);
  fprintf(f, "shininess %f\n", mat->shininess);
  fprintf(f, "alpha %f\n", mat->alpha);
}

void matprint(material *mat)
{
  mattofile("/dev/tty", mat);
}

lamp *
newlamp()
{
  lamp *la;
  la = (lamp*) ALLOC(sizeof(lamp));
  la->next = NULL;
  return la;
}

void 
freelamp(lamp *l)
{
  FREE(l);
}

lights *
newlights()
{
  lights *l;
  
  l = (lights *) ALLOC(sizeof(lights));
  l->lamps = NULL;
  return l;
}

void 
freelights(lights *li)
{
  lamp *l, *nl;
  l = li->lamps;
  while(l) 
    {
      nl = l->next;
      freelamp(l);
      l = nl;
    }
  FREE(li);
}

lights *
lightsfromfile(char *name)
{
  char fullname[256];
  FILE *f;
  lights *li;
  
  /* not finished hack */
  findname(name, fullname, "GFXPATH");
  f = fopen(fullname, "r");
  if (!f)
    {
      fprintf(stderr, "lightsfromfile: can't open file %s\n", fullname);
      return NULL;
    }
  li = lightsfromf(f);
  fclose(f);
  return li;
}

lights *
lightsfromf(FILE *f)
{
  lights *li;
  lamp *la, *lastla;
  char token[256];
  float r,g,b,w;
  int flag;
  
  li = newlights();
  la = 0;
  if (fscanf(f, "%s %f %f %f", token, &r, &g, &b) != 4)
    goto lightserror;
  if (strcmp(token, "ambient") != 0)
    goto lightserror;
  li->ambient.x = r;
  li->ambient.y = g;
  li->ambient.z = b;
  
  if (fscanf(f, "%s %d", token, &flag) != 2)
    goto lightserror;
  if (strcmp(token,"localviewer") != 0)
    goto lightserror;
  li->localviewer = flag;
  
  if (fscanf(f,"%s %f", token, &r) != 2)
    goto lightserror;
  if (strcmp(token, "attenconst") != 0)
    goto lightserror;
  li->attenconst = r;
  
  if (fscanf(f, "%s %f", token, &r) != 2)
    goto lightserror;
  if (strcmp(token, "attenmult") != 0)
    goto lightserror;
  li->attenmult = r;
  
  lastla = 0;
  
  while(1) 
    {
      if (fscanf(f,"%s %f %f %f", token, &r, &g, &b) != 4)
	break;
      if (strcmp(token,"ambient") != 0)
	goto lightserror;
      la = newlamp();
      la->ambient.x = r;
      la->ambient.y = g;
      la->ambient.z = b;
      
      if (fscanf(f, "%s %f %f %f", token, &r,&g,&b) != 4)
	break;
      if (strcmp(token, "color") != 0)
	goto lightserror;
      la->color.x = r;
      la->color.y = g;
      la->color.z = b;
      
      if (fscanf(f, "%s %f %f %f %f", token, &r, &g, &b, &w) != 5)
	if (strcmp(token,"position") != 0)
	  goto lightserror;
      la->position.x = r;
      la->position.y = g;
      la->position.z = b;
      la->position.w = w;
      if (!lastla)
	{
	  li->lamps = la;
	  lastla = la;
	}
      else
	{
	  lastla->next = la;
	  lastla = la;
	}
    }
  return li;

 lightserror:
  if (la)
    freelamp(la);
  freelights(li);
  return 0;
}
      
void
lightstof(FILE *f, lights *li)
{
  lamp *la;
  fprintf(f, "ambient %g %g %g\n",
	  li->ambient.x, li->ambient.y, li->ambient.z);
  fprintf(f,"localviewer %d\n", li->localviewer);
  fprintf(f,"attenconst %g\n", li->attenconst);
  fprintf(f,"attenmult %g\n", li->attenmult);
  la = li->lamps;
  while(la) 
    {
      fprintf(f, "ambient %f %f %f\n",
	      la->ambient.x, la->ambient.y, la->ambient.z);
      fprintf(f, "color %f %f %f\n",
	      la->color.x, la->color.y, la->color.z);
      fprintf(f, "position %f %f %f %f\n",
	      la->position.x, la->position.y, la->position.z, la->position.w);
      la = la->next;
    }

}

void
materialname(char *name)
{
  material *mat;

  mat = matfromfile(name);
  if (!mat)
    return;
  setmaterial(mat);
  freemat(mat);
}

void
setmaterial(material *mat)
{
  GLfloat emission[4], ambient[4], diffuse[4];
 
  if (!mat)
    {
      fprintf(stderr, "setmaterial: null material\n");
      return;
    }
  /* emission, ambient, diffuse, shininess components */
  COPYV(emission, mat->emission);
  COPYV(ambient, mat->ambient);
  COPYV(diffuse, mat->diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,mat->shininess);  
}
  
void
lightsname(char *name)
{
  lights *li;

  li = lightsfromfile(name);
  if (!li)
    return;
  setlights(li);
  freelights(li);
}

void
setlights(lights *li)
{
  int i;
  lamp *la;
  GLfloat ambient[4];
  GLfloat position[4];
  

  if (!li)
    {
      fprintf(stderr, "setlight: null light\n");
      return;
    }

  /* ambient, localviewer, attenuation */
  COPYV(ambient, li->ambient);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, li->localviewer);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, li->attenconst);
  la = li->lamps;

  for (i = 1; i <= 8; i++) 
    {
      if(!la)
	{
	  glEnable(GL_LIGHT0+i-1);
	}
      else
	{
	  COPYV(ambient, la->ambient); 
	  glLightfv(GL_LIGHT0+i, GL_AMBIENT, ambient);
	  COPYV(position, la->position);
	  glLightfv(GL_LIGHT0+i, GL_POSITION, position);
	  glEnable(GL_LIGHT0+i-1);
	  la = la->next;
	}
    }
	    
}











