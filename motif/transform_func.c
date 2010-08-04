/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname          | Funktion                              */
/* ------------------------------------------------------------- */
/* init_transformieren   | 		                	 */
/* fill_tt_transform     |  					 */
/* tt_transform_preedit  |	       				 */
/* tt_transform_activate |	                		 */
/* transform_cb          |			       		 */
/* get_schnitt_koord     |			       		 */
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
#include <Xm/TextF.h>
#include "color3d.h"
#include "motif.h"
#include "grafik.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define TRANSFORM_ZYKLUS           0
#define TRANSFORM_ANZAHL_ZYKLEN    1
#define TRANSFORM_ANZAHL_PLUS      2
#define TRANSFORM_ANZAHL_MINUS     3
#define TRANSFORM_WINKEL           4
#define TRANSFORM_X_ACHSE          5
#define TRANSFORM_Y_ACHSE          6
#define TRANSFORM_Z_ACHSE          7
#define TRANSFORM_SPIEGELN        10
#define TRANSFORM_XY_EBENE        11
#define TRANSFORM_XZ_EBENE        12
#define TRANSFORM_YZ_EBENE        13
#define TRANSFORM_SCHNEIDEN       20
#define TRANSFORM_SCHNEIDEN_AUS   21
#define TRANSFORM_DEF_KOORD       22
#define TRANSFORM_PUNKT1_X        23
#define TRANSFORM_PUNKT1_Y        24
#define TRANSFORM_PUNKT1_Z        25
#define TRANSFORM_PUNKT2_X        26
#define TRANSFORM_PUNKT2_Y        27
#define TRANSFORM_PUNKT2_Z        28
#define TRANSFORM_PUNKT3_X        29
#define TRANSFORM_PUNKT3_Y        30
#define TRANSFORM_PUNKT3_Z        31
#define TRANSFORM_DEF_NUMMER      40
#define TRANSFORM_PUNKTNR_1       41
#define TRANSFORM_PUNKTNR_2       42
#define TRANSFORM_PUNKTNR_3       43
#define TRANSFORM_ZENTRIEREN      44
#define TRANSFORM_DIS_CUTTING     45
#define TRANSFORM_ABBRECHEN      199
#define TRANSFORM_OK             200
#define TRANSFORM_DEF_PLANE      201

static Widget *widgetliste;
int g_dis_cutting[100];

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/*****************************************************************/
/*************************************************************************/

void addToDisCuttingCB(Widget w, XtPointer clientData, XtPointer callData);
/*************************************************************************/
void transform_cb(Widget widget, int action, XtPointer cb_ptr)
/*************************************************************************/
{
  int i;
  int j;
  char *name;
  char *strptr;
  int nrows;
  int pos_count;
  int *pos_list;
  int *id_list;
  static char hilfstr[4096];
  static char hilfstr2[4096];
  int anzahl;
  float winkel;
  char c_anzahl[2];
  t_koord punkt1, punkt2, punkt3;
  int nummer;
  //static t_bool schneiden_on;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** transform_cb(..., >%d<, ...) ***\n",action);
#endif

/*	eingabeabhaengige Aktionen */
  switch (action)
  {
    case TRANSFORM_ZYKLUS:
    {
/* Wert fuer Zyklus entsprechend des Togglebuttons setzen ----------- */
      if (XmToggleButtonGetState(widget_transform.TB_zyklus))
      {
        g_transform.zyklus_on = TRUE;
        XtSetSensitive(widget_transform.T_anz_zyk,       TRUE);
        XtSetSensitive(widget_transform.AB_anzahl_plus,  TRUE);
        XtSetSensitive(widget_transform.AB_anzahl_minus, TRUE);
        XtSetSensitive(widget_transform.T_winkel,        TRUE);
        XtSetSensitive(widget_transform.TB_achsex,       TRUE);
        XtSetSensitive(widget_transform.TB_achsey,       TRUE);
        XtSetSensitive(widget_transform.TB_achsez,       TRUE);

        XtSetSensitive(widget_transform.TB_spiegeln,     FALSE);
        XtSetSensitive(widget_transform.TB_schneiden,    FALSE);
      }
      else 
      {
        g_transform.zyklus_on = FALSE;
        XtSetSensitive(widget_transform.T_anz_zyk,       FALSE);
        XtSetSensitive(widget_transform.AB_anzahl_plus,  FALSE);
        XtSetSensitive(widget_transform.AB_anzahl_minus, FALSE);
        XtSetSensitive(widget_transform.T_winkel,        FALSE);
        XtSetSensitive(widget_transform.TB_achsex,       FALSE);
        XtSetSensitive(widget_transform.TB_achsey,       FALSE);
        XtSetSensitive(widget_transform.TB_achsez,       FALSE);

        XtSetSensitive(widget_transform.TB_spiegeln,     TRUE);
        XtSetSensitive(widget_transform.TB_schneiden,    TRUE);
      }
      break;
    }

    case TRANSFORM_ANZAHL_ZYKLEN:
    {
      anzahl = atoi(XmTextFieldGetString(widget_transform.T_anz_zyk));
      break;
    }

    case TRANSFORM_ANZAHL_PLUS:
    {
      g_transform.zyklus_anz++;
      sprintf(c_anzahl, "%d", g_transform.zyklus_anz);
      XmTextFieldSetString(widget_transform.T_anz_zyk, c_anzahl);
      break;
    }

    case TRANSFORM_ANZAHL_MINUS:
    {
      if (g_transform.zyklus_anz > 0) g_transform.zyklus_anz--;
      sprintf(c_anzahl, "%d", g_transform.zyklus_anz);
      XmTextFieldSetString(widget_transform.T_anz_zyk, c_anzahl);
      break;
    }

    case TRANSFORM_WINKEL:
    {
      winkel = atof(XmTextFieldGetString(widget_transform.T_winkel));
      g_transform.zyklus_ang = winkel;
      break;
    }

    case TRANSFORM_X_ACHSE:
    {
      g_transform.zyklus_achse = 'x';
      break;
    }

    case TRANSFORM_Y_ACHSE:
    {
      g_transform.zyklus_achse = 'y';
      break;
    }

    case TRANSFORM_Z_ACHSE:
    {
      g_transform.zyklus_achse = 'z';
      break;
    }

    case TRANSFORM_SPIEGELN:
    {
      /* Flagge fuer Spiegelung entsprechend des Togglebuttons setzen -------- */
      if (XmToggleButtonGetState(widget_transform.TB_spiegeln))
      {
        g_transform.spiegel_on = TRUE;
        XtSetSensitive(widget_transform.TB_ebenexy, TRUE);
        XtSetSensitive(widget_transform.TB_ebenexz, TRUE);
        XtSetSensitive(widget_transform.TB_ebeneyz, TRUE);

        XtSetSensitive(widget_transform.TB_schneiden, FALSE);
        XtSetSensitive(widget_transform.TB_zyklus ,  FALSE);
      }
      else
      {
        g_transform.spiegel_on = FALSE;
        XtSetSensitive(widget_transform.TB_ebenexy, FALSE);
        XtSetSensitive(widget_transform.TB_ebenexz, FALSE);
        XtSetSensitive(widget_transform.TB_ebeneyz, FALSE);

        XtSetSensitive(widget_transform.TB_schneiden, TRUE);
        XtSetSensitive(widget_transform.TB_zyklus ,  TRUE);
      }
      break;
    }

    case TRANSFORM_XY_EBENE:
    {
      if (XmToggleButtonGetState(widget_transform.TB_ebenexy))
      {
        g_transform.mirrorx = TRUE;
        g_transform.mirrory = FALSE;
        g_transform.mirrorz = FALSE;
      }
      else
        g_transform.mirrorx = FALSE;
      break;
    }

    case TRANSFORM_XZ_EBENE:
    {
      if (XmToggleButtonGetState(widget_transform.TB_ebenexz))
      {
        g_transform.mirrorx = FALSE;
        g_transform.mirrory = TRUE;
        g_transform.mirrorz = FALSE;
      }
      else
        g_transform.mirrory = FALSE;
      break;
    }

    case TRANSFORM_YZ_EBENE:
    {
      if (XmToggleButtonGetState(widget_transform.TB_ebeneyz))
      {
        g_transform.mirrorx = FALSE;
        g_transform.mirrory = FALSE;
        g_transform.mirrorz = TRUE;
      }
      else
        g_transform.mirrorz = FALSE;
      break;
    }

    case TRANSFORM_SCHNEIDEN:
    {
      /* Flagge fuer Schnitt entsprechend des Togglebuttons setzen -------- */
      if (XmToggleButtonGetState(widget_transform.TB_schneiden))
      {
        XmToggleButtonSetState(widget_transform.TB_schneiden_aus,
                               FALSE, FALSE);
        XtSetSensitive(widget_transform.TB_schneiden_aus, TRUE);
        g_transform.schneiden_on = TRUE;
        XtSetSensitive(widget_transform.TB_def_koord, TRUE);
        XtSetSensitive(widget_transform.TB_def_nummer, TRUE);
	XtSetSensitive(widget_transform.TB_def_plane, TRUE);

        XtSetSensitive(widget_transform.TB_spiegeln, FALSE);
        XtSetSensitive(widget_transform.TB_zyklus ,  FALSE);
      }
      else
      {
        g_transform.schneiden_on = FALSE;
        XtSetSensitive(widget_transform.TB_def_koord, FALSE);
        XtSetSensitive(widget_transform.TB_def_nummer, FALSE);
	XtSetSensitive(widget_transform.TB_def_plane, FALSE);
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

        XtSetSensitive(widget_transform.TB_spiegeln, TRUE);
        XtSetSensitive(widget_transform.TB_zyklus ,  TRUE);
      }
      break;
    }

    case TRANSFORM_SCHNEIDEN_AUS:
    {
      if (XmToggleButtonGetState(widget_transform.TB_schneiden_aus))
      {
        XmToggleButtonSetState(widget_transform.TB_schneiden, FALSE, FALSE);
        g_transform.schneiden_on = FALSE;
      }
      else
       g_transform.schneiden_on = TRUE;

      break;
    }

   case TRANSFORM_DEF_PLANE:
    {
      if (XmToggleButtonGetState(widget_transform.TB_def_plane))
	{
	  cutting_on = TRUE;
	  XmToggleButtonSetState(widget_transform.TB_def_nummer, 
				 FALSE, FALSE);
	  XmToggleButtonSetState(widget_transform.TB_def_koord, 
				 FALSE, FALSE);
	}
      else
	{
	  cutting_on = FALSE;
	}
      break;
    }
    
    case TRANSFORM_DEF_KOORD:
    {
      if (XmToggleButtonGetState(widget_transform.TB_def_koord))
      {
        XtSetSensitive(widget_transform.T_punkt1x, TRUE);
        XtSetSensitive(widget_transform.T_punkt1y, TRUE);
        XtSetSensitive(widget_transform.T_punkt1z, TRUE);
        XtSetSensitive(widget_transform.T_punkt2x, TRUE);
        XtSetSensitive(widget_transform.T_punkt2y, TRUE);
        XtSetSensitive(widget_transform.T_punkt2z, TRUE);
        XtSetSensitive(widget_transform.T_punkt3x, TRUE);
        XtSetSensitive(widget_transform.T_punkt3y, TRUE);
        XtSetSensitive(widget_transform.T_punkt3z, TRUE);
        XtSetSensitive(widget_transform.T_punkt1nr, FALSE);
        XtSetSensitive(widget_transform.T_punkt2nr, FALSE);
        XtSetSensitive(widget_transform.T_punkt3nr, FALSE);

      XmToggleButtonSetState(widget_transform.TB_def_nummer, 
			     FALSE, FALSE);
      XmToggleButtonSetState(widget_transform.TB_def_plane, 
			     FALSE, FALSE);
      }
      break;
    }

    case TRANSFORM_PUNKT1_X:
    {
      punkt1[0] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT1_Y:
    {
      punkt1[1] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT1_Z:
    {
      punkt1[2] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT2_X:
    {
      punkt2[0] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT2_Y:
    {
      punkt2[1] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT2_Z:
    {
      punkt2[2] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT3_X:
    {
      punkt3[0] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT3_Y:
    {
      punkt3[1] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_PUNKT3_Z:
    {
      punkt3[2] = atof(XmTextFieldGetString(widget_transform.T_punkt1x));
      break;
    }

    case TRANSFORM_DEF_NUMMER:
    {
      if (XmToggleButtonGetState(widget_transform.TB_def_nummer))
      {
        XtSetSensitive(widget_transform.T_punkt1x, FALSE);
        XtSetSensitive(widget_transform.T_punkt1y, FALSE);
        XtSetSensitive(widget_transform.T_punkt1z, FALSE);
        XtSetSensitive(widget_transform.T_punkt2x, FALSE);
        XtSetSensitive(widget_transform.T_punkt2y, FALSE);
        XtSetSensitive(widget_transform.T_punkt2z, FALSE);
        XtSetSensitive(widget_transform.T_punkt3x, FALSE);
        XtSetSensitive(widget_transform.T_punkt3y, FALSE);
        XtSetSensitive(widget_transform.T_punkt3z, FALSE);
        XtSetSensitive(widget_transform.T_punkt1nr, TRUE);
        XtSetSensitive(widget_transform.T_punkt2nr, TRUE);
        XtSetSensitive(widget_transform.T_punkt3nr, TRUE);

      XmToggleButtonSetState(widget_transform.TB_def_koord, 
			     FALSE, FALSE);
      XmToggleButtonSetState(widget_transform.TB_def_plane, 
			     FALSE, FALSE);
      }
      break;
    }

    case TRANSFORM_PUNKTNR_1:
    {
      nummer = atoi(XmTextFieldGetString(widget_transform.T_punkt1nr));
      if ((nummer > 0) && (nummer <= g_geodat.npunkt))
      {
        punkt1[0] = g_knoten[nummer-1].koord[0];
        punkt1[1] = g_knoten[nummer-1].koord[1];
        punkt1[2] = g_knoten[nummer-1].koord[2];
      }
      break;
    }

    case TRANSFORM_PUNKTNR_2:
    {
      nummer = atoi(XmTextFieldGetString(widget_transform.T_punkt2nr));
      if ((nummer > 0) && (nummer <= g_geodat.npunkt))
      {
        punkt2[0] = g_knoten[nummer-1].koord[0];
        punkt2[1] = g_knoten[nummer-1].koord[1];
        punkt2[2] = g_knoten[nummer-1].koord[2];
      }
      break;
    }

    case TRANSFORM_PUNKTNR_3:
    {
      nummer = atoi(XmTextFieldGetString(widget_transform.T_punkt3nr));
      if ((nummer > 0) && (nummer <= g_geodat.npunkt))
      {
        punkt3[0] = g_knoten[nummer-1].koord[0];
        punkt3[1] = g_knoten[nummer-1].koord[1];
        punkt3[2] = g_knoten[nummer-1].koord[2];
      }
      break;
    }

    case TRANSFORM_DIS_CUTTING:
    {
      
      break;
    }
    case TRANSFORM_ZENTRIEREN:
    {
      zentrieren(); 
      break;
    }

    case TRANSFORM_ABBRECHEN:
    {
      break;
    }

    case TRANSFORM_OK:
    {
      if (XmToggleButtonGetState(widget_transform.TB_zyklus))
      {
        g_transform.zyklus_on = TRUE;
        anzahl = atoi(XmTextFieldGetString(widget_transform.T_anz_zyk));
        winkel = atof(XmTextFieldGetString(widget_transform.T_winkel));
        if (anzahl * winkel > 360) 
        {
          strcpy (hilfstr, "cycle is more than a complete circle");
          warning(hilfstr);
        }
        g_transform.zyklus_anz = anzahl;
        g_transform.zyklus_ang = winkel;
        if (XmToggleButtonGetState(widget_transform.TB_achsex))
          g_transform.zyklus_achse = 'x';
        else if (XmToggleButtonGetState(widget_transform.TB_achsey))
          g_transform.zyklus_achse = 'y';
        else if (XmToggleButtonGetState(widget_transform.TB_achsez))
          g_transform.zyklus_achse = 'z';
      }
      if (XmToggleButtonGetState(widget_transform.TB_spiegeln))
      {
        g_transform.spiegel_on = TRUE;
        if (XmToggleButtonGetState(widget_transform.TB_ebenexy))
          g_transform.mirrorx = TRUE;
        else if (XmToggleButtonGetState(widget_transform.TB_ebenexz))
          g_transform.mirrory = TRUE;
        else if (XmToggleButtonGetState(widget_transform.TB_ebeneyz))
          g_transform.mirrorz = TRUE;
      }
      if (XmToggleButtonGetState(widget_transform.TB_schneiden))
      {
        g_transform.schneiden_on = TRUE;
	if (XmToggleButtonGetState(widget_transform.TB_def_plane))
	  {
	    schneide();
	    C3D_postredisplay();
	    break;
	  }
	else 
	  {
	    get_schnitt_koord();
	    schneide();
	  }
      }
      if (XmToggleButtonGetState(widget_transform.TB_schneiden_aus))
      {
        g_transform.schneiden_on = FALSE;
      }
      C3D_postredisplay();
      break;
    }
  return;
  }
/*************************************************************************/
}

/*************************************************************************/
int init_transformieren(void)
/*************************************************************************/
{
  t_widget_transform *w_ptr = &widget_transform;
  Widget shell;
  static char hilfstr[256];
  Atom wm_delete_window;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_transformieren() ***\n");
#endif
  if (w_ptr->S_transform == NULL)
  {
    if (!(w_ptr->S_transform = Createtransform_interface("S_transform", toplevel, NULL, 0)))
    {
      fprintf(stderr, "Createtransform_interface(...) returns NULL\n");
      return(2);
    }
    shell = w_ptr->S_transform;
    if (!(w_ptr->TB_zyklus       = my_XtNameToWidget(shell, "*TB_zyklus", TRUE))) return(1);
    if (!(w_ptr->T_anz_zyk       = my_XtNameToWidget(shell, "*T_anz_zyk", TRUE))) return(1);
    if (!(w_ptr->AB_anzahl_plus  = my_XtNameToWidget(shell, "*AB_anzahl_plus", TRUE))) return(1);
    if (!(w_ptr->AB_anzahl_minus = my_XtNameToWidget(shell, "*AB_anzahl_minus", TRUE))) return(1);
    if (!(w_ptr->T_winkel        = my_XtNameToWidget(shell, "*T_winkel", TRUE))) return(1);
    if (!(w_ptr->TB_achsex       = my_XtNameToWidget(shell, "*TB_achsex", TRUE))) return(1);
    if (!(w_ptr->TB_achsey       = my_XtNameToWidget(shell, "*TB_achsey", TRUE))) return(1);
    if (!(w_ptr->TB_achsez       = my_XtNameToWidget(shell, "*TB_achsez", TRUE))) return(1);
    if (!(w_ptr->TB_spiegeln     = my_XtNameToWidget(shell, "*TB_spiegeln", TRUE))) return(1);
    if (!(w_ptr->TB_ebenexy      = my_XtNameToWidget(shell, "*TB_ebenexy", TRUE))) return(1);
    if (!(w_ptr->TB_ebenexz      = my_XtNameToWidget(shell, "*TB_ebenexz", TRUE))) return(1);
    if (!(w_ptr->TB_ebeneyz      = my_XtNameToWidget(shell, "*TB_ebeneyz", TRUE))) return(1);
/*
    if (!(w_ptr->TB_schneiden    = my_XtNameToWidget(shell, "*TB_schneiden", TRUE))) return(1);
    if (!(w_ptr->TB_schneiden_aus= my_XtNameToWidget(shell, "*TB_schneiden_aus", TRUE))) return(1);
    if (!(w_ptr->TB_def_koord    = my_XtNameToWidget(shell, "*TB_def_koord", TRUE))) return(1);
    if (!(w_ptr->TB_def_plane    = my_XtNameToWidget(shell, "*TB_def_plane", TRUE))) return(1);
    if (!(w_ptr->T_punkt1x       = my_XtNameToWidget(shell, "*T_punkt1x", TRUE))) return(1);
    if (!(w_ptr->T_punkt1y       = my_XtNameToWidget(shell, "*T_punkt1y", TRUE))) return(1);
    if (!(w_ptr->T_punkt1z       = my_XtNameToWidget(shell, "*T_punkt1z", TRUE))) return(1);
    if (!(w_ptr->T_punkt2x       = my_XtNameToWidget(shell, "*T_punkt2x", TRUE))) return(1);
    if (!(w_ptr->T_punkt2y       = my_XtNameToWidget(shell, "*T_punkt2y", TRUE))) return(1);
    if (!(w_ptr->T_punkt2z       = my_XtNameToWidget(shell, "*T_punkt2z", TRUE))) return(1);
    if (!(w_ptr->T_punkt3x       = my_XtNameToWidget(shell, "*T_punkt3x", TRUE))) return(1);
    if (!(w_ptr->T_punkt3y       = my_XtNameToWidget(shell, "*T_punkt3y", TRUE))) return(1);
    if (!(w_ptr->T_punkt3z       = my_XtNameToWidget(shell, "*T_punkt3z", TRUE))) return(1);
    if (!(w_ptr->TB_def_nummer   = my_XtNameToWidget(shell, "*TB_def_nummer", TRUE))) return(1);
    if (!(w_ptr->T_punkt1nr      = my_XtNameToWidget(shell, "*T_punkt1nr", TRUE))) return(1);
    if (!(w_ptr->T_punkt2nr      = my_XtNameToWidget(shell, "*T_punkt2nr", TRUE))) return(1);
    if (!(w_ptr->T_punkt3nr      = my_XtNameToWidget(shell, "*T_punkt3nr", TRUE))) return(1);
*/
    if (!(w_ptr->TB_zentrieren   = my_XtNameToWidget(shell, "*TB_zentrieren", TRUE))) return(1);
    if (!(w_ptr->PB_beenden      = my_XtNameToWidget(shell, "*PB_beenden", TRUE))) return(1);
    if (!(w_ptr->PB_ok           = my_XtNameToWidget(shell, "*PB_ok", TRUE))) return(1);
    if (!(w_ptr->PB_hilfe        = my_XtNameToWidget(shell, "*PB_hilfe", TRUE))) return(1);
  /*  if (!(w_ptr->LS_dis_cutting  = my_XtNameToWidget(shell, "*LS_dis_cutting", TRUE))) return(1); */
    

/* window-manager delete-button */
    XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
    wm_delete_window = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(shell, wm_delete_window,
	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_ABBRECHEN);

/* Callbacks registrieren */

/* Togglebutton- und TextField-Callbacks */
   XtAddCallback(
        w_ptr->TB_zyklus, XmNvalueChangedCallback,
    	(XtCallbackProc)transform_cb, 
        (XtPointer)TRANSFORM_ZYKLUS);
    XtAddCallback(
        w_ptr->T_anz_zyk, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_ANZAHL_ZYKLEN);
    XtAddCallback(
        w_ptr->AB_anzahl_plus, XmNactivateCallback,
    	(XtCallbackProc)transform_cb, 
        (XtPointer)TRANSFORM_ANZAHL_PLUS);
    XtAddCallback(
        w_ptr->AB_anzahl_minus, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_ANZAHL_MINUS);
    XtAddCallback(
        w_ptr->T_winkel, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_WINKEL);
    XtAddCallback(
        w_ptr->TB_achsex, XmNvalueChangedCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_X_ACHSE);
    XtAddCallback(
        w_ptr->TB_achsey, XmNvalueChangedCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_Y_ACHSE);
    XtAddCallback(
        w_ptr->TB_achsez, XmNvalueChangedCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_Z_ACHSE);
    XtAddCallback(
        w_ptr->TB_spiegeln, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_SPIEGELN);
    XtAddCallback(
        w_ptr->TB_ebenexy, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_XY_EBENE);
    XtAddCallback(
        w_ptr->TB_ebenexz, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_XZ_EBENE);
    XtAddCallback(
        w_ptr->TB_ebeneyz, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_YZ_EBENE);
/*
    XtAddCallback(
        w_ptr->TB_schneiden, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_SCHNEIDEN);
    XtAddCallback(
        w_ptr->TB_schneiden_aus, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_SCHNEIDEN_AUS);
    XtAddCallback(
        w_ptr->TB_def_plane, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_DEF_PLANE);
    XtAddCallback(
        w_ptr->TB_def_koord, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_DEF_KOORD);
    XtAddCallback(
        w_ptr->T_punkt1x, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT1_X);
    XtAddCallback(
        w_ptr->T_punkt1y, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT1_Y);
    XtAddCallback(
        w_ptr->T_punkt1z, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT1_Z);
    XtAddCallback(
        w_ptr->T_punkt2x, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT2_X);
    XtAddCallback(
        w_ptr->T_punkt2y, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT2_Y);
    XtAddCallback(
        w_ptr->T_punkt2z, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT2_Z);
    XtAddCallback(
        w_ptr->T_punkt3x, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT3_X);
    XtAddCallback(
        w_ptr->T_punkt3y, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT3_Y);
    XtAddCallback(
        w_ptr->T_punkt3z, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKT3_Z);
    XtAddCallback(
        w_ptr->TB_def_nummer, XmNvalueChangedCallback,
        (XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_DEF_NUMMER);
    XtAddCallback(
        w_ptr->T_punkt1nr, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKTNR_1);
    XtAddCallback(
        w_ptr->T_punkt2nr, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKTNR_2);
    XtAddCallback(
        w_ptr->T_punkt3nr, XmNactivateCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_PUNKTNR_3);

    XtAddCallback(
	w_ptr->LS_dis_cutting, XmNbrowseSelectionCallback,
	(XtCallbackProc)addToDisCuttingCB,
	(XtPointer)NULL);
*/
    XtAddCallback(
        w_ptr->TB_zentrieren, XmNvalueChangedCallback,
    	(XtCallbackProc)transform_cb,
        (XtPointer)TRANSFORM_ZENTRIEREN);

    XtAddCallback(
        w_ptr->PB_beenden, XmNactivateCallback,
    	(XtCallbackProc)transform_cb, 
        (XtPointer)TRANSFORM_ABBRECHEN);
    XtAddCallback(
        w_ptr->PB_ok, XmNactivateCallback,
    	(XtCallbackProc)transform_cb, 
        (XtPointer)TRANSFORM_OK);
  }

  XtPopup(w_ptr->S_transform, XtGrabNone);

  //init_transform_interface();

  return(0);
/*************************************************************************/
}
		 
void addToDisCuttingCB(Widget w, XtPointer clientData, XtPointer callData)
{
  int i;                        /* counter */
  char *mat;
  Boolean result;
  XmListCallbackStruct *cdList; /* pointer to the scrolled list */
  cdList = (XmListCallbackStruct *) callData;

  /* check if item is allready in the selected list */
  for (i = 0; i < g_geodat.nstoff; i++)
    {
      /* item selected is equal to some item in 
       * list
       */
      if ((result = XmStringGetLtoR(cdList->item,
				 XmFONTLIST_DEFAULT_TAG, &mat)) == TRUE)
	{
	  if ( atoi(mat) == i)
	    g_dis_cutting[i] = 1; 
	}
    }

  
  printf("Value of selected item %i\n", g_dis_cutting[i]);
}
/*************************************************************************/
get_schnitt_koord()
/*************************************************************************/
{
  t_koord p1, p2, p3, t;
  double  g1[3], g2[3], abs,
          norm[3], d, plane;
/*************************************************************************/
  if (XmToggleButtonGetState(widget_transform.TB_def_koord))
  {
    g_transform.schnittp1_koord[0] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt1x));
    g_transform.schnittp1_koord[1] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt1y));
    g_transform.schnittp1_koord[2] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt1z));
    g_transform.schnittp2_koord[0] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt2x));
    g_transform.schnittp2_koord[1] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt2y));
    g_transform.schnittp2_koord[2] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt2z));
    g_transform.schnittp3_koord[0] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt3x));
    g_transform.schnittp3_koord[1] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt3y));
    g_transform.schnittp3_koord[2] = 
             atof(XmTextFieldGetString(widget_transform.T_punkt3z));

  }
  else if (XmToggleButtonGetState(widget_transform.TB_def_nummer))
  {
/* Hole Punktnummern ------------------------------------------------ */
    g_transform.schnittp1_nr = 
             atoi(XmTextFieldGetString(widget_transform.T_punkt1nr));
    g_transform.schnittp2_nr = 
             atoi(XmTextFieldGetString(widget_transform.T_punkt2nr));
    g_transform.schnittp3_nr = 
             atoi(XmTextFieldGetString(widget_transform.T_punkt3nr));

/* Speichern der Koordinaten -------------------------------------- */
    g_transform.schnittp1_koord[0] =
                   g_knoten[g_transform.schnittp1_nr - 1].koord[0];
    g_transform.schnittp1_koord[1] =
                   g_knoten[g_transform.schnittp1_nr - 1].koord[1];
    g_transform.schnittp1_koord[2] =
                   g_knoten[g_transform.schnittp1_nr - 1].koord[2];
    g_transform.schnittp2_koord[0] =
                   g_knoten[g_transform.schnittp2_nr - 1].koord[0];
    g_transform.schnittp2_koord[1] =
                   g_knoten[g_transform.schnittp2_nr - 1].koord[1];
    g_transform.schnittp2_koord[2] =
                   g_knoten[g_transform.schnittp2_nr - 1].koord[2];
    g_transform.schnittp3_koord[0] =
                   g_knoten[g_transform.schnittp3_nr - 1].koord[0];
    g_transform.schnittp3_koord[1] =
                   g_knoten[g_transform.schnittp3_nr - 1].koord[1];
    g_transform.schnittp3_koord[2] =
                   g_knoten[g_transform.schnittp3_nr - 1].koord[2];
  }

/* berechne Norm der Schnittebene ----------------------------------- */
  p1[0] = g_transform.schnittp1_koord[0];
  p1[1] = g_transform.schnittp1_koord[1];
  p1[2] = g_transform.schnittp1_koord[2];
  p2[0] = g_transform.schnittp2_koord[0];
  p2[1] = g_transform.schnittp2_koord[1];
  p2[2] = g_transform.schnittp2_koord[2];
  p3[0] = g_transform.schnittp3_koord[0];
  p3[1] = g_transform.schnittp3_koord[1];
  p3[2] = g_transform.schnittp3_koord[2];

  g1[0] = (double) (p2[0] - p1[0]);     /* die 2 Vektoren der Ebene - */
  g1[1] = (double) (p2[1] - p1[1]);
  g1[2] = (double) (p2[2] - p1[2]);

  g2[0] = (double) (p3[0] - p1[0]);
  g2[1] = (double) (p3[1] - p1[1]);
  g2[2] = (double) (p3[2] - p1[2]);

/* bestimmen des Normalenvektors der eingeg. Ebene ------------------ */
  norm[0]  = g1[1]*g2[2] - g1[2]*g2[1];
  norm[1]  = g1[2]*g2[0] - g1[0]*g2[2];
  norm[2]  = g1[0]*g2[1] - g1[1]*g2[0];

  abs = (float) sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
  norm[0] = norm[0] / abs;
  norm[1] = norm[1] / abs;
  norm[2] = norm[2] / abs;
 
  /* looks if normal is good */
  t[0] = p1[0] + norm[0];
  t[1] = p1[1] + norm[1];
  t[2] = p1[2] + norm[2];
  /* function of plane > 0 */
  plane = norm[0]*(t[0] - p1[0])+ norm[1]*(t[2] - p1[1]) + 
    norm[2]*(t[2] - p1[2]);
  if(plane < 0.0)
    {
      /* if not change sign */
      norm[0] = -norm[0];
      norm[1] = -norm[1];
      norm[2] = -norm[2];
    }
/* bestimmen der Konstanten d --------------------------------------- */
  d = (norm[0]*p1[0] + norm[1]*p1[1] + norm[2]*p1[2]);

  g_transform.norm[0] = (float) norm[0];
  g_transform.norm[1] = (float) norm[1];
  g_transform.norm[2] = (float) norm[2];
  g_transform.d = (float) d;

  return(0);
/*************************************************************************/
}
