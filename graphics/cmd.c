#include "memory.h"
#include "common.h"

void CMD_forwardtoserver(void);

#define MAX_ALIAS_NAME 32

typedef struct cmdalias_s
{
  struct cmdalias_s *next;
  char   name[MAX_ALIAS_NAME];
  char  *value;
}t_cmdalias;

t_cmdalias *cmd_alias;


t_bool cmd_wait;

#define ALIAS_LOOP_COUNT  16
int alias_count;


/*
Causes execution of the remainder of the command buffer to be delayed until
next frame. 
*/
void CMD_wait_f(void)
{
  cmd_wait = TRUE;
}

/* =========================================================================== */
/*                               COMMAND BUFFER                                */
/* =========================================================================== */

sizebuf_t  cmd_text;
byte       cmd_text_buf[8192];
byte       defer_text_buf[8192];

void CBUF_init(void)
{
  SZ_init(&cmd_text, cmd_text_buf, sizeof(cmd_text_buf));
}

/*
Adds command text at the end of the buffer
*/
void CBUF_addtext(char *text)
{
  int l;
  
  l = strlen(text);
  if ( cmd_text.cursize + l >= cmd_text.maxsize )
    {
      printf("CBUF_addtext: overflow\n");
      return;
    }
  SZ_write (&cmd_text, text, strlen(text));
}

/* 
Adds command text immediately after the current command
Adds a \n to the text
*/
void CBUF_inserttext(char *text)
{
  char   *temp;
  int     templen;
  
  /* copy off any commands still remaining in the exec buffer */
  templen = cmd_text.cursize;
  if (templen)
    {
      temp = (char*) ALLOC(templen);
      memcpy( temp, cmd_text.data, templen );
      SZ_clear(&cmd_text);
    }
  else
    temp = NULL;  /* shut up compiler */
  
  /* add the entire text of the file */
  CBUF_addtext(text);
  
  /* add the copied off data */
  if ( templen )
    {
      SZ_write ( &cmd_text, temp, templen );
      FREE( temp );
    }
}

void CBUF_copytodefer(void)
{
  memcpy( defer_text_buf, cmd_text_buf, cmd_text.cursize );
  defer_text_buf[cmd_text.cursize] = 0;
  cmd_text.cursize = 0;
}

void CBUF_insertfromdefer(void)
{
  CBUF_inserttext( defer_text_buf );
  defer_text_buf[0] = 0;
}

void CBUF_executetext( int exec_when, char *text )
{
  switch ( exec_when )
    {
    case EXEC_NOW:
      CMD_executestring(text);
      break;
    case EXEC_INSERT:
      CBUF_inserttext(text);
      break;
    case EXEC_APPEND:
      CBUF_addtext(text);
      break;
    default:
      fprintf(stderr, "ERROR FATAL: CBUF_executetext: bad exec_when");
    }
}

void CBUF_execute( void )
{
  int i;
  char *text;
  char line[1024];
  int quotes;
  
  alias_count = 0;
  
  while ( cmd_text.cursize )
    {
      /* find a \n or ; line break */
      text = (char *) cmd_text.data;
      
      quotes = 0;
      for ( i = 0; i < cmd_text.cursize; i++ )
	{
	  if ( text[i] == '"' )
	    quotes++;
	  if ( !(quotes&1) && text[i] == ';' )
	    break;  /* don't break if inside a quoted string */
	  if ( text[i] == '\n' )
	    break;
	}
      
      memcpy (line, text, i);
      line[i] = 0;
      
      /*  delete the text from command buffer and move remaining commands down
       *  this is necessary because commands (exec, alias) can insert data at the
       *  beginning of the text buffer 
       */
      if ( i == cmd_text.cursize )
	cmd_text.cursize = 0;
      else
	{
	  i++;
	  cmd_text.cursize -= i;
	  memmove (text, text+i, cmd_text.cursize);
	}
      /* execute the command line */
      CMD_executestring(line);
      
      if ( cmd_wait )
	{
	  /* skip out while text still remains in buffer, leaving it
	   * for next frame
	   */
	  cmd_wait = FALSE;
	  break;
	}
    }
}

/*
Adds command line parameters as script statements
Commands lead with a +, and continue until another +
Set commands are added early, so they are guaranteed to be set before
the client and server initialize for the first time.

Other commands are added late, after all initialization is complete.
*/

void CBUF_addearlycommands( t_bool clear )
{
  int i;
  char *s;
  
  for ( i = 0; i < COM_argc(); i++ )
  {
    s = COM_argv(i);
    if ( strcmp(s, "+set") )
      continue;
    CBUF_addtext(va("set %s %s\n", COM_argv(i+1), COM_argv(i+2)));
    if ( clear )
      {
	COM_clearargv(i);
	COM_clearargv(i+1);
	COM_clearargv(i+2);
      }
    i += 2;
  }
}

/*
Adds command line parameters as script statements
Commands lead with a +  and continue until another + or -

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
*/
t_bool CBUF_addlatecommands ( void )
{
  int i, j;
  int s;
  char *text, *build, c;
  int argc;
  t_bool  ret;
  
  /* build the combined string to parse from */
  s = 0;
  argc = COM_argc();
  for ( i = 1; i < argc; i++ )
    {
      s += strlen (COM_argv(i))+1;
    }
  if ( !s )
    return FALSE;
  
  text = (char *) ALLOC(s+1);
  text[0] = 0;
  for ( i = 1; i < argc; i++ )
    {
      strcat ( text, COM_argv(i) );
      if ( i != argc - 1)
	strcat( text, " " );
    }
  
  /* pull out the commands */
  build = (char *) ALLOC(s+1);
  build[0] = 0;
  
  for ( i = 0; i < s-1; i++ )
    {
      if ( text[i] == '+' )
	{
	  i++;
	  for ( j = i; (text[j] != '+') && (text[j] != '-') && (text[j] != 0); j++ )
	    ;
	  c = text[j];
	  text[j] = 0;
	  
	  strcat (build, text+i);
	  strcat (build, "\n");
	  text[j] = c;
	  i = j - 1;
	}
    }
  
  ret = (build[0] != 0);
  if ( ret )
    CBUF_addtext(build);
  FREE ( text );
  FREE ( build );
  
  return ret;
}

/* ====================================================================== */
/*                          SCRIPT COMMANDS                               */
/* ====================================================================== */

void CMD_exec_f(void)
{
  char   *f, *f2;
  int    len;
  
  if ( CMD_argc() != 2 )
    {
      printf("exec <filename>: execute a script file\n");
      return;
    }
  
  len = FS_loadfile (CMD_argv(1), (void **)&f);
  if (!f)
    {
      printf("couldn't exec %s\n", CMD_argv(1));
      return;
    }
  printf("execing %s\n", CMD_argv(1));
  
  /* the file doesn't have trailing 0, so we need to copy it off */
  f2 = (char *) ALLOC(len+1);
  memcpy(f2, f, len);
  f2[len] = 0;
  
  CBUF_inserttext(f2);
  FREE(f2);
  FS_freefile(f);
}

/*
Just prints the rest of the line to the console
*/
void CMD_echo_f(void)
{
  int i;
  
  for ( i = 1; i < CMD_argc(); i++ )
    printf("%s ", CMD_argv(i));
  printf("\n");
}


void CMD_alias_f ( void )
{
  t_cmdalias *a;
  char       cmd[1024];
  int        i, c;
  char       *s;
  
  if ( CMD_argc() == 1 )
    {
      printf( "Current alias commands: \n" );
      for ( a = cmd_alias; a; a = a->next )
	printf( "%s : %s\n", a->name, a->value );
      return;
    }
  
  s = CMD_argv(1);
  if ( strlen(s) >= MAX_ALIAS_NAME )
    {
      printf ( "Alias name is too long\n" );
      return;
    }

  /* if the alias already exists, reuse it */
  for ( a = cmd_alias; a ; a = a->next )
    {
      if ( !strcmp( s, a->name ) )
	{
	  FREE(a->value);
	  break;
	}
    }
  
  if (!a)
    {
      a = (t_cmdalias *) ALLOC(sizeof(t_cmdalias));
      a->next = cmd_alias;
      cmd_alias = a;
    }
  strcpy( a->name, s );
  
  /* copy the rest of the command line */
  cmd[0] = 0;
  c = CMD_argc();
  for ( i = 2; i < c; i++ )
    {
      strcat (cmd, CMD_argv(i));
      if ( i != (c - 1) )
	strcat( cmd, " " );
    }
  strcat( cmd, "\n" );
  
  a->value = copystring(cmd);
}

/* ========================================================================= */
/*                       COMMAND EXECUTION                                   */
/* ========================================================================= */

typedef struct cmd_function_s
{
  struct cmd_function_s   *next;
  char                    *name;
  xcommand_t              function;
}t_cmd_function;

static int     cmd_argc;
static char    *cmd_argv[MAX_STRING_TOKENS];
static char    *cmd_null_string =  " ";
static char    cmd_args[MAX_STRING_CHARS];

static t_cmd_function   *cmd_functions;   /* possible commands to execute */

int CMD_argc(void)
{
  return cmd_argc;
}

char *CMD_argv( int arg )
{
  if ( (unsigned) arg >= cmd_argc )
    return cmd_null_string;
  return cmd_argv[arg];
}


/*
 Returns a single string containing argv(1) to argv(argc()-1)
*/

char  *CMD_args(void)
{
  return cmd_args;
}

char *CMD_macroexpandstring ( char *text )
{
	int i, j, count, len;
	t_bool  inquote;
	char  *scan;
	static char  expanded[MAX_STRING_CHARS];
	char   temporary[MAX_STRING_CHARS];
	char   *token, *start;

	inquote = FALSE;
	scan = text;

	len = strlen(scan);
	if ( len >= MAX_STRING_CHARS )
	{
		printf("Line exceeded %i chars, discarded,\n", MAX_STRING_CHARS);
		return NULL;
	}

	count = 0;

	for ( i = 0; i < len; i++ )
	{
		if ( scan[i] == '"' )
			inquote ^= 1;
		if ( inquote )
			continue;
		if ( scan[i] != '$' )
			continue;
		/* scan out the complete macro */
		start = scan + i + 1;
		token = COM_parse(&start);
		if ( !start )
			continue;

		token = CVAR_variablestring(token);

		j = strlen(token);
		len += j;
		if ( len >= MAX_STRING_CHARS )
		{
			printf( "Expanded line exceeded %i chars, discarded.\n", MAX_STRING_CHARS );
			return NULL;
		}
		strncpy ( temporary, scan, i );
		strcpy(temporary+i, token);
		strcpy(temporary+i+j, start);

		strcpy(expanded, temporary);
		scan = expanded;
		i--;

		if (++count == 100)
		{
			printf("Macro expansion loop, discarded.\n");
			return NULL;
		}

	}

	if ( inquote )
	{
		printf( "Line has unmathched quote, discarded.\n" );
		return NULL;
	}

	return scan;
}

void CMD_tokenizestring(char *text, t_bool macro_expand )
{
	int i;
	char *com_token;

	/* clear the args from the last string */
	for ( i = 0; i < cmd_argc; i++ )
		FREE( cmd_argv[i] );

	cmd_argc = 0;
	cmd_args[0] = 0;

	/* macro expand the text */
	if ( macro_expand )
		text = CMD_macroexpandstring( text );
	if ( !text )
		return;

	while ( 1 )
	{
		/* skip whitespace up to a \n */
		while ( *text && *text <= ' ' && *text != '\n' )
		{
			text++;
			break;
		}

		if ( *text == '\n' )
			return;

		/* set cmd_args to everything after the first arg */
		if ( cmd_argc ==  1 )
		{
			int  l;
			strcpy ( cmd_args, text );

			/* strip off any trailing whitespace */
			l = strlen(cmd_args) - 1;
			for ( ; l >= 0; l-- )
				if ( cmd_args[l] <= ' ' )
					cmd_args[l] = 0;
				else
					break;
		}

		com_token = COM_parse( &text );
		if ( !text )
			return;

		if ( cmd_argc < MAX_STRING_TOKENS )
		{
			cmd_argv[cmd_argc] = (char *) ALLOC(strlen(com_token)+1);
			strcpy (cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}
}


void CMD_addcommand( char *cmd_name, xcommand_t function )
{
	t_cmd_function  *cmd;

	/* fail if the command is a variable name */
	if ( CVAR_variablestring(cmd_name)[0] )
	{
		printf("CMD_addcommand: %s already defined as a var \n", cmd_name);
		return;
	}

	/* fail if the command already exists */
	for ( cmd = cmd_functions; cmd ; cmd = cmd->next )
	{
		if ( !strcmp(cmd_name, cmd->name) )
		{
			printf( "CMD_addcommand: %s already defined\n", cmd_name );
			return;
		}
	}

	cmd = (t_cmd_function *) ALLOC( sizeof( t_cmd_function ) );
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next  = cmd_functions;
	cmd_functions = cmd;
}

void CMD_removecommand( char *cmd_name )
{
	t_cmd_function  *cmd, **back;

	back = &cmd_functions;
	while ( 1 )
	{
		cmd = *back;
		if ( !cmd )
		{
			printf( "CMD_removecommand: %s not added\n", cmd_name );
			return;
		}
		if ( !strcmp (cmd_name, cmd->name ) )
		{
			*back = cmd->next;
			FREE( cmd );
			return;
		}
		back = &cmd->next;
	}
}

t_bool CMD_exists( char *cmd_name )
{
	t_cmd_function *cmd;

	for ( cmd = cmd_functions; cmd; cmd = cmd->next )
	{
		if ( !strcmp( cmd_name, cmd->name ) )
			return TRUE;
	}
	return FALSE;
}


char *CMD_completecommand ( char *partial )
{
  t_cmd_function  *cmd;
  int             len;
  t_cmdalias     *a;
  
  len = strlen(partial);
  
  if ( !len )
    return NULL;
  
  /* check for exact match */
  for ( cmd = cmd_functions; cmd; cmd = cmd->next )
    {
      if ( !strcmp(partial, cmd->name) )
	return cmd->name;
    }
  
  for ( a = cmd_alias; cmd ; cmd = cmd->next )
    {
      if ( !strcmp( partial, a->name ) )
	return a->name;
    }
  
  /* check for partial match */
  for ( cmd = cmd_functions; cmd; cmd = cmd->next )
    {
      if ( !strncmp(partial, cmd->name, len) )
	return cmd->name;
    }
  
  for ( a = cmd_alias; a; a = a->next )
    {
      if ( !strncmp( partial, a->name, len ) )
	return a->name;
    }
  
  return NULL;
}

/*
A complete command line has been parsed, so try to execute it
*/

void CMD_executestring ( char *text )
{
  t_cmd_function   *cmd;
  t_cmdalias      *a;

  CMD_tokenizestring(text, TRUE);

  /* execute the command line */
  if ( !CMD_argc() )
    return;    /* no tokens */
  
  /* check functions */
  for ( cmd = cmd_functions; cmd ; cmd = cmd->next )
    {
      if ( !strcasecmp( cmd_argv[0], cmd->name ) )
	{
	  if ( !cmd->function )
	    {
	      /* forward to server command */
	      CMD_executestring (va("cmd %s", text));
	    }
	  else
	    {
	      cmd->function();
	    }
	  return;
	}
    }
  
  /* check alias */
  for ( a = cmd_alias; a ; a = a->next )
    {
      if ( strcasecmp( cmd_argv[0], a->name ) )

	{
	  if ( ++alias_count == ALIAS_LOOP_COUNT )
	    {
	      printf( "ALIAS_LOOP_COUNT\n" );
	      return;
	    }
	  CBUF_inserttext( a->value );
	  return;
	}
    }
  
  /* check cvars */
  if (CVAR_command())
    return;
  
  /* send it as a server command if we are connected */
  //CMD_forwardtoserver();
}

void CMD_list_f( void )
{
  t_cmd_function  *cmd;
  int  i;
  
  i = 0;
  for ( cmd = cmd_functions; cmd ; cmd = cmd->next, i++ )
    printf( "%s\n", cmd->name );
  printf(" %i commands\n", i);
}

void CMD_init(void)
{
  /* register our commands */
  CMD_addcommand( "cmdlist", CMD_list_f );
  CMD_addcommand( "exec", CMD_exec_f );
  CMD_addcommand( "echo", CMD_echo_f );
  CMD_addcommand( "alias", CMD_alias_f );
  CMD_addcommand( "wait", CMD_wait_f );
}

  

  
  
      
  
