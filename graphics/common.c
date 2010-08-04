#include "common.h"
#include "memory.h"
#include "../init/c3dinit.h"
#include "../ioclient/keys.h"
#include <stdio.h>
#include <stdarg.h>

#include <setjmp.h>

#define	MAXPRINTMSG	4096

#define MAX_NUM_ARGVS	50


int		com_argc;
char	*com_argv[MAX_NUM_ARGVS+1];

int		realtime;
int		server_state;

/* =============================================================== */

void SZ_init ( sizebuf_t *buf, byte *data, int length )
{
  memset ( buf, 0, sizeof( *buf ));
  buf->data = data;
  buf->maxsize = length;
}


void SZ_clear( sizebuf_t *buf )
{
  buf->cursize = 0;
  buf->overflowed = FALSE;
}

void *SZ_getspace( sizebuf_t *buf, int length )
{
  void  *data;
  if ( buf->cursize + length > buf->maxsize )
    {
      if (!buf->allowoverflow)
	COM_error( ERR_FATAL, "SZ_getspace: overfloe without allowoverflow set" );
      
      if (length > buf->maxsize )
	COM_error( ERR_FATAL, "SZ_getspace: %i is > full buffer size", length );
      printf("SZ_getspace: overflow\n");
      SZ_clear( buf );
      buf->overflowed = TRUE;
      
    }
  data = buf->data + buf->cursize;
  buf->cursize += length;
  
  return data;
}

void SZ_write ( sizebuf_t *buf, void *data, int length )
{
  memcpy ( SZ_getspace(buf, length), data, length );
}

void SZ_print ( sizebuf_t *buf, char *data )
{
  int len;

  len = strlen(data) + 1;
  
  if ( buf->cursize )
    {
      if ( buf->data[buf->cursize-1] )
	memcpy( (byte *) SZ_getspace(buf, len), data, len ); /* no trailing 0 */
      else
	memcpy( (byte *) SZ_getspace(buf, len-1)-1, data, len ); /* write over trailing 0 */
      
    }
  else
    memcpy ( (byte *) SZ_getspace(buf, len), data, len );
}

/* ====================================================================== */

/*
 *
 *
 * Both client and server can use this, and it will
 * do the apropriate things.
 *
 */
void COM_error (int code, char *fmt, ...)
{
	va_list		argptr;
	static char		msg[MAXPRINTMSG];
	static	t_bool	recursive;

	if (recursive)
		SYS_error ("recursive error after: %s", msg);
	recursive = TRUE;

	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);
	/*
	if (code == ERR_DISCONNECT)
	{
		CL_Drop ();
		recursive = false;
		longjmp (abortframe, -1);
	}
	else if (code == ERR_DROP)
	{
		Com_Printf ("********************\nERROR: %s\n********************\n", msg);
		SV_Shutdown (va("Server crashed: %s\n", msg), false);
		CL_Drop ();
		recursive = false;
		longjmp (abortframe, -1);
	}
	else
	{
		SV_Shutdown (va("Server fatal crashed: %s\n", msg), false);
		CL_Shutdown ();
	}

	if (logfile)
	{
		fclose (logfile);
		logfile = NULL;
	}
	*/
	SYS_error ("%s", msg);
}

/* Returns the position ( 1 to argc-1) in the program's argument list
 * where the given parameter apears, or 0 if not present
 */
int COM_checkparam( char *param )
{
  int i;
  
  for ( i = 1; i < com_argc; i++ )
    {
      if ( !strcmp( param, com_argv[i] ) )
	return i;
    }
  return 0;
}

int COM_argc( void )
{
  return com_argc;
}

char *COM_argv( int arg )
{
  if ( arg < 0 || arg >= com_argc || !com_argv[arg] )
    return " ";
  return com_argv[arg];
}

void COM_clearargv( int arg )
{
  if ( arg < 0 || arg >= com_argc || !com_argv[arg] )
    return;
  com_argv[arg] = "";
}

void COM_initargv( int argc, char **argv )
{
  int i;
  
  if ( argc > MAX_NUM_ARGVS )
    COM_error( ERR_FATAL, "argc > MAX_NUM_ARGVS" );
  com_argc = argc;
  for ( i = 0; i < argc; i++ )
    {
      if (!argv[i] || strlen(argv[i]) >= MAX_TOKEN_CHARS )
	com_argv[i] = " ";
      else
	com_argv[i] = argv[i];
    }
}

/*
 * Adds the given string at the ens of the current argument list
 */
void COM_addparm( char *parm )
{
  if (com_argc == MAX_NUM_ARGVS )
    COM_error(ERR_FATAL, "COM_addparam: MAX_NUM_ARGS" );
  com_argv[com_argc++] = parm;
}

char *copystring(char *in)
{
  char *out;
  
  out = (char *) ALLOC(strlen(in)+1);
  strcpy( out, in );
  return out;
}

/*
 * does a varargs printf into a temp buffer, so I don't need to have
 * varargs versions of all text functions.
 */
char	*va(char *format, ...)
{
  va_list		argptr;
  static char		string[1024];
	
  va_start (argptr, format);
  vsprintf (string, format, argptr);
  va_end (argptr);

  return string;	
}

/*
 * Compares strings without looking are they are 
 * different in case.
 */
int stricmp( char *s1, char *s2 )
{
#if defined(WIN32)
	return _stricmp( s1, s2 );
#else
	return strcasecmp( s1, s2 );
#endif 
}

char	com_token[MAX_TOKEN_CHARS];

/*
 *  COM_parse
 *  
 *  Parse a token out of a string
 *  
 */
char *COM_parse (char **data_p)
{
  int		c;
  int		len;
  char	*data;

  data = *data_p;
  len = 0;
  com_token[0] = 0;
	
  if (!data)
    {
      *data_p = NULL;
      return "";
    }
		
  // skip whitespace
  skipwhite:
  while ( (c = *data) <= ' ')
    {
      if (c == 0)
	{
	  *data_p = NULL;
	  return "";
	}
      data++;
    }
	
  // skip // comments
    if (c=='/' && data[1] == '/')
      {
	while (*data && *data != '\n')
	  data++;
	goto skipwhite;
      }

  // handle quoted strings specially
    if (c == '\"')
      {
	data++;
	while (1)
	  {
	    c = *data++;
	    if (c=='\"' || !c)
	      {
		com_token[len] = 0;
		*data_p = data;
		return com_token;
	      }
	    if (len < MAX_TOKEN_CHARS)
	      {
		com_token[len] = c;
		len++;
	      }
	  }
      }

  // parse a regular word
    do
      {
	if (len < MAX_TOKEN_CHARS)
	  {
	    com_token[len] = c;
	    len++;
	  }
	data++;
	c = *data;
      } while (c>32);

  if (len == MAX_TOKEN_CHARS)
    {
      //		Com_Printf ("Token exceeded %i chars, discarded.\n", MAX_TOKEN_CHARS);
      len = 0;
    }
  com_token[len] = 0;

  *data_p = data;
  return com_token;
}

int COM_serverstate (void)
{
  return server_state;
}

/*
int C3D_frame()
{
	re = GetRefAPI( NULL );
}
*/

int C3D_init()
{
	Swap_Init();
	CBUF_init();
	CMD_init();
	CVAR_init();
	KEY_init();
	FS_initfilesystem();
	//CBUF_addtext( "exec default.cfg" );
	CBUF_execute();
	C3D_initvar();
	C3D_initresults();
	printf( "\n==================== ViewCasts initialized ====================\n");
	return 0;
}

/*
===============================================================
       
                        BYTE ORDER FUNCTIONS

===============================================================
*/

t_bool bigendien;

// can't just use function pointers, or dll linkage can
// mess up when qcommon is included in multiple places
short	(*_BigShort) (short l);
short	(*_LittleShort) (short l);
int		(*_BigLong) (int l);
int		(*_LittleLong) (int l);
float	(*_BigFloat) (float l);
float	(*_LittleFloat) (float l);

short	BigShort(short l){return _BigShort(l);}
short	LittleShort(short l) {return _LittleShort(l);}
int		BigLong (int l) {return _BigLong(l);}
int		LittleLong (int l) {return _LittleLong(l);}
float	BigFloat (float l) {return _BigFloat(l);}
float	LittleFloat (float l) {return _LittleFloat(l);}

short   ShortSwap (short l)
{
	byte    b1,b2;

	b1 = l&255;
	b2 = (l>>8)&255;

	return (b1<<8) + b2;
}

short	ShortNoSwap (short l)
{
	return l;
}

int    LongSwap (int l)
{
	byte    b1,b2,b3,b4;

	b1 = l&255;
	b2 = (l>>8)&255;
	b3 = (l>>16)&255;
	b4 = (l>>24)&255;

	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
}

int	LongNoSwap (int l)
{
	return l;
}

float FloatSwap (float f)
{
	union
	{
		float	f;
		byte	b[4];
	} dat1, dat2;
	
	
	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

float FloatNoSwap (float f)
{
	return f;
}

/*
================
Swap_Init
================
*/
void Swap_Init (void)
{
	byte	swaptest[2] = {1,0};

// set the byte swapping variables in a portable manner	
	if ( *(short *)swaptest == 1)
	{
		bigendien = FALSE;
		_BigShort = ShortSwap;
		_LittleShort = ShortNoSwap;
		_BigLong = LongSwap;
		_LittleLong = LongNoSwap;
		_BigFloat = FloatSwap;
		_LittleFloat = FloatNoSwap;
	}
	else
	{
		bigendien = TRUE;
		_BigShort = ShortNoSwap;
		_LittleShort = ShortSwap;
		_BigLong = LongNoSwap;
		_LittleLong = LongSwap;
		_BigFloat = FloatNoSwap;
		_LittleFloat = FloatSwap;
	}

}


