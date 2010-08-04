/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   06.01.97                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname        | Funktion                                */
/* ------------------------------------------------------------- */
/* zeichne_stoff        | zeichnet Stoff Solid-Darstellung       */
/* zeichne_gitterstoff  | zeichnet Stoff Gitter-Darstellung      */
/* zeichne_result       | zeichnet Stoff Result-Darstellung      */
/* pick_Knoten          | waehlt aus einem Stoff einen Knoten    */
/* free_pick            | waehlt aus einem Stoff einen Knoten    */
/* Hilfsprozeduren      | zeichne_named_Knoten und finde_Knoten  */
/* Ccol                 | waehlt Farbe aus RGB_Color[2000][3]    */
/* zeichne_schnitt_elem | Hilfsprozedur fuer Schnittelemente     */
/* ------------------------------------------------------------- */
/* ============================================================= */
/* Adding support for the VertexArrays    13.10.2001             */
/* Includes :                                                    */
/* ==========                                                    */
#include <GL/gl.h>
#include <GL/glu.h>
#include "grafik.h"
#include "color3d.h"
#include "paulslib.h"
#include "sgiobj.h"
#include "iso.h"
#include "mem.h"
#include "errs.h"
#include "utilh.h"
#include "common.h"

#ifndef CALLBACK
#define CALLBACK
#endif

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*t_bool printok;   *//* Flag fuer Debug-Ausgabe ------------- */
t_color g_tcmap[MAX_COL];
int g_itri;
t_cvar *c3d_tex_raster;
t_cvar *c3d_tex_default;
t_cvar *c3d_tex_stripes;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

static void calcNormFace (Vert v0, Vert v1, Vert v2, GLfloat * norm);
void puttri (float *fptr, float *p0, float *p1, float *p2, float *n0,
	     float *n1, float *n2, float *c0, float *c1, float *c2);
Tri *btrilistElem (Tri * listp, long *ntri);
sgiobj *build_sgi (long numtri, Tri * listp);

/*#define PRINTOUT */
#ifndef PRINTOUT
#define printok  FALSE
#else
#define printok  TRUE
#endif
GLubyte colorTex[4];
static GLfloat texCoord1D;
static sgiobj *obj;
t_texture_info g_tex_info;

/* ********************************************************************* */
GLubyte *
Ccol (float tmp)		/* Bestimmung der Farbe zum Parameterwert t[i] */
/* ********************************************************************* */
{
  GLubyte *pcol;
  int col, i;
  int tcol;
  static flag = 0;
  static GLubyte *color = NULL;
  COLOUR c;			/* value of the color r,g,b */
  int icolmap = 1;		/* index of the colormap */
  FILE *fd;
/* ********************************************************************* */
  tcol = g_color_extra.color_name;
/* Allocate space for color ******************************************** */
  if (color == NULL)
    color = (GLubyte *) malloc (sizeof (GLubyte) * 3);
/* Welche Farbdarstellung haben wir ? ---------------------------------- */
  g_tabelle.art = 1;
  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))	/* 24-Bit + Default-Belegung */
    {
      if (tmp < g_loesung.Min)
	{
	  color[0] = (GLubyte) g_tabelle.colortable[1];
	  color[1] = (GLubyte) g_tabelle.colortable[2];
	  color[2] = (GLubyte) g_tabelle.colortable[3];
	  return (color);
	}
      if (tmp > g_loesung.Max)
	{
	  color[0] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 1];
	  color[1] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 2];
	  color[2] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 3];
	  return (color);
	}

      col = (int) ((tmp - g_loesung.Min) * ((float) COLORTABLE_LENGTH /
					    (g_loesung.Max - g_loesung.Min)));


      color[0] = (GLubyte) g_tabelle.colortable[4 * col + 1];
      color[1] = (GLubyte) g_tabelle.colortable[4 * col + 2];
      color[2] = (GLubyte) g_tabelle.colortable[4 * col + 3];

      /* if color map is hot to cold */
      if (tcol)
	{
	  c = GetColour (tmp, g_loesung.Min, g_loesung.Max, tcol);
	  color[0] = (GLubyte) (c.r * 255);
	  color[1] = (GLubyte) (c.g * 255);
	  color[2] = (GLubyte) (c.b * 255);
	}


    }
  else				/* Alle anderen Faelle haben Farbdaten in 128 Pixeln ---------- */
    {
      /*
         if (tmp < g_loesung.Min) return(ACHT_BIT_TEMP_OFFSET);
         if (tmp > g_loesung.Max) return(ACHT_BIT_COLORTABLE_LENGTH-1
         +ACHT_BIT_TEMP_OFFSET);
       */
      col = (int) ((tmp - g_loesung.Min) *
		   ((float) ACHT_BIT_COLORTABLE_LENGTH / (g_loesung.Max -
							  g_loesung.Min)) +
		   ACHT_BIT_TEMP_OFFSET);

    }
  /* **************** ende Ccol ***************************************** */
  return (color);
}

/* ********************************************************************* */
float
schnittemp (int punktnr)
/* ********************************************************************* */
{
/* ********************************************************************* */
  double t1, t2, tdiff;		/* Temp. auf Endpunkten der Kante */
  int p1, p2;
/* ********************************************************************* */
  /* Temperatur der Originalpunkte  */
  t1 = (double) g_loesung.temp[g_schnittpunktliste[punktnr - 1].p1 - 1];
  t2 = (double) g_loesung.temp[g_schnittpunktliste[punktnr - 1].p2 - 1];
  tdiff = t2 - t1;		/* Temperatur am Schnittpunkt --- */
  return (float) (t1 + tdiff * g_schnittpunktliste[punktnr - 1].verhaeltnis);
}

void CALLBACK
beginCallback (GLenum which)
{
  glBegin (which);
}

void CALLBACK
errorCallback (GLenum errorCode)
{
  const GLubyte *estring;

  estring = gluErrorString (errorCode);
  fprintf (stderr, "Tessellation Error: %s\n", estring);
}

void CALLBACK
endCallback (void)
{
  glEnd ();
}

void CALLBACK
vertexCallback (GLvoid * vertex)
{
  const GLdouble *pointer;

  pointer = (GLdouble *) vertex;
  glNormal3dv (pointer + 3);

  if (g_color_extra.texture_on)
    glTexCoord1d (*(pointer + 6));
  else
    glColor3dv (pointer + 6);

  glVertex3dv (vertex);
}

void CALLBACK
combineCallback (GLdouble coords[3],
		 GLdouble * vertex_data[4], GLfloat weight[4], GLdouble ** dataOut)
{
  GLdouble *vertex;
  int i;

  vertex = (GLdouble *) malloc (6 * sizeof (GLdouble));
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = coords[2];

  for (i = 3; i < 6; i++)
    vertex[i] = weight[0] * vertex_data[0][i]
      + weight[1] * vertex_data[1][i]
      + weight[2] * vertex_data[2][i] + weight[3] * vertex_data[3][i];

  *dataOut = vertex;
}


/* ************************************************************* */
int
zeichne_schnitt_elem (int ivol, GLuint zeichenmode, t_bool result)
/* ************************************************************* */
{
  int akt_flaeche;
  int akt_knoten;
  int akt_kante;
  int startknoten;
  int vorgaenger;
  int nachfolger;
  int testknoten;
  int testkante;
  int startkante;
  int p;
  float t;
  int i, j, n[3];
  t_bool suchflag;
  t_bool zeichenflag;
  t_bool loopflag;
  t_koord xyz;
  t_koord d[3];
  GLfloat dis1[3], dis2[3];
  GLfloat norm[3];
  GLfloat verts[20][3];
  int nvert = 0;
  float plane[4];
  GLubyte col[20][3], *tmpcol;
  GLfloat texcoord[20];


  t_arbeits_element *elem;
  elem = g_element[ivol].cut;


  if (printok)
    printf ("\nArbeitselement: %d", elem->anz_node);

  /* schleife ueber alle Flaechen: ----------------------------------- */
  for (akt_flaeche = 1; akt_flaeche <= elem->anz_area; akt_flaeche++)
    {
      if (printok)
	printf ("\nSuche Knoten an Flaeche %d", akt_flaeche);

      /* Finde ersten Knoten, der an Flaeche akt_flaeche liegt: ---------- */
      testknoten = 0;
      suchflag = TRUE;

      while (suchflag)
	{
	  while ((elem->node[testknoten].flag == 'W') ||
		 (elem->node[testknoten].flag == 'G'))
	    {
	      testknoten++;
	    }
	  startkante = 0;
	  for (testkante = 0; testkante < 3; testkante++)
	    {
	      if ((elem->node[testknoten].kante[testkante].flaeche[0] ==
		   akt_flaeche)
		  || (elem->node[testknoten].kante[testkante].flaeche[1] == akt_flaeche))
		{
		  suchflag = FALSE;
		  startkante = testkante;
		}
	    }
	  if (suchflag)
	    testknoten++;
	}

      startknoten = testknoten;
      vorgaenger = startknoten;
      akt_knoten = startknoten;

      loopflag = TRUE;
      i = 0;
      while (loopflag)
	{
	  if (printok)
	    printf ("\nAktueller Knoten: %d, point number: %d", akt_knoten,
		    elem->node[akt_knoten].punktnr);
	  if (elem->node[akt_knoten].punktnr > g_geodat.npunkt)
	    {
	      if (printok)
		printf ("\nSchnittpunkt");
	      p = elem->node[akt_knoten].punktnr - g_geodat.npunkt;

	      if (result)
		{
		  if (g_color_extra.texture_on)
		    texcoord[i] = CcolTex (schnittemp (p));
		  else
		    {
		      tmpcol = Ccol (schnittemp (p));
		      col[i][0] = tmpcol[0];
		      col[i][1] = tmpcol[1];
		      col[i][2] = tmpcol[2];
		    }
		}
	      xyz[0] = g_schnittpunktliste[p - 1].x;
	      xyz[1] = g_schnittpunktliste[p - 1].y;
	      xyz[2] = g_schnittpunktliste[p - 1].z;
	      verts[i][0] = xyz[0];
	      verts[i][1] = xyz[1];
	      verts[i][2] = xyz[2];
	    }
	  else
	    {
	      if (printok)
		printf ("\nOriginalpunkt");
	      p = elem->node[akt_knoten].punktnr;

	      if (result)
		{
		  t = g_loesung.temp[p - 1];
		  if (g_color_extra.texture_on)
		    {
		      myinit (TEX_DEFAULT, FALSE);
		      texcoord[i] = CcolTex (t);
		    }
		  else
		    {
		      tmpcol = Ccol (t);
		      col[i][0] = tmpcol[0];
		      col[i][1] = tmpcol[1];
		      col[i][2] = tmpcol[2];
		    }
		}
	      xyz[0] = g_knoten[p - 1].koord[0];
	      xyz[1] = g_knoten[p - 1].koord[1];
	      xyz[2] = g_knoten[p - 1].koord[2];
	      verts[i][0] = xyz[0];
	      verts[i][1] = xyz[1];
	      verts[i][2] = xyz[2];
	    }

	  suchflag = TRUE;
	  akt_kante = 0;
	  while (suchflag)
	    {
	      if (elem->node[akt_knoten].kante[akt_kante].nachbar != vorgaenger)
		{
		  if ((elem->node[akt_knoten].kante[akt_kante].flaeche[0] ==
		       akt_flaeche)
		      || (elem->node[akt_knoten].kante[akt_kante].
			  flaeche[1] == akt_flaeche))
		    {
		      nachfolger = elem->node[akt_knoten].kante[akt_kante].nachbar;
		      suchflag = FALSE;
		    }
		}
	      akt_kante++;

	      if (akt_kante > 3)
		{
		  suchflag = FALSE;
		  loopflag = FALSE;
		}

	    }			/* ENDE while (suchflag) */

	  vorgaenger = akt_knoten;
	  akt_knoten = nachfolger;

	  if (akt_knoten == startknoten)
	    loopflag = FALSE;
	  i++;
	  nvert = i;
	}			/* ENDE while (loopflag) */

      newell (verts, nvert, plane);
      glBegin (zeichenmode);
      for (j = 0; j < nvert; j++)
	{
	  glNormal3fv (plane);

	  if (g_color_extra.texture_on)
	    glTexCoord1f (texcoord[j]);
	  else
	    glColor3ubv (col[j]);

	  glVertex3fv (verts[j]);
	}
      glEnd ();
      glColor3ubv (RGB_Weiss);

    }
  /* End Schleife ueber alle Flaechen --------------------------------- */

  return (0);
  /* *************************** ende zeichne_schnitt_elem *************** */
}


int
zeichne_named_schnittelement (int element_index)
{
  t_koord k;
  int i, p_index, p;
  t_arbeits_element *elem;

  elem = g_element[element_index].cut;
  for (i = 0; i < elem->anz_node; i++)
    {
      if ((elem->node[i].flag == 'S') || (elem->node[i].flag == 'D'))
	{
	  p = elem->node[i].punktnr;
	  if (p > g_geodat.npunkt)
	    {
	      p_index = p - g_geodat.npunkt - 1;
	      k[0] = g_schnittpunktliste[p_index].x;
	      k[1] = g_schnittpunktliste[p_index].y;
	      k[2] = g_schnittpunktliste[p_index].z;
	    }
	  else
	    {
	      p_index = p - 1;
	      k[0] = g_knoten[p_index].koord[0];
	      k[1] = g_knoten[p_index].koord[1];
	      k[2] = g_knoten[p_index].koord[2];
	    }
	  glLoadName (p);
	  glBegin (GL_POINTS);
	  glVertex3fv (k);
	  glEnd ();
	}
    }
  return (0);
}

/* ********************************************************************* */
int
zeichne_stoff (int Stoffnr)	/* erzeugt Solidmodell : Stoff Stoffnr */
/* ********************************************************************* */
{
  int i, j,			/* Zaehlvariablen ------------------------- */
    nummer,			/* Nr fuer geschnittenes El. -------------- */
    ivol;			/* Zaehler fuer Elemente ------------------ */
  t_bool verschiebe_on = FALSE;	/* Flagge f. Verschiebung d. Stoffes  */
  t_koord p1_koord,		/* Punktkoordinaten ----------------------- */
    p2_koord, p3_koord, p4_koord, p5_koord, p6_koord, p7_koord, p8_koord;

  GLfloat d1[3], d2[3];		/* two vector from which we cal. normal on the surf. */
  GLfloat norm1[3], norm2[3], norm3[3], norm4[3], norm5[3], norm6[3];

  /* ***************************************************************** */
  /* Erzeugen der Solidmodelle --------------------------------------- */

  /* Verschiebung einzelner Stoffe ----------------------------------- */
  if (g_stoffgebiete[Stoffnr - 1].verschiebung[0] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[1] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[2])
    {
      glTranslatef (g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[2]);
      verschiebe_on = TRUE;
    }

  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, -g_ofen.akt_position);



  /* HEXAEDER -------------------------------------------------------- */
  /* Schleife ueber Elemente ----------------------------------------- */
  glBegin (GL_QUADS);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_hex;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_hex; ivol++)
    {
      /* Element zeichnen --------------------------------------------- */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. El. */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p7_koord[0] = g_knoten[g_element[ivol].pn[6] - 1].koord[0];
	  p7_koord[1] = g_knoten[g_element[ivol].pn[6] - 1].koord[1];
	  p7_koord[2] = g_knoten[g_element[ivol].pn[6] - 1].koord[2];

	  p8_koord[0] = g_knoten[g_element[ivol].pn[7] - 1].koord[0];
	  p8_koord[1] = g_knoten[g_element[ivol].pn[7] - 1].koord[1];
	  p8_koord[2] = g_knoten[g_element[ivol].pn[7] - 1].koord[2];



	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* calculate normal for 1 side of hexaedar */
	      calvec (p1_koord, p5_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Hexa-eder ------ */
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* calculate normal for 2 side of hexaedar */
	      calvec (p2_koord, p6_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Hexa-eder ------ */
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* calculate normal for 3 side of hexaedar */
	      calvec (p3_koord, p7_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Hexa-eder ------ */
	      glNormal3fv (norm3);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      /* calculate normal for 4 side of hexaedar */
	      calvec (p4_koord, p8_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Hexa-eder ------ */
	      glNormal3fv (norm4);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p1_koord);
	    }

	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Hexa-eder ------ */
	      glNormal3fv (norm5);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p4_koord);
	    }
	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* calculate normal for 6 side of hexaedar */
	      calvec (p5_koord, p8_koord, p6_koord, d1, d2);
	      crossprod (d1, d2, norm6);

	      /* 6. Seite Hexa-eder ------ */
	      glNormal3fv (norm6);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p6_koord);
	    }
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  /* end the previous drawing */
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	  /* start new drawing */
	  glBegin (GL_QUADS);
	}			/* End else */
    }				/* end Schleife ueber HEXAEDER ---------- */
  glEnd ();

  /* PENTAEDER --------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_pen;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_pen; ivol++)
    {
      /* Element zeichnen -------------------------------------------- */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  glBegin (GL_QUADS);
	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* calculate normal for 1 side of hexaedar */
	      calvec (p1_koord, p4_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Penta-eder ---- */
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p2_koord);
	    }
	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* calculate normal for 2 side of hexaedar */
	      calvec (p2_koord, p5_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Penta-eder ---- */
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p3_koord);
	    }
	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* calculate normal for 3 side of hexaedar */
	      calvec (p3_koord, p6_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Penta-eder ---- */
	      glNormal3fv (norm3);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p1_koord);
	    }
	  glEnd ();

	  glBegin (GL_TRIANGLES);
	  /* Indexverschiebung f. Pentaeder */
	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* calculate normal for 4 side of hexaedar */
	      calvec (p1_koord, p2_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Penta-eder ---- */
	      glNormal3fv (norm4);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	    }
	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p6_koord, p5_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Penta-eder ---- */
	      glNormal3fv (norm5);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p4_koord);
	    }
	  glEnd ();
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	}			/* End else */
    }				/* end Schleife ueber PENTAEDER --------- */

  /* TETRAEDER---------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------- */
  glBegin (GL_TRIANGLES);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_tet;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_tet; ivol++)
    {
      /* Element zeichnen ------------------------------------------ */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      calvec (p1_koord, p4_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Tetra-eder ----- */
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      calvec (p2_koord, p3_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Tetra-eder ----- */
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      calvec (p1_koord, p3_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Tetra-eder ----- */
	      glNormal3fv (norm3);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Tetra-eder ----- */
	      glNormal3fv (norm4);
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p4_koord);
	    }
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_TRIANGLES);
	}			/* End else */

    }				/* end Schleife ueber TETRAEDER ------------- */
  glEnd ();

/* Verschiebung rueckgaengig machen ------------------------------ */
  if (verschiebe_on)
    glTranslatef (-g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[2]);

  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, g_ofen.akt_position);
/* **************** ende zeichne_stoff ********************************* */
  return (0);
}

/* ********************************************************************* */
int
zeichne_gitterstoff (int Stoffnr, int schnitt)	/* erzeugt Drahtmodell  */
/* ********************************************************************* */
{
  int nummer,			/* Nummer fuer geschnittenes Element ---------- */
    ivol;			/* Zaehler fuer Elemente ---------------------- */
  t_bool verschiebe_on = FALSE;	/* Flagge f. Verschiebung des Stoffes */

  t_koord p1_koord,		/* Punktkoorinaten ---------------------------- */
    p2_koord, p3_koord, p4_koord, p5_koord, p6_koord, p7_koord, p8_koord;
/* ********************************************************************* */
/* Erzeugen der Drahtmodelle --------------------------------------- */

/* Verschiebung einzelner Stoffe ----------------------------------- */
  if (g_stoffgebiete[Stoffnr - 1].verschiebung[0] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[1] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[2])
    {
      glTranslatef (g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[2]);
      verschiebe_on = TRUE;
    }



/* Absenken des Gitters nur bei Resultdarstellung mit Netz -------- */
  if ((g_Maske[Stoffnr] == 8) && g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, -g_ofen.akt_position);

  /* Schleife ueber Elemente ------------------------------------- */
  /* HEXAEDER ---------------------------------------------------- */
  glBegin (GL_QUADS);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_hex;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_hex; ivol++)
    {
      /* Element zeichnen ---------  */
      if ((((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on))
	   && (schnitt == 1)) || (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p7_koord[0] = g_knoten[g_element[ivol].pn[6] - 1].koord[0];
	  p7_koord[1] = g_knoten[g_element[ivol].pn[6] - 1].koord[1];
	  p7_koord[2] = g_knoten[g_element[ivol].pn[6] - 1].koord[2];

	  p8_koord[0] = g_knoten[g_element[ivol].pn[7] - 1].koord[0];
	  p8_koord[1] = g_knoten[g_element[ivol].pn[7] - 1].koord[1];
	  p8_koord[2] = g_knoten[g_element[ivol].pn[7] - 1].koord[2];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Hexa-eder -------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Hexa-eder -------- */
	      glVertex3fv (p2_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Hexa-eder -------- */
	      glVertex3fv (p3_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      /* 4. Seite Hexa-eder -------- */
	      glVertex3fv (p4_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p1_koord);
	    }

	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* 5. Seite Hexa-eder -------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* 6. Seite Hexa-eder ---------- */
	      glVertex3fv (p5_koord);
	      glVertex3fv (p8_koord);
	      glVertex3fv (p7_koord);
	      glVertex3fv (p6_koord);
	    }
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (schnitt == 1)
	      && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_QUADS);
	}			/* End else */
    }				/* end  Schleife ueber HEXAEDER ------------- */
  glEnd ();

  /* PENTAEDER --------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_pen;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_pen; ivol++)
    {
      /* Element zeichnen ---------  */
      if ((((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on))
	   && (schnitt == 1)) || (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  glBegin (GL_QUADS);
	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Penta-eder ------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Penta-eder ------- */
	      glVertex3fv (p2_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Penta-eder ------- */
	      glVertex3fv (p3_koord);
	      glVertex3fv (p6_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p1_koord);
	    }
	  glEnd ();

	  glBegin (GL_TRIANGLES);

	  /* Indexverschiebung f. Pentaeder */
	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* 4. Seite Penta-eder ------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* 5. Seite Penta-eder ------- */
	      glVertex3fv (p6_koord);
	      glVertex3fv (p5_koord);
	      glVertex3fv (p4_koord);
	    }
	  glEnd ();
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (schnitt == 1))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	}			/* End else */
    }				/* end Schleife ueber PENTAEDER --------- */

  /* TETRAEDER------------------------------------------------------ */
  /* Schleife ueber Elemente --------------------------------------- */
  glBegin (GL_TRIANGLES);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_tet;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_tet; ivol++)
    {
      /* Element zeichnen ---------  */
      if ((((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on))
	   && (schnitt == 1)) || (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Tetra-eder ------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p4_koord);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Tetra-eder ------- */
	      glVertex3fv (p2_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Tetra-eder ------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p3_koord);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      /* 4. Seite Tetra-eder ------- */
	      glVertex3fv (p1_koord);
	      glVertex3fv (p2_koord);
	      glVertex3fv (p4_koord);
	    }
	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (schnitt == 1))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, FALSE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_TRIANGLES);
	}			/* End else */
    }				/* end Schleife ueber TETRAEDER - ----------- */
  glEnd ();

/* Verschiebung rueckgaengig machen ------------------------------ */
  if (verschiebe_on)
    glTranslatef (-g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[2]);

/* Rueckgaengig machen der Absenken ------------------------------ */
  if ((g_Maske[Stoffnr] == 8) && g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, g_ofen.akt_position);
/* **************** ende zeichne_gitterstoff *************************** */
  return (0);
}

/* ******************************************************************** */
int
zeichne_result (int Stoffnr)	/* erzeugt Resultmodell : Stoff Stoffnr */
/* ******************************************************************** */
{
  int i,			/* Zaehlvariablen -------------------------------- */
    numtri,			/* Nr fuer triangle.------------------------------ */
    ivol;			/* Zaehler fuer Elemente ------------------------- */
  t_bool verschiebe_on = FALSE;	/* Flagge f. Verschiebung des Stoffes - */
  static t_bool ispass = TRUE;	/* flag for counting boundary elements  */
  float t1, t2, t3, t4, t5, t6, t7, t8;	/* Temperaturen der Punkte ------- */
  GLfloat norm1[3], norm2[3], norm3[3], norm4[3], norm5[3], norm6[3];
  GLfloat d1[3], d2[3];
  t_linie *iso;
  struct listelem *isoline;

  static GLboolean buildlist = GL_TRUE;

  t_koord p1_koord,		/* Punktkoorinaten ------------------------------ */
    p2_koord, p3_koord, p4_koord, p5_koord, p6_koord, p7_koord, p8_koord;
  int anzahl;
  int start;
  float faktor;

/* ******************************************************************** */
/* Erzeugen der Resultmodelle ----------------------------------------- */

  /* if number of material is equal to num. 
   * of materials don't count
   * boundary elements any more
   */
  if (Stoffnr == g_geodat.nstoff)
    ispass = FALSE;

  /* Verschiebung einzelner Stoffe -------------------------------------- */
  if (g_stoffgebiete[Stoffnr - 1].verschiebung[0] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[1] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[2])
    {
      glTranslatef (g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[2]);
      verschiebe_on = TRUE;
    }

  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    {
      glTranslatef (0.0, 0.0, -g_ofen.akt_position);
    }

  /* this is temporary solution ???? */
  /*if(!draw_object(buildlist)) return 0; */

  /* HEXAEDER --------------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------------ */
  glBegin (GL_QUADS);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_hex;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_hex; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && (g_transform.schneiden_on)) || (!g_transform.schneiden_on) || (g_dis_cutting[Stoffnr]))	/* Element zeichnen ------- */
	{
	  if (ispass)
	    g_itri++;		/* number of boundary elements */

	  /* Koordinaten der Elemente */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p7_koord[0] = g_knoten[g_element[ivol].pn[6] - 1].koord[0];
	  p7_koord[1] = g_knoten[g_element[ivol].pn[6] - 1].koord[1];
	  p7_koord[2] = g_knoten[g_element[ivol].pn[6] - 1].koord[2];

	  p8_koord[0] = g_knoten[g_element[ivol].pn[7] - 1].koord[0];
	  p8_koord[1] = g_knoten[g_element[ivol].pn[7] - 1].koord[1];
	  p8_koord[2] = g_knoten[g_element[ivol].pn[7] - 1].koord[2];

	  /* Temperaturen der Punkte -- */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];
	  t5 = g_loesung.temp[g_element[ivol].pn[4] - 1];
	  t6 = g_loesung.temp[g_element[ivol].pn[5] - 1];
	  t7 = g_loesung.temp[g_element[ivol].pn[6] - 1];
	  t8 = g_loesung.temp[g_element[ivol].pn[7] - 1];

	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      calvec (p1_koord, p5_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Hexa-eder -------- */
	      glNormal3fv (norm1);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of hexaedar */
	      calvec (p2_koord, p6_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Hexa-eder -------- */
	      glNormal3fv (norm2);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	      glColor3ubv (Ccol (t7));
	      glVertex3fv (p7_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of hexaedar */
	      calvec (p3_koord, p7_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm3);


	      /* 3. seite hexa-eder -------- */
	      glNormal3fv (norm3);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	      glColor3ubv (Ccol (t7));
	      glVertex3fv (p7_koord);
	      glColor3ubv (Ccol (t8));
	      glVertex3fv (p8_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of hexaedar */
	      calvec (p4_koord, p8_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Hexa-eder -------- */
	      glNormal3fv (norm4);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	      glColor3ubv (Ccol (t8));
	      glVertex3fv (p8_koord);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	    }

	  if (g_element[ivol].nachbar[4] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Hexa-eder -------- */
	      glNormal3fv (norm5);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p5_koord, p8_koord, p6_koord, d1, d2);
	      crossprod (d1, d2, norm6);

	      /* 6. Seite Hexa-eder -------- */
	      glNormal3fv (norm6);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t8));
	      glVertex3fv (p8_koord);
	      glColor3ubv (Ccol (t7));
	      glVertex3fv (p7_koord);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	    }

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_QUADS);
	}			/* End else */
    }				/* end Schleife ueber HEXAEDER ------------------ */
  glEnd ();

  /* PENTAEDER ------------------------------------------------------- */
  /* Schleife ueber Elemente ----------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_pen;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_pen; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && (g_transform.schneiden_on)) || (!g_transform.schneiden_on) || (g_dis_cutting[Stoffnr]))	/* Element zeichnen ------- */
	{
	  if (ispass)
	    g_itri++;
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  /* Temperaturen der Punkte ------ */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];
	  t5 = g_loesung.temp[g_element[ivol].pn[4] - 1];
	  t6 = g_loesung.temp[g_element[ivol].pn[5] - 1];

	  glBegin (GL_QUADS);
	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 1 side of pentaedar */
	      calvec (p1_koord, p4_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Penta-eder ------ */
	      glNormal3fv (norm1);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of pentaedar */
	      calvec (p2_koord, p5_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Penta-eder ------ */
	      glNormal3fv (norm2);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of pentaedar */
	      calvec (p3_koord, p6_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Penta-eder ------ */
	      glNormal3fv (norm3);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	    }
	  glEnd ();

	  glBegin (GL_TRIANGLES);

	  /* Indexverschiebung f. Pent. */
	  if (g_element[ivol].nachbar[4] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of pentaedar */
	      calvec (p1_koord, p2_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Penta-eder ------ */
	      glNormal3fv (norm4);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of pentaedar */
	      calvec (p6_koord, p5_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Penta-eder ------- */
	      glNormal3fv (norm5);
	      glColor3ubv (Ccol (t6));
	      glVertex3fv (p6_koord);
	      glColor3ubv (Ccol (t5));
	      glVertex3fv (p5_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);

	    }
	  glEnd ();

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	}			/* End else */

    }				/* end Schleife ueber PENTAEDER ----------------- */

  /* TETRAEDER-------------------------------------------------------- */
  /* Schleife ueber Elemente ----------------------------------------- */
  glBegin (GL_TRIANGLES);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_tet;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_tet; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && 
		(g_transform.schneiden_on)) || (!g_transform.schneiden_on) || g_dis_cutting[Stoffnr])	/* Element zeichnen */
	{
	  if (ispass)
	    g_itri++;
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  /* Temperaturen der Punkte ------ */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];

	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 1 side of pentaedar */
	      calvec (p1_koord, p4_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Tetra-eder ------- */
	      glNormal3fv (norm1);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of tetraedar */
	      calvec (p2_koord, p3_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Tetra-eder ------- */
	      glNormal3fv (norm2);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of pentaedar */
	      calvec (p1_koord, p3_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Tetra-eder ------- */
	      glNormal3fv (norm3);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t3));
	      glVertex3fv (p3_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of pentaedar */
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Tetra-eder ------- */
	      glNormal3fv (norm4);
	      glColor3ubv (Ccol (t1));
	      glVertex3fv (p1_koord);
	      glColor3ubv (Ccol (t2));
	      glVertex3fv (p2_koord);
	      glColor3ubv (Ccol (t4));
	      glVertex3fv (p4_koord);
	    }

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if ((g_element[ivol].zeichnen == 2) && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_TRIANGLES);
	}			/* End else */
    }				/* end Schleife ueber TETRAEDER ------------- */
  glEnd ();


  /* draw isolines */
  if (g_isolinien.linien != NULL)
    {
      for (isoline = g_isolinien.linien->head, i = 0; isoline != NULL;
	   isoline = isoline->next, i++)
	{
	  iso = (t_linie *) isoline->data;
	  if (iso->on)
	    {
	      int n = 0;
	      anzahl = (int) (faktor * iso->breit);
	      start = (int) (faktor * (iso->wert - g_loesung.Min)) - anzahl / 2;
	      g_listp[i] = btrilist (g_listp[i], iso->wert, i + 1);

#ifdef DEBUG
	      apply (g_listp[i], inccounter, &n);
	      printf ("%d elements in trilist %i\n", n, i);
#endif
	      applyiso (g_listp[i], interpvert, iso->wert, iso->nr);
	    }
	}
    }
/* Edges on ------------------------------------------------------ */
  if (edgeinfo.edge_on)
    draw_polyline ();

/* Verschiebung rueckgaengig machen ------------------------------ */
  if (verschiebe_on)
    glTranslatef (-g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[2]);

/* Absenkung rueckgaengig machen --------------------------------- */
  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, g_ofen.akt_position);

  return;
/* ****************** ende zeichne_result ****************************** */
}


/* ********************************************************************* */
int
zeichne_named_knoten (int Stoffnr)
/* ********************************************************************* */
{
  int nummer,			/* Nummer fuer geschnittenes Element ---------- */
    ivol;			/* Zaehler fuer Elemente ---------------------- */
  t_bool verschiebe_on = FALSE;	/* Flagge f. Verschiebung des Stoffes */
  t_koord p1_koord,		/* Punktkoorinaten ---------------------------- */
    p2_koord, p3_koord, p4_koord, p5_koord, p6_koord, p7_koord, p8_koord;
  int p1_name,			/* Punktnummern als Namen ---------------------- */
    p2_name, p3_name, p4_name, p5_name, p6_name, p7_name, p8_name;
/* ********************************************************************* */
/* Erzeugen der Drahtmodelle --------------------------------------- */

/* Verschiebung einzelner Stoffe ----------------------------------- */
  if (g_stoffgebiete[Stoffnr - 1].verschiebung[0] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[1] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[2])
    {
      glTranslatef (g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[2]);
      verschiebe_on = TRUE;
    }

  /* Schleife ueber Elemente ------------------------------------- */
  /* HEXAEDER ---------------------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_hex;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_hex; ivol++)
    {
/* Element zeichnen --------- */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p7_koord[0] = g_knoten[g_element[ivol].pn[6] - 1].koord[0];
	  p7_koord[1] = g_knoten[g_element[ivol].pn[6] - 1].koord[1];
	  p7_koord[2] = g_knoten[g_element[ivol].pn[6] - 1].koord[2];

	  p8_koord[0] = g_knoten[g_element[ivol].pn[7] - 1].koord[0];
	  p8_koord[1] = g_knoten[g_element[ivol].pn[7] - 1].koord[1];
	  p8_koord[2] = g_knoten[g_element[ivol].pn[7] - 1].koord[2];

	  p1_name = g_element[ivol].pn[0];
	  p2_name = g_element[ivol].pn[1];
	  p3_name = g_element[ivol].pn[2];
	  p4_name = g_element[ivol].pn[3];
	  p5_name = g_element[ivol].pn[4];
	  p6_name = g_element[ivol].pn[5];
	  p7_name = g_element[ivol].pn[6];
	  p8_name = g_element[ivol].pn[7];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Hexa-eder -------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Hexa-eder -------- */
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p7_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p7_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Hexa-eder -------- */
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p8_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p8_koord);
	      glEnd ();
	      glLoadName (p7_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p7_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      /* 4. Seite Hexa-eder -------- */
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	      glLoadName (p8_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p8_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* 5. Seite Hexa-eder -------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* 6. Seite Hexa-eder ---------- */
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	      glLoadName (p7_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p7_koord);
	      glEnd ();
	      glLoadName (p8_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p8_koord);
	      glEnd ();
	    }
	}			/* End if Elementzeichnen == 1 */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      zeichne_named_schnittelement (ivol);
	    }
	}			/* End else */
    }				/* end  Schleife ueber HEXAEDER ------------- */

  /* PENTAEDER --------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_pen;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_pen; ivol++)
    {
/* Element zeichnen ---------- */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p1_name = g_element[ivol].pn[0];
	  p2_name = g_element[ivol].pn[1];
	  p3_name = g_element[ivol].pn[2];
	  p4_name = g_element[ivol].pn[3];
	  p5_name = g_element[ivol].pn[4];
	  p6_name = g_element[ivol].pn[5];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Penta-eder ------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Penta-eder ------- */
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Penta-eder ------- */
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	    }

	  /* Indexverschiebung f. Pentaeder */
	  if (g_element[ivol].nachbar[4] != Stoffnr)
	    {
	      /* 4. Seite Penta-eder ------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr)
	    {
	      /* 5. Seite Penta-eder ------- */
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p5_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p5_koord);
	      glEnd ();
	      glLoadName (p6_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p6_koord);
	      glEnd ();
	    }
	}			/* End if Elementzeichnen == 1 */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      zeichne_named_schnittelement (ivol);
	    }
	}			/* End else */
    }				/* end Schleife ueber PENTAEDER --------- */

  /* TETRAEDER------------------------------------------------------ */
  /* Schleife ueber Elemente --------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_tet;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_tet; ivol++)
    {
/* Element zeichnen ---------- */
      if (((g_element[ivol].zeichnen == 1) && (g_transform.schneiden_on)) ||
	  (!g_transform.schneiden_on))
	{
	  /* Koordinaten d. Elem */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p1_name = g_element[ivol].pn[0];
	  p2_name = g_element[ivol].pn[1];
	  p3_name = g_element[ivol].pn[2];
	  p4_name = g_element[ivol].pn[3];

	  if (g_element[ivol].nachbar[0] != Stoffnr)
	    {
	      /* 1. Seite Tetra-eder ------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr)
	    {
	      /* 2. Seite Tetra-eder ------- */
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr)
	    {
	      /* 3. Seite Tetra-eder ------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p3_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p3_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr)
	    {
	      /* 4. Seite Tetra-eder ------- */
	      glLoadName (p1_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p1_koord);
	      glEnd ();
	      glLoadName (p2_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p2_koord);
	      glEnd ();
	      glLoadName (p4_name);
	      glBegin (GL_POINTS);
	      glVertex3fv (p4_koord);
	      glEnd ();
	    }
	}			/* End if Elementzeichnen == 1 */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2)
	    {
	      zeichne_named_schnittelement (ivol);
	    }
	}			/* End else */
    }				/* end Schleife ueber TETRAEDER - ----------- */

/* Verschiebung rueckgaengig machen ------------------------------ */
  if (verschiebe_on)
    glTranslatef (-g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[2]);

/* **************** ende zeichne_named_knoten *************************** */
  return (0);
}

/* ********************************************************************* */
int
findeKnoten (int hits, GLuint * buffer)
/* ********************************************************************* */
{
  GLuint *ptr;
  unsigned int i, j;
  GLint names;
  GLuint minz, maxz, z, match;


  if (hits < 0)
    {
      fprintf (stderr, "\n Select-Buffer Overflow !\n");
      ask_user ("Nodes too close together: please zoom.", "Ok", "", "", 1);
      return (0);
    }
#ifdef DEBUG
  printf ("\nEs wurden %d Knoten selektiert.\n");
  ptr = (GLuint *) buffer;
  for (i = 0; i < hits; i++)
    {
      /* Fuer jeden Treffer Auswertung */
      names = *ptr;
      printf ("\nAnzahl der getroffenen Knoten: %d\n", names);
      ptr++;
      printf (" z1 ist %g;", (float) *ptr / 0xffffffff);
      ptr++;
      printf (" z2 ist %g\n", (float) *ptr / 0xffffffff);
      ptr++;
      printf (" die Nummer ist ");
      for (j = 0; j < names; j++)
	{
	  printf ("%d ", *ptr);
	  ptr++;
	}
      printf ("\n");
    }
#endif

  if (hits == 0)
    return (0);
  minz = 0xffffffff;
  ptr = (GLuint *) buffer;
  for (i = 0; i < hits; i++)
    {
      names = *ptr;
      ptr++;
      z = *ptr;
      ptr++;
      ptr++;
      if (minz >= z)
	{
	  minz = z;
	  match = *ptr;
	}
      ptr += names;
    }

  return (match);
}

/************************************************************************
pick_knoten: This routine pick the points from given mouse position

Parameters:
==========
             int m_x - mouse position in x coor.
	     int m_y - mouse position in y coor.
	     int pick_w - width of picking rectangle
	     int pick_h - height of picking rectangle
	     int buffer_size - size of the buffer for the points
Returns:
========
             Function returns the number of points found in the rectangle
*************************************************************************/
/* ******************************************************************** */
int
picke_knoten (int m_x, int m_y, int pick_w, int pick_h, int buffer_size)
/* ******************************************************************** */
{				/* m_x und m_y ist die Mausposition          */
  GLint viewport[4];		/* Buffer fuer die ViewportPixel-Koordinaten */
  GLuint *selbuffer;		/* Buffer fuer die 'Names' der Objekte       */
  float aspect;			/* Verhaeltnis w/h                           */
  int hits;
  int vp_x, vp_y, vp_w, vp_h;	/* Viewportkoordinaten */
  float delta_x;
  float delta_y;
  float breite;
  float hoehe;
  GLfloat matrix[16];
  float vektor[3];
  float scale_x;
  float scale_y;
  int stoff_nr;
  int colormap_index;
  t_koord center;

  float pickangle, diff_z;
  int i, j;
  /* wird spaeter in der Funktion uebergeben */

  /* Zeichnung vorbereiten: */

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadMatrixf (Projektions_matrix);

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadMatrixf (Save_mo_matrix);


  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
    colormap_index = STOFFARB_OFFSET + 1;
  else
    {
      colormap_index = ACHT_BIT_OFFSET;
      glClearIndex (colormap_index);
    }

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel (GL_FLAT);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* Speicher fuer Selectbuffer holen: */
  selbuffer = (GLuint *) malloc (16 * buffer_size);

  /* Buffer fuer die Namen (Longint) bekanntmachen */
  glSelectBuffer (buffer_size, selbuffer);

  /* Select-Modus einschalten */
  glRenderMode (GL_SELECT);


  /* Name-Stack initialisieren */
  glInitNames ();
  glPushName (-1);

  /* Viewport holen: */
  glGetIntegerv (GL_VIEWPORT, viewport);
  vp_x = viewport[0];
  vp_y = viewport[1];
  vp_w = viewport[2];
  vp_h = viewport[3];

  /* gelieferte Screenkoordinaten sind -Y ! */
  m_y = vp_h - m_y;


  /* Zoomfaktor festlegen: */
  scale_x = (float) vp_w / (float) pick_w;
  scale_y = (float) vp_h / (float) pick_h;


  /* Verschiebung in X- und Y-Richtung berechnen (bezogen auf Identity-Matrix) */
  breite = (g_projektion.right - g_projektion.left) / ((float) vp_w);
  hoehe = (g_projektion.top - g_projektion.bottom) / ((float) vp_h);
  delta_x = ((float) m_x - ((float) vp_w / 2.0)) * breite;
  delta_y = ((float) m_y - ((float) vp_h / 2.0)) * hoehe;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  if (g_projektion.perspektive)
    glOrtho (g_projektion.left, g_projektion.right, g_projektion.bottom,
	     g_projektion.top, g_projektion.near, g_projektion.far);
  else
    glFrustum (g_projektion.left, g_projektion.right, g_projektion.bottom,
	       g_projektion.top, g_projektion.near, g_projektion.far);

  glTranslatef (-(scale_x * delta_x), -(scale_y * delta_y), 0.0);
  glTranslatef (scale_x * g_transform.move[0], scale_y * g_transform.move[1],
		g_transform.move[2]);
  glScalef (scale_x, scale_y, 1.0);


  /* und jetzt die Zeichenroutine mit Names */


  /* Alle Stoffe zeichnen, die aktiv sind */
  for (i = 0; i < g_geodat.nstoff; i++)
    if (g_stoffgebiete[i].zeichne_on)
      {
	zeichne_named_knoten (g_stoffgebiete[i].nr);
	/* zeichne_gitterstoff(g_stoffgebiete[i].nr);  */
      }


  /* Anzahl der Treffer holen und Selection-Mode beenden */
  hits = 0;

  hits = glRenderMode (GL_RENDER);
  if (printok)
    printf ("\n%d Knoten im Suchradius.", hits);

  /* Den Knoten finden, der am naechsten beim Betrachter liegt */
  hits = findeKnoten (hits, selbuffer);
  if (printok)
    printf ("\nNehme Nummer: %d.\n", hits);

  /* lokalen Stack-Speicher wieder freigeben */
  free (selbuffer);

  /* Orientierungspunkte setzen: */

  /* Ursprungsprojektion wiederherstellen */




  /*if(C3D_win.doublebuffer)
     glXSwapBuffers(XtDisplay(gl_widget), XtWindow(gl_widget));
     else 
     glFlush();
   */
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();

  printf ("\nHits = %d.", hits);
  /* Nr. des gefundenen Knotens uebergeben */
  return (hits);
}

/* ********************************************************************* */
void
free_pick ()
/* ********************************************************************* */
{
  t_pickpunkt *p_pickpunkt;
  t_pickpunkt *s_pickpunkt;

  if (g_picking.anzahl != 0)
    {
      s_pickpunkt = p_pickpunkt = g_picking.punkte;
      while (s_pickpunkt != NULL)
	{
	  s_pickpunkt = s_pickpunkt->next;
	  free (p_pickpunkt);
	  p_pickpunkt = s_pickpunkt;
	}
      g_picking.anzahl = 0;
    }
  return;
/* ********************************************************************* */
}


int
pick_nodes (int m_x, int m_y, int pick_w, int pick_h, int buffer_size)
{
  GLint viewport[4];		/* viewport coordinates */
  GLuint *selectBuf;		/* select buffer for the names */
  GLint hits;
  float aspect;
  int i;

  /* allocate memory */
  selectBuf = (GLuint *) malloc (sizeof (GLuint) * buffer_size);
  if (selectBuf == NULL)
    fprintf (stderr, "Not enough memory\n");

  /* get the viewport */
  glGetIntegerv (GL_VIEWPORT, viewport);

  /* get the names buffer */
  glSelectBuffer (buffer_size, selectBuf);
  (void) glRenderMode (GL_SELECT);

  /* init the name buffer */
  glInitNames ();
  glPushName (-1);

  /* remebare the old transformation matrix */
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadMatrixf (Zeichen_matrix);

  gluPickMatrix ((GLdouble) m_x,
		 (GLdouble) (viewport[3] - m_y), pick_w, pick_h, viewport);
  aspect = ((float) viewport[2] / viewport[3]);
  gluPerspective (60.0, aspect, 1.0, 7.0);
  /* Alle Stoffe zeichnen, die aktiv sind */
  for (i = 0; i < g_geodat.nstoff; i++)
    if (g_stoffgebiete[i].zeichne_on)
      {
	zeichne_named_knoten (g_stoffgebiete[i].nr);
	/* zeichne_gitterstoff(g_stoffgebiete[i].nr);  */
      }
  glFlush ();

  /* set the number of hits */
  /*hits = 0; */

  /* return the number of hits */
  hits = glRenderMode (GL_RENDER);

  hits = processHits (hits, selectBuf);

  /* swap the double-buffer */
  if (C3D_win.doublebuffer)
    glXSwapBuffers (XtDisplay (gl_widget), XtWindow (gl_widget));
  else
    glFlush ();

  /* restore old matrix */
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();

  printf ("\nHits = %d.", hits);
  /* Nr. des gefundenen Knotens uebergeben */
  return (hits);

}


GLint
processHits (GLint hits, GLuint buffer[])
{
  /* local variables */
  unsigned int i, j;
  GLuint minz, z, match, *ptr;
  GLuint names;

  if (hits < 0)
    {
      fprintf (stderr, "WARNING: select buffer overflow!\n");
      return (-1);
    }
#ifdef DEBUG
  printf (" hits=%d\n", hits);

  /* get the pointer to buffer */
  ptr = buffer;
  /* for each hit get the number of names */
  for (i = 0; i < hits; i++)
    {
      names = *ptr;
      printf (" number of names for this hit = %d\n", names);
      ptr++;
      printf (" z1 is %g", (float) *ptr / 0xffffffff);
      ptr++;
      printf (" z2 is %g\n", (float) *ptr / 0xffffffff);
      ptr++;

      printf (" the name is ");
      /* print the names for each hits */
      for (j = 0; j < names; j++)
	{
	  printf ("%d ", (int) *ptr);
	  ptr++;
	}
      printf ("\n");
    }
#endif

  if (hits == 0)
    return 0;
  minz = 0xffffffff;
  ptr = buffer;
  for (i = 0; i < hits; i++)
    {
      /* z coordinate of first object */
      names = *ptr;
      ptr++;
      z = *ptr;
      ptr++;
      ptr++;

      if (z <= minz)
	{
	  minz = z;
	  match = *ptr;
	}
      ptr += names;
    }
  return (match);
}

/**************************************************************************
 This function calculates the vectors on the faces of the elements.
 This vector will be then cross multiply and normalize to get normal
 vector on the face.

 Parameters:
      -float p1[3], p2[3], p3[3] - coordinates of three points
 Returns:
      -GLfloat v1[3], v2[2] -two vectors wich defines the plane on the face
**************************************************************************/
void
calvec (float p1[3], float p2[3], float p3[3], GLfloat v1[3], GLfloat v2[3])
{
  int i;

  for (i = 0; i < 3; i++)
    {
      v1[i] = p2[i] - p1[i];
      v2[i] = p3[i] - p1[i];
    }
}

/************************************************************************
 This function displays the solutions on the polygon mesh, it uses 
 the texture mapping to enable better drawing of isolines, and contures.

 Parameters:
       -int Stoffnr - number of material

 Returns:
      -none
*************************************************************************/

/* ******************************************************************** */
int
zeichne_result_tex (int Stoffnr)	/* erzeugt Resultmodell : Stoff Stoffnr */
/* ******************************************************************** */
{
  int i,			/* Zaehlvariablen -------------------------------- */
    nummer,			/* Nr fuer geschnittenes El. --------------------- */
    ivol;			/* Zaehler fuer Elemente ------------------------- */
  t_bool verschiebe_on = FALSE;	/* Flagge f. Verschiebung des Stoffes - */
  static t_bool ispass = TRUE;	/* flag for counting boundary elements  */
  float t1, t2, t3, t4, t5, t6, t7, t8;	/* Temperaturen der Punkte ------- */
  GLfloat norm1[3], norm2[3], norm3[3], norm4[3], norm5[3], norm6[3];
  GLfloat d1[3], d2[3];

  t_koord p1_koord,		/* Punktkoorinaten ------------------------------ */
    p2_koord, p3_koord, p4_koord, p5_koord, p6_koord, p7_koord, p8_koord;
  int anzahl;
  int start;
  float faktor;
  static t_bool raster_on = TRUE;
  t_texture_info *t;
  t_bool texture_on;

  t = &g_tex_info;

/* ******************************************************************** */
/* Erzeugen der Resultmodelle ----------------------------------------- */

  /* if number of material is equal to num. 
   * of materials don't count
   * boundary elements any more
   */
  if (Stoffnr == g_geodat.nstoff)
    ispass = FALSE;

/* Verschiebung einzelner Stoffe -------------------------------------- */
  if (g_stoffgebiete[Stoffnr - 1].verschiebung[0] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[1] ||
      g_stoffgebiete[Stoffnr - 1].verschiebung[2])
    {
      glTranslatef (g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		    g_stoffgebiete[Stoffnr - 1].verschiebung[2]);
      verschiebe_on = TRUE;
    }

  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, -g_ofen.akt_position);

  /* make polygons white */
  glColor3f (1.0, 1.0, 1.0);
  glEnable (GL_TEXTURE_1D);

  if (raster_on && (g_tabelle.art == 3))
    {
      if (glIsTexture (t->texName[1]))
	{
	  /* initialize all parameters for texture mapping */
	  glBindTexture (GL_TEXTURE_1D, t->texName[1]);
	}
    }
  else if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
    {
      if (glIsTexture (t->texName[0]))
	glBindTexture (GL_TEXTURE_1D, t->texName[0]);
    }
  else
    {
      fprintf (stderr, "Error in texture mapping\n");
    }


  /* HEXAEDER --------------------------------------------------------- */
  /* Schleife ueber Elemente ------------------------------------------ */
  glBegin (GL_QUADS);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_hex;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_hex; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && 
		(g_transform.schneiden_on)) || (!g_transform.schneiden_on) || (g_dis_cutting[Stoffnr]))	/* Element zeichnen ------- */
	{
	  if (ispass)
	    g_itri++;		/* number of boundary elements */

	  /* Koordinaten der Elemente */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  p7_koord[0] = g_knoten[g_element[ivol].pn[6] - 1].koord[0];
	  p7_koord[1] = g_knoten[g_element[ivol].pn[6] - 1].koord[1];
	  p7_koord[2] = g_knoten[g_element[ivol].pn[6] - 1].koord[2];

	  p8_koord[0] = g_knoten[g_element[ivol].pn[7] - 1].koord[0];
	  p8_koord[1] = g_knoten[g_element[ivol].pn[7] - 1].koord[1];
	  p8_koord[2] = g_knoten[g_element[ivol].pn[7] - 1].koord[2];

	  /* Temperaturen der Punkte -- */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];
	  t5 = g_loesung.temp[g_element[ivol].pn[4] - 1];
	  t6 = g_loesung.temp[g_element[ivol].pn[5] - 1];
	  t7 = g_loesung.temp[g_element[ivol].pn[6] - 1];
	  t8 = g_loesung.temp[g_element[ivol].pn[7] - 1];

	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      calvec (p1_koord, p5_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Hexa-eder -------- */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm1);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm1);
	      glVertex3fv (p6_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm1);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of hexaedar */
	      calvec (p2_koord, p6_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Hexa-eder -------- */
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm2);
	      glVertex3fv (p6_koord);
	      glTexCoord1f (CcolTex (t7));
	      glNormal3fv (norm2);
	      glVertex3fv (p7_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm2);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of hexaedar */
	      calvec (p3_koord, p7_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm3);


	      /* 3. seite hexa-eder -------- */
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm3);
	      glVertex3fv (p3_koord);
	      glTexCoord1f (CcolTex (t7));
	      glNormal3fv (norm3);
	      glVertex3fv (p7_koord);
	      glTexCoord1f (CcolTex (t8));
	      glNormal3fv (norm3);
	      glVertex3fv (p8_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm3);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of hexaedar */
	      calvec (p4_koord, p8_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Hexa-eder -------- */
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm4);
	      glVertex3fv (p4_koord);
	      glTexCoord1f (CcolTex (t8));
	      glNormal3fv (norm4);
	      glVertex3fv (p8_koord);
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm4);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm4);
	      glVertex3fv (p1_koord);
	    }

	  if (g_element[ivol].nachbar[4] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Hexa-eder -------- */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm5);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm5);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm5);
	      glVertex3fv (p3_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm5);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of hexaedar */
	      calvec (p5_koord, p8_koord, p6_koord, d1, d2);
	      crossprod (d1, d2, norm6);

	      /* 6. Seite Hexa-eder -------- */
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm6);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t8));
	      glNormal3fv (norm6);
	      glVertex3fv (p8_koord);
	      glTexCoord1f (CcolTex (t7));
	      glNormal3fv (norm6);
	      glVertex3fv (p7_koord);
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm6);
	      glVertex3fv (p6_koord);
	    }

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2 && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_QUADS);
	}			/* End else */
    }				/* end Schleife ueber HEXAEDER ------------------ */
  glEnd ();

  /* PENTAEDER ------------------------------------------------------- */
  /* Schleife ueber Elemente ----------------------------------------- */
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_pen;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_pen; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && 
		(g_transform.schneiden_on)) || (!g_transform.schneiden_on) || (g_dis_cutting[Stoffnr]))	/* Element zeichnen ------- */
	{
	  if (ispass)
	    g_itri++;
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  p5_koord[0] = g_knoten[g_element[ivol].pn[4] - 1].koord[0];
	  p5_koord[1] = g_knoten[g_element[ivol].pn[4] - 1].koord[1];
	  p5_koord[2] = g_knoten[g_element[ivol].pn[4] - 1].koord[2];

	  p6_koord[0] = g_knoten[g_element[ivol].pn[5] - 1].koord[0];
	  p6_koord[1] = g_knoten[g_element[ivol].pn[5] - 1].koord[1];
	  p6_koord[2] = g_knoten[g_element[ivol].pn[5] - 1].koord[2];

	  /* Temperaturen der Punkte ------ */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];
	  t5 = g_loesung.temp[g_element[ivol].pn[4] - 1];
	  t6 = g_loesung.temp[g_element[ivol].pn[5] - 1];

	  glBegin (GL_QUADS);
	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 1 side of pentaedar */
	      calvec (p1_koord, p4_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Penta-eder ------ */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm1);
	      glVertex3fv (p4_koord);
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm1);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm1);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of pentaedar */
	      calvec (p2_koord, p5_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Penta-eder ------ */
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm2);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm2);
	      glVertex3fv (p6_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm2);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of pentaedar */
	      calvec (p3_koord, p6_koord, p1_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Penta-eder ------ */
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm3);
	      glVertex3fv (p3_koord);
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm3);
	      glVertex3fv (p6_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm3);
	      glVertex3fv (p4_koord);
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm3);
	      glVertex3fv (p1_koord);
	    }
	  glEnd ();

	  glBegin (GL_TRIANGLES);

	  /* Indexverschiebung f. Pent. */
	  if (g_element[ivol].nachbar[4] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of pentaedar */
	      calvec (p1_koord, p2_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Penta-eder ------ */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm4);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm4);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm4);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[5] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 5 side of pentaedar */
	      calvec (p6_koord, p5_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm5);

	      /* 5. Seite Penta-eder ------- */
	      glTexCoord1f (CcolTex (t6));
	      glNormal3fv (norm5);
	      glVertex3fv (p6_koord);
	      glTexCoord1f (CcolTex (t5));
	      glNormal3fv (norm5);
	      glVertex3fv (p5_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm5);
	      glVertex3fv (p4_koord);

	    }
	  glEnd ();

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2 && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	}			/* End else */

    }				/* end Schleife ueber PENTAEDER ----------------- */

  /* TETRAEDER-------------------------------------------------------- */
  /* Schleife ueber Elemente ----------------------------------------- */
  glBegin (GL_TRIANGLES);
  for (ivol = g_stoffgebiete[Stoffnr - 1].anfang_tet;
       ivol < g_stoffgebiete[Stoffnr - 1].ende_tet; ivol++)
    {
      if (((g_element[ivol].zeichnen == 1 || g_element[ivol].zeichnen == 3) && 
		(g_transform.schneiden_on)) || (!g_transform.schneiden_on) || (g_dis_cutting[Stoffnr]))	/* Element zeichnen */
	{
	  if (ispass)
	    g_itri++;
	  /* Koordinaten d. El.  */
	  p1_koord[0] = g_knoten[g_element[ivol].pn[0] - 1].koord[0];
	  p1_koord[1] = g_knoten[g_element[ivol].pn[0] - 1].koord[1];
	  p1_koord[2] = g_knoten[g_element[ivol].pn[0] - 1].koord[2];

	  p2_koord[0] = g_knoten[g_element[ivol].pn[1] - 1].koord[0];
	  p2_koord[1] = g_knoten[g_element[ivol].pn[1] - 1].koord[1];
	  p2_koord[2] = g_knoten[g_element[ivol].pn[1] - 1].koord[2];

	  p3_koord[0] = g_knoten[g_element[ivol].pn[2] - 1].koord[0];
	  p3_koord[1] = g_knoten[g_element[ivol].pn[2] - 1].koord[1];
	  p3_koord[2] = g_knoten[g_element[ivol].pn[2] - 1].koord[2];

	  p4_koord[0] = g_knoten[g_element[ivol].pn[3] - 1].koord[0];
	  p4_koord[1] = g_knoten[g_element[ivol].pn[3] - 1].koord[1];
	  p4_koord[2] = g_knoten[g_element[ivol].pn[3] - 1].koord[2];

	  /* Temperaturen der Punkte ------ */
	  t1 = g_loesung.temp[g_element[ivol].pn[0] - 1];
	  t2 = g_loesung.temp[g_element[ivol].pn[1] - 1];
	  t3 = g_loesung.temp[g_element[ivol].pn[2] - 1];
	  t4 = g_loesung.temp[g_element[ivol].pn[3] - 1];

	  if (g_element[ivol].nachbar[0] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 1 side of pentaedar */
	      calvec (p1_koord, p4_koord, p3_koord, d1, d2);
	      crossprod (d1, d2, norm1);

	      /* 1. Seite Tetra-eder ------- */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm1);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm1);
	      glVertex3fv (p4_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm1);
	      glVertex3fv (p3_koord);
	    }

	  if (g_element[ivol].nachbar[1] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 2 side of tetraedar */
	      calvec (p2_koord, p3_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm2);

	      /* 2. Seite Tetra-eder ------- */
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm2);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm2);
	      glVertex3fv (p3_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm2);
	      glVertex3fv (p4_koord);
	    }

	  if (g_element[ivol].nachbar[2] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 3 side of pentaedar */
	      calvec (p1_koord, p3_koord, p2_koord, d1, d2);
	      crossprod (d1, d2, norm3);

	      /* 3. Seite Tetra-eder ------- */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm3);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t3));
	      glNormal3fv (norm3);
	      glVertex3fv (p3_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm3);
	      glVertex3fv (p2_koord);
	    }

	  if (g_element[ivol].nachbar[3] != Stoffnr || g_element[ivol].zeichnen == 3)
	    {
	      /* calculate normal for 4 side of pentaedar */
	      calvec (p1_koord, p2_koord, p4_koord, d1, d2);
	      crossprod (d1, d2, norm4);

	      /* 4. Seite Tetra-eder ------- */
	      glTexCoord1f (CcolTex (t1));
	      glNormal3fv (norm4);
	      glVertex3fv (p1_koord);
	      glTexCoord1f (CcolTex (t2));
	      glNormal3fv (norm4);
	      glVertex3fv (p2_koord);
	      glTexCoord1f (CcolTex (t4));
	      glNormal3fv (norm4);
	      glVertex3fv (p4_koord);
	    }

	}			/* Ende if element zeichnen == 1 ------------------------- */
      else
	{
	  glEnd ();
	  /* Schnittelement wird gezeichnet ------------------------- */
	  if (g_element[ivol].zeichnen == 2 && (!g_dis_cutting[Stoffnr]))
	    {
	      glDisable (GL_CULL_FACE);
	      zeichne_schnitt_elem (ivol, GL_POLYGON, TRUE);
	      glEnable (GL_CULL_FACE);
	    }
	  glBegin (GL_TRIANGLES);
	}			/* End else */
    }				/* end Schleife ueber TETRAEDER ------------- */
  glEnd ();

  /* disable texture */
  disableTex ();
/* Edges on ------------------------------------------------------ */
  if (edgeinfo.edge_on)
    draw_polyline ();

/* Verschiebung rueckgaengig machen ------------------------------ */
  if (verschiebe_on)
    glTranslatef (-g_stoffgebiete[Stoffnr - 1].verschiebung[0],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[1],
		  -g_stoffgebiete[Stoffnr - 1].verschiebung[2]);

/* Absenkung rueckgaengig machen --------------------------------- */
  if (g_transform.absenken_on && g_ofen.stofen[Stoffnr - 1])
    glTranslatef (0.0, 0.0, g_ofen.akt_position);
  return (0);
/* ****************** ende zeichne_result_tex ****************************** */
}

/*************************************************************************
 Get value of texture coordinate.
 Parameters:
 ===========
            float tmp - given temperature
 Returns:
 ========
            texture coordinate 1D.
**************************************************************************/
GLfloat
CcolTex (float tmp)
{
  COLOUR c;			/* color structure   */
  int icolmap = 1;		/* index of colormap */
  float t;			/* temp. clamped to 0-255 */
  GLint maxtex[4];
  int tex_size;

  /* if colormap hot-cold and default */
  if (g_tabelle.art == 1)
    {
      t = (tmp - g_loesung.Min) / (g_loesung.Max - g_loesung.Min);
      texCoord1D = (GLfloat) t;
    }
  /* raster colormap */
  else if ((!C3D_win.achtbit) && (g_tabelle.art == 3))
    {
      t = (tmp - g_loesung.Min) / (g_loesung.Max - g_loesung.Min);
      texCoord1D = (GLfloat) t;
    }
  return texCoord1D;
}

/********************************************************************
 This function builds textures for displaying default 
 colormap.

 Parameters:
 ===========
     int tex_size - max texture size
	   int ntex     - number of textures to create
 Returns:
 ========
           none
*********************************************************************/
void
makeTexImageDef (int tex_size, int ntex)
{
	int tcol;			    /* type of colormap  */
	int i = 0, j = 0;
	int icolmap = 1;	/* index of colormap */
	COLOUR c;
	t_colorub color;
	float tmp;			  /* temperature value */
	int col;			    /* value of isoline  */
	float maxsize;
	t_texture_info *t;

	t = &g_tex_info;
	/* choose th colormap */
	tcol = g_color_extra.color_name;

	/* make the texture with maping the
	 * temperature range.
	 */
	maxsize = (float) tex_size;

	for (i = 0; i < tex_size; i++)
	{
		color = C3D_getcolor ((float) i, 0.0, maxsize, 0);

		/* get non-default color maps */
		if (tcol)
		{
			c = GetColour ((double) i, 0.0, maxsize, tcol);
			color.r = (GLubyte) (c.r * 255.0);
			color.g = (GLubyte) (c.g * 255.0);
			color.b = (GLubyte) (c.b * 255.0);
		}

		/* in case that color > 255 clamped to 255 */
		t->texImage[j][4 * i] = (GLubyte) ((color.r > 255) ? 255 : color.r);
		t->texImage[j][4 * i + 1] = (GLubyte) ((color.g > 255) ? 255 : color.g);
		t->texImage[j][4 * i + 2] = (GLubyte) ((color.b > 255) ? 255 : color.b);
		t->texImage[j][4 * i + 3] = (GLubyte) 255;
	}

}

/*******************************************************************
  Purpose:
  ========

  This function makes raster tex image for mapping the model with
  texture.

  Parameters
  ===========
           -none
  Returns
  ========
           -none
  
********************************************************************/
void
makeTexImageRas (void)
{
	int tcol;			/* type of colormap  */
	int i = 0, j = 0;		/* counters          */
	int icolmap = 1;		/* index of colormap */
	COLOUR c;			/* color structure   */
	double tmp;			/* temperature value */
	int col;			/* color index in colortable */
	float maximum;		/* max. temperature for colortables */
	t_texture_info *t;
	double dx = 1.0, x = 0.0;

	t = &g_tex_info;
	t->nstripes = 10;

	/* make texture for raster mapping
	 * of colors.
	 */
	maximum = t->tex_size[0];
	tcol = g_color_extra.color_name;
	for (x = 0.0; x < t->tex_size[0]; x = x + dx)
	{
		double nstrip;
		nstrip = t->tex_size[0] / (t->nstripes);
		tmp = staircase (x, nstrip, nstrip);

		if (tmp < 0.0)
		{
			colorTex[0] = (GLubyte) g_tabelle.colortable[1];
			colorTex[1] = (GLubyte) g_tabelle.colortable[2];
			colorTex[2] = (GLubyte) g_tabelle.colortable[3];
		}
		if (tmp > maximum)
		{
			colorTex[0] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 1];
			colorTex[1] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 2];
			colorTex[2] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 3];
		}

		if ( maximum > 0 )
			col = (int) (tmp * (COLORTABLE_LENGTH / maximum));


		colorTex[0] = (GLubyte) g_tabelle.colortable[4 * col + 1];
		colorTex[1] = (GLubyte) g_tabelle.colortable[4 * col + 2];
		colorTex[2] = (GLubyte) g_tabelle.colortable[4 * col + 3];
		colorTex[3] = (GLubyte) 255;

		/* get non-default color maps */
		if (tcol)
		{
			c = GetColour ((double) tmp, 0.0, maximum, tcol);
			colorTex[0] = (GLubyte) (c.r * 255.0);
			colorTex[1] = (GLubyte) (c.g * 255.0);
			colorTex[2] = (GLubyte) (c.b * 255.0);
		}

		/* in case that color > 255 clamped to 255 */
		t->texImage[1][4 * i + 0] = (GLubyte) ((colorTex[0] > 255) ? 255 : colorTex[0]);
		t->texImage[1][4 * i + 1] = (GLubyte) ((colorTex[1] > 255) ? 255 : colorTex[1]);
		t->texImage[1][4 * i + 2] = (GLubyte) ((colorTex[2] > 255) ? 255 : colorTex[2]);
		t->texImage[1][4 * i + 3] = (GLubyte) 255;
		i++;
	}

}

/*****************************************************************
 Purpose:
 ======== 
       This function initialize the default and raster
       texture for colormap. Then it binds the texture
       to texture object.
 Parameters:
 ==========
       -int flag - this flag is used to indicate for
                   which mode(raster or default)

                   we want to create tex. object
 Returns:
 =======
       -none
******************************************************************/
void
myinit (int flag, t_bool do_again)
{
  static int enable = 1;	/* enable initialization */
  int i;			/* counter */
  int ntex;
  GLint maxtex[4];		/* maximum size of texture */
  t_texture_info *t;

  t = &g_tex_info;
  C3D_clearcolor ();
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LESS);

  /* find the size of biggest texture */
  glGetIntegerv (GL_MAX_TEXTURE_SIZE, &(t->tex_size[0]));
  t->ntex = 2;

  if (enable || do_again)
    {
      makeTexImageDef (t->tex_size[0], t->ntex);
      makeTexImageRas ();
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

      glGenTextures (t->ntex + 1, t->texName);

      for (i = 0; i < t->ntex; i++)
	{
	  /* define texture map for Default mapping */
	  glBindTexture (GL_TEXTURE_1D, t->texName[i]);
	  glTexParameterf (GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	  glTexParameterf (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameterf (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexImage1D (GL_TEXTURE_1D, 0, 4, t->tex_size[0],
			0, GL_RGBA, GL_UNSIGNED_BYTE, &(t->texImage[i]));
	}


      glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glEnable (GL_TEXTURE_1D);
      glShadeModel (GL_SMOOTH);
      enable = 0;
    }

}

/* this function disable textures */
void
disableTex (void)
{
  if (glIsEnabled (GL_TEXTURE_1D))
    glDisable (GL_TEXTURE_1D);
}

/******************************************************************
  Purpose:
  ========
     This function initialize zeichen_stoffe routines
     It contains diverse set variables.
*******************************************************************/
void C3D_init_zeichne_stoffe(void)
{
	// register our variables
	c3d_tex_stripes = CVAR_get( "c3d_tex_stripes", "10", 0 );
	c3d_tex_raster  = CVAR_get( "c3d_tex_raster", "0", 0 );
	c3d_tex_default = CVAR_get( "c3d_tex_default", "1", 0 );
}

     
	
