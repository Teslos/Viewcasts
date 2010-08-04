#include "gl_local.h"
t_image *draw_chars;
t_bool scrap_dirty;

void DRAW_initlocal(void)
{
  /* load console characters */
  draw_chars = GL_findimage( "consolechar", it_pic );
  GL_bind( draw_chars->texnum );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}


void DRAW_char( int x, int y, int num )
{
  int  row,col;
  float frow, fcol, size;

  num &=255;
  
  if ( (num&127) == 32 )
    return;             /* space */
  
  if (y <= -8 )
    return;
  
  row = num>>4;
  col = num&15;
  
  frow = row * 0.0625;
  fcol = col * 0.0625;
  
  size = 0.0625;
  GL_bind (draw_chars->texnum);
  glBegin (GL_QUADS);
  glTexCoord2f (fcol, frow);
  glVertex2f (x, y);
  glTexCoord2f (fcol + size, frow);
  glVertex2f (x+8, y);
  glTexCoord2f (fcol + size, frow + size);
  glVertex2f (x+8, y+8);
  glTexCoord2f (fcol, frow + size);
  glVertex2f (x, y+8); 
  glEnd ();
}

t_image	*DRAW_findpic (char *name)
{
  t_image *gl;
  char	fullname[MAX_QPATH];
  
  if (name[0] != '/' && name[0] != '\\')
    {
      sprintf (fullname, "pics/%s.pcx", name);
      gl = GL_findimage (fullname, it_pic);
    }
  else
    gl = GL_findimage (name+1, it_pic);

  return gl;
}

void DRAW_getpicsize (int *w, int *h, char *pic)
{
	t_image *gl;

	gl = DRAW_findpic (pic);
	if (!gl)
	{
		*w = *h = -1;
		return;
	}
	*w = gl->width;
	*h = gl->height;
}
/*
=======================
 DRAW_stretchpic
=======================
*/
void DRAW_stretchpic( int x, int y, int w, int h, char *pic )
{
	t_image *gl;
	gl = DRAW_findpic(pic);
	if( !gl )
	{
		printf( "Can't find pic: %s\n", pic );
		return;
	}
	
	
}

void DRAW_pic (int x, int y, char *pic)
{
	t_image *gl;

	gl = DRAW_findpic (pic);
	if (!gl)
	{
		printf ("Can't find pic: %s\n", pic);
		return;
	}
	if (scrap_dirty)
		Scrap_Upload ();

	if ( ( ( gl_config.renderer == GL_RENDERER_MCD ) || ( gl_config.renderer & GL_RENDERER_RENDITION ) ) 
	     && !gl->has_alpha)
		glDisable (GL_ALPHA_TEST);

	GL_bind (gl->texnum);
	glBegin (GL_QUADS);
	glTexCoord2f (gl->sl, gl->tl);
	glVertex2f (x, y);
	glTexCoord2f (gl->sh, gl->tl);
	glVertex2f (x+gl->width, y);
	glTexCoord2f (gl->sh, gl->th);
	glVertex2f (x+gl->width, y+gl->height);
	glTexCoord2f (gl->sl, gl->th);
	glVertex2f (x, y+gl->height);
	glEnd ();

	if ( ( ( gl_config.renderer == GL_RENDERER_MCD ) || ( gl_config.renderer & GL_RENDERER_RENDITION ) )  
	     && !gl->has_alpha)
		glEnable (GL_ALPHA_TEST);
}
/*
====================
 DRAW_tileclear
 This repeats a 64*64 tile graphic to fill the screen around a sized down
 refresh window.
====================
*/
void DRAW_tileclear( int x, int y, int w, int h, char *pic )
{
	t_image *image;
	image = DRAW_findpic( pic );
	if (!image )
	{
		printf( "Can't find pic: %s\n", pic );
		return;
	}
	if (( ( gl_config.renderer == GL_RENDERER_MCD ) || ( gl_config.renderer & GL_RENDERER_RENDITION ) ) &&
		!image->has_alpha )
			glDisable( GL_ALPHA_TEST );
	
	GL_bind( image->texnum );
	glBegin( GL_QUADS );
	glTexCoord2f( x / 64.0, y / 64.0 );
	glVertex2f( x, y );
	glTexCoord2f( x , y );
	glVertex2f( x+w, y );
	glTexCoord2f( (x+w) / 64.0, (y+h) / 64.0 );
	glVertex2f( x+w, y+h );
	glTexCoord2f( x / 64.0, (y+h) / 64.0 );
	glVertex2f( x, y + h );
	glEnd();
	
	if ((( gl_config.renderer == GL_RENDERER_MCD )  || ( gl_config.renderer & GL_RENDERER_RENDITION )) 
		&& !image->has_alpha )
		glEnable( GL_ALPHA_TEST );
	
}

void DRAW_rectangle ( int x, int y, int w, int h, int c )
{
	glDisable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
	
	glVertex2f( x, y );
	glVertex2f( x+w, y );
	glVertex2f( x+w, y+h );
	glVertex2f( x, y+h );

	glEnd();
	glEnable( GL_TEXTURE_2D );
}

void DRAW_fill (int x, int y, int w, int h, int c)
{
	union
	{
		unsigned	c;
		byte		v[4];
	} color;

	if ( (unsigned)c > 255)
		SYS_error (ERR_FATAL, "Draw_Fill: bad color");

	glDisable (GL_TEXTURE_2D);

	color.c = d_8to24table[c];
	glColor3f (color.v[0]/255.0,
		color.v[1]/255.0,
		color.v[2]/255.0);

	glBegin (GL_QUADS);

	glVertex2f (x,y);
	glVertex2f (x+w, y);
	glVertex2f (x+w, y+h);
	glVertex2f (x, y+h);

	glEnd ();
	glColor3f (1,1,1);
	glEnable (GL_TEXTURE_2D);
}

/*
========================================
 DRAW_fadescreen
========================================
*/

void DRAW_fadescreen(void)
{
	glEnable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glColor4f( 0,0,0,0.8 );
	glBegin(GL_QUADS);

	glVertex2f( 0, 0 );
	glVertex2f( vid.width, 0 );
	glVertex2f( vid.width, vid.height );
	glVertex2f( 0, vid.height );

	glEnd();

	glColor4f( 1,1,1,1 );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
}

/*
======================================
 DRAW_stretchraw
======================================
*/
extern unsigned r_rawpalette[256];
void DRAW_stretchraw( int x, int y, int w, int h, int cols, int rows, byte *data )
{
	unsigned image32[256*256];
	unsigned char image8[256*256];

	int i, j, trows;
	byte *source;
	int frac, fracstep;
	float hscale;
	int  row;
	float t;

	GL_bind(0);

	if ( rows <= 256 )
	{
		hscale = 1;
		trows = rows;
	}
	else
	{	
		hscale = rows/256.0;
		trows = 256;
	}
	t = rows * hscale / 256;

	if ( !cglColorTableEXT )
	{
		unsigned *dest;
		for( i = 0; i < trows; i ++ )
		{
			row = (int) (i*hscale);
			if( row > rows )
				break;
			source = data + cols*row;
			dest = &image32[i*256];
			fracstep = cols*0x10000/256;
			frac = fracstep >> 1;
			for ( j = 0; j < 256; j++ )
			{
				dest[j] = r_rawpalette[source[frac>>16]];
				frac += fracstep;
			}
		}
		glTexImage2D( GL_TEXTURE_2D, 0, gl_tex_solid_format, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, image32 );
	}
	else
	{
		unsigned char *dest;

		for ( i = 0; i < trows; i ++ )
		{
			row = ( int ) ( i *hscale );
			if ( row > rows )
				break;
			source = data + cols * row;
			dest = &image8[i*256];
			fracstep = cols * 0x10000/256;
			frac = fracstep >> 1;
			for ( j = 0; j  < 256; j ++ )
			{
				dest[j] = source[frac>>16];
				frac += fracstep;
			}
		}
		glTexImage2D( GL_TEXTURE_2D,
				0,
				GL_COLOR_INDEX8_EXT,
				256,256,
				0,
				GL_COLOR_INDEX,
				GL_UNSIGNED_BYTE,
				image8 );
	}
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	if ( (gl_config.renderer == GL_RENDERER_MCD ) || ( gl_config.renderer & GL_RENDERER_RENDITION ) )
		glDisable( GL_ALPHA_TEST );

	glBegin( GL_QUADS );
	glTexCoord2f( 0, 0 );
	glVertex2f( x, y );
	glTexCoord2f( 1, 0 );
	glVertex2f( x+w, y );
	glTexCoord2f( 1, t );
	glVertex2f( x+w, y+h );
	glTexCoord2f( 0, t );
	glVertex2f( x, y+h );
	glEnd();


	if ( ( gl_config.renderer == GL_RENDERER_MCD ) || ( gl_config.renderer & GL_RENDERER_RENDITION ) )
		glEnable( GL_ALPHA_TEST );
}
	

