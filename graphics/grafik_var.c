/**********************************************************************/
/*             Globale Variablen fuer c3d                             */
/**********************************************************************/
/* Grafik ----------------------------------------------------------- */
#ifdef USE_GL
#include "grafik.h"
#endif /* #ifdef USE_GL */
#include "grafik.h"

/*t_winstruct  C3D_win;*/            /* Hauptfenster fuer Grafikausgabe  */
/*t_winstruct  CMAP_win;*/           /* Fenster fuer Colormap ---------- */
/*t_winstruct  ABK_win;*/            /* Fenster fuer Abkuehlkurven ----- */
long         Zmin,               /* Minimale Z-Buffertiefe --------- */
	     Zmax;               /* Maximale Z-Buffertiefe --------- */
t_transform  g_transform;        /* Transformationen der Geometrie - */
t_projektion g_projektion;       /* Projektionen der Geometrie ----- */
t_tabelle    g_tabelle;          /* Einstellung fuer Colormap ------ */
t_bool       Redraw;             /* Geometrie neu zeichnen --------- */
t_isolinie   g_isolinien;        /* Isolinien ---------------------- */
t_isosurfaces g_isosurfaces;     /* Isosurfaces -------------------- */
t_isovolumes  g_isovolumes;      /* Isovolumes --------------------- */ 
t_isocuts     g_isocuts;         /* Cuts --------------------------- */

/* Transformationsmatrizen ----------------------------------------- */
GLfloat      Save_pr_matrix[16],    /* gesicherte Projektionsmatr.-- */
             Save_mo_matrix[16],    /* gesicherte Modelviewmatrix -- */
             Projektions_matrix[16],/* Matrix mit Projektionen ----- */
             Zeichen_matrix[16],    /* Matrix mit Transformationen - */
             Rotation_matrix[16],   /* Rotationen Matrix ----------- */
             Translation_matrix[16]; /* Translation Matrix --------- */
 
/* Feld fuer die Motif-Buttons ------------------------------------- */
int 	     IsoFarben[5];	
GLubyte      c3d_foreground_color[3];
/******************************************************************* */
