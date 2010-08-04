/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   init_data.c                                      */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*   init_data  |                                                */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>

#include "color3d.h"
#include "motif.h"
#include "read_unv.h"

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* ******************************************************************** */
int init_data(t_bool start)
/* ******************************************************************** */
{
  int i;
  t_arbeits_element* help;
/* ******************************************************************** */

/* Oberflaeche initialisieren ----------------------------------------- */
  init_color3d();
  init_interface();

  mode = 0;
  
 
  g_geodat.nvol   = 0;
  g_geodat.ninter = 0;
  g_geodat.npunkt = 0;
  g_geodat.ngrp   = 0;
  g_geodat.nstoff = 0;

/* setze alle Feldpointer auf NULL ------------------------------------ */
  if (start)               /* 1. Aufruf der Routine ------------------- */
  {
    g_knoten                = (t_knoten *)NULL;
    g_element               = (t_element *)NULL;
    g_stoffgebiete          = (t_stoffgebiet *)NULL;
    g_stoffgruppen          = (t_stoffgruppe *)NULL;
    g_Maske                 = (int *)NULL;
    g_loesdef               = (t_loesdef *)NULL;
    loesungen               = (t_loesdef *)NULL;
    g_loesung.temp          = NULL;
    g_loesung.anzahl_felder = 0;
    g_loesung.zeiten        = NULL;
    g_loesung.param_gesetzt = FALSE;
    g_loesung.typ_gesetzt   = FALSE;
    g_loesung.offen         = FALSE;
    g_loesung.gescannt      = FALSE;
    g_loesung.gelesen       = FALSE;
	g_loesung.mmap          = FALSE;
    g_loesung.min_max       = FALSE;
    g_loesung.akt_min       = 0;
    g_loesung.akt_max       = 0;
    g_loesung.Min           = 0;
    g_loesung.Max           = 2000;
    g_loesung.schritt       = 0;
    strcpy(g_loesung.dateiname, "");
    strcpy(g_loesung.ausgabe, "");
    strcpy(g_loesung.parameter, "");
    strcpy(g_loesung.einheit, "");

    /* color extra */
	g_color_extra.transparency = 0.25;

	g_stroemung.vektor = (t_koord *)NULL;
    g_stroemung.zeiten = (float *)NULL;
    g_stroemung.factor = 100.0;
    g_stroemung.old_factor = 1.0;
    g_spannung.vektor  = (t_koord *)NULL;
    g_spannung.zeiten =  (float *)NULL;
    g_spannung.factor = 100.0;
    g_knoten_diff      = (t_knoten *)NULL;
    g_spannung.verzerrung = (t_knoten *)NULL;
    g_animation.zeiten = (float *)NULL;
    g_animation.art = 0;
    g_animation.anim_frame = 1;
  }
  else
  {
    if (g_knoten != NULL)
    {
      free(g_knoten);
      g_knoten = (t_knoten *)NULL;
    }
    
    if (g_schnittpunktliste != NULL) 
    {
      free (g_schnittpunktliste);
      g_schnittpunktliste = NULL;
      g_anz_schnittpunkte = 0;
    }
    
    if (g_element != NULL)
    {
      for(i=0;i<g_geodat.nvol;i++)
      {
        if(g_element[i].zeichnen==2)
        {
          help = g_element[i].cut;
          free(help->node);
          free(help);
          g_element[i].cut = NULL;
        } 
        g_element[i].zeichnen = 1;
      }
      free(g_element);
      g_element = (t_element *)NULL;
    }

    if (g_stoffgebiete != NULL)
    {
      free(g_stoffgebiete);
      g_stoffgebiete = (t_stoffgebiet *)NULL;
    }

    if (g_stoffgruppen != NULL)
    {
      free(g_stoffgruppen);
      g_stoffgruppen = (t_stoffgruppe *)NULL;
    }

    if (g_Maske != NULL)
    {
      free(g_Maske);
      g_Maske = (int *)NULL;
    }

    if (g_loesung.temp != NULL)
    {
      free(g_loesung.temp);
      g_loesung.temp = NULL;
    }
    g_loesung.anzahl_felder = 0;

    if (g_loesung.zeiten != NULL)
    {
      free(g_loesung.zeiten);
      g_loesung.zeiten = NULL;
    }
    g_loesung.param_gesetzt = FALSE;
    g_loesung.typ_gesetzt   = FALSE;
    g_loesung.offen         = FALSE;
    g_loesung.gescannt      = FALSE;
    g_loesung.gelesen       = FALSE;
    g_loesung.min_max       = TRUE;
    g_loesung.akt_min       = 0;
    g_loesung.akt_max       = 0;
    g_loesung.Min           = 0;
    g_loesung.Max           = 2000;
    g_loesung.schritt       = 0;
    strcpy(g_loesung.dateiname, "");
    strcpy(g_loesung.ausgabe, "");
    strcpy(g_loesung.parameter, "");
    strcpy(g_loesung.einheit, "");

    if (g_stroemung.vektor != NULL)
    {
      free(g_stroemung.vektor);
      g_stroemung.vektor = (t_koord *)NULL;
    }

    if (g_stroemung.zeiten != NULL)
    {
      free(g_stroemung.zeiten);
      g_stroemung.zeiten = (float *)NULL;
    }

    if (g_spannung.zeiten != NULL)
    {
      free(g_spannung.zeiten);
      g_spannung.zeiten = (float *)NULL;
    }

    if (g_spannung.vektor != NULL)
    {
      free(g_spannung.vektor);
      g_spannung.vektor = (t_koord *)NULL;
    }

    if (g_spannung.verzerrung != NULL)
    {
      free(g_spannung.verzerrung);
      g_spannung.verzerrung = (t_knoten *)NULL;
    }

    if (g_animation.zeiten != NULL)
    {
      free(g_animation.zeiten);
      g_animation.zeiten = (float *)NULL;
      g_animation.art = 0;
    }

  }
  g_spannung.on              = FALSE;   /* Spannungsdarstellung abschalten */
  g_spannung.vektor_offen    = FALSE;   /* Flagge Vektorloesg. geoeffnet   */
  g_spannung.vektor_gescannt = FALSE;   /* Flagge Spannungsloesg gelesen   */
  g_spannung.vektor_gelesen  = FALSE;   /* Flagge Vektorloesg. gelesen     */
  g_spannung.vektorani_on    = FALSE;   /* Flagge Spannungs-Animation      */

  g_stroemung.on             = FALSE;   /* Stroemungsdarstellung abschalten*/
  g_stroemung.art            = 1;       /* Simpel - Darstellung            */
  g_stroemung.vektor_offen   = FALSE;   /* Flagge Vektorloesg. geoeffnet   */
  g_stroemung.vektor_gescannt= FALSE;   /* Flagge Spannungsloesg gelesen   */
  g_stroemung.vektor_gelesen = FALSE;   /* Flagge Vektorloesg. gelesen     */
  g_stroemung.gitter_reduced = FALSE;   /* Es ist noch kein red. Gitter da */
  g_stroemung.knoten_sortiert= FALSE;   /* Knoten sind noch nicht sortiert */

  for(i=0;i<6;i++)
  {
    g_spannung.changes[i] =-1;
    g_stroemung.changes[i]=-1;
  }

  g_picking.punkte = NULL;             /* Liste fuer gepickte Punkte    */
  g_sgiobj.object  = NULL;             /* Sgo object                    */                
  g_sgiobj.gelesen = FALSE;            /* Sgo object is not read yet    */     
  loesung_define();

  return(0);
}

