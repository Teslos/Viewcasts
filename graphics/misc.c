/* Misc function for ogl */

#include "gl_local.h"
#include "c3d_gl.h"
#include "grafik.h"
#ifndef GL_TEXTURE0
int GL_TEXTURE0, GL_TEXTURE1;
#endif

t_glconfig gl_config;
t_glstate  gl_state;
t_cvar   *gl_ext_multitexture;
t_cvar   *gl_ext_pointparameters;
t_cvar   *gl_ext_subtexture;
t_cvar   *gl_ext_swapinterval;
extern t_cvar *vid_fullscreen;
static t_cvar   *gl_ext_palettedtexture;

void GL_strings(void)
{
  printf("GL_VENDOR: %s\n", gl_config.vendor_string);
  printf("GL_RENDERER: %s\n", gl_config.renderer_string);
  printf("GL_VERSION: %s\n", gl_config.version_string);
  printf("GL_EXTENSIONS: %s\n", gl_config.extensions_string);
  printf("GLX_EXTENSIONS: %s\n", gl_config.glxextensions_string);
}

int GL_init(void *instance, void *dpy)
{
	int err;
	char renderer_buffer[1000];
	char vendor_buffer[1000];
	char name [100];

	VID_loadrefresh( name );
	R_register();

	/* initalize our specific CGL bindings */
	if  ( !C3DGL_init() )
	{
		C3D_shutdown();
		return -1;
	}

	/* initialize OS specific parts of OpenGL */
	if ( !GLIMP_init(instance, dpy) )
	{
		C3D_shutdown();
		return -1;
	}

	GLIMP_initgraphics( *((Widget *)instance), vid_fullscreen->value );

	/* get various GL strings */
	gl_config.vendor_string     = (char *) glGetString(GL_VENDOR);
	gl_config.renderer_string   = (char *) glGetString(GL_RENDERER);
	gl_config.version_string    = (char *) glGetString(GL_VERSION);
	gl_config.extensions_string = (char *) glGetString(GL_EXTENSIONS);
	gl_config.glxextensions_string = (char *) glXGetClientString(dpy, GLX_EXTENSIONS);


	strcpy( renderer_buffer, gl_config.renderer_string );
	strlwr( renderer_buffer );

	strcpy( vendor_buffer, gl_config.vendor_string );
	strlwr( vendor_buffer );

	if ( strstr( renderer_buffer, "voodoo" ))
	{
		if ( !strstr( renderer_buffer, "rush" ) )
			gl_config.renderer = GL_RENDERER_VOODOO;
		else
			gl_config.renderer = GL_RENDERER_VOODOO_RUSH;
	}
	else if ( strstr( vendor_buffer, "sgi" ) )
		gl_config.renderer = GL_RENDERER_SGI;
	else if ( strstr( renderer_buffer, "permedia" ) )
		gl_config.renderer = GL_RENDERER_PERMEDIA2;
	else if ( strstr( renderer_buffer, "glint" ) )
		gl_config.renderer = GL_RENDERER_GLINT_MX;
	else if ( strstr( renderer_buffer, "glzicd" ) )
		gl_config.renderer = GL_RENDERER_REALIZM;
	else if ( strstr( renderer_buffer, "gdi" ) )
		gl_config.renderer = GL_RENDERER_MCD;
	else if ( strstr( renderer_buffer, "pcx2" ) )
		gl_config.renderer = GL_RENDERER_PCX2;
	else if ( strstr( renderer_buffer, "verite" ) )
		gl_config.renderer = GL_RENDERER_RENDITION;
	else
		gl_config.renderer = GL_RENDERER_OTHER;

	/* grab extensions */
	if ( strstr( gl_config.extensions_string, "GL_EXT_compiled_vertex_array" ) ||
			strstr( gl_config.extensions_string, "GL_SGI_compiled_vertex_array" ) )
	{
		printf( "...enabling GL_EXT_compiled_vertex_array\n" );
		cglLockArraysEXT = (void *) wglGetProcAddress( "glLockArraysEXT" );
		cglUnlockArraysEXT = (void *) wglGetProcAddress( "glUnlockArraysEXT" );
	}
	else
	{
		printf("...GL_EXT_compiled_vertex_array not found\n");
	}

#ifdef _WIN32
	if ( strstr( gl_config.extensions_string, "WGL_EXT_swap_control" ) )
	{
		wglSwapIntervalEXT = (BOOL (WINAPI *) (int)) wglGetProcAdress( "wglSwapIntervalEXT" );
		printf("...enabling WGL_EXT_swap_control\n");
	}
	else
	{
		printf("...WGL_EXT_swap_control not found\n");
	}
#endif
	if ( strstr( gl_config.glxextensions_string, "GLX_SGI_swap_control" ) )
	{
		//cglxSwapIntervalEXT = ( void (APIENTRY *) (GLuint)) wglGetProcAddress( "glXSwapIntervalSGI" );
		printf("...enabling GLX_SGI_swap_control\n");	
	}
	else 
	{
		printf("...GLX_SGI_swap_control not found\n");
	}	

	if ( strstr( gl_config.extensions_string, "GL_EXT_point_parameters" ) )
	{
		if ( gl_ext_pointparameters->value )
		{
			cglPointParameterfEXT = ( void (APIENTRY *) (GLenum, GLfloat) ) 
				wglGetProcAddress( "glPointParameterfEXT" );
			cglPointParameterfvEXT = ( void (APIENTRY *) (GLenum, const GLfloat *) ) 
				wglGetProcAddress( "glPointParameterfvEXT" );
			printf("...using GL_EXT_point_parameters\n" );
		}
		else
		{
			printf("...not using GL_EXT_point_parameters\n");
		}
	}
	else
	{
		printf ("...GL_EXT_point_parameters not found\n");
	}

#ifdef  blaux 
	if ( strstr( gl_config.extensions_string, "3DFX_set_global_palette" ) )
	{
		if ( gl_ext_palettedtexture->value )
		{
			printf("...using 3DFX_set_global_palette\n");
			cgl3DfxSetPaletteEXT = ( void ( APIENTRY *) (GLuint *) ) 
				wglGetProcAddress( "gl3DfxSetPaletteEXT" );
			cglColorTableEXT     = Fake_glColorTableEXT;
		}
		else
		{
			printf("...not using 3DFX_set_global_palette\n");
		}
	}
	else
	{
		printf("...3DFX_set_global_palette not found\n");
	}
#endif

	if ( !cglColorTableEXT &&
			strstr( gl_config.extensions_string, "GL_EXT_paletted_texture" ) && 
			strstr( gl_config.extensions_string, "GL_EXT_shared_texture_palette" ) )
	{
		if ( gl_ext_palettedtexture->value )
		{
			printf( "...using GL_EXT_shared_texture_palette\n");
			cglColorTableEXT = ( void ( APIENTRY *)
				 ( GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid * )) 
			wglGetProcAddress( "glColorTableEXT" );
		}
		else
		{
			printf("...not using GL_EXT_shared_texture_palette\n");
		}
	}
	else
	{
		printf("...GL_EXT_shared_texture_palette not found \n");
	}

	if ( strstr( gl_config.extensions_string, "GL_ARB_multitexture" ) )
	{
		if ( gl_ext_multitexture->value )
		{
			printf("...using GL_ARB_multitexture\n");
			cglMTexCoord2fSGIS = ( void * ) wglGetProcAddress( "glMultiTexCoord2fARB" );
			cglActiveTextureARB = ( void * ) wglGetProcAddress( "glActiveTextureARB" );
			cglClientActiveTextureARB = ( void * ) wglGetProcAddress ( "glClientActiveTextureARB" );
#ifndef GL_TEXTURE0
			GL_TEXTURE0 = GL_TEXTURE0_ARB;
			GL_TEXTURE1 = GL_TEXTURE1_ARB;
#endif
		}
		else
		{
			printf("...not using GL_ARB_multitexture\n" );
		}
	}
	else
	{
		printf("...GL_ARB_multitexture not found\n");
	}

	if ( strstr( gl_config.extensions_string, "GL_SGIS_multitexture" ) )
	{
		if (cglActiveTextureARB)
		{
			printf("...GL_SGIS_multitexture deprecated in favor of ARB_multitexture\n" );
		}
		else if ( gl_ext_multitexture->value )
		{
			printf("...using GL_SGIS_multitexture\n");
			cglMTexCoord2fSGIS = ( void * ) wglGetProcAddress( "glMTexCoord2fSGIS" );
			cglSelectTextureSGIS = ( void * ) wglGetProcAddress( "glSelectTextureSGI" );
#ifndef GL_TEXTURE0
			GL_TEXTURE0 = GL_TEXTURE0_SGIS;
			GL_TEXTURE1 = GL_TEXTURE1_SGIS;
#endif
		}
		else
		{
			printf ("...not using GL_SGIS_multitexture\n");
		}
	}
	else
	{
		printf("...GL_SGIS_multitexture not found\n");
	}

	if ( strstr( gl_config.extensions_string, "GL_EXT_subtexture" ) )
	{
		if ( gl_ext_subtexture->value )
		{
			printf( "...using GL_EXT_subtexture\n" );
			cglTexSubImage1DEXT = ( void * ) wglGetProcAddress( "glTexSubImage1DEXT" );

		}
		else
		{
			printf ("...not using GL_EXT_subtexture\n");
		}
	}
	else
	{
		printf( "...GL_EXT_subtexture not found \n");
	}

	GL_updateswapinterval();

	err = glGetError();
	if ( err != GL_NO_ERROR )
		printf("glGetError() = 0x%x\n", err);

}

// this is the slow, general version
int BoxOnPlaneSide ( vec3_t emins, vec3_t emaxs, struct cplane_t *p )
{
	int		i;
	float	dist1, dist2;
	int		sides;
	vec3_t	corners[2];

	for (i=0 ; i<3 ; i++)
	{
		if (p->normal[i] < 0)
		{
			corners[0][i] = emins[i];
			corners[1][i] = emaxs[i];
		}
		else
		{
			corners[1][i] = emins[i];
			corners[0][i] = emaxs[i];
		}
	}
	dist1 = dotproduct (p->normal, corners[0]) - p->dist;
	dist2 = dotproduct (p->normal, corners[1]) - p->dist;
	sides = 0;
	if (dist1 >= 0)
		sides = 1;
	if (dist2 < 0)
		sides |= 2;

	return sides;
}

void GL_updateswapinterval( void )
{
    if ( gl_swapinterval->modified )
    {
        gl_swapinterval->modified = FALSE;

        if ( !gl_state.stereo_enabled ) 
        {
#ifdef _sgi
            if ( cglxSwapIntervalEXT )
                cglxSwapIntervalEXT( gl_swapinterval->value );
#endif
        }
    }
}






