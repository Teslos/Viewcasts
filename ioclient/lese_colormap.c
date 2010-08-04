/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   30.01.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname       | Funktion                                 */
/* ------------------------------------------------------------- */
/* lese_colormapdatei | liest eine Colormapbelegung aus Datei    */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "color3d.h"
#include "strukturen.h"
#include "motif.h"
/*                                                               */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*****************************************************************/

/*****************************************************************/
int lese_colormapdatei(char* dateiname)
/*****************************************************************/
/*  RETURNS: int 0   - kein Fehler                                   */
/*              -1   - Fehler beim oeffnen der Datei                 */
/*              -2   - Fehler beim allozieren von Speicherplatz      */
/*              -3   - <file>  ist keine regulaere Datei             */
/*              -4   - <file>  ist leer oder nicht lesbar            */
{
  struct stat stbuf;  /* Struktur zur Ueberpruefung des Namens - */
  int  reg_file;      /* Variable z.Ueberpruefung d. Dateistatus */
  int  anzahl_felder; /* Anzahl Felder in Loesungsdatei -------- */
  FILE *fd;           /* Filepointer --------------------------- */
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf (stdout, "Colormapdatei = >%s<\n", dateiname);
#endif
  if (!file_read(dateiname)) return (-3);

/* Datei oeffnen ----------------------------------------------- */
  if ((fd = fopen(dateiname,"r")) == NULL) return -1;
  
  return 0;
/*****************************************************************/
}








