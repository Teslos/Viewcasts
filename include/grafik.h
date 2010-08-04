#ifndef  _c3d_h
#define _c3d_h

#include "config.h"
#include <Xm/Xm.h>

#include "color3d.h"
#include "motif.h"
#include "../graphics/ref.h"
#include "iso.h"
#include "paulslib.h"
#include "gendlist.h"
#include <GL/gl.h>
#ifdef HAVE_LINUX
#  include <GL/GLwMDrawA.h>
#else
#  include <X11/GLw/GLwMDrawA.h>
#endif
#include <X11/keysymdef.h>
#include <math.h>
/* Display mask values ---------------------------------------------- */
#define C3D_RGB        0 
#define C3D_RGBA       C3D_RGB
#define C3D_INDEX      1
#define C3D_SINGLE     0
#define C3D_DOUBLE     2
#define C3D_ACCUM      4
#define C3D_ALPHA      8
#define C3D_DEPTH      16
#define C3D_STENCIL    32
#define C3D_MULTISAMPLE 128
#define C3D_STEREO     256
#define C3D_WIND_IS_RGB(x)         (((x) & C3D_INDEX) == 0)
#define C3D_WIND_IS_INDEX(x)       (((x) & C3D_INDEX) != 0)
#define C3D_WIND_IS_SINGLE(x)      (((x) & C3D_DOUBLE) == 0)
#define C3D_WIND_IS_DOUBLE(x)      (((x) & C3D_DOUBLE) != 0)
#define C3D_WIND_HAS_ACCUM(x)      (((x) & C3D_ACCUM) != 0)
#define C3D_WIND_HAS_ALPHA(x)      (((x) & C3D_ALPHA) != 0)
#define C3D_WIND_HAS_DEPTH(x)      (((x) & C3D_DEPTH) != 0)
#define C3D_WIND_HAS_STENCIL(x)    (((x) & C3D_STENCIL) != 0)
#define C3D_WIND_IS_MULTISAMPLE(x) (((x) & C3D_MULTISAMPLE) != 0)
#define C3D_WIND_IS_STEREO(x)      (((x) & C3D_STEREO) != 0)
#define C3D_WIND_IS_LUMINANCE(x)   (((x) & C3D_LUMINANCE) != 0)

/* Transformationsmodi ---------------------------------------------- */
#define NICHTS      0
#define ROTATION    1
#define TRANSLATION 2
#define ZOOM        3
#define SPIEGELUNG  4
#define ZYKLUS      5
#define AUTOSCALE   6

/* Werte fuer Maske ------------------------------------------------ */
#define LOESCHE_ALLE     1
#define LOESCHE_STOFF    2
#define SETZE_ALLE       3
#define SETZE_STOFF      4
#define SETZE_STOFF_PLUS 5
#define SETZE_NETZ       6
#define SETZE_NETZ_MINUS 7

/* Werte fuer Darstellung ------------------------------------------ */
#define ZEICHNE_NICHT            0
#define ZEICHNE_SOLID            1
#define ZEICHNE_GITTER           2
#define ZEICHNE_TRANSPARENT      3
#define ZEICHNE_NETZ             4
#define ZEICHNE_RESULT           5
#define ZEICHNE_SOLID_NETZ       6
#define ZEICHNE_TRANSPARENT_NETZ 7
#define ZEICHNE_RESULT_NETZ      8
#define ZEICHNE_RAHMEN           9

/* Werte fuer Colormap ----------------------------------------------- */
#define FARBE_NETZ         2017      /* weiss ------------------------ */
#define FARBE_NETZ_ACHT     11       /* weiss ------------------------ */
#define FARBE_SCHWARZ      768       /* schwarz ---------------------- */
#define FARBE_SCHWARZ_ACHT  10       /* schwarz ---------------------- */
#define FARBE_WEISS        769       /* weiss ------------------------ */
#define FARBE_WEISS_ACHT    11       /* weiss ------------------------ */
#define FARBE_KREUZ        772       /* blau ------------------------- */
#define FARBE_KREUZ_ACHT    14       /* blau ------------------------- */
#define FARBE_SCHRIFT      774       /* gelb ------------------------- */
#define FARBE_SCHRIFT_ACHT  16       /* gelb ------------------------- */
#define FARBE_HUELLE       2019      /* gruen ------------------------ */
#define FARBE_HUELLE_ACHT   53       /* gruen ------------------------ */
#define STOFFARB_OFFSET    2016      /* Offset fuer Stoffdarstellung - */
#define ACHT_BIT_OFFSET     10       /* Offset fuer Stoffdarstellung - */
#define TEMPFARB_OFFSET    1024      /* Offset fuer Resultdarstellung  */
#define ACHT_BIT_TEMP_OFFSET 50      /* Offset f. Acht-Bit-Temp. Darst.*/
#define FARBE_KREUZ_X      772	     /* Farbe d. X-Achse --------------*/
#define FARBE_KREUZ_Y      774	     /* Farbe d. Y-Achse --------------*/
#define FARBE_KREUZ_Z      777	     /* Farbe d. Z-Achse --------------*/
#define FARBE_KREUZ_X_8BIT  13	     /* Farbe d. X-Achse 8-Bit Mode ---*/
#define FARBE_KREUZ_Y_8BIT  15	     /* Farbe d. Y-Achse 8-Bit Mode ---*/
#define FARBE_KREUZ_Z_8BIT  12	     /* Farbe d. Z-Achse 8-Bit Mode ---*/
#define ROLL_OFFSET          3500    /* Startindex der Rollfarben -----*/
#define ACHT_BIT_ROLL_OFFSET 185     /* Startindex der Rollfarben 8-Bit*/
#define ROLL_LENGTH          4       /* Laenge der Rollfarben ---------*/
#define FARBE_SCHAFT         3504    /* Farbe d. Schaftes -------------*/
#define FARBE_SCHAFT_ACHT    189     /* Farbe d. Schaftes 8-Bit -------*/
#define FARBE_SPITZE         3505    /* Farbe d. Spitze ---------------*/
#define FARBE_SPITZE_ACHT    190     /* Farbe d. Spitze 8-Bit ---------*/
/* Font ---------------------------------------------------------------*/
#define FONT_COURIER1  0
#define FONT_COURIER2  1
#define FONT_SGI1      2
#define FONT_SGI2      3
#define FONT_FIXED1    4
#define FONT_FIXED2    5
#define FONT_HELVETICA_MEDIUM_14 6
#define FONT_HELVETICA_BOLD_14   7
#define FONT_SWISS721_MEDIUM_14  8
#define FONT_SWISS721_BOLD_14    9

/* Texture ------------------------------------------------------------*/
#define TEX_WIDTH   1024             /* width of texture map ----------*/
#define TEX_HEIGHT  256              /* height of texture map ---------*/
#define TEX_WIDTH_RASTER 16          /* width of texture map raster ---*/
#define TEX_DEFAULT      0           /* texture with default colors ---*/
#define TEX_RASTER       1           /* texture with raster colors ----*/
/* True Colormap ------------------------------------------------------*/
#define MAX_COL         2016         /* max. number of colors ---------*/
#define CCROSS_X        2020         /* color of X-axe-----------------*/
#define CCROSS_Y        2022         /* color of Y-axe-----------------*/
#define CCROSS_Z        2024         /* color of Z-axe-----------------*/
#define HOT_COLD        1            /* type of the colormap ----------*/
/* zusaetzliche OpenGl-Fenster --------------------------------------- */
#define COLORMAP_FENSTER      1      /* Fenster fuer Farbpalette ----- */
#define ABKUEHLKURVEN_FENSTER 2      /* Fenster fuer Abkuehlkurven --- */
/* Overlay or normal window ------------------------------------------ */
#define C3D_NORMAL   0               /* Normal window ---------------- */
#define C3D_OVERLAY  1               /* Overlay window --------------- */
/* globale Strukturen in COLOR3D ------------------------------------- */
#include "strukturen.h"

/* Konstanten -------------------------------------------------------- */
#define PI 3.141519265
#define FEHLER  -1
#define MAX_COLORS 4000
#define ACHT_BIT_MAX_COLORS 256
#define COLORTABLE_LENGTH 2000
#define ACHT_BIT_COLORTABLE_LENGTH 130
#define ISOMAX 10

/* Stroemungsdarstellungen: ------------------------------------------ */
#define SIMPEL    1
#define REDUZIERT 2
#define TRACER    3
#define ROLLING   4
/* Isolinien --------------------------------------------------------- */
#define MAX_ISO   20                 /* max. number of isolines ------ */
/* Line width -------------------------------------------------------- */
#define LINE_WIDTH 2.0

/* Color packing routines -------------------------------------------- */
#  define PACK_COLOR(R,G,B,A)   ( (A)<<24 | (B)<<16 | (G)<<8 | (R) )
#  define UNPACK_RED(X)         ( (X) & 0xff )
#  define UNPACK_GREEN(X)       ( ( (X) >> 8 ) & 0xff )
#  define UNPACK_BLUE(X)        ( ( (X) >> 16 ) & 0xff )
#  define UNPACK_ALPHA(X)       ( ( (X) >> 24 ) & 0xff )

/* Math macros ------------------------------------------------------- */
#ifndef M_PI
# define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif
# define DEG2RAD(a) ( (a) * M_PI ) / 180.0F
# define RAD2DEG(a) ( (a) * 180.0F ) / M_PI 

/* Color routines ---------------------------------------------------- */
# define COPYCOLOR3(a,b)  ( (a)[0] = (b)[0], (a)[1] = (b)[1], (a)[2] = (b)[2] )

# define COPYCOLOR4(v1,v2)\
    ( (v1)[0] = (v2)[0],\
      (v1)[1] = (v2)[1],\
      (v1)[2] = (v2)[2],\
      (v1)[3] = (v2)[3] )

/* Common routines for speeding the program -------------------------- */
#define crossprod(v1,v2,n)\
        ((n)[0] = (v1)[1]*(v2)[2]-(v1)[2]*(v2)[1],\
         (n)[1] = (v1)[2]*(v2)[0]-(v1)[0]*(v2)[2],\
         (n)[2] = (v1)[0]*(v2)[1]-(v1)[1]*(v2)[0],\
          normalize(n))
#define dotproduct(v1,v2) \
        ((v1)[0]*(v2)[0]+(v1)[1]*(v2)[1]+(v1)[2]*(v2)[2])
#define Vecdis(vd,v1,v2)\
        ((vd)[0] = (v2)[0]-(v1)[0],\
         (vd)[1] = (v2)[1]-(v1)[1],\
         (vd)[2] = (v2)[2]-(v1)[2])
#define Veclen(l,v1)\
        ((l) = sqrt((v1)[0]*(v1)[0]+(v1)[1]*(v1)[1]+(v1)[2]*(v1)[2]))
#define Vecsub(vd,v1,v2) (Vecdis(vd,v2,v1))
#define Vecadd(vd,v1,v2) \
	((vd)[0] = (v1)[0]+(v2)[0],\
	 (vd)[1] = (v1)[1]+(v2)[1],\
	 (vd)[2] = (v1)[2]+(v2)[2])
#define Vecinv(vd,v1) \
	((vd)[0] = -(v1)[0],\
	 (vd)[1] = -(v1)[1],\
	 (vd)[2] = -(v1)[2])
#define Veccpy(vd,v1) \
        ((vd)[0] = (v1)[0],\
	 (vd)[1] = (v1)[1],\
	 (vd)[2] = (v1)[2])
#define Vecxscal(vd,v1,a) \
	((vd)[0] = (a)*(v1)[0],\
	 (vd)[1] = (a)*(v1)[1],\
	 (vd)[2] = (a)*(v1)[2])
#define DisPointToPlane(d,v1,n,dp) \
        ((d)=(((v1)[0]*(n)[0]+(v1)[1]*(n)[1]+(v1)[2]*(n)[2]+(dp)) / \
        (sqrt((n)[0]*(n)[0]+(n)[1]*(n)[1]+(n)[2]*(n)[2]))))

#define NormalToPlane(np,n) \
        ((np)[0]=(n)[0]/(sqrt((n)[0]*(n)[0]+(n)[1]*(n)[1]+(n)[2]*(n)[2])), \
	 (np)[1]=(n)[1]/(sqrt((n)[0]*(n)[0]+(n)[1]*(n)[1]+(n)[2]*(n)[2])), \
	 (np)[2]=(n)[2]/(sqrt((n)[0]*(n)[0]+(n)[1]*(n)[1]+(n)[2]*(n)[2])))
#define INFINITE  1e20;
#define TOLER 0.000001

#define MAXVEC(v1,v2) \
        (((sqrt((v1)[0]*(v1)[0]+(v1)[1]*(v1)[1]+(v1)[2]*(v1)[2])) > \
          (sqrt((v2)[0]*(v2)[0]+(v2)[1]*(v2)[1]+(v2)[2]*(v2)[2]))) ? \
	  (1) : (0))
#define MINVEC(v1,v2) \
        ( (( sqrt((v1)[0]*(v1)[0]+(v1)[1]*(v1)[1]+(v1)[2]*(v1)[2])  ) < \
           ( sqrt((v2)[0]*(v2)[0]+(v2)[1]*(v2)[1]+(v2)[2]*(v2)[2])  )) ? \
	   (1) : (0) )

/* Math functions -----------------------------------------------------*/
double heaviside(double x, double b, double t);
double staircase(double x, double b, double c);
double interpolate2(double t1, double t2, double ratio);
int box2plane(float *boxExtent, float (*boxAxis)[3], float *plane,
              float *center);
int edge_inter_plane(float *plane, float *p1, float *p2, float *ip);
float dist_pt2plane(float *pt, float *plane);
struct dlist* graham(int n, t_point **inpoints, float *pnormal);
void newell(float (*verts)[3], int nverts, float *plane);
void rotatept2vect(float *dst, const float *dir, const float *point, float degrees);
void projectpt2plane(float *dst, const float *p, const float *normal);
void concatrot(float in1[3][3], float in2[3][3], float out[3][3]);
void perpvect (float *dst, const float *src);
void anglevec (float *angle, const float *vec1, const float *vec2);
void onhull(int n, t_point **inpoints, float *pnormal);
void matMxV16(float *M, float *v, float *vr); 
void concattransforms ( float in1[3][4], float in2[3][4], float out[3][4] );
float lerpangle( float a2, float a1, float frac );
float anglemod(float a);
void M_settranslation(float v[4], float mat[16]);
void M_settranslation3f( float vx, float vy, float vz, float mat[16]);
void M_gettranslation(float v[4], float mat[16]);
void M_identity(float mat[16]);
void M_multmatrix( float in1[16], float in2[16], float out[16] );
void M_copymatrix( float in[16], float out[16] );
void M_setrotationx ( float v,  float mat[16] );
void M_setrotationy ( float v,  float mat[16] );
void M_setrotationz ( float v,  float mat[16] );
void M_setscale ( float s,  float mat[16] );
void M_printmatrix( float mat[16] );
void M_transfmat_to_opengl( float mat[4][4], float matopengl[16]);
/* Zeichenroutinen --------------------------------------------------- */
float get_maxkoord(void);
void set_maske(int aktion, int geb_nr, int art);
void get_anzahl(int *anz, int *r_anz);
int  init_color3d();
void setup(void);
void draw_kreuz(void);
void draw_rahmen(void);
void draw_polyline(void);
void darrowfv(GLfloat *spos, GLfloat *epos, GLfloat scalef, GLfloat fixsize);
void darrow2d(GLfloat *spos, GLfloat *epos, GLfloat scalef, GLfloat fixsize);
void zeichne_geometrie(void);
void draw_origin(void);
int  zeichne_stoff(int Stoffnr);
void zeichne_oberflaeche(void);
int  zeichne_gitterstoff(int a, int b);
int  zeichne_result(int Stoffnr);
int  zeichne_result_tex(int Stoffnr);
int  init_colors();
int  init_geo(int gl_width, int gl_height);
void swap_buffers(void);
void zeichne();
void model_matrix(void);
void track_model_matrix(void);
void project_matrix(void);
void C3D_track_project_matrix(void);
void C3D_finish_track_project_matrix(void);
void C3D_save_matrix(void);
t_loesung *C3D_searchresult(int, char *);
void update_scene(void);
void grafik_translate(void);
void grafik_rotate(void);
void grafik_zoom(void);
char punkt_vor_ebene(t_koord kord);
void displayAccum(int accsize);
void C3D_getworldpoint( int x, int y, float *v );
void C3D_fullscreen( Display *dpy, Window win, t_bool fullscreen );
/* Routinen fuer die Colormap --------------------------------------- */
void SetColorMap2000();		/* Belegt Standard 24-Bit Colormap -- */
void SetColorMap256();		/* Belegt Standard  8-Bit Colormap -- */
void SetColorMapGrau();		/* Belegt Standard  8-Bit Graumap  -- */
void SetColorMapRaster();       /* Belegt Standard  8-Bit Rastermap - */
int read_colortable();          /* Liest eine 24- oder 8er Colormap - */
void C3D_updateisolines();		/* Aktualisiert Cmap bez. Isolinien - */
void C3D_updateiso( struct list *liso, t_bool isosurface, t_bool fcreate );
void C3D_updatecut( struct list *lcut, t_bool fcreate );
void C3D_clip_tex(float,float);
void C3D_clip_tex_reset();
int  C3D_drawisosurface(t_iso *tiso, int nr);
void C3D_drawisovolume(t_iso *tiso, int nr);
int  C3D_drawcut(t_isocut *icut, int nr);
int C3D_drawobjects();
void C3D_updateref();
int change_isolinie_col(int liniennumer); /* Change color of the isol.*/
int change_transp_col();                  /* Change transp. color --- */
void maptcolor(int index, GLubyte red, 
	       GLubyte green, GLubyte blue);   /* Get default colormap -- */
void true_cmp();
GLubyte* Ccol(float tmp); /* Get color value ------------------- */
Colormap getShareableColormap(Display *dpy, 
			      XVisualInfo *vi);/* Get shareable colormap- */
/* Routinen fuer die Texture Colormap -------------------------------- */
GLfloat CcolTex(float tmp);
t_colorub C3D_getcolor( float v, float vmin, float vmax, int type );
void myinit(int flag, t_bool do_again);
void makeTexImageDef(int tex_size, int ntex);
void makeTexImageRas(void);
void disableTex(void);
void disableLighting();
/* Lighting --------------------------------------------------------- */
int lighting();
void normalize(float v[3]);
void calvec(float p1[3], float p2[3], float p3[3], 
	    GLfloat v1[3], GLfloat v2[3]);
void face_normals(float *norm, t_arbeits_element *elem, 
		  int akt_knot, int akt_flaeche);
/*void crossprod(float v1[3], float v2[3], float out[3]);*/
/* Cutting ---------------------------------------------------------- */
void mouse_plane(Widget w, XEvent *event, 
	   String* params, Cardinal* paramsAnzAdr); 
void find_edges(int Stoffnr);
void draw_cut_model();

/* Routinen fuer das Colormap-fenster ------------------------------- */
void schliesse_fenster(int);

/* Routinen fuer die Abkuehlkurven ---------------------------------- */
void init_abk_fenster();
void zeichne_skala(char**, char**, int, int);
void zeichne_kurven(float, float, float, float);
void DrawText(char*, XFontStruct*, Widget, float, float);
int gnuplot(void);
void DrawNumberAtPos( float, Widget,XFontStruct*, float, float);
int GetNumber(float, char*, int);
XFontStruct* QueryFont(Display*, int, GLXContext);
void fill_tt_picking();
void nice_label(double min, double max, char** labels, int *nticks);

/* Routinen fuer die Cutting -------------------------------------- */
void calcnorm( TRIANGLE *tri );
void drawscene (void);
void drawcutted(void);
void limitrot();     /* limits rotation         */
void mouse();        /* handle mouse invents    */
int  tracking();     /*                         */
void draw_scene();   /* draw complete scene     */
void setmatrix();    /* viewing transformations */
void setinvmatrix(); /* inv. viewing trans.     */
void winrect();      /* draw window rectangle   */
void initparams();   /* intialize pramas        */
void transpose(float *mat, float *mat_tran);
void multMxV ( float *mat,double *plv, float *rv );
void farclip(long b);
/* Routinen fuer Object Drawing ----------------------------------- */
int draw_object(GLboolean, char* );

extern t_transform g_transform;  /* alle Transformationen d.Geometrie */
extern t_projektion g_projektion;/* alle Projektionen der Geometrie - */
extern t_texture_info   g_tex_info;  /* texture info for the colortab.--- */
extern t_bewegen g_bewegen;      /* Bewegung v. Stoffen gegeneinander */
extern t_bool Redraw;            /* Geometrie neu zeichnen ---------- */
extern int  g_colormap_index;    /* Index fuer Colormapzugiff ------- */
extern long Zmin, Zmax;          /* Min- u.Maximale Grenze f.Z-Buffer */
extern t_isolinie g_isolinien;   /* Isolinien ----------------------- */
extern t_isosurfaces g_isosurfaces; /* Isosurfaces ------------------ */
extern t_isovolumes g_isovolumes;   /* Isovolumes ------------------- */
extern t_isocuts g_isocuts;         /* Cuts ------------------------- */
extern t_color g_tcmap[MAX_COL]; /* true color table ---------------- */
extern t_bool b_light_on;        /* flag for lighting --------------- */
extern t_color_extra g_color_extra; /* extras for colors -----------  */
extern t_bool verschiebung_on;   /* Stoffe-Verschiebung laeuft ------ */
extern int    verschiebe_stoff;  /* Stoffnr d. aktuell versch.Stoffes */
extern float  g_schritt;         /* Schrittweite fuer Verschiebung    */
extern int   g_dis_cutting[100]; /* material list which is not cut -- */
extern Tri   *g_listp[MAX_ISO];  /* global list pointer---------------*/
extern float xr[];               /* Contains x rotation-cutting plane-*/
extern float yr[];               /* Contains y rotation-cutting plane-*/
extern float depth[];            /* Contains depth of cutting plane --*/
extern t_edge edgeinfo;          /* Contains info for edge drawing ---*/
extern float g_factor;           /* Contains factor for polygon off. -*/
extern float g_bias;             /* Contains bias for polygon off. ---*/

/* Feld mit RGB-Farben ---------------------------------------------- */
extern GLubyte RGB_Schwarz[3];
extern GLubyte RGB_Weiss[3];
extern GLubyte RGB_Rot[3];
extern GLubyte RGB_Gruen[3];
extern GLubyte RGB_Blau[3];
extern GLubyte RGB_Gelb[3];
extern GLubyte RGB_Cyan[3];
extern GLubyte RGB_Magenta[3];
extern GLubyte RGB_Grau[3];
extern GLubyte RGB_Netz[3];
extern GLubyte RGB_Stoffe[100][3]; 
extern GLubyte RGB_Color[2000][3]; 
extern int*    colortable;
extern int IsoFarben[5];
extern GLubyte colorTex[4];
extern GLubyte texImage[4][4*TEX_WIDTH];
extern GLubyte texImageRaster[4*TEX_WIDTH_RASTER];
extern GLuint  texName[4];
extern t_color chcolor;                 /* Contains color of choosing ---*/
extern GLubyte c3d_foreground_color[3]; /* foreground color -------------*/
/* Transformationsmatrizen --------------------------------------------- */
extern GLfloat  Save_pr_matrix[16],    /* gesicherte Projektionsmatrix - */
                Save_mo_matrix[16],    /* gesicherte Modellmatrix ------ */
                Projektions_matrix[16],/* Matrix mit Projektionen ------ */
                Zeichen_matrix[16],    /* Matrix mit Transformationen -- */
                Rotation_matrix[16],   /* Matrix mit rotations Transf. - */
                Translation_matrix[16]; /* Translation transf. --------- */

extern GLubyte htone1[]; /* Halftone-Pattern f. transparente Darstellung */
extern GLubyte qtone1[]; /* Quarttone-Pattern f.transparente Darstellung */
extern GLubyte qtone2[]; /* Quarttone-Pattern f.transparente Darstellung */
extern GLubyte qtone3[]; /* Quarttone-Pattern f.transparente Darstellung */
extern GLubyte qtone4[]; /* Quarttone-Pattern f.transparente Darstellung */

extern Display *dpy;     /* Display f. Grafikfensterinitialisierung  */
extern Colormap cmap;    /* Colormap ------------------------------- */
extern refexport_t re;

#endif   /* #ifdef USE_GL */
















