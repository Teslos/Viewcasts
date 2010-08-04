/* ********************************************************************* */
/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   rgbfarben.c                                      */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*  init_colors | setze Colormap                                 */ 
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <GL/gl.h>
#include <GL/glu.h>
#include "grafik.h"
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/*****************************************************************/
/* RGB-Definitionen fuer C 3 D ---------------- */
/* ********************************************************************* */

/* Farben Hintergrund, Netz und Stoffe -------------------------- */
  GLubyte RGB_Schwarz[3] = {  0,   0,   0};
  GLubyte RGB_Weiss[3]   = {255, 255, 255};
  GLubyte RGB_Rot[3]     = {255,   0,   0};
  GLubyte RGB_Gruen[3]   = {  0, 255,   0};
  GLubyte RGB_Blau[3]    = {  0,   0, 255};
  GLubyte RGB_Gelb[3]    = {255, 255,   0};
  GLubyte RGB_Cyan[3]    = {  0, 255, 255};
  GLubyte RGB_Magenta[3] = {255,   0, 255};
  GLubyte RGB_Grau[3]    = {128, 128, 128};
  GLubyte RGB_Netz[3]    = {  0,   0,   0};

  GLubyte RGB_Color[2000][3];

  GLubyte RGB_Stoffe[100][3] =
  {
    255,   0,   0,   0, 255,   0,   0,   0, 255, 255, 255,   0,
    255,   0, 255,   0, 255, 255, 255, 200, 255, 100, 100, 150,
    100, 175, 255, 150, 125,   0, 200, 255,   0,   0, 150,  50,
    200, 200, 200, 255,   0, 100, 100, 100, 255, 200, 200,   0,
      0, 100,   0,   0,   0, 100, 100,   0,   0,   0, 200,   0,
      0,   0, 200, 200,   0,   0, 200, 200, 200, 100, 100, 100,
      0, 200, 200, 255,   0, 200, 200,   0, 200, 150,   0, 200,
    100,   0, 200,  50,   0, 200,   0,   0, 200,  50,  50, 100,
     50, 100, 100,  50, 150, 100,  50, 200, 100,  50, 255, 100,
    200, 100, 255, 200, 100, 200, 200, 100, 150, 200, 100, 100,
    200, 100,  50, 200, 100,   0, 100, 100, 100,  50, 100, 100,
      0, 100, 100,   0,   0,   0,  17,  17,  17,  34,  34,  34,
     51,  51,  51,  68,  68,  68,  85,  85,  85, 102, 102, 102,
    119, 119, 119, 136, 136, 136, 153, 153, 153, 170, 170, 170,
    187, 187, 187, 204, 204, 204, 221, 221, 221, 238, 238, 238,
    255, 255, 255, 255,   0, 128, 128, 128, 255, 128, 128, 255,
    128,   0, 128, 255,   0, 128, 255, 128, 128,   0,   0,   0,
    255, 128,   0, 128, 128,   0, 128, 255,   0, 255, 255, 153,
    153, 153, 170, 170, 170, 187, 187, 187, 204, 204, 204, 221,
    221, 221, 238, 238, 238, 255, 255, 255, 255,   0, 128, 128,
    128, 255, 128, 128, 255, 128,   0, 128, 255,   0, 128, 255,
    128, 128,   0,   0,   0, 255, 128,   0, 128, 128,   0, 128,
    255,   0, 255, 255, 128, 255, 128, 128,   0,   0,   0, 255,
    128,   0, 128, 128,   0, 128, 255,   0, 255, 255, 128, 255,
    128, 128,   0,   0,   0, 255, 128,   0, 128, 128,   0,   0
  };


/* ********************************************************************* */
int init_colors()     /* setze RGB_Colors ----------------------------*/
/* ********************************************************************* */
{
  int      i, istart;
  GLubyte  r, g, b;
/* ********************************************************************* */
/* Belegung der Colormap fuer Temperaturdarstellung - */
  istart = 0;
  r = g = 0;
  b = 255;
  
  for (i = 1; i < 225; i++) 
  {
    RGB_Color[istart][0]   = r++;
    RGB_Color[istart][1]   = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0]   = r;
    RGB_Color[istart][1]   = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0]   = r;
    RGB_Color[istart][1]   = g;
    RGB_Color[istart++][2] = b--;

    RGB_Color[istart][0]   = r;
    RGB_Color[istart][1]   = g;
    RGB_Color[istart++][2] = b;
  }

  for (i = 1; i < 32; i++)
  {
    RGB_Color[istart][0]   = r;
    RGB_Color[istart][1]   = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0]   = r;
    RGB_Color[istart][1]   = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r++;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b--;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;
    }

  for (i = 0; i < 100; i++) 
  {
    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g++;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;

    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b;
  }

  for (i = 0; i < 100; i++) 
  {
    b++;
    RGB_Color[istart][0] = r;
    RGB_Color[istart][1] = g;
    RGB_Color[istart++][2] = b++;
  }
  
}

/* ********************************************************************* */

#if FALSE
/* ********************************************************************* */
graumap()                      /* Colormap in Grauabstufungen ------- */
/* ********************************************************************* */
{
  Colorindex   i, j, istart;
  short        r, g, b;
/* ********************************************************************* */
  colmap = 2;                  /* Kennung fuer Graubelegung --------- */
  istart = tempfarb_ofset;
  r = g = b = 5;

  for (i = 0; i < 10; i++) 
  {
    for (j = 0; j < 200; j++)  mapcolor(istart++, r, b, g);
    r = b = g = (g+25); 
  }
}

/* ********************************************************************* */
rastermap()                    /* gerasterte Colormap --------------- */
/* ********************************************************************* */
{
     Colorindex   i, j, istart;
     short        r, g, b;
/* ********************************************************************* */
   colmap = 3;                 /* Kennung fuer Rasterbelegung ------- */
   istart = tempfarb_ofset;
   r = g = 0;
   b = 250;

   for (i = 0; i < 5; i++) 
   {
     for (j = 0; j < 200; j++)  mapcolor(istart++, r, g, b);
     r = r + 50;
     b = b - 50;
   }

   for (i = 5; i < 10; i++) 
   {
     for (j = 0; j < 200; j++)  mapcolor(istart++, r, g, b);
     g = g + 50;
   }
}
#endif
