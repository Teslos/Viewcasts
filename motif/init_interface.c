/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname              | Funktion                          */
/* ------------------------------------------------------------- */
/* init_interface            |                                   */
/* init_stoffe_interface     |                                   */
/* init_transform_interface  |                                   */
/* init_loesung_interface    |                                   */
/* init_2dgrafik_interface   |                                   */
/* init_vektor_interface     |                                   */
/* init_isolinien_interface  |                                   */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>

#include <Xm/ToggleB.h>

#include "color3d.h"
#include "motif.h"
#include "grafik.h"
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* initialisiert Menuebar im Hauptfenster ---------------------------- */

void init_interface()
{
  TF_in_TT_translations_table = XtParseTranslationTable("<FocusOut>: focus_out_TF_in_TT()");

  XtSetSensitive(widgetMain.PB_neu,                 FALSE);
  XtSetSensitive(widgetMain.PB_geop_lesen,           TRUE);
  XtSetSensitive(widgetMain.PB_unv_lesen,           FALSE);
  XtSetSensitive(widgetMain.PB_loes_lesen,          FALSE);
  XtSetSensitive(widgetMain.PB_spannung_lesen,      FALSE);
  XtSetSensitive(widgetMain.PB_stroemung_lesen,     FALSE);
  XtSetSensitive(widgetMain.PB_bild_lesen,          FALSE);
  XtSetSensitive(widgetMain.PB_bild_schreiben,      FALSE);
  XtSetSensitive(widgetMain.PB_snapshot,            FALSE);

  XtSetSensitive(widgetMain.CB_stoffe,              FALSE);
  XtSetSensitive(widgetMain.CB_transformieren,      FALSE);
  XtSetSensitive(widgetMain.CB_2dgrafik,            FALSE);
  XtSetSensitive(widgetMain.CB_isolinien,           FALSE);
  XtSetSensitive(widgetMain.CB_stroem_spann,        FALSE);
/*
  XtSetSensitive(widgetMain.CB_loesung,             FALSE);
*/

  XtSetSensitive(widgetMain.PB_animation_start,     FALSE);
  XtSetSensitive(widgetMain.PB_animation_stop,      FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt,         FALSE);
  XtSetSensitive(widgetMain.T_zeitschritt,          FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt_vor,     FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, FALSE);
  XtSetSensitive(widgetMain.PB_alleStoffe_an,       FALSE);
  XtSetSensitive(widgetMain.PB_alleStoffe_aus,      FALSE);

  b_init_main = TRUE;        /* Flagge dass Fenster initialisiert */
  b_spin = FALSE;
return;
}

/*************************************************************************/
void init_stoffe_interface(void)
/*************************************************************************/
{

  XmToggleButtonSetState(widget_stoffe.TB_alle_stoffe, g_transform.alle_stoffe_on, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_solid, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_netz, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_gitter, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_transparent, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_result, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_rahmen, TRUE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_absenken, FALSE, FALSE);
  XmToggleButtonSetState(widget_stoffe.TB_stoffgruppen, FALSE, FALSE);

  if (g_loesung.gescannt) 
    XtSetSensitive(widget_stoffe.TB_result, TRUE);
  else
    XtSetSensitive(widget_stoffe.TB_result, FALSE);

  if (g_ofen.ex)
    XtSetSensitive(widget_stoffe.TB_absenken, TRUE);
  else
    XtSetSensitive(widget_stoffe.TB_absenken, FALSE);

  XtSetSensitive(widget_stoffe.TB_stoffgruppen, FALSE);
  XtSetSensitive(widget_stoffe.TT_stoffgebiete, FALSE);

  b_init_stoffe = TRUE;        /* Flagge dass Fenster initialisiert */
  return;
}

/*************************************************************************/
void init_transform_interface(void)
/*************************************************************************/
{
  XmToggleButtonSetState(widget_transform.TB_zyklus, FALSE, FALSE);
  XmToggleButtonSetState(widget_transform.TB_achsez, TRUE, FALSE);
  XtSetSensitive(widget_transform.T_anz_zyk,       FALSE);
  XtSetSensitive(widget_transform.AB_anzahl_plus,  FALSE);
  XtSetSensitive(widget_transform.AB_anzahl_minus, FALSE);
  XtSetSensitive(widget_transform.T_winkel,        FALSE);
  XtSetSensitive(widget_transform.TB_achsex,       FALSE);
  XtSetSensitive(widget_transform.TB_achsey,       FALSE);
  XtSetSensitive(widget_transform.TB_achsez,       FALSE);

  XmToggleButtonSetState(widget_transform.TB_spiegeln, FALSE, FALSE);
  XtSetSensitive(widget_transform.TB_ebenexy,      FALSE);
  XtSetSensitive(widget_transform.TB_ebenexz,      FALSE);
  XtSetSensitive(widget_transform.TB_ebeneyz,      FALSE);

  XmToggleButtonSetState(widget_transform.TB_schneiden, FALSE, FALSE);
  XmToggleButtonSetState(widget_transform.TB_schneiden_aus, FALSE, FALSE);
  XmToggleButtonSetState(widget_transform.TB_def_koord, FALSE, FALSE);
  XmToggleButtonSetState(widget_transform.TB_def_nummer, FALSE, FALSE);
  XtSetSensitive(widget_transform.TB_schneiden, TRUE);
  XtSetSensitive(widget_transform.TB_schneiden_aus, FALSE);
  XtSetSensitive(widget_transform.TB_def_plane, FALSE);
  XtSetSensitive(widget_transform.TB_def_koord, FALSE);
  XtSetSensitive(widget_transform.TB_def_nummer,FALSE);
  XtSetSensitive(widget_transform.T_punkt1x,  FALSE);
  XtSetSensitive(widget_transform.T_punkt1y,  FALSE);
  XtSetSensitive(widget_transform.T_punkt1z,  FALSE);
  XtSetSensitive(widget_transform.T_punkt2x,  FALSE);
  XtSetSensitive(widget_transform.T_punkt2y,  FALSE);
  XtSetSensitive(widget_transform.T_punkt2z,  FALSE);
  XtSetSensitive(widget_transform.T_punkt3x,  FALSE);
  XtSetSensitive(widget_transform.T_punkt3y,  FALSE);
  XtSetSensitive(widget_transform.T_punkt3z,  FALSE);
  XtSetSensitive(widget_transform.T_punkt1nr, FALSE);
  XtSetSensitive(widget_transform.T_punkt2nr, FALSE);
  XtSetSensitive(widget_transform.T_punkt3nr, FALSE);

  if (g_transform.zyklus_on)
    {
      XmToggleButtonSetState(widget_transform. TB_zyklus, TRUE, TRUE);
    }
  if (g_transform.spiegel_on)
    {
      XmToggleButtonSetState(widget_transform.TB_spiegeln, TRUE, TRUE);
    }
  if (g_transform.schneiden_on)
    {
      XmToggleButtonSetState(widget_transform.TB_schneiden, TRUE, TRUE);
    }
  
  b_init_transform = TRUE;        /* Flagge dass Fenster initialisiert */
  return;
}

/*************************************************************************/
void init_loesung_interface(void)
/*************************************************************************/
{
  b_init_loesung = TRUE;        /* Flagge dass Fenster initialisiert */
  return;
}

/*************************************************************************/
void init_2dgrafik_interface(void)
/*************************************************************************/
{
  b_init_2dgrafik = TRUE;        /* Flagge dass Fenster initialisiert */
  XtSetSensitive(widget2DGrafik.TB_ausgabe, TRUE);
  XtSetSensitive(widget2DGrafik.TB_manuell, TRUE);
  XtSetSensitive(widget2DGrafik.T_omin, FALSE);
  XtSetSensitive(widget2DGrafik.T_omax, FALSE);
  XtSetSensitive(widget2DGrafik.T_amin, FALSE);
  XtSetSensitive(widget2DGrafik.T_amax, FALSE);
  XtSetSensitive(widget2DGrafik.PB_zeichne, FALSE);
  XtSetSensitive(widget2DGrafik.PB_loesche, FALSE);
  XtSetSensitive(widget2DGrafik.PB_calculate, FALSE);
  XtSetSensitive(widget2DGrafik.PB_addpoint, FALSE);

  if (XmToggleButtonGetState(widget2DGrafik.TB_manuell) ||
      XmToggleButtonGetState(widget2DGrafik.TB_pick))
    {
      XtSetSensitive(widget2DGrafik.T_omin, FALSE);
      XtSetSensitive(widget2DGrafik.T_omax, FALSE);
      XtSetSensitive(widget2DGrafik.T_amin, FALSE);
      XtSetSensitive(widget2DGrafik.T_amax, FALSE);
      XtSetSensitive(widget2DGrafik.PB_zeichne, TRUE);
      XtSetSensitive(widget2DGrafik.PB_loesche, TRUE);
      XtSetSensitive(widget2DGrafik.PB_calculate, TRUE);
    }
  return;
}

/*************************************************************************/
void init_vektor_interface(void)
/*************************************************************************/
{
  char c_hilf[12];

  b_init_vektor = TRUE;        /* Flagge dass Fenster initialisiert */

  if (g_animation.temp_on)
    XmToggleButtonSetState(widget_vektor.TB_temperatur, TRUE, FALSE);
  if (g_animation.spann_on)
    XmToggleButtonSetState(widget_vektor.TB_spannung, TRUE, FALSE);
  if (g_animation.stroem_on)
    XmToggleButtonSetState(widget_vektor.TB_stroemung, TRUE, FALSE);

  if (!XmToggleButtonGetState(widget_vektor.TB_spannung) &&
      !XmToggleButtonGetState(widget_vektor.TB_stroemung))
  {
    XtSetSensitive(widget_vektor.L_sp_faktor,     FALSE);
    XtSetSensitive(widget_vektor.T_sp_faktor,     FALSE);
    XtSetSensitive(widget_vektor.L_st_faktor,     FALSE);
    XtSetSensitive(widget_vektor.T_st_faktor,     FALSE);
    XtSetSensitive(widget_vektor.RC_animation, FALSE);
  }
  else
  {
    sprintf(c_hilf, "%8.3f", g_spannung.factor);
    XmTextFieldSetString(widget_vektor.T_sp_faktor, c_hilf);
    sprintf(c_hilf, "%8.3f", g_stroemung.factor);
    XmTextFieldSetString(widget_vektor.T_st_faktor, c_hilf);
  }

  if (g_loesung.gescannt)
    XtSetSensitive(widget_vektor.TB_temperatur, TRUE);
  else 
    XtSetSensitive(widget_vektor.TB_temperatur, FALSE);

  if (g_spannung.vektor_gescannt)
    XtSetSensitive(widget_vektor.TB_spannung, TRUE);
  else 
    XtSetSensitive(widget_vektor.TB_spannung, FALSE);

  if (g_stroemung.vektor_gescannt)
    XtSetSensitive(widget_vektor.TB_stroemung, TRUE);
  else 
    XtSetSensitive(widget_vektor.TB_stroemung, FALSE);

  return;
}

/*************************************************************************/
void init_isolinien_interface(void)
/*************************************************************************/
{
  char c_hilf[12];

  b_init_tabelle = TRUE;        /* Flagge dass Fenster initialisiert */

  if (g_tabelle.ex)
  {
    if (g_tabelle.on)
    {
      XmToggleButtonSetState(widget_isolinien.TB_tabelle_zeichnen,
                             TRUE, FALSE);
      XmToggleButtonSetState(widget_isolinien.TB_tabelle_nicht_zeichnen,
                             FALSE, FALSE);
    }
    else
    {
      XmToggleButtonSetState(widget_isolinien.TB_tabelle_zeichnen,
                             FALSE, FALSE);
      XmToggleButtonSetState(widget_isolinien.TB_tabelle_nicht_zeichnen,
                             TRUE, FALSE);
    }

    switch(g_tabelle.art)
    {
      case 1:
        XmToggleButtonSetState(widget_isolinien.TB_default, TRUE, FALSE);
        break;
      case 2:
        XmToggleButtonSetState(widget_isolinien.TB_grau, TRUE, FALSE);
        break;
      case 3:
        XmToggleButtonSetState(widget_isolinien.TB_raster, TRUE, FALSE);
        break;
      case 4:
        XmToggleButtonSetState(widget_isolinien.TB_datei, TRUE, FALSE);
        break;
    }

    XtSetSensitive(widget_isolinien.TB_tabelle_zeichnen, TRUE);
    XtSetSensitive(widget_isolinien.TB_tabelle_nicht_zeichnen, TRUE);
    XtSetSensitive(widget_isolinien.TB_default, TRUE);
    XtSetSensitive(widget_isolinien.TB_grau,    TRUE);
    XtSetSensitive(widget_isolinien.TB_raster,  TRUE);
    XtSetSensitive(widget_isolinien.TB_datei,   TRUE);
  }
  else
  {
    XtSetSensitive(widget_isolinien.TB_tabelle_zeichnen, FALSE);
    XtSetSensitive(widget_isolinien.TB_tabelle_nicht_zeichnen, FALSE);
    XtSetSensitive(widget_isolinien.TB_default, FALSE);
    XtSetSensitive(widget_isolinien.TB_grau,    FALSE);
    XtSetSensitive(widget_isolinien.TB_raster,  FALSE);
    XtSetSensitive(widget_isolinien.TB_datei,   FALSE);
  }
}









