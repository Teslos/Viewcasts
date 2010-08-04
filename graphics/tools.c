/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   tools. c                                         */
/*                                                               */
/* Routinenname               | Funktion                         */
/* ------------------------------------------------------------- */
/*  get_maxkoord              :  bestimme maximale Koordinaten   */
/*  get_minkoord              :  bestimme minimale Koordinaten   */
/*  get_center                :  Schwerpkt d.dargest.Stoffgebiete*/
/*  maxkoord_eintragen        :  trage maximale Koordinaten ein  */
/*  set_maske                 :  setzt Maske fuer Darstellung    */
/*  netz_aus                  :  schaltet Netz aus               */
/*  stoffnr_universal_aendern :  Umnummerierung: Stoffe Unv-file */
/*  zentrieren                :  zentriert Geometrie             */
/*  bestimme_position         :  bestimmt Absenkposition         */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include "color3d.h"
#include "strukturen.h"
#include "grafik.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define FEHLER  -1
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* ****************************************************************** */
float get_maxkoord()
/* ****************************************************************** */
{
  float maxkoord;
  int   i; 
/* ****************************************************************** */
/* Ruecksprung wenn noch keine Geometrie eingelesen ist ------------- */
  if (g_geodat.npunkt == 0) return (0.0);

  maxkoord = -10000000;                      /* init ---------------- */

 for (i = 1; i <= g_geodat.nstoff; i++)
 {
   if (g_Maske[i] != 0)
   {
     maxkoord = MAX(g_stoffgebiete[i-1].max[0], maxkoord);
     maxkoord = MAX(g_stoffgebiete[i-1].max[1], maxkoord);
     maxkoord = MAX(g_stoffgebiete[i-1].max[2], maxkoord);
   }
 }
 return (maxkoord);
}

/* ****************************************************************** */
float get_minkoord()
/* ****************************************************************** */
{
  float minkoord;
  int   i;
/* ****************************************************************** */
/* Ruecksprung wenn noch keine Geometrie eingelesen ist ------------- */
  if (g_geodat.npunkt == 0) return (0.0);

  minkoord = 10000000;

 for (i = 1; i <= g_geodat.nstoff; i++)
 {
   if (g_Maske[i] != 0)
   {
     minkoord = MIN(g_stoffgebiete[i-1].min[0], minkoord);
     minkoord = MIN(g_stoffgebiete[i-1].min[1], minkoord);
     minkoord = MIN(g_stoffgebiete[i-1].min[2], minkoord);
   }
 }
 return (minkoord);
}

/* ****************************************************************** */
int get_center(t_koord center)
/* ****************************************************************** */
{
  t_koord minkoord, maxkoord;
  int     i;
/* ****************************************************************** */
/* Ruecksprung wenn noch keine Geometrie eingelesen ist ------------- */
  if (g_geodat.npunkt == 0) return (0);

  minkoord[0] = minkoord[1] = minkoord[2] = 10000000;
  maxkoord[0] = maxkoord[1] = maxkoord[2] = -10000000;

  for (i = 1; i <= g_geodat.nstoff; i++)
  {

   if (g_Maske[i] != 0)
   {
     minkoord[0] = MIN(g_stoffgebiete[i-1].min[0], minkoord[0]);
     minkoord[1] = MIN(g_stoffgebiete[i-1].min[1], minkoord[1]);
     minkoord[2] = MIN(g_stoffgebiete[i-1].min[2], minkoord[2]);
     maxkoord[0] = MAX(g_stoffgebiete[i-1].max[0], maxkoord[0]);
     maxkoord[1] = MAX(g_stoffgebiete[i-1].max[1], maxkoord[1]);
     maxkoord[2] = MAX(g_stoffgebiete[i-1].max[2], maxkoord[2]);
   }
 }
 center[0] = (maxkoord[0] - minkoord[0]) / 2.0;
 center[1] = (maxkoord[1] - minkoord[1]) / 2.0;
 center[2] = (maxkoord[2] - minkoord[2]) / 2.0;
 return (1);
}

/* ****************************************************************** */
void set_maske(int aktion, int stoffnr, int wert)
/* ****************************************************************** */
{
/* Die gueltigen Stoffnummern beginnen bei 1, Stoffnummerist 0 falls  */
/* Wert ueber alle Stoffe gesetzt wird ------------------------------ */

/* setzen der Maske fuer die Darstellung entsprechend : ------------- */
/* wert: ------------------------------------------------------------ */
/* wert   = 0: nicht zeichnen --------------------------------------- */
/* wert   = 1: solid zeichnen --------------------------------------- */
/* wert   = 2: Drahtgitter zeichnen --------------------------------- */
/* wert   = 3: transparent zeichnen --------------------------------- */
/* wert   = 4: Netz zeichnen ---------------------------------------- */
/* wert   = 5: result zeichnen -------------------------------------- */
/* wert   = 6: solid zeichnen mit Netz ------------------------------ */
/* wert   = 7: transparent zeichnen mit Netz ------------------------ */
/* wert   = 8: result zeichnen mit Netz ----------------------------- */
/* wert   = 9: zeichne nur Rahmen ----------------------------------- */

  int i;               /* Schleifenzaehler -------------------------- */
/* ****************************************************************** */

  switch (aktion)
  {
    case LOESCHE_ALLE:
        for (i = 0; i <= g_geodat.nstoff; g_Maske[i++] = 0);
      break;
    case LOESCHE_STOFF:
        if (stoffnr > 0 && stoffnr <= g_geodat.nstoff)
          g_Maske[stoffnr] = 0;
      break;
    case SETZE_ALLE:
      for (i = 0; i <= g_geodat.nstoff; i++)
      {
        if (wert == 4)
        {
          if (g_Maske[i] == 1) 
            g_Maske[i] = 6;
          else if (g_Maske[i] == 3)
            g_Maske[i] = 7;
          else if (g_Maske[i] == 5)
            g_Maske[i] = 8;
        }
        else
          g_Maske[i] = wert;
      }
      break;
    case SETZE_STOFF:
      if (stoffnr > 0 && stoffnr <= g_geodat.nstoff)
      {
        for (i = 0; i <= g_geodat.nstoff; g_Maske[i++] = 0);
        if (wert == 4)
        {
          if (g_Maske[i] == 1) 
            g_Maske[i] = 6;
          else if (g_Maske[i] == 3)
            g_Maske[i] = 7;
          else if (g_Maske[i] == 5)
            g_Maske[i] = 8;
        }
        else
          g_Maske[stoffnr] = wert;
      }
      break;
    case SETZE_STOFF_PLUS:
      if (stoffnr > 0 && stoffnr <= g_geodat.nstoff)
      {
        if (wert == 4)
        {
          if (g_Maske[stoffnr] == 1) 
            g_Maske[stoffnr] = 6;
          else if (g_Maske[stoffnr] == 3)
            g_Maske[stoffnr] = 7;
          else if (g_Maske[stoffnr] == 5)
            g_Maske[stoffnr] = 8;
        }
        else
          g_Maske[stoffnr] = wert;
      }
      break;

    case SETZE_NETZ_MINUS:
      if (stoffnr > 0 && stoffnr <= g_geodat.nstoff)
      {
        if (g_Maske[stoffnr] == 6)
          g_Maske[stoffnr] = 1;
        else if (g_Maske[stoffnr] == 7)
          g_Maske[stoffnr] = 3;
        else if (g_Maske[stoffnr] == 8)
          g_Maske[stoffnr] = 5;
      }
      break;
  }
return;
}
/* ******************* ende set_maske ******************************* */

/* ****************************************************************** */
void stoffnr_universal_aendern(void)
/* ****************************************************************** */
{
  int  i,
      *stoffnr_index,
      *stoffnr,
       maxstoffnr;
/* ****************************************************************** */

  stoffnr   = (int*) malloc(g_geodat.nstoff * sizeof (int));

/* tatsaechlich existierende Stoffnummern sammeln ------------------ */
  for (i = 0; i < g_geodat.nstoff; stoffnr[i] = g_stoffgebiete[i++].nr);

/* bestimme Maximale Stoffnummer ----------------------------------- */
  maxstoffnr = 0;
  for (i = 0; i < g_geodat.nstoff; i++)
       maxstoffnr = MAX(maxstoffnr, stoffnr[i]);

/* erzeuge Feld fuer Umnummerierung -------------------------------- */
  stoffnr_index = (int*) malloc((maxstoffnr+1)*sizeof(int));
  stoffnr_index[0] = 0;
  for (i = 1; i <= g_geodat.nstoff; i++)
       stoffnr_index[stoffnr[i-1]] = i;

  for (i = 0; i < g_geodat.nstoff; i++)
       g_stoffgebiete[i].nr = i+1;

  for (i = 0; i < g_geodat.nvol; i++)
       g_element[i].stoff_nr = stoffnr_index[g_element[i].stoff_nr];

  free (stoffnr); 
  free (stoffnr_index); 
  return;
}

/* ****************************************************************** */
void maxkoord_eintragen(void)  /*   :  trage maximale Koordinaten ein */
/* ****************************************************************** */
{
  t_koord *stoffmax, *stoffmin;/* lokale Min-Max-Felder ------------- */
  t_element *p_element;        /* lokaler Pointer auf Elemente ------ */
  int i, j, k;                 /* Zaehler --------------------------- */
/* ****************************************************************** */
/* init Speicher fuer lokale Min-Max-Felder ------------------------- */
  stoffmax = (t_koord*)malloc(g_geodat.nstoff*sizeof(t_koord));
  stoffmin = (t_koord*)malloc(g_geodat.nstoff*sizeof(t_koord));

  for (j = 0; j < g_geodat.nstoff; j++)
  {
    stoffmax[j][0] = -10000000;              /* init Min-Max-Felder - */
    stoffmax[j][1] = -10000000;
    stoffmax[j][2] = -10000000;
    stoffmin[j][0] = 10000000;
    stoffmin[j][1] = 10000000;
    stoffmin[j][2] = 10000000;
  }
                                /* Schleife ueber alle Elemente ---- */
  p_element = g_element;        /* init lokalen Pointer f. Elementfeld*/
  for (j = 0; j < g_geodat.nvol; j++)
  {                             
    i = g_element[j].stoff_nr - 1; 
    for (k = 0; k < p_element->anz_knoten; k++)
    {
      stoffmax[i][0] = MAX(stoffmax[i][0], g_knoten[p_element->pn[k]-1].koord[0]);
      stoffmax[i][1] = MAX(stoffmax[i][1], g_knoten[p_element->pn[k]-1].koord[1]);
      stoffmax[i][2] = MAX(stoffmax[i][2], g_knoten[p_element->pn[k]-1].koord[2]);
      stoffmin[i][0] = MIN(stoffmin[i][0], g_knoten[p_element->pn[k]-1].koord[0]);
      stoffmin[i][1] = MIN(stoffmin[i][1], g_knoten[p_element->pn[k]-1].koord[1]);
      stoffmin[i][2] = MIN(stoffmin[i][2], g_knoten[p_element->pn[k]-1].koord[2]);
    }
    p_element++;
  }

  for (j = 0; j < g_geodat.nstoff; j++)
  {
    g_stoffgebiete[j].max[0] = stoffmax[j][0];
    g_stoffgebiete[j].max[1] = stoffmax[j][1];
    g_stoffgebiete[j].max[2] = stoffmax[j][2];
    g_stoffgebiete[j].min[0] = stoffmin[j][0];
    g_stoffgebiete[j].min[1] = stoffmin[j][1];
    g_stoffgebiete[j].min[2] = stoffmin[j][2];
  }
}
/* ****************************************************************** */

/* ****************************************************************** */
void zentrieren(void)  /*   :  zentrieren der Geometrie ------------  */
/* ****************************************************************** */
{
  t_koord min, max;
  float mitte;
  float x, y, z;
  int i;
/* ****************************************************************** */
/* Ruecksprung falls noch keine Geometrie gelesen ist --------------- */
  if (g_geodat.npunkt == 0) return;

/* Falls Verschiebevektor nicht ex. bestimme ihn -------------------- */
  if (!g_transform.zentriert_ex)
  {
/* bestimme minimale u. maximale Ausdehnumg in jede Richtung -------- */
    min[0] =  10000000;
    min[1] =  10000000;
    min[2] =  10000000;
    max[0] = -10000000;
    max[1] = -10000000;
    max[2] = -10000000;

   for (i = 1; i <= g_geodat.nstoff; i++)
   {
     min[0] = MIN(g_stoffgebiete[i-1].min[0], min[0]);
     min[1] = MIN(g_stoffgebiete[i-1].min[1], min[1]);
     min[2] = MIN(g_stoffgebiete[i-1].min[2], min[2]);
     max[0] = MAX(g_stoffgebiete[i-1].max[0], max[0]);
     max[1] = MAX(g_stoffgebiete[i-1].max[1], max[1]);
     max[2] = MAX(g_stoffgebiete[i-1].max[2], max[2]);
   }

/* bestimme Schwerpunkt der Geometrie ------------------------------ */
    mitte = ((max[0] - min[0]) / 2.0) + min[0];
    g_transform.zentrier_vector[0] = mitte;
    mitte = ((max[1] - min[1]) / 2.0) + min[1];
    g_transform.zentrier_vector[1] = mitte;
    mitte = ((max[2] - min[2]) / 2.0) + min[2];
    g_transform.zentrier_vector[2] = mitte;
    g_transform.zentriert_ex = TRUE; 
  }

  x = g_transform.zentrier_vector[0];
  y = g_transform.zentrier_vector[1];
  z = g_transform.zentrier_vector[2];

  if (!g_transform.zentriert_on)
  {
    if (x || y || z)
    {
      for (i = 0; i < g_geodat.npunkt; i++)
      {
        g_knoten[i].koord[0] = g_knoten[i].koord[0] - x;
        g_knoten[i].koord[1] = g_knoten[i].koord[1] - y;
        g_knoten[i].koord[2] = g_knoten[i].koord[2] - z;
      }
    }
    g_transform.zentriert_on = TRUE; 
  }
  else
  {
    if (x || y || z)
    {
      for (i = 0; i < g_geodat.npunkt; i++)
      {
        g_knoten[i].koord[0] = g_knoten[i].koord[0] + x;
        g_knoten[i].koord[1] = g_knoten[i].koord[1] + y;
        g_knoten[i].koord[2] = g_knoten[i].koord[2] + z;
      }
    }
    g_transform.zentriert_on = FALSE;
  }
}
/* ****************************************************************** */

/* ****************************************************************** */
void netz_aus()
/* ****************************************************************** */
{
  int i;

  for (i = 0; i <= g_geodat.nstoff; i++)
  {
    if (g_Maske[i] == 6)
      g_Maske[i] = 1;
    else if (g_Maske[i] == 7)
      g_Maske[i] = 3;
    else if (g_Maske[i] == 8)
      g_Maske[i] = 5;
  }
  return;
/* ****************************************************************** */
}

/* ****************************************************************** */
void bestimme_position()
/* ****************************************************************** */
{
  float zeit1, zeit2,
        posit, posit1, posit2,
        diffz, diffp,
        deltaz, deltap;
  int   i;
/* ****************************************************************** */
  for (i = 0; i < g_ofen.anz_stuetz; i++)/* Schleife ueber Stuetzst.  */ 
 {
                                        /* suche naechstgeleg. Zeitp. */
    if (g_loesung.zeiten[g_animation.akt_index] <= g_ofen.pos_zeiten[i])
    {
      if (i == 0)                       /* vor 1. Zeit -------------- */
      {
        g_ofen.akt_position = -g_ofen.positionen[i][2];
        return;
      }
      
                                        /* Zeit getroffen ----------- */
      if (g_ofen.pos_zeiten[i] == g_loesung.zeiten[g_animation.akt_index])
      {
        g_ofen.akt_position = -g_ofen.positionen[i][2];
        return; 
      }
                                        /* Position aendert sich nicht*/
      if (g_ofen.positionen[i][2] == g_ofen.positionen[i-1][2])
      {
        g_ofen.akt_position = -g_ofen.positionen[i][2];
        return;
      }
      zeit1  = g_ofen.pos_zeiten[i-1];  /* interpol.Position nach Zeit*/
      zeit2  = g_ofen.pos_zeiten[i];
      posit1 = g_ofen.positionen[i-1][2];
      posit2 = g_ofen.positionen[i][2];
      diffz  = zeit2 - zeit1;
      diffp  = posit2 - posit1;
      deltaz = g_loesung.zeiten[g_animation.akt_index] - zeit1;
      deltap = diffp/diffz;
      posit  = posit1 + deltaz * deltap;
      g_ofen.akt_position = -posit;
      return;
    }
  }                                     /* nach letzter Zeit -------- */
  if (g_loesung.zeiten[g_animation.akt_index] > 
      g_ofen.pos_zeiten[g_ofen.anz_stuetz - 1])
  {
    g_ofen.akt_position = -g_ofen.positionen[g_ofen.anz_stuetz - 1][2];
    return;
  }

/* ************************ ende bestimme_position ****************** */
}

