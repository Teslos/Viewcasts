#ifndef _widgets_h
#define _widgets_h

/* Strukturen fuer widgets in COLOR3D  --------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
/*************************************************************************/
/* Struktur fuer Widgets im Haupt-Fenster ------------------------------ */

typedef struct widget_main
{
  Widget CB_datei;
  Widget PB_neu;
  Widget PB_unv_lesen;
  Widget PB_geop_lesen;
  Widget PB_loes_lesen;
  Widget PB_spannung_lesen;
  Widget PB_stroemung_lesen;
  Widget PB_bild_lesen;
  Widget PB_bild_schreiben;
  Widget PB_object_schreiben;
  Widget PB_object_lesen;
  Widget PB_snapshot;
  Widget PB_createmovie;
  Widget PB_quit;

  Widget CB_stoffe;
  Widget CB_transformieren;
  Widget CB_2dgrafik;
  Widget CB_isolinien;
  Widget CB_stroem_spann;
  Widget CB_loesung;
  Widget CB_hilfe;
  /*Widget CB_extra;*/

  Widget S_colormap;
  Widget S_abkuehl;

  Widget XmForm_r;
  Widget PB_animation_start;
  Widget PB_animation_stop;
  Widget T_zeitschritt;
  Widget T_zeitschritt_start;
  Widget T_zeitschritt_end;
  Widget T_every_frame;
  Widget PB_zeitschritt;
  Widget PB_zeitschritt_vor;
  Widget PB_zeitschritt_zurueck;
  Widget Spin_rot;
  Widget RC_view;
  Widget PB_rot_minus_x;
  Widget PB_rot_minus_xx;
  Widget PB_rot_plus_x;
  Widget PB_rot_plus_xx;
  Widget PB_rot_minus_y;
  Widget PB_rot_minus_yy;
  Widget PB_rot_plus_y;
  Widget PB_rot_plus_yy;
  Widget PB_rot_minus_z;
  Widget PB_rot_minus_zz;
  Widget PB_rot_plus_z;
  Widget PB_rot_plus_zz;
  Widget RX_rot_x;
  Widget RX_rot_y;
  Widget RX_rot_z;

  Widget PB_move_minus_x;
  Widget PB_move_plus_x;
  Widget PB_move_minus_y;
  Widget PB_move_plus_y;
  Widget PB_move_minus_z;
  Widget PB_move_plus_z;

  Widget PB_zoom_manuell;
  Widget PB_zoom_auto;
  Widget PB_zoom_14x;
  Widget PB_zoom_07x;
  Widget PB_reset;

  Widget PB_alleStoffe_an;
  Widget PB_alleStoffe_aus;
  Widget RC_stoffe;
  Widget WB_zoom;

  Widget Frame_da;
  Widget Frame_cmap;
  Widget Frame_abk;
}t_widget_main;

/* Struktur fuer Widgets im Stoffe-Fenster ---------------------------- */

typedef struct widget_stoffe
{
  Widget S_stoffe;
  Widget TT_stoffgebiete;
  Widget XmForm_o;
  Widget RC_allestoffe;
  Widget TB_alle_stoffe;
  Widget TB_solid;
  Widget TB_transparent;
  Widget TB_netz;
  Widget TB_gitter;
  Widget TB_result;
  Widget TB_rahmen;
  Widget TB_absenken;
  Widget TB_stoffgruppen;
  Widget PB_beenden;
  Widget PB_darstellen;
  Widget PB_reset;
  Widget PB_hilfe;
  Widget RC_stoffe;
  Widget S_verschiebung;
  Widget XmForm;
  Widget PB_ok;
}t_widget_stoffe;

/* Struktur fuer Widgets im Transformations-Fenster ------------------- */

typedef struct widget_transform
{
  Widget S_transform;
  Widget PB_ok;
  Widget PB_beenden;
  Widget PB_hilfe;
  Widget TB_zyklus;
  Widget T_anz_zyk;
  Widget AB_anzahl_plus;
  Widget AB_anzahl_minus;
  Widget T_winkel;
  Widget RC_zyklus;
  Widget TB_achsex;
  Widget TB_achsey;
  Widget TB_achsez;
  Widget TB_spiegeln;
  Widget RC_spiegeln;
  Widget TB_ebenexy;
  Widget TB_ebenexz;
  Widget TB_ebeneyz;
  Widget TB_schneiden;
  Widget TB_schneiden_aus;
  Widget TB_def_plane;
  Widget TB_def_koord;
  Widget TB_def_nummer;
  Widget RC_schneiden;
  Widget T_punkt1x;
  Widget T_punkt1y;
  Widget T_punkt1z;
  Widget T_punkt2x;
  Widget T_punkt2y;
  Widget T_punkt2z;
  Widget T_punkt3x;
  Widget T_punkt3y;
  Widget T_punkt3z;
  Widget RC_nummer;
  Widget T_punkt1nr;
  Widget T_punkt2nr;
  Widget T_punkt3nr;
  Widget LS_dis_cutting;
  Widget TB_zentrieren;
}t_widget_transform;

/* Struktur fuer Widgets im 2D-Grafik-Fenster ------------------------- */
typedef struct widget_2dgrafik
{
  Widget S_2dgrafik;
  Widget L_header;
  Widget PB_ok;
  Widget PB_beenden;
  Widget PB_hilfe;
  Widget RC_eingabe;
  Widget TB_pick;
  Widget TB_manuell;
  Widget TB_ausgabe;
  Widget RC_ausgabe;
  Widget TB_abkuehlkurve;
  Widget TB_seigerung;
  Widget T_winkel;
  Widget L_winkel;
  Widget T_element;
  Widget L_element;
  Widget TT_picking;
  Widget TT_pickingX;
  Widget TT_pickingY;
  Widget TT_pickingZ;
  Widget RC_zeichne;
  Widget PB_zeichne;
  Widget PB_loesche;
  Widget PB_gnuplot;
  Widget PB_calculate;
  Widget PB_addpoint;
  Widget RC_achsen;
  Widget T_omin;
  Widget T_amin;
  Widget T_omax;
  Widget T_amax;
}t_widget_2dgrafik;


/* Struktur fuer Widgets im Tabellen / Isolinien-Fenster --------------- */
typedef struct widget_isolinien
{
  Widget S_isolinien;
  Widget TB_tabelle_zeichnen;
  Widget TB_tabelle_nicht_zeichnen;
  Widget TB_tabelle_auto_range;
  Widget PB_beenden;
  Widget PB_ok;
  Widget PB_hilfe;
  Widget L_tabelle;
  Widget RC_tabelle;
  Widget TB_default;
  Widget TB_grau;
  Widget TB_raster;
  Widget TB_datei;
  Widget L_isolinien;
  Widget TT_isolinien;
  Widget L_wert;
  Widget T_wert;
  Widget L_breit;
  Widget T_breit;
  Widget PB_erzeugen;
  Widget PB_loeschen;
  Widget TT_isosurfaces;
  Widget T_wertisosurface;
  Widget PB_genisosurface;
  Widget PB_delisosurface;
  Widget TT_isovolumes;
  Widget T_wertisovolume;
  Widget PB_genisovolume;
  Widget PB_delisovolume;
  Widget L_intervall;
  Widget L_max;
  Widget T_max;
  Widget L_min;
  Widget T_min;
}t_widget_isolinien;

/* Struktur fuer Widgets im Stroemung-Spannungs-Fenster */

typedef struct widget_vektor
{
  Widget S_vektor;
  Widget PB_ok;
  Widget PB_beenden;
  Widget PB_hilfe;
  Widget TB_skalar;
  Widget L_animation;
  Widget RC_animation;
  Widget L_start;
  Widget T_start;
  Widget PB_start;
  Widget L_stop;
  Widget T_stop;
  Widget PB_stop;
  Widget TB_temperatur;
  Widget TB_spannung;
  Widget L_sp_faktor;
  Widget T_sp_faktor;
  Widget AB_sp_faktor_up;
  Widget AB_sp_faktor_down;
  Widget TB_huelle_on;
  Widget L_deform;
  Widget RC_deform;
  Widget TB_huelle;
  Widget TB_modell;
  Widget TB_stroemung;
  Widget L_st_faktor;
  Widget T_st_faktor;
  Widget AB_st_faktor_up;
  Widget AB_st_faktor_down;
  Widget L_darstellung;
  Widget RC_stroem;
  Widget TB_pfeile;
  Widget TB_tracer;
  Widget TB_red_pfeile;
  Widget TB_rollen;
}t_widget_vektor;

/* Struktur fuer Widgets im xy_diagramm-Fenster **/
typedef struct widget_xy_diagramm
{
  Widget S_xy_diagramm;
  Widget XmForm;
  Widget PB_drucken;
  Widget PB_abbrechen;
  Widget PB_hilfe;
  Widget PM_dateien;
  Widget OM_dateien;
  Widget DA_xy_diagramm;
}t_widget_xy_diagramm;

/* Struktur fuer Widgets im loesungs-Fenster **/
typedef struct widget_loesung
{
  Widget S_loesung;
  Widget XmForm;
  Widget L_animation;
  Widget L_ani_anfang;
  Widget L_ani_ende;
  Widget T_ani_anfang;
  Widget T_ani_ende;
  Widget T_text;
  Widget T_parameter;
  Widget T_max;
  Widget T_min;
  Widget T_dimens;
  Widget PB_ok;
  Widget PB_ani_anfang;
  Widget PB_ani_ende;
  Widget PB_schliessen;
  Widget PB_hilfe;
}t_widget_loesung;

/* Struktur fuer Colormapwidget **/
typedef struct widget_cmap
{
  Widget S_cmap;
  Widget XmForm;
}t_widget_cmap;

/* Struktur fuer Pickingwidget **/
typedef struct widget_pick
{
  Widget S_pick;
  Widget XmForm;
}t_widget_pick;

/*************************************************************************/
#endif   /* #ifndef _widgets_h */
