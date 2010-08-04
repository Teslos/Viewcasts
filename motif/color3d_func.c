/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   color3d_func.c                                   */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*  main_cb                      |                               */
/*  initmainW                    |                               */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */
#include "color3d.h"
#include "motif.h"
#include "widgets.h"
#include "grafik.h"
#include "createmovie.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>
#include <Xm/DialogS.h>
#include <Xm/MessageB.h>
#include <Xm/ToggleB.h>
#include <Xm/SelectioB.h>
#include <Xm/TextF.h>
#ifdef __sgi
#include <Sgm/ThumbWheel.h>
#endif
#include "zoom.h"

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define MAIN_DATEI_NEU                   100
#define MAIN_DATEI_GEOP_LESEN            101
#define MAIN_DATEI_UNV_LESEN             102
#define MAIN_DATEI_LOESUNG_LESEN         103
#define MAIN_DATEI_SPANNUNG_LESEN        107
#define MAIN_DATEI_STROEMUNG_LESEN       108
#define MAIN_DATEI_BILD_LESEN            104
#define MAIN_DATEI_BILD_SCHREIBEN        105
#define MAIN_DATEI_OBJECT_LESEN          200
#define MAIN_DATEI_OBJECT_SCHREIBEN      201
#define MAIN_DATEI_SNAPSHOT              106
#define MAIN_DATEI_QUIT                  199

#define MAIN_ANIMATION_START             801
#define MAIN_ANIMATION_STOP              802
#define MAIN_ZEITSCHRITT                 803
#define MAIN_ZEITSCHRITT_SELECT          804
#define MAIN_ZEITSCHRITT_VOR             805
#define MAIN_ZEITSCHRITT_ZURUECK         806
#define MAIN_ZEITSCHRITT_START           807
#define MAIN_ZEITSCHRITT_ENDE            808
#define MAIN_EVERY_FRAME                 809

#define MAIN_ROT_MINUS_X                 901
#define MAIN_ROT_PLUS_X                  902
#define MAIN_ROT_MINUS_Y                 903
#define MAIN_ROT_PLUS_Y                  904
#define MAIN_ROT_MINUS_Z                 905
#define MAIN_ROT_PLUS_Z                  906
#define MAIN_ROT_MINUS_XX                907
#define MAIN_ROT_PLUS_XX                 908
#define MAIN_ROT_MINUS_YY                909
#define MAIN_ROT_PLUS_YY                 910
#define MAIN_ROT_MINUS_ZZ                911
#define MAIN_ROT_PLUS_ZZ                 912
#define MAIN_ROT_ENTER_X                 913
#define MAIN_ROT_ENTER_Y                 914
#define MAIN_ROT_ENTER_Z                 915
#define MAIN_MOVE_MINUS_X                921
#define MAIN_MOVE_PLUS_X                 922
#define MAIN_MOVE_PLUS_Y                 923
#define MAIN_MOVE_MINUS_Y                924
#define MAIN_MOVE_PLUS_Z                 925
#define MAIN_MOVE_MINUS_Z                926
#define MAIN_ZOOM_MANUELL                931
#define MAIN_ZOOM_AUTO                   932
#define MAIN_ZOOM_14X                    933
#define MAIN_ZOOM_07X                    934
#define MAIN_RESET                       935

#define MAIN_ALLESTOFFE_AN               941
#define MAIN_ALLESTOFFE_AUS              942


#define MAIN_STOFFE                     2000
#define MAIN_TRANSFORMATIONEN           3000
#define MAIN_2D_GRAFIK                  4000
#define MAIN_ISOLINIEN                  5000
#define MAIN_STROEM_SPANN               6000
#define MAIN_EXTRA_COLORS               6500
#define MAIN_EXTRA_SHADING              6600
#define MAIN_LOESUNG                    7000
#define MAIN_CUT                        7500


#define TRUE   1
#define FALSE  0

#define TEMPERATUR 1
#define SPANNUNG   2
#define STROEMUNG  3

/* Functions declarations */
static void ValueChangedCallback(Widget w, XtPointer clientData, XtPointer callData);
static void zoomInArmCallback(Widget w, XtPointer clientData, XtPointer callData);
#ifdef __sgi
static void ValChangedCallback( Widget w, XtPointer clientData, XtPointer callData );
static void DragCallback( Widget w, XtPointer clientData, XtPointer callData );
#endif

/* Routinen :                                                    */
/* ============================================================= */

/*****************************************************************/
void main_cb(Widget widget, XtPointer client_data, XtPointer call_data)
/*****************************************************************/
{
  XmString xmstring = NULL;
  XmString zeile = (XmString)NULL;
  static char message_text[4096];
  static char hilfstr[256];
  static char hilfstr2[256];
  char *hilfstr3;
  int result, laenge;
  int test_param;
  char *ext, *file_name;
  int  format, stereo;
  char file_dir_name[256];
  char file_dir_name2[256];
  float zeit;
  char c_zeit[10];
  char c_hilf[10];
  int index;
  Widget liste;
  Dimension width, height;
  Arg args[10];
  Cardinal anzargs;
  int i;
  static int answer;
  XmListCallbackStruct cbs;
  /* ************************************************************* */

  switch ((int)client_data)
    {
    case MAIN_DATEI_UNV_LESEN:
      {
	/* falls applRessourcen.unv_dir belegt ist ----------------------------- */
	if (strlen(applRessourcen.unv_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.unv_dir);
	strcat(hilfstr, "*.unv\0");

	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines Universal-Files", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a Universal-File", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "Universalfile = >%s<\n", file_dir_name);
#endif
	if (file_dir_name && strlen(file_dir_name))
	  {
	    /* Universalfile lesen */
	    set_watch_cursor_handler(toplevel);
	    
	    /*result = read_unv(file_dir_name); */
	    b_geo_gelesen = TRUE;
	    
	    if (result)
	      {
		sprintf(hilfstr, "Error: read_unv(>%s<) returns value = >%d<\n", 
			file_dir_name, result);
		warning(hilfstr);
		reset_cursor_handler(toplevel);
		break;
		if ((result = init_data(FALSE)))
		  {
		    fprintf(stderr, "init_data() return = >%d<\n", result);
		    warning("Error: Initialization");
		  }
	      }

	    /* Platz fuer g_Maske bereitstelle: Maske fuer grafische Darstellung --- */
	    if (g_Maske != NULL) free (g_Maske);
	    g_Maske=(int*)malloc((g_geodat.nstoff+1)*sizeof(int));
	    set_maske(SETZE_ALLE, 0, ZEICHNE_SOLID_NETZ);

	    reset_cursor_handler(toplevel);
	
	    XtSetSensitive(widgetMain.PB_neu,            TRUE);
	    XtSetSensitive(widgetMain.PB_geop_lesen,    FALSE);
	    XtSetSensitive(widgetMain.PB_unv_lesen,     FALSE);
	    XtSetSensitive(widgetMain.PB_loes_lesen,     TRUE);
	    XtSetSensitive(widgetMain.PB_spannung_lesen, TRUE);
	    XtSetSensitive(widgetMain.PB_stroemung_lesen,TRUE);
	    XtSetSensitive(widgetMain.CB_stoffe,         TRUE);
	    XtSetSensitive(widgetMain.PB_bild_lesen,    FALSE);
	    XtSetSensitive(widgetMain.PB_bild_schreiben,FALSE);

	    /* Belegung von x,y,z min/max der Stoffgebiete ------------------------- */
	  }
	strcpy(geo_datei_name, file_dir_name);
	geop_unv_read = 2;

	b_zeichnen = TRUE;         /* ab jetzt kann gezeichnet werden ---- */
	C3D_postredisplay();  /* Aufruf fuer Darstellung  ----------- */
	break;
      }
    case MAIN_DATEI_NEU:
      {
	/* zerstoere Stoffbuttons im Hauptfenster ------------------------------ */
	destroy_stoffbuttons();
	destroy_timelist();

	/* reinitialisierung der Flaggen und Feldpointer fuer Neueinlesen ------ */
	if (init_data(FALSE))
	  {
	    fprintf(stderr, "init_data() return = >%d<\n", result);
	    warning("Error: Initialization");
	    break;
	  }
 
	XtSetSensitive(widgetMain.PB_neu,            FALSE);
	XtSetSensitive(widgetMain.PB_geop_lesen,      TRUE);
	XtSetSensitive(widgetMain.PB_unv_lesen,      FALSE);
	XtSetSensitive(widgetMain.PB_loes_lesen,     FALSE);
	XtSetSensitive(widgetMain.PB_spannung_lesen, FALSE);
	XtSetSensitive(widgetMain.PB_stroemung_lesen,FALSE);
	XtSetSensitive(widgetMain.CB_stoffe,         FALSE);
	XtSetSensitive(widgetMain.CB_2dgrafik,       FALSE);
	XtSetSensitive(widgetMain.CB_stroem_spann,   FALSE);
	XtSetSensitive(widgetMain.PB_bild_lesen,     FALSE);
	XtSetSensitive(widgetMain.PB_bild_schreiben, FALSE);

	break;
      }
    case MAIN_DATEI_GEOP_LESEN:
      {
	/* falls applRessourcen.casts_geo_dir belegt ist ----------------------- */
	if (strlen(applRessourcen.casts_geo_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.casts_geo_dir);
	strcat(hilfstr, "*.geop\0");

	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines CASTS-Geometrie-Files", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a CASTS-Geometry-File", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "CASTS-Geometrie-File = >%s<\n", file_dir_name);
#endif
	if (file_dir_name && strlen(file_dir_name))
	  {
	    /* "geop"-file lesen --------------------------------------------------- */
	    set_watch_cursor_handler(toplevel);

	    result = read_geop(file_dir_name);
	    b_geo_gelesen = TRUE;

	    if (result == -1)
	      sprintf(hilfstr, "Error: read_geop(>%s<) returns value = >%d<\n", 
		      "Error when opening", result);
	    if (result == -2)
	      sprintf(hilfstr, "Error:read_geop(>%s<) returns value = >%d<\n", 
		      "Insufficient memory", result);
	    if (result == -3)
	      sprintf(hilfstr, "Error:read_geop(>%s<) returns value = >%d<\n", 
		      "No regular file ", result);
	    if (result)
	      {
		warning(hilfstr);
		reset_cursor_handler(toplevel);
		break;
		if ((result = init_data(FALSE)))
		  {
		    fprintf(stderr, "init_data() return = >%d<\n", result);
		    warning("Error: Initialization");
		  }
	      }

	    /* Platz fuer g_Maske bereitstelle: Maske fuer grafische Darstellung --- */
	    if (g_Maske != NULL) free (g_Maske);
	    g_Maske=(int*)malloc((g_geodat.nstoff + 1)*sizeof(int));

	    set_maske(SETZE_ALLE, 0, ZEICHNE_RAHMEN);
	    for(i=0; i<g_geodat.nstoff; g_stoffgebiete[i++].zeichne_on=TRUE);

	    XtSetSensitive(widgetMain.PB_neu,             TRUE);
	    XtSetSensitive(widgetMain.PB_geop_lesen,     FALSE);
	    XtSetSensitive(widgetMain.PB_unv_lesen,      FALSE);
	    XtSetSensitive(widgetMain.PB_loes_lesen,      TRUE);
	    XtSetSensitive(widgetMain.PB_spannung_lesen,  TRUE);
	    XtSetSensitive(widgetMain.PB_stroemung_lesen, TRUE);
	    XtSetSensitive(widgetMain.PB_bild_lesen,      TRUE);
	    XtSetSensitive(widgetMain.PB_bild_schreiben, FALSE);
	    XtSetSensitive(widgetMain.CB_stoffe,          TRUE);
	    XtSetSensitive(widgetMain.CB_transformieren,  TRUE);
	    XtSetSensitive(widgetMain.CB_isolinien,       TRUE);
	    XtSetSensitive(widgetMain.CB_2dgrafik,        TRUE);
	    XtSetSensitive(widgetMain.CB_stroem_spann,    TRUE);
	    XtSetSensitive(widgetMain.PB_alleStoffe_an,   TRUE);
	    XtSetSensitive(widgetMain.PB_alleStoffe_aus,  TRUE);

	    strcpy(geo_datei_name, file_dir_name);
	    geop_unv_read = 1;
     
	    if (init_stoffbuttons() == -1);

	    b_zeichnen = TRUE;         /* ab jetzt kann gezeichnet werden -- */
	    C3D_postredisplay();  /* Aufruf fuer Darstellung ---------- */
	    reset_cursor_handler(toplevel);
	  }
	break;
      }
    case MAIN_DATEI_LOESUNG_LESEN:
      {
       destroy_timelist();
	/* falls applRessourcen.casts_loes_dir belegt ist ---------------------- */
	if (strlen(applRessourcen.casts_loes_dir))
	  {
	    sprintf(hilfstr, "%s/", applRessourcen.casts_loes_dir);
	    strcpy(hilfstr2, geo_datei_name);
	    strtok(hilfstr2, ".");
	    strcat(hilfstr2, ".*\0");
	    hilfstr3 = strrchr(hilfstr2, 47);
	    if (hilfstr3)
	      strcpy(hilfstr2, &hilfstr3[1]);
	    strcat(hilfstr, hilfstr2);
	  }
	else
	  {
	    strcpy(hilfstr, geo_datei_name);
	    strtok(hilfstr, ".");
	    strcat(hilfstr, ".*\0");
	  }

	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Auswahl eines CASTS-Loesungs-Files", "Auswahl Scalar Loesung",
				  hilfstr, FALSE, FALSE, TRUE));
	else
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Selection of a CASTS-Result-File", "Select Scalar Solution",
				  hilfstr, FALSE, FALSE, TRUE));
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "CASTS-Loesungs-File = >%s<\n", file_dir_name);
#endif
	if (file_dir_name && strlen(file_dir_name))
	  {
	    /* Loesung einlesen ---------------------------------------------------- */
	    set_watch_cursor_handler(toplevel);
	    
	    result = lese_loesung(file_dir_name);
	    
	    if (g_loesung.mmap)
	      result = mmread_res_scalar(file_dir_name);

	    if (result == -1)
	      warning("Error when opening solution file");
	    if (result == -2)
	      warning("Error: Insufficient memory");
	    if (result == -3)
	      warning("Error: Not a  regular solution file");
	    if (result == -4)
	      warning("Error: Incorrect format or empty solution file");
	    if (result == -5)
	      warning("Error: Solution does not belong to geometry");
            if (result)
		reset_cursor_handler( toplevel );
	    if (!result)
	      {
		strcpy(tmp_datei_name, file_dir_name);
		strtok(tmp_datei_name, ".");
		strcat(tmp_datei_name, ".hflap");

		g_ofen.ex = FALSE;
		result = lese_absenkung(tmp_datei_name);

		if (!result)
		  {
		    g_ofen.ex = TRUE;
		    strcpy(g_ofen.dateiname, tmp_datei_name);
		  }
		if (result == 1)
		  sprintf(hilfstr, "Fehler in lese_absenkung(>%s<) Return-Wert = >%d<\n", 
			  "Fehler beim Oeffnen", result);
		if (result == 2)
		  sprintf(hilfstr, "Datei passt nicht zur Geometrie Return-Wert = >%d<\n", result);

		reset_cursor_handler(toplevel);

		XtSetSensitive(widgetMain.PB_animation_start,     TRUE);
		XtSetSensitive(widgetMain.PB_animation_stop,     FALSE);
		XtSetSensitive(widgetMain.PB_zeitschritt,         TRUE);
		XtSetSensitive(widgetMain.T_zeitschritt,          TRUE);
		XtSetSensitive(widgetMain.T_zeitschritt_start,    TRUE);
		XtSetSensitive(widgetMain.T_zeitschritt_end,      TRUE);
		XtSetSensitive(widgetMain.PB_zeitschritt_vor,     TRUE);
		XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, TRUE);
		XtSetSensitive(widgetMain.PB_bild_schreiben,      TRUE);
		
		init_loesstruct();
		lese_feld_ein(g_animation.akt_index);
		
		/* get local min. and max. */
		C3D_localminmax( &g_loesung.Max, &g_loesung.Min );
		set_maske(SETZE_ALLE, 0, ZEICHNE_RESULT);
		/*
		  erst abfragen ob widget existiert -------------------------------
		  XtSetSensitive(widget_stoffe.TB_result, TRUE);
		*/

		C3D_postredisplay();  /* Aufruf fuer Darstellung */
		if (g_tabelle.on) cmap_Redraw();
	      }
	  }
	break;
      }
    case MAIN_DATEI_SPANNUNG_LESEN:
      {
	/* falls applRessourcen.casts_loes_dir belegt ist ---------------------- */
	if (strlen(applRessourcen.casts_loes_dir))
	  {
	    sprintf(hilfstr, "%s/", applRessourcen.casts_loes_dir);
	    strcpy(hilfstr2, geo_datei_name);
	    strtok(hilfstr2, ".");
	    strcat(hilfstr2, ".disp*\0");
	    hilfstr3 = strrchr(hilfstr2, 47);
	    if (hilfstr3)
	      strcpy(hilfstr2, &hilfstr3[1]);
	    strcat(hilfstr, hilfstr2);
	  }
	else
	  {
	    strcpy(hilfstr, geo_datei_name);
	    strtok(hilfstr, ".");
	    strcat(hilfstr, ".disp*\0");
	  }

	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Auswahl eines CASTS-Vektor-Loesungs-Files", "Auswahl Vektor Loesungs",
				  hilfstr, FALSE, FALSE, TRUE));
	else
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Selection of a CASTS-Vektor-Result-File", "Select Vector Solution", 
				  hilfstr, FALSE, FALSE, TRUE));
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "CASTS-Vektor-File = >%s<\n", file_dir_name);
#endif
	if (file_dir_name && strlen(file_dir_name))
	  {
	    /* Spannungsloesung einlesen ------------------------------------------- */
	    set_watch_cursor_handler(toplevel);

	    i = strlen(file_dir_name);
	    strcpy(hilfstr, "dispp");
	    if (!strcmp(&file_dir_name[i - 5], hilfstr))
	      g_spannung.on = TRUE;
	    

	    /* Speicherplatz fuer Vektorfeld allocieren ---------------------------- */
	    if (g_spannung.verzerrung) free(g_spannung.verzerrung);
	    if ((g_spannung.verzerrung = (t_knoten *)
                 malloc(g_geodat.npunkt * sizeof(t_knoten))) == NULL)
	      {
		warning("Error: Insufficient memory");
		return;
	      }

	    if (g_spannung.vektor) free (g_spannung.vektor);
	    if ((g_spannung.vektor = (t_koord *)
                 malloc(g_geodat.npunkt * sizeof(t_koord))) == NULL)
	      {
		warning("Error: Insufficient memory");
		return;
	      }
	    result = lese_vektorloesung(file_dir_name, SPANNUNG);
	    reset_cursor_handler(toplevel);

	    if (result == -1)
	      warning("Error when opening solution file");
	    if (result == -2)
	      warning("Error: Insufficient memory ");
	    if (result == -3)
	      warning("Error: Not a regular solution file");
	    if (result == -4)
	      warning("Error: Incorrect format or file is empty");
	    if (result == -5)
	      warning("Error: Solution does not belong to geometry");

	    if (result) return;

	    XtSetSensitive(widgetMain.PB_animation_start,     TRUE);
	    XtSetSensitive(widgetMain.PB_animation_stop,     FALSE);
	    XtSetSensitive(widgetMain.PB_zeitschritt,         TRUE);
	    XtSetSensitive(widgetMain.T_zeitschritt,          TRUE);
	    XtSetSensitive(widgetMain.PB_zeitschritt_vor,     TRUE);
	    XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, TRUE);

	    strcpy(g_spannung.dateiname, file_dir_name);

	    /* erst abfragen ob widget existiert -------------------------------
	       XtSetSensitive(widget_stoffe.TB_result, TRUE);
	    */
	    C3D_postredisplay();  /* Aufruf fuer Darstellung */
	  }
	break;
      }

    case MAIN_DATEI_STROEMUNG_LESEN:
      {
	      /* falls applRessourcen.casts_loes_dir belegt ist ---------------------- */
	      if (strlen(applRessourcen.casts_loes_dir))
	      {
		      sprintf(hilfstr, "%s/", applRessourcen.casts_loes_dir);
		      strcpy(hilfstr2, geo_datei_name);
		      strtok(hilfstr2, ".");
		      strcat(hilfstr2, ".strom*\0");
		      hilfstr3 = strrchr(hilfstr2, 47);
		      if (hilfstr3)
			      strcpy(hilfstr2, &hilfstr3[1]);
		      strcat(hilfstr, hilfstr2);
	      }
	      else
	      {
		      strcpy(hilfstr, geo_datei_name);
		      strtok(hilfstr, ".");
		      strcat(hilfstr, ".strom*\0");
	      }

	      if (g_sprache == SPRACHE_DEUTSCH)
		      strcpy(file_dir_name, 
				      file_dir_auswahl("Auswahl eines CASTS-Vektor-Loesungs-Files", 
						" Auswahl Vektor Loesungs",
					      	hilfstr, FALSE, FALSE, TRUE));
	      else
		      strcpy(file_dir_name, 
				      file_dir_auswahl("Selection of a CASTS-Vector-Result-File", 
						"Select Vector Solution", 
					      	hilfstr, FALSE, FALSE, TRUE));
#ifdef DEBUG
	      if (applRessourcen.debug)
		      fprintf (stdout, "CASTS-Vektor-File = >%s<\n", file_dir_name);
#endif
	      if (file_dir_name && strlen(file_dir_name))
	      {
		      char tempstr[2][256];
		      int s;
		      /* Loesung einlesen */
		      set_watch_cursor_handler(toplevel);

		      i = strlen(file_dir_name);
		      strcpy(tempstr[0], "strom");
		      strcpy(tempstr[1], "vel");

		      for ( s = 0; s < 2; s ++ )
		      {	
			       if (!strcmp(SYS_extension( file_dir_name ), tempstr[s]))
				      g_stroemung.on = TRUE;
		      }
		      /* Speicherplatz fuer Vektorfeld allocieren ---------------------------- */
		      if (g_stroemung.vektor) free (g_stroemung.vektor);
		      g_stroemung.vektor = 
			      (t_koord *)malloc(g_geodat.npunkt * sizeof(t_koord)); 
		      if(g_stroemung.vektor == NULL)
		      {
			      warning("Error: Insufficient memory");
			      g_stroemung.on = FALSE;
			      return;
		      }
		      result = lese_vektorloesung(file_dir_name, STROEMUNG);
		      reset_cursor_handler(toplevel);

		      if (result == -1)
			      warning("Error when opening solution file");
		      if (result == -2)
			      warning("Error: Insufficient memory");
		      if (result == -3)
			      warning("Error: Solution is not a regular file");
		      if (result == -4)
			      warning("Error: Incorrect format or file is empty");
		      if (result == -5)
			      warning("Error: Solution does not belong to geometry");

		      if (result)
		      {
			      g_stroemung.on = FALSE;
			      return;
		      }

		      XtSetSensitive(widgetMain.PB_animation_start,     TRUE);
		      XtSetSensitive(widgetMain.PB_animation_stop,     FALSE);
		      XtSetSensitive(widgetMain.PB_zeitschritt,         TRUE);
		      XtSetSensitive(widgetMain.T_zeitschritt,          TRUE);
		      XtSetSensitive(widgetMain.PB_zeitschritt_vor,     TRUE);
		      XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, TRUE);

		      strcpy(g_stroemung.dateiname, file_dir_name);

		      /* erst abfragen ob widget existiert -------------------------------
			 XtSetSensitive(widget_stoffe.TB_result, TRUE);
		       */
		      C3D_postredisplay();  /* Aufruf fuer Darstellung */
	      }
	      break;
      }

    case MAIN_DATEI_BILD_LESEN:
      {
	/* falls applRessourcen.casts_geo_dir belegt ist ----------------------- */
	if (strlen(applRessourcen.casts_geo_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.casts_geo_dir);
	strcat(hilfstr, "*.img\0");
	
	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines Image-Files", NULL,
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a Image-File", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));

	if (file_dir_name && strlen(file_dir_name))
	  {
	    int result;
	    t_bool  b_img_read;

	    /* read image file inside the structure */
	    result = read_img(file_dir_name);

	    C3D_postredisplay();
	    /* errors codes */
	    /* set flag for image read */
	    b_img_read = TRUE;
	  }

	break;
      }
     case MAIN_DATEI_BILD_SCHREIBEN:
      {
	/* falls applRessourcen.casts_geo_dir belegt ist ----------------------- */
	if (strlen(applRessourcen.casts_geo_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.casts_geo_dir);
	strcat(hilfstr, "*.img\0");
	
	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines Image-Files", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a Image-File", NULL, 
						 hilfstr, FALSE, FALSE, FALSE));

	if (file_dir_name && strlen(file_dir_name))
	  {
	    int result;

	    /* write image file on disk */
	    result = write_img(file_dir_name);

	    C3D_postredisplay();
	    /* errors codes ????*/
	  }

	break;
      } 
    case MAIN_DATEI_OBJECT_LESEN:
      {
	/* falls applRessourcen.casts_geo_dir belegt ist ----------------------- */
	if (strlen(applRessourcen.casts_geo_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.casts_geo_dir);
	strcat(hilfstr, "*.sgo\0");
	
	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines Object-Files",NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a Object-File",NULL, 
						 hilfstr, FALSE, FALSE, FALSE));

	if (file_dir_name && strlen(file_dir_name))
	  {
	    int result;

	    /* write image file on disk */
	    result = read_object(file_dir_name);
	  }

	break;
      }
    case MAIN_DATEI_OBJECT_SCHREIBEN:
      {
	/* falls applRessourcen.casts_geo_dir belegt ist ----------------------- */
	if (strlen(applRessourcen.casts_geo_dir))
	  sprintf(hilfstr, "%s/", applRessourcen.casts_geo_dir);
	strcat(hilfstr, "*.sgo\0");
	
	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, file_dir_auswahl("Auswahl eines Object-Files",NULL, 
						 hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, file_dir_auswahl("Selection of a Object-File",NULL, 
						 hilfstr, FALSE, FALSE, FALSE));

	if (file_dir_name && strlen(file_dir_name))
	  {
	    int result;

	    /* write image file on disk */
	    result = draw_object(GL_TRUE, file_dir_name);
	    
	    /* errors codes ????*/
	  }
	break;
      }	
    case MAIN_DATEI_SNAPSHOT:
      {
	if (g_sprache == SPRACHE_DEUTSCH)
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Bild schreiben",NULL,
				  hilfstr, FALSE, FALSE, FALSE));
	else
	  strcpy(file_dir_name, 
		 file_dir_auswahl("Save picture",NULL, 
				  hilfstr, FALSE, FALSE, FALSE));
	
	/* get format of picture */
	strcpy(hilfstr, file_dir_name);
	file_name = strtok(hilfstr, ".");
	ext       = strtok(NULL, ".");
	if (ext == NULL) 
	  warning("Please assign one of these extensions:\ntga, rgb, raw, tif, eps, bmp\n");
        	
	if     (!strcmp(ext,"tga")) format = 1;
	else if(!strcmp(ext,"ppm")) format = 2;
	else if(!strcmp(ext,"rgb")) format = 3; 
	else if(!strcmp(ext,"raw")) format = 4; 
	else if(!strcmp(ext,"tif")) format = 5; 
	else if(!strcmp(ext,"eps")) format = 6; 
	else if(!strcmp(ext,"eps")) format = 7;
	else if(!strcmp(ext,"raw")) format = 8;  
	else if(!strcmp(ext,"bmp")) format = 9;  
	
	/* get the height and width of picture */ 
	XtVaGetValues(C3D_win.wid,XmNheight, &height, 
		XmNwidth, &width, NULL); 
	/* redraw whole screen */
	C3D_postredisplay();
	glFlush();
	/* write picture in file */
	WindowDump(width, height, 0, format, file_name);
	break;
      }
    case MAIN_DATEI_QUIT:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_DATEI_QUIT\n");
#endif
   if (g_sprache == SPRACHE_DEUTSCH)
   {
	  strcpy(hilfstr,
	       ask_user("Wollen Sie ViewCASTS wirklich beenden ?", "Ok", "Abbrechen", "", 2));
   }
   else
   {
      strcpy(hilfstr,
           ask_user("Do you really wish to quit ViewCASTS ?", "Ok", "Cancel", "", 2));
   }
	if (!strcmp(hilfstr, "Ok"))
	  {
	    if ((result = init_data(FALSE)))
	      {
		fprintf(stderr, "init_data() return = >%d<\n", result);
		exit(1);
	      }
	    exit(0);
	  }
	break;
      }

    case MAIN_ANIMATION_START:
      {
	XtSetSensitive(widgetMain.PB_animation_start, FALSE);
	XtSetSensitive(widgetMain.PB_animation_stop,  TRUE);
	animation_start();
	break;
      }
 
    case MAIN_ANIMATION_STOP:
      {
	XtSetSensitive(widgetMain.PB_animation_stop, FALSE);
	XtSetSensitive(widgetMain.PB_animation_start, TRUE);
	animation_stop();
	break;
      }
    case MAIN_ZEITSCHRITT_START:
      {
	zeit = atof(XmTextFieldGetString(widgetMain.T_zeitschritt_start));
	g_animation.anim_anfang = search_next_zeitschritt(&zeit);
	sprintf(c_zeit, "%8.3f", zeit);
	XmTextFieldSetString(widgetMain.T_zeitschritt_start, c_zeit);
	break;
      }
    case MAIN_ZEITSCHRITT_ENDE:
      {
	zeit = atof(XmTextFieldGetString(widgetMain.T_zeitschritt_end));
	g_animation.anim_ende = search_next_zeitschritt(&zeit);
	if ( g_animation.anim_ende <= g_animation.anim_anfang )
	  {
	    warning("Animation end time is smaller than animation start time.");
	    break;
	  }
	sprintf(c_zeit, "%8.3f", zeit);
	XmTextFieldSetString(widgetMain.T_zeitschritt_end, c_zeit);
	break;
      }
    case MAIN_EVERY_FRAME:
      {
	char c_every_frame[10];
	int every_frame = atoi(XmTextFieldGetString(widgetMain.T_every_frame));
	g_animation.anim_frame = every_frame;
	sprintf(c_every_frame, "%i", every_frame);
	printf("Every frame : %i\n", every_frame);
	XmTextFieldSetString(widgetMain.T_every_frame, c_every_frame);
	break;
      }
    case MAIN_ZEITSCHRITT:
      {
	zeit = atof(XmTextFieldGetString(widgetMain.T_zeitschritt));
	g_animation.akt_index = search_next_zeitschritt(&zeit);
	sprintf(c_zeit, "%8.3f", zeit);
	XmTextFieldSetString(widgetMain.T_zeitschritt, c_zeit);
	if (g_animation.temp_on)
	  result = lese_feld_ein(g_animation.akt_index);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	if (g_animation.spann_on)
	  result = lese_feld_ein(g_animation.akt_index);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	if (g_animation.stroem_on)
	  result = lese_feld_ein(g_animation.akt_index);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	C3D_postredisplay();    /* neuzeichnen -------------------- */
	break;
      }
 
    case MAIN_ZEITSCHRITT_SELECT:
      {
	g_animation.akt_index = loes_auswahl(widget, client_data, call_data) - 1;
	sprintf(c_zeit, "%8.3f", g_animation.zeiten[g_animation.akt_index]);
	XmTextFieldSetString(widgetMain.T_zeitschritt, c_zeit);
	result = 0;
	if (g_animation.temp_on)
	  result = lese_feld_ein(g_animation.akt_index);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	if (g_animation.spann_on)
	  result = lese_vektor(g_animation.akt_index, 2);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	if (g_animation.stroem_on)
	  result = lese_vektor(g_animation.akt_index, 3);
	if (result)
	  {
	    warning(" Error when reading solution");
	    break;
	  }
	C3D_updateref();
	C3D_postredisplay();    /* neuzeichnen -------------------- */
	break;
      }
 
    case MAIN_ZEITSCHRITT_VOR:
      {
	if (get_next_zeitschritt() == 0)
	  {
	    sprintf(c_zeit,"%8.3f",g_animation.zeiten[g_animation.akt_index]);
	    XmTextFieldSetString(widgetMain.T_zeitschritt, c_zeit);
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
 
    case MAIN_ZEITSCHRITT_ZURUECK:
      {
	if (get_previous_zeitschritt() == 0)
	  {
	    sprintf(c_zeit,"%8.3f",g_animation.zeiten[g_animation.akt_index]);
	    XmTextFieldSetString(widgetMain.T_zeitschritt, c_zeit);
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_ROT_PLUS_X:
      {
	g_transform.rotx = g_transform.rotx + 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotx);
	XmTextFieldSetString(widgetMain.RX_rot_x, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_PLUS_XX:
      {
	g_transform.rotx = g_transform.rotx + 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotx);
	XmTextFieldSetString(widgetMain.RX_rot_x, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_X:
      {
	g_transform.rotx = g_transform.rotx - 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotx);
	XmTextFieldSetString(widgetMain.RX_rot_x, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_XX:
      {
	g_transform.rotx = g_transform.rotx - 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotx);
	XmTextFieldSetString(widgetMain.RX_rot_x, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_PLUS_Y:
      {
	g_transform.roty = g_transform.roty + 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.roty);
	XmTextFieldSetString(widgetMain.RX_rot_y, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_PLUS_YY:
      {
	g_transform.roty = g_transform.roty + 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.roty);
	XmTextFieldSetString(widgetMain.RX_rot_y, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_Y:
      {
	g_transform.roty = g_transform.roty - 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.roty);
	XmTextFieldSetString(widgetMain.RX_rot_y, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_YY:
      {
	g_transform.roty = g_transform.roty - 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.roty);
	XmTextFieldSetString(widgetMain.RX_rot_y, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_PLUS_Z:
      {
	g_transform.rotz = g_transform.rotz + 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotz);
	XmTextFieldSetString(widgetMain.RX_rot_z, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_PLUS_ZZ:
      {
	g_transform.rotz = g_transform.rotz + 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotz);
	XmTextFieldSetString(widgetMain.RX_rot_z, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_Z:
      {
	g_transform.rotz = g_transform.rotz - 15.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotz);
	XmTextFieldSetString(widgetMain.RX_rot_z, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_MINUS_ZZ:
      {
	g_transform.rotz = g_transform.rotz - 45.0;
	sprintf(c_hilf, "%i", (int) g_transform.rotz);
	XmTextFieldSetString(widgetMain.RX_rot_z, c_hilf);
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_ENTER_X:
      {
	fprintf (stdout, "Rotation in main_cb() | client_data = >%d<\n", (int)client_data);
	fprintf (stdout, "Wert = >%s<\n", XmTextFieldGetString(widget));
	g_transform.rotx = atof(XmTextFieldGetString(widget));
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_ENTER_Y:
      {
	fprintf (stdout, "Rotation in main_cb() | client_data = >%d<\n", (int)client_data);
	fprintf (stdout, "Wert = >%s<\n", XmTextFieldGetString(widget));
	g_transform.roty = atof(XmTextFieldGetString(widget));
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_ROT_ENTER_Z:
      {
	fprintf (stdout, "Rotation in main_cb() | client_data = >%d<\n", (int)client_data);
	fprintf (stdout, "Wert = >%s<\n", XmTextFieldGetString(widget));
	g_transform.rotz = atof(XmTextFieldGetString(widget));
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }

    case MAIN_MOVE_MINUS_X:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[0] = 
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[0]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[0] = g_transform.move[0]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_MOVE_PLUS_X:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[0] = 
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[0]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[0] = g_transform.move[0]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_MOVE_PLUS_Y:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[1] =
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[1]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[1] = g_transform.move[1]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_MOVE_MINUS_Y:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[1] =
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[1]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[1] = g_transform.move[1]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_MOVE_PLUS_Z:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[2] =
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[2]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[2] = g_transform.move[2]+g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	break;
      }
    case MAIN_MOVE_MINUS_Z:
      {
	if (verschiebung_on)
	  {
	    g_stoffgebiete[verschiebe_stoff-1].verschiebung[2] =
	      g_stoffgebiete[verschiebe_stoff-1].verschiebung[2]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen -------------------- */
	  }
	else
	  {
	    g_transform.move[2] = g_transform.move[2]-g_schritt*10.0;
	    C3D_postredisplay();    /* neuzeichnen ------------------ */
	  }
	break;
      }

    case MAIN_ZOOM_MANUELL:
    case MAIN_ZOOM_AUTO:
    case MAIN_ZOOM_14X:
      {
	g_transform.scale = g_transform.scale * 1.4;
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }
    case MAIN_ZOOM_07X:
      {
	g_transform.scale = g_transform.scale * 0.7;
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }
    case MAIN_ALLESTOFFE_AN:
      {
	for (i = 0; i < g_geodat.nstoff; i++) 
	  {
	    g_stoffgebiete[i].zeichne_on = TRUE;
	    XmToggleButtonSetState(stoffbuttons[i], TRUE, FALSE);
	  }
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }
    case MAIN_ALLESTOFFE_AUS:
      {
	for (i = 0; i < g_geodat.nstoff; i++) 
	  {
	    g_stoffgebiete[i].zeichne_on = FALSE;
	    XmToggleButtonSetState(stoffbuttons[i], FALSE, FALSE);
	  }
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }
    case MAIN_RESET:
      {
	reset();
	C3D_postredisplay();    /* neuzeichnen ---------------------- */
	break;
      }
    case MAIN_STOFFE:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	    fprintf(stdout,"init_stoffe()\n");
#endif
	  if ((result = init_stoffe()))
	    {
	      sprintf(hilfstr, "Error: init_stoffe() returns value = >%d<\n", result);
	      warning(hilfstr);
	    }

	break;
      }
    case MAIN_TRANSFORMATIONEN:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_TRANSFORMATIONEN\n");
#endif
	if ((result = init_transformieren()))
	  {
	    sprintf(hilfstr, "Error: init_transformieren() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }
    case MAIN_2D_GRAFIK:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_2D_GRAFIK\n");
#endif
	if ((result = init_2dgrafik()))
	  {
	    sprintf(hilfstr, "Error: init_2dgrafik() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }
    case MAIN_ISOLINIEN:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_ISOLINIEN\n");
#endif
	if ((result = init_isolinien()))
	  {
	    sprintf(hilfstr, "Error: init_isolinien() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }
    case MAIN_STROEM_SPANN:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_STROEM_SPANN\n");
#endif
	if ((result = init_vektor()))
	  {
	    sprintf(hilfstr, "Error: init_vektor() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }
 
    case MAIN_EXTRA_COLORS:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_EXTRA\n");
#endif
	//if((result = init_extra_colors()))
	  {
	    sprintf(hilfstr, "Error: init_extra_colors() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }

    case MAIN_LOESUNG:
      {
#ifdef DEBUG
	if (applRessourcen.debug)
	  fprintf (stdout, "main_cb() MAIN_LOESUNG\n");
#endif
	if ((result = init_loesung()))
	  {
	    sprintf(hilfstr, "Error: init_loesung() returns value = >%d<\n", result);
	    warning(hilfstr);
	  }
	break;
      }
    default:
      {
	/* unknown callback type ----------------------------------------------- */
	fprintf (stderr, "Warning: Undefined parameter in main_cb()\n");
	fprintf (stderr, "(int)client_data = >%d<\n", (int)client_data);
	break;
      }
    }
  if (xmstring != NULL)
    XmStringFree(xmstring);
  return;
  /* ****************** ende main_cb ************************************* */
}

/* ********************************************************************* */
int init_mainW(void)
/* ********************************************************************* */
{
  int i;
  Widget widget;
  Widget shell;
  char hilfstr[1024];
  Atom wm_delete_window;

  char *widget_names []= { "PB_neu",
                           "PB_geop_lesen", "PB_unv_lesen",
                           "PB_loes_lesen", "PB_spannung_lesen",
                           "PB_stroemung_lesen",
                           "PB_bild_lesen", "PB_bild_schreiben",
			   "PB_object_lesen", "PB_object_schreiben",
                           "PB_snapshot", "PB_quit",
                           /*"PB_shading", "PB_colors",*/
                           "PB_animation_start", "PB_animation_stop",
                           "T_zeitschritt", "T_zeitschritt_start",
			   "T_zeitschritt_end","T_every_frame", "PB_zeitschritt",
                           "PB_zeitschritt_vor", "PB_zeitschritt_zurueck",
			   "PB_rot_plus_x", "PB_rot_minus_x",
                           "PB_rot_plus_xx", "PB_rot_minus_xx",
                           "PB_rot_plus_y", "PB_rot_minus_y",
                           "PB_rot_plus_yy", "PB_rot_minus_yy",
                           "PB_rot_plus_z", "PB_rot_minus_z",
                           "PB_rot_plus_zz", "PB_rot_minus_zz",
                           "RX_rot_x", "RX_rot_y", "RX_rot_z",
                           "PB_move_plus_x", "PB_move_minus_x",
                           "PB_move_plus_y", "PB_move_minus_y",
                           "PB_move_plus_z", "PB_move_minus_z",
                           "PB_zoom_14x", "PB_zoom_07x",
                           "PB_alleStoffe_an", "PB_alleStoffe_aus",
                           "PB_reset",
                           (char *)NULL};

  int w_client_data [] = {MAIN_DATEI_NEU,
                          MAIN_DATEI_GEOP_LESEN, MAIN_DATEI_UNV_LESEN,
                          MAIN_DATEI_LOESUNG_LESEN,
                          MAIN_DATEI_SPANNUNG_LESEN,
                          MAIN_DATEI_STROEMUNG_LESEN,
                          MAIN_DATEI_BILD_LESEN, MAIN_DATEI_BILD_SCHREIBEN,
			  MAIN_DATEI_OBJECT_LESEN, MAIN_DATEI_OBJECT_SCHREIBEN,
			  MAIN_DATEI_SNAPSHOT, MAIN_DATEI_QUIT,
			  /*MAIN_EXTRA_SHADING, MAIN_EXTRA_COLORS,*/
			  MAIN_ANIMATION_START, MAIN_ANIMATION_STOP,
			  MAIN_ZEITSCHRITT,MAIN_ZEITSCHRITT_START, 
			  MAIN_ZEITSCHRITT_ENDE, MAIN_EVERY_FRAME, 
			  MAIN_ZEITSCHRITT_SELECT,
			  MAIN_ZEITSCHRITT_VOR, MAIN_ZEITSCHRITT_ZURUECK,
			  MAIN_ROT_PLUS_X, MAIN_ROT_MINUS_X,
			  MAIN_ROT_PLUS_XX, MAIN_ROT_MINUS_XX,
                          MAIN_ROT_PLUS_Y, MAIN_ROT_MINUS_Y,
                          MAIN_ROT_PLUS_YY, MAIN_ROT_MINUS_YY,
			  MAIN_ROT_PLUS_Z, MAIN_ROT_MINUS_Z,
			  MAIN_ROT_PLUS_ZZ, MAIN_ROT_MINUS_ZZ,
			  MAIN_ROT_ENTER_X, MAIN_ROT_ENTER_Y,
			  MAIN_ROT_ENTER_Z,
			  MAIN_MOVE_PLUS_X, MAIN_MOVE_MINUS_X,
                          MAIN_MOVE_PLUS_Y, MAIN_MOVE_MINUS_Y,
                          MAIN_MOVE_PLUS_Z, MAIN_MOVE_MINUS_Z,
                          MAIN_ZOOM_14X, MAIN_ZOOM_07X,
                          MAIN_ALLESTOFFE_AN, MAIN_ALLESTOFFE_AUS,
                          MAIN_RESET,
		  	  (int)NULL};
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_mainW() ***\n");
#endif
  /* window-manager delete-button ---------------------------------------- */
  XtVaSetValues(toplevel, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
  wm_delete_window = XmInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW", False);
  XmAddWMProtocolCallback(toplevel, wm_delete_window, main_cb, (XtPointer)MAIN_DATEI_QUIT);

  for (i = 0; widget_names[i] != (char *)NULL; i++)
    {
      /* Widget-handles ueber Widget-Namen holen ----------------------------- */
      sprintf(hilfstr, "*%s", widget_names[i]);
      if (!(widget = my_XtNameToWidget(toplevel, hilfstr, TRUE)))
	return(1);

      /* Callbacks registrieren ---------------------------------------------- */
      XtAddCallback(widget, XmNactivateCallback, main_cb, (XtPointer)w_client_data[i]);
    }

  /* Widget-Handles holen ------------------------------------------------ */

  if (!(widgetMain.CB_datei          = my_XtNameToWidget(toplevel, "*CB_datei", TRUE))) return(1);

  if (!(widgetMain.PB_neu            = my_XtNameToWidget(toplevel, "*PB_neu", TRUE))) return(1);
  if (!(widgetMain.PB_bild_lesen     = my_XtNameToWidget(toplevel, "*PB_bild_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_unv_lesen      = my_XtNameToWidget(toplevel, "*PB_unv_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_geop_lesen     = my_XtNameToWidget(toplevel, "*PB_geop_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_loes_lesen     = my_XtNameToWidget(toplevel, "*PB_loes_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_spannung_lesen = my_XtNameToWidget(toplevel, "*PB_spannung_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_stroemung_lesen= my_XtNameToWidget(toplevel, "*PB_stroemung_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_bild_schreiben = my_XtNameToWidget(toplevel, "*PB_bild_schreiben", TRUE))) return(1);
  if (!(widgetMain.PB_object_schreiben = my_XtNameToWidget(toplevel, "*PB_object_schreiben", TRUE))) return(1);
  if (!(widgetMain.PB_object_lesen     = my_XtNameToWidget(toplevel, "*PB_object_lesen", TRUE))) return(1);
  if (!(widgetMain.PB_snapshot       = my_XtNameToWidget(toplevel, "*PB_snapshot", TRUE))) return(1);
  if (!(widgetMain.PB_createmovie    = my_XtNameToWidget(toplevel, "*PB_createmovie", TRUE))) return(1);
  if (!(widgetMain.PB_quit           = my_XtNameToWidget(toplevel, "*PB_quit", TRUE))) return(1);
  if (!(widgetMain.CB_stoffe         = my_XtNameToWidget(toplevel, "*CB_stoffe", TRUE))) return(1);
  if (!(widgetMain.CB_transformieren = my_XtNameToWidget(toplevel, "*CB_transformieren", TRUE))) return(1);
  if (!(widgetMain.CB_2dgrafik       = my_XtNameToWidget(toplevel, "*CB_2dgrafik", TRUE))) return(1);
  if (!(widgetMain.CB_isolinien      = my_XtNameToWidget(toplevel, "*CB_isolinien", TRUE))) return(1);
  if (!(widgetMain.CB_stroem_spann   = my_XtNameToWidget(toplevel, "*CB_stroem_spann", TRUE))) return(1);
  if (!(widgetMain.CB_hilfe          = my_XtNameToWidget(toplevel, "*CB_hilfe", TRUE))) return(1);
  /*
  if (!(widgetMain.PB_shading        = my_XtNameToWidget(toplevel, "*PB_shading", TRUE))) return(1);
  if (!(widgetMain.PB_colors         = my_XtNameToWidget(toplevel, "*PB_colors", TRUE))) return(1);
  */
  if (!(widgetMain.XmForm_r          = my_XtNameToWidget(toplevel, "*XmForm_r", TRUE))) return(1);
  if (!(widgetMain.RC_view           = my_XtNameToWidget(toplevel, "*RC_view", TRUE))) return(1);
  if (!(widgetMain.PB_animation_start= my_XtNameToWidget(toplevel, "*PB_animation_start", TRUE))) return(1);
  if (!(widgetMain.PB_animation_stop = my_XtNameToWidget(toplevel, "*PB_animation_stop", TRUE))) return(1);
  if (!(widgetMain.PB_zeitschritt    = my_XtNameToWidget(toplevel, "*PB_zeitschritt", TRUE))) return(1);
  if (!(widgetMain.T_zeitschritt     = my_XtNameToWidget(toplevel, "*T_zeitschritt", TRUE))) return(1);
  if (!(widgetMain.T_zeitschritt_start  = my_XtNameToWidget(toplevel, "*T_zeitschritt_start", TRUE))) return(1);
  if (!(widgetMain.T_zeitschritt_end    = my_XtNameToWidget(toplevel, "*T_zeitschritt_end", TRUE))) return(1);
  if (!(widgetMain.T_every_frame     = my_XtNameToWidget(toplevel, "*T_every_frame", TRUE))) return(1);
  if (!(widgetMain.PB_zeitschritt_vor= my_XtNameToWidget(toplevel, "*PB_zeitschritt_vor", TRUE))) return(1);
  if (!(widgetMain.PB_zeitschritt_zurueck = my_XtNameToWidget(toplevel, "*PB_zeitschritt_zurueck", TRUE))) return(1);
  if (!(widgetMain.Spin_rot          = my_XtNameToWidget(toplevel, "*Spin_rot", TRUE))) return(1);
  if (!(widgetMain.RX_rot_x          = my_XtNameToWidget(toplevel, "*RX_rot_x", TRUE))) return(1);
  if (!(widgetMain.RX_rot_y          = my_XtNameToWidget(toplevel, "*RX_rot_y", TRUE))) return(1);
  if (!(widgetMain.RX_rot_z          = my_XtNameToWidget(toplevel, "*RX_rot_z", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_x     = my_XtNameToWidget(toplevel, "*PB_rot_plus_x", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_xx    = my_XtNameToWidget(toplevel, "*PB_rot_plus_xx", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_x    = my_XtNameToWidget(toplevel, "*PB_rot_minus_x", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_xx   = my_XtNameToWidget(toplevel, "*PB_rot_minus_xx", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_y     = my_XtNameToWidget(toplevel, "*PB_rot_plus_y", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_yy    = my_XtNameToWidget(toplevel, "*PB_rot_plus_yy", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_y    = my_XtNameToWidget(toplevel, "*PB_rot_minus_y", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_yy   = my_XtNameToWidget(toplevel, "*PB_rot_minus_yy", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_z     = my_XtNameToWidget(toplevel, "*PB_rot_plus_z", TRUE))) return(1);
  if (!(widgetMain.PB_rot_plus_zz    = my_XtNameToWidget(toplevel, "*PB_rot_plus_zz", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_z    = my_XtNameToWidget(toplevel, "*PB_rot_minus_z", TRUE))) return(1);
  if (!(widgetMain.PB_rot_minus_zz   = my_XtNameToWidget(toplevel, "*PB_rot_minus_zz", TRUE))) return(1);
  if (!(widgetMain.PB_move_plus_x    = my_XtNameToWidget(toplevel, "*PB_move_plus_x", TRUE))) return(1);
  if (!(widgetMain.PB_move_minus_x   = my_XtNameToWidget(toplevel, "*PB_move_minus_x", TRUE))) return(1);
  if (!(widgetMain.PB_move_plus_y    = my_XtNameToWidget(toplevel, "*PB_move_plus_y", TRUE))) return(1);
  if (!(widgetMain.PB_move_minus_y   = my_XtNameToWidget(toplevel, "*PB_move_minus_y", TRUE))) return(1);
  if (!(widgetMain.PB_move_plus_z    = my_XtNameToWidget(toplevel, "*PB_move_plus_z", TRUE))) return(1);
  if (!(widgetMain.PB_move_minus_z   = my_XtNameToWidget(toplevel, "*PB_move_minus_z", TRUE))) return(1);
#ifdef HAVE_SGI
  if (!(widgetMain.WB_zoom           = my_XtNameToWidget(toplevel, "*WB_zoom", TRUE))) return(1);
#endif
  if (!(widgetMain.PB_zoom_14x       = my_XtNameToWidget(toplevel, "*PB_zoom_14x", TRUE))) return(1);
  if (!(widgetMain.PB_zoom_07x       = my_XtNameToWidget(toplevel, "*PB_zoom_07x", TRUE))) return(1);
  if (!(widgetMain.RC_stoffe         = my_XtNameToWidget(toplevel, "*RC_stoffe", TRUE))) return(1);
  if (!(widgetMain.PB_alleStoffe_an  = my_XtNameToWidget(toplevel, "*PB_alleStoffe_an", TRUE))) return(1);
  if (!(widgetMain.PB_alleStoffe_aus = my_XtNameToWidget(toplevel, "*PB_alleStoffe_aus", TRUE))) return(1);
  if (!(widgetMain.PB_reset          = my_XtNameToWidget(toplevel, "*PB_reset", TRUE))) return(1);

  if (!(widgetMain.Frame_da          = my_XtNameToWidget(toplevel, "*Frame_da", TRUE))) return(1);
  if (!(widgetMain.S_colormap        = my_XtNameToWidget(toplevel, "*S_colormap", TRUE))) return(1);
  shell = widgetMain.S_colormap;
  if (!(widgetMain.Frame_cmap        = my_XtNameToWidget(shell, "*Frame_cmap", TRUE))) return(1);
 
  /*if (!(widgetMain.S_abkuehl         = my_XtNameToWidget(toplevel, "*S_abkuehl", TRUE))) return(1);*/
  /*shell = widgetMain.S_abkuehl;*/
  
  /*if (!(widgetMain.Frame_abk         = my_XtNameToWidget(shell, "*Frame_abk", TRUE))) return(1);*/
  
  /* Callbacks registrieren ---------------------------------------------- */
  XtAddCallback(widgetMain.CB_stoffe, 
		XmNactivateCallback, main_cb, (XtPointer)MAIN_STOFFE);
  XtAddCallback(widgetMain.CB_transformieren, 
		XmNactivateCallback, main_cb, (XtPointer)MAIN_TRANSFORMATIONEN);
  XtAddCallback(widgetMain.CB_2dgrafik, 
		XmNactivateCallback, main_cb, (XtPointer)MAIN_2D_GRAFIK);
  XtAddCallback(widgetMain.CB_isolinien, 
		XmNactivateCallback, main_cb, (XtPointer)MAIN_ISOLINIEN);
  XtAddCallback(widgetMain.CB_stroem_spann, 
		XmNactivateCallback, main_cb, (XtPointer)MAIN_STROEM_SPANN);
  /*XtAddCallback(widgetMain.CB_extra, 
    XmNactivateCallback, main_cb, (XtPointer)MAIN_EXTRA);*/
  XtAddCallback(widgetMain.Spin_rot, 
		XmNvalueChangedCallback, ValueChangedCallback, NULL);
  XtAddCallback(widgetMain.PB_zoom_14x, 
		XmNarmCallback, zoomInArmCallback, NULL);
  XtAddCallback(widgetMain.PB_createmovie, 
                XmNactivateCallback, CreateMovieCallback, NULL);
#ifdef __sgi
  XtAddCallback(widgetMain.WB_zoom, XmNvalueChangedCallback, 
                ValChangedCallback, NULL);
  XtAddCallback(widgetMain.WB_zoom, XmNdragCallback, DragCallback, NULL);
#endif
  /* add callback for losing focus zeitschritt widgets */
  XtAddCallback(widgetMain.T_zeitschritt_start, XmNlosingFocusCallback, main_cb,
	  			(XtPointer) MAIN_ZEITSCHRITT_START);
  XtAddCallback(widgetMain.T_zeitschritt_end, XmNlosingFocusCallback, main_cb,
	  			(XtPointer) MAIN_ZEITSCHRITT_ENDE);
  XtAddCallback(widgetMain.T_every_frame, XmNlosingFocusCallback, main_cb,
	  			(XtPointer) MAIN_EVERY_FRAME);

  
  /* initialisiere Widgets ----------------------------------------------- */
  XtSetSensitive(widgetMain.PB_neu,                  TRUE);
  XtSetSensitive(widgetMain.PB_geop_lesen,           TRUE);
  XtSetSensitive(widgetMain.PB_unv_lesen,           FALSE);
  XtSetSensitive(widgetMain.PB_loes_lesen,           TRUE);
  XtSetSensitive(widgetMain.PB_spannung_lesen,       TRUE);
  XtSetSensitive(widgetMain.PB_stroemung_lesen,      TRUE);
  XtSetSensitive(widgetMain.CB_stoffe,              FALSE);
  XtSetSensitive(widgetMain.PB_bild_lesen,           TRUE);
  XtSetSensitive(widgetMain.PB_bild_schreiben,      FALSE);
  XtSetSensitive(widgetMain.CB_transformieren,      FALSE);
  XtSetSensitive(widgetMain.CB_isolinien,           FALSE);
  XtSetSensitive(widgetMain.CB_2dgrafik,            FALSE);
  XtSetSensitive(widgetMain.CB_stroem_spann,        FALSE);
  XtSetSensitive(widgetMain.PB_animation_start,     FALSE);
  XtSetSensitive(widgetMain.PB_animation_stop,      FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt,         FALSE);
  XtSetSensitive(widgetMain.T_zeitschritt,          FALSE);
  XtSetSensitive(widgetMain.T_zeitschritt_start,    FALSE);
  XtSetSensitive(widgetMain.T_zeitschritt_end,      FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt_vor,     FALSE);
  XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, FALSE);

  return(0);
  /* ******************* ende init_main ********************************** */
}


static void ValueChangedCallback(Widget w, XtPointer clientData, XtPointer callData)
{
  XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) callData;
  
  /*
   * set the new value of the spin button
   */
  if(cbs->set)
    b_spin = TRUE;
  else
    b_spin = FALSE;
}

static void zoomInArmCallback(Widget w, XtPointer clientData, XtPointer callData)
{

  XmPushButtonCallbackStruct *cbs = (XmPushButtonCallbackStruct *) callData;
  
  if (cbs->reason == XmCR_ARM)
    InstallLabeledPixmap(w, zoominf);
}
#ifdef __sgi
static void ValChangedCallback( Widget w, XtPointer clientData, XtPointer callData )
{
   SgThumbWheelCallbackStruct *cbs = (SgThumbWheelCallbackStruct *) callData;
   
   if ( cbs != NULL ) 
     {

#ifdef DEBUG
       printf("New value %d.\n", cbs->value);
#endif
       if ( cbs->value >= 500 )
          g_transform.scale = (float) ( 9*cbs->value / 500.0 - 8.0 );
       else
	     g_transform.scale = (float) ( 9*cbs->value / 5000.0 + 0.1 );
#ifdef DEBUG
       printf("Scale: %f\n", g_transform.scale );
#endif
     }
   C3D_postredisplay();
}

static void DragCallback( Widget w, XtPointer clientData, XtPointer callData )
{
   SgThumbWheelCallbackStruct *cbs = (SgThumbWheelCallbackStruct *) callData;
   
   if ( cbs != NULL )
     {
       if ( cbs->value >= 500.0 ) 
         g_transform.scale = (float) ( 9*cbs->value / 500.0 - 8.0 );
       else
         g_transform.scale = (float)( 9*cbs->value / 5000.0 + 0.1);
#ifdef DEBUG
       printf("Scale drag: %f\n", g_transform.scale );
#endif
     }

     /* repaint screen */
     C3D_postredisplay();
}
#endif
