#ifndef __COMMON_H__
#define __COMMON_H__

#include "shared.h"
#include "strukturen.h"
#include "files.h"

#define BASEDIRNAME "./"

typedef struct sizebuf_s
{
  t_bool allowoverflow;         /* if false, do a COM_error */
  t_bool overflowed;            /* set to true if the buffer size failed */
  byte  *data;
  int   maxsize;           
  int   cursize;
  int   readcount;
} sizebuf_t;

void SZ_init( sizebuf_t *buf, byte *data, int length );
void SZ_clear( sizebuf_t *buf );
void *SZ_getspace ( sizebuf_t *buf, int length );
void SZ_write( sizebuf_t *buf, void *data, int length );
void SZ_print( sizebuf_t *buf, char *data);

/* =============================================================== */
void COM_error (int code, char *fmt, ...);
int COM_argc(void);
char *COM_argv(int arg);
void COM_clearargv( int arg );
int  COM_checkparm( char *parm );
void COM_addparm  ( char *parm );

void COM_init ( void );
void COM_initargv( int argc, char **argv );
char *copystring (char *in);

/* =============================================================== */
/*  CMD                                                            */
/*  Command text buffering and command execution                   */
/* =============================================================== */

/*
Any number of commands can be added in a frame, from several different sources.
Most commands come from either keybindings or console line input, but remote
servers can also send across commands and entire text files can be execed.

The + command line options are also added to the command buffer.


*/
#define EXEC_NOW    0     /* don't return until completed */
#define EXEC_INSERT 1     /* insert at current position, but don't run yet */
#define EXEC_APPEND 2     /* add to end of the command buffer */

void CBUF_init();
/* allocates an intial text buffer that will grow as needed */

void CBUF_addtext( char *text );
/* as new commands are generated from the console or keybindings,
 * the text is added to the end of the command buffer.
 */

void CBUF_inserttext( char *text );
/* when a command wants to issue other commands immediately, the text is
 * inserted at the beginning of the buffer, before any remaining unexecuted
 * commands.
 */

void CBUF_executetext( int exec_when, char *text );
/* this can be used in place of either CBUF_addtext or CBUF_inserttext. */

void CBUF_addearlycommands( t_bool clear );
/* adds all the +set commands from the command line */

t_bool CBUF_addlatecommands( void );
/* adds all the remaining + commands from the command line 
 * returns true if any late commands were added, which
 * will keep the demoloop from immediately starting.
 */
     
void CBUF_execute(void);
/* Pulls off \n terminated lines of text from the command buffer and sends
 * them through CMD_executestring. Stops when the buffer is empty.
 * Normally called once per frame. but may be explicitly invoked.
 * Do not call inside a command function !
 */
     

void CBUF_copytodefer( void );
/* These two functions are used to defer any pending commands while a model
 * is being loaded
 */

/* ========================================================================= */

/*
   
Command execution takes a null terminated string, breaks it into tokens,
then searches for a command or variable that matches the first token.

*/

typedef void (*xcommand_t ) (void);

void CMD_init(void);

void CMD_addcommand ( char *cmd_name, xcommand_t function );
/* called by the init functions of other parts of the program to
 * register commands and functions to call for them.
 * The cmd_name is referenced later, so it should not be in temp memory
 * if function is NULL, the command will be forwarded to the server
 * as clc_stringcmd instead executed locally
 */

void CMD_removecommand( char *cmd_name );

t_bool CMD_exists( char *cmd_name );
/* used by the cvar code to check for cvar / command name overlap */

char *CMD_completecommand( char *partial );
/* attempts to match a partial command for automatic command line completion
 * returns NULL if nothing fits
 */

int CMD_argc(void);
char *CMD_argv(int arg);
char *CMD_args(void);

/* The functions that execute commands get their parameters with these
 * functions. CMD_argv() will return empty string, not a NULL
 * if arg > argc, so string operations are always safe.
 */

void CMD_tokenizestring( char *text, t_bool macro_expand );
/* Takes a null terminated string. Does not need to be \n terminated.
 * breaks the string up in arg tokens.
 */

void CMD_executestring( char *text );
/* parse a single line of text into arguments and tries to execute it
 * as if ti was typed at console.
 */
   

void CMD_forwardtoserver ( void );
/* adds the current command line as a clc_stringcmd to the client message.
 */

/* ====================================================================== */
/*  CVAR                                                                  */
/* ====================================================================== */

/*
t_cvar variables are used to hold scalar or string variables that can be 
changed in C mode.

The user can access cvars from the console in three ways:
r_draworder   prints current value
r_draworder 0 sets the current value to 0
set r_draworder 0 as above, but creates the cvar if not present
Cvars are restricted from having the same names as commands to keep
this interface from being ambigous.
*/


extern t_cvar *cvar_vars; 

t_cvar *CVAR_get( char *var_name, char *value, int flags );
/* creates the variable if it doesn't exist, or returns the existing one
  if it exists, the value will not be changed, but flags will be ORed in
  that allows variables to be unarchived without needing bitflags
*/

t_cvar *CVAR_set( char *var_name, char *value );
/* will create the variable if it doesn't exist */

t_cvar *CVAR_forceset( char *var_name, char *value );
/* will set variable even if NOSET or LATCH */

t_cvar *CVAR_fullset( char *var_name, char *value, int flags );


void CVAR_setvalue( char *var_name, float value );
/* expands value to a string and calls CVAR_set */

float CVAR_variablevalue( char *var_name );
/* returns 0 if not defined or non-numeric */

char *CVAR_variablestring( char *var_name );
/* returns empty string if not defined */

char *CVAR_completevariable( char *partial );
/* attemps to match a partial variable name for command line completion
 * returns NULL if nothing fits
 */

void CVAR_getlatchedvars( void );
/* any CVAR_LATCHED variables that have been set will now take effect */

t_bool CVAR_command( void );
/* called by CMD_executestring when CMD_argv(0) doesn't match a known command.
 * returns true if the command was a variable reference that was handled. (print or change)
 */

void CVAR_writevariables( char *path );
/* appends lines containing "set variable value" for all variables
 * with the archive flag set to true.
 */

void CVAR_init(void);

char *CVAR_userinfo( void );
/* returns an info string containing all the CVAR_USERINFO cvars */

char *CVAR_serverinfo( void );
/* returns an info string containing all the CVAR_SERVERINFO cvars */

extern t_bool userinfo_modified;
/* this is set each time a CVAR_USERINFO variable is changed */
/* so that the client knows to send it to the server */

/*
==============================================================

 FILE SYSTEM
==============================================================
*/
char	*FS_nextpath ( char *prevpath );
void	FS_createpath ( char *path );
void    FS_fclosefile( FILE *f );
int     FS_fopenfile( char *filename, FILE **file );
void    FS_read( void *buffer, int len, FILE *f );
int     FS_loadfile( char *path, void **buffer );
void    FS_freefile( void *buffer );
void    FS_initfilesystem( void );

/*
==============================================================

NON-PORTABLE SYSTEM SERVICES

==============================================================
*/
extern t_bool bigendien;
void	SYS_init (void);

void	SYS_appactivate (void);

void	SYS_unloadlib (void);
void	*Sys_getlibAPI (void *parms);
// loads the game dll and calls the api init function

char	*SYS_consoleinput (void);
void	SYS_consoleoutput (char *string);
void	SYS_sendkeyevents (void);
void	SYS_error (char *error, ...);
void	SYS_quit (void);
char	*SYS_getclipboarddata( void );
void	SYS_copyprotect (void);
char   *SYS_basename(char *);
char   *SYS_extension(char *);

#endif
