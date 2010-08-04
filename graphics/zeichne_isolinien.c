/*********************************************
 This routine shows the isolines on the
 model. 

 Authors:
 ======== 
          Helmut Vor & Toni Ivas
 Date:
 ====     
          07.08.2001
**********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "grafik.h"
#include "motif.h"
#include "iso.h"
#include "utilh.h"

/*
 * Macros which defines the intersection beetween 
 * two vertices of the same side in the triangle 
 * in x and y directions.
 */
	
#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#define zsect(p1,p2) (h[p2]*zh[p1]-h[p1]*zh[p2])/(h[p2]-h[p1])
#define vec1(p2,p1)  v1[0]=(xh[p2]-xh[p1]); \
                     v1[1]=(yh[p2]-yh[p1]); \
                     v1[2]=(zh[p2]-zh[p1]);

#define vec2(p2,p1)  v2[0]=(xh[p2]-xh[p1]); \
                     v2[1]=(yh[p2]-yh[p1]); \
                     v2[2]=(zh[p2]-zh[p1]);

#define MIN(x,y) ((x < y) ? x : y)
#define MAX(x,y) ((x > y) ? x : y)

#define MAX_TEM   10000.0         /*max. temp.*/
#define MIN_TEM   -273.0          /*min. temp.*/

static void drawl(float x1, float y1, float z1,
		  float x2, float y2, float z2, float n[3], int isol);
extern cmp_int(const void *, const void *);

/*global variables */
Vert *vert=NULL;
extern int g_itri;

/************************************************************
 Purpose:
 ========
        This function builds the linked list of triangles
	which is used to draw isolines on the model.
 Parameters:
 ==========
        Tri *listp - pointer to list of triangles.
        float isol - temp. of the isoline.
	int niso   - number of isolines.
 Returns:
 ========
       This function returns the list pointer to
       triangle list.
************************************************************/
Tri *btrilist(Tri *listp, float isol, int niso )
{
  Vert trivert[3];              /* vertex of triangle */
  Vert v[8];                    /* values of verticies */
  
 
  Tri *ntri=NULL;                /* new triangle, list pointer to triangle list */
  int ivol,i,j,id=0;             /* isovolume, counters and id of triangle*/
  int nverts=0;
  int ivert=0;
  int Stoffnr;                   /* number of current material */
  static int old_index = 0;      /* previous time-step */
  float mint=MAX_TEM, maxt=MIN_TEM; /* min and max temperature */
  float t;                          /* temporal variable */
  static t_stoffgebiet *oldmat=NULL;     /* list of "old" materials */
 
  /* allocate memory for "old" materials */
  if(oldmat == NULL)
    oldmat = (t_stoffgebiet *) malloc(g_geodat.nstoff*sizeof(t_stoffgebiet));
  if(oldmat == NULL) {
    fprintf(stderr,"Can't allocate memory\n");
    return NULL;
  }

  for(Stoffnr = 1; Stoffnr <= g_geodat.nstoff; Stoffnr++){
    if(listp == NULL)  /* first time initialize old materials to current value*/
      oldmat[Stoffnr-1].zeichne_on = g_stoffgebiete[Stoffnr-1].zeichne_on;
  }
  
  /* check if the time-step is changed*/
  if(g_animation.akt_index != old_index)
    {
      /* change old time when process all isolines */
      if(niso == g_isolinien.anzahl_isolinien) 
	old_index = g_animation.akt_index;
      freeall(listp);
      listp = NULL;
#ifdef DEBUG
      printf("free the trilist in time step %i\n", g_animation.akt_index);
#endif
    }
  /* check if materials are changed ??? */
  else
    {
      for(Stoffnr = 1; Stoffnr <= g_geodat.nstoff; Stoffnr++){
	if(oldmat[Stoffnr-1].zeichne_on != 
	   g_stoffgebiete[Stoffnr-1].zeichne_on){ /* materials changed */
	  /* free the previous linked list
	   * and build the new one.
	   */
	  freeall(listp);
	  listp = NULL;
	}
	/* materials not changed, non empty list return old list */
	else if(Stoffnr == g_geodat.nstoff && listp != NULL)  { 
	  return (listp);
	}
      }
    }

  /* allocate memory first time. */
  rtri(FREE);
  rtri(ALLOC);

  /*
   * Take all elements which are bounding surface
   * of the material and decompose them in
   * triangles. Put triangles in linked list and
   * calculate points on triangle which are in 
   * range of temperatures we want to show as 
   * isoline.
   */
  for (Stoffnr = 1; Stoffnr <= g_geodat.nstoff; Stoffnr++)
    {
      if (g_stoffgebiete[Stoffnr-1].zeichne_on)
	{
	  /* HEXAEDAR ----------------------------------*/
	  /* Loop over elements-------------------------*/
	  for(ivol = g_stoffgebiete[Stoffnr-1].anfang_hex;
	      ivol < g_stoffgebiete[Stoffnr-1].ende_hex; ivol++)
	    {
	      if(((g_element[ivol].zeichnen == 1)&&(g_transform.schneiden_on))||
		 (!g_transform.schneiden_on))
		{
		  nvert.nv += NPHEX;
		  rtri(ALLOC);
		  vert = nvert.vert;
		  mint = MAX_TEM;
		  maxt = MIN_TEM;
		  for(i = 0; i < NPHEX; i++)
		    {
		      /* check if element is inside
		       * temp. region for drawing
		       * isolines. Find max and
		       * min temperature.
		       */
		      t = g_loesung.temp[g_element[ivol].pn[i] - 1];
		      maxt = MAX(maxt, t);
		      mint = MIN(mint, t);
		      /* Temperature values */
		      vert[ivert].val   = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		      v[i] = vert[ivert];
		      ivert++;
		    }

	  	  if(mint < isol && isol < maxt)
		    {
		      if ((g_element[ivol].nachbar[0] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 1 side Hexaedar----------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[4];
			  trivert[2] = v[5];
			  ntri  = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[5];
			  trivert[1] = v[1];
			  trivert[2] = v[0];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[1] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 2 side Hexaedar----------------------------*/
			  trivert[0] = v[1];
			  trivert[1] = v[5];
			  trivert[2] = v[6];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[6];
			  trivert[1] = v[2];
			  trivert[2] = v[1];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[2] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 3 side Hexaedar----------------------------*/
			  trivert[0] = v[2];
			  trivert[1] = v[6];
			  trivert[2] = v[7];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[7];
			  trivert[1] = v[3];
			  trivert[2] = v[2];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[3] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 4 side Hexaedar----------------------------*/
			  trivert[0] = v[3];
			  trivert[1] = v[7];
			  trivert[2] = v[4];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[4];
			  trivert[1] = v[0];
			  trivert[2] = v[3];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[4] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 5 side Hexaedar----------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[1];
			  trivert[2] = v[2];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[2];
			  trivert[1] = v[3];
			  trivert[2] = v[0];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[5] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 6 side Hexaedar----------------------------*/
			  trivert[0] = v[4];
			  trivert[1] = v[5];
			  trivert[2] = v[6];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[6];
			  trivert[1] = v[7];
			  trivert[2] = v[4];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		    }
		}
	    }
	  /* PENTAEDAR ---------------------------------*/
	  /* Loop over elements-------------------------*/
	  for (ivol=g_stoffgebiete[Stoffnr-1].anfang_pen;
	       ivol<g_stoffgebiete[Stoffnr-1].ende_pen; ivol++)
	    {
	      if (((g_element[ivol].zeichnen == 1)&&(g_transform.schneiden_on))||
		  (!g_transform.schneiden_on)) 
		{
		  nvert.nv += NPPEN;
		  rtri(ALLOC);
		  vert = nvert.vert;
		  mint = MAX_TEM;
		  maxt = MIN_TEM; 
		  for(i = 0; i < NPPEN; i++)
		    {
		      t = g_loesung.temp[g_element[ivol].pn[i] - 1];
		      maxt = MAX(maxt, t);
		      mint = MIN(mint, t);
			
		      /* Temperature values */
		      vert[ivert].val = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		      v[i] = vert[ivert];
		      ivert++;
		    }

		  if(mint < isol && isol < maxt)
		    {
		      if ((g_element[ivol].nachbar[0] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 1 side Pentaedar---------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[3];
			  trivert[2] = v[4];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[4];
			  trivert[1] = v[1];
			  trivert[2] = v[0];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[1] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 2 side Pentaedar---------------------------*/
			  trivert[0] = v[1];
			  trivert[1] = v[4];
			  trivert[2] = v[5];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[5];
			  trivert[1] = v[2];
			  trivert[2] = v[1];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[2] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 3 side Pentaedar---------------------------*/
			  trivert[0] = v[2];
			  trivert[1] = v[5];
			  trivert[2] = v[3];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			  trivert[0] = v[3];
			  trivert[1] = v[0];
			  trivert[2] = v[2];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[4] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 4 side Pentaedar---------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[1];
			  trivert[2] = v[2];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[5] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 5 side Pentaedar---------------------------*/
			  trivert[0] = v[5];
			  trivert[1] = v[4];
			  trivert[2] = v[3];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		    }
		}
	    }

	  /* TETRAEDAR ---------------------------------*/
	  for (ivol=g_stoffgebiete[Stoffnr-1].anfang_tet;
	       ivol<g_stoffgebiete[Stoffnr-1].ende_tet; ivol++)
	    {
	      if (((g_element[ivol].zeichnen == 1)&&(g_transform.schneiden_on))||
		  (!g_transform.schneiden_on))
		{
		  nvert.nv += NPTET;
		  rtri(ALLOC);
		  vert = nvert.vert;
		  mint = MAX_TEM;
		  maxt = MIN_TEM;  
		  for(i = 0; i < NPTET; i++)
		    {
		      t = g_loesung.temp[g_element[ivol].pn[i] - 1];
		      maxt = MAX(maxt, t);
		      mint = MIN(mint, t);

		      /* Temperature values */
		      vert[ivert].val = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		      v[i] = vert[ivert];
		      ivert++;
		    }

		  if(mint < isol && isol < maxt)
		    {
		      if ((g_element[ivol].nachbar[0] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 1 side Tetraedar---------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[3];
			  trivert[2] = v[2];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[1] != Stoffnr)||
			  (g_transform.schneiden_on))
			{  
			  /* 2 side Tetraedar---------------------------*/
			  trivert[0] = v[1];
			  trivert[1] = v[2];
			  trivert[2] = v[3];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[2] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 3 side Tetraedar---------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[2];
			  trivert[2] = v[1];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		      if ((g_element[ivol].nachbar[3] != Stoffnr)||
			  (g_transform.schneiden_on))
			{
			  /* 4 side Tetraedar---------------------------*/
			  trivert[0] = v[0];
			  trivert[1] = v[1];
			  trivert[2] = v[3];
			  ntri = newitem(id++, trivert);
			  listp = addend(listp, ntri);
			}
		    }
		}
	    }
	}
      /* update old list of materials */
      oldmat[Stoffnr-1].zeichne_on = g_stoffgebiete[Stoffnr-1].zeichne_on;
	  
    }
  /* give back the list of triangles */
  return listp;
}


/**************************************************
 Purpose:
 ========
         This function interpolates vertices
         of triangle to find given temperature.
 Parametres:
 ===========
         float tempg - given temperature
	 Tri  *tri   - triangle
	 Iso  *line  - calculated isoline (not used)
 Returns:
 ========
         It returns nothing.
***************************************************/
void interpvert(Tri *tri, float tempg, Iso *line)
{
  int m;          /* index in the triangle points */

  float x1,x2,y1,y2,z1,z2;
  float xh[3], yh[3], zh[3]; /* coordinates of the points in triangle */
  float v1[3],v2[3];         /* vectors of triangle */ 
  float  out[3];             /* normal vector of triangle */
  float h[3];                /* values of the points (temperature,...) */
  int sh[3];            
  int case_value;
  int m1,m2,m3;              /* index of the points in triangle */ 
  
  /* this array gives different cases for 
   * calculating isoline
   */
  int castab[3][3][3] = {
    {{0,0,8},{0,2,5},{7,6,9}},
    {{0,3,4},{1,3,1},{4,3,0}},
    {{9,6,7},{5,2,0},{8,0,0}}
  };
 
  
  for(m = 0; m < 3; m++)
    {
      h[m]  = *tri->vert[m].val - tempg;
      xh[m] = *tri->vert[m].xyz[0];
      yh[m] = *tri->vert[m].xyz[1];
      zh[m] = *tri->vert[m].xyz[2];
      if(h[m] > 0.0)
	sh[m] = 1;
      else if(h[m] < 0.0)
	sh[m] = -1;
      else 
	sh[m] = 0;
    }
  
  m1 = 0;
  m2 = 1;
  m3 = 2;
  case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
  if (case_value != 0)
    {
      switch(case_value) 
	{
	  /* calculate the x,y,z between two temp. points */
	  /* case 1 line between vertices 1 and 2 */
	case 1:
	  x1 = xh[m1];
	  y1 = yh[m1];
	  z1 = zh[m1];
	  x2 = xh[m2];
	  y2 = yh[m2];
	  z2 = zh[m2];
	  break;

	  /* case 2 line between vertices 2 and 3 */
	case 2:
	  x1 = xh[m2];
	  y1 = yh[m2];
	  z1 = zh[m2];
	  x2 = xh[m3];
	  y2 = yh[m3];
	  z2 = zh[m3];
	  break;

	  /* case 3 line between vertices 3 and 1 */
	case 3:
	  x1 = xh[m3];
	  y1 = yh[m3];
	  z1 = zh[m3];
	  x2 = xh[m3];
	  y2 = yh[m3];
	  z2 = zh[m3];
	  break;

	  /* case 4 line between vertex 1 and side 2-3 */
	case 4:
	  x1 = xh[m1];
	  y1 = yh[m1];
	  z1 = zh[m1];
	  x2 = xsect(m2,m3);
	  y2 = ysect(m2,m3);
	  z2 = zsect(m2,m3);
	  break;

	  /* case 5 line between vertex 2 and side 3-1 */
	case 5:
	  x1 = xh[m2];
	  y1 = yh[m2];
	  z1 = zh[m2];
	  x2 = xsect(m3,m1);
	  y2 = ysect(m3,m1);
	  z2 = zsect(m3,m1);
	  break;

	  /* case 6 line between vertex 3 and side 1-2 */
	case 6:
	  x1 = xh[m3];
	  y1 = yh[m3];
	  z1 = zh[m3];
	  x2 = xsect(m1,m2);
	  y2 = ysect(m1,m2);
	  z2 = zsect(m1,m2);
	  break;

	  /* case 7 line between side 0-1 and side 1-2 */
	case 7:
	  x1 = xsect(m1,m2);
	  y1 = ysect(m1,m2);
	  z1 = zsect(m1,m2);
	  x2 = xsect(m2,m3);
	  y2 = ysect(m2,m3);
	  z2 = zsect(m2,m3);
	  break;

	  /* case 8 line between side 2-3 and side 3-1 */
	case 8:
	  x1 = xsect(m2,m3);
	  y1 = ysect(m2,m3);
	  z1 = zsect(m2,m3);
	  x2 = xsect(m3,m1);
	  y2 = ysect(m3,m1);
	  z2 = zsect(m3,m1);
	  break;

	  /* case 9 line between side 3-1 and side 1-2 */
	case 9:
	  x1 = xsect(m3,m1);
	  y1 = ysect(m3,m1);
	  z1 = zsect(m3,m1);
	  x2 = xsect(m1,m2);
	  y2 = ysect(m1,m2);
	  z2 = zsect(m1,m2);
	  break;
	default:
	  break;
	}
      /*calculate normal vector*/
      vec1(m2,m1)
	vec2(m3,m1)
	crossprod(v1,v2,out);
      drawl(x1,y1,z1,x2,y2,z2,out, line->niso);
    }
  
}

/******************************************************
 Purpose:
 ========
         Draw the isolines by drawing line by line.

 Parameters:
 ===========
         float x1   - point 1
	 float y1   - point 1
	 float z1   - point 1
	 float x2   - point 2
	 float y2   - point 2
	 float z2   - point 2
	 float n[3] - normal of current triangle
	 int isol   - isoline value

 Returns:
 ========
         It returns nothing.
********************************************************/
static void drawl(float x1, float y1, float z1,
		  float x2, float y2, float z2, float n[3], int isol)
{
  int r,g,b;  /* color values */
  struct listelem *litem, *isolinie;
  void *p;
  t_linie *iso, *linie;

  /* make current the drawing context */
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
 
  /* get color of the isoline */
  isolinie = g_isolinien.linien->head;
  linie = (t_linie *) isolinie->data;
  while(linie->nr != isol)
    {
      isolinie = isolinie->next;
      linie = (t_linie *) isolinie->data;
    }
  p = glookup(g_isolinien.linien, (void *)linie, cmp_int);
  litem = (struct listelem *) p;
  iso = (t_linie *) litem->data;
  r = (int)(iso->r*255.0);
  g = (int)(iso->g*255.0);
  b = (int)(iso->b*255.0);

  /* adjust all drawing parameters */
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(4.0);

  glShadeModel(GL_SMOOTH);
  x1 += 0.1*n[0];
  y1 += 0.1*n[1];
  z1 += 0.1*n[2];
  x2 += 0.1*n[0];
  y2 += 0.1*n[1];
  z2 += 0.1*n[2];

  /* draw isoline as polyline */
  glBegin(GL_LINES);
   glColor3ub(r,g,b);
   glVertex3f(x1,y1,z1);
   glVertex3f(x2,y2,z2);
  glEnd();
  glFlush();
}

/* my_realloc: reallocate memory 
 * This function allocates and deallocates 
 * the memory.
 */
int rtri(int f)
{
  Vert *tp;
  if(f){
    if(nvert.vert != NULL){
      free(nvert.vert);
      nvert.vert = NULL;
    }
  }
  else{
    if(nvert.vert == NULL){
      nvert.vert = (Vert *) malloc(NINTV*8*sizeof(Vert));
      if(nvert.vert == NULL)
	return -1;
      nvert.maxv = NINTV*8;
      nvert.nv = 0;
    }
    while(nvert.nv >= nvert.maxv){
      tp = (Vert *) realloc(nvert.vert, (NVGROW*nvert.maxv)*sizeof(Vert)); 
      if(tp == NULL)
	return -1;
      nvert.maxv *= NVGROW;
      nvert.vert = tp;
    }
  }
  return (nvert.nv);
}











