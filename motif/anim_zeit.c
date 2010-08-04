/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   13.01.97                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname             | Funktion                           */
/* ------------------------------------------------------------- */
/* animation_start          |                                    */
/* animation_stop           |                                    */
/* search_next_zeitschritt  |                                    */
/* get_next_zeitschritt     |                                    */
/* get_previous_zeitschritt |                                    */
/* setze_sensitiv_alle      |                                    */
/* lese_next_zeit           |                                    */
/* next_zeit_cb             |                                    */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include "color3d.h"
#include "grafik.h"
#include "motif.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
XtWorkProcId animateID;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* ************************************************************* */
static void animate(XtPointer closure, XtIntervalId *id )
{
  XtInputMask maske;
  if ((maske = XtAppPending(kontext)))
    {
      XtAppProcessEvent(kontext, maske);
    } 
  else
    {
      C3D_postredisplay();
      animateID = XtAppAddTimeOut(kontext, 1, animate, 0);
    }
}

int animation_start()
/* ************************************************************* */
{
  char c_zeit[10];
  static XtIntervalId timeid;
 XtInputMask maske;


/* setze alle Widgets insensitiv ausser Animation stop */
  setze_sensitiv_alle(FALSE);
/* setze Flagge fuer Animation auf TRUE und zeichne naechsten */
/* Zeitschritt bis Animation abgebrochen wird --------------- */
  g_animation.anim_on = TRUE;

  while (g_animation.anim_on)
  {

	 
	  if ((maske = XtAppPending(kontext)))
	  {
		  XtAppProcessEvent(kontext, maske);
	  } 
	  else
	  {
	    next_zeit_cb();
	    animateID = XtAppAddTimeOut(kontext,1, animation_start, 0);
	    //C3D_postredisplay();
	  }
	  
  }
  return (1);
}
/* ************************************************************* */

/* ************************************************************* */
int animation_stop()
/* ************************************************************* */
{
  setze_sensitiv_alle(TRUE);
  XtRemoveTimeOut( animateID );
  g_animation.anim_on = FALSE;
  animateID = 0;
  return (1);
}
/* ************************************************************* */

/* ************************************************************* */
int search_next_zeitschritt(float *zeit)
/* ************************************************************* */
{
	int i;

	i = 0;
	while ((g_animation.zeiten[i]<*zeit)&&(i<g_animation.anzahl_felder)) i++;
	*zeit = g_animation.zeiten[i];
	return (i);
}
/* ************************************************************* */

/* ************************************************************* */
int get_next_zeitschritt()
/* ************************************************************* */
{
	if (g_animation.anzahl_felder > (g_animation.akt_index + 1))
	{
		if (g_loesung.gescannt && g_animation.temp_on)
		{

			if (g_loesung.mmap)
			{
				if (mmlese_feld(++g_animation.akt_index))
					warning("Insufficient memory");
			}
			else 
			{
				if (lese_feld(++g_animation.akt_index))
					warning("Insufficient memory ");

			}
		}

		if (g_stroemung.vektor_gescannt && g_animation.stroem_on)
		{
			if ((lese_vektor(++g_animation.akt_index, 3)))
			{
				warning("Insufficient memory ");
			}
		}
		if (g_spannung.vektor_gescannt && g_animation.spann_on)
		{
			if ((lese_vektor(++g_animation.akt_index, 2)))
			{
				warning("Insufficient memory ");
			}
		}
	}
	else return (-1);
	/* update */
	C3D_updateref();
	return(0);
}
/* ************************************************************* */

/* ************************************************************* */
int get_previous_zeitschritt()
/* ************************************************************* */
{
	if (g_animation.akt_index > 0) 
	{
		if (g_loesung.gescannt && g_animation.temp_on)
		{
			if (g_loesung.mmap)
			{
				if (mmlese_feld(--g_animation.akt_index))
					warning("Insufficient memory ");
			}
			else
			{
				if (lese_feld(--g_animation.akt_index))
					warning("Insufficient memory ");
			}
		}

		if ( g_stroemung.vektor_gescannt && g_animation.stroem_on)
		{
			if ((lese_vektor(--g_animation.akt_index,3)))
			{
				warning("Insufficient memory ");
			}
		}
		if (g_spannung.vektor_gescannt && g_animation.spann_on)
		{
			if ((lese_vektor(--g_animation.akt_index,2)))
			{
				warning("Insufficient memory ");
			}
		}
	}
	else return (-1);
	/* update */
	C3D_updateref();
	return(0);
}
/* ************************************************************* */

/* ************************************************************* */
static void lese_next_zeit()
/* ************************************************************* */
{
	if (g_animation.akt_index < g_animation.anim_anfang)
		g_animation.akt_index = g_animation.anim_anfang;

	if (g_animation.akt_index == g_animation.anim_ende)
		g_animation.akt_index = g_animation.anim_anfang;
	else
		g_animation.akt_index++;

	if (g_loesung.gescannt && g_animation.temp_on)
	{
		if (g_loesung.mmap)
			mmlese_feld(g_animation.akt_index);
		else
			lese_feld(g_animation.akt_index);
	}

	if (g_spannung.on)
	{
		if (g_spannung.akt_index < g_spannung.anim_anfang)
			g_spannung.akt_index = g_spannung.anim_anfang;

		if (g_spannung.akt_index == g_spannung.anim_ende)
			g_spannung.akt_index = g_spannung.anim_anfang;
		else
			g_spannung.akt_index++;
		g_spannung.vektor_gelesen = FALSE;

		if (g_spannung.vektor_gescannt && g_animation.spann_on)
			lese_vektor(g_animation.akt_index, 2);
	}
	if (g_stroemung.on)
	{
		if (g_stroemung.akt_index < g_stroemung.anim_anfang)
			g_stroemung.akt_index = g_stroemung.anim_anfang;

		if (g_stroemung.akt_index == g_stroemung.anim_ende)
			g_stroemung.akt_index = g_stroemung.anim_anfang;
		else
			g_stroemung.akt_index++;
		g_stroemung.vektor_gelesen = FALSE;

		if (g_stroemung.vektor_gescannt && g_animation.stroem_on)
			lese_vektor(g_animation.akt_index, 3);
	}

	return;
	/* ***************** ende lese_next_zeit *********************** */
}

/* ************************************************************* */
int next_zeit_cb()
/* ************************************************************* */
{
	char c_zeit[10];
	int eq = 0;
	g_animation.anim_frame = (g_animation.anim_frame) ? g_animation.anim_frame : 1;
	if (( eq = g_animation.akt_index % g_animation.anim_frame ) == 0)
	{
		lese_next_zeit();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		C3D_updateref();
		zeichne();
		sprintf(c_zeit, "%8.3f", g_animation.zeiten[g_animation.akt_index]);
		XmTextFieldSetString(widgetMain.T_zeitschritt, c_zeit);
		return 1;
	}
	else
	{
		/* increase actual index frame */
		if(g_animation.akt_index >= g_animation.anim_ende)
			g_animation.akt_index = g_animation.anim_anfang;
		else
			g_animation.akt_index++;
		return 0;
	}

/* ************************************************************* */
}

void C3D_updateref()
{
	refimport_t ri;
	re = GetRefIsoAPI( ri );
	if ( re.C3D_updateiso )
		re.C3D_updateiso( g_isosurfaces.isosurfaces, TRUE, TRUE );
	C3D_postredisplay();
}
