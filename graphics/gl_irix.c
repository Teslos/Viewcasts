#include "../graphics/gl_local.h"

void ( APIENTRY * cglPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY * cglPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY * cglColorTableEXT)( GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid * );
void ( APIENTRY * cglTexSubImage1DEXT)( GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid * );
void ( APIENTRY * cglSelectTextureSGIS)( GLenum );
void ( APIENTRY * cglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );
void ( APIENTRY * cglActiveTextureARB)( GLenum );
void ( APIENTRY * cglClientActiveTextureARB)( GLenum );
void ( APIENTRY * cglLockArraysEXT) (int , int);
void ( APIENTRY * cglUnlockArraysEXT) (void);
void ( APIENTRY * cglNormalize) (GLenum);
void ( APIENTRY * cglxSwapIntervalEXT)(GLint);


t_bool C3DGL_init()
{
#ifdef HAVE_SGI
	cglPointParameterfEXT = 0;
	cglColorTableEXT = glColorTableSGIS;
	cglTexSubImage1DEXT = glTexSubImage1DEXT;
	cglSelectTextureSGIS = 0;
	cglMTexCoord2fSGIS = 0;
	cglActiveTextureARB = 0;
	cglClientActiveTextureARB = 0;
	cglLockArraysEXT = 0;
	cglUnlockArraysEXT = 0;
	cglNormalize = glEnable;
	cglxSwapIntervalEXT = glXSwapIntervalSGI;
#endif
	cglPointParameterfEXT = 0;
	cglColorTableEXT = glColorTable;
	cglTexSubImage1DEXT = glTexSubImage1D;
	cglSelectTextureSGIS = 0;
	cglMTexCoord2fSGIS = 0;
	cglActiveTextureARB = 0;
	cglClientActiveTextureARB = 0;
	cglLockArraysEXT = 0;
	cglUnlockArraysEXT = 0;
	cglNormalize = glEnable;
	cglxSwapIntervalEXT = 0;

	return TRUE;
}

void C3D_shutdown()
{
  
  cglPointParameterfEXT = NULL;
  cglColorTableEXT = NULL;
  cglTexSubImage1DEXT = NULL;
  cglSelectTextureSGIS = NULL;
  cglMTexCoord2fSGIS = NULL;
  cglActiveTextureARB = NULL;
  cglClientActiveTextureARB = NULL;
  cglLockArraysEXT = NULL;
  cglUnlockArraysEXT = NULL;
  cglNormalize = NULL;
  cglxSwapIntervalEXT = NULL;
}
