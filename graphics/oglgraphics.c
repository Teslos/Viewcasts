/* This is misc. function for drawing OpenGL graphics */

#include "grafik.h"

static void multmatrix( float in[3][3], float vec[3] );

void darrowfv(GLfloat *bpos, GLfloat *endpos, GLfloat scalef, GLfloat fixsize)
{
  GLUquadricObj *quadratic, *quadratic1;
  GLfloat ld[3];  /* distance between two vectors */
  GLfloat length; /* length of vector */
  float cx,cy,cz,d,cyd,czd,cxcyd,cxczd,scale;
  float rotx[3][3] = {{1.,0.,0,},
		      {0.,1.,0.},
		      {0.,0.,1.}};
  float roty[3][3] = {{1.,0.,0,},
		      {0.,1.,0.},
		      {0.,0.,1.}};
  float rot90[3][3] = {{0.,0., 1.},
		       {0.,1., 0.},
		       {-1.,0.,0.}};
  float rot[3][3], rot1[3][3];

  Vecdis(ld, endpos, bpos);
  Veclen(length, ld);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  shadeon();
  setspecular(0.6,0.6,0.6);
  setshininess(128);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  quadratic = gluNewQuadric();
  quadratic1 = gluNewQuadric();
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluQuadricOrientation(quadratic1, GLU_OUTSIDE);
  gluQuadricDrawStyle(quadratic, GLU_FILL);
  gluQuadricDrawStyle(quadratic1, GLU_FILL);
  gluQuadricNormals(quadratic, GLU_SMOOTH);

  /* make cylinder */
  if (scalef == 0 )
    return;
  if (fixsize != 0)
    scale = fixsize;
  else if (scalef != 0)
    scale = length * scalef;
  else 
    scale = length;

  /* Determine the direction cosines */
  cx = ld[0] / length;
  cy = ld[1] / length;
  cz = ld[2] / length;
  d     =  sqrt(cy * cy + cz * cz);
  cyd   = (d == 0.) ? 0. : cy / d;
  czd   = (d == 0.) ? 1. : cz / d;
  cxcyd = (d == 0.) ? 0. : (cx * cy) / d;
  cxczd = (d == 0.) ? 1. : (cx * cz) / d;
  
  rotx[0][0] = cx;
  rotx[0][2] = d;
  rotx[2][0] = -d;
  rotx[2][2] = cx;
  roty[1][1] = czd;
  roty[1][2] = -cyd;
  roty[2][1] = cyd;
  roty[2][2] = czd;

  concatrot( roty, rotx, rot1 );
  concatrot( rot90, rot1, rot );

  /* scale the vector */
  rot[0][0] *= scale;
  rot[1][1] *= scale;
  rot[2][2] *= scale;
  
  multmatrix(rot, bpos);

  gluCylinder(quadratic, 0.1f, 0.1f, 1, 16, 4);
  glTranslatef(0.0,0.0,1);

  /* make cone */
  gluCylinder(quadratic1,0.15f,0.0f,1./3. * 1, 16, 4);

  glPopMatrix();
  shadeoff();
  glDisable(GL_BLEND);
}

static GLuint head( float headcoor[][3] )
{
	GLuint headlist;
	headlist = glGenLists(1);
	glNewList( headlist, GL_COMPILE );
	glBegin( GL_LINES );
	glVertex3fv( headcoor[0] );
	glVertex3fv( headcoor[1] );
	glVertex3fv( headcoor[2] );
	glVertex3fv( headcoor[3] );
	glEnd();
	glEndList();
	return headlist;
}		

static GLuint tail( float tail[][3] )
{
	GLuint taillist; 	
	taillist = glGenLists(1);
	glNewList( taillist, GL_COMPILE );
	glBegin( GL_LINES );
	glVertex3fv( tail[0] );
	glVertex3fv( tail[1] );
	glEnd();
	glEndList();
	return taillist;
} 

void
darrow2d( GLfloat *bpos, GLfloat *epos, GLfloat scalef, GLfloat fixsize )
{
	GLfloat ld[3];  /* distance between two vectors */
	GLfloat length; /* length of vector */
	static int firstt = 1;
	static GLuint taill, headl;
	float cx,cy,cz,d,cyd,czd,cxcyd,cxczd,scale;
	float rotx[3][3] = {{1.,0.,0,},
		{0.,1.,0.},
		{0.,0.,1.}};
	float roty[3][3] = {{1.,0.,0,},
		{0.,1.,0.},
		{0.,0.,1.}};
	float rot90[3][3] = {{0.,0., 1.},
		{0.,1., 0.},
		{-1.,0.,0.}};
	float rot[3][3], rot1[3][3];

	Vecdis(ld, epos, bpos);
  	Veclen(length, ld);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	if ( firstt )
	{
		float headcoor[][3] = { {0.15,0.,0.}, {0.,0.,0.33}, {0.,0.,0.33,}, {-0.15,0.,0.}};
		float tailcoor[][3] = { {0.,0.,0.}, {0.,0.,1.} }; 
		headl = head( headcoor );
		taill = tail( tailcoor );
		firstt = 0;
	}
	/* scale vector */
	if ( scalef == 0 )
		return;
	if ( fixsize != 0 )
		scale = fixsize;
	else if ( scalef != 0 )
		scale = length * scalef;
	else
		scale = length;

	cx = ld[0] / length;
	cy = ld[1] / length;
	cz = ld[2] / length;

	d = sqrt( cy * cy + cz * cz );
	cyd = ( d == 0 ) ? 0. : cy / d;
	czd = ( d == 0 ) ? 1. : cz / d;
	cxcyd = ( d == 0 ) ? 0. : (cx * cy) / d;
	cxczd = ( d == 0 ) ? 1. : (cx * cz) / d;

	rotx[0][0] = cx;
	rotx[0][2] = d;
	rotx[2][0] = -d;
	rotx[2][2] = cx;
	roty[1][1] = czd;
	roty[1][2] = -cyd;
	roty[2][1] = cyd;
	roty[2][2] = czd;

	concatrot( roty, rotx, rot1 );
	concatrot( rot90, rot1, rot );

	/* scale vector */
	rot[0][0] *= scale;
	rot[1][1] *= scale;
	rot[2][2] *= scale;

	multmatrix( rot, bpos );

	glCallList( taill );
	glTranslatef( 0.0, 0.0, 1.0 );
	glCallList( headl );
	glPopMatrix();	
}

static void 
multmatrix( float in[3][3], float vec[3] )
{
  GLfloat mat[16];

  /* transpose matrix and save as 16 succesive values */
  mat[0] = in[0][0];
  mat[1] = in[1][0];
  mat[2] = in[2][0];
  mat[3] = 0.0;
  mat[4] = in[0][1];
  mat[5] = in[1][1];
  mat[6] = in[2][1];
  mat[7] = 0.0;
  mat[8] = in[0][2];
  mat[9] = in[1][2];
  mat[10] = in[2][2];
  mat[11] = 0.0;
  mat[12] = vec[0];
  mat[13] = vec[1];
  mat[14] = vec[2];
  mat[15] = 1.0;
  
  glMultMatrixf(mat);
}
