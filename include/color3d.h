/* ******************************************************************** */
/* Includefile fuer Color3d Rev 3.0 -------------------------------- */
/* Umstellung auf OSF-Motif und OpenGl ----------------------------- */
/* Author : Helmut Vor, Access eV, Inzestrasse 5, 52070 Aachen ----- */
/* Dateiname:   color3d.h                                            */
/* ******************************************************************** */

#ifndef _color3d_h
#define _color3d_h

#include "strukturen.h"
#include <GL/glu.h>
#include <GL/gl.h>

#ifndef BOOL_DEFINED
typedef int t_bool;
#define TRUE   1
#define FALSE  0
#define BOOL_DEFINED
#endif /* #ifndef BOOL_DEFINED */

#define EPS 0.000001

/* ******************************************************************** */
#define DELETE_ALL        -1
#define SPRACHE_DEUTSCH    1
#define SPRACHE_ENGLISCH   2
/* ******************************************************************** */

/* Prototypen der Funktionen ------------------------------------------ */

int init_data(int);
int make_fenster_cmap(Widget top, t_winstruct *fenster);
int knoten_add(t_knoten **liste, t_knoten *new, int *n);
int element_add(t_element **liste, t_element *new, int *n);
int stoffgebiet_add(t_stoffgebiet **liste, t_stoffgebiet *new, int *n);
int get_stoff_geb_by_nr(t_stoffgebiet *liste, int n, int stoffgebiets_nr);
/* Animation Functions ------------------------------------------------ */
int get_next_zeitschritt();
int get_previous_zeitschritt();
int next_zeit_cb();
int scann_loesung(t_loesung *res);
int c3d();

/* extern-Deklarationen ----------------------------------------------- */

extern GLuint fontOffset;

extern t_knoten *g_knoten;	       /* globale Knoten-Liste -------- */
extern t_knoten *g_knoten_diff;	       /* globale Liste f. Spannungsknoten */
extern t_element *g_element;	       /* globale Element-Liste ------- */
extern t_geodat g_geodat;	       /* globale Info zur Geometrie -- */
extern t_stoffgebiet *g_stoffgebiete;  /* globale Stoffgebiets-Liste -- */
extern t_stoffgruppe *g_stoffgruppen;  /* globale Stoffgruppen-Liste -- */
extern t_stroemung g_stroemung;        /* globale Stroemungsstruktur -- */
extern t_spannung g_spannung;	       /* globale Spannungsstruktur --- */
extern t_loesung g_loesung;            /* globale Loesungsstruktur ---- */
extern struct list *g_results;         /* global array of result struc. */
extern t_loesdef *g_loesdef;           /* globale Loesungsdefinitions - */
extern t_loesdef *loesungen;           /* globale Loesungsdefinitions - */
extern int anzloesungen;               /* Nummer der Loesungsdefiniti.- */
extern t_bool *neue_elemente;          /* globale geschnittene El.(:=1) */
extern t_bool *elemente_zeichnen;      /* Basispointer fuer Schnitt --- */
extern t_arbeits_element *g_schnittelement;/* Basispointer f. Schnitt  */
extern t_schnittpunktlist *g_schnittpunktliste;/* Liste der Schnittp. - */
extern int g_anz_schnittpunkte;        /* Anzahl der Schnittpunkte ---- */
extern t_picking g_picking;            /* globale Liste gepickter Punkte*/
extern t_ofen g_ofen;                  /* globale Struktur f. Absenkung */
extern t_animation g_animation;        /* globale Struktur f. Animation */
extern t_tabelle g_tabelle;            /* Einstellung fuer Farbtabelle  */
extern t_color_extra g_color_extra;    /* Einstellung fuer Color-Extra -*/
extern t_sgiobj g_sgiobj;              /* Einstellung fuer Sgo-object --*/
extern t_bool b_first_color3d;
extern t_bool b_zeichnen;
extern t_bool b_geo_gelesen;
extern t_bool b_loes_gelesen;
extern t_bool b_loes_on;           /* Flagge ob Loesung dargestellt ist */
extern t_bool b_recolor;           /* Resultbild neu zeichnen --------- */
extern t_bool b_ofen_on;           /* Flagge ob Ofendarstellung zeichnen*/
extern char geo_datei_name[256];
extern char tmp_datei_name[256];
extern char *castsroot;
extern int geop_unv_read;          /* Flagge: Geo-Datei = geop oder unv */
extern int g_sprache;
extern int *g_Maske;
extern int gl_width;               /* Groesse des Grafikfensters ------ */
extern int gl_height;
extern int g_ClearColor;           /* set the background color ---------*/
extern t_bool b_init_main;         /* init.des Mainfensterinterface --- */
extern t_bool b_init_loesung;      /* init.des Loesungfensterinterface  */
extern t_bool b_init_stoffe;       /* init.des Stoffefensterinterface - */
extern t_bool b_init_transform;    /* init.des Transformfensterinterface*/
extern t_bool b_init_2dgrafik;     /* init.des 2dfensterinterface ----- */
extern t_bool b_init_vektor;       /* init.des Vektorfensterinterface - */
extern t_bool b_init_tabelle;      /* init.des Tabellenfensterinterface */
extern t_bool b_spin;              /* init. of the spin                 */

extern t_winstruct C3D_win;        /* Hauptfenster fuer Grafikausgabe - */
extern t_winstruct CMAP_win;       /* Fenster fuer Colormapausgabe ---- */
extern t_winstruct ABK_win;        /* Fenster fuer Abkuehlkurven ------ */
/* ******************************************************************** */
#endif   /* #ifndef _color3d_h */












