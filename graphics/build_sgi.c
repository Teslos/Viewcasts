/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Toni Ivas & Helmut Vor                           */
/* Datum    :   06.01.97                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname        | Funktion                                */
/* ------------------------------------------------------------- */ 
/*-------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */
#include <GL/gl.h>
#include <GL/glu.h>
#include "grafik.h"
#include "color3d.h"
#include "paulslib.h"
#include "sgiobj.h"
#include "utilh.h"
void puttri(float *fptr, float *p0, float *p1, float *p2, float *n0, 
	    float *n1, float *n2, float *c0, float *c1, float *c2);
static void calcNormFace(Vert v0, Vert v1, Vert v2, GLfloat *norm);
			
/************************************************************
 Purpose:
 ========
        This function builds the linked list of triangles
	which is used to draw the model. This routine is
	practicly the same as btrilist.
 Parameters:
 ==========
        Tri *listp - pointer to list of triangles.
        long *numtri   - number of triangles.
	
 Returns:
 ========
       This function returns the list pointer to
       triangle list.
************************************************************/
Tri *btrilistElem(Tri *listp, long *numtri)
{
  Vert trivert[3];               /* vertex of triangle                          */
  Vert v[8];                     /* values of verticies                         */
  
  Vert *vert=NULL;
  Tri *ntri=NULL;                /* new triangle, list pointer to triangle list */
  int ivol,i,j,id=0;             /* isovolume, counters and id of triangle      */
  int nverts=0;
  int ivert=0;
  float norm[3];                 /* face-normals of the triangles               */
  int Stoffnr;                   /* number of current material                  */
  static int old_index = 0;      /* previous time-step                          */
  float t;                       /* temporal variable                           */
  static t_stoffgebiet 
    *oldmat=NULL;                /* list of "old" materials                     */
 
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
  
 
  /* check if materials are changed ??? */
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
		  for(i = 0; i < NPHEX; i++)
		    {
		      /* Temperature values */
		      vert[ivert].val   = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		     
		      v[i] = vert[ivert];
		      ivert++;
		    }

	  	
		  if (g_element[ivol].nachbar[0] != Stoffnr)
		    {
		      /* 1 side Hexaedar----------------------------*/
		      /* calculate normals on face of triangle*/
		      calcNormFace(v[0], v[4], v[5], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[4];
		      trivert[2] = v[5];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[5], v[1], v[0], norm);
		      trivert[0] = v[5];
		      trivert[1] = v[1];
		      trivert[2] = v[0];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[1] != Stoffnr)
		    {		    
		      /* 2 side Hexaedar----------------------------*/
		      calcNormFace(v[1], v[5], v[6], norm);
		      trivert[0] = v[1];
		      trivert[1] = v[5];
		      trivert[2] = v[6];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[6], v[2], v[1], norm);
		      trivert[0] = v[6];
		      trivert[1] = v[2];
		      trivert[2] = v[1];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[2] != Stoffnr)
		    {
		      /* 3 side Hexaedar----------------------------*/
		      calcNormFace(v[2], v[6], v[7], norm);
		      trivert[0] = v[2];
		      trivert[1] = v[6];
		      trivert[2] = v[7];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[7], v[3], v[2], norm);
		      trivert[0] = v[7];
		      trivert[1] = v[3];
		      trivert[2] = v[2];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[3] != Stoffnr)
		    {
		      /* 4 side Hexaedar----------------------------*/
		      calcNormFace(v[3], v[7], v[4], norm);
		      trivert[0] = v[3];
		      trivert[1] = v[7];
		      trivert[2] = v[4];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[4], v[0], v[3], norm);
		      trivert[0] = v[4];
		      trivert[1] = v[0];
		      trivert[2] = v[3];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[4] != Stoffnr)
		    {		    
		      /* 5 side Hexaedar----------------------------*/
		      calcNormFace(v[0], v[1], v[2], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[1];
		      trivert[2] = v[2];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[2], v[3], v[0], norm);
		      trivert[0] = v[2];
		      trivert[1] = v[3];
		      trivert[2] = v[0];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[5] != Stoffnr)
		    {
		      /* 6 side Hexaedar----------------------------*/
		      calcNormFace(v[5], v[4], v[7], norm);
		      trivert[0] = v[5];
		      trivert[1] = v[4];
		      trivert[2] = v[7];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[7], v[6], v[5], norm);
		      trivert[0] = v[7];
		      trivert[1] = v[6];
		      trivert[2] = v[5];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
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

		  for(i = 0; i < NPPEN; i++)
		    {
		      /* Temperature values */
		      vert[ivert].val = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		      v[i] = vert[ivert];
		      ivert++;
		    }

		  
		  if (g_element[ivol].nachbar[0] != Stoffnr)
		    {
		      /* 1 side Pentaedar---------------------------*/
		      calcNormFace(v[0], v[3], v[1], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[3];
		      trivert[2] = v[1];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[1], v[3], v[4], norm);
		      trivert[0] = v[1];
		      trivert[1] = v[3];
		      trivert[2] = v[4];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[1] != Stoffnr)
		    {
		      /* 2 side Pentaedar---------------------------*/
		      calcNormFace(v[1], v[4], v[2], norm);
		      trivert[0] = v[1];
		      trivert[1] = v[4];
		      trivert[2] = v[2];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[2], v[4], v[5], norm);
		      trivert[0] = v[2];
		      trivert[1] = v[4];
		      trivert[2] = v[5];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[2] != Stoffnr)
		    {
		      /* 3 side Pentaedar---------------------------*/
		      calcNormFace(v[2], v[5], v[0], norm);
		      trivert[0] = v[2];
		      trivert[1] = v[5];
		      trivert[2] = v[0];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		      calcNormFace(v[0], v[5], v[3], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[5];
		      trivert[2] = v[3];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[4] != Stoffnr)
		    {
		      /* 4 side Pentaedar---------------------------*/
		      calcNormFace(v[0], v[1], v[2], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[1];
		      trivert[2] = v[2];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[5] != Stoffnr)
		    {
		      /* 5 side Pentaedar---------------------------*/
		      calcNormFace(v[5], v[4], v[3], norm);
		      trivert[0] = v[5];
		      trivert[1] = v[4];
		      trivert[2] = v[3];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
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
		  for(i = 0; i < NPTET; i++)
		    {
		      /* Temperature values */
		      vert[ivert].val = &g_loesung.temp[g_element[ivol].pn[i] - 1];
		      for( j = 0; j < 3; j++){
			vert[ivert].xyz[j] = &g_knoten[g_element[ivol].pn[i] - 1].koord[j];
		      }
		      v[i] = vert[ivert];
		      ivert++;
		    }

		
		  if (g_element[ivol].nachbar[0] != Stoffnr)
		    {
		      /* 1 side Tetraedar---------------------------*/
		      calcNormFace(v[0], v[3], v[2], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[3];
		      trivert[2] = v[2];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[1] != Stoffnr)
		    {  
		      /* 2 side Tetraedar---------------------------*/
		      calcNormFace(v[1], v[2], v[3], norm);
		      trivert[0] = v[1];
		      trivert[1] = v[2];
		      trivert[2] = v[3];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[2] != Stoffnr)
		    {
		      /* 3 side Tetraedar---------------------------*/
		      calcNormFace(v[0], v[2], v[1], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[2];
		      trivert[2] = v[1];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		  if (g_element[ivol].nachbar[3] != Stoffnr)
		    {
		      /* 4 side Tetraedar---------------------------*/
		      calcNormFace(v[0], v[1], v[3], norm);
		      trivert[0] = v[0];
		      trivert[1] = v[1];
		      trivert[2] = v[3];
		      ntri  = newelem(id++, trivert, norm);
		      listp = addend(listp, ntri);
		    }
		}
	    }
	}
	
      /* update old list of materials */
      oldmat[Stoffnr-1].zeichne_on = g_stoffgebiete[Stoffnr-1].zeichne_on;
	  
    }
  /* how many triangles */
  *numtri = id;
  /* give back the list of triangles */
  return listp;
}


/************************************************************************
 This function build new sgi object.
************************************************************************/
sgiobj *build_sgi(long numtri, Tri *listp)
{
  sgiobj *obj;
  float v[3][3];    /* vertices          */
  float *n0,*n1,
    *n2;            /* normals           */
  float* fptr;      /* pointer to data   */
  int i, j;
  int itri;         /* index of triangle */
  GLubyte *col[3];
  float c[3][3];

  /* build object */
  obj  = (sgiobj *)newtriobj(numtri);
  fptr = (float *)obj->data;
  /* thru all triangles in list */
  for(itri = 0; itri < numtri; itri++) {
    listp = lookup(listp, itri);
    /* get vertices */
    for(i = 0; i < 3; i++) {
      v[0][i] = *(listp->vert[0].xyz[i]);
      v[1][i] = *(listp->vert[1].xyz[i]);
      v[2][i] = *(listp->vert[2].xyz[i]);
    }
    /* get normals */
    n0 = listp->norm;
    n1 = listp->norm;
    n2 = listp->norm;

    /* get temperatures (colors) */
    for(i = 0; i < 3; i++) {
      col[i] = Ccol(*(listp->vert[i].val));
      for(j = 0; j < 3; j++) {
	    c[i][j] = (float)(col[i][j] / 255.0);
      }
    }
    puttri(fptr, v[0], v[1], v[2], n0, n1,  n2,
	   c[0], c[1], c[2]);
    fptr += 3*PNTLONGS;
  }
  return obj;
} 	

/************************************************************************
 This function add new data for every triangle.
************************************************************************/
void puttri(float *fptr, float *p0, float *p1, float *p2, float *n0, 
	    float *n1, float *n2, float *c0, float *c1, float *c2)
{

  *fptr++ = n0[0];
  *fptr++ = n0[1];
  *fptr++ = n0[2];

  *fptr++ = c0[0];
  *fptr++ = c0[1];
  *fptr++ = c0[2];

  *fptr++ = p0[0];
  *fptr++ = p0[1];
  *fptr++ = p0[2];

  *fptr++ = n1[0];
  *fptr++ = n1[1];
  *fptr++ = n1[2];

  *fptr++ = c1[0];
  *fptr++ = c1[1];
  *fptr++ = c1[2];

  *fptr++ = p1[0];
  *fptr++ = p1[1];
  *fptr++ = p1[2];

  *fptr++ = n2[0];
  *fptr++ = n2[1];
  *fptr++ = n2[2];

  *fptr++ = c2[0];
  *fptr++ = c2[1];
  *fptr++ = c2[2];

  *fptr++ = p2[0];
  *fptr++ = p2[1];
  *fptr++ = p2[2];
}

/************************************************************************
 This function draws and builds sgo object.
*************************************************************************/
int draw_object(GLboolean buildlist, char *file)
{
  int numtri;
  int how;
  Tri *listtri = NULL;
  sgiobj *obj;
	
  /* build the list */
  if(buildlist) {
    listtri = btrilistElem(listtri, &numtri);
    buildlist = buildlist;
    obj = build_sgi(numtri, listtri);
    
    /* dump the sgo file */
    writesgiobj(file, obj);
  }

  /* draw the list */ 
  how = DRAW_POINTS|DRAW_COLORS|DRAW_NORMALS;
  drawsgiobj(obj,how);
  return 0;
}

/************************************************************************
 This function reads and draw sgo object inside color3d.
************************************************************************/
int read_object(char *file)
{
  sgiobj *obj;
  
  /* read sgo object */
  obj = readsgiobj(file);
  g_sgiobj.object = tmeshobj(obj);
  
  /* how to draw the sgo object */
  g_sgiobj.how = DRAW_POINTS|DRAW_COLORS|DRAW_NORMALS;
  g_sgiobj.gelesen = TRUE;
  return 0;
}

/************************************************************************
 This function returns normals of the given triangle.
 
 Parameters:
 ===========
         Vert v0,v1,v2, - vertices of the triangle.
	 
 Returns:
 ========
        GLfloat* norm - return the values of the normal 
	                on the face of triangle.
************************************************************************/									 
static void calcNormFace(Vert v0, Vert v1, Vert v2, GLfloat *norm)
{
  t_koord v, u, t;      /* vertices of the triangle      */
  GLfloat d1[3],d2[3];  /* two vectors for cross product */
  int i;                /* counter                       */

  /* calculate cross product */
  for(i = 0; i < 3; i++){
    v[i] = *(v0.xyz[i]);
    u[i] = *(v1.xyz[i]);
    t[i] = *(v2.xyz[i]);
  }
  calvec(v, u, t, d1, d2);
  crossprod(d1, d2, norm);
}
