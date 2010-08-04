#ifdef _WIN32
#  include <windows.h>
#endif

#include <stdio.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef __linux__
#ifndef GL_COLOR_INDEX8_EXT
#define GL_COLOR_INDEX8_EXT GL_COLOR_INDEX
#endif
#endif

#include "ref.h"
#include "../graphics/common.h"
#include "../graphics/shared.h"

#include "c3d_gl.h"

#define	REF_VERSION	"GL 0.01"

// up / down
#define	PITCH	0

// left / right
#define	YAW		1

// fall over
#define	ROLL	2


#ifndef __VIDDEF_T
#define __VIDDEF_T
typedef struct
{
	unsigned		width, height;			// coordinates from main game
} viddef_t;
#endif

extern	viddef_t	vid;

//typedef t_koord vec3_t;

/*

  skins will be outline flood filled and mip mapped
  pics and sprites with alpha will be outline flood filled
  pic won't be mip mapped

  model skin
  sprite frame
  wall texture
  pic

*/

typedef enum 
{
	it_skin,
	it_sprite,
	it_wall,
	it_pic,
	it_sky
} t_imagetype;

typedef struct image_s
{
	char	name[MAX_QPATH];			// game path, including extension
	t_imagetype	type;
	int		width, height;				// source image
	int		upload_width, upload_height;	// after power of two and picmip
	int		registration_sequence;		// 0 = free
	struct msurface_s	*texturechain;	// for sort-by-texture world drawing
	int		texnum;						// gl texture binding
	float	sl, tl, sh, th;				// 0,0 - 1,1 unless part of the scrap
	t_bool	scrap;
	t_bool	has_alpha;

	t_bool paletted;
} t_image;

#define	TEXNUM_LIGHTMAPS	1024
#define	TEXNUM_SCRAPS		1152
#define	TEXNUM_IMAGES		1153

#define		MAX_GLTEXTURES	1024

//===================================================================

typedef enum
{
	rserr_ok,

	rserr_invalid_fullscreen,
	rserr_invalid_mode,

	rserr_unknown
} rserr_t;

//#include "gl_model.h"

void GL_BeginRendering (int *x, int *y, int *width, int *height);
void GL_EndRendering (void);

void GL_SetDefaultState( void );
void GL_updateswapinterval( void );

extern	float	gldepthmin, gldepthmax;

typedef struct
{
	float	x, y, z;
	float	s, t;
	float	r, g, b;
} glvert_t;


#define	MAX_LBM_HEIGHT		480

#define BACKFACE_EPSILON	0.01


//====================================================

extern	t_image		gltextures[MAX_GLTEXTURES];
extern	int			numgltextures;


extern	t_image		*r_notexture;
extern	t_image		*r_particletexture;
extern	int			r_visframecount;
extern	int			r_framecount;
//extern	cplane_t	frustum[4];



extern	int			gl_filter_min, gl_filter_max;

//
// view origin
//
extern	vec3_t	vup;
extern	vec3_t	vpn;
extern	vec3_t	vright;
extern	vec3_t	r_origin;

//
// screen size info
//
//extern	refdef_t	r_newrefdef;
extern	int		r_viewcluster, r_viewcluster2, r_oldviewcluster, r_oldviewcluster2;

extern	t_cvar	*r_norefresh;
extern	t_cvar	*r_lefthand;
extern	t_cvar	*r_drawentities;
extern	t_cvar	*r_drawworld;
extern	t_cvar	*r_speeds;
extern	t_cvar	*r_fullbright;
extern	t_cvar	*r_novis;
extern	t_cvar	*r_nocull;
extern	t_cvar	*r_lerpmodels;

extern	t_cvar	*r_lightlevel;	// FIXME: This is a HACK to get the client's light level

extern t_cvar	*gl_vertex_arrays;

extern t_cvar	*gl_ext_swapinterval;
extern t_cvar	*gl_ext_palettedtexture;
extern t_cvar	*gl_ext_multitexture;
extern t_cvar	*gl_ext_pointparameters;
extern t_cvar	*gl_ext_compiled_vertex_array;
extern t_cvar   *gl_ext_subtexture;

extern t_cvar	*gl_particle_min_size;
extern t_cvar	*gl_particle_max_size;
extern t_cvar	*gl_particle_size;
extern t_cvar	*gl_particle_att_a;
extern t_cvar	*gl_particle_att_b;
extern t_cvar	*gl_particle_att_c;

extern	t_cvar	*gl_nosubimage;
extern	t_cvar	*gl_bitdepth;
extern	t_cvar	*gl_mode;
extern	t_cvar	*gl_log;
extern	t_cvar	*gl_lightmap;
extern	t_cvar	*gl_shadows;
extern	t_cvar	*gl_dynamic;
extern  t_cvar  *gl_monolightmap;
extern	t_cvar	*gl_nobind;
extern	t_cvar	*gl_round_down;
extern	t_cvar	*gl_picmip;
extern	t_cvar	*gl_skymip;
extern	t_cvar	*gl_showtris;
extern	t_cvar	*gl_finish;
extern	t_cvar	*gl_ztrick;
extern	t_cvar	*gl_clear;
extern	t_cvar	*gl_cull;
extern	t_cvar	*gl_poly;
extern	t_cvar	*gl_texsort;
extern	t_cvar	*gl_polyblend;
extern	t_cvar	*gl_flashblend;
extern	t_cvar	*gl_lightmaptype;
extern	t_cvar	*gl_modulate;
extern	t_cvar	*gl_playermip;
extern	t_cvar	*gl_drawbuffer;
extern	t_cvar	*gl_3dlabs_broken;
extern  t_cvar  *gl_driver;
extern	t_cvar	*gl_swapinterval;
extern	t_cvar	*gl_texturemode;
extern	t_cvar	*gl_texturealphamode;
extern	t_cvar	*gl_texturesolidmode;
extern  t_cvar  *gl_saturatelighting;
extern  t_cvar  *gl_lockpvs;

extern	t_cvar	*vid_fullscreen;
extern	t_cvar	*vid_gamma;

extern	t_cvar		*intensity;

extern	int		gl_lightmap_format;
extern	int		gl_solid_format;
extern	int		gl_alpha_format;
extern	int		gl_tex_solid_format;
extern	int		gl_tex_alpha_format;

extern	int		c_visible_lightmaps;
extern	int		c_visible_textures;

extern	float	r_world_matrix[16];


void GL_bind (int texnum);
void GL_mbind( GLenum target, int texnum );
void GL_texenv( GLenum value );
void GL_enablemultitexture( t_bool enable );
void GL_selecttexture( GLenum );

//====================================================================

//extern	model_t	*r_worldmodel;

extern	unsigned	d_8to24table[256];

extern	int		registration_sequence;



int 	R_init( void *hinstance, void *hWnd );
void	R_shutdown( void );

//void R_renderview (refdef_t *fd);
void GL_screenshot_f (void);

t_bool R_cullbox (vec3_t mins, vec3_t maxs);



#if 0
short LittleShort (short l);
short BigShort (short l);
int	LittleLong (int l);
float LittleFloat (float f);

char	*va(char *format, ...);
// does a varargs printf into a temp buffer
#endif

void COM_StripExtension (char *in, char *out);

void	DRAW_getpicsize (int *w, int *h, char *name);
void	DRAW_pic (int x, int y, char *name);
void	DRAW_stretchpic (int x, int y, int w, int h, char *name);
void	DRAW_char (int x, int y, int c);
void	DRAW_tileclear (int x, int y, int w, int h, char *name);
void	DRAW_fill (int x, int y, int w, int h, int c);
void    DRAW_rectangle(int x, int y, int w, int h, int c);
void	DRAW_fadescreen (void);
void	DRAW_stretchraw (int x, int y, int w, int h, int cols, int rows, byte *data);

void	R_BeginFrame( float camera_separation );
void	R_SwapBuffers( int );
void	R_SetPalette ( const unsigned char *palette);
void    R_register( void );

int		Draw_GetPalette (void);

void GL_ResampleTexture (unsigned *in, int inwidth, int inheight, unsigned *out,  int outwidth, int outheight);

struct image_s *R_RegisterSkin (char *name);

void LoadPCX (char *filename, byte **pic, byte **palette, int *width, int *height);
t_image *GL_loadpic (char *name, byte *pic, int width, int height, t_imagetype type, int bits);
t_image	*GL_findimage (char *name, t_imagetype type);
void	GL_texturemode( char *string );
void	GL_imagelist_f (void);

void	GL_settexturepalette( unsigned palette[256] );

void	GL_initimages (void);
void	GL_shutdownimages (void);

void	GL_freeunusedimages (void);

void GL_texturealphamode( char *string );
void GL_texturesolidmode( char *string );

/*
** GL extension emulation functions
*/
//void GL_drawparticles( int n, const particle_t particles[], const unsigned colortable[768] );

/*
** GL config stuff
*/
#define GL_RENDERER_VOODOO		0x00000001
#define GL_RENDERER_VOODOO2   	0x00000002
#define GL_RENDERER_VOODOO_RUSH	0x00000004
#define GL_RENDERER_BANSHEE		0x00000008
#define		GL_RENDERER_3DFX		0x0000000F

#define GL_RENDERER_PCX1		0x00000010
#define GL_RENDERER_PCX2		0x00000020
#define GL_RENDERER_PMX			0x00000040
#define		GL_RENDERER_POWERVR		0x00000070

#define GL_RENDERER_PERMEDIA2	0x00000100
#define GL_RENDERER_GLINT_MX	0x00000200
#define GL_RENDERER_GLINT_TX	0x00000400
#define GL_RENDERER_3DLABS_MISC	0x00000800
#define		GL_RENDERER_3DLABS	0x00000F00

#define GL_RENDERER_REALIZM		0x00001000
#define GL_RENDERER_REALIZM2	0x00002000
#define		GL_RENDERER_INTERGRAPH	0x00003000

#define GL_RENDERER_3DPRO		0x00004000
#define GL_RENDERER_REAL3D		0x00008000
#define GL_RENDERER_RIVA128		0x00010000
#define GL_RENDERER_DYPIC		0x00020000

#define GL_RENDERER_V1000		0x00040000
#define GL_RENDERER_V2100		0x00080000
#define GL_RENDERER_V2200		0x00100000
#define		GL_RENDERER_RENDITION	0x001C0000

#define GL_RENDERER_O2          0x00100000
#define GL_RENDERER_IMPACT      0x00200000
#define GL_RENDERER_RE			0x00400000
#define GL_RENDERER_IR			0x00800000
#define		GL_RENDERER_SGI			0x00F00000

#define GL_RENDERER_MCD			0x01000000
#define GL_RENDERER_OTHER		0x80000000

typedef struct
{
	int         renderer;
	const char *renderer_string;
	const char *vendor_string;
	const char *version_string;
	const char *extensions_string;
    const char *glxextensions_string;
	t_bool  	allow_cds;
} t_glconfig;

typedef struct
{
	float inverse_intensity;
	t_bool   fullscreen;

	int     prev_mode;

	unsigned char *d_16to8table;

	int lightmap_textures;

	int	currenttextures[2];
	int currenttmu;

	float camera_separation;
	t_bool   stereo_enabled;

	unsigned char originalRedGammaTable[256];
	unsigned char originalGreenGammaTable[256];
	unsigned char originalBlueGammaTable[256];
} t_glstate;

extern t_glconfig  gl_config;
extern t_glstate   gl_state;

/*
====================================================================

IMPORTED FUNCTIONS

====================================================================
*/

extern	refimport_t ri;


/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/
/*
void		GLimp_BeginFrame( float camera_separation );
void		GLimp_EndFrame( void );
int 		GLimp_Init( void *hinstance, void *hWnd );
void		GLimp_Shutdown( void );
int     	GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen );
void		GLimp_AppActivate( qboolean active );
void		GLimp_EnableLogging( qboolean enable );
void		GLimp_LogNewFrame( void );

*/
void            C3D_shutdown();
t_bool          GLIMP_initgraphics( Widget w, t_bool fullscreen );
t_bool          GLIMP_init( Widget w, void *dpy );
t_bool          VID_loadrefresh( char *name ); 




