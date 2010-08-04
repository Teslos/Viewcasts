/* This is edge algorithm for cutted material */
#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "grafik.h"

#define IS_NOT_CUTTED(elem,knot) \
(!(((elem)->node[(knot)].flag=='S') || ((elem)->node[(knot)].flag=='G')))
#define IS_CUTTED(elem,knot) \
((((elem)->node[(knot)].flag=='S') || ((elem)->node[(knot)].flag=='G')))

#define MAXELEM  16
enum
{
  NOT_CUTTED,
  CUTTED = 2,
  TANGENT,
};

enum
{
  HEXA = 8,
  PENTA = 6,
  TETA = 4
};

enum 
{
  BLACK = 1,
  WHITE,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PINK,
  CYAN
};

static int bound_elem (int elem_num, int *bfaces, int *nf);
static void getcoordcut (int point, t_koord xyz);
static void setcoloredge (int color);
static GLuint edge_list;
int edgecolor;

void
find_edges (int Stoffnr)
{
  Array_T cutelem, belem;
  int i;			/* counter */
  int maxelem;			/* max number of elements */
  int nelem = 0;		/* current number of elements */
  int nbelem = 0;		/* current number of boundary elements */
  int nf = 0;                   /* number of cutted boundary faces for one elem.*/
  int nbf = 0;                  /* number of cutted boundary faces */
  int nonbound = 0;             /* current number of non-boundary elements */
  t_arbeits_element *elem;	/* current working element */
  int bfaces[] = { -1, -1, -1, -1, -1, -1 };
  t_koord xyz0, xyz1;
  int ncut, knot, l;
  int cut_nodes[100];

  maxelem = MAXELEM;
  cutelem = Array_new (maxelem, sizeof (int));

  /* Find all cutted elements of one material */
  for (i = g_stoffgebiete[Stoffnr-1].anfang_tet; 
       i <= g_stoffgebiete[Stoffnr-1].ende_tet ; i++)	/* for all elements */
    {
      if (g_element[i].zeichnen == CUTTED)
	{
	  if (nelem >= maxelem)
	    {
	      Array_resize (cutelem, maxelem * 2);
	      maxelem *= 2;
	    }
	  Array_put (cutelem, nelem, (void *) &i);
	  nelem++;
	}
    }

  for (i = g_stoffgebiete[Stoffnr-1].anfang_hex; 
       i <= g_stoffgebiete[Stoffnr-1].ende_hex ; i++)	/* for all elements */
    {
      if (g_element[i].zeichnen == CUTTED)
	{
	  if (nelem >= maxelem)
	    {
	      Array_resize (cutelem, maxelem * 2);
	      maxelem *= 2;
	    }
	  Array_put (cutelem, nelem, (void *) &i);
	  nelem++;
	}
    }

  for (i = g_stoffgebiete[Stoffnr-1].anfang_pen; 
       i <= g_stoffgebiete[Stoffnr-1].ende_pen; i++)	/* for all elements */
    {
      if (g_element[i].zeichnen == CUTTED)
	{
	  if (nelem >= maxelem)
	    {
	      Array_resize (cutelem, maxelem * 2);
	      maxelem *= 2;
	    }
	  Array_put (cutelem, nelem, (void *) &i);
	  nelem++;
	}
    }

  /* Find in list of cutted elements all elements which are boundary
   * elements.
   */
  belem = Array_new (maxelem, sizeof (int));
  maxelem = MAXELEM;
 
  for (i = 0; i < nelem; i++)
    {
      int elem_num = *((int *) Array_get (cutelem, i));
      /* test if is boundary element */
      if (bound_elem (elem_num, bfaces, &nf))
	{
	  if (nbelem >= maxelem)
	    {
	    Array_resize (belem, maxelem * 2);
	  	maxelem *= 2;
	    }
	  Array_put (belem, nbelem, (void *) &elem_num);
	  nbelem++;
	}
      else
	{
	  /* count how many non boundary elements */
	  nonbound++;
	}

    }

  /* start display list */
  edge_list = glGenLists (1);
#ifdef DEBUG
  printf("Creating edge list %i\n", edge_list);
#endif
  glNewList (edge_list, GL_COMPILE);

  /* Find nodes which lies on boundary surface of element. */
#ifdef DEBUG 
  printf("Number of cutted elements: %i\n", nelem);
  printf("Number of cutted boundary elements: %i\n",nbelem);
  printf("Number of cutted non-boundary elements: %i\n",nonbound);
#endif 
  for (i = 0; i < nbelem; i++)
    {
      int f;
      int elem_num = *((int *) Array_get (belem, i));
      elem = g_element[elem_num].cut;
      ncut = 0;
      for (knot = 0; knot < elem->anz_node; knot++)
	{
	  if (IS_CUTTED (elem, knot))
	    {
	      cut_nodes[ncut] = knot;
	      ncut++;
	    }
	}

      bound_elem (elem_num, bfaces, &nf);

      /* go around all boundary faces */
      for (f = 0; f < nf; f++)
	{
	  int testk, k;
	  nbf++;

	  /* Go around cutted or tangent node */
	  for (k = 0; k < ncut; k++ )
	    {
	      knot = cut_nodes[k];
	      for (testk = 0; testk < 3; testk++)
		{
		  /*---------------------- IMPORTANT ---------------------------*
                   * We know that first face is allways smaller number, so      *
                   * we test if this is cut plane. Cut plane is always 1.       *
                   * To know boundary faces we must transform numbers from      *
                   * normal element to cutted element. Correspondence betweem   *
                   * original element faces and cutted element faces is in array*
                   * hflaeche.                                                  *
                   *------------------------------------------------------------*/
		  int nf0,nf1;
		  int f0   = elem->node[knot].kante[testk].flaeche[0];
		  int f1   = elem->node[knot].kante[testk].flaeche[1];
		  int next = elem->node[knot].kante[testk].nachbar;
		  nf0      = elem->node[next].kante[testk].flaeche[0];
		  nf1      = elem->node[next].kante[testk].flaeche[1];
		  
		  if ((f0 == elem->hflaeche[0])
		      && (f1 == elem->hflaeche[bfaces[f]]))
		    {
		      int p0, p1, tk=0;
				     		     
		      if (IS_CUTTED (elem, next))
			{
			  p1 = elem->node[next].punktnr;
			  p0 = elem->node[knot].punktnr;
			  if (p1 != p0)
			    {
			      getcoordcut (p0, xyz0);
			      getcoordcut (p1, xyz1);
			      glBegin (GL_LINES);
			      glVertex3fv (xyz0);
			      glVertex3fv (xyz1);
			      glEnd ();
			    }
			}
		    }
		}
	    }
	}
    }
#ifdef DEBUG
  printf("Num boundary faces: %i\n", nbf);
#endif
  /* end display list */
  glEndList ();
  
  /* Free memory */
  Array_free (&cutelem);
  Array_free (&belem);
}

static void
getcoordcut (int point, t_koord xyz)
{
  int tmp;
  int p = point;

  tmp = p - g_geodat.npunkt;
  if (tmp > 0)
    {
      xyz[0] = g_schnittpunktliste[tmp - 1].x;
      xyz[1] = g_schnittpunktliste[tmp - 1].y;
      xyz[2] = g_schnittpunktliste[tmp - 1].z;
    }
  else				/* or tangent node */
    {
      xyz[0] = g_knoten[p - 1].koord[0];
      xyz[1] = g_knoten[p - 1].koord[1];
      xyz[2] = g_knoten[p - 1].koord[2];

    }

}


/* Draw all found nodes as polyline (display list) */
void
draw_polyline ()
{
#ifdef DEBUG
  printf("Drawing edge: %i\n",edge_list);
#endif 
  glLineWidth(2.0);
  setcoloredge( edgeinfo.edgecolor );
  glCallList (edge_list);
  glLineWidth(1.0);
  glColor3f(1.0,1.0,1.0);
  glFlush ();
}

/* boudary element */
static int
bound_elem (int ibound, int *bfaces, int *nf)
{
  int i, f;			/* counter */
  t_element *e;			/* current element */
  int nfaces;			/* number of faces for element */
  t_bool bf = FALSE;            /* flag for boundary element */
  t_bool penta = FALSE;         /* flag for penta */

  e = &g_element[ibound];

  switch (e->anz_knoten)
    {
    case HEXA:
      nfaces = 6;
      break;
      /* I know, I know penta has five faces, 
       * but in geop we have six entries for faces of penta.
       */
    case PENTA:
      nfaces = 6;
      penta = TRUE;
      break;
    case TETA:
      nfaces = 4;
      break;
    default:
      fprintf (stderr, "Unknown element number\n");
      break;
    }

  f = 0;
  for (i = 0; i < nfaces; i++)
    {
      if (penta && i == 4)       /* if is penta and fifth element, jump over it */
      	continue;
      if (e->nachbar[i] != e->stoff_nr)
	{
	  bfaces[f] = i+1;
	  f++;
	  bf = TRUE;
	}
    }

  *nf = f;                      /* number of bound. faces */
  if (bf)
    return 1;			/* this boundary element */
  else
    return 0;                   /* guess, not boundary face */
}

static void
setcoloredge( int color )
{
  GLubyte *col;

  switch ( color ) 
    {
    case BLACK:
      col = RGB_Schwarz;
      break;
    case WHITE:
      col = RGB_Weiss;
      break;
    case RED:
      col = RGB_Rot;
      break;
    case GREEN:
      col = RGB_Gruen;
      break;
    case BLUE:
      col = RGB_Blau;
      break;
    case YELLOW:
      col = RGB_Gelb;
      break;
    case PINK:
      //col = RGB_Pink;
      break;
    case CYAN:
      col = RGB_Cyan;
      break;
    default:
      fprintf(stderr, "setcoloredge: not valid color\n");
      break;
    }
      
  glColor3ubv( col );
}

void
debug_hook (int num)
{
  int k;
  t_arbeits_element *e;
  e = g_element[num].cut;
  for (k = 0; k < e->anz_node; k++)
    printf ("Node number: %i\n", e->node[k].punktnr);
  printf ("How many nodes: %i\n", e->anz_node);
  printf ("How many faces: %i\n", e->anz_area);

}
