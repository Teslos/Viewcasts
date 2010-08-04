/* cvar.c dynamic variable tracking */
#include "common.h"
#include "memory.h"

t_cvar *cvar_vars;

static t_bool CVAR_infovalidate(char *s)
{
  if ( strstr (s, "\\") )
    return FALSE;
  if ( strstr (s, "\"") )
    return FALSE;
  if ( strstr (s, ";") )
    return FALSE;
  return TRUE;
}


static t_cvar *CVAR_findvar(char *var_name)
{
  t_cvar  *var;
  for ( var = cvar_vars; var ; var = var->next )
    {
      if ( !strcmp(var_name, var->name) )
	  return var;
    }

  return NULL;
}

float CVAR_variablevalue( char *var_name )
{
  t_cvar *var;
  
  var = CVAR_findvar(var_name);
  if ( !var )
    return 0;
  
  return atof( var->string );
}

char *CVAR_variablestring( char *var_name )
{
  t_cvar *var;
  var = CVAR_findvar(var_name);
  if (!var)
    return "";
  return var->string;
}

char *CVAR_completevariable( char *partial )
{
  t_cvar   *cvar;
  int      len;
  
  if ( !len )
    return NULL;
  
  /* check exact match */
  for ( cvar = cvar_vars; cvar; cvar = cvar->next )
    if ( !strncmp(partial, cvar->name, len))
      return cvar->name;
  
  return NULL;
}


t_cvar *CVAR_get ( char *var_name, char *var_value, int flags)
{
  t_cvar *var;
  
  if ( flags & (CVAR_USERINFO | CVAR_SERVERINFO) )
    if (!CVAR_infovalidate( var_name ))
      {
	printf("invalid info cvar name\n");
	return NULL;
      }
  var = CVAR_findvar( var_name );

  if ( var )
    {
      var->flags |= flags;
      return var;
    }
  
  if (!var_value)
    return NULL;
  
  if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
    {
      if ( !CVAR_infovalidate(var_value) )
	{
	  printf("invalidate info cvar value\n");
	  return NULL;
	}
    }
  
  var = (t_cvar*) ALLOC(sizeof(*var));
  var->name = copystring(var_name);
  var->string = copystring(var_value);
  var->modified = TRUE;
  var->value    = atof(var->string);
  
  /* link the variables in */
  var->next = cvar_vars;
  cvar_vars = var;
  
  var->flags = flags;
  
  return var;
}


t_cvar *CVAR_set2(char *var_name, char *value, t_bool force)
{
  t_cvar *var;
  var = CVAR_findvar(var_name);
  if (!var)
    {
      /* create it */
      return CVAR_get(var_name, value, 0);
    }
  
  if ( var->flags & ( CVAR_USERINFO | CVAR_SERVERINFO) )
    if ( !CVAR_infovalidate(value) )
      {
	printf( "invalid info cvar value\n" );
	return var;
      }
  
  if ( !force )
    {
      if ( var->flags & CVAR_NOSET )
	{
	  printf ("%s is write protected.\n", var_name );
	  return var;
	}
      
      if ( var->flags & CVAR_LATCH )
	{
	  if ( var->latched_string )
	    {
	      if ( strcmp(value, var->latched_string) == 0 )
		return var;
	      FREE( var->latched_string );
	    }
	  else
	    {
	      if ( strcmp(value, var->string) == 0 )
		return var;
	    }
	  
	  if (COM_serverstate())
	    {
	      printf("%s will be changed for next session.\n", var_name);
	      var->latched_string = copystring(value);
	    }
	  else
	    {
	      var->string = copystring(value);
	      var->value  = atof(var->string);
	      if ( !strcmp(var->name, "session"))
		; /* make nothing */
	    }
	  return var;
	}
    }
  else
    {
      if (var->latched_string)
	{
	  FREE(var->latched_string);
	  var->latched_string = NULL;
	}
    }

  if( !strcmp(value, var->string) )
    return var;  /* not changed */
  
  var->modified = TRUE;
  
  if ( var->flags & CVAR_USERINFO )
    userinfo_modified = TRUE;
  
  FREE(var->string);
  
  var->string = copystring(value);
  var->value = atof(var->string);
  
  return var;
}

t_cvar *CVAR_forceset(char *var_name, char *value)
{
  return CVAR_set2(var_name, value, TRUE);
}

t_cvar *CVAR_set(char *var_name, char *value)
{
  return CVAR_set2(var_name, value, FALSE);
}

t_cvar *CVAR_fullset( char *var_name, char *value, int flags )
{
  t_cvar *var;
  
  var = CVAR_findvar(var_name);
  if ( !var )
    {
      return CVAR_get(var_name, value, flags);
    }
  
  var->modified = TRUE;
  
  if ( var->flags & CVAR_USERINFO )
    userinfo_modified = TRUE;
  
  FREE( var->string );
  
  var->string = copystring(value);
  var->value  = atof(var->string);
  var->flags  = flags;
  
  return var;
}

void CVAR_setvalue ( char *var_name, float value )
{
  char val[32];
  
  if ( value == (int) value )
    sprintf(val, "%i", (int)value);
  else
    sprintf(val, "%f", value);
  CVAR_set(var_name, val);
}

void CVAR_getlatchedvars ( void )
{
  t_cvar *var;
  
  for ( var = cvar_vars; var ; var = var->next )
    {
      if (!var->latched_string)
	continue;
      FREE ( var->string );
      var->string = var->latched_string;
      var->latched_string = NULL;
      var->value = atof(var->string);
      if (!strcmp(var->name, "session"))
	{
	  ;
	}
    }
}


t_bool CVAR_command(void)
{
  t_cvar   *v;
  v = CVAR_findvar(CMD_argv(0));
  if (!v)
    return FALSE;
  
  if (CMD_argc() == 1)
    {
      printf("\"%s\" is \"%s\"\"\n", v->name, v->string);
      return TRUE;
    }
  
  CVAR_set (v->name, CMD_argv(1));
  return TRUE;
}


void CVAR_set_f(void)
{
  int c;
  int flags;
  
  c = CMD_argc();
  if (c != 3 && c != 4)
    {
      printf("usage: set <variable> <value> \n");
      return;
    }
  
  if (c==4)
    {
      if (!strcmp(CMD_argv(3), "u"))
	flags = CVAR_USERINFO;
      else if(!strcmp(CMD_argv(3), "s"))
	flags = CVAR_SERVERINFO;
      else
	{
	  printf("flags can only be 'u' or 's'\n");
	  return;
	}
      CVAR_fullset(CMD_argv(1), CMD_argv(2), flags);
    }
  else
    CVAR_set(CMD_argv(1), CMD_argv(2));
}

void CVAR_writevariables(char *path)
{
  t_cvar *var;
  char buffer[1024];
  FILE *f;
  
  f = fopen(path, "a");
  for ( var = cvar_vars ; var ; var = var->next )
    {
      if (var->flags & CVAR_ARCHIVE)
	{
	  sprintf( buffer, "set %s \"%s\"\n", var->name, var->string);
	  fprintf( f, "%s", buffer );
	}
    }
  fclose(f);
}

void CVAR_list_f(void)
{
  t_cvar *var;
  int i;
  
  i = 0;
  for ( var = cvar_vars; var ; var = var->next, i++ )
    {
      if ( var->flags & CVAR_ARCHIVE )
	printf("*");
      else
	printf(" ");
      if ( var->flags & CVAR_USERINFO )
	printf("U");
      else
	printf(" ");
      if ( var->flags & CVAR_SERVERINFO )
	printf("S");
      else
	printf(" ");
      if ( var->flags & CVAR_NOSET )
	printf("-");
      else if ( var->flags & CVAR_LATCH )
	printf("L");
      else
	printf(" ");
      printf(" %s \"%s\"\n", var->name, var->string);
    }
  printf("%i cvars\n", i);
}

t_bool userinfo_modified;
char *INFO_valueforkey (char *s, char *key)
{
  char	pkey[512];
  static	char value[2][512];	// use two buffers so compares
    // work without stomping on each other
      static	int	valueindex;
  char	*o;
  
  valueindex ^= 1;
  if (*s == '\\')
    s++;
  while (1)
    {
      o = pkey;
      while (*s != '\\')
	{
	  if (!*s)
	    return "";
	  *o++ = *s++;
	}
      *o = 0;
      s++;

      o = value[valueindex];

      while (*s != '\\' && *s)
	{
	  if (!*s)
	    return "";
	  *o++ = *s++;
	}
      *o = 0;

      if (!strcmp (key, pkey) )
	return value[valueindex];

      if (!*s)
	return "";
      s++;
    }
}

void INFO_removekey (char *s, char *key)
{
  char	*start;
  char	pkey[512];
  char	value[512];
  char	*o;

  if (strstr (key, "\\"))
    {
      //		Com_Printf ("Can't use a key with a \\\n");
      return;
    }

  while (1)
    {
      start = s;
      if (*s == '\\')
	s++;
      o = pkey;
      while (*s != '\\')
	{
	  if (!*s)
	    return;
	  *o++ = *s++;
	}
      *o = 0;
      s++;

      o = value;
      while (*s != '\\' && *s)
	{
	  if (!*s)
	    return;
	  *o++ = *s++;
	}
      *o = 0;

      if (!strcmp (key, pkey) )
	{
	  strcpy (start, s);	// remove this part
	    return;
	}

      if (!*s)
	return;
    }

}

void INFO_setvalueforkey (char *s, char *key, char *value)
{
  char	newi[MAX_INFO_STRING], *v;
  int		c;
  int		maxsize = MAX_INFO_STRING;
  
  if (strstr (key, "\\") || strstr (value, "\\") )
    {
      printf ("Can't use keys or values with a \\\n");
      return;
    }
  
  if (strstr (key, ";") )
    {
      printf ("Can't use keys or values with a semicolon\n");
      return;
    }
  
  if (strstr (key, "\"") || strstr (value, "\"") )
    {
      printf ("Can't use keys or values with a \"\n");
      return;
    }
  
  if (strlen(key) > MAX_INFO_KEY-1 || strlen(value) > MAX_INFO_KEY-1)
    {
      printf ("Keys and values must be < 64 characters.\n");
      return;
    }

  INFO_removekey (s, key);
  if (!value || !strlen(value))
    return;
  
  sprintf (newi, "\\%s\\%s", key, value);
  
  if (strlen(newi) + strlen(s) > maxsize)
    {
      printf ("Info string length exceeded\n");
      return;
    }
  
  // only copy ascii values
    s += strlen(s);
  v = newi;
  while (*v)
    {
      c = *v++;
      c &= 127;		// strip high bits
	if (c >= 32 && c < 127)
	  *s++ = c;
    }
  *s = 0;
}

char *CVAR_bitinfo (int bit)
{
  static char info[MAX_INFO_STRING];
  t_cvar   *var;
  
  info[0] = 0;
  for (var = cvar_vars; var ; var = var->next ) 
    {
      if ( var->flags & bit )
	INFO_setvalueforkey ( info, var->name, var->string);
    }
  return info;
}

char * CVAR_userinfo(void)
{
  return CVAR_bitinfo(CVAR_USERINFO);
}

char *CVAR_serverinfo(void)
{
  return CVAR_bitinfo(CVAR_SERVERINFO);
}

void CVAR_init(void)
{
  CMD_addcommand("set", CVAR_set_f);
  CMD_addcommand("cvarlist", CVAR_list_f);
}


