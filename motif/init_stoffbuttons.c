/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.03.97                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname        | Funktion                                */
/* ------------------------------------------------------------- */
/* init_stoffbuttons   | initialisiere Button fuer jeden Stoff   */
/* destroy_stoffbuttons| loesche stoffbutton                     */
/* stoffbuttons_cb     | Callback fuer stoffbuttons              */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>

#include <Xm/ToggleB.h>

#include "color3d.h"
#include "motif.h"
#include "grafik.h"

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* ********************************************************************* */
static void stoffbuttons_cb(
/* ********************************************************************* */
#if NeedFunctionPrototypes
  Widget widget, XtPointer client_data, XtPointer call_data)
#else
  widget, client_data, call_data)
  Widget widget;
  XtPointer client_data;
  XtPointer call_data;
#endif
{
  if (g_stoffgebiete[(int)client_data].zeichne_on)
    g_stoffgebiete[(int)client_data].zeichne_on = FALSE;
  else
    g_stoffgebiete[(int)client_data].zeichne_on = TRUE;
  C3D_postredisplay();
  return;
}

/* ********************************************************************* */
int init_stoffbuttons(void)
/* ********************************************************************* */
/* erzeuge und initialisiere einen Aus/Ein-Toggelbutton fuer jeden Stoff */
/* im Hauptfenster */
{
  int        i;
  char       hilfstr[256];
  char       c_num[2];
  XmString   xm_string;
  XtPointer *StoffFar[25];
/* ********************************************************************* */

/* Belege Defaultfarben fuer Isolinien ------------------------------ */
  StoffFar[0]  = (XtPointer) "#FF0000";         /* rot -------------- */
  StoffFar[1]  = (XtPointer) "#00FF00";         /* gruen------------- */
  StoffFar[2]  = (XtPointer) "#0000FF";         /* blau  ------------ */
  StoffFar[3]  = (XtPointer) "#FFFF00";         /* gelb ------------- */
  StoffFar[4]  = (XtPointer) "#FF00FF";         /* pink ------------- */
  StoffFar[5]  = (XtPointer) "#00FFFF";         /* cyan ------------- */
  StoffFar[6]  = (XtPointer) "#F0C3F0";         /* rosa ------------- */
  StoffFar[7]  = (XtPointer) "#64BE96";         /* hellblau --------- */
  StoffFar[8]  = (XtPointer) "#64AFFF";         /* hellgruen -------- */
  StoffFar[9]  = (XtPointer) "#967C00";         /* ------------- */
  StoffFar[10] = (XtPointer) "#C8DE00";         /* ------------- */
  StoffFar[11] = (XtPointer) "#009632";         /* ------------- */
  StoffFar[12] = (XtPointer) "#C8C8C8";         /* ------------- */
  StoffFar[13] = (XtPointer) "#FF0096";         /* ------------- */
  StoffFar[14] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[15] = (XtPointer) "#C8C800";         /* bis hier */
  StoffFar[16] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[17] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[18] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[19] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[20] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[21] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[22] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[23] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[24] = (XtPointer) "#6464FF";         /* ------------- */
  StoffFar[25] = (XtPointer) "#6464FF";         /* ------------- */

  if ((stoffbuttons=(Widget*)malloc(g_geodat.nstoff*sizeof(Widget)))==NULL)
  {
    warning("Insufficient memory");
    return -1;
  }
  for (i = 0; i < g_geodat.nstoff; i++)
  {
    sprintf(c_num, "%d", i+1);
    xm_string = XmStringCreateLocalized(c_num);
   
    assert(XmIsRowColumn(widgetMain.RC_stoffe));

    stoffbuttons[i] = XtVaCreateManagedWidget(
                            c_num, xmToggleButtonWidgetClass,
                            widgetMain.RC_stoffe,
                            XmNalignment, XmALIGNMENT_BEGINNING,
                            XmNfillOnArm, TRUE,
                            XmNarmColor, StoffFar[i],
                            XmNbottomShadowColor, StoffFar[i],
                            XmNlabelType, XmSTRING,
                            XmNlabelString, xm_string,
                            0, 0);
    XtAddCallback(stoffbuttons[i], XmNvalueChangedCallback,
                  (XtCallbackProc)stoffbuttons_cb, (XtPointer)i);
    
    XmToggleButtonSetState(stoffbuttons[i], TRUE, FALSE);
    XmStringFree(xm_string);
  }
  return 1;
}

/* ********************************************************************* */
void destroy_stoffbuttons(void)
/* ********************************************************************* */
{
  int i;
  
  for (i = 0; i < g_geodat.nstoff; i++)
  {
    XtDestroyWidget(stoffbuttons[i]);
  }
  if (stoffbuttons != NULL) free (stoffbuttons);
  stoffbuttons = NULL;
  return;
}

