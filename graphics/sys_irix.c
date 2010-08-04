#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <mntent.h>
#include <dirent.h>

#include <dlfcn.h>
#include "../graphics/common.h"

t_cvar *nostdout;

unsigned sys_frame_time;

t_bool stdin_active = TRUE;


void SYS_consoleoutput( char  *string )
{
  if (nostdout && nostdout->value)
    return;
  fputs(string, stdout);
}

void SYS_printf(char *fmt, ...)
{
  va_list argptr;
  char		text[1024];
  unsigned char		*p;

  va_start (argptr,fmt);
  vsprintf (text,fmt,argptr);
  va_end (argptr);

  if (strlen(text) > sizeof(text))
    SYS_error("memory overwrite in SYS_printf");

  if (nostdout && nostdout->value)
    return;

  for (p = (unsigned char *)text; *p; p++) {
    *p &= 0x7f;
    if ((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
      printf("[%02x]", *p);
    else
      putc(*p, stdout);
  }
}

void SYS_error (char *error, ...)
{ 
  va_list     argptr;
  char        string[1024];

  // change stdin to non blocking
  fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);
    
  va_start (argptr,error);
  vsprintf (string,error,argptr);
  va_end (argptr);
  fprintf(stderr, "Error: %s\n", string);
    
  //CL_Shutdown ();
  //Qcommon_Shutdown ();
  _exit (1);

} 

void SYS_warn (char *warning, ...)
{ 
  va_list     argptr;
  char        string[1024];
    
  va_start (argptr,warning);
  vsprintf (string,warning,argptr);
  va_end (argptr);
  fprintf(stderr, "Warning: %s", string);
}
 
int SYS_filetime (char *path)
{
  struct	stat	buf;
  
  if (stat (path,&buf) == -1)
    return -1;
  
  return buf.st_mtime;
}

int curtime;
int SYS_milliseconds (void)
{
  struct timeval tp;
  struct timezone tzp;
  static int		secbase;
  
  gettimeofday(&tp, &tzp);
  
  if (!secbase)
    {
      secbase = tp.tv_sec;
      return tp.tv_usec/1000;
    }

  curtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;
	
  return curtime;
}


void SYS_mkdir (char *path)
{
  mkdir (path, 0777);
}

char *strlwr (char *s)
{
  char *origs = s;
  while (*s) {
    *s = tolower(*s);
    s++;
  }
  return origs;
}

static	char	findbase[MAX_OSPATH];
static	char	findpath[MAX_OSPATH];
static	char	findpattern[MAX_OSPATH];
static	DIR		*fdir;

static t_bool CompareAttributes(char *path, char *name,
	unsigned musthave, unsigned canthave )
{
	struct stat st;
	char fn[MAX_OSPATH];

	// . and .. never match
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
		return FALSE;

	sprintf(fn, "%s/%s", path, name);
	if (stat(fn, &st) == -1)
		return FALSE; // shouldn't happen

	if ( ( st.st_mode & S_IFDIR ) && ( canthave & SFF_SUBDIR ) )
		return FALSE;

	if ( ( musthave & SFF_SUBDIR ) && !( st.st_mode & S_IFDIR ) )
		return FALSE;

	return TRUE;
}

char *SYS_findfirst (char *path, unsigned musthave, unsigned canhave)
{
  struct dirent *d;
  char *p;

  if (fdir)
    SYS_error ("SYS_beginfind without close");

  //	COM_FilePath (path, findbase);
  strcpy(findbase, path);

  if ((p = strrchr(findbase, '/')) != NULL) {
    *p = 0;
    strcpy(findpattern, p + 1);
  } else
    strcpy(findpattern, "*");

  if (strcmp(findpattern, "*.*") == 0)
    strcpy(findpattern, "*");
	
  if ((fdir = opendir(findbase)) == NULL)
    return NULL;
  while ((d = readdir(fdir)) != NULL) {
    if (!*findpattern || glob_match(findpattern, d->d_name)) {
      //			if (*findpattern)
	//				printf("%s matched %s\n", findpattern, d->d_name);
      if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
	sprintf (findpath, "%s/%s", findbase, d->d_name);
	return findpath;
      }
    }
  }
  return NULL;
}

char *SYS_findnext (unsigned musthave, unsigned canhave)
{
	struct dirent *d;

	if (fdir == NULL)
		return NULL;
	while ((d = readdir(fdir)) != NULL) {
		if (!*findpattern || glob_match(findpattern, d->d_name)) {
//			if (*findpattern)
//				printf("%s matched %s\n", findpattern, d->d_name);
			if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
				sprintf (findpath, "%s/%s", findbase, d->d_name);
				return findpath;
			}
		}
	}
	return NULL;
}

void SYS_findclose (void)
{
	if (fdir != NULL)
		closedir(fdir);
	fdir = NULL;
}

char *SYS_basename( char *path )
{
  char *p;
  if ( path == NULL )
    return " ";
  strcpy( findbase, path );
  if ((p = strrchr(findbase, '/')) != NULL) {
    *p = 0;
    strcpy(findpattern, p + 1);
  } else
    strcpy(findpattern, "*");
  return findpattern;
}

char *SYS_extension( char *path )
{
  char *p;
  if ( path == NULL )
    return " ";
  strcpy( findbase, path );
  if ((p = strrchr(findbase, '.')) != NULL) {
    *p = 0;
    strcpy(findpattern, p + 1);
  } else
    strcpy(findpattern, "*");
  return findpattern;
}

void SYS_sendkeyevents( void )
{
//	if( KBD_update_fp )
//		KBD_update_fp();
	//grab frame time
	sys_frame_time = 0;
}

char *SYS_getclipboarddata(void)
{
	return NULL;
}
