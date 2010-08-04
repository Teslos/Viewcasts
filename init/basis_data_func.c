/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :    7.01.97                                         */
/* Dateiname:   basisfunktionen.c                                */
/*                                                               */
/* Zweck : Datei enthaelt Basisfunktionen fuer die Verwaltung    */
/*         der Datenstrukturen und der benoetigten Dateien       */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/* add_zeit     | fuegt weiteren Zeitschritt an Zeitenliste an   */
/* add_stoffgruppe | hinzufuegen einer neuen Stoffgruppe         */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include "color3d.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/*****************************************************************/

/* ********************************************************************* */
int add_zeit(float **liste, float *neu, int *n)
/* ********************************************************************* */
/* **liste : Datenliste, die erweitert erweitert werden soll -------- */
/* *neu    : neues Element ------------------------------------------ */
/* *n      : Anzahl Elemente */
{

/* Allozieren des Speichers fuer ein neues Listenelement ------------ */
  if ((*liste=(float *)realloc(*liste, sizeof(float)*(*n))) == NULL)
  return(-1);	            /* Fehler: nicht genuegend Speicher ----- */

  memcpy((char *)((*liste)+(*n)), (char *)neu, sizeof(float));
  *n = *n + 1;
  return(0);
}

/* ********************************************************************* */
int stoffgruppe_add(t_stoffgruppe **liste, int *stoffgeb_nr,
                    int n_stoffgeb, int *n)
/* ********************************************************************* */
/* **liste     : datenliste, die erweitert erweitert werden soll ---- */
/* stoffgeb_nr : Liste der Stoffgebietsnummern ---------------------- */
/* n_stoffgeb  : Anzahl Stoffgebiete -------------------------------- */
/* *n          : Anzahl Elemente ------------------------------------ */
{
  int i;
  int nummer = 1;
  /*int geb_index;*/
  t_stoffgruppe *neu;
  
  /* Allozieren des Speichers fuer ein neues Listenelement ------------ */
  if ((neu = (t_stoffgruppe *)malloc(sizeof(t_stoffgruppe))) == NULL)
    return(-1);		  /* Fehler: nicht genuegend Speicher --------- */

  neu->nr = nummer;
  neu->anzahl = n_stoffgeb;
  neu->stoffe = (int *)NULL;

  if (n_stoffgeb > 0)
  {
    if ((neu->stoffe = (int *)malloc(sizeof(int) * n_stoffgeb)) == NULL)
      return(-1);		/* Fehler: nicht genuegend Speicher */

    for (i = 0; i < n_stoffgeb; i++) /* Schleife ueber alle selektierten Stoffgebiete */
    {
/*
      if ((geb_index = get_stoffgebiet_by_nr(g_stoffgebiete, g_geodat.nstoff, stoffgeb_nr[i])) >= 0)
      {
        if (!g_stoffgebiete[geb_index].gruppennr) 
        {
          neu->stoffe[i] = g_stoffgebiete[geb_index].nr;
          g_stoffgebiete[geb_index].gruppennr = neu;
        }
        else
        {
          neu->anzahl--;
        }
      }
      else
      {
        fprintf(stderr, "get_stoffgebiet_by_nr(g_stoffgebiete, >%d<, >%d<) return >%d<\n",
        g_geodat.nstoff, stoffgeb_nr[i], geb_index);
        return(-2);
      }
*/
    }
  }
  else
  {
    neu->stoffe = (int *)NULL;
  }

  return(0);
}

