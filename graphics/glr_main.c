#include "gl_local.h"
#include "grafik.h"
#include "shared.h"
#include "ref.h"
refimport_t	ri;
refdef_t        r_newrefdef;
t_cplane  	frustum[4];
t_cvar  *r_norefresh;
t_cvar  *r_nocull;
t_cvar	*gl_nosubimage;
t_cvar	*gl_allow_software;

t_cvar	*gl_vertex_arrays;

t_cvar	*gl_particle_min_size;
t_cvar	*gl_particle_max_size;
t_cvar	*gl_particle_size;
t_cvar	*gl_particle_att_a;
t_cvar	*gl_particle_att_b;
t_cvar	*gl_particle_att_c;

t_cvar	*gl_ext_swapinterval;
t_cvar	*gl_ext_palettedtexture;
t_cvar	*gl_ext_multitexture;
t_cvar	*gl_ext_pointparameters;
t_cvar	*gl_ext_compiled_vertex_array;
//t_cvar  *gl_ext_subtexture;

t_cvar	*gl_log;
t_cvar	*gl_bitdepth;
t_cvar	*gl_drawbuffer;
t_cvar  *gl_driver;
t_cvar	*gl_lightmap;
t_cvar	*gl_shadows;
t_cvar	*gl_mode;
t_cvar	*gl_dynamic;
t_cvar  *gl_monolightmap;
t_cvar	*gl_modulate;
t_cvar	*gl_nobind;
t_cvar	*gl_round_down;
t_cvar	*gl_picmip;
t_cvar	*gl_skymip;
t_cvar	*gl_showtris;
t_cvar	*gl_ztrick;
t_cvar	*gl_finish;
t_cvar	*gl_clear;
t_cvar	*gl_cull;
t_cvar	*gl_polyblend;
t_cvar	*gl_flashblend;
t_cvar	*gl_playermip;
t_cvar  *gl_saturatelighting;
t_cvar	*gl_swapinterval;
t_cvar	*gl_texturemode;
t_cvar	*gl_texturealphamode;
t_cvar	*gl_texturesolidmode;
t_cvar	*gl_lockpvs;

t_cvar	*gl_3dlabs_broken;

t_cvar	*vid_fullscreen;
t_cvar	*vid_gamma;
t_cvar	*vid_ref;

void GL_strings();

t_bool R_cullbox (vec3_t mins, vec3_t maxs)
{
  int		i;
  
  if (r_nocull->value)
    return FALSE;
  
  for (i=0 ; i<4 ; i++)
    if ( BOX_ON_PLANE_SIDE(mins, maxs, &frustum[i]) == 2)
      return TRUE;
  return FALSE;
}
void R_setupframe(void)
{
}
void R_setfrustum(void)
{
}
void R_setupGL(void)
{
}

void R_clear(void)
{
	
}


void R_renderview(refdef_t *fd )
{
	if ( r_norefresh->value )
		return;

	r_newrefdef = *fd;

	R_setupframe();
	
	R_setfrustum();
	
	R_setupGL();
	
	//R_drawalphasurfaces();
}

void R_setGL2D(void)
{
	glViewport( 0,0, vid.width, vid.height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, vid.width, vid.height, 0, -99999, 99999 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glColor4f(1,1,1,1);	
}

void R_setlightlevel( void )
{
}

void R_renderframe( refdef_t *fd )
{
	R_renderview( fd );
	R_setlightlevel( );
	R_setGL2D();
}

void R_register( void )
{
	//r_lefthand = ri.CVAR_get( "hand", "0", CVAR_USERINFO | CVAR_ARCHIVE );
	//r_norefresh = ri.CVAR_get ("r_norefresh", "0", 0);
	//r_fullbright = ri.CVAR_get ("r_fullbright", "0", 0);
	//r_drawentities = ri.CVAR_get ("r_drawentities", "1", 0);
	//r_drawworld = ri.CVAR_get ("r_drawworld", "1", 0);
	//r_novis = ri.CVAR_get ("r_novis", "0", 0);
	r_nocull = ri.CVAR_get ("r_nocull", "0", 0);
	//r_lerpmodels = ri.CVAR_get ("r_lerpmodels", "1", 0);
	//r_speeds = ri.CVAR_get ("r_speeds", "0", 0);

	//r_lightlevel = ri.CVAR_get ("r_lightlevel", "0", 0);

	gl_nosubimage = ri.CVAR_get( "gl_nosubimage", "0", 0 );
	gl_allow_software = ri.CVAR_get( "gl_allow_software", "0", 0 );

	gl_particle_min_size = ri.CVAR_get( "gl_particle_min_size", "2", CVAR_ARCHIVE );
	gl_particle_max_size = ri.CVAR_get( "gl_particle_max_size", "40", CVAR_ARCHIVE );
	gl_particle_size = ri.CVAR_get( "gl_particle_size", "40", CVAR_ARCHIVE );
	gl_particle_att_a = ri.CVAR_get( "gl_particle_att_a", "0.01", CVAR_ARCHIVE );
	gl_particle_att_b = ri.CVAR_get( "gl_particle_att_b", "0.0", CVAR_ARCHIVE );
	gl_particle_att_c = ri.CVAR_get( "gl_particle_att_c", "0.01", CVAR_ARCHIVE );

	gl_modulate = ri.CVAR_get ("gl_modulate", "1", CVAR_ARCHIVE );
	gl_log = ri.CVAR_get( "gl_log", "0", 0 );
	gl_bitdepth = ri.CVAR_get( "gl_bitdepth", "0", 0 );
	gl_mode = ri.CVAR_get( "gl_mode", "3", CVAR_ARCHIVE );
	gl_lightmap = ri.CVAR_get ("gl_lightmap", "0", 0);
	gl_shadows = ri.CVAR_get ("gl_shadows", "0", CVAR_ARCHIVE );
	gl_dynamic = ri.CVAR_get ("gl_dynamic", "1", 0);
	gl_nobind = ri.CVAR_get ("gl_nobind", "0", 0);
	gl_round_down = ri.CVAR_get ("gl_round_down", "1", 0);
	gl_picmip = ri.CVAR_get ("gl_picmip", "0", 0);
	gl_skymip = ri.CVAR_get ("gl_skymip", "0", 0);
	gl_showtris = ri.CVAR_get ("gl_showtris", "0", 0);
	gl_ztrick = ri.CVAR_get ("gl_ztrick", "0", 0);
	gl_finish = ri.CVAR_get ("gl_finish", "0", CVAR_ARCHIVE);
	gl_clear = ri.CVAR_get ("gl_clear", "0", 0);
	gl_cull = ri.CVAR_get ("gl_cull", "1", 0);
	gl_polyblend = ri.CVAR_get ("gl_polyblend", "1", 0);
	gl_flashblend = ri.CVAR_get ("gl_flashblend", "0", 0);
	gl_playermip = ri.CVAR_get ("gl_playermip", "0", 0);
	gl_monolightmap = ri.CVAR_get( "gl_monolightmap", "0", 0 );
	gl_driver = ri.CVAR_get( "gl_driver", "opengl32", CVAR_ARCHIVE );
	gl_texturemode = ri.CVAR_get( "gl_texturemode", "GL_LINEAR_MIPMAP_NEAREST", CVAR_ARCHIVE );
	gl_texturealphamode = ri.CVAR_get( "gl_texturealphamode", "default", CVAR_ARCHIVE );
	gl_texturesolidmode = ri.CVAR_get( "gl_texturesolidmode", "default", CVAR_ARCHIVE );
	gl_lockpvs = ri.CVAR_get( "gl_lockpvs", "0", 0 );

	gl_vertex_arrays = ri.CVAR_get( "gl_vertex_arrays", "0", CVAR_ARCHIVE );

	gl_ext_swapinterval = ri.CVAR_get( "gl_ext_swapinterval", "1", 0 );
	gl_ext_palettedtexture = ri.CVAR_get( "gl_ext_palettedtexture", "1", CVAR_ARCHIVE );
	gl_ext_multitexture = ri.CVAR_get( "gl_ext_multitexture", "1", CVAR_ARCHIVE );
	gl_ext_pointparameters = ri.CVAR_get( "gl_ext_pointparameters", "1", CVAR_ARCHIVE );
	gl_ext_compiled_vertex_array = ri.CVAR_get( "gl_ext_compiled_vertex_array", "1", CVAR_ARCHIVE );
	gl_ext_subtexture = ri.CVAR_get( "gl_ext_subtexture", "1", 0 );

	gl_drawbuffer = ri.CVAR_get( "gl_drawbuffer", "GL_BACK", 0 );
	gl_swapinterval = ri.CVAR_get( "gl_swapinterval", "2", 0 );

	gl_saturatelighting = ri.CVAR_get( "gl_saturatelighting", "0", 0 );

	gl_3dlabs_broken = ri.CVAR_get( "gl_3dlabs_broken", "1", CVAR_ARCHIVE );

	vid_fullscreen = ri.CVAR_get( "vid_fullscreen", "0", CVAR_ARCHIVE );
	vid_gamma = ri.CVAR_get( "vid_gamma", "1.0", CVAR_ARCHIVE );
	vid_ref = ri.CVAR_get( "vid_ref", "soft", CVAR_ARCHIVE );
	//ri.CMD_addcommand( "imagelist", GL_ImageList_f );
	//ri.CMD_addcommand( "screenshot", GL_ScreenShot_f );
	ri.CMD_addcommand( "gl_strings", GL_strings );
}
/*
t_bool R_setmode(void)
{
	if ( vid_fullscreen->modified && !gl_config.allow_cds )
	{
		printf ( "R_setmode -CDS not allowed with this driver\n");
		CVAR_setvalue( "vid_fullscreen", !vid_fullscreen->value );
		vid_fullscreen->modified = FALSE;
	}
	
	fullscreen = vid_fullscreen->value;
	
	vid_fullscreen->modified = FALSE;
	gl_mode->modified = FALSE;
	
	if ( ( err GLIMP_setmode( &vid.width, &vid.height, gl_mode->value,
	fullscreen ) ) == rserr_ok )
	{
		gl_state.prev_mode = gl_mode.state;
	}
	else
	{
		if ( err == rserr_invalid_fullscreen )
		{
			ri.CVAR_setvalue( "vid_fullscreen", 0);
			vid_fullscreen->modified = FALSE;
			printf( "R_setmode - fullscreen unavailable in this mode");
			if ( ( err = GLIMP_setmode( &vid.width, &vid.height,
							gl_state.prev_mode, FALSE ) ) == rserr_ok )
				return TRUE;
		}
		else if ( err == rserr_invalid_mode )
		{
			r.CVAR_setvalue( "gl_mode", gl_state.prev_mode);
			gl_mode->modified = FALSE;
			printf( "R_setmode - invalid mode\n" );
		}

		if ( ( err = GLIMP_setmode( &vid.width, &vid.height,
						gl_state.prev_mode, FALSE ) ) != rserr_ok )
		{
			printf( "could not revert to safe mode" );
			return FALSE;
		}
	}
	return TRUE;
}
*/
void R_beginframe(float camera_separation)
{
	gl_state.camera_separation = camera_separation;
	GLIMP_beginframe( camera_separation );

	/*
	** draw buffer stuff
	*/
	if ( gl_drawbuffer->modified )
	{
		gl_drawbuffer->modified = FALSE;
		if ( gl_state.camera_separation == 0 || !gl_state.stereo_enabled )
		{
			if ( stricmp( gl_drawbuffer->string, "GL_FRONT" ) == 0 )
				glDrawBuffer( GL_FRONT );
			else	
				glDrawBuffer( GL_BACK );
		}
	}

	/*
	** texturemode stuff
	*/
	if ( gl_texturemode->modified )
	{
		GL_texturemode( gl_texturemode->string );
		gl_texturemode->modified = FALSE;
	}
	
	if ( gl_texturealphamode->modified )
	{
		GL_texturealphamode( gl_texturealphamode->string );
		gl_texturealphamode->modified = FALSE;
	}

	if ( gl_texturesolidmode->modified )
	{
		GL_texturesolidmode( gl_texturesolidmode->string );
		gl_texturesolidmode->modified = FALSE;
	}

	GL_updateswapinterval();

	R_clear();
}

void R_endframe(void)
{
}

/*
=================
 R_setpalette
=================
*/
unsigned r_rawpalette[256];

void R_setpalette( const unsigned char *palette )
{
}

refexport_t GetRefAPI( refimport_t rimp )
{

	refexport_t re;
	
	ri = rimp;
	
	re.api_version = API_VERSION;
	
	re.RenderFrame = R_renderframe;
	re.DrawGetPicSize = DRAW_getpicsize;
	re.DrawPic = DRAW_pic;
	re.DrawStretchPic = DRAW_stretchpic;
	re.DrawChar = DRAW_char;
	re.DrawTileClear = DRAW_tileclear;
	re.DrawFill = DRAW_fill;
	re.DrawRectangle = DRAW_rectangle;
	re.DrawFadeScreen = DRAW_fadescreen;
	re.DrawStretchRaw = DRAW_stretchraw;
	
	//re.Init = R_init;
	re.BeginFrame = R_beginframe;
	re.EndFrame = R_endframe;
	re = GetRefIsoAPI( ri );
	return re;
}
	

