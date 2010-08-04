/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   zeichnen.c                                       */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*      zeichne | Steuerroutine                                  */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>

#include "color3d.h"
#include "grafik_func.h"
#include "grafik.h"
#include "motif.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*****************************************************************/

/* ****************************************************************** */
void zeichne()
/* ****************************************************************** */
{
	int   stoffnr;
	float maxkoord;
	int oldMask[g_geodat.nstoff]; /*number of materials*/

	/* ****************************************************************** */
	/* Background-Farbe setzen: */ 
	if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
	{
		glClearIndex(FARBE_SCHWARZ);
	}
	else
	{
		glClearIndex(FARBE_SCHWARZ_ACHT);
	}

	/* OpenGl-init ------------------------------------------------------ */
	glViewport(0, 0, C3D_win.gl_width, C3D_win.gl_height);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	C3D_clearcolor();
	/*if (g_transform.kreuz_on)  zeichne_kreuz();*/

	/* draw cutted model */
	if (cutting_on)
	{
		drawcutted();
		return;
	}

	/* bilde Transformationsmatrizen ------------------------------------ */
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	track_project_matrix();

	glMatrixMode (GL_MODELVIEW);
	C3D_track_model_matrix();



	/* Ruecksprung wenn keine Geometrie gelesen ------------------------- */
	if (b_geo_gelesen && b_zeichnen)
	{
		/* bestimme maximale Koordinate der zu zeichnenden Stoffe ----------- */
		maxkoord = get_maxkoord();
		if (g_transform.rahmen_on) 
		{
			for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
			{
				zeichne_rahmen(stoffnr);
			}
		}
		else if (g_transform.gitter_on)
		{
			for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
			{
				oldMask[stoffnr] = g_Maske[stoffnr];
				g_Maske[stoffnr] = ZEICHNE_GITTER;
			}
			zeichne_geometrie();
			/* restore old */
			for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
				g_Maske[stoffnr] = oldMask[stoffnr];
		}
		else if (cutting_on)
		{
			zeichne_geometrie();
		} 
		else
		{
			zeichne_geometrie();
		}
	}

	if (g_sgiobj.gelesen)
	{ 
		shadeon();
		drawsgiobj(g_sgiobj.object, g_sgiobj.how);
		shadeoff();
	}

	if (g_transform.kreuz_on)  
	{
		zeichne_kreuz();
		draw_origin();
	}
	/* XXX test drawing for zeichne tabelle */
	zeichne_tabelle();

	/* swap und clear double-Buffers ------------------------------------- */
	if(C3D_win.doublebuffer)
		glXSwapBuffers(XtDisplay(gl_widget), XtWindow(gl_widget));
	else
		glFlush();

	C3D_finish_track_model_matrix();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	return;
	/* ********************* ende zeichne ******************************* */
}

