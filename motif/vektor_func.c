/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   06.06.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname            | Funktion                            */
/* ------------------------------------------------------------- */
/* init_vektor             |		        		 */
/* tb_vektor_valuechanged  |	                        	 */
/* tb_vektor_activate      |	                        	 */
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
#define START           0
#define START_SELECT    1
#define STOP            2
#define STOP_SELECT     3
#define TEMPERATUR     21
#define SPANNUNG        4
#define SP_FAKTOR       5
#define SP_FAKTOR_UP    6
#define SP_FAKTOR_DOWN  7
#define STROEMUNG       8
#define ST_FAKTOR       9
#define ST_FAKTOR_UP   10
#define ST_FAKTOR_DOWN 11
#define HUELLE_ON      12
#define HUELLE         13
#define MODELL         14 
#define PFEILE         15 
#define VTRACER        16 
#define RED_PFEILE     17 
#define ROLLEN         18 
#define BEENDEN        19 
#define OK             20 

static Widget *widgetliste;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/* tb_vektor_valuechanged                                        */
/* ************************************************************* */

/* ******************************************************************** */
void tb_vektor_valuechanged(Widget widget, int action, XtPointer cb_ptr)
/* ******************************************************************** */
{
  int i;
  int j;
  int stoffnr;
  int result;
  char *select;
  char c_hilf[12];
  int st_schritt = 10;
  int sp_schritt = 10;
  t_knoten *org_knoten;
  t_bool spannung_an,
         loesung_an,
         stroemung_an;
  float  zeit;
/* ******************************************************************** */
#ifdef DEBUG  
 if (applRessourcen.debug)
    fprintf(stdout, "** tb_vektor_valuechanged(.., >%d<, ..) **\n",action);
#endif
/* Huellfarbe setzen */
  if((!C3D_win.achtbit)&&(g_tabelle.art==1))
  {
    g_spannung.huellfarbe = FARBE_HUELLE;
  }
  else
  {
    g_spannung.huellfarbe = FARBE_HUELLE_ACHT;
  }

/* Schalter als "ungeschaltet" markieren: */
 
/* Beziehen sich die Eingaben auf Spannung oder Stroemung ? */
  spannung_an  = (g_spannung.changes[0]==1)
    ||(g_spannung.on&&(g_spannung.changes[0]==-1));
  stroemung_an = (g_stroemung.changes[0]==1)
    ||(g_stroemung.on&&(g_stroemung.changes[0]==-1));
 
/* Togglebuttonabhaengige Aktionen ------------------------------------ */
  switch (action)
  {
    case TEMPERATUR:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_temperatur))
        g_animation.temp_on = TRUE;
      else
        g_animation.temp_on = FALSE;
      break;
    }
    case SPANNUNG:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_spannung))
      {  
        g_spannung.changes[0]  = 1;
        g_stroemung.changes[0] = 0;
/*
        g_spannung.on  = TRUE;
        g_stroemung.on = FALSE;
*/
        XmToggleButtonSetState(widget_vektor.TB_stroemung, FALSE, FALSE);

        XtSetSensitive(widget_vektor.L_sp_faktor,    TRUE);
        XtSetSensitive(widget_vektor.T_sp_faktor,    TRUE);
        XtSetSensitive(widget_vektor.RC_animation,   TRUE);
        XtSetSensitive(widget_vektor.TB_huelle_on,   TRUE);
        XtSetSensitive(widget_vektor.L_deform,       TRUE);
        XtSetSensitive(widget_vektor.RC_deform,      TRUE);
        XtSetSensitive(widget_vektor.RC_stroem,     FALSE);
        XtSetSensitive(widget_vektor.L_darstellung, FALSE);
      }
      else
      {
        g_spannung.changes[0] = 0;
/*
        g_spannung.on  = FALSE;
*/
        XtSetSensitive(widget_vektor.TB_huelle_on, FALSE);
        XtSetSensitive(widget_vektor.L_deform,     FALSE);
        XtSetSensitive(widget_vektor.RC_deform,    FALSE);
      }
      break;
    }

    case STROEMUNG:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_stroemung))
      {
        g_spannung.changes[0]  = 0;
        g_stroemung.changes[0] = 1;
/*
        g_spannung.on  = FALSE;
        g_stroemung.on = TRUE;
*/
        XmToggleButtonSetState(widget_vektor.TB_spannung, FALSE, FALSE);
        XmToggleButtonSetState(widget_vektor.TB_pfeile, TRUE, FALSE);
        XtSetSensitive(widget_vektor.L_st_faktor,   TRUE);
        XtSetSensitive(widget_vektor.T_st_faktor,   TRUE);
        XtSetSensitive(widget_vektor.RC_animation,  TRUE);
        XtSetSensitive(widget_vektor.TB_huelle_on, FALSE);
        XtSetSensitive(widget_vektor.L_deform,     FALSE);
        XtSetSensitive(widget_vektor.RC_deform,    FALSE);
        XtSetSensitive(widget_vektor.RC_stroem,     TRUE);
        XtSetSensitive(widget_vektor.L_darstellung, TRUE);
        XtSetSensitive(widget_vektor.TB_tracer,    FALSE);
        XtSetSensitive(widget_vektor.TB_rollen,    FALSE);
        XtSetSensitive(widget_vektor.TB_red_pfeile, TRUE);
      }
      else
      {
        XtSetSensitive(widget_vektor.RC_stroem,     FALSE);
        XtSetSensitive(widget_vektor.L_darstellung, FALSE);
        g_stroemung.changes[0] = 0;
      }
      break;
    }

    case SP_FAKTOR:
    {
/* Lese Faktor ------------------------------------------------------- */
      select = XmTextFieldGetString(widget_vektor.T_sp_faktor);
      if (spannung_an) 
        g_spannung.factor = atof(select);
      XtFree(select);
      break;
    }

    case SP_FAKTOR_UP:
    {
      g_spannung.factor = g_spannung.factor + sp_schritt;
      sprintf(c_hilf, "%8.3f", g_spannung.factor);
      XmTextFieldSetString(widget_vektor.T_sp_faktor, c_hilf);
      break;
    }

    case SP_FAKTOR_DOWN:
    {
      if (g_spannung.factor < 1.0 + sp_schritt)
        g_spannung.factor = 1.0;
      else
        g_spannung.factor = g_spannung.factor - sp_schritt;
      sprintf(c_hilf, "%8.3f", g_spannung.factor);
      XmTextFieldSetString(widget_vektor.T_sp_faktor, c_hilf);
      break;
    }

    case ST_FAKTOR:
    {
/* Lese Faktor ------------------------------------------------------- */
      select = XmTextFieldGetString(widget_vektor.T_st_faktor);
      /*
      if (stroemung_an)
        g_stroemung.factor =  atof(select);
      */
      XtFree(select);
      g_stroemung.changes[5] = 1;
      break;
    }

    case ST_FAKTOR_UP:
    {
      
      /*
      g_stroemung.factor = g_stroemung.factor + st_schritt;
      sprintf(c_hilf, "%8.3f", g_stroemung.factor);
      XmTextFieldSetString(widget_vektor.T_st_faktor, c_hilf);
      */
      g_stroemung.changes[5] = 1;
      break;
    }

    case ST_FAKTOR_DOWN:
    {
      /*
      if (g_stroemung.factor < 1.0 + st_schritt)
        g_stroemung.factor = 1.0;
      else
        g_stroemung.factor = g_stroemung.factor - st_schritt;
      sprintf(c_hilf, "%8.3f", g_stroemung.factor);
      XmTextFieldSetString(widget_vektor.T_st_faktor, c_hilf);
      */
      g_stroemung.changes[5] = 1;
      break;
    }

/* Eingabe der Animations-Startzeit ---------------------------------- */
    case START:
    {
/* Lese Zeit --------------------------------------------------------- */
      select = XmTextFieldGetString(widget_vektor.T_start);
      zeit = atof(select);
      if (zeit <= g_animation.zeiten[0])
        g_animation.anim_anfang = 0;
      else if (zeit >= g_animation.zeiten[g_animation.anzahl_felder - 1])
        g_animation.anim_anfang = g_animation.anzahl_felder - 1;
      else
      {
        for (i = 0; i < g_animation.anzahl_felder; i++)
          if (zeit <= g_animation.zeiten[i])
            g_animation.anim_anfang = i;
      }
      XtFree(select);
      break;
    }

/* aktiviere Auswahlmenue fuer Animation-Startzeit ------------------- */
    case START_SELECT:
      g_animation.anim_anfang = 
	loes_auswahl(widget,(XtPointer)action, (XtPointer)cb_ptr) - 1;
      break;

/* Eingabe der Animations-Stopzeit ----------------------------------- */
    case STOP:
    {
/* Lese Zeit --------------------------------------------------------- */
      select = XmTextFieldGetString(widget_vektor.T_stop);
      zeit = atof(select);
      if (zeit <= g_animation.zeiten[0])
        g_animation.anim_ende = 0;
      else if (zeit >= g_animation.zeiten[g_animation.anzahl_felder - 1])
        g_animation.anim_ende = g_animation.anzahl_felder - 1;
      else
      {
        for (i = 0; i < g_animation.anzahl_felder; i++)
          if (zeit <= g_animation.zeiten[i])
            g_animation.anim_ende = i;
      }
      XtFree(select);
      break;
    }

/* aktiviere Auswahlmenue fuer Animation-Endzeit --------------------- */
    case STOP_SELECT:
      g_animation.anim_ende = 
	loes_auswahl(widget,(XtPointer)action, (XtPointer)cb_ptr) - 1;
      break;
    
/* zeichne Huelle ---------------------------------------------------- */
    case HUELLE_ON:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_huelle_on))
        g_spannung.changes[1] = 1;
      else
        g_spannung.changes[1] = 0;
      break;
    }

/* deformiere Huelle ------------------------------------------------- */
    case HUELLE:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_huelle))
        g_spannung.changes[2] = 1;
      break;
    }

/* deformiere Modell ------------------------------------------------- */
    case MODELL:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_modell))
        g_spannung.changes[2] = 0;
      break;
    }

/* Stroemungsdarstellung mit Pfeilen ---------------------------------- */
    case PFEILE:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_pfeile))
      {   
        g_stroemung.changes[1] = 1;
        g_stroemung.changes[2] = 0;
        g_stroemung.changes[3] = 0;
        g_stroemung.changes[4] = 0;
      }
      break;
    }

/* Stroemungsdarstellung mit Tracern ---------------------------------- */
    case VTRACER:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_tracer))
      {   
        g_stroemung.changes[1] = 0;
        g_stroemung.changes[2] = 0;
        g_stroemung.changes[3] = 1;
        g_stroemung.changes[4] = 0;
      }
      break;
    }

/* Stroemungsdarstellung mit reduzierten Pfeilen ---------------------- */
    case RED_PFEILE:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_red_pfeile))
      {   
        g_stroemung.changes[1] = 0;
        g_stroemung.changes[2] = 1;
        g_stroemung.changes[3] = 0;
        g_stroemung.changes[4] = 0;
      }
      break;
    }

/* Stroemungsdarstellung mit Farb-scroling ---------------------------- */
    case ROLLEN:
    {
      if (XmToggleButtonGetState(widget_vektor.TB_rollen))
      {   
        g_stroemung.changes[1] = 0;
        g_stroemung.changes[2] = 0;
        g_stroemung.changes[3] = 0;
        g_stroemung.changes[4] = 1;
      }
      break;
    }

    case BEENDEN:
    {
      /* Neue Einstellungen verwerfen */
      g_spannung.changes[0]  = -1; /* on */
      g_spannung.changes[1]  = -1; /* huelle_on */
      g_spannung.changes[2]  = -1; /* deform_huelle */
      g_spannung.changes[3]  = -1; /* anim_anfang */
      g_spannung.changes[4]  = -1; /* anim_ende */
      g_spannung.changes[5]  = -1; /* akt_index */
      
      g_stroemung.changes[0] = -1; /* on */
      g_stroemung.changes[1] = -1; /* Simpel */
      g_stroemung.changes[2] = -1; /* Red.Gitter */
      g_stroemung.changes[3] = -1; /* Tracer */
      g_stroemung.changes[4] = -1; /* Stromlinie */
      g_stroemung.changes[5] = -1; /* Fator-Update */
      
      break;
    }
    
    case OK:
    {
      if (spannung_an)
      {
        select = XmTextFieldGetString(widget_vektor.T_sp_faktor);
        g_spannung.factor = atof(select);
        g_spannung.on  = TRUE;
        g_stroemung.on = FALSE;
        if(g_spannung.changes[1] == 0)
        {
          g_spannung.huelle_on = FALSE;
        }
        else if(g_spannung.changes[1] == 1)
        {
          g_spannung.huelle_on = TRUE;
        }
        if(g_spannung.changes[2]==0)
        {
          g_spannung.verspanne_solid_huelle = 0;
        }
        if(g_spannung.changes[2]==1)
        {
          g_spannung.verspanne_solid_huelle = 1;
        }
        for (i = 0; i < g_geodat.npunkt; i++)
        {
          g_spannung.verzerrung[i].koord[0] = g_knoten[i].koord[0]
          +g_spannung.factor*g_spannung.vektor[i][0];
          g_spannung.verzerrung[i].koord[1] = g_knoten[i].koord[1]
          +g_spannung.factor*g_spannung.vektor[i][1];
          g_spannung.verzerrung[i].koord[2] = g_knoten[i].koord[2]
          +g_spannung.factor*g_spannung.vektor[i][2];
        }
      }

      else if (stroemung_an)
      {
        g_stroemung.on = 1;
        g_spannung.on  = 0;
        select = XmTextFieldGetString(widget_vektor.T_st_faktor);

        if(g_stroemung.changes[1]>0)
          g_stroemung.art = 1;
        if(g_stroemung.changes[2]>0)
          g_stroemung.art = 2;
        if(g_stroemung.changes[3]>0)
          g_stroemung.art = 3;
        if(g_stroemung.changes[4]>0)
          g_stroemung.art = 4;
        if(g_stroemung.art == 2)
          g_stroemung.gitter_reduced = FALSE;
        if(g_stroemung.changes[5] > 0)
        {
          g_stroemung.old_factor = g_stroemung.factor;
          g_stroemung.factor = atof(select);
          dehne_vektoren();
          g_stroemung.old_factor = 1.0;
        }
      }
      else if (g_spannung.changes[0]==0)
      {
        /* Spannung abschalten */
        g_spannung.on = 0;
      }
      else if (g_stroemung.changes[0]==0)
      {
        /* Stroemung abschalten */
        g_stroemung.on = 0;
      }
      XtFree(select);

/* Alle Werte als upgedatet markieren: */
      g_spannung.changes[0]  = -1; /* on */
      g_spannung.changes[1]  = -1; /* huelle_on */
      g_spannung.changes[2]  = -1; /* deform_huelle */
      g_spannung.changes[3]  = -1; /* anim_anfang */
      g_spannung.changes[4]  = -1; /* anim_ende */
      g_spannung.changes[5]  = -1; /* akt_index */

      g_stroemung.changes[0] = -1; /* on */
      g_stroemung.changes[1] = -1; /* Simpel */
      g_stroemung.changes[2] = -1; /* Red.Gitter */
      g_stroemung.changes[3] = -1; /* Tracer */
      g_stroemung.changes[4] = -1; /* Stromlinie */
      g_stroemung.changes[5] = -1; /* Fator-Update */

      C3D_postredisplay();
      break;
    }
    return;
  }
/* ******************************************************************** */
}

/* ******************************************************************** */
int init_vektor(void)
/* ******************************************************************** */
{
  int result;
  t_widget_vektor *w_ptr = &widget_vektor;
  Widget shell;
  static char hilfstr[256];
  Atom wm_delete_window;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_vektor() ***\n");
#endif
  if (w_ptr->S_vektor == NULL)
  {
    if (!(w_ptr->S_vektor = Createvektor_interface("S_vektor", toplevel, NULL, 0)))
    {
      fprintf(stderr, "Createvektor_interface(...) returns NULL\n");
      return(2);
    }
    shell = w_ptr->S_vektor;
    if (!(w_ptr->TB_temperatur = my_XtNameToWidget(shell,
                           "*TB_temperatur", TRUE))) return(1);
    if (!(w_ptr->TB_spannung = my_XtNameToWidget(shell,
                           "*TB_spannung", TRUE))) return(1);
    if (!(w_ptr->TB_stroemung = my_XtNameToWidget(shell,
                           "*TB_stroemung", TRUE))) return(1);
    if (!(w_ptr->L_sp_faktor        = my_XtNameToWidget(shell,
                           "*L_sp_faktor", TRUE))) return(1);
    if (!(w_ptr->T_sp_faktor        = my_XtNameToWidget(shell,
                           "*T_sp_faktor", TRUE))) return(1);
    if (!(w_ptr->AB_sp_faktor_up          = my_XtNameToWidget(shell,
                           "*AB_sp_faktor_up", TRUE))) return(1);
    if (!(w_ptr->AB_sp_faktor_down          = my_XtNameToWidget(shell,
                           "*AB_sp_faktor_down", TRUE))) return(1);
    if (!(w_ptr->L_st_faktor        = my_XtNameToWidget(shell,
                           "*L_st_faktor", TRUE))) return(1);
    if (!(w_ptr->T_st_faktor        = my_XtNameToWidget(shell,
                           "*T_st_faktor", TRUE))) return(1);
    if (!(w_ptr->AB_st_faktor_up          = my_XtNameToWidget(shell,
                           "*AB_st_faktor_up", TRUE))) return(1);
    if (!(w_ptr->AB_st_faktor_down          = my_XtNameToWidget(shell,
                           "*AB_st_faktor_down", TRUE))) return(1);

    if (!(w_ptr->L_animation          = my_XtNameToWidget(shell,
                           "*L_animation", TRUE))) return(1);
    if (!(w_ptr->RC_animation          = my_XtNameToWidget(shell,
                           "*RC_animation", TRUE))) return(1);
    if (!(w_ptr->PB_start          = my_XtNameToWidget(shell,
                           "*PB_start", TRUE))) return(1);
    if (!(w_ptr->PB_stop          = my_XtNameToWidget(shell,
                           "*PB_stop", TRUE))) return(1);
    if (!(w_ptr->T_start        = my_XtNameToWidget(shell,
                           "*T_start", TRUE))) return(1);
    if (!(w_ptr->T_stop        = my_XtNameToWidget(shell,
                           "*T_stop", TRUE))) return(1);
    if (!(w_ptr->TB_huelle_on          = my_XtNameToWidget(shell,
                           "*TB_huelle_on", TRUE))) return(1);
    if (!(w_ptr->L_deform          = my_XtNameToWidget(shell,
                           "*L_deform", TRUE))) return(1);
    if (!(w_ptr->RC_deform          = my_XtNameToWidget(shell,
                           "*RC_deform", TRUE))) return(1);
    if (!(w_ptr->TB_huelle          = my_XtNameToWidget(shell,
                           "*TB_huelle", TRUE))) return(1);
    if (!(w_ptr->TB_modell             = my_XtNameToWidget(shell,
                           "*TB_modell", TRUE))) return(1);
    if (!(w_ptr->L_darstellung          = my_XtNameToWidget(shell,
                           "*L_darstellung", TRUE))) return(1);
    if (!(w_ptr->RC_stroem          = my_XtNameToWidget(shell,
                           "*RC_stroem", TRUE))) return(1);
    if (!(w_ptr->TB_pfeile          = my_XtNameToWidget(shell,
                           "*TB_pfeile", TRUE))) return(1);
    if (!(w_ptr->TB_tracer             = my_XtNameToWidget(shell,
                           "*TB_tracer", TRUE))) return(1);
    if (!(w_ptr->TB_red_pfeile          = my_XtNameToWidget(shell,
                           "*TB_red_pfeile", TRUE))) return(1);
    if (!(w_ptr->TB_rollen             = my_XtNameToWidget(shell,
                           "*TB_rollen", TRUE))) return(1);
    if (!(w_ptr->PB_beenden          = my_XtNameToWidget(shell,
                           "*PB_beenden", TRUE))) return(1);
    if (!(w_ptr->PB_ok               = my_XtNameToWidget(shell,
                           "*PB_ok", TRUE))) return(1);
    if (!(w_ptr->PB_hilfe            = my_XtNameToWidget(shell,
                           "*PB_hilfe", TRUE))) return(1);

/* window-manager delete-button */
    XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
    wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW",
                                    False);

/* Callbacks registrieren */

/* Togglebutton- und Pushbutton-Callbacks ------------------- */
    XtAddCallback(
        w_ptr->TB_temperatur, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)TEMPERATUR);
    XtAddCallback(
        w_ptr->TB_spannung, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)SPANNUNG);
    XtAddCallback(
        w_ptr->TB_stroemung, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)STROEMUNG);
    XtAddCallback(
        w_ptr->T_sp_faktor, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)SP_FAKTOR);
    XtAddCallback(
        w_ptr->AB_sp_faktor_up, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)SP_FAKTOR_UP);
    XtAddCallback(
        w_ptr->AB_sp_faktor_down, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)SP_FAKTOR_DOWN);
    XtAddCallback(
        w_ptr->T_st_faktor, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)ST_FAKTOR);
    XtAddCallback(
        w_ptr->AB_st_faktor_up, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)ST_FAKTOR_UP);
    XtAddCallback(
        w_ptr->AB_st_faktor_down, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)ST_FAKTOR_DOWN);
    XtAddCallback(
        w_ptr->T_start, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)START);
    XtAddCallback(
        w_ptr->PB_start, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)START_SELECT);
    XtAddCallback(
        w_ptr->T_stop, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)STOP);
    XtAddCallback(
        w_ptr->PB_stop, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)STOP_SELECT);
    XtAddCallback(
        w_ptr->TB_huelle_on, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)HUELLE_ON);
    XtAddCallback(
        w_ptr->TB_huelle, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)HUELLE);
    XtAddCallback(
        w_ptr->TB_modell, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)MODELL);
    XtAddCallback(
        w_ptr->TB_pfeile, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)PFEILE);
    XtAddCallback(
        w_ptr->TB_tracer,XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)VTRACER);
    XtAddCallback(
        w_ptr->TB_red_pfeile, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)RED_PFEILE);
    XtAddCallback(
        w_ptr->TB_rollen, XmNvalueChangedCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)ROLLEN);
    XtAddCallback(
        w_ptr->PB_beenden, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)BEENDEN);
    XtAddCallback(
        w_ptr->PB_ok, XmNactivateCallback,
        (XtCallbackProc)tb_vektor_valuechanged,
        (XtPointer)OK);
  }

  XtPopup(w_ptr->S_vektor, XtGrabExclusive);

  init_vektor_interface();

  return(0);
/* ********************************************************************* */
}
