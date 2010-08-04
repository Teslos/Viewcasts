/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   10.01.97                                         */
/* Dateiname:   loesung_func.c                                   */
/*                                                               */
/* Routinenname            | Funktion                            */
/* ------------------------------------------------------------- */
/* init_loesung            |	 				 */
/* tb_loesung_activate     |				         */
/* fill_tt_isolinien       |                                      */
/* tt_isolinien_preedit    |                                      */
/* tt_isolinien_activate   |                                      */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>
#include <Xm/TextF.h>

#include "color3d.h"
#include "motif.h"
#include "grafik.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define LOESUNG_OK               0
#define LOESUNG_TEXT             1
#define LOESUNG_PARAMETER        2
#define LOESUNG_MIN              3
#define LOESUNG_MAX              4
#define LOESUNG_SCHRITT          5
#define LOESUNG_DIMENS           6
#define LOESUNG_ANI_ANFANG       7
#define LOESUNG_ANI_ENDE         8
#define LOESUNG_PB_ANI_ANFANG    9
#define LOESUNG_PB_ANI_ENDE     10
#define LOESUNG_INT_ANFANG      11
#define LOESUNG_INT_ENDE        12
#define LOESUNG_INT_SCHRITT     13
#define LOESUNG_SCHLIESSEN     199
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/************************************************************************/
void tb_loesung_activate(Widget widget, int action, XtPointer cb_ptr)
/************************************************************************/
{
  int index;
  int i;
  int j;
  int stoffnr;
  int result;
  char *name;
  char *strptr;
  int nrows;
  int pos_count;
  int *pos_list;
  int *id_list;
  static char hilfstr[4096];
  static char hilfstr2[4096];
  t_loesung l_loesung;
  char *Textfeld;
  char c_hilfe[256];
  float zeit;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tb_loesung_activate(.., >%d<, ..) ***\n",action);
#endif
/* Belege lokale Loesungsstruktur ------------------------------------- */
      strcpy(l_loesung.ausgabe,   g_loesung.ausgabe);
      strcpy(l_loesung.parameter, g_loesung.parameter);
      strcpy(l_loesung.einheit,   g_loesung.einheit);
      l_loesung.akt_min       =   g_loesung.akt_min;
      l_loesung.akt_max       =   g_loesung.akt_max;
      l_loesung.akt_schritt   =   g_loesung.akt_schritt;
      l_loesung.Min           =   g_loesung.Min;
      l_loesung.Max           =   g_loesung.Max;
      l_loesung.schritt       =   g_loesung.schritt;
      l_loesung.anim_anfang   =   g_loesung.anim_anfang;
      l_loesung.anim_ende     =   g_loesung.anim_ende;

/*	Togglebuttonabhaengige Aktionen */
  switch (action)
  {
    case LOESUNG_OK:
    {
#ifdef DEBUG
      if (applRessourcen.debug) fprintf(stdout, "LOESUNG_OK\n");
#endif
/* uebernehme gesetzte Werte in globale Struktur ---------------------- */
      g_loesung.param_gesetzt = TRUE;
      Textfeld = XmTextFieldGetString(widget_loesung.T_text);
      strcpy(g_loesung.ausgabe, Textfeld);
      XtFree(Textfeld);

      Textfeld = XmTextFieldGetString(widget_loesung.T_parameter);
      strcpy(g_loesung.parameter, Textfeld);
      XtFree(Textfeld);

      Textfeld = XmTextFieldGetString(widget_loesung.T_dimens);
      strcpy(g_loesung.einheit, Textfeld);
      XtFree(Textfeld);

      Textfeld = XmTextFieldGetString(widget_loesung.T_min);
      g_loesung.Min = atof(Textfeld);
      XtFree(Textfeld);

      Textfeld = XmTextFieldGetString(widget_loesung.T_max);
      g_loesung.Max = atof(Textfeld);
      XtFree(Textfeld);

      Textfeld = XmTextFieldGetString(widget_loesung.T_ani_anfang);
      zeit= atof(Textfeld);
      XtFree(Textfeld);
      index = search_next_zeitschritt(&zeit);
      g_loesung.anim_anfang = index;

      Textfeld = XmTextFieldGetString(widget_loesung.T_ani_ende);
      zeit = atof(Textfeld);
      XtFree(Textfeld);
      index = search_next_zeitschritt(&zeit);
      g_loesung.anim_ende = index;

      break;
    }

    case LOESUNG_SCHLIESSEN:
    {
      strcpy(g_loesung.parameter, l_loesung.parameter);
      strcpy(g_loesung.einheit,   l_loesung.einheit);
      g_loesung.akt_min       =   l_loesung.akt_min;
      g_loesung.akt_max       =   l_loesung.akt_max;
      g_loesung.akt_schritt   =   l_loesung.akt_schritt;
      g_loesung.Min           =   l_loesung.Min;
      g_loesung.Max           =   l_loesung.Max;
      g_loesung.schritt       =   l_loesung.schritt;
      g_loesung.anim_anfang   = l_loesung.anim_anfang;
      g_loesung.anim_ende     = l_loesung.anim_ende;
      break;
    }

    case LOESUNG_TEXT:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_text);
      strcpy(l_loesung.ausgabe, Textfeld);
      XtFree(Textfeld);
      break;
    }

    case LOESUNG_PARAMETER:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_parameter);
      strcpy(l_loesung.parameter, Textfeld);
      XtFree(Textfeld);
      break;
    }

    case LOESUNG_MIN:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_min);
      l_loesung.Min = atof(Textfeld);
      XtFree(Textfeld);
      break;
    }

    case LOESUNG_MAX:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_max);
      l_loesung.Max = atof(Textfeld);
      XtFree(Textfeld);
      break;
    }

    case LOESUNG_DIMENS:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_dimens);
      strcpy(l_loesung.einheit, Textfeld);
      XtFree(Textfeld);
      break;
    }

    case LOESUNG_ANI_ANFANG:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_ani_anfang);
      zeit= atof(Textfeld);
      XtFree(Textfeld);
      index = search_next_zeitschritt(&zeit);
      l_loesung.anim_anfang = zeit;
      break;
    }

    case LOESUNG_ANI_ENDE:
    {
      Textfeld = XmTextFieldGetString(widget_loesung.T_ani_ende);
      zeit = atof(Textfeld);
      XtFree(Textfeld);
      index = search_next_zeitschritt(&zeit);
      l_loesung.anim_ende = zeit;
      break;
    }

    case LOESUNG_PB_ANI_ANFANG:
    {
      index = loes_auswahl(widget, (XtPointer)action, (XtPointer)cb_ptr) - 1;
      zeit = g_loesung.zeiten[index];
      sprintf(c_hilfe, "%8.3f", zeit);
      XmTextFieldSetString(widget_loesung.T_ani_anfang, c_hilfe);
      l_loesung.anim_anfang = index;
      break;
    }

    case LOESUNG_PB_ANI_ENDE:
    {
      index = loes_auswahl(widget, (XtPointer)action, (XtPointer)cb_ptr) - 1;
      sprintf(hilfstr,"%8.3f", g_loesung.zeiten[index]);
      XmTextFieldSetString(widget_loesung.T_ani_ende, hilfstr);
      l_loesung.anim_ende = index;
      break;
    }
  }
  return;
/************************************************************************/
}

/************************************************************************/
int init_loesung(void)
/************************************************************************/
{
  int result;
  t_widget_loesung *w_ptr = &widget_loesung;
  Widget shell;
  static char hilfstr[256];
  Atom wm_delete_window;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_loesung() ***\n");
#endif
  if (w_ptr->S_loesung == NULL)
  {
    if (!(w_ptr->S_loesung =
               Createloesung_interface("S_loesung", toplevel, NULL, 0)))
    {
      fprintf(stderr, "Createloesung_interface(...) returns NULL\n");
      return(2);
    }
    shell = w_ptr->S_loesung;

    if (!(w_ptr->PB_ok         = my_XtNameToWidget(shell, "*PB_ok", TRUE))) return(1);
    if (!(w_ptr->PB_schliessen = my_XtNameToWidget(shell, "*PB_schliessen", TRUE))) return(1);
    if (!(w_ptr->PB_hilfe      = my_XtNameToWidget(shell, "*PB_hilfe", TRUE))) return(1);
    if (!(w_ptr->T_ani_anfang  = my_XtNameToWidget(shell, "*T_ani_anfang", TRUE))) return(1);
    if (!(w_ptr->PB_ani_anfang = my_XtNameToWidget(shell, "*PB_ani_anfang", TRUE))) return(1);
    if (!(w_ptr->T_ani_ende    = my_XtNameToWidget(shell, "*T_ani_ende", TRUE))) return(1);
    if (!(w_ptr->PB_ani_ende   = my_XtNameToWidget(shell, "*PB_ani_ende", TRUE))) return(1);
    if (!(w_ptr->T_text        = my_XtNameToWidget(shell, "*T_text", TRUE))) return(1);
    if (!(w_ptr->T_parameter   = my_XtNameToWidget(shell, "*T_parameter", TRUE))) return(1);
    if (!(w_ptr->T_max         = my_XtNameToWidget(shell, "*T_max", TRUE))) return(1);
    if (!(w_ptr->T_min         = my_XtNameToWidget(shell, "*T_min", TRUE))) return(1);
    if (!(w_ptr->T_dimens      = my_XtNameToWidget(shell, "*T_dimens", TRUE))) return(1);
    if (!(w_ptr->PB_hilfe      = my_XtNameToWidget(shell, "*PB_hilfe", TRUE))) return(1);

/* window-manager delete-button */
    XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
    wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(shell, wm_delete_window,
	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_SCHLIESSEN);

/* Callbacks registrieren */

/* Button-Callbacks */
    XtAddCallback(
        w_ptr->PB_ok, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_OK);
    XtAddCallback(
        w_ptr->PB_schliessen, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate, 
        (XtPointer)LOESUNG_SCHLIESSEN);
    XtAddCallback(
        w_ptr->T_ani_anfang, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_ANI_ANFANG);
    XtAddCallback(
        w_ptr->T_ani_ende, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_ANI_ENDE);
    XtAddCallback(
        w_ptr->T_text, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_TEXT);
    XtAddCallback(
        w_ptr->T_parameter, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_PARAMETER);
    XtAddCallback(
        w_ptr->T_max, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_MAX);
    XtAddCallback(
        w_ptr->T_min, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_MIN);
    XtAddCallback(
        w_ptr->T_dimens, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_DIMENS);
    XtAddCallback(
        w_ptr->PB_ani_anfang, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_PB_ANI_ANFANG);
    XtAddCallback(
        w_ptr->PB_ani_ende, XmNactivateCallback,
    	(XtCallbackProc)tb_loesung_activate,
        (XtPointer)LOESUNG_PB_ANI_ENDE);
    }

  XtPopup(w_ptr->S_loesung, XtGrabExclusive);

  init_loesung_interface();

/* setze vordef.Parameter f.Loesung, falls ex. u. nicht explizit gesetzt */
  if ((g_loesung.typ_gesetzt) && (!g_loesung.param_gesetzt))
  {
    XmTextFieldSetString(w_ptr->T_text, g_loesung.ausgabe);
    XmTextFieldSetString(w_ptr->T_dimens, g_loesung.einheit);
    XmTextFieldSetString(w_ptr->T_parameter, g_loesung.parameter);
    sprintf(hilfstr, "%8.3f", g_loesung.Max);
    XmTextFieldSetString(w_ptr->T_max, hilfstr);
    sprintf(hilfstr, "%8.3f", g_loesung.Min);
    XmTextFieldSetString(w_ptr->T_min, hilfstr);
    sprintf(hilfstr, "%8.3f", g_loesung.zeiten[g_loesung.anim_anfang]);
    XmTextFieldSetString(w_ptr->T_ani_anfang, hilfstr);
    sprintf(hilfstr, "%8.3f", g_loesung.zeiten[g_loesung.anim_ende]);
    XmTextFieldSetString(w_ptr->T_ani_ende, hilfstr);
  }

  return(0);
/*************************************************************************/
}

/*************************************************************************/
void pb_beenden()
/*************************************************************************/
{
  return;
/*************************************************************************/
}
