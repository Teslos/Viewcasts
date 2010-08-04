#ifndef APIENTRY
#  define APIENTRY
#endif

#include <GL/gl.h>

#define wglGetProcAddress(x) NULL 

extern	void ( APIENTRY * cglPointParameterfEXT)( GLenum param, GLfloat value );
extern	void ( APIENTRY * cglPointParameterfvEXT)( GLenum param, const GLfloat *value );
extern	void ( APIENTRY * cglColorTableEXT)( GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid * );
extern	void ( APIENTRY * cglTexSubImage1DEXT)( GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid * );

extern	void ( APIENTRY * cglLockArraysEXT) (int , int);
extern	void ( APIENTRY * cglUnlockArraysEXT) (void);
extern	void ( APIENTRY * cglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );
extern	void ( APIENTRY * cglSelectTextureSGIS)( GLenum );

extern	void ( APIENTRY * cglActiveTextureARB)( GLenum );
extern	void ( APIENTRY * cglClientActiveTextureARB)( GLenum );
extern  void ( APIENTRY * cglNormalize)(GLenum);
extern  void ( APIENTRY * cglxSwapIntervalEXT)(GLint);

/*
** extension constants
*/
#define GL_POINT_SIZE_MIN_EXT				0x8126
#define GL_POINT_SIZE_MAX_EXT				0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT	0x8128
#define GL_DISTANCE_ATTENUATION_EXT			0x8129

#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL GL_NORMALIZE
#endif

#ifdef __sgi
 #ifndef GL_SHARED_TEXTURE_PALETTE_EXT
  #define GL_SHARED_TEXTURE_PALETTE_EXT		GL_TEXTURE_COLOR_TABLE_SGI
 #endif
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif

#define GL_TEXTURE0_SGIS					0x835E
#define GL_TEXTURE1_SGIS					0x835F
#define GL_TEXTURE0_ARB						0x84C0
#define GL_TEXTURE1_ARB						0x84C1
#ifndef GL_TEXTURE0
extern int GL_TEXTURE0;
extern int GL_TEXTURE1;
extern int GL_TEXTURE2;
#endif
