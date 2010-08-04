#include "strukturen.h"
#ifndef __SHARED_H__
#define __SHARED_H__

typedef t_koord  vec3_t;

typedef unsigned char  byte;

#ifndef qboolean
//typedef  enum{false, true} qboolean;
#endif

/*#ifndef t_bool
typedef  int  t_bool;
#endif */

#define MAX_STRING_CHARS   1024         /* max length of a string passed to CMD_tokenizestring() */
#define MAX_STRING_TOKENS  80           /* max tokens resulting from CMD_tokenizestring() */
#define MAX_TOKEN_CHARS    128          /* max length of an individual token */
#define MAX_QPATH          64
#define MAX_OSPATH         128

/* Errors */
#define ERR_FATAL          0

/* ==================================================================== */
short BigShort( short l );
short LittleShort( short l );
int BigLong( int l );
int LittleLong( int l );
float BigFloat( float l );
float LittleFloat( float l );
void Swap_Init( void );

char  *va( char *format, ... );
/* key/value info strings */
#define MAX_INFO_KEY    64
#define MAX_INFO_VALUE  64
#define MAX_INFO_STRING 512
/* ==================================================================== */
/* COM                                                                  */
/* ==================================================================== */
char *COM_skippath( char *pathname );
void  COM_stripextension( char *in, char *out );
void  COM_filebase( char *in, char *out );
void  COM_filepath( char *in, char *out );
void  COM_defaultextension( char *path, char *extension );

char *COM_parse(char **data_p);
int  COM_serverstate (void);
/* data is an in/out parm, returns a parsed out token */

/* ==================================================================== */
/* CVARS (console variables)                                            */
/* ==================================================================== */

#ifndef CVAR
#define CVAR

#define CVAR_ARCHIVE     1      /* set to cause it to be saved to vars.rc */
#define CVAR_USERINFO    2      /* added to userinfo when changed         */
#define CVAR_SERVERINFO  3      /* added to serverinfo when changed       */
#define CVAR_NOSET       8      /* don't allow change from console at all */
                                /* but can be set from the command line   */
#define CVAR_LATCH       16     /* save changes until server restart      */

/* nothing outside CVAR_*( ) functions should modify these fields! */

typedef struct cvar_s
{
  char        *name;   
  char        *string;
  char        *latched_string;       /* for CVAR_LATCH vars */
  int         flags;
  t_bool      modified;              /* set each time the cvar is changed */
  float       value;
  struct cvar_s *next;
} t_cvar;

#endif /* CVAR */

typedef struct cplane_t {
	vec3_t	normal;
	float	dist;
	byte	type;			/* for fast side tests */
	byte	signbits;		/* signx + (signy<<1) + (signz<<1) */
	byte	pad[2];
}t_cplane;

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct cplane_t *plane);

#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
(((p)->type < 3)?						\
 (										\
 ((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))
/*
==============================================================

SYSTEM SPECIFIC

==============================================================
*/

extern	int	curtime;		// time returned by last Sys_Milliseconds

int     SYS_milliseconds (void);
void	SYS_mkdir (char *path);

// directory searching
#define SFF_ARCH    0x01
#define SFF_HIDDEN  0x02
#define SFF_RDONLY  0x04
#define SFF_SUBDIR  0x08
#define SFF_SYSTEM  0x10

/*
** pass in an attribute mask of things you wish to REJECT
*/
char	*SYS_findfirst (char *path, unsigned musthave, unsigned canthave );
char	*SYS_findnext ( unsigned musthave, unsigned canthave );
void	SYS_findclose (void);

#endif
