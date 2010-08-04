/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/* init_stoffe            |                                      */
/* fill_tt_stoffe         |       				 */
/* tt_stoffe_preedit      |	            			 */
/* tt_stoffe_activate     |	                     	         */
/* tb_stoff_activate      |		     	                 */
/* absenken               |		     	                 */
/* schiebe_stoff          |		          	         */
/* setze_sensitiv         |			                 */
/* setze_farbe            |			                 */
/* reset_transform        |			                 */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Xm/Xm.h>
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
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define STOFFE_ALLE             0
#define STOFFE_ALLE_SOLID       1
#define STOFFE_ALLE_GITTER      2
#define STOFFE_ALLE_NETZ        3
#define STOFFE_ALLE_TRANSPARENT 4
#define STOFFE_ALLE_RESULT      5
#define STOFFE_ALLE_RAHMEN      6
#define STOFFE_ABSENKEN         7
#define STOFFE_GRUPPEN          8
#define DARSTELLEN              9 
#define RESET                  10 
#define STOFFE_NAME           100
#define STOFFE_SCHLIESSEN     200
#define STOFFE_BEWEGEN_AUS    300
#define STOFFE_NOP            223

static Widget *widgetliste;
static void init_color_mat();
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* *************************************************************** */

/* ****************************************************************** */
void tb_stoff_activate(Widget widget, int action, XtPointer cb_ptr)
/* ****************************************************************** */
{
  int i;
  int j;
  int stoffnr;
  int result;
  int anz_select;
  int *select;
  char *name;
  char *strptr;
  int nrows;
  int pos_count;
  int *pos_list;
  int *id_list;
  static char hilfstr[4096];
  static char hilfstr2[4096];
  static t_bool alle_stoffe = FALSE;
/* ****************************************************************** */
#ifdef DEBUG  
 if (applRessourcen.debug)
    fprintf(stdout, "*** tb_stoff_activate(.., >%d<, ..) ***\n",action);
#endif
  if (XmToggleButtonGetState(widget_stoffe.TB_alle_stoffe))
    alle_stoffe = TRUE;         /* Einstellung f. alle Stoffe gleich  */

/* hole selektierte Stoffe ------------------------------------------ */
  if (!alle_stoffe)
  {
  }

/* Togglebuttonabhaengige Aktionen ---------------------------------- */
  switch (action)
  {
    case STOFFE_ALLE:
    {
      if (applRessourcen.debug) fprintf(stdout, "STOFFEALL\n");

/* Stofftabelle aktivieren / inaktivieren --------------------------- */

      if (XmToggleButtonGetState(widget_stoffe.TB_alle_stoffe))
      {
        alle_stoffe = TRUE;
        XtSetSensitive(widget_stoffe.TT_stoffgebiete, FALSE);
      }
      else
      {
        alle_stoffe = FALSE;
        XtSetSensitive(widget_stoffe.TT_stoffgebiete, TRUE);
      }
      break;
    }

    case STOFFE_ALLE_SOLID:
    {
      g_transform.rahmen_on = FALSE;
        if (XmToggleButtonGetState(widget_stoffe.TB_solid))
        {
          if (XmToggleButtonGetState(widget_stoffe.TB_netz))
            set_maske(SETZE_ALLE, NULL, ZEICHNE_SOLID_NETZ);
          else
            set_maske(SETZE_ALLE, NULL, ZEICHNE_SOLID);
          C3D_postredisplay();
          fill_tt_stoffe();
        }
      break;
    }

    case STOFFE_ALLE_GITTER:
    {
      g_transform.rahmen_on = FALSE;
      if (XmToggleButtonGetState(widget_stoffe.TB_gitter))
      {
        set_maske(SETZE_ALLE, NULL, ZEICHNE_GITTER);
        C3D_postredisplay();
        fill_tt_stoffe();
      }
      break;
    }

    case STOFFE_ALLE_TRANSPARENT:
    {
      g_transform.rahmen_on = FALSE;
      if (XmToggleButtonGetState(widget_stoffe.TB_transparent))
      {
        if (XmToggleButtonGetState(widget_stoffe.TB_netz))
          set_maske(SETZE_ALLE, NULL, ZEICHNE_TRANSPARENT_NETZ);
        else
          set_maske(SETZE_ALLE, NULL, ZEICHNE_TRANSPARENT);
        C3D_postredisplay();
        fill_tt_stoffe();
      }
      break;
    }

    case STOFFE_ALLE_NETZ:
    {
      g_transform.rahmen_on = FALSE;
      if (XmToggleButtonGetState(widget_stoffe.TB_netz))
      {
        set_maske(SETZE_ALLE, NULL, ZEICHNE_NETZ);
        C3D_postredisplay();
        fill_tt_stoffe();
      }
      else
      {
/* Netz ausschalten ---------------------------------------- */
        netz_aus();
        C3D_postredisplay();
        fill_tt_stoffe();
      }
      break;
    }

    case STOFFE_ALLE_RESULT:
    {
      g_transform.rahmen_on = FALSE;
      if (XmToggleButtonGetState(widget_stoffe.TB_result))
      {
        if (XmToggleButtonGetState(widget_stoffe.TB_netz))
          set_maske(SETZE_ALLE, NULL, ZEICHNE_RESULT_NETZ);
        else
          set_maske(SETZE_ALLE, NULL, ZEICHNE_RESULT);
        C3D_postredisplay();
        fill_tt_stoffe();
      }
      break;
    }

    case STOFFE_ABSENKEN:
    {
      if (XmToggleButtonGetState(widget_stoffe.TB_absenken))
      {
        if (g_ofen.ex)
          g_transform.absenken_on = TRUE;
          
        C3D_postredisplay();
      }
      else
      {
        g_transform.absenken_on = FALSE;
      }
      fill_tt_stoffe();
      break;
    }

    case STOFFE_GRUPPEN:
    {
      g_transform.rahmen_on = FALSE;
      if (XmToggleButtonGetState(widget_stoffe.TB_solid))
      {
        set_maske(SETZE_ALLE, NULL, ZEICHNE_RESULT);
        C3D_postredisplay();
      }
      fill_tt_stoffe();
      break;
    }

    case STOFFE_ALLE_RAHMEN:
    {
      if (XmToggleButtonGetState(widget_stoffe.TB_rahmen))
      {
        set_maske(SETZE_ALLE, NULL, ZEICHNE_RAHMEN);
        g_transform.rahmen_on = TRUE;
        C3D_postredisplay();
      }
      fill_tt_stoffe();
      break;
    }
    
    case DARSTELLEN:
    {
      C3D_postredisplay();
      break;
    }
    
    case RESET:
    {
      set_maske(SETZE_ALLE, NULL, ZEICHNE_SOLID);
      set_maske(SETZE_ALLE, NULL, ZEICHNE_NETZ);
      reset_transform();
      C3D_postredisplay();
      break;
    }

    case STOFFE_SCHLIESSEN:
    {
      if (verschiebung_on)
      {
        XtPopdown(widget_stoffe.S_verschiebung);
        setze_sensitiv(TRUE);
        verschiebung_on = FALSE;
      }

      XtPopdown(widget_stoffe.S_stoffe);
      C3D_postredisplay();
      break;
    }

    case STOFFE_BEWEGEN_AUS:
    {
      setze_sensitiv(TRUE);
      verschiebung_on = FALSE;
      XtPopdown(widget_stoffe.S_verschiebung);
      break;
    }

    case STOFFE_NOP:
    {
      C3D_postredisplay();
      break;
    }

  return;
  }
/* ****************************************************************** */
}

/* ****************************************************************** */
int fill_tt_stoffe()
/* ****************************************************************** */
{
  int i, j;
  int darstellung[3];
  char verschiebung[9];
  char Value[3][81];
  t_stoffgebiet *stoffgebiet;
  static char *d_mal_set[] = {"Solid", "Result", "Transp.", "Gitter"};
  static char *e_mal_set[] = {"Solid", "Result", "Transp.", "Gitter"};
  static char **p_set;
/* ****************************************************************** */
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** fill_tt_stoffe() ***\n");
#endif
  XtVaSetValues(widget_stoffe.TT_stoffgebiete,
                BXmNrecomputeLayout, FALSE, NULL);
  XtVaSetValues(widget_stoffe.TT_stoffgebiete, BXmNnumRows,
                g_geodat.nstoff, NULL);
  BXmTextTableSetNumColumns(widget_stoffe.TT_stoffgebiete, 12);

  for(i = 0; i < g_geodat.nstoff; i++)
  {
    sprintf(Value[0], "%d", g_stoffgebiete[i].nr);
    strcpy(Value[1], g_stoffgebiete[i].name);
    strcpy(Value[2], " - ");
    BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,1,/* Name */
                         Value[0]);
    BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,2,/* Nr.  */
                         Value[1]);
    BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,3,/*Gruppe*/
                         Value[2]);
/* setze Art der Darstellung ---------------------------------------- */
/*
    if (g_sprache == SPRACHE_DEUTSCH)
      p_set = d_temp_set;
    else
      p_set = e_temp_set;
*/
    get_darstellung(i + 1, darstellung, verschiebung);

    for (j = 4; j < 8; j++)
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1, j, " ");

    if (darstellung[0] == 5)
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1, 7, " * ");
    else if (darstellung[0] > 0)
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,
          darstellung[0] + 3, " * ");

    if (darstellung[1])
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete,i+1,8," * ");
    else
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete,i+1,8," ");

    if (darstellung[2])
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete,i+1,9," * ");
    else
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete,i+1,9," ");

    if (verschiebung != NULL)
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,
            10, verschiebung);
    else
      BXmTextTableSetField(widget_stoffe.TT_stoffgebiete, i+1,
            10, " ");
  }

  init_color_mat();
  XtVaSetValues(widget_stoffe.TT_stoffgebiete,
                BXmNrecomputeLayout, TRUE, NULL);

  BXmTextTableDeselectAll(widget_stoffe.TT_stoffgebiete);

  return(0);
}
/* ****************************************************************** */

static void init_color_mat()
{
  static char color_string[20];
  static t_bool initialized = FALSE;
  short r,g,b;
  int pac;
  int i;
  int colormap_index;

  if ( initialized )
     return;

  if((!C3D_win.achtbit) && (g_tabelle.art==1))
     colormap_index = STOFFARB_OFFSET + 1;

  for (i = 0; i < g_geodat.nstoff; i++)
  {
    r = g_tabelle.colortable[4*(colormap_index+i+1)+1];
    g = g_tabelle.colortable[4*(colormap_index+i+1)+2]; 
    b = g_tabelle.colortable[4*(colormap_index+i+1)+3]; 
    pac = PACK_COLOR(b,g,r,0);
    sprintf(color_string, "#%06x", pac);
    BXmTextTableSetBackgroundColor( widget_stoffe.TT_stoffgebiete, i+1, 
	11, (String) color_string );
  }

  initialized = TRUE;
}

/* ****************************************************************** */
int get_darstellung(int stoffnr, int *darstellung, char *verschiebung)
/* ****************************************************************** */
{
  int status, i;

  if (stoffnr <= 0 || stoffnr > g_geodat.nstoff)
  {
    warning(" invalid material number");
    return -1;
  }

  darstellung[0] = darstellung[1] = darstellung[2] = 0;

  status = g_Maske[stoffnr];

  switch (status)
  {
    case 1:                                     /* solid */
     darstellung[0] = 1;
     break;
    case 2:                                     /* Gitter */
     darstellung[0] = 2;
     break;
    case 3:                                     /* transparent */
     darstellung[0] = 3;
     break;
    case 4:                                     /* Netz */
     darstellung[1] = 1;
     break;
    case 5:                                     /* result */
     darstellung[0] = 5;
     break;
    case 6:                                     /* solid + Netz */
     darstellung[0] = 1;
     darstellung[1] = 1;
     break;
    case 7:                                     /* transparent + Netz */
     darstellung[0] = 3;
     darstellung[1] = 1;
     break;
    case 8:                                     /* result + Netz */
     darstellung[0] = 5;
     darstellung[1] = 1;
     break;
  }

/* Absenkung im Ofen ------------------------------------------- */
  for (i = 0; i < g_ofen.anz_stofen; i++)
    if (g_ofen.stofen[i] == stoffnr)
      darstellung[2] = 1;

/* Verschiebung der Geometrie ---------------------------------- */
  strcpy(verschiebung, "");
  if (g_stoffgebiete[stoffnr-1].verschiebung[0] > EPS)  
    strcat(verschiebung, "+x,");
  else if (g_stoffgebiete[stoffnr-1].verschiebung[0] < -EPS)
    strcat(verschiebung, "-x,");
  if (g_stoffgebiete[stoffnr-1].verschiebung[1] > EPS)  
    strcat(verschiebung, "+y,");
  else if (g_stoffgebiete[stoffnr-1].verschiebung[1] < -EPS)
    strcat(verschiebung, "-y,");
  if (g_stoffgebiete[stoffnr-1].verschiebung[2] > EPS)  
    strcat(verschiebung, "+z");
  else if (g_stoffgebiete[stoffnr-1].verschiebung[2] < -EPS)
    strcat(verschiebung, "-z");

  return 0;
/* ****************************************************************** */
}

/* ****************************************************************** */
/* TextTable-widget Stoffgruppen fuer Stoffgruppendefinition          */
/* ****************************************************************** */
void tt_stoffe_preedit(Widget widget, XtPointer client_data,
                       BXmTextTablePreEditCallbackStruct *cb_ptr)
/* ****************************************************************** */
{
  int i;
  char *strptr;
  char wert[81];
  int aktuelle_stoffnr;
  int n_selected;
  int setting;
  static char hilfstr[4096];
  int result;
  static char *d_mal_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
  static char *e_mal_set[]  = {"Solid", "Gitter", "Transp.", "Rahmen" };
  static char **p_set;
  int darstellung[3];
  char verschiebung[9];
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tt_stoffe_preedit() ***\n");
#endif
/* Feld nicht editieren --------------------------------------------- */
  cb_ptr->doit = FALSE;

/* falls aktuelle Zeile nicht selektiert ist, wird diese selektiert */

/* hole Stoff-Ptr. -------------------------------------------------- */
      BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
      aktuelle_stoffnr = atoi(strptr);

    get_darstellung(aktuelle_stoffnr, darstellung, verschiebung);
  
/* spaltenabhaengige Aktionen --------------------------------------- */
  switch (cb_ptr->currentColumn)
  {
    case 1:			/* Nummer --------------------------- */
    case 2:			/* Name ----------------------------- */
    case 3:			/* Gruppe --------------------------- */
    {
/* falls aktuelle Zeile nicht selektiert ist, wird diese selektiert */
      if (BXmTextTablePosIsSelected (widget, cb_ptr->currentRow))
        BXmTextTableDeselect(widget, cb_ptr->currentRow, FALSE);
       else
        BXmTextTableSelect(widget, cb_ptr->currentRow, FALSE);
      break;
    }
    case 4:			/* Solid ---------------------------- */
    {
      set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_SOLID);
      if (darstellung[1])
        set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_SOLID_NETZ);
      break;
    }
    case 5:			/* Gitter --------------------------- */
    {
      set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_GITTER);
      break;
    }
    case 6:			/* Transparent ---------------------- */
    {
      set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_TRANSPARENT);
      if (darstellung[1])
        set_maske(SETZE_STOFF_PLUS,aktuelle_stoffnr,
                  ZEICHNE_TRANSPARENT_NETZ);
      break;
    }
    case 7:			/* Result --------------------------- */
    {
      if (g_loesung.gescannt)
      {
        set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_RESULT);
        if (darstellung[1])
          set_maske(SETZE_STOFF_PLUS,aktuelle_stoffnr,ZEICHNE_RESULT_NETZ);
      }
      break;
    }
    case 8:			/* Netz	----------------------------- */
    {
      if (darstellung[1])
        set_maske(SETZE_NETZ_MINUS, aktuelle_stoffnr, ZEICHNE_NETZ);
      else
        set_maske(SETZE_STOFF_PLUS, aktuelle_stoffnr, ZEICHNE_NETZ);
      break;
    }
    case 9:			/* absenken ------------------------- */
    {
      absenken();
      break;
    }
    case 10:			/* verschieben ---------------------- */
    {
      schiebe_stoff(aktuelle_stoffnr);
      break;
    }
    case 11:			/* Farbe ---------------------------- */
    {
      setze_farbe(widget, aktuelle_stoffnr);
      break;
    }
  }
  fill_tt_stoffe();

/*
  XtVaGetValues(widget, BXmNselectedPosCount, &n_selected, NULL);
  XtSetSensitive(widget_stoffgruppen.PB_delete, n_selected);
  XtSetSensitive(widget_stoffgruppen.PB_show_stoffgruppe, n_selected);
*/
  return;
}
/* ****************************************************************** */

/* ****************************************************************** */
void tt_stoffe_activate(Widget widget, XtPointer client_data,
                        BXmTextTableActivateCallbackStruct *cb_ptr)
/* ****************************************************************** */
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tt_stoffe_activate() ***\n");
#endif
/* naechstes Feld nicht editieren ----------------------------------- */
  cb_ptr->editing = FALSE;

  return;
}
/* ****************************************************************** */

/* ****************************************************************** */
int init_stoffe(void)
/* ****************************************************************** */
{
  int result;
  t_widget_stoffe *w_ptr = &widget_stoffe;
  Widget shell;
  static char hilfstr[256];
  Atom wm_delete_window;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_stoffe() ***\n");
#endif
  if (w_ptr->S_stoffe == NULL)
  {
    if (!(w_ptr->S_stoffe = Createstoffe_interface("S_stoffe", toplevel, NULL, 0)))
    {
      fprintf(stderr, "Createstoffe_interface(...) returns NULL\n");
      return(2);
    }
    shell = w_ptr->S_stoffe;
    if (!(w_ptr->TB_alle_stoffe = my_XtNameToWidget(shell, "*TB_alle_stoffe", TRUE))) return(1);
    if (!(w_ptr->RC_allestoffe  = my_XtNameToWidget(shell, "*RC_allestoffe", TRUE))) return(1);
    if (!(w_ptr->TB_solid       = my_XtNameToWidget(shell, "*TB_solid", TRUE))) return(1);
    if (!(w_ptr->TB_gitter      = my_XtNameToWidget(shell, "*TB_gitter", TRUE))) return(1);
    if (!(w_ptr->TB_transparent = my_XtNameToWidget(shell, "*TB_transparent", TRUE))) return(1);
    if (!(w_ptr->TB_result      = my_XtNameToWidget(shell, "*TB_result", TRUE))) return(1);
    if (!(w_ptr->TB_netz        = my_XtNameToWidget(shell, "*TB_netz", TRUE))) return(1);
    if (!(w_ptr->TB_absenken    = my_XtNameToWidget(shell, "*TB_absenken",
TRUE))) return(1);
    if (!(w_ptr->TB_stoffgruppen= my_XtNameToWidget(shell, "*TB_stoffgruppen", TRUE))) return(1);
    if (!(w_ptr->TB_rahmen      = my_XtNameToWidget(shell, "*TB_rahmen", TRUE))) return(1);
    if (!(w_ptr->PB_beenden     = my_XtNameToWidget(shell, "*PB_beenden", TRUE))) return(1);
    if (!(w_ptr->PB_hilfe       = my_XtNameToWidget(shell, "*PB_hilfe", TRUE))) return(1);
    if (!(w_ptr->PB_darstellen  = my_XtNameToWidget(shell, "*PB_darstellen", TRUE))) return(1);
    if (!(w_ptr->PB_reset       = my_XtNameToWidget(shell, "*PB_reset", TRUE))) return(1);
    if (!(w_ptr->TT_stoffgebiete= my_XtNameToWidget(shell, "*TT_stoffgebiete", TRUE))) return(1);
    if (!(w_ptr->S_verschiebung = my_XtNameToWidget(shell, "*S_verschiebung", TRUE))) return(1);
    shell = w_ptr->S_verschiebung;
    if (!(w_ptr->PB_ok          = my_XtNameToWidget(shell, "*PB_ok", TRUE))) return(1);

/* window-manager delete-button */
    XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, 0);
    wm_delete_window = XmInternAtom(XtDisplay(shell),"WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(shell, wm_delete_window,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_NOP);

    shell = w_ptr->S_stoffe;
    XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, 0);
    wm_delete_window = XmInternAtom(XtDisplay(shell),"WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(shell, wm_delete_window,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_NOP);

/* Callbacks registrieren */
/* Callbacks fuer Stofftabelle ------------------------------ */
    XtAddCallback(
        w_ptr->TT_stoffgebiete, XmNpreEditCallback,
        (XtCallbackProc)tt_stoffe_preedit,
        (XtPointer)NULL);
    XtAddCallback(
        w_ptr->TT_stoffgebiete, XmNactivateCallback,
        (XtCallbackProc)tt_stoffe_activate,
        (XtPointer)NULL);
    XtOverrideTranslations(
        BXmTextTableGetChild(w_ptr->TT_stoffgebiete, BXmCHILD_INPUT),
        TF_in_TT_translations_table);

/* Togglebutton- und Pushbutton-Callbacks ------------------- */
    XtAddCallback(
        w_ptr->TB_alle_stoffe, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE);
    XtAddCallback(
        w_ptr->TB_solid, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_SOLID);
    XtAddCallback(
        w_ptr->TB_gitter, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_GITTER);
    XtAddCallback(
        w_ptr->TB_netz, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_NETZ);
    XtAddCallback(
        w_ptr->TB_transparent, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_TRANSPARENT);
    XtAddCallback(
        w_ptr->TB_result, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_RESULT);
    XtAddCallback(
        w_ptr->TB_absenken, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ABSENKEN);
    XtAddCallback(
        w_ptr->TB_stoffgruppen, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_GRUPPEN);
    XtAddCallback(
        w_ptr->TB_rahmen, XmNvalueChangedCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_ALLE_RAHMEN);
    XtAddCallback(
        w_ptr->PB_darstellen, XmNactivateCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)DARSTELLEN);
    XtAddCallback(
        w_ptr->PB_reset, XmNactivateCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)RESET);
    XtAddCallback(
        w_ptr->PB_beenden, XmNactivateCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_SCHLIESSEN);
    XtAddCallback(
        w_ptr->PB_ok, XmNactivateCallback,
        (XtCallbackProc)tb_stoff_activate,
        (XtPointer)STOFFE_BEWEGEN_AUS);
  }

  XtPopup(w_ptr->S_stoffe, XtGrabNone);

  init_stoffe_interface();
  fill_tt_stoffe();

  return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
int absenken()
/* ****************************************************************** */
{
  return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
int schiebe_stoff(int stoff)
/* ****************************************************************** */
{
/* Alle Widgets im Hauptfenster insensitiv setzen ausser "move" ----- */
/* Abbruch mit Mouseclick in der Drawingarea ------------------------ */
   verschiebung_on = TRUE;
   verschiebe_stoff = stoff;
   setze_sensitiv(FALSE);
   XtPopup(widget_stoffe.S_verschiebung, XtGrabNone);
  return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
int setze_farbe(Widget widget, int stoff)
/* ****************************************************************** */
{
  Widget colchooser;
  Arg args[1];

  args[0].value = stoff;
  args[0].name = "current material";
  colchooser = CreateColorChooser("S_mat_color_chooser", widget, args, 1);
  
 return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
/* Change the color index in color-table.                             */
/* ****************************************************************** */
int change_mat_col(int stoffnr)
{
#ifdef HAVE_SGI
  int colormap_index;
  if((!C3D_win.achtbit) && (g_tabelle.art==1))
     colormap_index = STOFFARB_OFFSET + 1;

  g_tabelle.colortable[4*(colormap_index+stoffnr)+1] = chcolor.color[0];
  g_tabelle.colortable[4*(colormap_index+stoffnr)+2] = chcolor.color[1];
  g_tabelle.colortable[4*(colormap_index+stoffnr)+3] = chcolor.color[2];
 
  // set color on the widget
  BXmTextTableSetBackgroundColor(widget_stoffe.TT_stoffgebiete, stoffnr, 
	11, (String) chcolor.color_string); 	
  
#endif
}

/* ****************************************************************** */
int setze_sensitiv(t_bool set)
/* ****************************************************************** */
{
  XtSetSensitive(widgetMain.CB_datei,               set);
  XtSetSensitive(widgetMain.CB_transformieren,      set);
  XtSetSensitive(widgetMain.CB_2dgrafik,            set);
  XtSetSensitive(widgetMain.CB_isolinien,           set);
  XtSetSensitive(widgetMain.CB_stroem_spann,        set);
  XtSetSensitive(widgetMain.PB_rot_minus_x,         set);
  XtSetSensitive(widgetMain.PB_rot_minus_xx,        set);
  XtSetSensitive(widgetMain.PB_rot_plus_x,          set);
  XtSetSensitive(widgetMain.PB_rot_plus_xx,         set);
  XtSetSensitive(widgetMain.PB_rot_minus_y,         set);
  XtSetSensitive(widgetMain.PB_rot_minus_yy,        set);
  XtSetSensitive(widgetMain.PB_rot_plus_y,          set);
  XtSetSensitive(widgetMain.PB_rot_plus_yy,         set);
  XtSetSensitive(widgetMain.PB_rot_minus_z,         set);
  XtSetSensitive(widgetMain.PB_rot_minus_zz,        set);
  XtSetSensitive(widgetMain.PB_rot_plus_z,          set);
  XtSetSensitive(widgetMain.PB_rot_plus_zz,         set);
  XtSetSensitive(widgetMain.PB_zoom_14x,            set);
  XtSetSensitive(widgetMain.PB_zoom_07x,            set);

  if (g_loesung.gelesen)
  {
/*
    XtSetSensitive(widgetMain.CB_loesung,             set);
*/
    XtSetSensitive(widgetMain.PB_animation_start,     set);
    XtSetSensitive(widgetMain.PB_animation_stop,      set);
    XtSetSensitive(widgetMain.T_zeitschritt,          set);
    XtSetSensitive(widgetMain.PB_zeitschritt,         set);
    XtSetSensitive(widgetMain.PB_zeitschritt_vor,     set);
    XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, set);
  }

  return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
setze_sensitiv_alle(t_bool set)
/* ****************************************************************** */
{
  XtSetSensitive(widgetMain.CB_datei,               set);
  XtSetSensitive(widgetMain.CB_transformieren,      set);
  XtSetSensitive(widgetMain.CB_2dgrafik,            set);
  XtSetSensitive(widgetMain.CB_isolinien,           set);
  XtSetSensitive(widgetMain.CB_stroem_spann,        set);
/*
  XtSetSensitive(widgetMain.CB_loesung,             set);
*/
  assert(XtIsWidget(widgetMain.PB_rot_minus_x));
  XtSetSensitive(widgetMain.PB_rot_minus_x,         set);
  XtSetSensitive(widgetMain.PB_rot_plus_x,          set);
  XtSetSensitive(widgetMain.PB_rot_minus_y,         set);
  XtSetSensitive(widgetMain.PB_rot_plus_y,          set);
  XtSetSensitive(widgetMain.PB_rot_minus_z,         set);
  XtSetSensitive(widgetMain.PB_rot_plus_z,          set);
  XtSetSensitive(widgetMain.PB_move_minus_x,        set);
  XtSetSensitive(widgetMain.PB_move_plus_x,         set);
  XtSetSensitive(widgetMain.PB_move_minus_y,        set);
  XtSetSensitive(widgetMain.PB_move_plus_y,         set);
  XtSetSensitive(widgetMain.PB_move_minus_z,        set);
  XtSetSensitive(widgetMain.PB_move_plus_z,         set);
  assert(XtIsWidget(widgetMain.PB_zoom_14x));
  XtSetSensitive(widgetMain.PB_zoom_14x,            set);
  XtSetSensitive(widgetMain.PB_zoom_07x,            set);
  XtSetSensitive(widgetMain.PB_animation_start,     set);
  XtSetSensitive(widgetMain.T_zeitschritt,          set);
  XtSetSensitive(widgetMain.PB_zeitschritt,         set);
  XtSetSensitive(widgetMain.PB_zeitschritt_vor,     set);
  XtSetSensitive(widgetMain.PB_zeitschritt_zurueck, set);
  if (set)
    XtSetSensitive(widgetMain.PB_animation_stop,    set);
  else
    XtSetSensitive(widgetMain.PB_animation_stop,   !set);
  return(0);
/* ****************************************************************** */
}

/* ****************************************************************** */
int reset_transform()
/* ****************************************************************** */
{
  int i;

  for (i = 0; i < g_geodat.nstoff; i++)
  {
    g_stoffgebiete[i].verschiebung[0] = 0.0;
    g_stoffgebiete[i].verschiebung[1] = 0.0;
    g_stoffgebiete[i].verschiebung[2] = 0.0;
  }
  return(0);
/* ****************************************************************** */
}
