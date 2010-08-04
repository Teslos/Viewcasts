/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Toni Ivas & Helmut Vor                           */
/* Datum    :   23.12.03                                         */
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


void cut_cb( Widget w, int   action, XtPointer callData )
{
  t_koord punkt1, punkt2, punkt3;
  int nummer;

  switch( action )
    {
    case TRANSFORM_SCHNEIDEN:
      {
	/* Flagge fuer Schnitt entsprechend des Togglebuttons setzen -------- */
	if (XmToggleButtonGetState(widget_transform.TB_schneiden))
	  {
	    g_transform.schneiden_on = TRUE;
	    XtSetSensitive(widget_transform.TB_def_koord, TRUE);
	    XtSetSensitive(widget_transform.TB_def_nummer, TRUE);
	    XtSetSensitive(widget_transform.TB_def_plane, TRUE);

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

	  }
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
    }
}

int init_cut(void)
{
  Widget shell;
  t_widget_transform  *w_ptr = &widget_transform;
  shell = cutShell;

  if (!(w_ptr->TB_schneiden    = my_XtNameToWidget(shell, "*toggleCut", TRUE))) return(1);
  if (!(w_ptr->TB_def_koord    = my_XtNameToWidget(shell, "*TB_def_coord", TRUE))) return(1);
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


  XtAddCallback(
		w_ptr->TB_schneiden, XmNvalueChangedCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_SCHNEIDEN);
  XtAddCallback(
		w_ptr->TB_def_plane, XmNvalueChangedCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_DEF_PLANE);
  XtAddCallback(
		w_ptr->TB_def_koord, XmNvalueChangedCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_DEF_KOORD);
  XtAddCallback(
		w_ptr->T_punkt1x, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT1_X);
  XtAddCallback(
		w_ptr->T_punkt1y, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT1_Y);
  XtAddCallback(
		w_ptr->T_punkt1z, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT1_Z);
  XtAddCallback(
		w_ptr->T_punkt2x, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT2_X);
  XtAddCallback(
		w_ptr->T_punkt2y, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT2_Y);
  XtAddCallback(
		w_ptr->T_punkt2z, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT2_Z);
  XtAddCallback(
		w_ptr->T_punkt3x, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT3_X);
  XtAddCallback(
		w_ptr->T_punkt3y, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT3_Y);
  XtAddCallback(
		w_ptr->T_punkt3z, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKT3_Z);
  XtAddCallback(
		w_ptr->TB_def_nummer, XmNvalueChangedCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_DEF_NUMMER);
  XtAddCallback(
		w_ptr->T_punkt1nr, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKTNR_1);
  XtAddCallback(
		w_ptr->T_punkt2nr, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKTNR_2);
  XtAddCallback(
		w_ptr->T_punkt3nr, XmNactivateCallback,
		(XtCallbackProc)cut_cb,
		(XtPointer)TRANSFORM_PUNKTNR_3);
}


