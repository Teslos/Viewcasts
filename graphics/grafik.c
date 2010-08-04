/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   grafik.c                                         */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/*  c3d              : Initialisierung und Hauptmenueaufruf      */
/*  update_screens   : Refresh der graphik-Fenster               */
/*  init_geo         : Initialisie Geometriedaten f. Darstellung */
/*  init_color3d     : Initialisierung der Grafikeinstellungen   */
/*  init_transform   : init. Transformationen                    */
/*  update_scene     : steuert Transformationen                  */
/*  grafik_translate : Verschiebung d. Geometrie per mouse       */
/*  grafik_rotate    : Drehung d. Geometrie per mouse            */
/*  grafik_zoom      : Zoomen d. Geometrie nach Funktionstasten  */
/*  reset            : zuruecksetzen aller Transformationen      */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
/*#include <X11/Xirisw/GlxMDraw.h>*/

#include "color3d.h"
#include "motif.h"
#include "grafik.h"
#include "pattern.h"
#include "grafik_func.h"
#include "rgbfarben.h"
#include "trackball.h"

#include <GL/gl.h>
#include <GL/glu.h>
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* ****************************************************************** */
/* c3d - Haupt- und Initialisierungsroutinen ------------------------ */ 
/* ****************************************************************** */
/* Programm benoetigt : Z-Buffer und Doublebuffer Modus ------------- */

/* ****************************************************************** */
int c3d(gl_width, gl_height)
/* ****************************************************************** */
{
  init_color3d(); 
  init_colors();
  /* initialisiere Farben fuer Isolinien ------------------------------ */
  IsoFarben[0] = 0x00FF00;
  IsoFarben[1] = 0x00FFFF;
  IsoFarben[2] = 0xFF00FF;
  IsoFarben[3] = 0xFFFFFF;
  IsoFarben[4] = 0x404040;

  if (g_geodat.npunkt != 0)
    init_geo(gl_width, gl_height);   /* init Geometriedarstellung */

  zeichne();                           /* Darstellung der Geometrie */

  return;
  /* ********************* ende c3d *********************************** */
}
 
/* ****************************************************************** */
int init_geo(int gl_width, int gl_height)
/* ****************************************************************** */
{
  float XYZ;
  /* ****************************************************************** */
  if (g_geodat.npunkt != 0)
    XYZ = ABS(get_maxkoord());
  else
    XYZ = 1;
    
  g_projektion.dist   =   2.0 * XYZ;
  g_projektion.near   =   2.0 * XYZ;
  g_projektion.far    =  40.0 * XYZ;
  g_projektion.top    =   2.0 * XYZ;
  g_projektion.bottom = - 2.0 * XYZ;
  g_projektion.right  =   2.0 * XYZ;
  g_projektion.left   = - 2.0 * XYZ;
  g_projektion.geo_factor = 5.0 *( XYZ / (MAX(C3D_win.gl_width, C3D_win.gl_height)));
  g_projektion.aspect = (GLfloat)C3D_win.gl_width / C3D_win.gl_height;
  /* adjust projection */
  g_projektion.top /= g_projektion.aspect;
  g_projektion.bottom /= g_projektion.aspect;

  g_transform.move[2]         = -5.0 * XYZ;
  g_schritt = XYZ / 100.0;
  return;
  /* ****************** ende init_geo ********************************* */
}

/* ****************************************************************** */
int init_color3d()
/* ****************************************************************** */
{
  int i;

  if (!b_first_color3d) return;
  b_first_color3d = FALSE;

  /* init Z-Buffer ------------------------------------------------ */
  glEnable(GL_DEPTH_TEST);
  glDepthMask((GLboolean) TRUE);

  /* belege Window-struktur und initialisiere Grafik -------------- */
    C3D_win.gl_width  = gl_width;
    C3D_win.gl_height = gl_height; 
    C3D_win.asp = (float) C3D_win.gl_width / (float) C3D_win.gl_height;
    g_projektion.aspect = C3D_win.asp;

  /* Defaultbelegung der Projektionsstruktur -------------------------- */
  g_projektion.perspektive = TRUE;
  g_projektion.azim   = 0;
  g_projektion.inc    = 900;
  g_projektion.twist  = 0;
  g_projektion.fovy   = 500.0;
  g_projektion.aspect = 1.0;
  g_projektion.near   =   2.0;    /* Clipping Planes -------------- */
  g_projektion.far    =  40.0;    /*  */
  g_projektion.top    =   2.0;    /*  */
  g_projektion.bottom =  -2.0;    /*  */
  g_projektion.right  =   2.0;    /*  */
  g_projektion.left   =  -2.0;    /*  */
  g_projektion.dist   =  15.0;    /* Sichtweite ------------------- */
  g_projektion.geo_factor = 0.01; /* Faktor f. Verschiebung ------- */

  init_transform();      /* init. Transformationen ---------------- */
  shadeinit();
  

  /* ****************************************************************** */
  return;
}

/* ****************************************************************** */
int init_transform()
/* ****************************************************************** */
{
  float XYZ;
  char wert[8];
  static int first_rot=1;
  /* ****************************************************************** */
  if (g_geodat.npunkt != 0)
    XYZ = ABS(get_maxkoord());
  else
    XYZ = 1.0;

  /* Defaultbelegung der Transformationsstruktur --------------------- */
  g_transform.move[0]         = 0.0;
  g_transform.move[1]         = 0.0;
  g_transform.move[2]         = -5.0 * XYZ;
  g_transform.pan[0]          = 0.0;
  g_transform.pan[1]          = 0.0;
  g_transform.pan[2]          = 0.0;
  g_transform.rotx            = -90.0;
  g_transform.roty            = 0.0;
  g_transform.rotz            = 0.0;
  if (first_rot)
    {
      trackball(curquat, 0.0, 0.0, 0.0, 0.0);
      first_rot = 0;
    }
  g_transform.scale           = 1.0;
  g_transform.mirrorx         = FALSE;
  g_transform.mirrory         = FALSE;
  g_transform.mirrorz         = FALSE;
  g_transform.spiegel_on      = FALSE;
  g_transform.zyklus_on       = FALSE;
  g_transform.kreuz_on        = TRUE;
  g_transform.rahmen_on       = FALSE;
  g_transform.geo_on          = TRUE;
  g_transform.rand_on         = FALSE;
  g_transform.absenken_on     = FALSE;
  g_transform.alle_stoffe_on  = TRUE;
  g_transform.stoffgruppen_on = FALSE;
  g_transform.scale_k         = 1.0;
  g_transform.zyklus_anz      = 2;
  g_transform.zyklus_ang      = 45.0;
  g_transform.zyklus_achse    = 'z';
  g_transform.norm[0]		= 0.0;
  g_transform.norm[1]		= 0.0;
  g_transform.norm[2]		= 0.0;
  g_transform.d 		= 0.0;

  /* Vorbelegung fuer Isolinien -------------------------------------- */
  g_isolinien.anzahl_isolinien = 0;
  g_isolinien.linien = NULL;

  /* initialize global matrices */
  M_identity(Rotation_matrix);
  M_identity(Translation_matrix);

  /* Defaultbelegung der Transformationsstruktur --------------------- */
  Redraw      = TRUE;          /* Geometrie neu zeichnen --------- */
    
  /* Gebe Rotationswerte im Hauptfenster aus ---------------------------- */
  sprintf(wert, "%i", (int) g_transform.rotx);
  XmTextFieldSetString(widgetMain.RX_rot_x, wert);
  sprintf(wert, "%i", (int) g_transform.roty);
  XmTextFieldSetString(widgetMain.RX_rot_y, wert);
  sprintf(wert, "%i", (int) g_transform.rotz);
  XmTextFieldSetString(widgetMain.RX_rot_z, wert);
}

/* ****************************************************************** */
void update_scene()
/* ****************************************************************** */
{
  switch (mode)
    {
    case TRANSLATE:
      {
	grafik_translate();
	break;
      }
    case ROTATE:
      {
	grafik_rotate();
	break;
      }
    case ZOOM:
      {
	grafik_zoom();
	break;
      }
    }
}

/* ****************************************************************** */
void grafik_translate()
/* ****************************************************************** */
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  //glTranslatef((GLfloat)((mx-omx)/25.0), 0.0, 0.0);
  //glTranslatef(0.0, (GLfloat)((omy-my)/25.0), 0.0);

  glMultMatrixf(Projektions_matrix);
  glGetFloatv(GL_PROJECTION_MATRIX, Projektions_matrix);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  
}

/* ****************************************************************** */
void grafik_rotate()
/* ****************************************************************** */
{
  glPushMatrix();
  glLoadIdentity();

  if (b_spin)
    {
      build_rotmatrix(m, curquat);
      glMultMatrixf(&m[0][0]);
    }
  else
    {
      glRotatef(mx-omx, 1, 0, 0);

      glMultMatrixf(Zeichen_matrix);
      glGetFloatv(GL_MODELVIEW_MATRIX, Zeichen_matrix);
    }

  glPopMatrix();
}

/* ****************************************************************** */
void grafik_zoom()
/* ****************************************************************** */
{
  glPushMatrix();
  glLoadIdentity();

  glScalef(g_transform.scale, g_transform.scale, g_transform.scale);
  glMultMatrixf(Zeichen_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, Zeichen_matrix);

  glPopMatrix();
}

/* ****************************************************************** */
void reset()
/* ****************************************************************** */
{
  init_color3d();
  init_transform();
  
  return;
}
