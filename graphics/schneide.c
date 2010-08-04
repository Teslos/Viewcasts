/* ********************************************************************* */
/*  Name des Programms : C O L O R 3 D      Rev. 2
/* ********************************************************************* */
/*
/*  Urheber : Helmut Vor
/*
/*  erstellt am  :  21.10.1992
/*  Aenderung    :  Einfuehrung dynamischer Datenstrukturen.
/*  geaendert am :  17.04.1998
/*                  Umstellung auf OpenGL ES,StK 
/*		    & neuer Schnittalgorithmus
/*
/*  Beschreibung der Routinen :
/*         schneide         : Aktivieren der Schnitt-Routinen
/*         punkt_vor_ebene  : Testen ob Netzpunkt vor oder hinter Ebene
/*         teile_strecke    : Hilfsprozedur
/*        
/* ********************************************************************* */
#include "color3d.h"
#include "grafik.h"
#include "strukturen.h"
#include "mem.h"



typedef struct schnittpunkt
{
  float d3[3];                  /* 3D-Koordinaten des Schnittpunktes  */
  float rot[3];			/* 3D-Koordinaten des gedreht. S.P.   */
  int   punktnr;                /* Index der dazugehoerigen 3D Koord. */
  float winkel;                 /* Winkel bzgl. Schnittpunkt 0 ------ */
  t_bool visit;			/* Flag fuer Sortierung ------------- */
  float nor[3];                 /* Normalen fuer die schnittpunkt --- */
  
} t_schnittpunkt;

/*#define PRINTOUT*/  

#define EPSILON_TEST 0.005
#define EPSILON_SAME 1.0
#define SKALIERUNG   1000.0

/* Number of faces for particular element */
enum {NFTETA = 4, NFPENTA, NFHEXA};  

static int hexan[] = {0,1,4,
		      1,2,5,
		      2,3,6,
		      3,0,7,
		      4,5,7,
		      0,1,3};
static int pentan[] = {0,1,4,1,2,5,2,0,3,3,4,5,0,2,1};
static int tetan[] = {0,1,3,1,2,3,2,0,3,1,0,2};
 
static int teile_strecke(int, int, t_schnittpunkt*, char);
static void triad_vect(float v[][3], int n[], int p, t_arbeits_element *elem);
void set_inter(int*, int*, int*);
/* ********************************************************************** */
int schneide()     /* schneiden des Gusstuecks an der Schnittebene ------ */
/* ********************************************************************** */
{
  int    i,                 /* Elementzaehler ------------------- */
    j, k, l, z,             /* Zaehler -------------------------- */
    m, n,                   /* Knotenindizes -------------------- */
    index,                  /* Index auf Ikenn_feld ------------- */
    minimum,		/* Index des kleinsten Winkels ------ */
    anz_max_schnittpunkte,  /* max. Anzahl von Schnittknoten/El.- */
    anz_to_clip,            /* # wegzuclippender Knoten/Element - */
    anz_cut,                /* lokaler Schnittpunktzaehler/Elem - */
    akt_knoten,             /* akt. Knotenzaehler des Element's - */
    p_index,                /* Index fuer Schnittpunkte --------- */
    *p_element_zeichnen;    /* Arbeitspointer auf element_zeichnen*/
  char   *punkte_zeichnen = NULL,/* Basispointer Punkte-zeichnen ----- */
    *p_punkte_zeichnen=NULL;/* Basispointer Punkte-zeichnen ----- */
  char   punkt_vor_ebene();     /* Routine -------------------------- */
  float  alpha_min,		/* kleinster Winkel ----------------- */
    x,y,			/* Epsilonumgebungen ---------------- */
    sub_x,sub_y,sub_z,	        /* Mittelpunkt der Schnittebene ----- */
    hx,hy,hz,		        /* Hilfskoordinaten ----------------- */
    nx,ny,nz,nzz,               /* Koordinaten fuer Normalenvektor -- */
    rxx,rxy,rxz,ryx,ryy,ryz,rzx,rzy,rzz, /* Rot-Matrix ---------- */
    rotx,roty,ll,           /* Rotationswinkel ------------------ */
    rcx,rcy,rsx,rsy;        /* sin- u. cos Anteile d Drehmatrix _ */
  int    p,                 /* akt. Pktnr. des i-ten Elements --- */
    element_index,	    /* Element-Nummer ------------------- */
    stoffgebiet_index,	    /* Stoff - Nummer ------------------- */
    p1,p2,                  /* Pktnrn beider Endpunkte von Kante l*/
    f,g,f0,f1,f2,g0,g1,g2,  /* Hilfvariable,enthaelt Flaechennr.  */
    j0,j1,k0,k1,l0,l1,m0,m1,/* dito ----------------------------- */
    n1,n2,                  /* Vorgaenger- u Nachfolgerschnittpkt */ 
    knoten_index,           /* Knoten_index f. Feld schnittpunkt- */
    neue_schnittpunkt_nr,   /* Nr. des neu berechne. Schnittpunkt */
    start,                  /* */
    *reihe = NULL,	    /* geordnete Liste der Schnittp.Ind.- */
    hilf,                   /* */
    *hflaeche = NULL;       /* max.#Flaechen/Schnittelement ----- */
  t_bool stoffaktuell,	    /* While-Schleifenflag -------------- */
    printok;		    /* Flag fuer DEBUG-Ausgabe ---------- */
  int *fn;                  /* Temporary array ------------------ */
  int  in;                  /* Counter to normals --------------- */
  FILE *fptest;             /* Test file ------------------------ */
        	  
  typedef struct standard_element /* Standardverkettung fuer Hexa,Penta */
  {                               /* und Tetraeder -------------------- */
    int anz_node;                 /* Anzahl der Knoten (8,6 oder 4) --- */
    int anz_area;                 /* Anzahl Flaechen/Elementtyp (8,6,4) */
    t_uni_knoten *node;           /* node: Feld von anz_node Knoten --- */
  } t_standard_element;
  
  t_standard_element standard_element_hex,
    standard_element_pen,
    standard_element_tet,
    *hilfsstandardelement = NULL;
  
  t_arbeits_element  *arbeits_element = NULL; /* Typ global, Variable lokal */
  t_arbeits_element  *ende_globaler_liste;
  t_arbeits_element  *help;
 
  t_schnittpunkt *schnittpunkt = NULL; /* Liste der neuen Schnittpunkte - */
  t_schnittpunkt *h	      = NULL; /* Hilfszeiger fuer realloc ------ */
 
  t_knoten *org_knoten;   /* Zeiger auf original-Geometrie -------------- */

  /* ********************************************************************* */

  printok = FALSE;
#ifdef PRINTOUT
  printok = TRUE;
#endif

  if(printok) {
    printf("\nSchnitt-Algorithmus beginnt.");
    printf("\nSchnittebene geht durch:");
    printf("\nP(%d) %5f %5f %5f",1,g_transform.schnittp1_koord[0],
	   g_transform.schnittp1_koord[1], g_transform.schnittp1_koord[2]);
    printf("\nP(%d) %5f %5f %5f",2,g_transform.schnittp2_koord[0],
	   g_transform.schnittp2_koord[1], g_transform.schnittp2_koord[2]);
    printf("\nP(%d) %5f %5f %5f",3,g_transform.schnittp3_koord[0],
	   g_transform.schnittp3_koord[1], g_transform.schnittp3_koord[2]); 
    printf("\nNorm geht durch:");
    printf("\nNorm %5f %5f %5f Abstand:%5f",g_transform.norm[0],
	   g_transform.norm[1],g_transform.norm[2], g_transform.d);
  } /* END OF PRINT */

  /* allozierten Platz freigeben -------------------------------------- */

  if (g_schnittpunktliste != NULL) 
    {
      FREE (g_schnittpunktliste);
      if(printok) {
	printf("\nAlte Schnittpunktliste weggeworfen.");
      } /* END OF PRINT */
    }
  g_schnittpunktliste = NULL;
  g_anz_schnittpunkte = 0;

  /* Platz allockieren fuer Schnittfeld ------------------------------- */
  if ((punkte_zeichnen=(char*) ALLOC((g_geodat.npunkt)*sizeof(char)))==NULL)
    {
      fprintf(stderr,"\nNicht genuegend Speicher fuer %d Bools.",g_geodat.npunkt);
      return -1;                          /* Kein Platz ------------- */
    }
  
  /* Falls Spannungsloesung vorliegt, muss auf den verspannten Koordinaten */
  /* Geschnitten werden:							 */

  if(g_spannung.on&&(g_spannung.verzerrung!=NULL))
    {
      org_knoten = g_knoten;
      g_knoten = g_spannung.verzerrung;
    }

  p_punkte_zeichnen = punkte_zeichnen;  /* init Pointer ----------- */

  /* fuer alle Punkte feststellen ob Punkt vor der Schnittebene liegt - */

  if(printok) {
    printf("\nBelege Punkt_Vor_Ebene - Feld.");
  } /* END OF PRINT */

  for (i=0;i<g_geodat.npunkt;i++)
    *(p_punkte_zeichnen++) = punkt_vor_ebene(g_knoten[i].koord);

  /* fuer alle Elemente feststellen,ob Element gezeichnet werden soll:  */
  /* Element wird gezeichnet,wenn alle Punkte hinter der Schnittebene - */
  /* liegen oder Element von der Ebene geschnitten wird --------------- */
  p_punkte_zeichnen = punkte_zeichnen;
  
  /* Alten Schnitt wegwerfen ------------------------------------------ */
  for(i=0;i<g_geodat.nvol;i++)
    {
      if(g_element[i].zeichnen==2)
	{
	  help = g_element[i].cut;
	  FREE(help->node);
	  FREE(help);
	  g_element[i].cut = NULL;
	} 
      g_element[i].zeichnen = 1;
    }
 
  /*--------------------------------------------------------------------*/
  /* je ein STANDARD-ELEMENT fuer HEXA-,PENTA- und TETRAEDER aufbauen - */
  /*--------------------------------------------------------------------*/
  standard_element_hex.anz_area = 6;
  standard_element_hex.anz_node = 8; /* STANDARD HEXAEDER hat 8 Knoten*/
  anz_max_schnittpunkte = (standard_element_hex.anz_node/2) + 2;

  /* Speicherplatz allozieren fuer die 8+6 Dreibeine ------------------ */
  standard_element_hex.node=(t_uni_knoten *)ALLOC(sizeof(t_uni_knoten)
			   *(standard_element_hex.anz_node + anz_max_schnittpunkte));
  if (standard_element_hex.node == NULL) 
    {
      fprintf(stderr,"\nKein Speicher fuer Standard-Hex!");
      return -1;
    }


  /* Vorinitialisieren des STANDARD-HEXAEDER-ELEMENTES -----------------*/
  if(printok) {
    printf("\nBelege Standard-Hex.");
  } /* END OF PRINT */
  for(i=0; i<standard_element_hex.anz_node+anz_max_schnittpunkte; i++)
    {
      standard_element_hex.node[i].punktnr             = -1;
      standard_element_hex.node[i].flag                = 'U';
      for(j=0; j<3; j++)
	{ 
	  standard_element_hex.node[i].kante[j].nachbar    = -1; 
	  standard_element_hex.node[i].kante[j].flaeche[0] = -1;
	  standard_element_hex.node[i].kante[j].flaeche[1] = -1;
	} 
    }

  standard_element_hex.node[0].kante[0].nachbar    = 1; /* Punkt 0 */
  standard_element_hex.node[0].kante[0].flaeche[0] = 1;
  standard_element_hex.node[0].kante[0].flaeche[1] = 5;
  standard_element_hex.node[0].kante[1].nachbar    = 3;
  standard_element_hex.node[0].kante[1].flaeche[0] = 4;
  standard_element_hex.node[0].kante[1].flaeche[1] = 5;
  standard_element_hex.node[0].kante[2].nachbar    = 4;
  standard_element_hex.node[0].kante[2].flaeche[0] = 1;
  standard_element_hex.node[0].kante[2].flaeche[1] = 4;
  
  standard_element_hex.node[1].kante[0].nachbar    = 0; /* Punkt 1 */
  standard_element_hex.node[1].kante[0].flaeche[0] = 1;
  standard_element_hex.node[1].kante[0].flaeche[1] = 5;
  standard_element_hex.node[1].kante[1].nachbar    = 2;
  standard_element_hex.node[1].kante[1].flaeche[0] = 2;
  standard_element_hex.node[1].kante[1].flaeche[1] = 5;
  standard_element_hex.node[1].kante[2].nachbar    = 5;
  standard_element_hex.node[1].kante[2].flaeche[0] = 1;
  standard_element_hex.node[1].kante[2].flaeche[1] = 2;

  standard_element_hex.node[2].kante[0].nachbar    = 1; /* Punkt 2 */
  standard_element_hex.node[2].kante[0].flaeche[0] = 2;
  standard_element_hex.node[2].kante[0].flaeche[1] = 5;
  standard_element_hex.node[2].kante[1].nachbar    = 3;
  standard_element_hex.node[2].kante[1].flaeche[0] = 3;
  standard_element_hex.node[2].kante[1].flaeche[1] = 5;
  standard_element_hex.node[2].kante[2].nachbar    = 6;
  standard_element_hex.node[2].kante[2].flaeche[0] = 2;
  standard_element_hex.node[2].kante[2].flaeche[1] = 3;

  standard_element_hex.node[3].kante[0].nachbar    = 0; /* Punkt 3 */
  standard_element_hex.node[3].kante[0].flaeche[0] = 4;
  standard_element_hex.node[3].kante[0].flaeche[1] = 5;
  standard_element_hex.node[3].kante[1].nachbar    = 2;
  standard_element_hex.node[3].kante[1].flaeche[0] = 3;
  standard_element_hex.node[3].kante[1].flaeche[1] = 5;
  standard_element_hex.node[3].kante[2].nachbar    = 7;
  standard_element_hex.node[3].kante[2].flaeche[0] = 3;
  standard_element_hex.node[3].kante[2].flaeche[1] = 4;

  standard_element_hex.node[4].kante[0].nachbar    = 0; /* Punkt 4 */
  standard_element_hex.node[4].kante[0].flaeche[0] = 1;
  standard_element_hex.node[4].kante[0].flaeche[1] = 4;
  standard_element_hex.node[4].kante[1].nachbar    = 5;
  standard_element_hex.node[4].kante[1].flaeche[0] = 1;
  standard_element_hex.node[4].kante[1].flaeche[1] = 6;
  standard_element_hex.node[4].kante[2].nachbar    = 7;
  standard_element_hex.node[4].kante[2].flaeche[0] = 4;
  standard_element_hex.node[4].kante[2].flaeche[1] = 6;

  standard_element_hex.node[5].kante[0].nachbar    = 1; /* Punkt 5 */
  standard_element_hex.node[5].kante[0].flaeche[0] = 1;
  standard_element_hex.node[5].kante[0].flaeche[1] = 2;
  standard_element_hex.node[5].kante[1].nachbar    = 4;
  standard_element_hex.node[5].kante[1].flaeche[0] = 1;
  standard_element_hex.node[5].kante[1].flaeche[1] = 6;
  standard_element_hex.node[5].kante[2].nachbar    = 6;
  standard_element_hex.node[5].kante[2].flaeche[0] = 2;
  standard_element_hex.node[5].kante[2].flaeche[1] = 6;

  standard_element_hex.node[6].kante[0].nachbar    = 2; /* Punkt 6 */
  standard_element_hex.node[6].kante[0].flaeche[0] = 2;
  standard_element_hex.node[6].kante[0].flaeche[1] = 3;
  standard_element_hex.node[6].kante[1].nachbar    = 5;
  standard_element_hex.node[6].kante[1].flaeche[0] = 2;
  standard_element_hex.node[6].kante[1].flaeche[1] = 6;
  standard_element_hex.node[6].kante[2].nachbar    = 7;
  standard_element_hex.node[6].kante[2].flaeche[0] = 3;
  standard_element_hex.node[6].kante[2].flaeche[1] = 6;

  standard_element_hex.node[7].kante[0].nachbar    = 3; /* Punkt 7 */
  standard_element_hex.node[7].kante[0].flaeche[0] = 3;
  standard_element_hex.node[7].kante[0].flaeche[1] = 4;
  standard_element_hex.node[7].kante[1].nachbar    = 4;
  standard_element_hex.node[7].kante[1].flaeche[0] = 4;
  standard_element_hex.node[7].kante[1].flaeche[1] = 6;
  standard_element_hex.node[7].kante[2].nachbar    = 6;
  standard_element_hex.node[7].kante[2].flaeche[0] = 3;
  standard_element_hex.node[7].kante[2].flaeche[1] = 6;

  /* STANDARD-ELEMENT PENTAEDER ----------------- */
  standard_element_pen.anz_area = 5;
  standard_element_pen.anz_node = 6;            /* STANDARD PENTAEDER */
  anz_max_schnittpunkte = (standard_element_pen.anz_node/2) + 2;

  /* Speicherplatz allozieren fuer die 6+5 Dreibeine ------------------ */
  standard_element_pen.node=(t_uni_knoten *)ALLOC(sizeof(t_uni_knoten)
			   *(standard_element_pen.anz_node + anz_max_schnittpunkte));

  if (standard_element_pen.node == NULL)
    {
      fprintf(stderr,"\nKein Speicher fuer Standard_Pen.");
      return -1;
    }

  /* Vorinitialisieren des STANDARD-PENTAEDER-ELEMENTES ----------------*/
  if(printok) {
    printf("\nBelege Standard-Pen.");
  } /* END OF PRINT */
  for(i=0; i<standard_element_pen.anz_node+anz_max_schnittpunkte; i++)
    {
      standard_element_pen.node[i].punktnr             = -1;
      standard_element_pen.node[i].flag                = 'U';
      for(j=0; j<3; j++)
	{ 
	  standard_element_pen.node[i].kante[j].nachbar    = -1; 
	  standard_element_pen.node[i].kante[j].flaeche[0] = -1;
	  standard_element_pen.node[i].kante[j].flaeche[1] = -1;
	} 
    }

  standard_element_pen.node[0].kante[0].nachbar    = 1; /* Punkt 0 */
  standard_element_pen.node[0].kante[0].flaeche[0] = 1;
  standard_element_pen.node[0].kante[0].flaeche[1] = 4;
  standard_element_pen.node[0].kante[1].nachbar    = 2;
  standard_element_pen.node[0].kante[1].flaeche[0] = 3;
  standard_element_pen.node[0].kante[1].flaeche[1] = 4;
  standard_element_pen.node[0].kante[2].nachbar    = 3;
  standard_element_pen.node[0].kante[2].flaeche[0] = 1;
  standard_element_pen.node[0].kante[2].flaeche[1] = 3;
  
  standard_element_pen.node[1].kante[0].nachbar    = 0; /* Punkt 1 */
  standard_element_pen.node[1].kante[0].flaeche[0] = 1;
  standard_element_pen.node[1].kante[0].flaeche[1] = 4;
  standard_element_pen.node[1].kante[1].nachbar    = 2;
  standard_element_pen.node[1].kante[1].flaeche[0] = 2;
  standard_element_pen.node[1].kante[1].flaeche[1] = 4;
  standard_element_pen.node[1].kante[2].nachbar    = 4;
  standard_element_pen.node[1].kante[2].flaeche[0] = 1;
  standard_element_pen.node[1].kante[2].flaeche[1] = 2;

  standard_element_pen.node[2].kante[0].nachbar    = 0; /* Punkt 2 */
  standard_element_pen.node[2].kante[0].flaeche[0] = 3;
  standard_element_pen.node[2].kante[0].flaeche[1] = 4;
  standard_element_pen.node[2].kante[1].nachbar    = 1;
  standard_element_pen.node[2].kante[1].flaeche[0] = 2;
  standard_element_pen.node[2].kante[1].flaeche[1] = 4;
  standard_element_pen.node[2].kante[2].nachbar    = 5;
  standard_element_pen.node[2].kante[2].flaeche[0] = 2;
  standard_element_pen.node[2].kante[2].flaeche[1] = 3;

  standard_element_pen.node[3].kante[0].nachbar    = 0; /* Punkt 3 */
  standard_element_pen.node[3].kante[0].flaeche[0] = 1;
  standard_element_pen.node[3].kante[0].flaeche[1] = 3;
  standard_element_pen.node[3].kante[1].nachbar    = 4;
  standard_element_pen.node[3].kante[1].flaeche[0] = 1;
  standard_element_pen.node[3].kante[1].flaeche[1] = 5;
  standard_element_pen.node[3].kante[2].nachbar    = 5;
  standard_element_pen.node[3].kante[2].flaeche[0] = 3;
  standard_element_pen.node[3].kante[2].flaeche[1] = 5;

  standard_element_pen.node[4].kante[0].nachbar    = 1; /* Punkt 4 */
  standard_element_pen.node[4].kante[0].flaeche[0] = 1;
  standard_element_pen.node[4].kante[0].flaeche[1] = 2;
  standard_element_pen.node[4].kante[1].nachbar    = 3;
  standard_element_pen.node[4].kante[1].flaeche[0] = 1;
  standard_element_pen.node[4].kante[1].flaeche[1] = 5;
  standard_element_pen.node[4].kante[2].nachbar    = 5;
  standard_element_pen.node[4].kante[2].flaeche[0] = 2;
  standard_element_pen.node[4].kante[2].flaeche[1] = 5;

  standard_element_pen.node[5].kante[0].nachbar    = 2; /* Punkt 5 */
  standard_element_pen.node[5].kante[0].flaeche[0] = 2;
  standard_element_pen.node[5].kante[0].flaeche[1] = 3;
  standard_element_pen.node[5].kante[1].nachbar    = 3;
  standard_element_pen.node[5].kante[1].flaeche[0] = 3;
  standard_element_pen.node[5].kante[1].flaeche[1] = 5;
  standard_element_pen.node[5].kante[2].nachbar    = 4;
  standard_element_pen.node[5].kante[2].flaeche[0] = 2;
  standard_element_pen.node[5].kante[2].flaeche[1] = 5;

  /* STANDARD-ELEMENT TETRAEDER ----------------- */
  standard_element_tet.anz_area = 4;  
  standard_element_tet.anz_node = 4;            /* STANDARD TETRAEDER */
  anz_max_schnittpunkte = (standard_element_tet.anz_node/2) + 2;

  /* Speicherplatz allozieren fuer die 4+4 Dreibeine ------------------ */
  standard_element_tet.node=(t_uni_knoten *)ALLOC(sizeof(t_uni_knoten)
						   *(standard_element_tet.anz_node + anz_max_schnittpunkte));

  if (standard_element_tet.node == NULL) 
    {
      fprintf(stderr,"\nKein Speicher fuer Standard_Tet!");
      return -1;
    }

  /* Vorinitialisieren des STANDARD-TETRAEDER-ELEMENTES ----------------*/
  if(printok) {
    printf("\nBelege Standard-Tet.");
  } /* END OF PRINT */

  for(i=0; i<standard_element_tet.anz_node+anz_max_schnittpunkte; i++)
    {
      standard_element_tet.node[i].punktnr             = -1;
      standard_element_tet.node[i].flag                = 'U';
      for(j=0; j<3; j++)
	{ 
	  standard_element_tet.node[i].kante[j].nachbar    = -1; 
	  standard_element_tet.node[i].kante[j].flaeche[0] = -1;
	  standard_element_tet.node[i].kante[j].flaeche[1] = -1;
	} 
    }

 /*  standard_element_tet.node[0].kante[0].nachbar    = 1; /\* Punkt 0 *\/ */
/*   standard_element_tet.node[0].kante[0].flaeche[0] = 1; */
/*   standard_element_tet.node[0].kante[0].flaeche[1] = 4; */
/*   standard_element_tet.node[0].kante[1].nachbar    = 2; */
/*   standard_element_tet.node[0].kante[1].flaeche[0] = 3; */
/*   standard_element_tet.node[0].kante[1].flaeche[1] = 4; */
/*   standard_element_tet.node[0].kante[2].nachbar    = 3; */
/*   standard_element_tet.node[0].kante[2].flaeche[0] = 1; */
/*   standard_element_tet.node[0].kante[2].flaeche[1] = 3; */
  
/*   standard_element_tet.node[1].kante[0].nachbar    = 0; /\* Punkt 1 *\/ */
/*   standard_element_tet.node[1].kante[0].flaeche[0] = 1; */
/*   standard_element_tet.node[1].kante[0].flaeche[1] = 4; */
/*   standard_element_tet.node[1].kante[1].nachbar    = 2; */
/*   standard_element_tet.node[1].kante[1].flaeche[0] = 2; */
/*   standard_element_tet.node[1].kante[1].flaeche[1] = 4; */
/*   standard_element_tet.node[1].kante[2].nachbar    = 3; */
/*   standard_element_tet.node[1].kante[2].flaeche[0] = 1; */
/*   standard_element_tet.node[1].kante[2].flaeche[1] = 2; */

/*   standard_element_tet.node[2].kante[0].nachbar    = 0; /\* Punkt 2 *\/ */
/*   standard_element_tet.node[2].kante[0].flaeche[0] = 3; */
/*   standard_element_tet.node[2].kante[0].flaeche[1] = 4; */
/*   standard_element_tet.node[2].kante[1].nachbar    = 1; */
/*   standard_element_tet.node[2].kante[1].flaeche[0] = 2; */
/*   standard_element_tet.node[2].kante[1].flaeche[1] = 4; */
/*   standard_element_tet.node[2].kante[2].nachbar    = 3; */
/*   standard_element_tet.node[2].kante[2].flaeche[0] = 2; */
/*   standard_element_tet.node[2].kante[2].flaeche[1] = 3; */

/*   standard_element_tet.node[3].kante[0].nachbar    = 0; /\* Punkt 3 *\/ */
/*   standard_element_tet.node[3].kante[0].flaeche[0] = 1; */
/*   standard_element_tet.node[3].kante[0].flaeche[1] = 3; */
/*   standard_element_tet.node[3].kante[1].nachbar    = 1; */
/*   standard_element_tet.node[3].kante[1].flaeche[0] = 1; */
/*   standard_element_tet.node[3].kante[1].flaeche[1] = 2; */
/*   standard_element_tet.node[3].kante[2].nachbar    = 2; */
/*   standard_element_tet.node[3].kante[2].flaeche[0] = 2; */
/*   standard_element_tet.node[3].kante[2].flaeche[1] = 3; */

  standard_element_tet.node[0].kante[0].nachbar    = 1; /* Punkt 0 */
  standard_element_tet.node[0].kante[0].flaeche[0] = 3;
  standard_element_tet.node[0].kante[0].flaeche[1] = 4;
  standard_element_tet.node[0].kante[1].nachbar    = 2;
  standard_element_tet.node[0].kante[1].flaeche[0] = 1;
  standard_element_tet.node[0].kante[1].flaeche[1] = 3;
  standard_element_tet.node[0].kante[2].nachbar    = 3;
  standard_element_tet.node[0].kante[2].flaeche[0] = 1;
  standard_element_tet.node[0].kante[2].flaeche[1] = 4;
  
  standard_element_tet.node[1].kante[0].nachbar    = 0; /* Punkt 1 */
  standard_element_tet.node[1].kante[0].flaeche[0] = 3;
  standard_element_tet.node[1].kante[0].flaeche[1] = 4;
  standard_element_tet.node[1].kante[1].nachbar    = 2;
  standard_element_tet.node[1].kante[1].flaeche[0] = 2;
  standard_element_tet.node[1].kante[1].flaeche[1] = 3;
  standard_element_tet.node[1].kante[2].nachbar    = 3;
  standard_element_tet.node[1].kante[2].flaeche[0] = 2;
  standard_element_tet.node[1].kante[2].flaeche[1] = 4;

  standard_element_tet.node[2].kante[0].nachbar    = 0; /* Punkt 2 */
  standard_element_tet.node[2].kante[0].flaeche[0] = 1;
  standard_element_tet.node[2].kante[0].flaeche[1] = 3;
  standard_element_tet.node[2].kante[1].nachbar    = 1;
  standard_element_tet.node[2].kante[1].flaeche[0] = 2;
  standard_element_tet.node[2].kante[1].flaeche[1] = 3;
  standard_element_tet.node[2].kante[2].nachbar    = 3;
  standard_element_tet.node[2].kante[2].flaeche[0] = 1;
  standard_element_tet.node[2].kante[2].flaeche[1] = 2;

  standard_element_tet.node[3].kante[0].nachbar    = 0; /* Punkt 3 */
  standard_element_tet.node[3].kante[0].flaeche[0] = 1;
  standard_element_tet.node[3].kante[0].flaeche[1] = 4;
  standard_element_tet.node[3].kante[1].nachbar    = 1;
  standard_element_tet.node[3].kante[1].flaeche[0] = 2;
  standard_element_tet.node[3].kante[1].flaeche[1] = 4;
  standard_element_tet.node[3].kante[2].nachbar    = 2;
  standard_element_tet.node[3].kante[2].flaeche[0] = 1;
  standard_element_tet.node[3].kante[2].flaeche[1] = 2;

  /* -------------------------------------------------------------------*/
  /* ----------------------F A Z I T -----------------------------------*/
  /* Wir haben jetzt 3 Speicherbloecke namens:
     standard_element_hex,	(fuer das HEXAEDER)
     standard_element_pen und	(fuer das PENTAEDER)
     standard_element_tet.	(fuer das TETRAEDER)
   
     Diese Speicherbloecke bestehen aus:
     einem int - Wert, der die Anzahl der Knoten des Elementes angibt &
     einem Block Knoten. (Eck-Knoten + moegliche Schnittknoten)
   
     Ein Knoten in dem Block enthaelt Informationen, welche Nachbarn 
     seine 3 Kanten haben, und an welchen Flaechen sie liegen
   
     Diese Informationen sind bei allen Elementtypen (HEXAEDER,PENTAEDER
     und TETRAEDER) gleich. Welche Punktnummern die Punkte haben, haengt
     natuerlich von der Geometrie ab. Sie werden spaeter dazugefuegt    */
  /* -------------------------------------------------------------------*/    


  /* Schleife ueber alle STOFFE --------------------------------------------*/

  for (stoffgebiet_index=1; stoffgebiet_index<=g_geodat.nstoff; stoffgebiet_index++)  
    {

      /* ---------------Schleife ueber alle Elemente des Stoffes ------------ */

      element_index = g_stoffgebiete[stoffgebiet_index-1].anfang_hex+1; 
      stoffaktuell = TRUE;
      while(stoffaktuell)
	{  
	  if(element_index == g_stoffgebiete[stoffgebiet_index-1].ende_hex+1)
	    element_index = g_stoffgebiete[stoffgebiet_index-1].anfang_pen+1;
	  if(element_index == g_stoffgebiete[stoffgebiet_index-1].ende_pen+1)
	    element_index = g_stoffgebiete[stoffgebiet_index-1].anfang_tet+1;
	  if(element_index == g_stoffgebiete[stoffgebiet_index-1].ende_tet)
	    stoffaktuell = FALSE;
#ifndef PRINTOUT
	  /*
	    if(element_index==134) printok = TRUE;
	    else printok = FALSE;
	  */
#endif

	  /* ---------------Was fuer ein ELEMENTTYP ? ------------------------- */
	  if(((element_index-1)>=g_stoffgebiete[stoffgebiet_index-1].anfang_hex)&&
	     ((element_index-1)<g_stoffgebiete[stoffgebiet_index-1].ende_hex))
	    {
	      /* Element ist ein Hexaeder -------------------------------------- */
	      hilfsstandardelement = &standard_element_hex;
	      if(printok) {
		printf("\nElement %d aus Stoff %d ist ein Hexaeder",element_index,stoffgebiet_index);
	      } /* END OF PRINT */
	    }
	  else
	    {
	      if(((element_index-1)>=g_stoffgebiete[stoffgebiet_index-1].anfang_pen)&&
		 ((element_index-1)<g_stoffgebiete[stoffgebiet_index-1].ende_pen))
		{
		  hilfsstandardelement = &standard_element_pen;
		  if(printok) {
		    printf("\nElement %d aus Stoff %d ist ein Pentaeder",element_index,stoffgebiet_index);
		  } /* END OF PRINT */
		}
	      else
		{
		  hilfsstandardelement = &standard_element_tet;
		  if(printok) {
		    printf("\nElement %d aus Stoff %d ist ein Tetraeder",element_index,stoffgebiet_index);
		  } /* END OF PRINT */
		}
	    }


	  /* ---------------ARBEITS-Element erzeugen -------------------------- */


	  arbeits_element=(t_arbeits_element*)ALLOC(sizeof(t_arbeits_element));
	  if (arbeits_element == NULL)
	    {
	      fprintf(stderr,"\nKein Speicher fuers Arbeitselement!");
	      return(-1);
	    }
     
	  /* Wieviel Knoten braucht das Element (8,6,4!) ? -----------*/
	  arbeits_element->anz_node=hilfsstandardelement->anz_node;
	  anz_max_schnittpunkte = (arbeits_element->anz_node/2)+2;
     
	  /* Wieviel Flaechen braucht das Element maximal ? (6,5,4!)-----------*/
	  arbeits_element->anz_area=hilfsstandardelement->anz_area;
    
	  /* Platz fuer die Dreibeine (t_uni_knoten) --------------------- */
	  arbeits_element->node =(t_uni_knoten *) 
	    ALLOC((arbeits_element->anz_node + anz_max_schnittpunkte)*sizeof(t_uni_knoten));
	  if (arbeits_element->node == NULL)
	    {
	      fprintf(stderr,"\nKein Speicher fuer die Knoten des Arbeitselementes!");
	      return(-1);
	    }
     
	  /* NextZeiger raus! */                                         
	  arbeits_element->next = NULL;

	  /* Knoten des STANDARD Element's kopieren -- */
	  memcpy(arbeits_element->node, hilfsstandardelement->node,
		 sizeof(t_uni_knoten)
		 *(arbeits_element->anz_node+anz_max_schnittpunkte));

	  if(printok) {           
	    printf("\nStandard-Element in Arbeitselement kopiert.");
	  } /* END OF PRINT */

                            
	  /* ---------------GEOMETRIEABHAENGIGE Werte eintragen --------------- */

	  anz_to_clip=0;         /* nichtdarstellbare ('W') Knoten zaehlen */ 
	  j=0;                   /* darstellbaren Knoten ('D') zaehlen --- */
	  g=0;
     

	  /* alle Knoten abklappern -------------------------------- */                                    
	  for (k=0; k<arbeits_element->anz_node; k++)
	    {
	      p=g_element[element_index-1].pn[k]; /* akt. Pktnr. des i-ten Elements */
          
	      /* Punktnummer in Knoten-Struktur eintragen ------------------- */
	      arbeits_element->node[k].punktnr=p;
      
	      /*Je nach Position des Punktes, D(front) oder W(back) als Flag- */
	      if (p_punkte_zeichnen[p-1]=='D') 
		{
		  j++;
		  arbeits_element->node[k].flag = 'D';
		}
	      else
		{
		  if(p_punkte_zeichnen[p-1]=='W')
		    { 
		      arbeits_element->node[k].flag = 'W';
		      /* Bei 'W' ein wegzuclippender Punkt mehr -------------------*/
		      anz_to_clip++;
		    }
		  else
		    {
		      arbeits_element->node[k].flag = 'G'; 
		      g++;
		      anz_to_clip++;
		    }
		}

	      if(printok) {
		printf("\nPunkt %d hat %c - Flag.",p,arbeits_element->node[k].flag);
	      } /* END OF PRINT */
	    }   /* ende aller 8 HEX-Knoten */

	 
	  if(printok) {
	    printf("\n%d Knoten werden weggeschnitten.",anz_to_clip);
	  } /* END OF PRINT */
     
	  /* ----------------Wird Element ueberhaupt geschnitten ? -------------*/
     
	  if((j+g)==arbeits_element->anz_node)
	    {
	      g_element[element_index-1].zeichnen = 1;
	      if(printok) {
		printf("\nElement %d wird komplett gezeichnet.",element_index);
	      } /* END OF PRINT */

	      if(g>0)
		{
		  g_element[element_index-1].zeichnen = 3;
		  if(printok) {
		    printf("\nElement %d wird tangiret gezeichnet.",element_index);
		  } /* END OF PRINT */
	      
		}
	      if(arbeits_element->node!=NULL) FREE(arbeits_element->node);
	      if(arbeits_element!=NULL) FREE(arbeits_element);
	    }

	  

	  if(j==0)
	    {
	      g_element[element_index-1].zeichnen = 0; 
	      if(printok) {
		printf("\nElement %d wird komplett weggeschnitten.",element_index);
	      } /* END OF PRINT */
	      if(arbeits_element->node!=NULL) FREE(arbeits_element->node);
	      if(arbeits_element!=NULL) FREE(arbeits_element);
	    }

	  if((j>0)&&((anz_to_clip-g)>0))
	    {
	      /* Schneide-Algotithmus -----------------------------------------*/
	      g_element[element_index-1].zeichnen = 2; 
	      if(printok) {
		printf("\nElement %d wird geschnitten.",element_index);
	      } /* END OF PRINT */
      
	      /* ----------------SCHNITTPUNKTE bestimmen und eintragen -------------*/
     
	      /* Zuerst lokalen Platz fuer Schnittpunkte anlegen: */
	      schnittpunkt = (t_schnittpunkt*)
		ALLOC(anz_max_schnittpunkte * sizeof(t_schnittpunkt));
	      if (schnittpunkt==NULL)
		{
		  fprintf(stderr,"\nKein Speicher fuer lok. Schnittpunktliste!");
		  return(-1);
		}
     
	      reihe = (int*)ALLOC(anz_max_schnittpunkte * sizeof(int));

	      if (reihe == NULL)
		{
		  fprintf(stderr,"\nKein Speicher fuer lok. Reihenfeld");
		  return(-1);
		}

	      anz_cut=0;                 /* Schnittpunktzaehler auf null setzen */
	      m=0;                       /* */
     
	      if(printok) {
		printf("\nAnz_Node = %d",arbeits_element->anz_node);
	      } /* END OF PRINT */
     
	      /* Schleife ueber alle WEGZUCLIPPENDEN KNOTEN des akt. ARBEITS-El */
	      for (akt_knoten=0; akt_knoten<anz_to_clip; akt_knoten++) 
		{
		  /* Den naechsten wegzuclippenden Punkt finden: ----------------- */
		  while(arbeits_element->node[m].flag == 'D') 
		    {
		      if(printok) {
			printf("\nKnoten %d hat kein W- oder G- Flag",m);
		      } /* END OF PRINT */
		      m++;
		    }
		  if(printok) {
		    printf("\nChecke Knoten %d.",m);
		  } /* END OF PRINT */
		  /* m:= Index, des naechsten wegzuclippenden Punktes im -------- */
		  /* ----------------------------------geschnittenen Element 'i'- */
      
		  /* Schleife ueber die 3 zugehoerigen KANTEN des Punktes 'm' --- */
		  for (l=0; l<3; l++)
		    {
		      /* Index des Nachbarn an der Kante l holen ------------------- */
		      n = arbeits_element->node[m].kante[l].nachbar;

		      if(printok) {
			printf("\nTeste Nachbar %d mit Flag %c",l,
			       arbeits_element->node[n].flag);
		      } /* END OF PRINT */       
		      /* Falls Nachbar n darzustellen (D) ist, tritt Schnittpunkt auf*/
		      if (arbeits_element->node[n].flag == 'D')
			{
			  /* Merke die Punktnummern beider Endpunkte von Kante l ------ */
			  p1=arbeits_element->node[m].punktnr;
			  p2=arbeits_element->node[n].punktnr;
			  if(printok) { 
			    printf("\n%d-ter Schnittpunkt zw. %d (%d) und %d (%d) auf Kante %d."
				   ,anz_cut,m,p1,n,p2,l);			
			  } /* END OF PRINT */ 
			  /* Schnittpunkt der Strecke p1->p2 mit der Sch-Ebene berechnen,
			     in Schnittpunktliste eintragen, und neue Punktnummer erhalten */
			  /* Diese Routine muessen wir noch anpassen: */

			  /* ggf hier Offset des letzten Punktes addieren */

			  neue_schnittpunkt_nr =
			    teile_strecke(p1,p2,&schnittpunkt[anz_cut],
					  arbeits_element->node[m].flag) + g_geodat.npunkt +1; 
			  if(printok) {
			    printf("\nKoordinaten: x=%5f y=%5f z=%5f",schnittpunkt[anz_cut].d3[0],
				   schnittpunkt[anz_cut].d3[1],schnittpunkt[anz_cut].d3[2]); 
			  } /* END OF PRINT */
        
			  /* So, jetzt stehen die Schnittpunktdaten im Feld schnittpunkt */
			  /* Schnittpunkt als neuen Knoten eintragen: ------------------ */
			  knoten_index = arbeits_element->anz_node+anz_cut;
			  if(printok) {
			    printf("\nneuer Knoten: %d",knoten_index);
			    printf("\nneue Schnittpunkt-Nr: %d",neue_schnittpunkt_nr);
			  } /* END OF PRINT */
        
			  /* Punktnummer des Schnittpunktes eintragen ------------------ */
			  arbeits_element->node[knoten_index].punktnr = neue_schnittpunkt_nr;
			  /* Das Flag wird auf 'S' fuer Schnittpunkt gesetzt ----------- */
			  arbeits_element->node[knoten_index].flag    = 'S';
        
			  /* Der Nachbar des Schnittpunktes ist der D-Knoten (n) ------- */
			  arbeits_element->node[knoten_index].kante[0].nachbar = n;

			  /* Die Flaechen an der Kante sind ja die Selben -------------- */
			  arbeits_element->node[knoten_index].kante[0].flaeche[0]=
                            arbeits_element->node[m].kante[l].flaeche[0];
			  arbeits_element->node[knoten_index].kante[0].flaeche[1]=
                            arbeits_element->node[m].kante[l].flaeche[1];
        
			  /* Neuen Schnittpunkt an den "D"-Knoten (n) haengen ---------- */
			  z=0; 
			  while (arbeits_element->node[n].kante[z].nachbar != m) z++;
			  if(printok) {
			    printf("\nDie %d-te Kante geht zum Schnittpunkt %d",z,
				   neue_schnittpunkt_nr);
			  } /* END OF PRINT */
			  arbeits_element->node[n].kante[z].nachbar = knoten_index;
        
			  /* So, jetzt ist der Schnittpunkt schon mit einer Kante bestimmt*/  
			  /* Die anderen beiden Kanten muessen zu den anderen Schnittpunk-*/
			  /* ten gehen */
        
			  /* Schnittpunktzaehler (fuer dieses Element) erhoehen -------- */
			  anz_cut++;
			} /* Ende if Flag = D */
		    }  /* Ende Schleife l ueber Kanten */
      
		  /* Hier haben wir alle 3 Kanten von m aus abgeklappert und zw. - */
		  /* 0 und 3 Schnittpunkte gefunden und ins node-feld eingetragen. */
		  /* Jetzt koennen wir zum naechsten 'W' - Knoten ---------------- */
		  m++;   
		}   /* Ende akt_knoten ueber Knoten */
	      /* alle 'W'bzw 'G' Knoten des Elementes sind abgeklappert ------- */
     
	      /*---------------------------------------------------------------------*/
	      /*-----------------------F A Z I T : ----------------------------------*/
	      /*---------------------------------------------------------------------*/
     
	      /* Jetzt haben wir zwar alle Schnittpunkte gefunden, indem wir die 
		 geometrieabhaengigen Daten in unser Standard-Element eingetragen
		 haben und dann die Kanten von 'W' Knoten nach 'D' Knoten untersucht
		 haben, aber diese neuen Schnittpunkte haengen jetzt nur an einem
		 (D)-Knoten. 
		 Grundvoraussetzung fuer das Modell ist aber, dass jeder Knoten genau
		 3 Nachbarn hat. Mit anderen Worten uns fehlen 2 Kantenbeschreibungen.
		 Es ist aber so, dass das genau auch wieder Schnittpunke sind. Sie
		 spannen naemlich die Schnittflaeche auf !

		 Also bilden wir die konvexe Huelle der Schnittpunkte und bekommen so
		 eine Winkelreihenfolge, die uns wiederum fuer jeden Schnittpunkt einen
		 Vorgaenger und einen Nachfolger liefert. Das sind genau die beiden 
		 noch fehlenden Nachbarn.	                                       	*/
	      
              
	      if(printok) {                             
		printf("\nBilde jetzt konvexe Huelle.");
	      } /* END OF PRINT */

	      /* Schnittflaeche auf den Koordinaten Ursprung verschieben: ----------- */
	      /* Schleife ueber alle Schnittpunkte : -------------------------------- */

	      sub_x = sub_y = sub_z = 0.0;
	      for(j=0;j<anz_cut;j++)
		{
		  sub_x = sub_x + schnittpunkt[j].d3[0];
		  sub_y = sub_y + schnittpunkt[j].d3[1];
		  sub_z = sub_z + schnittpunkt[j].d3[2];  
  
		}
	      sub_x = sub_x / (float)anz_cut;
	      sub_y = sub_y / (float)anz_cut;
	      sub_z = sub_z / (float)anz_cut;
 
 
	      /* Mittelwert abziehen: */
	      for(j=0;j<anz_cut;j++)
		{
		  schnittpunkt[j].rot[0]=schnittpunkt[j].d3[0]-sub_x;
		  schnittpunkt[j].rot[1]=schnittpunkt[j].d3[1]-sub_y;
		  schnittpunkt[j].rot[2]=schnittpunkt[j].d3[2]-sub_z;
		}
 
 
	      /* Normalenvektor zwischen S1 und S2 bilden -------------------------- */
	      nx= g_transform.norm[0];
	      ny= g_transform.norm[1];
	      nz= g_transform.norm[2];
	      if ((ny == 0.0) && (nz == 0.0))
		{
		  /* Einheitsmatrix */
		  rsx = 0.0;
		  rcx = 1.0;
		}
	      else
		{
		  /* Winkel, um den um X gedreht wird: --------------------------------*/
		  /* Winkel zw. Normalenvektor und XZ-Ebene ---------------------------*/
		  ll  = sqrt(ny*ny + nz*nz);
		  rcx = sqrt((nz/ll)*(nz/ll));
		  if(nz<0.0) rcx = -rcx; 
		  rsx = sqrt(1-rcx*rcx);
		  if(ny<0.0) rsx = -rsx;
		}
	      /* Rotationsmatrix um X: */
	      rxx = 1.0;
	      rxy = 0.0;
	      rxz = 0.0;
	      ryx = 0.0;
	      ryy = rcx;
	      ryz = -rsx;
	      rzx = 0.0;
	      rzy = rsx;
	      rzz = rcx;

	      /* Normalenvektor wird in die XZ-Ebene projeziert ----------------------*/ 
	      hx = rxx*nx + rxy*ny + rxz*nz;
	      hy = ryx*nx + ryy*ny + ryz*nz;
	      hz = rzx*nx + rzy*ny + rzz*nz;

	      if(printok) {
		printf("\nUm X-Winkel gedrehter NV: %5f %5f %5f",hx,hy,hz);
	      } /* END OF PRINT */

	      /* Winkel, um den um Y gedreht wird: -----------------------------------*/
	      if ( hx == 0.0)
		{
		  rcy = 1.0;
		  rsy = 0.0;
		}
	      else
		{
		  rcy = sqrt(hz*hz);
		  if(hz<0.0) rcy = -rcy; 
		  rsy = sqrt(1-hz*hz);
		  if(hx>0.0) rsy = -rsy;
		}
	      /* Jetzt alle Punkte von 0 bis anz_cut-1 um rotx und dann um roty drehen*/

	      for (j=0;j<anz_cut;j++)
		{
		  /* Zuerst die Rotationsmatritzen Bilden: */
		  /* Rotationsmatrix um X: OpenGL verwendet die 'transponierte' Matrix */
		  rxx = 1.0;
		  rxy = 0.0;
		  rxz = 0.0;
		  ryx = 0.0;
		  ryy = rcx;
		  ryz = -rsx;
		  rzx = 0.0;
		  rzy = rsx;
		  rzz = rcx;

		  hx = rxx*schnittpunkt[j].rot[0]+rxy*schnittpunkt[j].rot[1]
		    +rxz*schnittpunkt[j].rot[2];
		  hy = ryx*schnittpunkt[j].rot[0]+ryy*schnittpunkt[j].rot[1]
		    +ryz*schnittpunkt[j].rot[2];
		  hz = rzx*schnittpunkt[j].rot[0]+rzy*schnittpunkt[j].rot[1]
		    +rzz*schnittpunkt[j].rot[2];


		  schnittpunkt[j].rot[0] = hx;
		  schnittpunkt[j].rot[1] = hy;
		  schnittpunkt[j].rot[2] = hz;

		  /* Rotationsmatrix um Y: */
		  rxx = rcy;
		  rxy = 0.0;
		  rxz = rsy;
		  ryx = 0.0;
		  ryy = 1.0;
		  ryz = 0.0;
		  rzx = -rsy;
		  rzy = 0.0;
		  rzz = rcy;

		  hx = rxx*schnittpunkt[j].rot[0]+rxy*schnittpunkt[j].rot[1]
		    +rxz*schnittpunkt[j].rot[2];
		  hy = ryx*schnittpunkt[j].rot[0]+ryy*schnittpunkt[j].rot[1]
		    +ryz*schnittpunkt[j].rot[2];
		  hz = rzx*schnittpunkt[j].rot[0]+rzy*schnittpunkt[j].rot[1]
		    +rzz*schnittpunkt[j].rot[2];
		  schnittpunkt[j].rot[0] = hx;
		  schnittpunkt[j].rot[1] = hy;
		  schnittpunkt[j].rot[2] = hz;
		  if(printok) { 
		    printf("\ngedrehter Schnittpunkt: %d %5f %5f %5f",j,hx,hy,hz);
		  } /* END OF PRINT */
		}

	      /* Jetzt ist die Schnittebene auf die XY Ebene projeziert.  */
	      /* So, dann koennen wir die Z-Komponente endlich ignorieren */

	      /* 2D-Flaeche auf den Koordinaten Ursprung verschieben: ----------- */
	      /* Schleife ueber alle Schnittpunkte : -------------------------------- */

	      sub_x = sub_y = 0.0;
	      for(j=0;j<anz_cut;j++)
		{
		  sub_x = sub_x + schnittpunkt[j].rot[0];
		  sub_y = sub_y + schnittpunkt[j].rot[1];
		}
	      sub_x = sub_x / (float)anz_cut;
	      sub_y = sub_y / (float)anz_cut;
 
 
	      /* Mittelwert abziehen: */
	      for(j=0;j<anz_cut;j++)
		{
		  schnittpunkt[j].rot[0] = schnittpunkt[j].rot[0]-sub_x;
		  schnittpunkt[j].rot[1] = schnittpunkt[j].rot[1]-sub_y;
		}
 

	      /* Schleife ueber S(0) bis S(anz_cut-1) ------------------------------- */
	      for (j=0;j<anz_cut;j++)
		{
		  rotx = schnittpunkt[j].rot[0];
		  roty = schnittpunkt[j].rot[1];
		  ll   = sqrt(rotx*rotx + roty*roty);

		  /* Winkel zwischen X-Achse und 2D-Vektor zu S(j) ausrechnen ---------- */
		  if ((rotx > 0) && (roty > 0 ))
		    schnittpunkt[j].winkel = (180.0/PI)*acosf(rotx/ll);
		  if ((rotx <= 0) && (roty > 0))
		    schnittpunkt[j].winkel = 180.0 -(180.0/PI) * acosf(-rotx/ll);
		  if ((rotx <= 0) && (roty <= 0))
		    schnittpunkt[j].winkel = 180.0 +(180.0/PI) * acosf(-rotx/ll);
		  if ((rotx > 0) && (roty <= 0))
		    schnittpunkt[j].winkel = 360.0 -(180.0/PI) * acosf(rotx/ll);

		  if (schnittpunkt[j].winkel>359.995) schnittpunkt[j].winkel = 0.0;
		 
		  if (printok)
		    printf("\nWinkel(%d): %5f mit Flag: %d",
			   j,schnittpunkt[j].winkel,schnittpunkt[j].visit);
		}

	      /* Nach Winkel sortieren ---------------------------------------------- */
	      for (k=0;k<anz_cut;k++)
		{
		  alpha_min = 360.0;
		  for (j=0;j<anz_cut;j++)
		    {
		      if ((schnittpunkt[j].winkel<=alpha_min)&&(!schnittpunkt[j].visit))
			{
			  minimum = j; 
			  alpha_min =  schnittpunkt[j].winkel;
			}
		    }

		  if (printok) {
		    printf("\nPosition:%d Schnittpunkt %d",k,minimum);
		    printf("\nDer kleinsten Winkel hat %d mit %f Grad.",minimum,
			   schnittpunkt[minimum].winkel);
		  } /* END OF PRINT */ 

		  schnittpunkt[minimum].visit = TRUE;
		  reihe[k]=minimum;
		}

	      /* Jetzt Reihenfolge auf gleiche Koordinaten checken: */
	      start = arbeits_element->anz_node;
	      for(j=0;j<anz_cut-1;j++)
		{
		  k = (j+1)%anz_cut; 
		  l = (j+2)%anz_cut;
		  m = (j+3)%anz_cut;
 
		  j0 = arbeits_element->node[reihe[j]+start].kante[0].flaeche[0];
		  j1 = arbeits_element->node[reihe[j]+start].kante[0].flaeche[1];
		  k0 = arbeits_element->node[reihe[k]+start].kante[0].flaeche[0];
		  k1 = arbeits_element->node[reihe[k]+start].kante[0].flaeche[1]; 
		  l0 = arbeits_element->node[reihe[l]+start].kante[0].flaeche[0];
		  l1 = arbeits_element->node[reihe[l]+start].kante[0].flaeche[1];
		  m0 = arbeits_element->node[reihe[m]+start].kante[0].flaeche[0];
		  m1 = arbeits_element->node[reihe[m]+start].kante[0].flaeche[1]; 
 
		  x = schnittpunkt[reihe[j]].winkel - schnittpunkt[reihe[k]].winkel;
		  if (x<0.0) x=-x;
		  if (x<EPSILON_SAME)
		    {
		      /* Hier haben wir's mit Doppelten Schnittpunkten zu tun:
			     Testen, ob Flaechen nicht passen: */
  
		      y = schnittpunkt[reihe[l]].winkel - schnittpunkt[reihe[m]].winkel;
		      if (y<0.0) y=-y;
		      if ((x<EPSILON_SAME)&&(anz_cut>3))
			{
			  /* Doppelt, Doppelt: */
			  if(((k0!=l0)&&(k0!=l1)&&(k1!=l0)&&(k1!=l1))&&
			     ((k0!=m0)&&(k0!=m1)&&(k1!=m0)&&(k1!=m1)))
			    {
			      /* Ersten Pack drehen: */
			      g = reihe[j];
			      reihe[j]=reihe[k];
			      reihe[k]=g;
			
			      if(printok) {
				printf("\nFlip %d mit %d",j,k);
			      } /* END OF PRINT */
			      
			      /* Und Flaechen tauschen: */
			      g0 = j0;
			      g1 = j1;
			      j0 = k0;
			      j1 = k1;
			      k0 = g0;
			      k1 = g1;
			    }
			 
			  if ((k0!=l0)&&(k0!=l1)&&(k1!=l0)&&(k1!=l1))
			    {
			      /* Zweiten Pack drehen: */
			      g = reihe[l];
			      reihe[l] = reihe[m];
			      reihe[m] = g;
			      if(printok) {
				printf("\nFlip %d mit %d",l,m);
			      } /* END OF PRINT */
			    }
			  j+=2;
			}
		      else
			{
			  /* Doppelt, Einfach: */
			  if(((k0!=l0)&&(k0!=l1))&&((k1!=l0)&&(k1!=l1)))
			    {
			      g = reihe[j];
			      reihe[j]=reihe[k];
			      reihe[k]=g;
			      
			      if(printok) {
				printf("\nFlip %d mit %d",j,k);
			      } /* END OF PRINT */
			      
			    }
			  j++;
			} 
		    }
		  else
		    {
		      x = schnittpunkt[reihe[k]].winkel - schnittpunkt[reihe[l]].winkel;
		      if (x<0.0) x=-x;
		      if (x<EPSILON_SAME)
			{
			  /* Einfach, Doppelt */
			  if ((j0!=k0)&&(j0!=k1)&&(j1!=k0)&&(j1!=k1))
			    {
			      g = reihe[k];
			      reihe[k]=reihe[l];
			      reihe[l]=g;
			     
			      if(printok) {
				printf("\nFlip %d mit %d",k,l);
			      } /* END OF PRINT */

			    }
			  j++;
			}
		      else
			{
			  /* Einfach, Einfach */
			}
		    }
		  if (printok) {
		    printf("\nJ:%d",j);
		  } /* END OF PRINT */

		}
  

	      /* Jetzt die Kanten Verbinden */
	      if (printok) {
		printf("\nKanten verbinden");
	      } /* END OF PRINT */

	      start = arbeits_element->anz_node;
	      for (j=0;j<anz_cut;j++)
		{
		  z=0;
		  while (reihe[z]!=j) z++;
		  /* Bestimme Vorgaenger- und Nachfolgerschnittpunkt (n1,n2) */
		  n1 = reihe[((z+anz_cut-1)%anz_cut)];
		  n2 = reihe[((z+anz_cut+1)%anz_cut)];
	
		  if (printok) {
		    printf("\nVorgaenger:%d Nachfolger:%d -Schnittpunkt",n1,n2);
		  } /* END OF PRINT */

		  /* Verkette die fehlenden 2 Kanten der Schnittpunkte ----- */
		  arbeits_element->node[start+j].kante[1].nachbar   = n1+start;
		  arbeits_element->node[start+j].kante[1].flaeche[0]= 0;
		  arbeits_element->node[start+j].kante[2].nachbar   = n2+start;
		  arbeits_element->node[start+j].kante[2].flaeche[0]= 0;

		  /* Weche Flaechen liegen noch an den Kanten 1 & 2 ? */
  
		  f0 = arbeits_element->node[start+j].kante[0].flaeche[0];
		  f1 = arbeits_element->node[start+j].kante[0].flaeche[1];
  
		  /* In f0 und f1 sind die zu verteilenden Flaechen. ----- */
  
		  /* An welchem D-Punkt liegt der Vorgaenger ? ----------- */
  
		  g  = arbeits_element->node[start+n1].kante[0].nachbar;
  
		  g0 = arbeits_element->node[g].kante[0].nachbar;
		  g1 = arbeits_element->node[g].kante[1].nachbar; 
		  g2 = arbeits_element->node[g].kante[2].nachbar; 
  
		  if (g0==n1+start) f2=0;
		  if (g1==n1+start) f2=1;
		  if (g2==n1+start) f2=2;
  
		  /* So, vom D-Punkt g geht die f2-te Kante zu n1. -------- */
  
		  if ((arbeits_element->node[g].kante[f2].flaeche[0]==f0)||
		     (arbeits_element->node[g].kante[f2].flaeche[1]==f0))
		    {
		      /* Dann liegt die Kante[1] von j an f0 und Kante[2] an f1 */
		      arbeits_element->node[start+j].kante[1].flaeche[1]=f0;
		      arbeits_element->node[start+j].kante[2].flaeche[1]=f1;
		    }
		  else
		    {
		      /* Sonst umgekehrt: ------------------------------------ */
		      arbeits_element->node[start+j].kante[1].flaeche[1]=f1;
		      arbeits_element->node[start+j].kante[2].flaeche[1]=f0;
		    }

		  if (printok) {  
		    printf("\nFlaeche 1 an Kante 1",arbeits_element->node[start+j].kante[1].flaeche[1]);
		    printf("\nFlaeche 1 an Kante 2",arbeits_element->node[start+j].kante[2].flaeche[1]);
		  } /* END OF PRINT */

		}  /* ENDE Kanten verbinden */

	 
	      /* So, jetzt koennen Schnittpunktliste und reihe wieder freigegeben werden*/
	      FREE(schnittpunkt);
	      FREE(reihe);

	      /* So, jetzt sind alle Schnittpunkte komplett verknuepft */
	      /* Jetzt muesste man das Arbeitselement entruempeln und 
		 dann in eine globale Liste haengen, damit der Zeichenalgorithmus
		 darauf zugreifen kann. */

	      /* ---------- FLAECHEN-Nummerierung im ARBEITS-Element erneuern -----------*/
	      /* +1 := hinzugekommene Schnittflaeche */
	      hilf = arbeits_element->anz_area +1;

	      /* Hilfsfeld anlegen ------------------------------------------------ */
	      hflaeche = (int*)ALLOC(hilf*sizeof(int));

	      /* Vorinitialisieren des Flaechenfelds */
	      for (i=0; i<hilf; i++) hflaeche[i]=0;

	      /* abklappern des AKTUELLEN ARBEITS-ELEMENTS */
	      hflaeche[0] = 1;    /* Flaeche 0 := Schnittflaeche */
	      for (i=0; i<arbeits_element->anz_node; i++)
		{
		  if (arbeits_element->node[i].flag == 'D')
		    {
		      for (j=0; j<3; j++)
			{
			  hflaeche[ arbeits_element->node[i].kante[j].flaeche[0] ] = 1;
			  hflaeche[ arbeits_element->node[i].kante[j].flaeche[1] ] = 1;
			} /* ENDE for j #Kante */
		    }  
		}  /* ENDE for i #Punkte */

	      if(printok) {

		printf("\nhflaeche: ");
		for(i=0;i<hilf;i++) printf(",%d ",hflaeche[i]);

	      } /* END OF PRINT */ 

	      /* Im 2.-ten Durchlauf die 'neue' Reihenfolge der Flaechen aktualisieren */
	      z = 1; 
	      for (i=1; i<hilf; i++)
		{
		  if (hflaeche[i]==1)
		    {
		      z++;
		      hflaeche[i]=z;
		    }
		  else
		    hflaeche[i] = -1;
		} /* ENDE hilf=anz_area +1 */

	      /* Alte Flaechennummern in Arbeitselement eintragen */
	      arbeits_element->hflaeche = hflaeche;

	      /* Neue Flaechennummern in Arbeitselement eintragen: */
	      for (i=0; i<arbeits_element->anz_node+anz_cut; i++)
		{
		  if ((arbeits_element->node[i].flag != 'W')&&
		      (arbeits_element->node[i].flag != 'G'))
		    {
		      for (j=0; j<3; j++)
			{
			  arbeits_element->node[i].kante[j].flaeche[0]  = 
			    hflaeche[ arbeits_element->node[i].kante[j].flaeche[0] ];
			  arbeits_element->node[i].kante[j].flaeche[1]  = 
			    hflaeche[ arbeits_element->node[i].kante[j].flaeche[1] ];
			}  /* ENDE for j #Kante */
		    }  /* ENDE if != 'W'&&'G' */
		}  /* ENDE for i #Punkte + Schnittpunkte */

	      /* neue Anzahl PUNKTE und FLAECHEN pro Element aktualisieren */
	      arbeits_element->anz_node += anz_cut;
	      
 
	      if(printok) {
		printf("\nneue # Punkte/Schnitt-Element: %d ",
		       arbeits_element->anz_node);
	      } /* END OF PRINT */

	      arbeits_element->anz_area = z;

	      /* Arbeitselement ausgeben: */
	      if(printok) {
		printf("\nArbeiselement:");
		printf("\n #node: %d  #area: %d", arbeits_element->anz_node,
		       arbeits_element->anz_area);
		for(i=0;i<arbeits_element->anz_node;i++)
		  {
		    printf("\nKnoten %d: Punktnummer:%d Flag:%c",
			   i,arbeits_element->node[i].punktnr,arbeits_element->node[i].flag );
		    for(j=0;j<3;j++)
		      {
			printf("\nKante %d: Nachbar:%d Flaeche[0]:%d Flaeche[1]:%d",j,
			       arbeits_element->node[i].kante[j].nachbar,
			       arbeits_element->node[i].kante[j].flaeche[0],
			       arbeits_element->node[i].kante[j].flaeche[1]);
		      }
		  }
	      } /* END OF PRINT */
	      
	      /*if (hflaeche != NULL) FREE(hflaeche);*/		
     
	      /* Neues Arbeitselement an Org.-Element haengen: */
	      g_element[element_index-1].cut = arbeits_element;
     
	    } /* Ende If Element geschnitten ? */		
 
	  element_index++;
	} /* Ende While Stoffaktuell */

      if(printok) { 
	printf("\nVerlasse Stoffgebiet %d.",stoffgebiet_index);
      } /* END OF PRINT */
    }  /* Ende Schleife ueber alle Stoffe */


  /* Falls Spannungsloesung vorliegt, muessen die Original-Koordinaten */
  /* wiederhergestellt werden:	                                     */

  if(g_spannung.on&&(g_spannung.verzerrung!=NULL))
    {
      g_knoten = org_knoten;
    }
  return(0);
  /* ***************** ende schneide ************************************* */
}
 
/* ********************************************************************* */
char punkt_vor_ebene(t_koord kord)      /* feststellen ob Punkt zeichnen */
     /* ********************************************************************* */
{
  double xtrans, ytrans, ztrans, wtrans;
  double s;
  
  /* ********************************************************************* */
  xtrans = kord[0];
  ytrans = kord[1];
  ztrans = kord[2];
  wtrans = 1.0;
   
  xtrans = xtrans / wtrans;
  ytrans = ytrans / wtrans;
  ztrans = ztrans / wtrans;
  s = (double) ((g_transform.norm[0]*xtrans + g_transform.norm[1]*ytrans 
		 + g_transform.norm[2]*ztrans) - g_transform.d); 

  if (s < -EPSILON_TEST) return 'W';    /* Punkt liegt vor der Ebene --- */
  if ((s>=-EPSILON_TEST)&&
      (s<=EPSILON_TEST)) return 'G';	/* Punkt liegt genau drauf ----- */
  else return 'D';                      /* Punkt liegt hinter der Ebene  */
  
  /* ***************** ende punkt_vor_ebene ****************************** */
}

/* ********************************************************************* */
static int teile_strecke(int p1,int p2,t_schnittpunkt *lok_schnittpunkt,char flag)
/* ********************************************************************* */
{
  t_koord  punkt1, p1trans,
    punkt2, p2trans,
    punkt1_2, punkt1_2s, punkts, spunkt;
  float    laengev, laenges, laengeorg, wtrans;
  double   lambda, nenner;
  int      k1,k2,i, kante, hilf;
  float    hilfverhaeltnis;
  t_schnittpunktlist *h;
  t_bool korn_on = FALSE;
/* ********************************************************************* */

  /* Ersteinmal testen, ob der Schnittpunkt schon bestimmt ist: -------*/
  if(p2<p1)
    {
      k1 = p2;
      k2 = p1;
      hilfverhaeltnis = 1.0;
    }
  else
    {
      k1 = p1;
      k2 = p2;
      hilfverhaeltnis = 0.0;
    }
  
  for(i=0;i<g_anz_schnittpunkte;i++)
    {
      if(g_schnittpunktliste[i].p1 == k1)
	{
	  if(g_schnittpunktliste[i].p2 == k2) 
	    {
	      /* Schnittpunkt ist schon da: */    
	      /* bekannte Werte in die lokale Struktur eintragen: */
	      lok_schnittpunkt->d3[0] = g_schnittpunktliste[i].x;
	      lok_schnittpunkt->d3[1] = g_schnittpunktliste[i].y;
	      lok_schnittpunkt->d3[2] = g_schnittpunktliste[i].z;
	      lok_schnittpunkt->rot[0] = 0.0;
	      lok_schnittpunkt->rot[1] = 0.0;
	      lok_schnittpunkt->rot[1] = 0.0;
	      lok_schnittpunkt->visit = FALSE;
	      lok_schnittpunkt->punktnr = i;
	      lok_schnittpunkt->winkel = 0.0;
	      return(i);
	    }
	}
    }

  /* Wenn wir hier sind, muss erst ein neuer Schnittpunkt berechnet werden */

  /* Neuen Schnittpunktplatz anlegen */
  g_anz_schnittpunkte++;
  h = (t_schnittpunktlist*)realloc(g_schnittpunktliste,
				   g_anz_schnittpunkte*sizeof(t_schnittpunktlist));
  g_schnittpunktliste = h;

  /* Vorinitialisieren: */
  g_schnittpunktliste[g_anz_schnittpunkte-1].p1=0;
  g_schnittpunktliste[g_anz_schnittpunkte-1].p2=0;
  g_schnittpunktliste[g_anz_schnittpunkte-1].x=0.0;
  g_schnittpunktliste[g_anz_schnittpunkte-1].y=0.0;
  g_schnittpunktliste[g_anz_schnittpunkte-1].z=0.0;

  /* Testen, ob Schnittpunkt von D oder G-Punkt ausgeht : */
  if(flag=='W')
    {
      punkt1[0] = g_knoten[k1-1].koord[0];
      punkt1[1] = g_knoten[k1-1].koord[1];
      punkt1[2] = g_knoten[k1-1].koord[2];
      punkt2[0] = g_knoten[k2-1].koord[0];
      punkt2[1] = g_knoten[k2-1].koord[1];
      punkt2[2] = g_knoten[k2-1].koord[2];
    
      punkt1_2[0] = punkt2[0] - punkt1[0];
      punkt1_2[1] = punkt2[1] - punkt1[1];
      punkt1_2[2] = punkt2[2] - punkt1[2];
  
      for (i=0; i<3; i++) 
	{
	  p1trans[i]   = punkt1[i];
	  p2trans[i]   = punkt2[i];
	  punkt1_2s[i] = punkt1_2[i];
	}

      lambda = g_transform.norm[0]*(double)p1trans[0] + g_transform.norm[1]
	*(double)p1trans[1] + g_transform.norm[2]*(double)p1trans[2];
      lambda = g_transform.d - lambda;
      nenner = g_transform.norm[0]*(double)punkt1_2s[0] + g_transform.norm[1]
	*(double)punkt1_2s[1] + g_transform.norm[2]*(double)punkt1_2s[2];
      if (nenner != 0.0) lambda = lambda / nenner;
      if (ABS(lambda) > 1.0) lambda = SIG(lambda) * 1.0;

      spunkt[0] = p1trans[0] + lambda * punkt1_2s[0];
      spunkt[1] = p1trans[1] + lambda * punkt1_2s[1];
      spunkt[2] = p1trans[2] + lambda * punkt1_2s[2];

      punkts[0] = spunkt[0] - p1trans[0];
      punkts[1] = spunkt[1] - p1trans[1];
      punkts[2] = spunkt[2] - p1trans[2];

      laengev = (double) sqrt(punkt1_2s[0]*punkt1_2s[0] 
			      + punkt1_2s[1]*punkt1_2s[1] + punkt1_2s[2]*punkt1_2s[2]);
      laenges = (double) sqrt(punkts[0]*punkts[0] + punkts[1]*punkts[1]
			      + punkts[2]*punkts[2]);
  
  
      lok_schnittpunkt->d3[0] = spunkt[0];
      lok_schnittpunkt->d3[1] = spunkt[1];
      lok_schnittpunkt->d3[2] = spunkt[2];
 
      g_schnittpunktliste[g_anz_schnittpunkte-1].x = spunkt[0];
      g_schnittpunktliste[g_anz_schnittpunkte-1].y = spunkt[1];
      g_schnittpunktliste[g_anz_schnittpunkte-1].z = spunkt[2];
	  g_schnittpunktliste[g_anz_schnittpunkte-1].verhaeltnis = laenges/laengev;

      /* Verhaeltnisse besser nicht Schnittpktabh. sondern Kanten-abh. machen */
      if (korn_on) 
	  {
      	if (laenges<laengev * 0.5) /* Schnittpkt liegt bei Punkt1 ----------- */
		g_schnittpunktliste[g_anz_schnittpunkte-1].verhaeltnis = 0.0;
      	else                       /* Schnittpkt liegt bei Punkt2 ----------- */
		g_schnittpunktliste[g_anz_schnittpunkte-1].verhaeltnis = 1.0;
	  }
    } /* ENDE FLAG W */
  else
    {
      /* FLAG ist G */
      lok_schnittpunkt->d3[0] = g_knoten[p1-1].koord[0];
      lok_schnittpunkt->d3[1] = g_knoten[p1-1].koord[1];
      lok_schnittpunkt->d3[2] = g_knoten[p1-1].koord[2];
  
      g_schnittpunktliste[g_anz_schnittpunkte-1].x = g_knoten[p1-1].koord[0];
      g_schnittpunktliste[g_anz_schnittpunkte-1].y = g_knoten[p1-1].koord[1];
      g_schnittpunktliste[g_anz_schnittpunkte-1].z = g_knoten[p1-1].koord[2];
      g_schnittpunktliste[g_anz_schnittpunkte-1].verhaeltnis = hilfverhaeltnis;
    }
 
  lok_schnittpunkt->winkel = 0.0;
  lok_schnittpunkt->rot[0] = 0.0;
  lok_schnittpunkt->rot[1] = 0.0;
  lok_schnittpunkt->rot[1] = 0.0;
  lok_schnittpunkt->visit = FALSE;
  lok_schnittpunkt->punktnr = g_anz_schnittpunkte-1;
 
  g_schnittpunktliste[g_anz_schnittpunkte-1].p1 = k1;
  g_schnittpunktliste[g_anz_schnittpunkte-1].p2 = k2;
  
  /* Neue Schnittpunktnummer uebergeben: */
  return(g_anz_schnittpunkte-1);
}
/* *********************** ende teile_strecke ************************** */

/* transform this function in that way that function returns face
   normals calculated for praticular face */

void face_normals(float *norm, t_arbeits_element *elem, int akt_knot, int akt_flaeche) 
{
  int j,k=0;
  float v[3][3], x[3];
  int n[3];
  int kant[3];

  /* calculate the normals */
  /* get neighb. nodes */
  for (j = 0; j < 3; j++) 
    {
      /* first remeber all the nodes. I will need them after */
      n[j] = elem->node[akt_knot].kante[j].nachbar;
      
      /* find all nodes on the given surface */
      if (elem->node[akt_knot].kante[j].flaeche[0] == akt_flaeche ||
	  elem->node[akt_knot].kante[j].flaeche[1] == akt_flaeche)
	kant[k++] = j;
    } 

  /* calculate vectors of kante */
  triad_vect(v, n, akt_knot, elem);
	 
  /* calculate volume of this three vector */
  crossprod(v[0],v[1], x);
  if (dotproduct(x,v[2]) >= 0.0) /* this is right orientation of triad vectors */
    {
      crossprod(v[kant[0]], v[kant[1]], norm);
    }
  else /* then it is left oriented */
    {
      crossprod(v[kant[0]], v[kant[1]], norm);
      /* inverse vector */
      Vecinv(norm, norm);
    }
}

static void triad_vect(float v[][3],  int n[], int p, t_arbeits_element *elem) 
{
  int i,j=0, k, tp;   
  float triad[4][3];
  int inter;          /* intersection of faces */
  int set[3][6];      /* sets */
  int nach;
  
  /* get the coordinates of nodes */
  if (elem->node[p].punktnr > g_geodat.npunkt)
    {
      tp = elem->node[p].punktnr - g_geodat.npunkt;
      triad[0][0] = g_schnittpunktliste[tp-1].x;
      triad[0][1] = g_schnittpunktliste[tp-1].y;
      triad[0][2] = g_schnittpunktliste[tp-1].z;
    }
  else
    {
      k = elem->node[p].punktnr;
      triad[0][0] = g_knoten[k - 1].koord[0];
      triad[0][1] = g_knoten[k - 1].koord[1];
      triad[0][2] = g_knoten[k - 1].koord[2];
    }

  for (i = 0; i < 3; i++) {
    if (elem->node[n[i]].punktnr > g_geodat.npunkt)
      {
	tp = elem->node[n[i]].punktnr - g_geodat.npunkt;
	triad[i+1][0] = g_schnittpunktliste[tp-1].x;
	triad[i+1][1] = g_schnittpunktliste[tp-1].y;
	triad[i+1][2] = g_schnittpunktliste[tp-1].z;
      }
    else 
      {
	k = elem->node[n[i]].punktnr;   /* node number */
	triad[i+1][0] = g_knoten[k - 1].koord[0];
	triad[i+1][1] = g_knoten[k - 1].koord[1];
	triad[i+1][2] = g_knoten[k - 1].koord[2];
      }
  }
  /* calculate vectors of triad (v[0], v[1], v[2])*/
  Vecdis(v[0], triad[0], triad[1]);
  Vecdis(v[1], triad[0], triad[2]);
  Vecdis(v[2], triad[0], triad[3]);
}
























































