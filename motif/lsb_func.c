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
/*  selectCB         |  Selection Callback                       */
/*  loes_auswahl     |  Selection von Loesungszeitschritt        */
/*  initListe        |  Initialisiert Liste der Loesungsfelder   */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>

#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/SelectioB.h>

#include "motif.h"
#include "color3d.h"

/* globale Variable                                              */
/* ================                                              */
  static int answer;
static Widget s_loesel = NULL;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

void selectCB(Widget widget, XtPointer client_data,
                                XtPointer call_data)
{
  XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;

  client_data = (XtPointer) cbs->item_position;
  answer = cbs->item_position;
  return;
}

Widget initListe(Widget widget, XtPointer client_data, 
		 XtPointer call_data)
{
  XmString selection = (XmString)NULL;
  static Widget liste;
  Arg args[10];
  Cardinal anzargs;
  float *zeiten;
  char hilfstr[256];
  int anzahl_zeiten;
  int i;

  XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;

 
  anzahl_zeiten = g_animation.anzahl_felder;
  zeiten = g_animation.zeiten;
  
  s_loesel = XtVaCreatePopupShell("S_loesel",
				  overrideShellWidgetClass,
				  toplevel,
				  XmNallowShellResize, TRUE,
				  XmNdeleteResponse, XmUNMAP,
				  XmNx, cbs->event->xbutton.x_root - 10, 
				  XmNy, cbs->event->xbutton.y_root - 10,
				  0, 0);
  anzargs = 0;
  XtSetArg( args[anzargs], XmNvisibleItemCount, 10);
  anzargs++;                       /* 10 sichtbare Zeilen */
  assert( XtIsShell( s_loesel )); 
  liste = XmCreateScrolledList(s_loesel, "Liste", 
			       args, anzargs);
  
  XtAddCallback(liste, XmNbrowseSelectionCallback,
		(XtCallbackProc)selectCB, (XtPointer)&answer);
  
  for (i = 0; i < anzahl_zeiten; i++)
    {
      sprintf(hilfstr, " Time: %8.3f [s] ", zeiten[i]);
      selection = XmStringCreateLocalized(hilfstr);
      XmListAddItem(liste, selection, 0);
      XmStringFree(selection);
    }
  XtRealizeWidget(s_loesel);      /* instantiate it now */
  XtManageChild(liste);

  return s_loesel;
}

/* *************************************************************** */

/* *************************************************************** */
int loes_auswahl(Widget w, XtPointer client_data, XtPointer call_data)
/* *************************************************************** */
{
  extern Widget toplevel;
  extern XtAppContext kontext;
  
  Widget xmform;
  int antwort;
/* *************************************************************** */
  XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;

  /* erzeuge Widgets */
  if (s_loesel == NULL)
     s_loesel = initListe(w, client_data, call_data);
 
  /* set position of mouse */
  XtVaSetValues(s_loesel, XmNx, cbs->event->xbutton.x_root - 100,
		XmNy, cbs->event->xbutton.y_root - 10,
		NULL);
  XtPopup(s_loesel, XtGrabExclusive);
  while (answer == 0)
  XtAppProcessEvent(kontext, XtIMAll);

  XtPopdown(s_loesel);

  XSync(XtDisplay(s_loesel), 0);
  XmUpdateDisplay(toplevel);

  antwort = answer;
  answer  = 0;
  return ((int) antwort);
/* *************************************************************** */
}

int destroy_timelist()
{
  if ( s_loesel != NULL ) 
    {
      XtDestroyWidget(s_loesel);
      s_loesel = NULL;
    }
  return 0;
}
