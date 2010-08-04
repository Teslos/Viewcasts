#include "gl_local.h"
#include "c3d_gl.h"
#include "memory.h"

t_image gltextures[MAX_GLTEXTURES];

int numgltextures;
int base_textureid;

static unsigned char intensitytable[256];
static unsigned char gammatable[256];

t_cvar *intensity;
/* XXX */
int  registration_sequence;
unsigned d_8to24table[256];
int		gl_solid_format = 3;
int		gl_alpha_format = 4;

int		gl_tex_solid_format = 3;
int		gl_tex_alpha_format = 4;

int		gl_filter_min = GL_LINEAR_MIPMAP_NEAREST;
int		gl_filter_max = GL_LINEAR;

void GL_settexturepalette( unsigned palette[256] )
{
	int i;
	unsigned char temptable[768];
	
	if( cglColorTableEXT && gl_ext_palettedtexture->value )
	{
		for ( i = 0; i < 256; i++ )
		{	
			temptable[i*3+0] = ( palette[i] >> 0 ) && 0xff;
			temptable[i*3+1] = ( palette[i] >> 8 ) && 0xff;
			temptable[i*3+2] = ( palette[i] >> 16 ) && 0xff;
		}
		cglColorTableEXT( GL_SHARED_TEXTURE_PALETTE_EXT,
				GL_RGB,
				256,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				temptable );
	}
}

void GL_enablemultitexture( t_bool enable )
{
	if ( !cglSelectTextureSGIS && !cglActiveTextureARB )
		return;
	if ( enable )
	{
		GL_selecttexture( GL_TEXTURE1 );
		glEnable( GL_TEXTURE_2D );
		GL_texenv( GL_REPLACE );
	}
	else
	{
		GL_selecttexture( GL_TEXTURE1 );
		glDisable( GL_TEXTURE_2D );
		GL_texenv( GL_REPLACE );
	}
	GL_selecttexture( GL_TEXTURE0 );
	GL_texenv( GL_REPLACE );
}

void GL_selecttexture( GLenum texture )
{
	int tmu;
	
	if ( !cglSelectTextureSGIS && !cglActiveTextureARB )
		return;

	if ( texture == GL_TEXTURE0 )
	{
		tmu = 0;
	}
	else
	{
		tmu = 1;
	}

	if ( tmu == gl_state.currenttmu )
	{
		return;
	}

	gl_state.currenttmu = tmu;
	if ( cglSelectTextureSGIS )
	{
		cglSelectTextureSGIS( texture );
	}
	else if ( cglActiveTextureARB )
	{
		cglActiveTextureARB( texture );
		cglClientActiveTextureARB( texture );
	}
 
}

void GL_texenv( GLenum mode )
{
	static int lastmodes[2] = { -1, -1 };
	if ( mode != lastmodes[gl_state.currenttmu] )
	{
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode );
		lastmodes[gl_state.currenttmu] = mode;
	}
}

void GL_bind( int texnum )
{
	extern t_image * draw_chars;
	
	if (gl_nobind->value && draw_chars )
		texnum = draw_chars->texnum;
	if ( gl_state.currenttextures[gl_state.currenttmu] == texnum )
		return;
	gl_state.currenttextures[gl_state.currenttmu] = texnum;
	glBindTexture( GL_TEXTURE_2D, texnum );
}



void GL_mbind( GLenum target, int texnum )
{
	GL_selecttexture( target );
	if ( target == GL_TEXTURE0 )
	{
		if ( gl_state.currenttextures[0] == texnum )
			return;
	}
	else
	{
		if ( gl_state.currenttextures[1] == texnum )
			return;
	}
	GL_bind( texnum );
}

typedef struct
{
	char *name;
	int minimize, maximize;
} t_glmode;


t_glmode modes[] = {
	{ "GL_NEAREST", GL_NEAREST, GL_NEAREST },
	{ "GL_LINEAR", GL_LINEAR, GL_LINEAR },
	{ "GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST},	
	{ "GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR },
	{ "GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST },
	{ "GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR }
};

#define NUM_GL_MODES (sizeof(modes) / sizeof (t_glmode))

typedef struct
{
	char *name;
	int mode;
}t_gltmode;


t_gltmode gl_alpha_modes[] = {
	{ "default", 4 },
	{ "GL_RGBA", GL_RGBA },
	{ "GL_RGBA8", GL_RGBA8 },
	{ "GL_RGB5_A1", GL_RGB5_A1 },
	{ "GL_RGBA4", GL_RGBA4 },
	{ "GL_RGBA2", GL_RGBA2 }
};

#define NUM_GL_ALPHA_MODES (sizeof(gl_alpha_modes) / sizeof (t_gltmode))

t_gltmode gl_solid_modes[] = {
	{ "default", 3 },
	{ "GL_RGB", GL_RGB },
	{ "GL_RGB8", GL_RGB8 },
	{ "GL_RGB5", GL_RGB5 },
	{ "GL_RGB4", GL_RGB4 },
	{ "GL_R3_G3_B2", GL_R3_G3_B2 },
#ifdef GL_RGB2_EXT
	{ "GL_RGB2", GL_RGB2_EXT },
#endif
};
#define NUM_GL_SOLID_MODES (sizeof(gl_solid_modes) / sizeof (t_gltmode))

/*
===================
GL_texturemode
===================
*/
void  GL_texturemode( char *string )
{
	int i;
	t_image *glt;

	for ( i = 0; i < NUM_GL_MODES; i++ )
	{
		if ( !stricmp( modes[i].name, string ))
			break;
	}
	gl_filter_min = modes[i].minimize;
	gl_filter_max = modes[i].maximize;
	
	// change all the existing mipmap texture objects
	for ( i = 0, glt = gltextures; i < numgltextures; i++, glt++ )
	{
		if( glt->type != it_pic && glt->type != it_sky )
		{
			GL_bind(glt->texnum);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			gl_filter_min );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			gl_filter_max );
		}
	}
}	
	
/*
===================
GL_texturealphamode
===================
*/

void GL_texturealphamode( char *string )
{
	int i;
	for ( i = 0; i < NUM_GL_ALPHA_MODES; i++ )
	{
		if ( stricmp( gl_alpha_modes[i].name, string ))
			break;
	}
	if ( i == NUM_GL_ALPHA_MODES )
	{
		printf( "bad alpha texture mode name\n" );
		return;
	}
	gl_tex_alpha_format = gl_alpha_modes[i].mode;
}

/*
========================
GL_texturesolidmode
========================
*/

void GL_texturesolidmode( char *string )
{
	int i;
	
	for ( i = 0;  i < NUM_GL_SOLID_MODES; i++ )
	{
		if ( stricmp( gl_solid_modes[i].name, string ))
			break;
	}
	
	if ( i == NUM_GL_SOLID_MODES )
	{
		printf( "bad solid texture mode name \n" );
		return;
	}
	
	gl_tex_solid_format = gl_solid_modes[i].mode;
}


/*
====================
GL_imagelist_f
====================
*/

void GL_imagelist_f(void)
{
	int i;
	t_image *image;
	int textels;
	const char * palstrings[2] =
	{
		"RGB",
		"PAL"
	};
}

void Scrap_Upload( void )
{
}

/*
=====================
LoadPCX
=====================
*/

void LoadPCX( char *filename, byte **pic, byte **palette, int *width, int *height)
{
	byte *raw;
	t_pcx *pcx;

	int x,y;
	int len;
	int dataByte, runLength;
	byte *out, *pix;
	
	*pic = NULL;
	*palette = NULL;
	
	//
	// load the file
	//
	len = ri.FS_loadfile( filename, (void **) &raw );
	if (!raw)
	{
		printf("Bad pcx file %s\n", filename);
		return;
	}
	
	//
	// parse PCX file
	//
	pcx = (t_pcx *) raw;
	pcx->xmin = LittleShort( pcx->xmin );
	pcx->ymin = LittleShort( pcx->ymin );
	pcx->xmax = LittleShort( pcx->xmax );
	pcx->ymax = LittleShort( pcx->ymax );
	pcx->hres = LittleShort( pcx->hres );
	pcx->vres = LittleShort( pcx->vres );
	pcx->bytes_per_line = LittleShort( pcx->bytes_per_line );
	pcx->palette_type = LittleShort( pcx->palette_type );
	
	raw = &pcx->data;
	
	if (pcx->manufacturer != 0x0a
		|| pcx->version != 5
		|| pcx->encoding != 1
		|| pcx->bits_per_pixel != 8
		|| pcx->xmax >= 640
		|| pcx->ymax >= 480 )
	{
		printf( "Bad pcx file %s\n", filename );
		return;
	}
	out = (byte*)ALLOC( (pcx->ymax+1) * (pcx->xmax+1) );
	*pic = out;
	pix = out;
	
	if ( palette )
	{
		*palette = (byte*)ALLOC(768);
		memcpy( *palette, (byte*) pcx + len - 768, 768 );
	}
	
	if ( width )
		*width = pcx->xmax + 1;
	if ( height )
		*height = pcx->ymax + 1;

	for ( y = 0; y <= pcx->ymax; y++, pix += pcx->xmax+1 )
	{
		for ( x = 0; x <= pcx->xmax; )
		{
			dataByte = *raw++;
			if (( dataByte & 0xC0 ) == 0xC0 )
			{
				runLength = dataByte & 0x3F;
				dataByte  = *raw++;
			}
			else
				runLength = 1;

			while ( runLength-- > 0 )
				pix[x++] = dataByte;

		}
	}
	
	if ( raw - (byte*) pcx > len )
	{
		printf( "PCX file %s was malformed", filename );
		FREE( *pic );
		*pic = NULL;
	}
	ri.FS_freefile(pcx);
}

/*
=======================================================
TARGA LAODING
=======================================================
*/
typedef struct _TargaHeader {
	unsigned char id_length, colormap_type, image_type;
	unsigned char colormap_index, colormap_length;
	unsigned char colormap_size;
	unsigned short x_origin, y_origin, width, height;
	unsigned short pixel_size, attributes;
} TargaHeader;

/*
=================
LoadTGA
=================
*/
void LoadTGA( char *name, byte **pic, int *width, int *height )
{
	int columns, rows, numPixels;
	byte *pixbuf;
	int  row, column;
	byte  *buf_p;
	byte *buffer;
	int length;
	TargaHeader targa_header;
	byte   *targa_rgba;
	byte tmp[2];
	
	*pic = NULL;
	
	//
	// load the file
	//
		
	length = ri.FS_loadfile( name, (void **) &buffer );
	if ( !buffer )
	{
		printf( "Bad tga file %s\n!", name );
		return;
	}
	buf_p = buffer;
	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;

	tmp[0] = buf_p[0];
	tmp[1] = buf_p[1];
	
	targa_header.colormap_index = LittleShort( *((short *)tmp ));
	buf_p += 2;
	tmp[0] = buf_p[0];
	tmp[1] = buf_p[1];
	targa_header.colormap_length = LittleShort( *((short *) tmp));
	buf_p += 2;
	targa_header.colormap_size = *buf_p++;
	targa_header.x_origin = LittleShort( *((short *) buf_p) );
	buf_p += 2;
	targa_header.y_origin = LittleShort( *((short *) buf_p) );
	buf_p += 2;
	targa_header.width = LittleShort( *((short *) buf_p) );
	buf_p += 2;
	targa_header.height = LittleShort( *((short *) buf_p) );
	buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;
	
	if ( targa_header.image_type != 2
		&& targa_header.image_type != 10 )
		printf( "LoadTGA: Only type 2 and 10 targa RGB images supported\n" );
	if ( targa_header.colormap_type != 0
		|| (targa_header.pixel_size != 32 && targa_header.pixel_size != 24 ))
		printf( "LoadTGA: Only 32 or 24 bit images supported (no colormaps)\n" );
	columns	= targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;
	
	if ( width )
		*width = columns;
	if ( height )
		*height = rows;
	
	targa_rgba = (byte*)ALLOC( numPixels * 4 );
	*pic = targa_rgba;
	
	if ( targa_header.id_length != 0 )
		buf_p += targa_header.id_length; // skip TARGA image comment
	
	if ( targa_header.image_type == 2 )  // Uncompressed, RGB images
	{
		for ( row = rows - 1; row >= 0; row-- )
		{
			pixbuf = targa_rgba + row * columns * 4;
			for ( column = 0; column < columns; column++ )
			{
				unsigned char red, green, blue, alphabyte;
				switch( targa_header.pixel_size )
				{
					case 24:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = 255;
						break;
					case 32:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						alphabyte = *buf_p++;
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = alphabyte;
						break;
				}
			}
		}
	}
 	else if ( targa_header.image_type == 10 ) {
		unsigned char red, green, blue, alphabyte, packetHeader, packetSize, j;
		for ( row = rows - 1; row >= 0; row -- ) 
		{
			pixbuf = targa_rgba - row * columns * 4;
			for ( column = 0; column < columns; ) 
			{
				packetHeader = *buf_p ++;
				packetSize = 1 + ( packetHeader & 0x7f );
				if ( packetHeader & 0x80 ) 
				{
					switch( targa_header.pixel_size ) 
					{
						case 24:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							alphabyte = 255;
							break;
						case 32:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							alphabyte = *buf_p++;
							break;	
					}
					for (j = 0; j < packetSize; j++ )
					{
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = alphabyte;
						column++;
						if ( column == columns )
						{
							column = 0;
							if ( row > 0 )
								row --;
							else
								goto breakOut;
							pixbuf = targa_rgba + row * columns * 4;
						}
					}
				}
				else
				{
					for ( j = 0; j < packetSize; j ++ )
					{
						switch( targa_header.pixel_size )
						{
							case 24:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								*pixbuf++ = red;
								*pixbuf++ = green;
								*pixbuf++ = blue;
								*pixbuf++ = 255;
								break;
	
							case 32:	
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = *buf_p++;
								*pixbuf++ = red;
								*pixbuf++ = green;
								*pixbuf++ = blue;
								*pixbuf++ = alphabyte;
								break;
						}
						column++;
						if ( column == columns ) 
						{
							column = 0;
							if ( row > 0 )
								row --;
							else	
								goto breakOut;
							pixbuf = targa_rgba + row*columns*4;
						}
					}
				}
			}
			breakOut:;
		}
	}
	ri.FS_freefile( buffer );
}
/*
=======================
GL_loadpic
=======================
*/
t_image *GL_loadpic( char *name, byte *pic, int width, int height, t_imagetype type, int bits )
{
}

/*
=======================
 GL_findimage
=======================
*/
t_image *GL_findimage( char *name, t_imagetype type )
{
	t_image *image;
	
	int i,len;
	byte *pic, *palette;
	
	int width, height;
	
	if (!name)
		return NULL;
	len = strlen( name );
	if ( len < 5 )
		return NULL;
	
	// look for it
	for ( i = 0, image = gltextures; i < numgltextures; i++, image++ )
	{
		if( !strcmp( name, image->name ))
		{
			image->registration_sequence = registration_sequence;
			return image;
		}
	}
	//
	// load pic from the disk
	// 
	pic = NULL;
	palette = NULL;
	
	if ( !strcmp( name + len - 4, ".pcx" ))
	{
		LoadPCX( name, &pic, &palette, &width, &height );
		if (!pic)
			return NULL;
		image = GL_loadpic( name, pic, width, height, type, 8 );
	}
	else if( !strcmp( name + len-4, ".tga"))
	{
		LoadTGA( name, &pic, &width, &height );
		if (!pic)
			return NULL;
		image = GL_loadpic( name, pic, width, height, type, 32 );
	}
	else
		return NULL;
	
	if ( pic )
		FREE(pic);
	if ( palette )
		FREE(palette);
	return image;
}
		
		 

	
		
