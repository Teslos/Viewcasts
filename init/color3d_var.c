/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   color3d_var.c                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*              |  Definitionsdatei fuer globale Variablen       */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/DrawingA.h>

#include "color3d.h"
#include "memory.h"
#include "genlist.h"


/* ******************************************************************* */
GLuint     fontOffset;
t_knoten   *g_knoten = NULL;      /* globale Knoten-Liste ------------ */
t_knoten   *g_knoten_diff = NULL; /* globale Liste f.Spannungsknoten - */
t_element  *g_element = NULL;     /* globale Element-Liste ----------- */
t_geodat    g_geodat;             /* globale Information zur Geometrie */
t_mem       g_mem;                /* globale memory ------------------ */
t_stoffgebiet  *g_stoffgebiete = NULL; /* globale Stoffgebiets-Liste - */
t_stoffgruppe  *g_stoffgruppen = NULL; /* globale Stoffgruppen-Liste - */
t_picking  g_picking;             /* globale Liste gepickter Punkte -- */
t_bool     *neue_elemente = NULL; /* globale geschnittene El. (:=1) -- */
t_bool     *elemente_zeichnen = NULL; /* Basispointer fuer Schnitt --- */
t_arbeits_element *g_schnittelement = NULL;  /* Basispointer f. Schnitt*/
t_schnittpunktlist *g_schnittpunktliste = NULL;/* Liste der Schnittpunkte*/
int g_anz_schnittpunkte = 0;   /* Anzahl der Schnittpunkte  -----------*/
t_stroemung g_stroemung;       /* globale Struktur f. Stroemungsdarst. */
t_spannung  g_spannung;        /* globale Struktur f. Spannungsdarst.- */
t_loesdef  *g_loesdef;         /* globale Struktur f. Loesungsdefinit. */
t_loesdef *loesungen;          /* globale Struktur f. Loesungsdefinit. */
t_loesung   g_loesung;         /* globale Struktur f. Loesungsdarst.-- */
struct list *g_results;
t_ofen      g_ofen;            /* globale Struktur f.Absenkung im Ofen */
t_animation g_animation;       /* globale Struktur f. Animation ------ */
t_light     g_light[8];        /* globale Struktur f. Light----------- */
t_sgiobj    g_sgiobj;          /* globale Struktur f. Sgo-object ----- */
t_winstruct C3D_win;           /* globale Struktur f.OpenGl-Fenster -- */
t_winstruct CMAP_win;          /* globale Struktur f.Colormap-Fenster  */
t_winstruct ABK_win;           /* globale Struktur f.Abk.kurven-Fenster*/
t_color_extra g_color_extra;   /* globale Struktur f.Color-Extra ----- */

/* ******************************************************************* */
int   *g_Maske = NULL;          /* Maske fuer Darstellung d.Geometrie  */
int    g_sprache;	        /* Sprache : 1=deutsch, 2=englisch --- */
int    geop_unv_read;	        /* 1 : geop-Datei  2 : unv-Datei ----- */
int    gl_width;                /* Groesse des Grafikfensters  ------- */
int    gl_height;
int    anzloesungen;            /* Anzhal der Loesungen -------------- */
t_bool b_first_color3d = TRUE;  /* initialisiere c3d ----------------- */
t_bool b_zeichnen = FALSE;      /* Flagge ob gezeichnet werden soll -- */
t_bool b_geo_gelesen = FALSE;   /* Flagge ob Geometrie eingelesen ist- */
t_bool b_loes_gelesen = FALSE;  /* Flagge ob Loesung eingelesen ist -- */
t_bool b_loes_on = FALSE;       /* Flagge ob Loesung dargestellt ist - */ 
t_bool b_recolor = FALSE;       /* Flagge ob Resultbild neu zeichnen - */
t_bool b_ofen_on = FALSE;       /* Flagge ob Ofendarstellung zeichnen  */
t_bool b_spin = FALSE;          /* Flag for spinning the object------- */
t_bool b_init_main;             /* init.des Mainfensterinterface ----- */
t_bool b_init_loesung;          /* init.des Loesungfensterinterface -- */
t_bool b_init_stoffe;           /* init.des Stoffefensterinterface --- */
t_bool b_init_transform;        /* init.des Transformfensterinterface- */
t_bool b_init_2dgrafik;         /* init.des 2dfensterinterface ------- */
t_bool b_init_vektor;           /* init.des Vektorfensterinterface --- */
t_bool b_init_tabelle;          /* init.des Tabellenfensterinterface - */
t_bool b_light_on = TRUE;       /* init lighting --------------------- */
t_bool verschiebung_on;         /* Stoffe-Verschiebung laeuft -------- */
int    verschiebe_stoff;        /* Stoffnr des aktuell versch. Stoffes */
float  g_schritt = 0.1;         /* Schrittweite fuer Verschiebung ---- */

char   geo_datei_name[256];     /* Geometrie-Datei (*.unv oder *.geop) */
char   tmp_datei_name[256];     /* Temperaturloesungsdatei (*.tmp) --- */
char   vek_datei_name[256];     /* Spannungs+Stroemungsdatei (*.dispp) */
                                /* + (*.strom) ----------------------- */
Display *dpy;                   /* Display fuer Grafikinitialisierung  */
Colormap cmap;                  /* Colormap -------------------------- */
float  g_factor = 0.4;          /* Default factor for offset polygon --*/
float  g_bias = 0.002;          /* Default bias for offset polygon ----*/
/* ******************************************************************* */

