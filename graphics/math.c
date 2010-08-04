/* This functions evaluates Heaviside function and
 * staircase function. 
 */
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "grafik.h"
#include "gendlist.h"
#include "genstack.h"
#include "memory.h"

enum{   X = 0,
	Y = 1,
	Z = 2,
	D = 3
};

#define SWAP(t,x,y)     { t = x; x = y; y = t; }
#define ISDOUBLE_EQ(a,b) ((fabs((a)-(b)) >= (double) TOLER) ? 0 : 1)
#define VZERO(v)    (v[X] = v[Y] = v[Z] = 0.0)
#define VNORM(v)    (sqrt(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]))
#define VDOT(u, v)  (u[0] * v[0] + u[1] * v[1] + u[2] * v[2])
#define VINCR(u, v) (u[X] += v[X], u[Y] += v[Y], u[Z] += v[Z])

static void mulMxV(float M[][4], float v[], float vr[]);
static void mulMxM(float M[][4], float Mn[][4], float Mr[][4]);
static void find_lowest(int n, t_point **P);
static void swap(int i, int j, t_point **P);
static float area2(float *a, float *b, float *c);
static int area_sign(float *a, float *b, float *c);
int compare(const void *tpi, const void *tpj);
static int left(t_point a, t_point b, t_point c);
void destroy_stack(void *data);

/* global variables */
float *PointLow;
int ndelete = 0;

/**********************************************
 Purpose:
 ========
   This function is used for interpolation 
   of colortable.
 Parameters:
 ===========
         -double x - independent variable
         -double b - width of step
         -double c - height of step
***********************************************/
double staircase(double x, double b, double c)
{
	double sum = 0.0, s = 0.0;
	int i, steps;
	steps = (int)(x/b);
	for (i = 0; i < steps; i++) {
		s = heaviside( x, -b, 10e-5) + c - 1;
		sum += s;
	}
	sum += heaviside(x, 0, 10e-5) + c - 1;
	return sum;
}

/**********************************************
 Purpose:
 ========
   This function is calculates Heaviside 
   function.
 Parameters:
 ===========
         -double x - independent variable
         -double b - width of step
         -double t - precision of aproximation
***********************************************/
double heaviside(double x, double b, double t)
{
    x = x - b;
    return 1.0/(1.0 + exp(-x/t));
}

/**********************************************
 Purpose:
 ========
   This function interpolates between two val.
 Parameters:
 ===========
	     -double t1 - first value
         -double t2 - second value
         -double ratio - ratio between values
**********************************************/
double interpolate2(double t1, double t2, double ratio)
{
	return (t1 + (t2 - t1) * ratio);
}

/**********************************************
 Purpose:
 ========
   This function returns intersection of cube 
   and plane.
 Parameters:
 ===========
	 -float boxExtent - extents of the box
         -float boxAxis   - axis vectors for box 
         -float plane     - plane equation, a,b,c,d
         -float center    - center of box
**********************************************/
int box2plane(float *boxExtent, float (*boxAxis)[3], float *plane,
              float *center)
{
  float temp;
  float r;
  float pseudodist;
  double dot1, dot2, dot3;

  dot1 = dotproduct(boxAxis[0],plane);
  dot2 = dotproduct(boxAxis[1],plane);
  dot3 = dotproduct(boxAxis[2],plane);
  r = boxExtent[0]*fabs(dot1)+
      boxExtent[1]*fabs(dot2)+
      boxExtent[2]*fabs(dot3);

  pseudodist = dotproduct(plane, center)-plane[3];

  return pseudodist < r;
}

float dist_pt2plane(float *pt, float *plane)
{
  float dist;
  
  dist = dotproduct(plane, pt) - plane[3];
  return(dist);
}

/**********************************************
 Purpose:
 ========
   This function returns intersection of edge 
   and plane.
 Parameters:
 ===========
	 -float *p1   - start point of edge
         -float *p2   - end point of edge 
         -float *plane- plane equation, a,b,c,d
         -float *ip   - intersection point if exits.
 Returns:
 ========
         -float *ip - intersection point,
          -1 if negative, 0 - on plane, 1 positive
**********************************************/
int edge_inter_plane(float *plane, float *p1, float *p2, float *ip)
{
  double temp1, temp2;
  int sign1, sign2;

  /* add small number to distance plane[3] to orgin,
   * this guards against problems with null point.  
   */
  //plane[3] += 0.00001;
  temp1 = plane[0]*p1[0] + plane[1]*p1[1] + 
    plane[2]*p1[2] - plane[3];
  if (temp1 < -TOLER)
    sign1 = -1;
  else if (temp1 > TOLER)
    sign1 = 1;
  else 
    {
      assert(ISDOUBLE_EQ(temp1,0.0));
      return 0;
    }

  temp2 = plane[0]*p2[0] + plane[1]*p2[1] +
    plane[2]*p2[2] - plane[3];
  if (temp2 < -TOLER)
    sign2 = -1;
  else if (temp2 > TOLER)
    sign2 = 1;
  else {
    assert(ISDOUBLE_EQ(temp2,0.0));
    ip[0] = p2[0];
    ip[1] = p2[1];
    ip[2] = p2[2];

    return ( ( sign1 == -1) ? -1 : 1);
  }
  
  if (sign1 ^ sign2)
    {
      double dx,dy,dz;
      double denom, tt;

      /* compute intersection point */
      dx = p2[0]-p1[0];
      dy = p2[1]-p1[1];
      dz = p2[2]-p1[2];

      denom = plane[0]*dx + plane[1]*dy + plane[2]*dz;
      tt    = -(plane[0]*p1[0] + plane[1]*p1[1] 
		+ plane[2]*p1[2] - plane[3])/denom;

      ip[0] = p1[0] + tt*dx;
      ip[1] = p1[1] + tt*dy;
      ip[2] = p1[2] + tt*dz;
      
      assert(sign1 == 1 || sign1 == -1);
      return ((sign1 == -1) ? -1 : 1);
	
    }
  else
    return (0);
}

void printpt(struct dlistelem *, void* );

void
onhull(int n, t_point **inpoints, float *pnormal)
{
  float z[3] = { 0.0F, 0.0F, 1.0F };
  float dst[3], p[3], dir[3];
  int i;
  float degrees, angle;

  anglevec( &angle, pnormal, z );
  degrees = RAD2DEG( angle );

  crossprod( pnormal, z, dir );
  for (i = 0; i < n; i++)
    {
      p[0] = inpoints[i]->v[0]; 
      p[1] = inpoints[i]->v[1];
      p[2] = inpoints[i]->v[2];

      rotatept2vect( dst, dir, p, degrees );
      
      inpoints[i]->v[0] = dst[0];
      inpoints[i]->v[1] = dst[1];
      inpoints[i]->v[2] = dst[2];
    }
  find_lowest( n, inpoints );
  qsort( &inpoints[1], n-1, sizeof( t_point ** ), compare );
}

  
/**********************************************
 Purpose:
 ========
   This function preforms Graham scan to find  
   convex hull.
 Parameters:
 ===========
        - int n - number of points on the hull
        - dlistelem *P - array of stack elements.
 Returns:
 ========
        - returns stack of hull points.
**********************************************/
struct dlist* graham(int n, t_point **inpoints, float *pnormal)
{
  int i,j;
  float R[4][4] = {{0.0,0.0,0.0,0.0},
		   {0.0,0.0,0.0,0.0},
		   {0.0,0.0,0.0,0.0},
		   {0.0,0.0,0.0,1.0}};
  float l;
 
  struct dlist *top = NULL;
  t_point *telem;
  struct dlistelem *P[100];
  t_point *p1, *p2, *p;
  t_point *P1, *P2, *P3, *vr;


  float vd12[3], vd13[3], vd[3], vn[3];

  NEW(vr);
  for (i = 0; i < n; i++)
    printf("Intersection points: %i, %f, %f\n", inpoints[i]->vnum,
	   inpoints[i]->v[0], inpoints[i]->v[1]);	   
  /* create stack items from intersection points */
  for (i = 0; i < n; i++)
    P[i] = (struct dlistelem *)dnewitem((void *)inpoints[i]);
  
  /* To project points of hull in xy plane we take three points P1 P2 P3 
   * not all at same line. Translate P1 to origin T = [-x1 -y1 -z1].
   * Then we rotate to get P1P2 in x direction. Rx = [r1x r2x r3x] = P1P2/|P1P2|.
   * P1P2P3 must be ortogonal to z direction. Rz = [r1z r2z r3z] = P1P2 x P1P3/ |P1P2 x P1P3|.
   * Finally Ry = Rz x Rx. So complete transformation is M = Transpose[Rx Ry Rz] * T.
   */
  P1 = inpoints[0]; P2 = inpoints[1];
  P3 = inpoints[2];

  for (j = 0; (j < n-2) && ISDOUBLE_EQ( area2(P1->v,P2->v, P3->v), 0.0 ); j++) 
    {
      printf("POINTS ARE ALMOST COLLINEAR\n");
      P3 = inpoints[2+j];
    }
  
  Vecdis(vd12, P1->v, P2->v);
  Veclen(l, vd12);
  l *= 1000.0;
  Vecxscal(vd12, vd12, 1000.0);
 // assert(ISDOUBLE_EQ(l,0.0));

  /* rotation matrix around x coord. */
  R[0][0] = vd12[0]/l; R[0][1] = vd12[1]/l;
  R[0][2] = vd12[2]/l; R[0][3] = 0.0;

  Vecdis(vd13, P1->v, P3->v);
  crossprod(vd12, vd13, vn);

  /* rotation matrix around z coord. */ 
  R[2][0] = vn[0]; R[2][1] = vn[1];
  R[2][2] = vn[2]; R[2][3] = 0.0;

  /* rotation matrix around y coord. */
  crossprod(R[2], R[0], R[1]);

  for (i = 0; i < n; i++)
    {
      t_point *vp;
     
      vp = inpoints[i];
      vr->vnum = vp->vnum;
 
      /* translate all points so P1 is in origin. (R*T)*P */
      Vecdis(vr->v, P1->v, vp->v);
      mulMxV(R, vp->v, vr->v);
      printf("Projected point:%i, x:%f, y:%f\n", vr->vnum, vr->v[0], vr->v[1]);
      *vp = *vr;
      
    }

  /* find lowest point in array */
  find_lowest( n, inpoints );
  printf("Lowest Point: %i, %f, %f, %f\n", inpoints[0]->vnum, 
	 inpoints[0]->v[0], inpoints[0]->v[1], inpoints[0]->v[2]);
  for (i = 0; i < n; i++)
   printf("After swaping point: %i, %f, %f \n", inpoints[i]->vnum, 
	  inpoints[i]->v[0], inpoints[i]->v[1]); 

  /* sort the points in acsending order */
  qsort(&inpoints[1], n-1, sizeof(point **), compare);
  for (i = 0; i < n; i++)
    printf("Ordered points: %i, %f, %f\n", inpoints[i]->vnum, 
	  inpoints[i]->v[0], inpoints[i]->v[1]);

  /* initialize stack */
  NEW(top);
  stackinit(top, destroy_stack);

  stackpush( top, dnewitem((void*)inpoints[0]) );
  stackpush( top, dnewitem((void*)inpoints[1]) );

  dapply(top, printpt, "Pushing num:%i: x:%f, y:%f, z:%f\n");

  /* bottom two elements will never be removed */
  i = 2;
 
  while (i < n) 
    {
      if (!top->head)
	return NULL;

      p2 = (t_point *)top->head->next->data;
      p1 = (t_point *)top->head->data;
      p  = (t_point *)inpoints[i];

      if (left(*p1, *p2, *p)) {
	top->head = stackpush( top, dnewitem((void*)inpoints[i]) );
	i++;
      } else {
	NEW(telem);
	stackpop(top, (void *)&telem);
	printf("Poping : %i\n", (point *)telem->vnum);
	FREE(telem);	
      }
    }

  FREE(vr);
  return (top);
}

void printpt(struct dlistelem *p, void *arg)
{
    char *fmt;
    t_point *navp;

    fmt = (char *) arg;
    navp = (t_point *) p->data;
    printf(fmt, navp->vnum, navp->v[0],navp->v[1],navp->v[2]);
}

static void find_lowest(int n, t_point **P)
{
  int i;
  int m = 0;
  t_point *pi, *pm;

  for (i = 0; i < n; i++)
    {
      pi = P[i];
      pm = P[m];  

      if ((pi->v[1] < pm->v[1]) ||
	  (pi->v[1] == pm->v[1]) && (pi->v[0] > pm->v[0]))
	m = i;

      swap(0, m, P);
    }

  /* pass left lowest point to compare function */
  PointLow = P[0]->v;
}

static void swap(int i, int j, t_point **P)
{
  int temp;
  float ftemp;
  t_point *pi, *pj;

  pi =  P[i];
  pj =  P[j];

  SWAP(temp,  pi->vnum, pj->vnum);
  SWAP(ftemp, pi->v[0], pj->v[0]);
  SWAP(ftemp, pi->v[1], pj->v[1]);
}

/* The area is positive if a,b,c are ccw, negative if cw, and zero collinear */
static float area2(float *a, float *b, float *c)
{
  return (b[0] - a[0]) * (c[1] - a[1]) -
         (c[0] - a[0]) * (b[1] - a[1]);
}

static int area_sign(float *a, float *b, float *c)
{
  double area2;
  
  area2 = (b[0] - a[0]) * (c[1] - a[1]) -
          (c[0] - a[0]) * (b[1] - a[1]);

  if      (area2 > 0.5 ) return  1;
  else if (area2 < -0.5) return -1;
  else                   return  0;
} 



int compare(const void *tpi, const void *tpj)
{
  int a;
  float x, y;
  t_point *pi, *pj;
    
  pi  = (t_point *) (*(t_point **)tpi);
  pj  = (t_point *) (*(t_point **)tpj);
  
  a = area_sign(PointLow, pi->v, pj->v);
  if (a > 0)
    return (-1);
  else if (a < 0)
    return (1);
  else 
    {
      x = abs(pi->v[0] - PointLow[0]) - abs(pj->v[0] - PointLow[0]);
      y = abs(pi->v[1] - PointLow[1]) - abs(pj->v[1] - PointLow[1]);
  
      ndelete++;

      if ((x < 0) || (y < 0))
	{
	  pi->delete = TRUE;
	  return -1;
	}
      else if (x > 0 || y > 0)
	{
	  pj->delete = TRUE;
	  return 1;
	}
      else
	{
	  if (pi->vnum > pj->vnum)
	    pj->delete = TRUE;
	  else
	    pi->delete = TRUE;
	  return 0;
	}
    }
}

/* Returns true iff c is strictly to the left of direct line a-b. */
int left(t_point a, t_point b, t_point c)
{
  return area2(a.v, b.v, c.v) > 0;
}

void destroy_stack(void *data)
{
  t_point *p;

  p = (t_point *) data;
  FREE (p);
}

static void mulMxV(float M[][4], float v[], float vr[])
{
  vr[0] = M[0][0]*v[0]+M[0][1]*v[1]+M[0][2]*v[2]+M[0][3]*v[3];
  vr[1] = M[1][0]*v[0]+M[1][1]*v[1]+M[1][2]*v[2]+M[1][3]*v[3];
  vr[2] = M[2][0]*v[0]+M[2][1]*v[1]+M[2][2]*v[2]+M[2][3]*v[3];
  vr[3] = M[3][0]*v[0]+M[3][1]*v[1]+M[3][2]*v[2]+M[3][3]*v[3];
}

void mulMxV16(float M[], float v[], float vr[])
{
  vr[0] = M[0]*v[0]+M[4]*v[1]+M[8]*v[2]+M[12]*v[3];
  vr[1] = M[1]*v[0]+M[5]*v[1]+M[9]*v[2]+M[13]*v[3];
  vr[2] = M[2]*v[0]+M[6]*v[1]+M[10]*v[2]+M[14]*v[3];
  vr[3] = M[3]*v[0]+M[7]*v[1]+M[11]*v[2]+M[15]*v[3];
}
  
static void mulMxM(float M[][4], float Mn[][4], float Mr[][4])
{
  int i,j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      Mr[i][j] = M[i][j]*Mn[j][i];
}

/**************************************************************
  newell--computes the plane equation of an arbitrary
  3D polygon using Newell's method.

  Parameters:
      verts  - list of the vertices of the polygon
      nverts - number of vertices of the polygon
  Returns:
      plane  - normalized (unit normal) plane equation
****************************************************************/
void 
newell(float (*verts)[3], int nverts, float *plane)
{
  int i;
  float refpt[3];
  float normal[3];
  float *u, *v, len;
  
  
  /* compute the polygon normal and a reference point on
   * the plane. Note that the actual reference point is
   * refpt / nverts
   */
  VZERO(normal);
  VZERO(refpt);
  for(i = 0; i < nverts; i++) {
    u = verts[i];
    v = verts[(i + 1) % nverts];
    normal[X] += (u[Y] - v[Y]) * (u[Z] + v[Z]);
    normal[Y] += (u[Z] - v[Z]) * (u[X] + v[X]);
    normal[Z] += (u[X] - v[X]) * (u[Y] + v[Y]);
    VINCR(refpt, u);
  }
  /* normalize the polygon normal to obtain the first
   * three coefficients of the plane equation
   */
  len = VNORM(normal);
  plane[X] = normal[X] / len;
  plane[Y] = normal[Y] / len;
  plane[Z] = normal[Z] / len;
  /* compute the last coefficient of the plane equation */
  len *= nverts;
  plane[D] = -VDOT(refpt, normal) / len;
}

/**************************************************************
  rotatept2vect -- rotate point around given vector for given 
  degrees.

  Parameters:
       float *dir - direction vector
       float *point - point rotated
       float degrees - how many degrees
  Returns:
       float *dst - calculated point  
**************************************************************/
void 
rotatept2vect( float *dst, const float *dir, const float *point, float degrees )
{
  float   m[3][3];
  float   im[3][3];
  float   zrot[3][3];
  float   tmpmat[3][3];
  float   rot[3][3];
  int i;
  float vr[3], vup[3], vf[3];

  vf[0] = dir[0];
  vf[1] = dir[1];
  vf[2] = dir[2];

  perpvect(vr, dir);
  crossprod(vr, vf, vup);
  
  m[0][0] = vr[0];
  m[1][0] = vr[1];
  m[2][0] = vr[2];

  m[0][1] = vup[0];
  m[1][1] = vup[1];
  m[2][1] = vup[2];
  
  m[0][2] = vf[0];
  m[1][2] = vf[1];
  m[2][2] = vf[2];

  memcpy(im, m, sizeof(im));

  im[0][1] = m[1][0];
  im[0][2] = m[2][0];
  im[1][0] = m[0][1];
  im[1][2] = m[2][1];
  im[2][0] = m[0][2];
  im[2][1] = m[1][2];

  memset( zrot, 0, sizeof( zrot ));
  zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0;

  zrot[0][0] = cos( DEG2RAD( degrees ) );
  zrot[0][1] = sin( DEG2RAD( degrees ) );
  zrot[1][0] = -sin( DEG2RAD( degrees ) );
  zrot[1][1] = cos( DEG2RAD( degrees ) );
  
  concatrot( m, zrot, tmpmat );
  concatrot( tmpmat, im, rot );
  
  for (i = 0; i < 3; i++)
    {
      dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
    }
}

void 
projectpt2plane( float *dst, const float *p, const float *normal)
{
  float d;
  float n[3];
  float inv_denom;
  
  inv_denom = 1.0F / dotproduct( normal, normal );
  
  d = dotproduct( normal, p ) * inv_denom;
  
  n[0] = normal[0] * inv_denom;
  n[1] = normal[1] * inv_denom;
  n[2] = normal[2] * inv_denom;
  
  dst[0] = p[0] - d * n[0];
  dst[1] = p[1] - d * n[1];
  dst[2] = p[2] - d * n[2];  
}

void
anglevec( float *angle, const float* vec1, const float *vec2 )
{
  float cs, l1, l2;
  Veclen(l1, vec1);
  Veclen(l2, vec2);
  cs = dotproduct( vec1, vec2 ) / ( l1 * l2 );
  *angle = (float) acos( (double) cs );
}

void 
perpvect( float *dst, const float *src )
{
  int pos;
  int i;
  float minelem = 1.0F;
  float tempvec[3];

  for (pos = 0, i = 0; i < 3; i++)
    {
      if ( fabs( src[i] ) < minelem )
	{ 
	  pos = i;
	  minelem = fabs( src[i] );
	}
    }
  tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
  tempvec[pos] = 1.0F;

  /* project point onto the plane defined by src */
  projectpt2plane( dst, tempvec, src );
  
  /* normalize result */
  normalize( dst );
}

void
concatrot( float in1[3][3], float in2[3][3], float out[3][3] )
{
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
              in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
              in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
              in1[0][2] * in2[2][2];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
              in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
              in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
              in1[1][2] * in2[2][2];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][1] +
              in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
              in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
              in1[2][2] * in2[2][2];
}

void
concattransforms ( float in1[3][4], float in2[3][4], float out[3][4] )
{
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
    in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
    in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
    in1[0][2] * in2[2][2];
  out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
    in1[0][2] * in2[2][3] + in1[0][3];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
    in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
    in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
    in1[1][2] * in2[2][2];
  out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
    in1[1][2] * in2[2][3] + in1[1][3];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
    in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
    in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
    in1[2][2] * in2[2][2];
  out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
    in1[2][2] * in2[2][3] + in1[2][3];
}

float
lerpangle( float a2, float a1, float frac )
{
  if (a1 - a2 > 180)
    a1 -= 360;
  if (a1 - a2 < -180)
    a1 += 360;
  return a2 + frac * (a1 - a2);
}

float	
anglemod(float a)
{
#if 0
  if (a >= 0)
    a -= 360*(int)(a/360);
  else
    a += 360*( 1 + (int)(-a/360) );
#endif
  a = (360.0/65536) * ((int)(a*(65536/360.0)) & 65535);
  return a;
}  

void M_settranslation(float v[4], float mat[16])
{
  float matt[16], out[16];
  M_identity(matt);
  matt[12] = v[0];
  matt[13] = v[1];
  matt[14] = v[2];
  M_multmatrix( mat, matt, out );
  M_copymatrix( out, mat );
}

void M_settranslation3f( float vx, float vy, float vz, float mat[16])
{
  float v[4];
  v[0] = vx; v[1] = vy; v[2] = vz;
  M_settranslation( v, mat );
}

void M_gettranslation(float v[4], float mat[16])
{
  v[0] = mat[12];
  v[1] = mat[13];
  v[2] = mat[14];
  v[3] = 1;
}

void M_identity(float mat[16])
{
  mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
  mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
  mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
  mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

void M_multmatrix( float in1[16], float in2[16], float out[16] )
{
  out[0] = in1[0] * in2[0] + in1[4] * in2[1] + 
    in1[8] * in2[2] + in1[12] * in2[3];
  out[4] = in1[0] * in2[4] + in1[4] * in2[5] + 
    in1[8] * in2[6] + in1[12] * in2[7];
  out[8] = in1[0] * in2[8] + in1[4] * in2[9] + 
    in1[8] * in2[10] + in1[12] * in2[11];
  out[12] = in1[0] * in2[12] + in1[4] * in2[13] + 
    in1[8] * in2[14] + in1[12] * in2[15];
  
  out[1] = in1[1] * in2[0] + in1[5] * in2[1] + 
    in1[9] * in2[2] + in1[13] * in2[3];
  out[5] = in1[1] * in2[4] + in1[5] * in2[5] + 
    in1[9] * in2[6] + in1[13] * in2[7];
  out[9] =in1[1] * in2[8] + in1[5] * in2[9] + 
    in1[9] * in2[10] + in1[13] * in2[11];
  out[13] =in1[1] * in2[12] + in1[5] * in2[13] + 
    in1[9] * in2[14] + in1[13] * in2[15];
  
  out[2] = in1[2] * in2[0] + in1[6] * in2[1] + 
    in1[10] * in2[2] + in1[14] * in2[3];
  out[6] = in1[2] * in2[4] + in1[6] * in2[5] + 
    in1[10] * in2[6] + in1[14] * in2[7];
  out[10] = in1[2] * in2[8] + in1[6] * in2[9] + 
    in1[10] * in2[10] + in1[14] * in2[11];
  out[14] = in1[2] * in2[12] + in1[6] * in2[13] + 
    in1[10] * in2[14] + in1[14] * in2[15];

  out[3] = in1[3] * in2[0] + in1[7] * in2[1] + 
    in1[11] * in2[2] + in1[15] * in2[3];
  out[7] = in1[3] * in2[4] + in1[7] * in2[5] + 
    in1[11] * in2[6] + in1[15] * in2[7];
  out[11] = in1[3] * in2[8] + in1[7] * in2[9] + 
    in1[11] * in2[10] + in1[15] * in2[11];
  out[15] = in1[3] * in2[12] + in1[7] * in2[13] + 
    in1[11] * in2[14] + in1[15] * in2[15];
}

void M_copymatrix( float in[16], float out[16] )
{
  int i;
  for (i = 0; i < 16; i++)
    out[i] = in[i];
}

void M_setrotationx ( float v,  float mat[16] )
{

  float cv, sv;
  float mat1[16], out[16];
  cv = cos( DEG2RAD(v) );
  sv = sin( DEG2RAD(v) );
  
  M_identity(mat1);
  mat1[5] = cv; mat1[10] = cv;
  mat1[6] = sv; mat1[9] = -sv;
  
  M_multmatrix( mat, mat1, out );
  M_copymatrix( out, mat );
  
}
  
void M_setrotationy ( float v,  float mat[16] )
{

  float cv, sv;
  float mat1[16], out[16];
  cv = cos( DEG2RAD(v) );
  sv = sin( DEG2RAD(v) );
  
  M_identity(mat1);
  mat1[0] = mat1[10] = cv;
  mat1[2] = -sv; mat1[8] = sv;
  
  M_multmatrix( mat, mat1, out );
  M_copymatrix( out, mat );
  
}

void M_setrotationz ( float v,  float mat[16] )
{

  float cv, sv;
  float mat1[16], out[16];
  cv = cos( DEG2RAD(v) );
  sv = sin( DEG2RAD(v) );
  
  M_identity(mat1);
  mat1[0] = mat1[5] = cv;
  mat1[4] = -sv; mat1[1] = sv;
  
  M_multmatrix( mat, mat1, out );
  M_copymatrix( out, mat );
  
}

void M_setscale ( float s,  float mat[16] )
{
  mat[0] *= s;
  mat[5] *= s;
  mat[10] *= s;
}

void M_printmatrix( float mat[16] )
{
  int i,j;
  for (i = 0; i < 4; i++)
    {
      printf("|");
      for (j = 0; j < 4; j++)
	printf(" %f", mat[i + j*4]);
      printf(" |\n");
    }
  printf("\n");
}

void M_transfmat_to_opengl(float mat[4][4], float matopengl[16])
{
  int i,j;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      matopengl[i+4*j] = mat[i][j];
}
 
