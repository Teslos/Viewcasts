/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   08.09.98                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname         | Funktion                               */
/* ------------------------------------------------------------- */
/* zeichne_abkuehlkurven| 	         			 */
/* zeichne_skala        |  		         		 */
/* zeichne_kurven       |	       		         	 */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "color3d.h"
#include "motif.h"
#include "grafik.h"

#define ARROW   1
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
#define MAX(a,b)   ((a)>(b)?(a):(b))
#define CYELLOW    2021
#define XDIS       70.0
#define YDIS       50.0
#define DEFPRES    2     
#define MIN_VALUE  -10000
#define MAX_VALUE   10000
     
enum{ GNORM = 1, GSCI = 2};

/* functions */
static int cull_curve(float maxwert, float minwert, 
		      float maxzeit, float minzeit, t_pickpunkt *pick);

/* ****************************************************************** */
int zeichne_abkuehlkurven()
{
  t_pickpunkt *p_pickpunkt;
  struct listelem *pickpunkt;

  int         i;
  float       maxwert, minwert,
              maxzeit, minzeit;
#ifdef DEBUG
  if (applRessourcen.debug)
    fprintf(stderr, "*** Start zeichne_abkuehlkurven()  ***\n");
#endif

  /* ****************************************************************** */
  /* Ruecksprung falls keine Loesung eingelesen ----------------------- */
  if (!g_loesung.gelesen) return (1);
  /* Fenster initialisieren und oeffnen falls noch nicht auf ---------- */
  /*
  if (!ABK_win.on)
    {
      XtPopup(widget_main.S_abkuehl, XtGrabNone);
      ABK_win.on = TRUE;
    }
  */

  maxwert     = MIN_VALUE;
  minwert     = MAX_VALUE;
     
 
  if (g_picking.punkte != NULL) 
    {
      pickpunkt = g_picking.punkte->head;

      /* Schleife ueber alle gepickten Punkte, mit lesen der Loesungswerte */
      while (pickpunkt != NULL)
	{
	  p_pickpunkt = (t_pickpunkt*) pickpunkt->data;
	      
	  /* bestimme minimalen und maximalen Wert der Abkuehlkurven ----------- */ 
	  maxwert     = MAX(maxwert, p_pickpunkt->max);
	  minwert     = MIN(minwert, p_pickpunkt->min);
      
	  pickpunkt = pickpunkt->next;
	}
    }

  minzeit = g_loesung.zeiten[0];
  maxzeit = g_loesung.zeiten[g_loesung.anzahl_felder-1];

  /* set apropriate min and max if user manualy scale curve */
  if (g_picking.manuell_skaliert || g_picking.fscale)
    {
      maxwert = (g_picking.O_max < maxwert && 
		 g_picking.O_max > minwert) ? g_picking.O_max : maxwert;
      minwert = (g_picking.O_min < maxwert && 
		 g_picking.O_min > minwert) ? g_picking.O_min : minwert;
      maxzeit = (g_picking.A_max < maxzeit &&
		 g_picking.A_max > minzeit) ? g_picking.A_max : maxzeit;
      minzeit = (g_picking.A_min < maxzeit &&
		 g_picking.A_min > minzeit) ? g_picking.A_min : minzeit;
    }

  zeichne_kurven(maxwert, minwert, maxzeit, minzeit);

  return 0;
}

/* ****************************************************************** */
void zeichne_skala(char** labels_zeit, 
		   char** labels_wert, int nticks_zeit, int nticks_wert)
/* ****************************************************************** */
{
  t_koord p_koord[4];           /* Koordinaten des Farbfeldes -------- */
  float   diff;                 /* Unterteilung der Farbpalette ------ */
  float   pos_maxzeit[2],       /* position of min, max time --------- */
          pos_minzeit[2];
  float   pos_maxwert[2],       /* position of min, max value -------- */
          pos_minwert[2];
  XFontStruct* font;            /* font to draw labels --------------- */ 
  float xdiff, ydiff;           /* difference beetwen ticks ---------- */ 
  float umin,vmin;              /* translations in viewport (u,v) ---- */
  Dimension width, height;      /* width and height of window -------- */
  int plot;                     /* ploting style --------------------- */
  int i,j;                      /* counters -------------------------- */
  int pixel_width,              /* pixel height and width ------------ */
      pixel_height;
  char  dummy[81];
  GLubyte col[3];
  float minwert, maxwert, minzeit, maxzeit;
  static float arrow[3][2]={
    {4.0,0.0},
    {0.0,20.0},
    {-4.0,0.0}
  };

  /* choose default plot style */
  plot = GSCI;

  umin = XDIS; 
  vmin = YDIS; 
  
  /* Bestimme Breite und Hoehe des Tabellenfensters --------------------- */
  XtVaGetValues(widgetMain.S_abkuehl, XmNheight, &height, 
		XmNwidth, &width, NULL);

  p_koord[0][0] =  umin;
  p_koord[0][1] =  vmin;
  p_koord[1][0] =  umin;
  p_koord[1][1] =  height-vmin;
  p_koord[2][0] =  width-umin;
  p_koord[2][1] =  height-vmin;
  p_koord[3][0] =  width-umin;
  p_koord[3][1] =  vmin;

  /*for(i = 0; i < 4; i++){
    p_koord[i][0] += umin;
    p_koord[i][1] += vmin;
    }*/
  /* get max, min values */
  maxwert = atof(labels_wert[nticks_wert]);
  minwert = atof(labels_wert[0]);
  maxzeit = atof(labels_zeit[nticks_zeit]);
  minzeit = atof(labels_zeit[0]);

  /* load the font */
  font = QueryFont(XtDisplay(ABK_win.wid), FONT_FIXED2, ABK_win.cx);

  diff = 40.0;
  if((!C3D_win.achtbit)&&(g_tabelle.art==1))
    {
      for(i = 0; i < 3; i++)
	col[i] = (GLubyte) g_tabelle.colortable[4*FARBE_SCHWARZ+i+1];
      /*glIndexi(FARBE_SCHWARZ);*/
    }
  else
    {
      glIndexi(FARBE_SCHWARZ_ACHT);
    }

  glColor3ubv(c3d_foreground_color);   /*yellow or black*/

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, 0.0);
  glLineWidth(1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin( GL_QUADS );
  glVertex2fv(p_koord[0]);
  glVertex2fv(p_koord[1]);
  glVertex2fv(p_koord[2]);
  glVertex2fv(p_koord[3]);
  glEnd();

  if(plot == GNORM)
    { 
      /*draw arrows*/
      glNewList(ARROW,GL_COMPILE);
      glBegin(GL_TRIANGLES);
      glVertex2fv(arrow[0]);
      glVertex2fv(arrow[1]);
      glVertex2fv(arrow[2]);
      glEnd();
      glEndList();

      /*Translate arrows*/
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(p_koord[2][0],p_koord[2][1],0.0);
      glCallList(ARROW);
      glPopMatrix();
    
      glPushMatrix();
      glTranslatef(p_koord[1][0],p_koord[1][1],0.0);
      glRotatef(-90.0, 0.0, 0.0, 1.0);
      glCallList(ARROW);
      glPopMatrix();
    }
   
  pos_maxwert[0] = p_koord[1][0];
  pos_maxwert[1] = p_koord[1][1];
  pos_minwert[0] = p_koord[0][0];
  pos_minwert[1] = p_koord[0][1];
    
  pos_maxzeit[0] = p_koord[3][0];
  pos_maxzeit[1] = p_koord[3][1];
  pos_minzeit[0] = p_koord[0][0];
  pos_minzeit[1] = p_koord[0][1];

  pixel_width  = XTextWidth(font, "a" ,1);
  pixel_height = font->ascent + font->descent;
  pos_maxwert[0] -= pixel_width*(GetNumber(maxwert, dummy,DEFPRES) + 2);
  pos_minwert[0] -= pixel_width*(GetNumber(maxwert, dummy,DEFPRES) + 2);
  pos_minzeit[1] -= pixel_height;
  pos_maxzeit[1] -= pixel_height;
  pos_maxzeit[0] -= pixel_width*GetNumber(maxzeit, dummy,DEFPRES);
  pos_minzeit[0] -= pixel_width*GetNumber(maxzeit, dummy,DEFPRES);
  
  /* calculate the difference 
   * beetwen two ticks on graph
   */
  xdiff = (pos_maxzeit[0] - pos_minzeit[0]) / nticks_zeit;
  ydiff = (pos_maxwert[1] - pos_minwert[1]) / nticks_wert;

  /* make current this widget */
  glXMakeCurrent(XtDisplay(ABK_win.wid), 
		 XtWindow(ABK_win.wid), ABK_win.cx);
    
  /* position and draw the values 
   * for every tick draw a label
   */
  
  for(i = 0; i <= nticks_zeit; i++)
    {
      DrawNumberAtPos(atof(labels_zeit[i]), ABK_win.wid, font,
		      pos_minzeit[0]+xdiff*i,
		      pos_minzeit[1]);
      glBegin(GL_LINES);
      glVertex2f(p_koord[0][0]+xdiff*i, p_koord[0][1]);
      glVertex2f(p_koord[0][0]+xdiff*i, p_koord[1][1]);
      glEnd();
    }
  for(i = 0; i <= nticks_wert; i++)
    {
      DrawNumberAtPos(atof(labels_wert[i]), ABK_win.wid, font,
		      pos_minwert[0], 
		      pos_minwert[1]+ydiff*i);
      glBegin(GL_LINES);
      glVertex2f(p_koord[0][0], p_koord[0][1]+ydiff*i);
      glVertex2f(p_koord[3][0], p_koord[0][1]+ydiff*i);
      glEnd();
    }

}

/**************************************************************************
 zeichne_kurven - shows the cooling curves (temperature vs time).

 Parameters:
 ===========
           float maxwert - maximum value of "cooling" curve
	   float minwert - minimum value of "cooling" curve
	   float minzeit - minimum time of "cooling" curve
	   float maxzeit - maximum time of "cooling" curve
 Returns:
 ========
           It returns nothing.
**************************************************************************/
void zeichne_kurven(float maxwert, float minwert,
                    float maxzeit, float minzeit)
{
  t_pickpunkt *pickpunkt; 
  struct listelem  *p_pickpunkt;
  t_koord     p_koord;
  int         i,j;              /* counters                         */
  int     nticks_wert=10,
          nticks_zeit=10;       /* number of ticks on the graph     */
  int     near, far;            /* near- und far-Clippingplanes     */
  int     colmap_index;         /* Farbindex fuer Colormap          */
  float   xdiff, ydiff;         /* distance beetween points on cur. */
  float   maxval, minval;       /* round max and min values         */
  float   maxtime, mintime;     /* round max and min times          */
  float   dt;                   /* change in time                   */
  float   umin,vmin;            /* translations in viewport         */
  GLXContext glcontext;         /* GLX context for the widget       */
  char   *title;                /* title of the window              */
  XFontStruct*    font;
  GLubyte col[3];               /* color values                     */
  Dimension height, width;      /* height, width of widget          */
  char    **labels_wert;        /* array of labels                  */
  char    **labels_zeit;      
  int a, b, ts_range;           /* time-step range variables        */
  int k;                        /* color index                      */

  /* ********************************************************************* */
  /* Ruecksprung, falls kein Colormapfenster geoeffnet ------------------- */
  /*if (!g_tabelle.on) return;*/ 

  /* query for font */
  font = QueryFont(XtDisplay(ABK_win.wid),FONT_FIXED2, ABK_win.cx);
    
  /* setzt Drawing Area auf Mainfenster zurueck -------------------------- */
  GLwDrawingAreaMakeCurrent(ABK_win.wid, ABK_win.cx);

#ifdef DEBUG
  /*check for current context*/
  glcontext = glXGetCurrentContext();
  if(glcontext != ABK_win.cx)
    fprintf(stderr, "Something is wrong with GLX context!\n");
#endif

  /* Bestimme Breite und Hoehe des Tabellenfensters ---------------------- */
  XtVaGetValues(widgetMain.S_abkuehl, XmNheight, &height, 
		XmNwidth, &width, NULL);
 
  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
    colmap_index = STOFFARB_OFFSET;
  else
    colmap_index = ACHT_BIT_OFFSET;

  /* OpenGl-init ---------------------------------------------------------- */
  glViewport(0, 0, width, height);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(1.5);

  glShadeModel(GL_SMOOTH);
  C3D_clearcolor();
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* Lade Identitaetsmatrix auf Stack und gehe in Ortho-Darstellung ------- */
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluOrtho2D(0.0, width, 0.0, height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);
  
  labels_wert = (char**) malloc((size_t)(nticks_wert+1)*sizeof(char*));
  if(labels_wert == NULL) 
    fprintf(stderr, "Can't allocate labels-value\n"); 
  labels_wert[0] = (char *) malloc((size_t)((nticks_wert+1)*10)*sizeof(char));
  if(labels_wert == NULL)
    fprintf(stderr, "Can't allocate labels-value\n");
  for(i = 1; i <= nticks_wert+1; i++) 
    labels_wert[i] = labels_wert[i-1] + 10;
  
  labels_zeit = (char**) malloc((size_t)(nticks_zeit+1)*sizeof(char*));
  if(labels_zeit == NULL) 
    fprintf(stderr, "Can't allocate labels-time\n");
  labels_zeit[0] = (char*) malloc ((size_t)((nticks_zeit+1)*10)*sizeof(char));
  if (labels_zeit[0] == NULL) 
    fprintf(stderr, "Can't allocate labels-time\n");
  for(i = 1; i <= nticks_zeit+1; i++) 
    labels_zeit[i] = labels_zeit[i-1] + 10;

  /* choose nice labels for drawing graphs */
  nice_label(minwert, maxwert, labels_wert, &nticks_wert);
  nice_label(minzeit, maxzeit, labels_zeit, &nticks_zeit);
  
  /* translations in viewport. */
  umin = XDIS;
  vmin = YDIS;

  maxtime = atof(labels_zeit[nticks_zeit]);
  mintime = atof(labels_zeit[0]);
  maxval  = atof(labels_wert[nticks_wert]);
  minval  = atof(labels_wert[0]);

  assert(maxtime >= mintime);
  assert(maxval >= minval);

  /* transformations (scaling of coordinates)*/
  xdiff = (width-2*umin)/(maxtime - mintime);
  ydiff = (height-2*vmin)/(maxval - minval);

  /*
   * Draw cooling curves for all points,
   * which are currently picked.
   */
  k = 1;

  if (g_picking.punkte != NULL)
    {
      p_pickpunkt = g_picking.punkte->head;
      while(p_pickpunkt != NULL)
	{
	  k++;
	  pickpunkt = (t_pickpunkt *)p_pickpunkt->data;

	  /* If curve is complete out of visible 
	   * range of graph, jump to next curve.
	   */
	  if (cull_curve(maxwert, minwert, maxzeit, minzeit, pickpunkt))
	    {
	      p_pickpunkt = p_pickpunkt->next;
	      continue;
	    }

	  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
	    {
	     col[0] = UNPACK_RED  (pickpunkt->rgba);
	     col[1] = UNPACK_GREEN(pickpunkt->rgba);
	     col[2] = UNPACK_BLUE (pickpunkt->rgba);
	     glColor3ubv(col);
	    }
	  else
	    glIndexi(ACHT_BIT_OFFSET + k);
      
	 /* 
	  * find range for min and max time,
	  * minzeit < a and b < maxzeit.
	  */
	 for (i = 0; 
	      g_loesung.zeiten[i] <= minzeit; i++)
	   a = i;
	 for (j = g_loesung.anzahl_felder - 1; 
	      g_loesung.zeiten[j] >= maxzeit; j--)
	   b = j;

	 /* range of time-steps */
	 ts_range = b - a;

#ifdef DEBUG
	 printf ("Time step range a=%d, b=%d, range=%d\n",
		 a, b, ts_range);
#endif
	  
	 /*
	  * Draw line which is
	  * "cooling" curve.
	  */
	 dt = (maxzeit - minzeit) / ts_range;
	 
	 glScissor(umin, vmin, width-umin, height-vmin);
	 glEnable(GL_SCISSOR_TEST);

	 glBegin(GL_LINE_STRIP);

	 for (i = a; i <= b; i++)
	   {
	     p_koord[0] = (g_loesung.zeiten[i] - mintime) * xdiff + umin;

	     if (pickpunkt->punkt_nr <= g_geodat.npunkt)  /* normal nodes */
	       {
		 if (pickpunkt->werte[i] >= minwert && pickpunkt->werte[i] <= maxwert)
		   p_koord[1] = (pickpunkt->werte[i] - minval) * ydiff + vmin;
	       }
	     else    /* cutted nodes */
	       {
		 if (pickpunkt->werte1[i] >= minwert && pickpunkt->werte1[i] <= maxwert)
		   p_koord[1] = (pickpunkt->werte1[i] - minval) * ydiff + vmin;
	       }

	     glVertex2fv(p_koord);
	   }

	 glEnd();
	 glDisable(GL_SCISSOR_TEST);

	 p_pickpunkt = p_pickpunkt->next;
	}
    }

  /* Zeichne Skalierung der Abkuehlkurven -------------------------------- */
  zeichne_skala(labels_zeit, labels_wert, nticks_zeit, nticks_wert);
  
  /* free memory */
  free((char*)labels_wert[0]);
  free((char*)labels_wert);
  free((char*)labels_zeit[0]);
  free((char*)labels_zeit);

  /* display message */
  DrawText(geo_datei_name, font, widgetMain.S_abkuehl, 0,0);

  /* swap und clear double-Buffers --------------------------------------- */
  GLwDrawingAreaSwapBuffers (ABK_win.wid);
  
  /* setze Projektionsmatrix zurueck ------------------------------------- */
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  /* setzt Drawing Area auf Mainfenster zurueck -------------------------- */
  GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
}

void init_abk_fenster()

{
  int     farbemin, farbemax;   /* minimaler u. maximaler Farbwert --- */
  int     near, far;            /* near- und far-Clippingplanes ------ */
  int     colmap_index;         /* Farbindex fuer Colormap ----------- */
  Dimension width, height;      /* dimensions */


  /* Ruecksprung, falls kein Abkuehlkurvenfenster geoeffnet ----------- */
  if (!ABK_win.on) return;
  /* Bestimmr Breite und Hoehe des Tabellenfensters ------------------- */

  XtVaGetValues(widgetMain.S_abkuehl, XmNheight, &height, 
		XmNwidth, &width, NULL);

  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
    colmap_index = STOFFARB_OFFSET;
  else
    colmap_index = ACHT_BIT_OFFSET;
  
  /* OpenGl-init ------------------------------------------------------ */
  glViewport(0, 0, width, height);
  glClearIndex(colmap_index);
  glClear(GL_COLOR_BUFFER_BIT);

  near = -1;
  far  =  1;
 
  /* Lade Identitaetsmatrix auf Stack und gehe in Ortho-Darstellung -- */
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glOrtho(0, 500, 0, 500, near, far);

  if ((!C3D_win.achtbit)&&(g_tabelle.art==1)) /* 24-Bit + Default-Belegung --- */
    {
      farbemin = TEMPFARB_OFFSET;
      farbemax = TEMPFARB_OFFSET + COLORTABLE_LENGTH;
    }
  else
    {
      farbemin = ACHT_BIT_TEMP_OFFSET;
      farbemax = ACHT_BIT_TEMP_OFFSET + ACHT_BIT_COLORTABLE_LENGTH;
    }

  glPopMatrix();
  glShadeModel(GL_FLAT);
}

/**********************************************************
 This function query for X - font.
 It returns the desierd font.

 Parameters:
 ===========
            dpy      -  display for xwindow
	    font_num -  specify the font
	    cx       -  GLX graphic context
 Returns:
 ========
            font      - font for use
***********************************************************/

XFontStruct* QueryFont(Display *dpy, int font_num, GLXContext cx)
{
  /* local variables */
  XFontStruct* font;
  int i = 0;
  const int NUM_FONTS = 10;
 
  /* font list for GLX */
  char* FontList[]= {
    "-adobe-courier-bold-o-normal--14-140-75-75-m-90-iso8859-1",
    "-adobe-courier-bold-o-normal--10-100-75-75-m-60-iso8859-1",
    "-*-screen-medium-r-normal--15-*-*-*-m-80-iso8859-1",
    "-*-screen-medium-r-normal--10-*-*-*-m-60-iso8859-1",
    "-*-fixed-medium-r-normal--15-*-*-*-c-90-iso8859-1",
    "-*-fixed-medium-r-normal--13-*-*-*-c-70-iso8859-1",
    "-*-helvetica-medium-r-normal--14-*-*-*-p-76-iso8859-1",
    "-*-helvetica-bold-r-normal--14-*-*-*-p-76-iso8859-1",
    "-*-swiss721-medium-r-normal--14-*-*-*-p-106-iso8859-1",
    "-*-swiss721-bold-r-normal--14-*-*-*-p-106-iso8859-1",
  };

  /* query font from X server */
  font = XLoadQueryFont(dpy, FontList[font_num]);

  /* search for the next useable font */
  for ( i = 0; i < NUM_FONTS  && font == NULL; i++ )
    {
      font = XLoadQueryFont(dpy, FontList[i]);
    }

  return(font);
}

/************************************************************
 This function draws the text position on the center-top
 of the widget.
 
 Parameters:
 ===========
           message   - text to display
	   font_info - info about XFont
	   w         - widget to display information in
 Returns:
 ========
           -none
************************************************************/
void DrawText(char* message, XFontStruct *font_info, Widget w, 
	      float xpos, float ypos)
{
  int pixel_width, pixel_height;
  Dimension height, width;
  int offset_w, offset_h;
  int i;
  Font fid;
  GLubyte col[3];
  
  /* get height, width */
  if (xpos == 0.0 && ypos == 0.0)
    XtVaGetValues(w, XmNheight, &height, 
		  XmNwidth, &width, NULL);
   
  /* see how long is message */
  pixel_width  = XTextWidth (font_info, message, strlen(message));
  pixel_height = font_info->ascent + font_info->descent;

  if (xpos == 0 && ypos == 0.0)
    {
      /* calculate the middle of wind. */
      offset_w = (width-pixel_width)/2;
      offset_h = height-pixel_height;
    }
  else
    {
      offset_w = xpos;
      offset_h = ypos;
    }

  fid = font_info->fid;
  glColor3ubv(c3d_foreground_color);

  /* display message */
  glRasterPos2f(offset_w, offset_h);
  glXUseXFont(fid, 32, 96, (2000+32));
  glListBase(2000);
  glCallLists(strlen(message), GL_UNSIGNED_BYTE, message);
}

/****************************************************************
 This function draws the number on widget at specified position.
 
 Parameters:
 ===========
            value     - value to display
            w         - widget to display
            font_info - font informations
            xpos      - xpos at widget
	    ypos      - ypos at widget
 Returns:
 ========
            -none
****************************************************************/
void DrawNumberAtPos(float value, Widget w,XFontStruct *font_info,
		     float xpos, float ypos)
{
  int pixel_width, pixel_height;
  Dimension height, width;
  int offset_w, offset_h;
  Font fid;
  char des[81];
  char num[10];

  /* get height, width */
  XtVaGetValues(w, XmNheight, &height, 
		XmNwidth, &width, NULL);

  assert(width>0);
  assert(height>0);
  
  /* get the number from the float value */
  sprintf(num, "%7.4g",value);
  
  /* draw only in this format */
  sprintf(des, "%9s", num);
  fid = font_info->fid;
  glRasterPos2f(xpos, ypos);
  glXUseXFont(fid, 32, 96, (2000+32));
  glListBase(2000);
  glCallLists(strlen(des), GL_UNSIGNED_BYTE, des);
  glFlush();
}

/****************************************************************
 This function return "naked" number from the string.
 
 Parameters:
 ==========
              number - string given
	      des    - destination string
	      pres   - precision of number
 Returns:
 ========
              returns numbers of digits
****************************************************************/
int GetNumber(float number, char *des, int pres)
{
  int count = 0, pcount = 0 ;
  char *ptr, *pdes;
  char tmp[81];
  int fl=0;

  /* copy the pointer to local pointer*/
  sprintf(tmp, "%f", number);
  ptr = &tmp[0];
  pdes = &des[0];
  
  while(((*ptr) != '\0') && (pcount <= pres))
    {
      /* count number of digits */
      if(isdigit(*ptr) || (*ptr) == '.')
      {
	if((*ptr) == '.')
	  fl = 1;
	*pdes = *ptr;
	pdes++;
	*pdes = '\0';
	count++;
	if(fl) pcount++;
      }
      ptr++;
    }
  return count;
}

/****************************************************************
 This function is culling the curve with respect to graph area.

 Parameters:
 ===========
         float maxwert, minwert - max and min value of ordinate.
         float maxzeit, minzeit - max and min value of abcisse.
 Returns:
 ========
         returns true if curve is culled
         otherwise false.
****************************************************************/
static int cull_curve(float maxwert, float minwert, 
	       float maxzeit, float minzeit, t_pickpunkt *pick)
{
  if (pick->min > maxwert || pick->max < minwert)
    return (TRUE);
  else
    return (FALSE);
}

/****************************************************************
 This function writes a file for displaying with gnuplot.
 
 Parameters:
 ==========
             -none
 Returns:
 ========
              returns 0 if everything ok,
              -1 otherwise.
****************************************************************/      
int gnuplot()
{
  int i = 0;
  FILE *fp;
  char str[10];
  char name[100];
  char dateiname1[100];
  char dateiname2[100];
  char ausgabe[256];
  int np = 0;
  static int index_curve=0;
  t_pickpunkt *punkt;
  struct listelem *lpunkte;

  lpunkte = g_picking.punkte->head;
  
  np = 0;
  while(lpunkte)
    {
      index_curve++;
      punkt = (t_pickpunkt*) lpunkte->data;
      
      /* open file for reading and writing */
      sprintf(str,"%d",index_curve);
      strcpy(name,g_loesung.dateiname);
      strcat(name,str);
      fp = fopen(name,"w");
      if (fp == NULL) 
      {
	fprintf(stderr,"Error in opening gnuplot file\n");
	return(-1);
      }

      /* write header file */
      fprintf(fp,"# Result File: %s\n", g_loesung.dateiname);
      fprintf(fp,"# Node: %i\n", punkt->punkt_nr);
      fprintf(fp,"# Coordinates: x=%f, y=%f, z=%f\n", punkt->koord[0], 
	      punkt->koord[1], punkt->koord[2]);

      for (i = 0; i < g_loesung.anzahl_felder; i++) 
	{
	  fprintf(fp,"%g %g\n",
		  g_loesung.zeiten[i], punkt->werte[i]);
	}
    
      fclose(fp);
      lpunkte = lpunkte->next;
      np++;
      if (np == 1)
        strcpy(dateiname1, name);
      else
        strcpy(dateiname2, name);
    }
/* gebe Benutzer die geschriebenen Dateien an */
   sprintf(ausgabe, "%d parameter curves have been written to files:\n %s to %s",
          np, dateiname1, dateiname2);
   ask_user(ausgabe, "Ok", "", "", 1);

  return(0);
}








