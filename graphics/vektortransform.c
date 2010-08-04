/* ********************************************************************* */
/*  Name des Programms : C 3 D      Rev. 0                               */
/* ********************************************************************* */
/*                                                                       */
/*  Urheber : Helmut Vor                                                 */
/*                                                                       */
/*  erstellt am  :  10.01.1996                                           */
/*  geaendert am :                                                       */
/*  Aenderung    :                                                       */
/*                                                                       */
/*  Beschreibung der Routinen :                                          */
/*  trans_vektor      : erzeugt Translationsvektor                       */
/*  rot_vektor        : erzeugt Rotationsvektor                          */
#include "grafik.h"
/* ********************************************************************* */
void trans_vektor(int *x, int *y, int *z)
/* ********************************************************************* */
{
/* Tarnslationsvektor wird entspr. der Rotation der Geometrie gedreht */  
  int i, j;
  float rotmat[3][3];
  float rotx, roty, rotz;
  float vekt[3];
  GLfloat  Rmat[16];
  GLfloat  Trans_Rmat[16];
/* ********************************************************************* */
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(g_transform.rotx, 1.0, 0.0, 0.0);
  glRotatef(g_transform.roty, 0.0, 1.0, 0.0);
  glRotatef(g_transform.rotz, 0.0, 0.0, 1.0);
  glGetFloatv(GL_MODELVIEW_MATRIX, Rmat);

/* transponiere Matrix --------------------------------------------- */
/*
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      Trans_Rmat[i+j*4] = Rmat[i*4+j]; 
  Trans_Rmat[15] = 1;
*/
  
/* transformiere Vektor(x,y,z) ------------------------------------- */
/*
  vekt[0] = Trans_Rmat[0] * *x + Trans_Rmat[1] * *y + Trans_Rmat[2] * *z;
  vekt[1] = Trans_Rmat[4] * *x + Trans_Rmat[5] * *y + Trans_Rmat[6] * *z;
  vekt[2] = Trans_Rmat[8] * *x + Trans_Rmat[9] * *y + Trans_Rmat[10] * *z;
*/
  vekt[0] = Rmat[0] * *x + Rmat[1] * *y + Rmat[2] * *z;
  vekt[1] = Rmat[4] * *x + Rmat[5] * *y + Rmat[6] * *z;
  vekt[2] = Rmat[8] * *x + Rmat[9] * *y + Rmat[10] * *z;

/* transformierte Koordinaten zurueckgeben ------------------------- */
  *x = (int) vekt[0];
  *y = (int) vekt[1];
  *z = (int) vekt[2];
/* ********************* ende trans_vektor ***************************** */
  return;
}

/* ********************************************************************* */
void rot_vektor(float x, float y, float z)
/* ********************************************************************* */
{
/* Rotationsvektor wird entspr. der Rotation der Geometrie gedreht -- */  
  float rotmat[3][3];
  float rotx, roty, rotz;
/* ********************************************************************* */
/* ********************* ende rot_vektor ******************************* */
  return;
}

