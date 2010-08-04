/* ******************************************************************** */
/* Datei enthaelt Typdeklarationen f.alle globale Datentypen in COLOR3D */
/* ******************************************************************** */
/* Dateiname: strukturen.h                                              */
/* ******************************************************************** */
#ifndef _strukturen_h
#define _strukturen_h
/* ******************************************************************** */
#ifndef BOOL_DEFINED                /* fuer mehrfach includes  */
typedef int t_bool;
#define TRUE    1
#define FALSE   0
#define BOOL_DEFINED
#endif                              /* #ifndef BOOL_DEFINED */
/* ******************************************************************** */
#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <GL/gl.h>
#include <GL/GLwMDrawA.h>
#include "genlist.h"
#include "sgiobj.h"
/* ******************************************************************** */
/* Funktionen ------------------------------------------------------- */
#define MIN( a, b ) (((a) < (b)) ? (a) : (b))
#define MAX( a, b ) (((a) > (b)) ? (a) : (b))
#define ABS( a )    (((a) > 0) ? (a) : -(a))
#define SIG( a )    (((a) > 0) ? (1) : -(1))
/* ******************************************************************** */
typedef float t_koord[3];   /* Koordinaten eines Punktes ------------- */

typedef struct knoten
{
    int knoten_nr;          /* Knoten-Nummer ------------------------- */
    t_koord koord;          /* x-,y- und z-Koordinate ---------------- */
} t_knoten;

typedef struct norm
{
  float normals[3];
} t_norm;

typedef struct uni_kante
{
  int nachbar;
  int flaeche[2];
} t_uni_kante;
    
typedef struct uni_knoten
{
  int punktnr;
  char flag;
  t_uni_kante kante[3];
} t_uni_knoten;

typedef struct arbeits_element
{
  int anz_node;
  int anz_area;
  int *hflaeche;
  t_uni_knoten *node;
  struct arbeits_element *next;
} t_arbeits_element;

typedef struct gitterpunkt
{
  t_koord  koo;
  float    dis[4];
  int      pnr[4];
} t_gitterpunkt;

typedef struct 
{
  GLubyte r,g,b;
} t_colorub;

typedef struct element
{
    int element_nr;         /* Element-Nummer ------------------------ */
    int anz_knoten;         /* Anzahl der Kontenpunkte --------------- */
    int pn[8];              /* Punktnummern der Elemente ------------- */
    int nachbar[6];         /* Stoffnummer des Nachbarelementes ------ */
    int stoff_nr;           /* Stoffgebiets-Nummer ------------------- */
    int zeichnen;           /* Element wird gezeichnet  = 1
                               Element wird geschnitten = 2 
			       Element wird tangiert    = 3
                               Element wird nicht gezeichnet = 0 ----- */
    t_arbeits_element *cut; /* Zeiger auf geschnittenes Element ------ */
} t_element;

typedef struct stoff
{
  int           n_punkt;    /* Anzahl der interpol. Gitterpunkte ----- */
  t_gitterpunkt *startpunkt;/* Koordinaten der interpol. Gitterpunkte  */
  int           n_org_punkt;/* Anzahl der original Gitterpunkte ------ */
  int           *punktlist; /* Koordinaten der original Gitterpunkte - */
  t_koord       *vektor;    /* interpolierte Vektoren ---------------- */
  t_koord       *org_vektor;/* berechnete Vektoren ------------------- */
} t_stoff;

typedef struct geodat
{
    int npunkt;             /* Anzahl Knotenpunkte ------------------- */
    int nvol;               /* Anzahl Volumenelemente ---------------- */
    int ninter;             /* Anzahl Interface-Elemente ------------- */
    int ngrp;               /* Anzahl Stoffgruppen ------------------- */
    int nstoff;             /* Anzahl Stoffe ------------------------- */
} t_geodat;

typedef struct stoffgebiet
{
    int nr;                 /* Stoffgebietsnummer -------------------- */
    char name[80];          /* Stoffgebietsname ---------------------- */
    int gruppennr;          /* Nummer der Stoffgruppe ---------------- */
    t_koord min;            /* minimale Stoffgebietsabmessungen ------ */
    t_koord max;            /* maximale Stoffgebietsabmessungen ------ */
    int anfang_hex;         /* Anfangsadresse der Hexaeder ----------- */
    int ende_hex;           /* Endadresse der Hexaeder --------------- */
    int anfang_pen;         /* Anfangsadresse der Pentaeder ---------- */
    int ende_pen;           /* Endadresse der Pentaeder -------------- */
    int anfang_tet;         /* Anfangsadresse der Tetraeder ---------- */
    int ende_tet;           /* Endadresse der Tetraeder -------------- */
    int zeichne_on;         /* Stoff wird dargestellt Ja/Nein -------- */
    t_koord verschiebung;   /* Verschiebevektor f. Explosionszeichnung */
} t_stoffgebiet;

typedef struct stoffgruppe
{
    int nr;                 /* Nummer der Stoffgruppe ----------------- */
    int anzahl;             /* Anzahl Stoffe der Stoffgruppe ---------- */
    int *stoffe;            /* Liste mit Stoffen der Stoffgruppe ------ */
} t_stoffgruppe;

typedef struct pickpunkt
{
    int     art;            /* Koordinate   = 1 ---------------------- */
                            /* Abkuehlkurve = 2 ---------------------- */
                            /* Seigerung    = 3 ---------------------- */
    int     punkt_nr;       /* Punktnummer --------------------------- */
    t_koord koord;          /* Koordinaten des Punktes --------------- */
    float   min, max,       /* minimaler u. maximaler Parameterwert -- */
            *werte,         /* Parameterwert zu jedem Loesungsfeld --- */
            *werte1,        /* Parameterwert zu zweite punkt --------- */
            *werte2;        /* Parameterwerte aus 2. Loesung --------- */
	unsigned int rgba;      /* RGBA-value ---------------------------- */
	char   color_string[8]; /* Color string value -------------------- */
    struct pickpunkt *next; /* Zeiger auf naechstes Listenelement ---- */
} t_pickpunkt;

typedef struct picking 
{
    t_bool      pick_on;    /* Eingabe durch picken, sonst manuell --- */
    int         anzahl;     /* Anzahl der gepickten Punkte ----------- */
    t_bool      kurve_on;   /* Kurve an/aus -------------------------- */
    struct list *punkte;    /* Punktnummer des gepickten Punktes ----- */
    t_bool     manuell_skaliert;/* skalierung erfolgt manuell -------- */
    t_bool     fscale;       /* the picking plot is not scaled ----- */
    float      A_min, A_max;/* minimaler u. maximaler Abszissenwert -- */
    float      O_min, O_max;/* minimaler u. maximaler Ordinatenwert -- */
} t_picking;

typedef struct loesdef
{
    char   dateiende[20];   /* Name der Loesungsdatei ---------------- */
    char   text[40];        /* Ausgabestring im Grafikfenster -------- */
    char   parameter[10];   /* Parameter der Loesung ----------------- */
    char   einheit[10];     /* Dimension der Loesung ----------------- */
    float  Min,             /* Minimal einstellbarer Wert ------------ */
           Max,             /* Maximal einstellbarer Wert ------------ */
           schritt;         /* Schrittweite fuer Einstellung --------- */
} t_loesdef;


typedef struct loesung
{
    FILE   *loes_in;        /* Einleseunit fuer Loesung -------------- */
    char   dateiname[256];  /* Name der Loesungsdatei ---------------- */
    char   ausgabe[40];     /* Ausgabestring im Grafikfenster -------- */
    char   parameter[10];   /* Parameter der Loesung ----------------- */
    char   einheit[10];     /* Einheit der Loesung ------------------- */
    t_loesdef definition;   /* Definition of the result file --------- */        
    t_bool offen,           /* Loesungsdatei ist geoeffnet ----------- */
           gescannt,        /* Loesungsdatei ist gescannt ------------ */
           gelesen,         /* Loesungsdatei ist gelesen ------------- */
           typ_gesetzt,     /* Dateityp ist vordefiniert ------------- */
           min_max,         /* Auto-Skala on ------------------------- */
	   mmap,            /* Memory map on ------------------------- */
           range_local,     /* Sets local range to auto -------------- */
           range_glob,      /* Sets global range to auto ------------- */
           param_gesetzt;   /* Parameter sind explizit gesetzt ------- */
    float  akt_min,         /* Minimaler Wert des aktuellen Feldes --- */
           akt_max,         /* Maximaler Wert des aktuellen Feldes --- */
           akt_schritt,     /* aktuelle Schrittweite ----------------- */
           Min,             /* Minimal einstellbarer Wert ------------ */
           Max,             /* Maximal einstellbarer Wert ------------ */
           schritt,         /* Schrittweite fuer Einstellung --------- */
           anim_slow;       /* Darstellungsverzoegerung -------------- */
    int    anzahl_felder,   /* Anzahl der Loesungsfelder in der Datei  */
           anim_anfang,     /* Anfangsfeld fuer Animation------------- */
           anim_ende,       /* Endfeld fuer Animation ---------------- */
           anim_on,         /* Animation laeuft ---------------------- */
           akt_index;       /* Index des aktuellen Loesungsfeldes ---- */
    float* zeiten;          /* Feld mit Zeiten der Loesungsfelder ---- */
    float* temp;            /* aktuelles Loesungsfeld ---------------- */
} t_loesung;

typedef struct spannung
{
    FILE    *vektor_in;     /* Unit fuer Spannungsloesung ------------ */
    char    dateiname[256]; /* Name der Spannungsdatei --------------- */
    char    ausgabe[40];    /* Ausgabestring im Grafikfenster -------- */
    char    parameter[10];  /* Parameter der Spannungsloesung -------- */
    char    einheit[10];    /* Einheit der Spannungsloesung ---------- */
    t_bool  on,             /* Spannung darstellen ------------------- */
            huelle_on,      /* Huelle darstellen --------------------- */
            vektor_offen,   /* Vektorloesung geoeffnet --------------- */
            vektor_gescannt,/* Spannungsloesung gescannt ------------- */
            vektor_gelesen; /* Spannungsloesung gelesen -------------- */
    t_koord akt_min,        /* Minimaler Wert des aktuellen Feldes --- */
            akt_max,        /* Maximaler Wert des aktuellen Feldes --- */
            Min,            /* Minimal einstellbarer Wert ------------ */
            Max;            /* Maximal einstellbarer Wert ------------ */
    int     anzahl_felder,  /* Anzahl der Spannungsschritte ---------- */
            anim_anfang,    /* Anfangsfeld fuer Animation ------------ */
            anim_ende,      /* Endfeld fuer Animation ---------------- */
            vektorani_on,   /* Animation fuer Spannung --------------- */
            huellfarbe,     /* Farbe der Huelle ---------------------- */
            verspanne_solid_huelle,   /* solid = 0; Huelle = 1 ------- */
            akt_index,      /* Index des akt. Vektorloesungsfeldes --- */
            old_index;      /* alter Index f. ------------------------ */
    float   factor,         /* Faktor fuer Ueberhoehung der Darstellung*/
            *zeiten;        /* Feld mit Zeiten aus der Spannungsloesung*/
    t_koord *vektor;        /* akt. Vektorloesungsfeld der Spannungen  */
    t_knoten*verzerrung;    /* Feld fuer verzerrte Geometriedaten ---- */
    int     changes[6];     /* Feld fuer die geaenderten Werte ------- */
                            /* changes[0] : Spannungs ---------------- */
                            /* changes[1] : Stroemung ---------------- */
                            /* changes[2] : Huelle deformieren ------- */
                            /* changes[3] : Animation - Anfang ------- */
                            /* changes[4] : Animation - Ende --------- */
                            /* changes[5] : aktueller Index ---------- */
} t_spannung;

typedef struct stroemung
{
    FILE    *vektor_in;     /* Unit fuer Stroemungsloesung ----------- */
    char    dateiname[256]; /* Name der Stroemungsdatei -------------- */
    char    ausgabe[40];    /* Ausgabestring im Grafikfenster -------- */
    char    parameter[10];  /* Parameter der Stroemungsloesung ------- */
    char    einheit[10];    /* Einheit der Stroemungsloesung --------- */
    t_bool  on,             /* Stroemung darstellen ------------------ */
            vektor_offen,   /* Vektorloesung geoeffnet --------------- */
            vektor_gescannt,/* Stroemungsloesung gescannt ------------ */
            vektor_gelesen, /* Stroemungsloesung gelesen ------------- */
            gitter_reduced, /* Flag, ob Gitter schon reduziert ist --- */
            knoten_sortiert;/* Flag, ob Knoten schon sortiert sind --- */
    t_koord akt_min,        /* Minimaler Wert des aktuellen Feldes --- */
            akt_max,        /* Maximaler Wert des aktuellen Feldes --- */
            Min,            /* Minimal einstellbarer Wert ------------ */
            Max;            /* Maximal einstellbarer Wert ------------ */
    int     anzahl_felder,  /* Anzahl der Spannungsschritte ---------- */
            anim_anfang,    /* Anfangsfeld fuer Animation ------------ */
            anim_ende,      /* Endfeld fuer Animation ---------------- */
            vektorani_on,   /* Animation fuer Stroemung -------------- */
            akt_index;      /* Index des akt. Vektorloesungsfeldes --- */
    float   factor,         /* Faktor fuer Ueberhoehung der Darstellung*/
            old_factor,     /* dito zur Rueckberechnung -------------- */
            pfeilspitze,    /* Farbe d. Pfeilspitze f. red. Pfeildarst.*/
            pfeilspitze_cycle,/* Pfeil im Colormapmodus mit Cycling -- */
            v_x,v_y,v_z,    /* Verschub fuer reg. Gitter ------------- */
            *zeiten;        /* Feld mit Zeiten aus Stroemungsloesung - */
    t_koord * vektor;       /* akt. Vektorloesungsfeld der Stroemungen */
    int     art,            /* Pfeile            = 0;
                               Partikel(Tracer)  = 1;
                               reduzierte Pfeile = 2;
                               Roling            = 3  ---------------- */
            pfeilfarbe;     /* Farbe der Pfeile ---------------------- */
    int     changes[6];     /* Feld fuer die geanderten Werte -------- */
    t_stoff stoff[50];      /* Struktur pro Stoff zur Darstellung ---- */
} t_stroemung;

typedef struct animation
{
    float  *zeiten;         /* Feld mit den Zeiten fuer die Animation  */
    t_bool temp_on,         /* Animation ueber Temperatur: ----------- */
           spann_on,        /* Animation ueber Spannung: ------------- */
           stroem_on,       /* Animation ueber Stroemung: ------------ */
           anim_on;         /* Animation laeuft ---------------------- */
    int    akt_index,       /* aktuell dargestelltes Loesungsfeld ---- */
           anzahl_felder,   /* Anzahl der Loesungsfelder ------------- */
           anim_anfang,     /* Anfangsfeld fuer Animation ------------ */
           anim_ende,       /* Endfeld fuer Animation ---------------- */
           anim_frame,      /* Frame fuer Animation ------------------ */
           art;             /* 1 = Temp., 2 = Spannung, 3 = Stroemung  */
} t_animation;

typedef struct ofen
{
    char   dateiname[256];  /* Name der Ofendatei (bisher ~.hflap ---- */
    t_bool on,              /* Ofen darstellen ----------------------- */
           ex;              /* Ofenfile existiert -------------------- */
    int    *stofen,         /* Stoffgebiete des Ofens ---------------- */
           anz_stofen,      /* Anzahl Stoffgebiete des Ofens --------- */
           anz_stuetz;      /* Anzahl Stuetzstellen fuer Absenkung --- */
    float  *pos_zeiten;     /* Zeitstuetzwerte (aus ~.hflap) --------- */
    float  akt_position;    /* Position zum aktuellen Zeitpunkt ------ */
    t_koord *positionen;    /* Positionen ---------------------------- */ 
} t_ofen;


typedef struct schnittknoten
{
    int knoten_nr;          /* neue Knoten-Nummer -------------------- */
    t_koord koord;          /* x-,y- und z-Koordinate ---------------- */
    t_knoten *punkt1,       /* 1. Punkt der Kante -------------------- */
             *punkt2;       /* 2. Punkt der Kante -------------------- */
    float    verhaeltnis;   /* Streckenverhaeltnis Punkte, Schnittpunkt*/
    struct schnittknoten *next; /* Naechster Knoten der Seite -------- */
} t_schnittknoten;

/* Struktur fuer Fenster --------------------------------------------- */
typedef struct overlay
{
  Window win;              /* visual for rendering ---------- */
  Widget wid;              /* widget for overlay rendering -- */
  GLXContext cx;             
  XVisualInfo *vi;        
  t_bool visAlloced;       /* if vis needs deallocate on destroy */ 
  Colormap cmap;           /* RGB colormap for window;None if CI */
  Colormap colormap;
  int showState;           /* if overlay window mapped --------- */
  t_bool treatAsSingle;    /* treat as single buffered --------- */
  t_bool isDirect;         /* if direct context ---------------- */
  int transparentPixel;    /* transparent pixel value ---------- */
} t_overlay;

typedef struct winstruct
{
  Widget wid;            /* Widget -------------------------- */
  Window win;            /* Window -------------------------- */
  Window render_win;     /* Render window ------------------- */
  unsigned int display_mode; /* Display mode ---------------- */
  char *display_string;      /* Display string -------------- */
  Widget frame;          /* Widget -------------------------- */
  int    num;            /* Nummer des Fensters ------------- */
  t_bool on;             /* Fenster ist offen --------------- */
  t_bool ex;             /* Fenster ist generiert ----------- */
  int gl_width,          /* Breite des Fensters ------------- */
      gl_height,         /* Hoehe des Fensters -------------- */
      xorg, yorg;        /* Ursprung des Fensters ----------- */
  float  asp;            /* Verhaeltnis der Kanten ---------- */
  XVisualInfo *vi;       /* Visualinformationen ------------- */
  GLXContext cx;         /* Renderingcontext ---------------- */
  GLXContext render_cx;  /* Render context  ----------------- */
  Display   *dpy;        /* Display ------------------------- */
  t_bool achtbit;        /* Achtbit-Colormapfenster --------- */
  t_bool z_buffer;       /* Z-Bufferfenster ----------------- */
  t_bool doublebuffer;   /* Doublebufferfenster ------------- */
  Colormap cmap;         /* Pointer auf Colormap ------------ */
  Colormap overlayColormap; /* Pointer auf Colormap --------- */
  int      overlayDepth;   /* Depth of overlay -------------- */
  Visual  *overlayVisual;  /* Visual information ------------ */
  t_overlay *overlay;      /* overlay visual ---------------- */
  int stensize;            /* Size of Stencil Buffer -------- */
  int font_height;         /* Font height ------------------- */
} t_winstruct;


/* Struktur fuer alle Transformationen ------------------------------- */
typedef struct transform
{
  float   move[3];        /* Verschiebung in X-, Y-, Z-Richtung */
  float   pan[3];         /* Panning in X-, Y-, Z- directions   */
  float   rotx,           /* Rotation in X-Richtung ----------- */
          roty,           /* Rotation in Y-Richtung ----------- */
          rotz;           /* Rotation in Z-Richtung ----------- */
  float   scale;          /* Skalierungsfaktor: alle Richtungen */
  int     mirrorx,        /* Spiegelung an XZ-Ebene ----------- */
          mirrory,        /* Spiegelung an YZ-Ebene ----------- */
          mirrorz;        /* Spiegelung an XY-Ebene ----------- */
  t_bool  spiegel_on,     /* Spiegelung ist eingeschaltet ----- */
          zyklus_on,      /* zykl. Darstellung ist eingeschaltet*/
          schneiden_on,   /* zykl. Darstellung ist eingeschaltet*/
          kreuz_on,       /* Achsenkreuz darstellen ----------- */
          alle_stoffe_on, /* Einstellung gilt fuer alle Stoffe  */
          rahmen_on,      /* Rahmen darstellen ---------------- */
	  gitter_on,      /* Gitter darstellen ---------------- */
	  rotation_on,    /* Rotation on ---------------------- */
          geo_on,         /* zeichne Geometrie ---------------- */
          result_on,      /* zeichne Ergebnis ----------------- */
          absenken_on,    /* Geometrie im Ofen absenken ------- */
          stoffgruppen_on,/* Stoffgruppendarstellung ---------- */
          zentriert_on,   /* Geometrie ist zentriert ---------- */
          zentriert_ex,   /* Zentriervektor existiert---------- */
          rand_on;        /* zeichne Randbedingungen ---------- */
  float   scale_k;        /* skalierung fuer Achsenkreuz ------ */
  int     zyklus_anz;     /* Anzahl der Zyklen ---------------- */
  float   zyklus_ang,     /* Winkel der Zyklen ---------------- */
  	  d;		  /* Abstand der Ebene vom Ursprung --- */
  char    zyklus_achse;   /* Drehachse fuer Zyklus ------------ */
  t_koord schnittp1_koord,/* Koordinaten Schnittpunkt 1 ------- */
          schnittp2_koord,/* Koordinaten Schnittpunkt 2 ------- */
          schnittp3_koord,/* Koordinaten Schnittpunkt 3 ------- */
          norm,	  	  /* Normalenvektor der Ebene --------- */
          zentrier_vector;/* Verschiebung fuer Zentrierung ---- */
  int     schnittp1_nr,   /* Nummer Schnittpunkt 1 ------------ */
          schnittp2_nr,   /* Nummer Schnittpunkt 2 ------------ */
          schnittp3_nr;   /* Nummer Schnittpunkt 3 ------------ */
} t_transform;

/* Struktur fuer alle Projektionen ----------------------------------- */
typedef struct projektion
{
  float   near,           /* Clipping-Planes ------------------ */
          far,
          bottom,
          top,
          left,
          right,
          dist;           /* Entfernung zum Auge -------------- */
  float   fovy,           /* Rotation in X-Richtung ----------- */
          azim,           /* Sichtwinkel des Betrachters ------ */
          inc,
          twist;
  float   aspect;         /* Kantenverhaeltnis d. Darstellungsfensters */
  t_bool  perspektive;    /* Art d. perspektivischen Darstellung */
  float   geo_factor;     /* Geometrieabhaengiger Faktor f. Verschiebung */
} t_projektion;

typedef struct bewegen
{
  int   bewegt;           /* 0 = nichts, 1 = bewegt, 2 = gedreht */
  int   anz_stoff;        /* # bewegte Stoffe ------------------ */
  int   *stoffe;          /* Feld mit bewegten Stoffen --------- */
  float position;         /* akt. Position in Koord-achsen-Richtung */
  int   achse;            /* Bewegungs- Dreh-Richtung ---------- */
  float winkel;
  float geschwindigkeit;  /* Bewgungs- Drehgeschwindigkeit ----- */
  float **zeit_wert;      /* Zeit- Wertepaare fuer Bewegung ---- */
} t_bewegen;

typedef struct tabelle
{
  int   x_pos;            /* X-Position im Fenster ------------- */
  int   y_pos;            /* Y-Position im Fenster ------------- */
  int   breite;           /* Fensterbreite --------------------- */
  int   size;             /* size of color legend -------------- */
  int   margin_x;         /* margin in x direction ------------- */
  int   margin_y;         /* margin in y direction ------------- */
  int   pos;              /* position of color-legend ---------- */
  int   hoehe;            /* Fensterhoehe ---------------------- */
  int   anzahl_farben;    /* Anzahl der Farben ----------------- */
  int   art;              /* 1 = Default; 2 = Grau; 3 = Raster - */
                          /* 4 = Datei ------------------------- */
  float intensity;        /* Intensity ------------------------- */
  t_bool on;              /* Tabelle ist dargestellt ----------- */
  t_bool ex;              /* Tabellenfenster existiert --------- */
  float min;              /* Minimaler Parameterwert ----------- */
  float max;              /* Maximaler Parameterwert ----------- */
  int schritte;           /* Anzahl der Unterteilungen --------- */
  t_bool colortable_ex;   /* Tabellenfenster existiert --------- */
  int start_index_datei;  /* Offset fuer Belegung aus Datei ---- */
  int   end_index_datei;  /* Ende der Dateibelegung ------------ */
  char dateiname[256];    /* Name der Datei fuer die Belegung -- */
  int anz_bitplanes;      /* Anzahl der Bitplanes des Visuals -- */
  int* colortable;        /* Daten der Colormap ---------------- */
  int startindex;         /* Offset fuer Belegung der Colormap - */
} t_tabelle;

typedef struct linie
{
    t_bool  on;             /* Isolinie belegt ----------------------- */
    int     nr;             /* Nummer der Isolinie ------------------- */
    float   wert,           /* Parameterwert ------------------------- */
            breit;          /* Breite der Linie in Grad -------------- */
    float   r, g, b;        /* Farbe --------------------------------- */
} t_linie;

typedef struct isolinie
{
  int      anzahl_isolinien;/* Anzahl der gesetzten Isolinien -------- */
  struct list *linien;      /* Feld mit Isolinien -------------------- */
} t_isolinie;

typedef struct isomat
{
	int max_vert;
	float *iso_vertices;
	GLubyte *iso_colors;
	float *iso_normals;
	int ntri;
} t_isomat;

typedef struct iso
{
	float wert;
	t_bool on;
	t_bool field;
	t_isomat *mat;
	int    nr;
	float  r,g,b;	
} t_iso;

typedef struct isosurfaces
{
	int nisosurfaces;
	struct list *isosurfaces;
} t_isosurfaces;

typedef struct isovolumes
{
	int nisovolumes;
	struct list *isovolumes;
} t_isovolumes;

typedef struct cut
{
	int max_vert;
	int max_vals;
	int max_tex_coords;
	float *vertices;
	float *vals;
	GLfloat *tex_coords;
	int ntri;
}t_cut;

typedef struct isocut
{
	t_bool on;
	t_bool field;
	float cplane[4];
	t_cut *mat;
	int nr;
	float r,g,b;
}t_isocut;

typedef struct cuts
{
	int ncuts;
	struct list *cuts;
}t_isocuts;

typedef struct schnittpunktlist
{
     int p1,p2;		     /* Punktnummern der Nachbarn ------------ */
     float x,y,z,	     /* 3D Koordinaten des Schnittpunktes ---- */
           verhaeltnis;      /* Streckenverhaeltnis p1,p2 zu Schnittpkt*/
} t_schnittpunktlist;

typedef struct color
{
  GLubyte color[3];           /* RGB value for color */
  XmString color_string;        /* RGB value in string */
} t_color;

typedef struct light
{
  int   nlight;              /* number of light */
  float ambient[4];          /* RGBA values for ambient light */
  float diffuse[4];          /* RGBA values for diffuse light */
  float specular[4];         /* RGBA values for specular light */
  float spotdir[3];          /* spot direction */
  float position[3];         /* position of the light */
  float scut;                /* spot cut off */
  float ca;                  /* constant attenutation  */
  float la;                  /* linear attenutation    */
  float qa;                  /* quadratic attenutation */
  t_bool on;                 /* on or off              */
} t_light;

typedef struct subtex 
{
  GLubyte **subTexImage;     /* sub texture image */
  int nsubtex;                /* number of sub textures */
  GLuint *subtex_size;       /* size of sub textures */
} t_subtex;

typedef struct texture_info
{
  GLubyte texImage[4][4*1024];       /* texture image */
  t_subtex *subtex;         /* sub texture struct */
  int ntex;                 /* number of defined textures */
  int currentex;            /* current texture */
  GLuint texName[4];        /* texture names */
  GLuint tex_size[4];       /* size of texture (max) */
  int nstripes;             /* number of stripes */
} t_texture_info;

typedef struct color_ex
{
  t_bool  texture_on;        /* flag for texture         */
  float clipabove;           /* cliping above            */
  float clipbellow;          /* cliping bellow           */
  int   color_name;          /* current color table name */
  float transparency;        /* current transp. value    */
  t_bool  trans_color_on;    /* transparency color on    */
  GLubyte trans_color[4];    /* transparency color       */
} t_color_extra;

typedef struct point_t
{
  t_bool delete;             /* delete the point         */
  int vnum;                  /* index of point           */
  float v[3];                /* vertex coordinates       */
} t_point;

typedef struct edge
{
  t_bool edge_on;
  int edgecolor;
  float factor;
  float bias;
} t_edge;

typedef struct tagsgiobj
{
  sgiobj *object;           /* pointer to sgo object     */
  int    how;               /* how to draw               */
  t_bool gelesen;           /* is it read                */
} t_sgiobj;
#endif /* _strukturen_h */








