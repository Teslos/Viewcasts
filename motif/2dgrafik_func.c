/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   11.05.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname         | Funktion                               */
/* ------------------------------------------------------------- */
/* init_2dgrafik        | 	         			 */
/* fill_tt_picking      |  		         		 */
/* tt_2dgrafik_preedit  |	       		         	 */
/* tt_2dgrafik_postedit |                                        */
/* tt_2dgrafik_activate |	                	         */
/* tb_2dgrafik_activate |			                 */
/* add_pickpunkt        |			                 */
/* loesche_punkt        |			                 */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include "memory.h"

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
#define GRAFIK_ABKUEHLKURVEN  1
#define GRAFIK_AUSGABE        2
#define GRAFIK_PICK           3
#define GRAFIK_MANUELL        4
#define GRAFIK_ZEICHNE        5
#define GRAFIK_LOESCHE        6
#define GRAFIK_RESET          7
#define DARSTELLEN            8 
#define ORDINATE_MIN          9 
#define ORDINATE_MAX         10 
#define ABSZISSE_MIN         11 
#define ABSZISSE_MAX         12
#define SCHLIESSE_ABKFENSTER 13
#define GRAFIK_SCHLIESSEN   200
#define GRAFIK_ADD_POINT    202

enum{ PICK_COLORS = 13 };
XtPointer *PickColor[] = {  (XtPointer) "#FF0000", 
		       (XtPointer)"#00FF00", 
		       (XtPointer)"#0000FF",
		       (XtPointer)"#00FFFF", 
		       (XtPointer)"#FF00FF", 
		       (XtPointer)"#FFFFFF", 
		       (XtPointer)"#404040",
		       (XtPointer)"#C67171", 
		       (XtPointer)"#71C671", 
		       (XtPointer)"#8E8E38", 
		       (XtPointer)"#7171C6",
		       (XtPointer)"#8E388E", 
		       (XtPointer)"#388E8E"};

unsigned int rgbPickColor[] = {0x0000FF, 0x00FF00, 0xFF0000,
				 0xFFFF00, 0xFF00FF, 0xFFFFFF, 0x404040,
				 0x17176c, 0x176c17, 0x83e8e8, 0x6c1717,
				 0xe883e8, 0xe8e883};
/* Macro */
#define Length(v1) sqrt(dotproduct(v1,v1))

static Widget *widgetliste;
static int postedit = 0;
static void scaleModifyCB(Widget, XtPointer, XtPointer);
static void scaleSetValue(void);
void tt_2dgrafik_calculate(Widget widget, XtPointer client_data,
			   XtPointer cb_ptr);
static int cmp_int(const void *cmpdata, const void *data);
static void destroy(void *data);

struct Row
{
  t_koord koord_man;
  int currRow;
}t_row;

struct Row row;

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* *************************************************************** */

/* ****************************************************************** */
void tb_2dgrafik_activate(Widget widget, int action, XtPointer cb_ptr)
/* ****************************************************************** */
{
  int i;
  int j;
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
  static t_bool alle_2dgrafik = FALSE;
  char *Textfeld;
  char msegaufruf[200];
  char  casts_filename[50];
  char  casts_dir[100];
  int slash_pos;
  t_pickpunkt pickpunkt;
  
  /* ****************************************************************** */
#ifdef DEBUG  
  if (applRessourcen.debug)
       fprintf(stdout, "***tb_2dgrafik_activate(..., >%d<, ...)***\n",action);
#endif  
  /* Togglebuttonabhaengige Aktionen ---------------------------------- */
  switch (action)
    {
    case GRAFIK_PICK:
      {
#ifdef DEBUG
	if (applRessourcen.debug) fprintf(stdout, "grafik_picK\n");
#endif
	/* Koordinaten picken ----------------------------------------------- */

	/* Ausgabe und Abkuehlkurven sensitiv / insensitiv setzen ----------- */ 
	if (XmToggleButtonGetState(widget2DGrafik.TB_pick ))
	  {
	    g_picking.pick_on = TRUE;
	    XtSetSensitive(widget2DGrafik.PB_zeichne, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_loesche, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_calculate, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_addpoint, FALSE);
	    /* schalte um auf Picking-Modus ------------------------------------- */
	  }
	else
	  {
	    g_picking.pick_on = FALSE;
	    /* schalte Picking-Modus aus ---------------------------------------- */
	  }
	break;
      }
    case GRAFIK_MANUELL:
      {
	if(XmToggleButtonGetState(widget2DGrafik.TB_manuell))
	  {
	    /* set the flag for manual picking of node */
	    g_picking.pick_on = FALSE;
	    XtSetSensitive(widget2DGrafik.PB_addpoint, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_zeichne, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_loesche, TRUE);
	    XtSetSensitive(widget2DGrafik.PB_calculate, TRUE);
	  }
      }
    case GRAFIK_ABKUEHLKURVEN:
      {
	if (XmToggleButtonGetState(widget2DGrafik.TB_abkuehlkurve))
	  {
	    g_picking.kurve_on = TRUE;
	  }
	break;
      }

    case GRAFIK_AUSGABE:
      {
	if (XmToggleButtonGetState(widget2DGrafik.TB_ausgabe))
	  {
	    g_picking.kurve_on = FALSE;
        gnuplot();
        XmToggleButtonSetState(widget2DGrafik.TB_ausgabe, FALSE, FALSE);
	  }
	break;
      }

    case GRAFIK_ZEICHNE:
      {
	if (XmToggleButtonGetState(widget2DGrafik.TB_abkuehlkurve))
	  {
	    /* set min & max for abkuehlkurven */
	    scaleSetValue();
	    /* setze Flagge fuer Abkuehlkurve bei selektierten Punkten */
	    /* Falls Fenster nicht existiert, generiere Fenster ------------- */
	    if (!ABK_win.ex)
	      {
		if (make_fenster_abk(toplevel, &ABK_win) == 0)
		  {
		    ABK_win.ex = TRUE;
		    ABK_win.on = FALSE;
		  }
	      }

	    /* Falls Fenster nicht offen, oeffne Fenster ------------- */
	    if (!ABK_win.on)
	      {
		XtPopup(widgetMain.S_abkuehl,XtGrabNone);
		ABK_win.on = TRUE;
	      }
	    /* draw abkuehlkurven only if window is open and existing */
	    if(ABK_win.ex && ABK_win.on)
	      zeichne_abkuehlkurven();
	  }
	break;
      }

    case SCHLIESSE_ABKFENSTER:
      {
	if (ABK_win.on)
	  {
	    XtPopdown(widgetMain.S_abkuehl);
	    ABK_win.on = FALSE;
	  }
	break;
      }

    case GRAFIK_LOESCHE:
      {
	/* loesche markierten Punkt aus der Liste --------------------------- */
	/* hole Linien-Ptr. ------------------------------------------------- */
	XtVaGetValues(widget2DGrafik.TT_picking, BXmNnumRows, &nrows,
		      NULL);

	for (i = 1; i <= nrows; i++)
	  {
	    if (BXmTextTablePosIsSelected(widget2DGrafik.TT_picking, i))
	      {
		BXmTextTableGetField(widget2DGrafik.TT_picking, i, 1, FALSE,
				     &strptr);

		if (loesche_punkt(atoi(strptr)))
		  warning("No node to remove");
	      }
	  }
	fill_tt_picking();
	break;
      }
    case GRAFIK_ADD_POINT:
      {
     pickpunkt.punkt_nr = 0;
	 add_pickpunkt(pickpunkt);
	 fill_tt_picking();
	 break;
      }
    case DARSTELLEN:
      {
	break;
      }

    case ORDINATE_MIN:
      {
	g_picking.fscale = TRUE;
	Textfeld = XmTextFieldGetString(widget_loesung.T_min);
	g_picking.O_min = atof(Textfeld);
	XtFree(Textfeld);
	break;
      }

    case ORDINATE_MAX:
      {
	g_picking.fscale = TRUE;
	Textfeld = XmTextFieldGetString(widget_loesung.T_min);
	g_picking.O_max = atof(Textfeld);
	XtFree(Textfeld);
	break;
      }

    case ABSZISSE_MIN:
      {
	g_picking.fscale = TRUE;
	Textfeld = XmTextFieldGetString(widget_loesung.T_min);
	g_picking.A_min = atof(Textfeld);
	XtFree(Textfeld);
	break;
      }

    case ABSZISSE_MAX:
      {
	g_picking.fscale = TRUE;
	Textfeld = XmTextFieldGetString(widget_loesung.T_min);
	g_picking.A_max = atof(Textfeld);
	XtFree(Textfeld);
	break;
      }

    case GRAFIK_SCHLIESSEN:
      {
	C3D_postredisplay();
	break;
      }
    }
}


/**********************************************************************
 fill_tt_picking: routine get the selected points and populate
 BXwidget table with values of x,y,z coordinates
  
 Parameters:
 ============
            -none
 Returns:
 ========
            -nothing
**********************************************************************/
void fill_tt_picking()
{
  int i, j;
  int darstellung[3];
  char verschiebung[9];
  char Value[5][81];
  XtPointer *temp;
  t_pickpunkt *p_punkt;
  struct listelem *punkt;
  int anzahl = 0;
  XmString xmstr;
  char *pick_col[PICK_COLORS];
  

 
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** fill_tt_picking() ***\n");
#endif
    
  assert (XtIsWidget(widget2DGrafik.TT_picking));
  
  XtVaSetValues(widget2DGrafik.TT_picking,
                BXmNrecomputeLayout, FALSE, NULL);
  XtVaSetValues(widget2DGrafik.TT_picking, BXmNnumRows,
                g_picking.anzahl, NULL);
  BXmTextTableSetNumColumns(widget2DGrafik.TT_picking, 5);
  
  if (g_picking.punkte != NULL)  /* if we have initialized the list */
    {
      /* if we have points inside the list
       * set widgets for entering max and min values
       */
      punkt = g_picking.punkte->head;
      if (punkt != NULL)
	{
	  XtSetSensitive(widget2DGrafik.T_omin, TRUE);
	  XtSetSensitive(widget2DGrafik.T_omax, TRUE);
	  XtSetSensitive(widget2DGrafik.T_amin, TRUE);
	  XtSetSensitive(widget2DGrafik.T_amax, TRUE);
	}
      else
	{
	  XtSetSensitive(widget2DGrafik.T_omin, FALSE);
	  XtSetSensitive(widget2DGrafik.T_omax, FALSE);
	  XtSetSensitive(widget2DGrafik.T_amin, FALSE);
	  XtSetSensitive(widget2DGrafik.T_amax, FALSE);
	}

      for(i = 0; i < g_picking.anzahl; i++)
	{
	  p_punkt = (t_pickpunkt *) punkt->data;
	  sprintf(Value[0], "%i", p_punkt->punkt_nr);
	  if (p_punkt->punkt_nr == 0 /*&& !postedit*/)
	    {
	      strcpy(Value[1], " - ");
	      strcpy(Value[2], " - ");
	      strcpy(Value[3], " - ");
	      strcpy(Value[4], " - ");
	    }
	  else
	    {
	      sprintf(Value[1], "%f", p_punkt->koord[0]);
	      sprintf(Value[2], "%f", p_punkt->koord[1]);
	      sprintf(Value[3], "%f", p_punkt->koord[2]);
	      if ( g_loesung.gelesen )
	        {	
	          if (p_punkt->punkt_nr > g_geodat.npunkt)
		    sprintf(Value[4], "%f", p_punkt->werte1[g_animation.akt_index]);
	          else
		    sprintf(Value[4], "%f", p_punkt->werte [g_animation.akt_index]);
		}
	    }

           
	  temp = (XtPointer)pick_col[i];
	  BXmTextTableSetField(widget2DGrafik.TT_picking,i+1,1,
			       Value[0]);
	  BXmTextTableSetField(widget2DGrafik.TT_picking,i+1,2,
			       Value[1]);
	  BXmTextTableSetField(widget2DGrafik.TT_picking,i+1,3,
			       Value[2]);
	  BXmTextTableSetField(widget2DGrafik.TT_picking,i+1,4,
			       Value[3]);
	  BXmTextTableSetField(widget2DGrafik.TT_picking,i+1,5,
			       Value[4]);
    	BXmTextTableSetBackgroundColor(widget2DGrafik.TT_picking,i+1,5,
					     (String) p_punkt->color_string); 
	  punkt = punkt->next;
	}
    }
 
  XtVaSetValues(widget2DGrafik.TT_picking,
                BXmNrecomputeLayout, TRUE, NULL);

  BXmTextTableDeselectAll(widget2DGrafik.TT_picking);
}


/* ****************************************************************** */
/* TextTable-widget Stoffgruppen fuer Stoffgruppendefinition          */
/* ****************************************************************** */
void tt_2dgrafik_preedit(Widget widget, XtPointer client_data,
			 BXmTextTablePreEditCallbackStruct *cb_ptr)
/* ****************************************************************** */
{
  int i;
  char *strptr;
  char wert[81];
  int n_selected;
  int setting;
  static char hilfstr[4096];
  int result;
  int darstellung[3];
  char verschiebung[9];
  int aktuelle_kurve;
  Arg args[1];
  Widget wcolchooser;
  
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tt_2dgrafik_preedit() ***\n");
#endif
  /* Feld nicht editieren --------------------------------------------- */
  if(!g_picking.pick_on)
    cb_ptr->doit = TRUE;
  else
    cb_ptr->doit = FALSE;

  /* falls die aktuelle Zeile nicht selektiert ist, wird sie selektiert */

  /* hole Gruppen-Ptr. ------------------------------------------------ */
  BXmTextTableGetField(widget, cb_ptr->currentRow,1,FALSE,&strptr);
  aktuelle_kurve = atoi(strptr);

  /* falls die aktuelle Zeile nicht selektiert ist, wird sie selektiert */
  if (BXmTextTablePosIsSelected (widget, cb_ptr->currentRow))
    BXmTextTableDeselect(widget, cb_ptr->currentRow, FALSE);
  else
    BXmTextTableSelect(widget, cb_ptr->currentRow, FALSE);

  /* spaltenabhaengige Aktionen --------------------------------------- */
  switch (cb_ptr->currentColumn)
    {
    case 1:			/* Nummer --------------------------- */
      {
	/* read node number from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	
	break;
      }
    case 2:			/* X-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[0] = atof(strptr); 
	break;
      }
    case 3:			/* Y-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[1] = atof(strptr);
	break;
      }
    case 4:			/* Z-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[2] = atof(strptr); 
	break;
      }
    case 5:			/* Farbe ---------------------------- */
      { 
	int nnode = 0;
	BXmTextTableGetField(widget, cb_ptr->currentRow, 1,
			     FALSE, &strptr);
	nnode = atoi(strptr);
	args[0].value = nnode;
	args[0].name  = "number node";
#ifdef HAVE_SGI
	wcolchooser   = CreateColorChooser("S_cool_chooser", widget,
					   args, 1);
#endif
	break;
      }
    }

  /* disable apply button only if we are in manual mode */
	if(XmToggleButtonGetState(widget2DGrafik.TB_manuell))
  		XtSetSensitive(widget2DGrafik.PB_zeichne, FALSE);

  fill_tt_picking();
 }
/* ****************************************************************** */



/**********************************************************************
 Purpose:
 ====== 
        set the new manually picked "cooling" curves
 Parameters:
 ===========
         Standard callback parameters
 Returns:
 =======
         none
***********************************************************************/
void tt_2dgrafik_postedit(Widget widget, XtPointer client_data,
			 BXmTextTablePostEditCallbackStruct *cb_ptr)
{
  int aktuelle_kurve;       /* number of current curve (node number) */
  char *strptr;             /* temp variable  */
  t_pickpunkt *lpunkte;     /* list of points */
  struct listelem *punkte;   /* pointer to list of points */
  int  i;                   /* counter        */
 
  /* get the node number of current curve */
  BXmTextTableGetField(widget, cb_ptr->currentRow, 1, FALSE, &strptr);
  aktuelle_kurve = atoi(strptr);

  /* get number of the row */
  row.currRow = cb_ptr->currentRow;

  /* get the point in the table */
  punkte  = g_picking.punkte->head;
  for (i = 0; i < (cb_ptr->currentRow - 1); i++) 
    punkte = punkte->next;

  lpunkte = (t_pickpunkt *) punkte->data;
  switch (cb_ptr->currentColumn)
    {
    case 1:			/* Nummer --------------------------- */
      {
	/* check if node exists */
	if (aktuelle_kurve <= 0 || aktuelle_kurve > g_geodat.npunkt) return;
	
	/* get coordinates of node and 
	 * write coordinates to list of picked points 
	 */
	lpunkte->koord[0] = g_knoten[aktuelle_kurve - 1].koord[0];
	lpunkte->koord[1] = g_knoten[aktuelle_kurve - 1].koord[1];
	lpunkte->koord[2] = g_knoten[aktuelle_kurve - 1].koord[2];
	lpunkte->punkt_nr = aktuelle_kurve;
	belege_punktwerte(lpunkte);

	break;
      }
    case 2:			/* X-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[0] = atof(strptr);

	/* put new coordinate in list of picked points */
	lpunkte->koord[0] = row.koord_man[0];
	lpunkte->koord[1] = row.koord_man[1];
	lpunkte->koord[2] = row.koord_man[2];
	break;
      }
    case 3:			/* Y-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[1] = atof(strptr);
	lpunkte->koord[0] = row.koord_man[0];
	lpunkte->koord[1] = row.koord_man[1];
	lpunkte->koord[2] = row.koord_man[2];
	break;
      }
    case 4:			/* Z-Koordinate --------------------- */
      {
	/* read coordinate from table */
	BXmTextTableGetField(widget, cb_ptr->currentRow, BXmCURRENT_POSITION,
			     FALSE, &strptr);
	row.koord_man[2] = atof(strptr);
	lpunkte->koord[0] = row.koord_man[0];
	lpunkte->koord[1] = row.koord_man[1];
	lpunkte->koord[2] = row.koord_man[2];
	break;
      }
    case 5:			/* Farbe ---------------------------- */
      { 
	break;
      }
    }

  postedit = 1;
  /* enable apply button */
  XtSetSensitive(widget2DGrafik.PB_zeichne, TRUE);
  fill_tt_picking();
}

/***************************************************************************
 Purpose:
 ========
         -Calculate minimum distance from given point to
          all nodes of mesh.
 Parameters:
 ===========
         -Standard callback parameters
 Returns:
 =======
         -none
****************************************************************************/
void tt_2dgrafik_calculate(Widget widget, XtPointer client_data,
			   XtPointer call_data)
{
  int i;
  int min_node;
  float min_len;
  float len;
  t_koord dis;
  t_koord koord_node;
  t_pickpunkt *lpunkte;
  struct listelem *punkte;

  min_len = INFINITE;

  /* for every node calculate distance */
  for (i = 0; i < g_geodat.npunkt; i++)
    { 
      koord_node[0] = g_knoten[i].koord[0];
      koord_node[1] = g_knoten[i].koord[1];
      koord_node[2] = g_knoten[i].koord[2];
      
      Vecdis(dis, row.koord_man, koord_node);
     
      /* remeber minimum distance */
      len     = Length(dis);
      if (len < min_len)
	{
	  min_len  = len;
	  min_node = i+1;
	}
    }
  
  /* get the point which is minimum distance
   * from given point.
   */
  punkte = g_picking.punkte->head;
  for (i = 0; i < (row.currRow - 1); i++)
    {
      punkte = punkte->next;
    }
#ifdef DEBUG
  printf("Minimum node:%i\n", min_node);
#endif
  lpunkte = (t_pickpunkt*) punkte->data;
  /* update row                */
  lpunkte->koord[0] = g_knoten[min_node-1].koord[0];
  lpunkte->koord[1] = g_knoten[min_node-1].koord[1];
  lpunkte->koord[2] = g_knoten[min_node-1].koord[2];
  lpunkte->punkt_nr = min_node; 
  if ( g_loesung.gelesen )
      belege_punktwerte(lpunkte);
  fill_tt_picking();   
}

/* ****************************************************************** */
void tt_2dgrafik_activate(Widget widget, XtPointer client_data,
			  BXmTextTableActivateCallbackStruct *cb_ptr)
/* ****************************************************************** */
{
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** tt_2dgrafik_activate() ***\n");
#endif
  /* naechstes Feld nicht editieren ----------------------------------- */
  cb_ptr->editing = FALSE;
}

/**********************************************************************
 init_2dgrafik: This routine gets the pointer to toplevel widget and
 creates all necessery callbacks for diffrent buttons and widgets
 on 2dgrafik shell.

 Parameters:
 ==========
          -none
 Returns:
 ========
          0 - function sucessfull
	  1 - can't find all widgets
	  2 - no top level widget
**********************************************************************/
int init_2dgrafik(void)
{
  int result;                                  /* result of function  */
  t_widget_2dgrafik  *w_ptr;                   /* assign the top wid. */
  Widget shell;                                /* toplevel widget     */
  static char hilfstr[256];                    /* help string         */
  Atom wm_delete_window;            
  int i;

  w_ptr= &widget2DGrafik;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stdout, "*** void init_2dgrafik() ***\n");
#endif
  /* if toplevel doesn't exist
   * create one
   */
  if (w_ptr->S_2dgrafik == NULL)
    {
       w_ptr->S_2dgrafik= 
	 Create2dgrafik_interface("S_2dgrafik", toplevel, NULL, 0);
      if ( w_ptr->S_2dgrafik == NULL)
	{
	  fprintf(stderr, "Create2dgrafik(...) returns NULL\n");
	  return(2);
	}
    
      /* point to 2dgrafik widget */
      shell = w_ptr->S_2dgrafik;
      if (!(w_ptr->RC_eingabe = 
            my_XtNameToWidget(shell, "*RC_eingabe", TRUE))) return(1);
      if (!(w_ptr->TB_pick =
            my_XtNameToWidget(shell, "*TB_pick", TRUE))) return(1);
      if (!(w_ptr->TB_manuell =
            my_XtNameToWidget(shell, "*TB_manuell", TRUE))) return(1);
      if (!(w_ptr->RC_ausgabe = 
            my_XtNameToWidget(shell, "*RC_ausgabe", TRUE))) return(1);
      if (!(w_ptr->TB_abkuehlkurve =
            my_XtNameToWidget(shell, "*TB_abkuehlkurve",TRUE)))return(1);
      if (!(w_ptr->TB_ausgabe =
            my_XtNameToWidget(shell, "*TB_ausgabe", TRUE))) return(1);
      if (!(w_ptr->PB_zeichne =
            my_XtNameToWidget(shell, "*PB_zeichne", TRUE))) return(1);
      if (!(w_ptr->PB_loesche =
            my_XtNameToWidget(shell, "*PB_loesche", TRUE))) return(1);
      if (!(w_ptr->PB_calculate =
            my_XtNameToWidget(shell, "*PB_calculate", TRUE))) return(1);
      if (!(w_ptr->PB_addpoint =
            my_XtNameToWidget(shell, "*PB_addpoint", TRUE))) return(1);
      if (!(w_ptr->RC_achsen = 
            my_XtNameToWidget(shell, "*RC_achsen", TRUE))) return(1);
      if (!(w_ptr->TT_picking =
	    my_XtNameToWidget(shell, "*TT_picking", TRUE))) return(1);
      
      if (!(w_ptr->RC_zeichne = 
            my_XtNameToWidget(shell, "*RC_zeichne", TRUE))) return(1);

      if (!(w_ptr->T_omin = 
            my_XtNameToWidget(shell, "*T_omin", TRUE))) return(1);
      if (!(w_ptr->T_omax = 
            my_XtNameToWidget(shell, "*T_omax", TRUE))) return(1);
      if (!(w_ptr->T_amin = 
            my_XtNameToWidget(shell, "*T_amin", TRUE))) return(1);
      if (!(w_ptr->T_amax = 
            my_XtNameToWidget(shell, "*T_amax", TRUE))) return(1);

      /* window-manager delete-button */
      XtVaSetValues(shell, XmNdeleteResponse, XmDO_NOTHING, NULL, 0);
      wm_delete_window = XmInternAtom(XtDisplay(shell),"WM_DELETE_WINDOW",
				      False);
      XmAddWMProtocolCallback(shell, wm_delete_window,
			      (XtCallbackProc)tb_2dgrafik_activate,
			      (XtPointer)GRAFIK_SCHLIESSEN);

      /* Callbacks registrieren */
      /* Callbacks fuer Stofftabelle -------------------------------------- */
      
      XtAddCallback(
		    w_ptr->TT_picking, XmNpreEditCallback,
		    (XtCallbackProc)tt_2dgrafik_preedit,
		    (XtPointer)NULL);
      XtAddCallback(
		    w_ptr->TT_picking, XmNpostEditCallback,
		    (XtCallbackProc)tt_2dgrafik_postedit,
		    (XtPointer)NULL);
      XtAddCallback(
		    w_ptr->TT_picking, XmNactivateCallback,
		    (XtCallbackProc)tt_2dgrafik_activate,
		    (XtPointer)NULL);
      
     
      XtOverrideTranslations(
			     BXmTextTableGetChild(w_ptr->TT_picking, BXmCHILD_INPUT),
			     TF_in_TT_translations_table);
      
      /* Togglebutton- und Pushbutton-Callbacks --------------------------- */
      XtAddCallback(
		    w_ptr->TB_abkuehlkurve, XmNvalueChangedCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_ABKUEHLKURVEN);
      XtAddCallback(
		    w_ptr->TB_ausgabe, XmNvalueChangedCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_AUSGABE);
      XtAddCallback(
		    w_ptr->TB_pick, XmNvalueChangedCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_PICK);
      XtAddCallback(
		    w_ptr->TB_manuell, XmNvalueChangedCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_MANUELL);
      XtAddCallback(
		    w_ptr->PB_zeichne, XmNactivateCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_ZEICHNE);
      XtAddCallback(
		    w_ptr->PB_loesche, XmNactivateCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer)GRAFIK_LOESCHE);
      XtAddCallback(w_ptr->PB_calculate, XmNactivateCallback,
		    (XtCallbackProc)tt_2dgrafik_calculate,
		    (XtPointer) NULL);
      XtAddCallback(w_ptr->PB_addpoint, XmNactivateCallback,
		    (XtCallbackProc)tb_2dgrafik_activate,
		    (XtPointer) GRAFIK_ADD_POINT);

      /* add callbacks for scaling min and max values of
       * ordinate and abcisse.
       */
      XtAddCallback(w_ptr->T_omin, XmNmodifyVerifyCallback,
		    (XtCallbackProc)scaleModifyCB,
		    (XtPointer)ORDINATE_MIN);
      XtAddCallback(w_ptr->T_omax, XmNmodifyVerifyCallback,
		    (XtCallbackProc)scaleModifyCB,
		    (XtPointer)ORDINATE_MAX);
      XtAddCallback(w_ptr->T_amin, XmNmodifyVerifyCallback,
		    (XtCallbackProc)scaleModifyCB,
		    (XtPointer)ABSZISSE_MIN);
      XtAddCallback(w_ptr->T_amax, XmNmodifyVerifyCallback,
		    (XtCallbackProc)scaleModifyCB,
		    (XtPointer)ABSZISSE_MAX); 

      /*
	XtAddCallback(
        w_ptr->PB_beenden, XmNactivateCallback,
        (XtCallbackProc)tb_2dgrafik_activate,
        (XtPointer)GRAFIK_SCHLIESSEN);
      */
    }

  XtPopup(w_ptr->S_2dgrafik, XtGrabNone);

  
  /*
    XtSetSensitive(widget2DGrafik.PB_zeichne, FALSE);
  */
  init_2dgrafik_interface();
  fill_tt_picking();

  return(0);
}

/******************************************************************
  This routine changes the colour of "cooling" curve.
  
  Parameters:
  =======
          -int npunkt - for which point number we change color.
  Returns:
  =====
          -none 
*******************************************************************/
#ifdef HAVE_SGI
void change_cool_curve(int npunkt)
{
  int linnr, pointnr;
  t_pickpunkt *pickpunkt;
  struct listelem *punkt;

  /* This is necessary while color chooser changes 
   * current OpenGL drawing context. 
   */
  if (ABK_win.on && ABK_win.cx)
    glXMakeCurrent(ABK_win.dpy, XtWindow(ABK_win.wid), ABK_win.cx);
 
  if ((punkt = glookup(g_picking.punkte, (void*) &npunkt, cmp_int)) == NULL)
    return;
  
  /* change color */
  pickpunkt = (t_pickpunkt*) punkt->data;

  pickpunkt->rgba = PACK_COLOR(chcolor.color[0],chcolor.color[1],
				chcolor.color[2],0);

  /* Set the new color to text table */ 
  for(linnr = 0; linnr < g_picking.anzahl; linnr++)
    {
      String str;
      BXmTextTableGetField(widget2DGrafik.TT_picking,linnr+1, 1,
			   TRUE, &str);
#ifdef DEBUG
      fprintf(stderr, "Line number in cool curve: %s\n", str);
#endif
      pointnr = atoi(str);
      XtFree(str);
      if (npunkt == pointnr)
 	strcpy(pickpunkt->color_string, chcolor.color_string);
    }

  fill_tt_picking();
}
#endif

/******************************************************************
  This routine checks all characters inside this widget
  to see if their are valid numbers.
*******************************************************************/
static void scaleModifyCB(Widget w, XtPointer clientData, XtPointer callData)
{

  char *string;  /* value of scale */
  int i;

  XmTextVerifyCallbackStruct *cbs = 
    (XmTextVerifyCallbackStruct *) callData;

  /*
   * Test all characters of any new input to be sure they
   * are digits only. Set doit to FALSE if any character fails. 
   */
  
  if (cbs->text->ptr)
    {
      string = cbs->text->ptr;
      
      for (i = 0; i < cbs->text->length; i++) 
	{
	  if (!isdigit(string[i]))
	    cbs->doit = FALSE;
	}
    }

}

/**************************************************
 Set the value for min and max of the plot

 Parameters:
 ===========
           -none
 Returns:
 =======
           -sets min and max in global varibles,
           g_picking.O_min, g_picking.O_max,
           g_picking.A_min, g_picking.A_max.
          
***************************************************/
static void scaleSetValue()
{

  char *value; /* value of min or max */
  
  /* Get the value of min and max */
  value = XmTextFieldGetString(widget2DGrafik.T_omin);
  g_picking.fscale = TRUE;
  g_picking.O_min = atof(value);
  XtFree(value);
  value = XmTextFieldGetString(widget2DGrafik.T_omax);
  g_picking.O_max = atof(value);
  XtFree(value);
  value = XmTextFieldGetString(widget2DGrafik.T_amin);
  g_picking.A_min = atof(value);
  XtFree(value);
  value = XmTextFieldGetString(widget2DGrafik.T_amax);
  g_picking.A_max = atof(value);
  XtFree(value);

}

/**************************************************
 Add the picked point to table.

 Parameters:
 ===========
          -t_pickpunkt pickpunkt - structure of the 
	      picked point
 Returns:
 =======
         -none
          
***************************************************/
int add_pickpunkt(t_pickpunkt pickpunkt)
{
  int i,j;
  t_pickpunkt *npick = NULL;
  short int r,g,b,a;

  /* create and add new pick-point */
  NEW(npick);

  j = g_picking.anzahl;
  if (pickpunkt.punkt_nr != 0)
    {
      npick->art      = pickpunkt.art; /* default value */
      npick->punkt_nr = pickpunkt.punkt_nr;
      npick->koord[0] = pickpunkt.koord[0];
      npick->koord[1] = pickpunkt.koord[1];
      npick->koord[2] = pickpunkt.koord[2];
      npick->min      = pickpunkt.min;
      npick->max      = pickpunkt.max;
      npick->werte    = pickpunkt.werte;
      npick->werte2   = pickpunkt.werte2;
  	  npick->rgba     = rgbPickColor[j % PICK_COLORS];
      strcpy(npick->color_string, PickColor[j % PICK_COLORS]);
    }
	else
	{
		npick->art      = 0; /* default value */
		npick->punkt_nr = 1;
		npick->koord[0] = g_knoten[0].koord[0];
		npick->koord[1] = g_knoten[0].koord[1];
		npick->koord[2] = g_knoten[0].koord[2];
		npick->min      = 0;
		npick->max      = 0;
		npick->werte    = NULL;
		npick->werte2   = NULL;
		npick->rgba     = rgbPickColor[j % PICK_COLORS];
		strcpy(npick->color_string, PickColor[j % PICK_COLORS]);
	}

  if ( g_loesung.gelesen )
    belege_punktwerte(npick); 

  if (g_picking.punkte == NULL)
    {
      NEW(g_picking.punkte);
      glistinit(g_picking.punkte, destroy);
    }

  g_picking.punkte->head = gaddend(g_picking.punkte,
				   gnewitem((void*)npick));
				  
  (g_picking.anzahl)++;

  return 0;
}

/**************************************************
 Delete the picked point from table.

 Parameters:
 ===========
          -int nr - node number of the picked point
 Returns:
 =======
         -none
          
***************************************************/
int loesche_punkt(int nr) 
{
    t_pickpunkt *pickpunkt;
    struct listelem *punkt;

    punkt      = g_picking.punkte->head;
    pickpunkt  = (t_pickpunkt *)punkt->data;
#ifdef DEBUG
    fprintf(stderr, "Erasing point number: %i\n", nr);    
#endif

    /* find in the list this pick point and delete it */
    gdelitem(g_picking.punkte, (void *)&nr, cmp_int);
    
    --g_picking.anzahl;
    return 0;
}

static int cmp_int(const void *cmpdata, const void *data)
{
  int *nr, nr1;
  t_pickpunkt *pick;
  
  nr = (int *) cmpdata;
  pick = (t_pickpunkt *) data;
  nr1 = pick->punkt_nr;
  
  if (*nr > nr1)
    return 1;
  else if(*nr < nr1)
    return -1;
  else
    return 0;
}

static void destroy(void *data)
{
    t_pickpunkt *p;

    p = (t_pickpunkt *) data;
    free (p);
}








