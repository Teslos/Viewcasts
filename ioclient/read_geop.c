/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/* read_geop    : einlesen der geo-Datei                         */
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
#include "memory.h"
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* ********************************************************************* */
int read_geop(char *file)
/* ********************************************************************* */
/*  RETURNS: int 0   - kein Fehler                                   */
/*              -1   - Fehler beim oeffnen der Datei                 */
/*              -2   - Fehler beim allozieren von Speicherplatz      */
/*              -3   - <file>  ist keine regulaere Datei             */
/*              10   - Fehler beim anlegen der Knotenliste           */
/*              11   - Fehler beim anlegen der Elementliste          */
/*              12   - Fehler beim anlegen der Stoffgeb.-liste       */
{
  struct stat stbuf;      /* Struktur zur Ueberpruefung des Namens - */
  int  index,             /* Index fuer Ikenn-feld ----------------- */
       reg_file,          /* Variable z.Ueberpruefung d. Dateistatus */
       i, j, k, l,        /* Zaehler ------------------------------- */
       anzahl, 
       filelaenge,        /* Laenge des Dateinamens ---------------- */
       anzheipu,          /* Anzahl Heizerpunkte ------------------- */
       result,            /* Rueckgabewert der Leseroutinen -------- */
       anfang, ende;      /* Schleifenzaehler ---------------------- */ 
  int  anzahl_hex,        /* Hilfsvariablen zum einlesen der         */ 
       ende_hex,          /* Anzahl und Endadresse zu Elementtypen   */
       anzahl_pen,        /* fuer jedes Stoffgebiet ---------------- */
       ende_pen,
       anzahl_tet,
       ende_tet;
  float minx, maxx,       /* minimale u. maximale Koordinaten eines  */
        miny, maxy,       /* Stoffgebietes ------------------------- */
        minz, maxz;
  char c_dummy[1024],     /* Hilfsstring zum einlesen -------------- */
       c_name[1024],      /* Hilfsstring fuer Stoffgeb.Name -------- */
       c_nr[3];           /* Hilfsstring fuer Stoffgeb.Nr. --------- */
  FILE *fd, *fopen();     /* Filepointer --------------------------- */
  t_knoten knoten;        /* lokale Struktur fuer einen Knoten ----- */
  t_element element;      /* lokale Struktur fuer ein Element ------ */
  t_stoffgebiet stoffgebiet; /* lokale Struktur f. ein Stoffgebiet - */

/* ********************************************************************* */
/* Ueberpruefen der Datei "file" ----------------------------------- */
  if (!file_read(file))
  {
     strcpy(c_dummy, file);
     strcat(c_dummy, " is not a regular file");
     warning(c_dummy);
     return (-3);
  }
/* Datei oeffnen --------------------------------------------------- */
  if ((fd = fopen(file,"r")) == NULL) return -1;

/* lese Geodat-Struktur -------------------------------------------- */
    i = fscanf(fd, "%*35c%i\n", &g_geodat.npunkt);
    i = fscanf(fd, "%*34c%i\n", &anzheipu);
    i = fscanf(fd, "%*34c%i\n", &g_geodat.nvol);
    i = fscanf(fd, "%*34c%i\n", &g_geodat.ninter);
    i = fscanf(fd, "%*34c%i\n", &g_geodat.nstoff);
    i = fscanf(fd, "%*s%*s%*s\n");

/* ueberlesen der Texte -------------------------------------------- */
    i = fscanf(fd, "%*s\n");
    i = fscanf(fd, "%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s\n");
    i = fscanf(fd, "%*s\n");

/* allocieren und belegen der Stoffgebiet-Struktur ------------------ */
    if ((g_stoffgebiete = (t_stoffgebiet*) 
	 ALLOC(g_geodat.nstoff * sizeof(t_stoffgebiet)))==NULL)
    {
      g_geodat.npunkt = 0;
      g_geodat.nvol = 0;
      g_geodat.nstoff = 0;
      return -2;             /* Kein Platz ---------------------- */
    }

/* Fuelle Stoffgebiete-Struktur --------------------------------- */
    for (j = 0; j < g_geodat.nstoff; j++)
    { /* liest Anz. hexa-,penta-, tetraeeder von Stoff j     */
      i = fscanf(fd, "%*i%*i%i%i%*i%i%i%*i%i%i\n",
                 &anzahl_hex, &ende_hex, &anzahl_pen,
                 &ende_pen, &anzahl_tet, &ende_tet);
      if (j == 0)
        g_stoffgebiete[j].anfang_hex = 0;
      else 
        g_stoffgebiete[j].anfang_hex = g_stoffgebiete[j-1].ende_tet;

      g_stoffgebiete[j].ende_hex = ende_hex;
      g_stoffgebiete[j].anfang_pen = g_stoffgebiete[j].ende_hex;
      g_stoffgebiete[j].ende_pen = ende_pen;
      g_stoffgebiete[j].anfang_tet = g_stoffgebiete[j].ende_pen;
      g_stoffgebiete[j].ende_tet = ende_tet;
    
/* erzeuge Stoffgebietsnamen -------------------------------------- */
      sprintf(c_nr, "%d", j+1);
      if (g_sprache == SPRACHE_DEUTSCH)
      {
        strcpy(c_name, "Stoff_");
      }
      else
      {
        strcpy(c_name, "Material_");
      }
      strcat(c_name, c_nr);
      strcat(c_name, "\0");
      strcpy(g_stoffgebiete[j].name, c_name);
      g_stoffgebiete[j].nr = j + 1;
      g_stoffgebiete[j].gruppennr = 0;
      g_stoffgebiete[j].verschiebung[0] = 0.0;
      g_stoffgebiete[j].verschiebung[1] = 0.0;
      g_stoffgebiete[j].verschiebung[2] = 0.0;
    }

/* Test ob ityvol-Feld mit Anzahl-Elemente uebereinstimmt --------- */
   if (g_stoffgebiete[g_geodat.nstoff - 1].ende_tet != g_geodat.nvol)
   {
     warning("Number of elements and number in ITYVOL field not the same");
     g_geodat.nvol = g_stoffgebiete[g_geodat.nstoff -1].ende_tet;
   }

/* ueberlesen der Texte -------------------------------------------- */
    i = fscanf(fd, "%*s%*s%*s%*s%*s\n");

/* allocieren und belegen der Knoten-Struktur ---------------------- */
    if ((g_knoten = (t_knoten*) ALLOC(g_geodat.npunkt * sizeof(t_knoten)))==NULL)
    {
      g_geodat.npunkt = 0;
      g_geodat.nvol = 0;
      g_geodat.nstoff = 0;
      return -2;             /* Kein Platz ---------------------- */
    }

/* einlesen der Koordinaten ---------------------------------------- */
    for (j = 0; j < g_geodat.npunkt; j++)
    {
      i = fscanf(fd, "%f%f%f\n", &g_knoten[j].koord[0],
                   &g_knoten[j].koord[1], &g_knoten[j].koord[2]);
      g_knoten[j].knoten_nr = j+1;
    }

/* falls Heizerpunkte vorhanden, werden diese ueberlesen ----------- */
    if (anzheipu > 0)
    {
      for (j = 0; j < g_geodat.npunkt; j++)
        fscanf(fd, "%*f%*f%*f\n");
    }

/* ueberlesen der Texte -------------------------------------------- */
    i = fscanf(fd, "%*s%*s%*s%*s%*s\n");

/* allocieren und belegen der Element-Struktur ---------------------- */
    if ((g_element = (t_element*) ALLOC((g_geodat.nvol + g_geodat.ninter) * sizeof(t_element)))==NULL)
    {
      g_geodat.npunkt = 0;
      g_geodat.nvol = 0;
      g_geodat.nstoff = 0;
      return -2;             /* Kein Platz ---------------------- */
    }

/* Schleife ueber alle Stoffe ----------------------------------- */
    for (j=0; j<g_geodat.nstoff; j++)
    {               /* init Min u. Max fuer jedes Stoffgeb. ----- */
      maxx = maxy = maxz = -10000.0;
      minx = miny = minz = -maxx;
      anfang = g_stoffgebiete[j].anfang_hex;
      ende   = g_stoffgebiete[j].ende_hex;
      for (k = anfang; k < ende; k++)
      {                 /* lese Hexaeder ------------------------ */
        i = fscanf(fd, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i",
                    &g_element[k].pn[0], &g_element[k].pn[1],
                    &g_element[k].pn[2], &g_element[k].pn[3],
                    &g_element[k].pn[4], &g_element[k].pn[5],
                    &g_element[k].pn[6], &g_element[k].pn[7],
                    &g_element[k].nachbar[0], &g_element[k].nachbar[1],
                    &g_element[k].nachbar[2], &g_element[k].nachbar[3],
                    &g_element[k].nachbar[4], &g_element[k].nachbar[5]);
        g_element[k].element_nr = k+1;
        g_element[k].anz_knoten   = 8;
        g_element[k].stoff_nr     = j + 1;
        g_element[k].zeichnen = 1;
        g_element[k].cut = NULL;

        for (i = 0; i < 8; i++)
        { 
          maxx = MAX(maxx, g_knoten[g_element[k].pn[i]-1].koord[0]);
          maxy = MAX(maxy, g_knoten[g_element[k].pn[i]-1].koord[1]);
          maxz = MAX(maxz, g_knoten[g_element[k].pn[i]-1].koord[2]);
          minx = MIN(minx, g_knoten[g_element[k].pn[i]-1].koord[0]);
          miny = MIN(miny, g_knoten[g_element[k].pn[i]-1].koord[1]);
          minz = MIN(minz, g_knoten[g_element[k].pn[i]-1].koord[2]);
        }
      }                 /*  Schleife Hexaeder Ende ------------------ */

      anfang = g_stoffgebiete[j].anfang_pen;
      ende   = g_stoffgebiete[j].ende_pen;
      for (k = anfang; k < ende; k++)
      {               /* lese Penataeder --------------------------- */
         i = fscanf(fd, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i",
                &g_element[k].pn[0], &g_element[k].pn[1],
                &g_element[k].pn[2], &g_element[k].pn[3],
                &g_element[k].pn[4], &g_element[k].pn[5],
                &g_element[k].pn[6], &g_element[k].pn[7],
                &g_element[k].nachbar[0], &g_element[k].nachbar[1],
                &g_element[k].nachbar[2], &g_element[k].nachbar[3],
                &g_element[k].nachbar[4], &g_element[k].nachbar[5]);
         g_element[k].element_nr= k+1;
         g_element[k].anz_knoten = 6;
         g_element[k].stoff_nr  = j + 1;
         g_element[k].zeichnen = 1;
         g_element[k].cut = NULL;
         
         for (i = 0; i < 6; i++)
         { 
          maxx = MAX(maxx, g_knoten[g_element[k].pn[i]-1].koord[0]);
          maxy = MAX(maxy, g_knoten[g_element[k].pn[i]-1].koord[1]);
          maxz = MAX(maxz, g_knoten[g_element[k].pn[i]-1].koord[2]);
          minx = MIN(minx, g_knoten[g_element[k].pn[i]-1].koord[0]);
          miny = MIN(miny, g_knoten[g_element[k].pn[i]-1].koord[1]);
          minz = MIN(minz, g_knoten[g_element[k].pn[i]-1].koord[2]);
          }
      }

      anfang = g_stoffgebiete[j].anfang_tet;
      ende   = g_stoffgebiete[j].ende_tet;
      for (k = anfang; k < ende; k++)
	{               /* lese Tetraeder ---------------------------- */
          i = fscanf(fd, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i",
		     &g_element[k].pn[0], &g_element[k].pn[1],
		     &g_element[k].pn[2], &g_element[k].pn[3],
		     &g_element[k].pn[4], &g_element[k].pn[5],
		     &g_element[k].pn[6], &g_element[k].pn[7],
		     &g_element[k].nachbar[0], &g_element[k].nachbar[1],
		     &g_element[k].nachbar[2], &g_element[k].nachbar[3],
		     &g_element[k].nachbar[4], &g_element[k].nachbar[5]);
          g_element[k].element_nr= k+1;
          g_element[k].anz_knoten = 4;
          g_element[k].stoff_nr  = j + 1;
          g_element[k].zeichnen = 1;
          g_element[k].cut = NULL;
          
          for (i = 0; i < 4; i++)
	    { 
	      maxx = MAX(maxx, g_knoten[g_element[k].pn[i]-1].koord[0]);
	      maxy = MAX(maxy, g_knoten[g_element[k].pn[i]-1].koord[1]);
	      maxz = MAX(maxz, g_knoten[g_element[k].pn[i]-1].koord[2]);
	      minx = MIN(minx, g_knoten[g_element[k].pn[i]-1].koord[0]);
	      miny = MIN(miny, g_knoten[g_element[k].pn[i]-1].koord[1]);
	      minz = MIN(minz, g_knoten[g_element[k].pn[i]-1].koord[2]);
	    }
      }
/* belege Min- ud Max-Koordinaten in Stoffgebiete-Struktur --------- */
      g_stoffgebiete[j].min[0] = minx;
      g_stoffgebiete[j].min[1] = miny;
      g_stoffgebiete[j].min[2] = minz;
      g_stoffgebiete[j].max[0] = maxx;
      g_stoffgebiete[j].max[1] = maxy;
      g_stoffgebiete[j].max[2] = maxz;
    }

/* einlesen der Interface-Elemente --------------------------------- */
    for (k = g_geodat.nvol; k < g_geodat.ninter + g_geodat.nvol; k++)
      {                 /* Schleife ueber alle Hexaeder -------------- */
	i = fscanf(fd, "%i%i%i%i%i%i%i%i%*i%*i%*i%*i%*i%*i",
		   &g_element[k].pn[0], &g_element[k].pn[1],
		   &g_element[k].pn[2], &g_element[k].pn[3],
		   &g_element[k].pn[4], &g_element[k].pn[5],
		   &g_element[k].pn[6], &g_element[k].pn[7]);
	g_element[k].element_nr= g_geodat.nvol + k+1;
	g_element[k].anz_knoten = 8;
	g_element[k].stoff_nr  = g_geodat.nstoff +1;
      }
  return (0);
}
