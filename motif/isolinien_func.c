/* ============================================================= */
/* COLOR3D - OpenGL-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname               | Funktion                         */
/* ------------------------------------------------------------- */
/* init_isolinien             |		        		 */
/* fill_tt_isolinien          |		        		 */
/* tt_isolinien_preedit       |	       	        		 */
/* tt_isolinien_postedit      |                                  */
/* tt_isolinien_activate      |	                        	 */
/* tb_tabelle_valuechanged    |	                        	 */
/* tb_isolinien_activate      |	                        	 */
/* tb_isolinien_valuechanged  |	                        	 */
/* add_isolinie               |                                  */
/* delete_isolinie            |                                  */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>

#include <Xm/PushB.h>
#include <Xm/PushBG.h>

#include <BXm.h>
#include <Regex.h>
#include <TextTable.h>

#include "color3d.h"
#include "motif.h"
#include "grafik.h"
#include "genlist.h"
#include "memory.h"

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define TABELLE_ZEICHNEN        0
#define TABELLE_NICHT_ZEICHNEN  1
#define COLORMAP_DEFAULT        2
#define COLORMAP_RASTER         3
#define COLORMAP_DATEI          4
#define COLORMAP_GRAU           5
#define ISOLINIE_ERZEUGEN       6
#define ISOLINIE_LOESCHEN       8
#define ISOLINIE_WERT           9
#define ISOLINIE_BREIT         10
#define INTERVALL_MINIMUM      11
#define INTERVALL_MAXIMUM      12 
#define BEENDEN                13 
#define OK                     14 
#define TABELLE_AUTO_RANGE     15
#define ISOSURFACE_GENERATE    16
#define ISOSURFACE_DELETE      17
#define ISOVOLUME_GENERATE     18
#define ISOVOLUME_DELETE       19
#define ISOSURFACE             255

XtPointer *IsoFar[11] = {
/* Belege Defaultfarben fuer Isolinien ----------------------------- */
(XtPointer) "#00FF00",            /* gruen ------------ */
(XtPointer) "#00FFFF",            /* cyan ------------- */
(XtPointer) "#FF00FF",            /* magenta  --------- */
(XtPointer) "#FFFFFF",            /* weiss ------------ */
(XtPointer) "#404040",            /* grau  ------------ */
(XtPointer) "#C67171",
(XtPointer) "#71C671", 
(XtPointer) "#8E8E38",
(XtPointer) "#7171C6",
(XtPointer) "#8E388E",
(XtPointer) "#388E8E",
};

static Widget *widgetliste;

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* *************************************************************** */
void set_decor(Widget);
int cmp_int(const void *cmpdata, const void *data);
int cmp_iso(const void *cmpdata, const void *data);
void destroy(void *data);
void printle(struct listelem *p, void *arg);

/* ******************************************************************* */
void tb_tabelle_valuechanged(Widget widget, int action, XtPointer cb_ptr)
/* ******************************************************************* */
{
	int i;
	int j;
	int liniennummer;
	int result;
	int *select;
	char *strptr;
	int nrows;
	int pos_count;
	int *pos_list;
	int *id_list;
	static char hilfstr[4096];
	static char hilfstr2[4096];
	Widget colormap_fenster;
	t_linie neue_linie;
	t_iso   new_iso;
	char *Textfeld;
	char *text_wert;

	/* ******************************************************************* */
#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf(stdout," tb_tabelle_valuechanged(.., >%d<, ..) \n",action);
#endif
	/* Togglebuttonabhaengige Aktionen ----------------------------------- */
	switch (action)
	{
		case TABELLE_ZEICHNEN:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_tabelle_zeichnen))
				{  
					XmToggleButtonSetState(widget_isolinien.TB_tabelle_nicht_zeichnen, 
							FALSE, FALSE);
					XtSetSensitive(widget_isolinien.TB_default, TRUE);
					XtSetSensitive(widget_isolinien.TB_grau,    TRUE);
					XtSetSensitive(widget_isolinien.TB_raster,  TRUE);
					XtSetSensitive(widget_isolinien.TB_datei,   TRUE);
					/* Oeffne Tabellenfenster und zeichne Tabelle ------------------------- */
					if (!g_tabelle.on && g_tabelle.ex)
					{
						set_decor(widgetMain.S_colormap);
						//XtPopup(widgetMain.S_colormap,XtGrabNone);
						g_tabelle.on = TRUE;
					}
					if (g_tabelle.on) zeichne_tabelle();
				}
				else
				{
					XmToggleButtonSetState(widget_isolinien.TB_tabelle_nicht_zeichnen,
							TRUE, FALSE);
					XtSetSensitive(widget_isolinien.TB_default, FALSE);
					XtSetSensitive(widget_isolinien.TB_grau,    FALSE);
					XtSetSensitive(widget_isolinien.TB_raster,  FALSE);
					XtSetSensitive(widget_isolinien.TB_datei,   FALSE);
				}
				break;
			}

		case TABELLE_NICHT_ZEICHNEN:
			{
#ifdef DEBUG
				if (applRessourcen.debug) fprintf(stdout,"TABELLE_NICHT_ZEICHNEN\n");
#endif
				if (XmToggleButtonGetState(widget_isolinien.TB_tabelle_nicht_zeichnen))
				{
					XmToggleButtonSetState(widget_isolinien.TB_tabelle_zeichnen,
							FALSE, FALSE);
					XtSetSensitive(widget_isolinien.TB_default, FALSE);
					XtSetSensitive(widget_isolinien.TB_grau,    FALSE);
					XtSetSensitive(widget_isolinien.TB_raster,  FALSE);
					XtSetSensitive(widget_isolinien.TB_datei,   FALSE);
					/* schliesse Tabellenfenster ------------------------------------------- */
					if (g_tabelle.ex && g_tabelle.on)
					{
						XtPopdown(widgetMain.S_colormap);
						g_tabelle.on = FALSE;
					}
				}
				else
				{
					XmToggleButtonSetState(widget_isolinien.TB_tabelle_zeichnen,
							TRUE, FALSE);
					XtSetSensitive(widget_isolinien.TB_default, TRUE);
					XtSetSensitive(widget_isolinien.TB_grau,    TRUE);
					XtSetSensitive(widget_isolinien.TB_raster,  TRUE);
					XtSetSensitive(widget_isolinien.TB_datei,   TRUE);
				}
				break;
			}

		case TABELLE_AUTO_RANGE:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_tabelle_auto_range))
				{
					int i;
					char text[20];
					set_watch_cursor_handler(toplevel);
					set_watch_cursor_handler(widget_isolinien.S_isolinien);
					g_loesung.min_max = TRUE; /* set autorange to true */
					g_loesung.Min = INFINITE;
					g_loesung.Max = -INFINITE;
					for (i = 0; i < g_loesung.anzahl_felder; i++)
						lese_feld_ein(i+1);
					sprintf(text, "%f", g_loesung.Min);
					XmTextFieldSetString(widget_isolinien.T_min, text);
					sprintf(text, "%f", g_loesung.Max);
					XmTextFieldSetString(widget_isolinien.T_max, text);
					g_loesung.min_max = FALSE;
					reset_cursor_handler(widget_isolinien.S_isolinien);
					reset_cursor_handler(toplevel);
				}
				else 
				{
					g_loesung.min_max = FALSE; /* set autorange to false */
				}
				break;
			}

		case COLORMAP_DEFAULT:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_default))
				{
					if (!C3D_win.achtbit)
						SetColorMap2000();
					else
						SetColorMap256();
					C3D_updateisolines();
					if (g_tabelle.on) zeichne_tabelle();
					C3D_postredisplay();
				}
				break;
			}

		case COLORMAP_RASTER:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_raster))
				{
					SetColorMapRaster();
					C3D_updateisolines();
					if (g_tabelle.on) zeichne_tabelle();
					C3D_postredisplay();
				}
				break;
			}

		case COLORMAP_DATEI:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_datei))
				{
					/* Auswahl einer Colormap-Datei ----------------------------- */
					if(C3D_win.achtbit) 
					{
						SetColorMap256();
					}
					else  
					{
						SetColorMap2000();
					}
					strcpy(g_tabelle.dateiname,"testtable.col");
					read_colortable();
					C3D_updateisolines();
					if (g_tabelle.on) zeichne_tabelle();
					C3D_postredisplay();
				}
				break;
			}

		case COLORMAP_GRAU:
			{
				if (XmToggleButtonGetState(widget_isolinien.TB_grau))
				{
					SetColorMapGrau();
					C3D_updateisolines();
					if (g_tabelle.on) zeichne_tabelle();
					C3D_postredisplay();
				}
				break;
			}

		case ISOLINIE_ERZEUGEN:
			{
				text_wert = XmTextGetString(widget_isolinien.T_wert);
				neue_linie.wert = atof(text_wert);
				XtFree(text_wert);

				text_wert = XmTextGetString(widget_isolinien.T_breit);
				neue_linie.breit = atof(text_wert);
				XtFree(text_wert);

				if (neue_linie.wert + neue_linie.breit / 2.0 > g_loesung.Max ||
						neue_linie.wert - neue_linie.breit / 2.0 < g_loesung.Min)
				{
					warning("Invalid values");
					break;
				}

				if (add_isolinie(&neue_linie) == -1)
					warning("Isoline generation not possible");
				fill_tt_isolinien();
				C3D_updateisolines();
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

		case ISOLINIE_LOESCHEN:
			{
				/* hole Linien-Ptr. -------------------------------------------------- */
				XtVaGetValues(widget_isolinien.TT_isolinien, BXmNnumRows,
						&nrows, NULL);

				for (i = 1; i <= nrows; i++)
				{

					if (BXmTextTablePosIsSelected(widget_isolinien.TT_isolinien, i) == True)
					{
						BXmTextTableGetField(widget_isolinien.TT_isolinien, i, 1, FALSE,
								&strptr);

						if (delete_isolinie(atoi(strptr)))
							warning("No isoline left to delete");
					}
				}
				fill_tt_isolinien();
				C3D_updateisolines();
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

		case ISOSURFACE_GENERATE:
			{
				text_wert = XmTextGetString(widget_isolinien.T_wertisosurface);
				new_iso.wert = atof(text_wert);
				XtFree(text_wert);

				if (new_iso.wert > g_loesung.Max ||
						new_iso.wert < g_loesung.Min)
				{
					warning("Invalid values");
					break;
				}

				if (add_iso( &g_isosurfaces.isosurfaces, &new_iso,
							&g_isosurfaces.nisosurfaces ) == -1)
					warning("Isoline generation not possible");
				fill_tt_iso( widget_isolinien.TT_isosurfaces, g_isosurfaces.isosurfaces, 
						g_isosurfaces.nisosurfaces );
				C3D_updateiso( g_isosurfaces.isosurfaces, TRUE, TRUE );
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

		case ISOSURFACE_DELETE:
			{
				/* hole Linien-Ptr. -------------------------------------------------- */
				XtVaGetValues(widget_isolinien.TT_isosurfaces, BXmNnumRows,
						&nrows, NULL);

				for (i = 1; i <= nrows; i++)
				{

					if (BXmTextTablePosIsSelected(widget_isolinien.TT_isosurfaces, i) == True)
					{
						BXmTextTableGetField(widget_isolinien.TT_isosurfaces, i, 1, FALSE,
								&strptr);

						if (delete_iso(&g_isosurfaces.isosurfaces, atoi(strptr),
									&g_isosurfaces.nisosurfaces))
							warning("No isoline left to delete");
					}
				}
				fill_tt_iso( widget_isolinien.TT_isosurfaces, g_isosurfaces.isosurfaces,
						g_isosurfaces.nisosurfaces );
				C3D_updateiso( g_isosurfaces.isosurfaces, TRUE, FALSE );
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}
		case ISOVOLUME_GENERATE:
			{
				text_wert = XmTextGetString(widget_isolinien.T_wertisovolume);
				new_iso.wert = atof(text_wert);
				XtFree(text_wert);

				if (new_iso.wert > g_loesung.Max ||
						new_iso.wert < g_loesung.Min)
				{
					warning("Invalid values");
					break;
				}

				if (add_iso( &g_isovolumes.isovolumes, &new_iso,
							&g_isovolumes.nisovolumes ) == -1)
					warning("Isoline generation not possible");
				fill_tt_iso( widget_isolinien.TT_isovolumes, g_isovolumes.isovolumes, 
						g_isovolumes.nisovolumes );
				C3D_updateiso( g_isovolumes.isovolumes, FALSE, TRUE );
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

		case ISOVOLUME_DELETE:
			{
				/* hole Linien-Ptr. -------------------------------------------------- */
				XtVaGetValues(widget_isolinien.TT_isovolumes, BXmNnumRows,
						&nrows, NULL);

				for (i = 1; i <= nrows; i++)
				{

					if (BXmTextTablePosIsSelected(widget_isolinien.TT_isovolumes, i) == True)
					{
						BXmTextTableGetField(widget_isolinien.TT_isovolumes, i, 1, FALSE,
								&strptr);

						if (delete_iso(&g_isovolumes.isovolumes, atoi(strptr),
									&g_isovolumes.nisovolumes))
							warning("No isoline left to delete");
					}
				}
				fill_tt_iso( widget_isolinien.TT_isovolumes, g_isovolumes.isovolumes,
						g_isovolumes.nisovolumes );
				C3D_updateiso( g_isovolumes.isovolumes, FALSE, FALSE );
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}
		case INTERVALL_MINIMUM:
			{
				Textfeld = XmTextFieldGetString(widget_isolinien.T_min);
				g_loesung.Min = atof(Textfeld);
				XtFree(Textfeld);
				C3D_updateisolines();
				break;
			}

		case INTERVALL_MAXIMUM:
			{
				if(widget_isolinien.T_max) {
					Textfeld = XmTextFieldGetString(widget_isolinien.T_max);
					g_loesung.Max = atof(Textfeld);
					XtFree(Textfeld);
					C3D_updateisolines();
				}
				break;
			}

		case BEENDEN:
			{
				if (!g_loesung.min_max)
				{
					Textfeld = XmTextFieldGetString(widget_isolinien.T_min);
					g_loesung.Min = atof(Textfeld);
					XtFree(Textfeld);

					Textfeld = XmTextFieldGetString(widget_isolinien.T_max);
					g_loesung.Max = atof(Textfeld);
					XtFree(Textfeld);
				}
				else
				{
					g_loesung.Min = g_loesung.akt_min;
					g_loesung.Max = g_loesung.akt_max;
				}

				C3D_postredisplay();
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

		case OK:
			{
				Textfeld = XmTextFieldGetString(widget_loesung.T_min);
				g_loesung.Min = atof(Textfeld);
				XtFree(Textfeld);

				Textfeld = XmTextFieldGetString(widget_loesung.T_max);
				g_loesung.Max = atof(Textfeld);
				XtFree(Textfeld);

				C3D_postredisplay();
				if (g_tabelle.on) zeichne_tabelle();
				break;
			}

			return;
	}
	/* ******************************************************************* */
}

/* ******************************************************************* */
int fill_tt_isolinien()
/* ******************************************************************* */
{
  int i, j;
  char Value[3][81];
  t_linie *linie;
 
  char c_hilf[64];
  t_linie *iso;
  struct listelem *isoline;
  /* ****************************************************************** */
  
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** fill_tt_isolinien() ***\n");
#endif

  XtVaSetValues(widget_isolinien.TT_isolinien,
                BXmNrecomputeLayout, FALSE, NULL);
  XtVaSetValues(widget_isolinien.TT_isolinien, BXmNnumRows,
                g_isolinien.anzahl_isolinien, NULL);
  BXmTextTableSetNumColumns(widget_isolinien.TT_isolinien, 4);

  if (g_isolinien.linien != NULL) 
    {
      for(isoline = g_isolinien.linien->head, i = 0; isoline != NULL; 
	  isoline = isoline->next, i++)
	{
	  iso = (t_linie *)isoline->data;
	  sprintf(Value[0], "%d", iso->nr);
	  sprintf(Value[1], "%f", iso->wert);
	  sprintf(Value[2], "%f", iso->breit);
	  BXmTextTableSetField(widget_isolinien.TT_isolinien, i+1 ,1 ,/* Nr. */
			       Value[0]);
	  BXmTextTableSetField(widget_isolinien.TT_isolinien, i+1 ,2 ,/* Wert */
			       Value[1]);
	  BXmTextTableSetField(widget_isolinien.TT_isolinien, i+1 ,3 ,/*Breite*/
			       Value[2]);
	 
	  BXmTextTableSetBackgroundColor(widget_isolinien.TT_isolinien, i+1,4,
					   (String) IsoFar[i]);
	}
    }

  XtVaSetValues(widget_isolinien.TT_isolinien,
                BXmNrecomputeLayout, TRUE, NULL);

  BXmTextTableDeselectAll(widget_isolinien.TT_isolinien);

  sprintf(c_hilf, "%f", g_loesung.Min);
  XmTextFieldSetString(widget_isolinien.T_min, c_hilf);
  sprintf(c_hilf, "%f", g_loesung.Max);
  XmTextFieldSetString(widget_isolinien.T_max, c_hilf);

  return(0);
}

/* ******************************************************************* */
int fill_tt_iso(Widget w, struct list *isol, int niso)
/* ******************************************************************* */
{

	int i;
	char Value[3][81];

	char c_hilf[64];
	t_iso *newiso;
	struct listelem *iso;
	/* ****************************************************************** */
#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf(stdout, "*** fill_tt_isosurfaces() ***\n");
#endif

	XtVaSetValues(w,
			BXmNrecomputeLayout, FALSE, NULL);
	XtVaSetValues(w, BXmNnumRows,
			 niso, NULL);
	BXmTextTableSetNumColumns(w, 4);

	if (isol != NULL) 
	{
		for(iso = isol->head, i = 0; iso != NULL; 
				iso = iso->next, i++)
		{
			newiso = (t_iso *)iso->data;
			sprintf(Value[0], "%d", newiso->nr);
			sprintf(Value[1], "%f", newiso->wert);
			BXmTextTableSetField(w, i+1 ,1 ,/* Nr. */
					Value[0]);
			BXmTextTableSetField(w, i+1 ,2 ,/* Wert */
					Value[1]);

			BXmTextTableSetBackgroundColor(w, i+1,3,
					(String) IsoFar[i]);
		}
	}

	XtVaSetValues(w, BXmNrecomputeLayout, TRUE, NULL);

	BXmTextTableDeselectAll(w);

	return(0);
}

/* ******************************************************************* */

/* ******************************************************************* */
void tt_isolinien_preedit(Widget widget, XtPointer client_data,
                          BXmTextTablePreEditCallbackStruct *cb_ptr)
/* ******************************************************************* */
{
	int i;
	char *strptr;
	char wert[81];
	int liniennummer;
	int n_selected;
	int setting;
	static char hilfstr[4096];
	int result;
	static char *d_iso_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
	static char *e_iso_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
	static char **p_set;
	t_widget_isolinien *w_ptr = &widget_isolinien;
	Widget wcolchooser;
	Arg args[1];

#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf(stdout, "*** tt_isolinien_preedit() ***\n");
#endif
	/* Feld editieren --------------------------------------------- */
	if (g_isolinien.anzahl_isolinien > 0)
		cb_ptr->doit = TRUE;
	else
		cb_ptr->doit = FALSE;

	/* hole Stoff-Ptr. -------------------------------------------------- */
	BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
	liniennummer = atoi(strptr);

	/* falls aktuelle Zeile nicht selektiert ist, wird diese selektiert */
	if (BXmTextTablePosIsSelected (widget, cb_ptr->currentRow))
		BXmTextTableDeselect(widget, cb_ptr->currentRow, FALSE);
	else
		BXmTextTableSelect(widget, cb_ptr->currentRow, FALSE);

	/* spaltenabhaengige Aktionen --------------------------------------- */
	switch (cb_ptr->currentColumn)
	{
		case 1:                     /* Nummer --------------------------- */
		case 2:                     /* Wert ----------------------------- */
		case 3:                     /* Breite --------------------------- */
			break;
		case 4:                     /* Farbe ---------------------------- */
			{
				args[0].value = cb_ptr->currentRow;
				args[0].name  = "current row";
#ifdef HAVE_SGI
				wcolchooser   = CreateColorChooser("S_color_chooser", widget,
						args, 1);
#endif
				break;
			}
	}
	fill_tt_isolinien();

	return;
}

/***********************************************************************
    This function preedits the text table for isovolume or isosurface.

***********************************************************************/
void tt_iso_preedit(Widget widget, XtPointer client_data,
                          BXmTextTablePreEditCallbackStruct *cb_ptr)
/* ******************************************************************* */
{
	int i;
	char *strptr;
	char wert[81];
	int liniennummer;
	int n_selected;
	int setting;
	static char hilfstr[4096];
	int isosurface;
	int result;
	static char *d_iso_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
	static char *e_iso_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
	static char **p_set;
	t_widget_isolinien *w_ptr = &widget_isolinien;
	Widget wcolchooser;
	Arg args[1];

	isosurface = (int) client_data;

#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf(stdout, "*** tt_isolinien_preedit() ***\n");
#endif
	/* Feld editieren --------------------------------------------- */
	if (g_isolinien.anzahl_isolinien > 0)
		cb_ptr->doit = TRUE;
	else
		cb_ptr->doit = FALSE;

	/* hole Stoff-Ptr. -------------------------------------------------- */
	BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
	liniennummer = atoi(strptr);

	/* falls aktuelle Zeile nicht selektiert ist, wird diese selektiert */
	if (BXmTextTablePosIsSelected (widget, cb_ptr->currentRow))
		BXmTextTableDeselect(widget, cb_ptr->currentRow, FALSE);
	else
		BXmTextTableSelect(widget, cb_ptr->currentRow, FALSE);

	/* spaltenabhaengige Aktionen --------------------------------------- */
	switch (cb_ptr->currentColumn)
	{
		case 1:                     /* Nummer --------------------------- */
		case 2:                     /* Wert ----------------------------- */
			break;
		case 3:                     /* Farbe ---------------------------- */
			{
				args[0].value = cb_ptr->currentRow;
				args[0].name  = "current row";
#ifdef HAVE_SGI
				wcolchooser   = CreateColorChooser("S_iso_color_chooser", widget,
						args, 1);
#endif
				break;
			}
	}

	/* choose isosurface or isovolume */
	if ( isosurface )
		fill_tt_iso( widget, g_isosurfaces.isosurfaces, g_isosurfaces.nisosurfaces );
	else
		fill_tt_iso( widget, g_isovolumes.isovolumes, g_isovolumes.nisovolumes );
	return;
}


void tt_isolinien_postedit(Widget widget, XtPointer client_data,
			 BXmTextTablePostEditCallbackStruct *cb_ptr)
{
	int act_iso;
	t_linie *isoline;
	struct listelem *lisolines;
	char *strptr;
	int i;

	/* get the number of current isoline */
	BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
	act_iso = atoi(strptr);

	/* get the isoline */
	lisolines = g_isolinien.linien->head;	
	for( i = 0; i < (cb_ptr->currentRow - 1); i++ )
		lisolines = lisolines->next;

	isoline = (t_linie *)lisolines->data;
	
	switch(cb_ptr->currentColumn)
	{
		case 1:	   /* Number */
			/* do nothing */
			break;
		case 2:
			BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			FALSE, &strptr);
			isoline->wert = atof(strptr);
			break;
		case 3:
			BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			FALSE, &strptr);
			isoline->breit = atof(strptr);
			break;
		case 4:   /* Color */
			/* do nothing */
			break;
	}
	fill_tt_isolinien();
}

void tt_iso_postedit(Widget widget, XtPointer client_data,
			 BXmTextTablePostEditCallbackStruct *cb_ptr)
{
	int act_iso;
	t_iso *iso;
	int isosurface;
	struct listelem *liso;
	char *strptr;
	int i;

	isosurface = (int) client_data;

	/* get the number of current isoline */
	BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
	act_iso = atoi(strptr);

	/* get the isoline */
	if ( isosurface )
		liso = g_isosurfaces.isosurfaces->head;	
	else
		liso = g_isovolumes.isovolumes->head;

	for( i = 0; i < (cb_ptr->currentRow - 1); i++ )
		liso = liso->next;

	iso = (t_iso *)liso->data;
	
	switch(cb_ptr->currentColumn)
	{
		case 1:	   /* Number */
			/* do nothing */
			break;
		case 2:
			BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			FALSE, &strptr);
			iso->wert = atof(strptr);
			break;
					break;
		case 3:   /* Color */
			/* do nothing */
			break;
	}

	if ( isosurface )
		fill_tt_iso(widget, g_isosurfaces.isosurfaces, g_isosurfaces.nisosurfaces );
	else
		fill_tt_iso(widget, g_isovolumes.isovolumes, g_isovolumes.nisovolumes );
}

/* ******************************************************************* */

/* ******************************************************************* */
void tt_isolinien_activate(Widget widget, XtPointer client_data,
                           BXmTextTableActivateCallbackStruct *cb_ptr)
/* ******************************************************************* */
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tt_isolinien_activate() ***\n");
#endif
  /* naechstes Feld nicht editieren ------------------------------------ */
  cb_ptr->editing = FALSE;

  return;
}
/* ******************************************************************* */

/* ******************************************************************* */
int init_isolinien(void)
/* ******************************************************************* */
{
	int result;
	t_widget_isolinien *w_ptr = &widget_isolinien;
	Widget shell;
	static char hilfstr[256];
	Atom wm_delete_window;
#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf(stdout, "*** void init_isolinien() ***\n");
#endif
	if (w_ptr->S_isolinien == NULL)
	{
		if (!(w_ptr->S_isolinien = Createisolinien_interface("S_isolinien", toplevel, NULL, 0)))
		{
			fprintf(stderr, "Createisolinien_interface(...) returns NULL\n");
			return(2);
		}
		shell = w_ptr->S_isolinien;
		if (!(w_ptr->TB_tabelle_zeichnen = my_XtNameToWidget(shell,
						"*TB_tabelle_zeichnen", TRUE))) return(1);
		if (!(w_ptr->TB_tabelle_nicht_zeichnen = my_XtNameToWidget(shell,
						"*TB_tabelle_nicht_zeichnen", TRUE))) return(1);
		if (!(w_ptr->TB_tabelle_auto_range = my_XtNameToWidget(shell,
						"*TB_tabelle_auto_range", TRUE))) return(1);
		if (!(w_ptr->RC_tabelle          = my_XtNameToWidget(shell,
						"*RC_tabelle", TRUE))) return(1);
		if (!(w_ptr->TB_default          = my_XtNameToWidget(shell,
						"*TB_default", TRUE))) return(1);
		if (!(w_ptr->TB_grau             = my_XtNameToWidget(shell,
						"*TB_grau", TRUE))) return(1);
		if (!(w_ptr->TB_raster           = my_XtNameToWidget(shell,
						"*TB_raster", TRUE))) return(1);
		if (!(w_ptr->TB_datei            = my_XtNameToWidget(shell,
						"*TB_datei", TRUE))) return(1);
		if (!(w_ptr->PB_beenden          = my_XtNameToWidget(shell,
						"*PB_beenden", TRUE))) return(1);
		if (!(w_ptr->PB_ok               = my_XtNameToWidget(shell,
						"*PB_ok", TRUE))) return(1);
		if (!(w_ptr->PB_hilfe            = my_XtNameToWidget(shell,
						"*PB_hilfe", TRUE))) return(1);
		if (!(w_ptr->PB_erzeugen         = my_XtNameToWidget(shell,
						"*PB_erzeugen", TRUE))) return(1);
		if (!(w_ptr->PB_loeschen         = my_XtNameToWidget(shell,
						"*PB_loeschen", TRUE))) return(1);
		if (!(w_ptr->TT_isolinien        = my_XtNameToWidget(shell,
						"*TT_isolinien", TRUE))) return(1);
		if (!(w_ptr->T_wert        = my_XtNameToWidget(shell,
						"*T_wert", TRUE))) return(1);
		if (!(w_ptr->T_breit        = my_XtNameToWidget(shell,
						"*T_breit", TRUE))) return(1);
		if (!(w_ptr->PB_erzeugen        = my_XtNameToWidget(shell,
						"*PB_erzeugen", TRUE))) return(1);
		if (!(w_ptr->PB_loeschen        = my_XtNameToWidget(shell,
						"*PB_loeschen", TRUE))) return(1);
		if (!(w_ptr->TT_isosurfaces     = my_XtNameToWidget( shell, 
						"*TT_isosurfaces", TRUE ))) return(1);
		if (!(w_ptr->T_wertisosurface       = my_XtNameToWidget(shell,
						"*T_wertisosurface", TRUE))) return(1);
		if (!(w_ptr->PB_genisosurface        = my_XtNameToWidget(shell,
						"*PB_genisosurface", TRUE))) return(1);
		if (!(w_ptr->PB_delisosurface        = my_XtNameToWidget(shell,
						"*PB_delisosurface", TRUE))) return(1);
		if (!(w_ptr->TT_isovolumes     = my_XtNameToWidget( shell, 
						"*TT_isovolumes", TRUE ))) return(1);
		if (!(w_ptr->T_wertisovolume       = my_XtNameToWidget(shell,
						"*T_wertisovolume", TRUE))) return(1);
		if (!(w_ptr->PB_genisovolume        = my_XtNameToWidget(shell,
						"*PB_genisovolume", TRUE))) return(1);
		if (!(w_ptr->PB_delisovolume        = my_XtNameToWidget(shell,
						"*PB_delisovolume", TRUE))) return(1);

		if (!(w_ptr->T_max        = my_XtNameToWidget(shell,
						"*T_max", TRUE))) return(1);
		if (!(w_ptr->T_min        = my_XtNameToWidget(shell,
						"*T_min", TRUE))) return(1);

		/* window-manager delete-button */
		XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
		wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW",
				False);
		XmAddWMProtocolCallback(shell, wm_delete_window,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)BEENDEN);

		/* Callbacks registrieren */
		/* Callbacks fuer Isolinientabelle --------------------------- */
		XtAddCallback(
				w_ptr->TT_isolinien, XmNpreEditCallback,
				(XtCallbackProc)tt_isolinien_preedit,
				(XtPointer)NULL);
		XtAddCallback(
				w_ptr->TT_isolinien, XmNpostEditCallback,
				(XtCallbackProc)tt_isolinien_postedit,
				(XtPointer)NULL);

		XtAddCallback(
				w_ptr->TT_isolinien, XmNactivateCallback,
				(XtCallbackProc)tt_isolinien_activate,
				(XtPointer)NULL);
		XtOverrideTranslations(
				BXmTextTableGetChild(w_ptr->TT_isolinien, BXmCHILD_INPUT),
				TF_in_TT_translations_table);
		/* Callbacks for isosurfaces table -------------------------- */	
		XtAddCallback( 
				w_ptr->TT_isosurfaces, XmNpreEditCallback,
				(XtCallbackProc) tt_iso_preedit,
				(XtPointer) ISOSURFACE );
		XtAddCallback( 
				w_ptr->TT_isosurfaces, XmNpostEditCallback,
				(XtCallbackProc) tt_iso_postedit,
				(XtPointer) ISOSURFACE );
		/* Callbacks for isovolumes table -------------------------- */	
		XtAddCallback( 
				w_ptr->TT_isovolumes, XmNpreEditCallback,
				(XtCallbackProc) tt_iso_preedit,
				(XtPointer) 0 );
		XtAddCallback( 
				w_ptr->TT_isovolumes, XmNpostEditCallback,
				(XtCallbackProc) tt_iso_postedit,
				(XtPointer) 0 );
		/* Togglebutton- und Pushbutton-Callbacks ------------------- */
		XtAddCallback(
				w_ptr->TB_tabelle_zeichnen, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)TABELLE_ZEICHNEN);
		XtAddCallback(
				w_ptr->TB_tabelle_nicht_zeichnen, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)TABELLE_NICHT_ZEICHNEN);
		XtAddCallback(
				w_ptr->TB_tabelle_auto_range, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)TABELLE_AUTO_RANGE);
		XtAddCallback(
				w_ptr->TB_default, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)COLORMAP_DEFAULT);
		XtAddCallback(
				w_ptr->TB_grau, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)COLORMAP_GRAU);
		XtAddCallback(
				w_ptr->TB_raster, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)COLORMAP_RASTER);
		XtAddCallback(
				w_ptr->TB_datei, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)COLORMAP_DATEI);
		XtAddCallback(
				w_ptr->PB_erzeugen, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOLINIE_ERZEUGEN);
		XtAddCallback(
				w_ptr->PB_loeschen, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOLINIE_LOESCHEN);
		XtAddCallback(
				w_ptr->PB_genisosurface, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOSURFACE_GENERATE);
		XtAddCallback(
				w_ptr->PB_delisosurface, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOSURFACE_DELETE);
		XtAddCallback(
				w_ptr->PB_genisovolume, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOVOLUME_GENERATE);
		XtAddCallback(
				w_ptr->PB_delisovolume, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOVOLUME_DELETE);
		XtAddCallback(
				w_ptr->T_wert, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOLINIE_WERT);
		XtAddCallback(
				w_ptr->T_breit, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)ISOLINIE_BREIT);
		XtAddCallback(
				w_ptr->T_min, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)INTERVALL_MINIMUM);
		XtAddCallback(
				w_ptr->T_max, XmNvalueChangedCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)INTERVALL_MAXIMUM);
		XtAddCallback(
				w_ptr->PB_beenden, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)BEENDEN);
		XtAddCallback(
				w_ptr->PB_ok, XmNactivateCallback,
				(XtCallbackProc)tb_tabelle_valuechanged,
				(XtPointer)BEENDEN);
	}

	XtPopup(w_ptr->S_isolinien, XtGrabExclusive);

	init_isolinien_interface();
	fill_tt_isolinien();
	fill_tt_iso(w_ptr->TT_isosurfaces, g_isosurfaces.isosurfaces, g_isosurfaces.nisosurfaces);
	fill_tt_iso(w_ptr->TT_isovolumes, g_isovolumes.isovolumes, g_isovolumes.nisovolumes);

	return(0);
	/* ******************************************************************* */
}

/* ******************************************************************* */
int add_isolinie(t_linie *neue_linie)
/* ******************************************************************* */
{
  int i, j;
  t_linie *nisolinie;
  
  /* suche naechste freie Nummer -------------------------------------- */
  i = 0; 
  j = 0;

  /* allocate new isolinie */
  j = g_isolinien.anzahl_isolinien;
  NEW(nisolinie);
  nisolinie->wert = neue_linie->wert;
  nisolinie->breit = neue_linie->breit;
  nisolinie->on = TRUE;
  nisolinie->nr = g_isolinien.anzahl_isolinien + 1;
  nisolinie->r  = (1.0*(IsoFarben[j%5]>>16))/256.0;
  nisolinie->g  = (1.0*((IsoFarben[j%5]&0xFF00)>>8))/256.0;
  nisolinie->b  = (1.0*(IsoFarben[j%5]&0xFF))/256.0;
  
  if (g_isolinien.linien == NULL)
    {
      /* initialize list */
      NEW(g_isolinien.linien);
      glistinit(g_isolinien.linien, destroy);
    }

  /* add isolinie to linked list */
  g_isolinien.linien->head = gaddend(g_isolinien.linien, 
				     gnewitem((void*)nisolinie));
 
  g_isolinien.anzahl_isolinien++;
 
  return 1;
/* ******************************************************************* */
}

int add_iso( struct list **liso, t_iso *new_iso, int *numiso )
{
	t_iso *niso;
	int i,j;
 	i = j = 0;	

	/* allocate new iso */
	NEW( niso );
	niso->mat = (t_isomat *) ALLOC( sizeof( niso->mat[0] ) * g_geodat.nstoff );
	j = *numiso;
	niso->wert = new_iso->wert;
	niso->on   = new_iso->on;
	niso->nr   = *numiso + 1;

	for ( i = 0; i < g_geodat.nstoff; i++ )
	{
		(niso->mat[i]).iso_vertices = NULL;
		(niso->mat[i]).iso_colors = NULL;
		
		(niso->mat[i]).ntri = 0;
	}
	niso->field = FALSE;
	niso->r    = (1.0*(IsoFarben[j%5]>>16))/256.0;
	niso->g    = (1.0*((IsoFarben[j%5]&0xFF00)>>8))/256.0;
	niso->b    = (1.0*(IsoFarben[j%5]&0xFF))/256.0;
	
	if ( *liso == NULL )
	{
		NEW( *liso );
		glistinit( *liso, destroy );
	}
	(*liso)->head = gaddend( *liso, gnewitem( (void*) niso));
	(*numiso)++;	
}

/**********************************************************************
  Change color of isoline.
***********************************************************************/
#ifdef HAVE_SGI
int change_isolinie_col(int liniennumer)
{
	t_linie *q_linie, *linie;
	struct listelem *isolinie;

	/* This necessary while color chooser changes
	 * current OpenGL drawing context. And we
	 * want to draw to main window.
	 */
	glXMakeCurrent(C3D_win.dpy, XtWindow(C3D_win.wid), C3D_win.cx);

	isolinie = glookup(g_isolinien.linien, (void *) &liniennumer, cmp_int);

	linie = (t_linie *)isolinie->data;

	linie->r = (float)(chcolor.color[0]/255.0);
	linie->g = (float)(chcolor.color[1]/255.0);
	linie->b = (float)(chcolor.color[2]/255.0);  

	/* change background color of the text table */
	strcpy (IsoFar[liniennumer-1], chcolor.color_string);

	fill_tt_isolinien();
	C3D_updateisolines();
	return 0;
}

int change_iso_col( int linum )
{
	t_iso *liso;
	struct listelem *elem;
	glXMakeCurrent( C3D_win.dpy, XtWindow(C3D_win.wid), C3D_win.cx );
	
	elem = glookup( g_isosurfaces.isosurfaces, (void *) &linum, cmp_iso );
	liso = ( t_iso *) elem->data;
	
	liso->r = (float) ( chcolor.color[0] / 255.0 );
	liso->g = (float) ( chcolor.color[1] / 255.0 );
	liso->b = (float) ( chcolor.color[2] / 255.0 );
	
	strcpy( IsoFar[linum - 1], chcolor.color_string );

	fill_tt_iso( widget_isolinien.TT_isosurfaces, g_isosurfaces.isosurfaces,
						g_isosurfaces.nisosurfaces );
	C3D_updateiso(g_isosurfaces.isosurfaces, TRUE, FALSE ); 
}
#endif

/* ******************************************************************* */
int delete_isolinie(int liniennumer)
/* ******************************************************************* */
{
  t_linie *q_linie, *linie;
  struct listelem *isolinie;
      
  /* ******************************************************************* */
  
  
  /* delete isolinie from list */
  gdelitem(g_isolinien.linien, (void*) &liniennumer, cmp_int);
 
  --g_isolinien.anzahl_isolinien;
  
  return 0;
}

int cmp_iso( const void *cmpdata, const void* data )
{
	int *nr, nr1;
	t_iso *cmpiso, *iso;

	nr = (int *) cmpdata;
	iso = (t_iso *) data;
	nr1 = iso->nr;

	if (*nr > nr1)
		return 1;
	else if(*nr < nr1)
		return -1;
	else
		return 0;
}

int delete_iso( struct list **liso, int linnum, int *numiso )
{
	t_iso *iso;
	struct listelem *p;
	int i,j;

	/* delete iso from list */
	for( p = (*liso)->head; p != NULL; p = p->next )
	{
		iso = (t_iso*) p->data;
		if ( iso->nr == linnum )
		{
			for ( j = 0; j < g_geodat.nstoff; j ++ )
			{
				if ( (iso->mat[j]).iso_vertices != NULL )
					FREE( (iso->mat[j]).iso_vertices );

				if ( (iso->mat[j]).iso_colors != NULL )
					FREE( (iso->mat[j]).iso_colors );
			}
			FREE( iso->mat );
			FREE( iso );
		}
	}
	gdelitem( *liso, (void*) &linnum, cmp_iso );
	(*numiso)--;
	return 0;
}

int cmp_int(const void *cmpdata, const void *data)
{
  int *nr, nr1;
  t_linie *cmpiso, *iso;
  
  nr = (int *) cmpdata;
  iso = (t_linie *) data;
  nr1 = iso->nr;
  
  if (*nr > nr1)
    return 1;
  else if(*nr < nr1)
    return -1;
  else
    return 0;
}

void destroy(void *data)
{
    t_linie *p;

    p = (t_linie *) data;
    free (p);
}

void printle(struct listelem *p, void *arg)
{
    char *fmt;
    t_linie *niso;

    fmt = (char *) arg;
    niso = (t_linie *) p->data;
    printf(fmt, niso->wert, niso->breit);
}

void set_decor(Widget shell)
{
  XtVaSetValues(shell, XmNmwmDecorations,
		MWM_DECOR_ALL 
		| MWM_DECOR_MENU,
		NULL);
}
