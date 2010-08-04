/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Ivas Toni & Helmut Vor                           */
/* Datum    :   09.10.02                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/* setmode         | set the atributes for drawing the line    */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */
#include <GL/gl.h>
#include <GL/glu.h>
#include "grafik.h"

#define INTENSITY_SCALE 0.1f

enum {
    AA_BEGIN = 1,
    AA_BLEND_CONST,
    AA_BLEND_ARB,
    AA_OFF,
    AA_END,

    INTENSITY_FULL,
    INTENSITY_INCREASE,
    INTENSITY_DECREASE,

    Z_ON,
    Z_OFF,

    BLEND_ON,
    BLEND_OFF
};
  
static float intensity = 1.0;

void setmode(int mode)
{
  switch (mode)
    {

      /* setting constant blending of the line */
    case AA_BLEND_CONST:
      glEnable(GL_LINE_SMOOTH);
      setmode(BLEND_ON);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      setmode(Z_ON);
      break;

    case AA_BLEND_ARB:
      glEnable(GL_LINE_SMOOTH);
      setmode(BLEND_ON);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
      
    case AA_OFF:
      glDisable(GL_LINE_SMOOTH);
      setmode(BLEND_OFF);
      setmode(Z_ON);
      break;

    case INTENSITY_FULL:
      intensity = 1.0f;
      g_tabelle.intensity = intensity;
      break;
      
    case INTENSITY_INCREASE:
      intensity *= 1.0f + INTENSITY_SCALE;
      if (intensity > 1.0f)
	intensity = 1.0f;
      g_tabelle.intensity = intensity; 
      break;

    case INTENSITY_DECREASE:
      intensity *= 1.0f - INTENSITY_SCALE;
      g_tabelle.intensity = intensity;
      break;

      /* Intesity mode */
    case Z_ON:
      glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
      glDepthFunc(GL_LEQUAL);
      break;

    case Z_OFF:
      glDisable(GL_DEPTH_TEST);
      glDepthMask(GL_FALSE);
      break;
      
      /* Blending mode */
    case BLEND_ON:
      glEnable(GL_BLEND);
      break;

    case BLEND_OFF:
      glDisable(GL_BLEND);
      break;
    
    default:
      fprintf(stderr, "Color3d: Error unknown mode for drawing lines\n");
    }
}
