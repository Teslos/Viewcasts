
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h> /* O_RDONLY */
#include <sys/stat.h>	/* fuer file-status */
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h> /* close und read */
#include <Xm/Xm.h>
#include <Xm/ToggleB.h>

#include "errs.h"
#include "color3d.h"

/* ******************************************************************* */
/* Funktionen:                                                         */
/*      file_exist               : prueft ob Datei existiert --------- */
/*      file_read                : prueft ob Datei ex. u. lesbar ----- */
/*      file_write               : prueft ob Datei ex. u. beschreibbar */
/*      file_2_string            :     */
/*      my_XtNameToWidget        :     */
/*      my_XmToggleButtonSetState: erzwingt ValueChanged-Callback ---- */
/*      sensitive_and_on         : prueft: TB sensitiv u.eingeschaltet */
/* ******************************************************************* */

/* ******************************************************************* */
int file_exist(char *datei)
/* ******************************************************************* */
{
  static struct stat datei_status;

  int stat_result     = stat(datei, &datei_status);
  int is_regular_file = ((datei_status.st_mode & S_IFREG) == S_IFREG);
  int access_result   = !access(datei, F_OK);
#ifdef DEBUG
  fprintf(stderr, "datei = >%s<\n", datei);
  fprintf(stderr, "stat(datei, &datei_status) = >%d<\n", stat_result);
  fprintf(stderr, "((datei_status.st_mode & S_IFREG) == S_IFREG) = >%d<\n",
                     is_regular_file);
  fprintf(stderr, "(!access(datei, F_OK))    = >%d<\n", access_result);
#endif

/* die Datei ist ein regulaeres file */
  if (!stat_result && is_regular_file && access_result)
    return(1);
  else
    return(0);
/***********************************************************************/
}

int file_info(char *datei, t_loesung *loesung)
{
	char *p;
	char *sline = NULL;
	char *extension = NULL;
	char *name_data = NULL;
	char *sepp;
	char sepc;
	int i;

	sline = (char*) malloc((strlen(datei)+1)*sizeof(char));

	strcpy(sline, datei);
	p = sline;
	/* get file's extension */
	sepp = p + strcspn(sline,".");
	sepc = sepp[0];
	sepp[0] = '\0';
	name_data = p;
	p = sepp + 1;
	extension = p;

	/* for all known exten. */
	for (i = 0; i < anzloesungen-1; i++)
	{
		if(strcmp(extension, loesungen[i].dateiende) == NULL)
		{
			strcpy(g_loesdef[0].dateiende, loesungen[i].dateiende);
			strcpy(g_loesdef[0].text, loesungen[i].text);
			strcpy(g_loesdef[0].parameter,loesungen[i].parameter);
			strcpy(g_loesdef[0].einheit, loesungen[i].einheit);
			strcpy(loesung->definition.dateiende, loesungen[i].dateiende);
			strcpy(loesung->definition.text, loesungen[i].text);
			strcpy(loesung->definition.parameter,loesungen[i].parameter);
			strcpy(loesung->definition.einheit,loesungen[i].einheit);

			loesung->Min = loesungen[i].Min;
			loesung->Max = loesungen[i].Max;
		}
	}
	free(sline);
	return(0);
}

/* ******************************************************************* */
int file_read(char *datei)
/* ******************************************************************* */
{
  static struct stat datei_status;

  int stat_result     = stat(datei, &datei_status);
  int is_regular_file = ((datei_status.st_mode & S_IFREG) == S_IFREG);
  int access_result   = !access(datei, R_OK);
#ifdef DEBUG
  fprintf(stderr, "datei = >%s<\n", datei);
  fprintf(stderr, "stat(datei, &datei_status)  = >%d<\n", stat_result);
  fprintf(stderr, "((datei_status.st_mode & S_IFREG) == S_IFREG) = >%d<\n",
                     is_regular_file);
  fprintf(stderr, "(!access(datei, R_OK))      = >%d<\n", access_result);
#endif

/* die Datei ist ein regulaeres file und ist lesbar */
  if (!stat_result && is_regular_file && access_result)
    return(1);
  else
    return(0);
/***********************************************************************/
}

/* ******************************************************************* */
int file_write(char *datei)
/* ******************************************************************* */
{
  static struct stat datei_status;

  int stat_result     = stat(datei, &datei_status);
  int is_regular_file = ((datei_status.st_mode & S_IFREG) == S_IFREG);
  int access_result   = !access(datei, W_OK);

/* die Datei ist ein regulaeres file und ist beschreibbar */
  if (!stat_result && is_regular_file && access_result)
    return(1);
  else
    return(0);
/***********************************************************************/
}

/***********************************************************************
 Opens file for reading.

 Parameters:
 ===========
         -char *dateiname - name of the file.
         

 Returns:
 ========
         -file descriptor
***********************************************************************/
int rawopen(const char *dateiname)
{
  int fd;

  /* open file */
  if (fd = open(dateiname, O_RDONLY) < 0)
    return -1;
  return fd;
}

/***********************************************************************
 This function writes n bytes in memory mapped file.

 Parameters:
 ===========
         int fd - file descriptor.
         const void *vptr - pointer to memory.
         size_t n - size of memory to write.

 Returns:
 ========
         n - number of bytes read.
***********************************************************************/
ssize_t writen (int fd, const void *vptr, size_t n)
{
  size_t   nleft;
  ssize_t  nwritten;
  const char *ptr;
  
  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
    if ( (nwritten = write(fd, ptr, nleft)) <= 0)
      //error(Ewriten);
	 
    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}

/***********************************************************************
 This function reads n bytes in memory mapped file.

 Parameters:
 ===========
         int fd - file descriptor.
         const void *vptr - pointer to memory.
         size_t n - size of memory to write.

 Returns:
 ========
         n - number of bytes read.
***********************************************************************/  
ssize_t readn (int fd, void *vptr, size_t n)
{
  size_t  nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) 
    {
      if ( (nread = read(fd, ptr, nleft)) < 0)
	{
	  //error(Ereadn);
	  return nread;
	}
      else if (nread == 0)
	break;
      
      nleft -= nread;
      ptr   += nread;
    }
  return (n - nleft);
}
	
/***********************************************************************
 This function gets soft and hard limits of the current machine.

 Parameters:
 ===========
         -char *name  - name of the resource.
         -int resource - index of resource.
         -struct rlimit *limit - pointer to struct limit.

 Returns:
 ========
          0 - sucsses.
	  1 - error in getting resources.
***********************************************************************/	  
int pr_limits(char *name, int resource, struct rlimit *limit)
{
   
  if (getrlimit(resource, limit) < 0)
    {
      //error(Erlimit);
      return (-1);
    }

  return 0;
}
    
/* ******************************************************************* */
int file_2_string(char *datei, char **string)
/* ******************************************************************* */
{
  int i;
  int fd, len, nread;
  char *text;
  static struct stat datei_status;

  if (!file_read(datei))
    return(1);

  if (!(fd = open(datei, O_RDONLY)))
    return(2);

  if (fstat(fd, &datei_status) == -1)
  {
    close(fd);
    return(3);
  }

  if (!(text = XtMalloc((len = datei_status.st_size) + 1)))
  {
    close(fd);
    return(4);
  }

/* lese len Bytes */
  nread = read(fd, text, len);

/* NULL-Character durch Blanks ersetzen */
  for (i = 0; i < len; i++)
    if (text[i] == '\0')
      text[i] = ' ';

/* String NULL-terminieren */
  text[len] = '\0';

  *string = text;
  return(0);
/***********************************************************************/
}

/* ******************************************************************* */
Widget my_XtNameToWidget(Widget widget, char *widget_string, int stderr_out)
/* ******************************************************************* */
{
  static Widget w;

  if (!(w = XtNameToWidget(widget, widget_string)))
  {
    if (stderr_out)
      fprintf(stderr, "XtNameToWidget(..., \"%s\") returns NULL\n", widget_string);
    return((Widget)NULL);
  }
#ifdef DEBUG
  fprintf(stderr, "XtNameToWidget(..., \"%s\") :\n", widget_string);
#endif
  return(w);
}
/***********************************************************************/

/* ******************************************************************* */
void my_XmToggleButtonSetState(Widget widget, int state, int notify)
/* ******************************************************************* */
{
  if (XmToggleButtonGetState(widget) == state)
  {
    XmToggleButtonSetState(widget, !state, FALSE);
    XmToggleButtonSetState(widget, state, TRUE);
  }
  else
    XmToggleButtonSetState(widget, state, TRUE);
  return;
/***********************************************************************/
}

/* ******************************************************************* */
int sensitive_and_on(Widget widget)
/* ******************************************************************* */
{
  return(XtIsSensitive(widget) && XmToggleButtonGetState(widget));
/***********************************************************************/
}
