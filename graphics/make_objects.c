/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   make_objects.c                                   */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/*  zeichne_kreuz    | erzeugt Koordinatenkreuz                  */
/*  zeichne_rahmen   | erzeugt Rahmen                            */
/*  zeichne_tabelle  | erzeugt Tabelle                           */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include "grafik.h"
#include "paulslib.h"
#include "sovLayerUtil.h"
#include "glut.h"
#include "ref.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
enum{
	C3D_TOP,
	C3D_BOTTOM,
	C3D_LEFT,
	C3D_RIGHT
};
	
/* Routinen :                                                    */
/* ******************************************************************** */

void draw_x()
{
  glBegin(GL_LINES);
  glVertex3f(-0.20, 0.33, 0.0);
  glVertex3f(0.20,-0.33,0.0);
  glVertex3f(0.20,0.33,0.0);
  glVertex3f(-0.20,-0.33,0.0);
  glEnd();
    
}

void draw_y()
{
  glBegin(GL_LINES);
  glVertex3f(-0.20, 0.33, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.20, 0.33, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, -0.33, 0.0);
  glEnd();
}

void draw_z()
{
  glBegin(GL_LINES);
  glVertex3f(-0.20, 0.33, 0.0);
  glVertex3f(0.20, 0.33, 0.0);
  glVertex3f(0.20, 0.33, 0.0);
  glVertex3f(-0.20, -0.33, 0.0);
  glVertex3f(-0.20, -0.33, 0.0);
  glVertex3f(0.20, -0.33, 0.0);
  glEnd();
}

void draw_origin()
{
  float x_x,x_y,x_z,x_t,
  y_x,y_y,y_z,y_t,
  z_x,z_y,z_z,z_t;	
  GLfloat matrix[16];
  float laenge,ix,iy;
  int vp_x,vp_y,vp_w,vp_h;	/* Viewportkoordinaten */
  GLubyte rasters[3][8] = {0,198,198,108,56,108,198,198, /*X*/
  			   96,112,56,28,62,103,99,0,	 /*Y*/
  			   0,254,92,48,24,12,254,0};	 /*Z*/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(Projektions_matrix);
  ///glTranslatef(g_transform.move[0], g_transform.move[1],
  ///	       g_transform.move[2]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(Translation_matrix);
  glPushMatrix();
  M_setrotationx(90.0, Rotation_matrix);
  glMultMatrixf(Rotation_matrix);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glBegin(GL_LINES);

  glColor3ubv(RGB_Gelb);
  glVertex3f( 0.0, 0.0, 0.0);
  glVertex3f( 0.0, 0.0, -1.0);
   
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);
    
  glVertex3f( 0.0, 0.0, 0.0);
  glVertex3f( 1.0, 0.0, 0.0);
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  glLineStipple (4, 0xAAAA);

  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, -0.4);
  glVertex3f(0.4, 0.0, -0.4);
  glVertex3f(0.4, 0.0, -0.4);
  glVertex3f(0.4, 0.0,  0.0); 
 
  glVertex3f(0.0, 0.4,  0.0);
  glVertex3f(0.4, 0.4,  0.0);
  glVertex3f(0.4, 0.4,  0.0);
  glVertex3f(0.4, 0.0,  0.0);

  glVertex3f(0.0, 0.0, -0.4);
  glVertex3f(0.0, 0.4, -0.4);
  glVertex3f(0.0, 0.4, -0.4);
  glVertex3f(0.0, 0.4,  0.0);

  glEnd();
  glDisable(GL_LINE_STIPPLE);
    
  glPushMatrix();
  glRotatef(90.0,0,1,0);
  glTranslatef(0.0,0.0,1.5);
  draw_x();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.0,0.0,-1.5);
  draw_y();
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(-90.0,1,0,0);
  glTranslatef(0.0,0.0,1.5);
  draw_z();
  glPopMatrix();
  
  glPopMatrix();
}

/* ******************************************************************** */
void zeichne_kreuz()   /* zeichne das Koordinatenkreuz ---------------- */
/* ******************************************************************** */
{
  int colormap_index;
  GLubyte col[3][3];
  float lx,ly,lz;
  GLint viewport[4]; 		/* Buffer fuer ViewportPixel-Koordinaten*/
  int vp_x,vp_y,vp_w,vp_h;	/* Viewportkoordinaten */
  float x_x,x_y,x_z,x_t,
        y_x,y_y,y_z,y_t,
        z_x,z_y,z_z,z_t;	/* Koordinaten fuer die Zeichen */
  int i,j;
  int color_x,color_y,color_z;
  static int first=0;
  GLubyte rasters[3][8] = {0,198,198,108,56,108,198,198, /*X*/
  			   96,112,56,28,62,103,99,0,	 /*Y*/
  			   0,254,92,48,24,12,254,0};	 /*Z*/
  
  float laenge,ix,iy;
  float scale;
  GLfloat matrix[16];
  
/* ******************************************************************** */
 
  if((!C3D_win.achtbit)&&(g_tabelle.art==1))
  {
    colormap_index = STOFFARB_OFFSET;
  }
  else
  {
    colormap_index = ACHT_BIT_OFFSET;
  }

  disableTex();
  glLineWidth(1.0);
  
  glGetIntegerv(GL_VIEWPORT,viewport);
  vp_x = viewport[0];
  vp_y = viewport[1];
  vp_w = viewport[2];
  vp_h = viewport[3];
  
  lx  = (float)vp_w;
  ly  = (float)vp_h;
  lz  = lx;
  
  laenge = 0.13;
  
  lx = laenge*801.0/lx;
  ly = laenge*752.0/ly;
  lz = laenge;
 
  glLineWidth(1.0);
  for(i = 0; i < 3; i++)
    col[0][i] = (GLubyte)g_tabelle.colortable[4*(colormap_index+1)+i+1];
  glColor3ubv(col[0]);
 
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(first) {
    glTranslatef(0.0, 0.0, 0.0);
    first = 1;
  }else{
    if (g_projektion.perspektive)
      glOrtho(g_projektion.left,g_projektion.right,g_projektion.bottom,
	      g_projektion.top, g_projektion.near, g_projektion.far);
    else
      glFrustum(g_projektion.left,g_projektion.right,g_projektion.bottom,
		g_projektion.top, g_projektion.near, g_projektion.far);

    glTranslatef(g_transform.move[0], g_transform.move[1],
		 g_transform.move[2]);
  }
  glLineWidth(2.0);
   
  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
  {
   color_x = CCROSS_X;
   color_y = CCROSS_Y;
   color_z = CCROSS_Z;
  }
  else
  {
   color_x = FARBE_KREUZ_X_8BIT;
   color_y = FARBE_KREUZ_Y_8BIT;
   color_z = FARBE_KREUZ_Z_8BIT;
  } 
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-0.9,-0.9,0.0);
 
  glRotatef(g_transform.rotx+90.0,1.0,0.0,0.0);
  glRotatef(g_transform.rotz,0.0,1.0,0.0);
  glRotatef(-g_transform.roty,0.0,0.0,1.0);
 
  glBegin(GL_LINES);

  /* X-Achse */
  for(i = 0; i < 3; i++)
    col[0][i] = (GLubyte)g_tabelle.colortable[4*color_x+i+1];
  glColor3ubv(col[0]);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(lx,0.0,0.0);
  
  /* Y-Achse */
  for(i = 0; i < 3; i++)
    col[1][i] = (GLubyte)g_tabelle.colortable[4*color_y+i+1];
  glColor3ubv(col[1]);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,-ly);
 
  /* Z-Achse */
  for(i = 0; i < 3; i++)
    col[2][i] = (GLubyte)g_tabelle.colortable[4*color_z+i+1];
  glColor3ubv(col[2]);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,lz,0.0);
  
  glEnd();
  
  glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
  
  x_x = matrix[4*0+0];
  x_y = -matrix[4*0+1];
  x_z = matrix[4*0+2]; 
  
  z_x = matrix[4*1+0];
  z_y = -matrix[4*1+1];
  z_z = matrix[4*1+2]; 
  
  y_x = -matrix[4*2+0];
  y_y = matrix[4*2+1];
  y_z = -matrix[4*2+2]; 
  
  glMatrixMode(GL_PROJECTION);
    glOrtho(0,vp_w,0,vp_h,-1.0,1.0);
    ix = ((float)vp_w/801.0)*40.0;
    iy = ((float)vp_h/752.0)*30.0;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 
      glColor3ubv(col[0]);
      glRasterPos2i(ix+x_x*60.0,iy-x_y*60.0); 
      glBitmap(8,8,0.0,0.0,0.0,0.0,rasters[0]);
  
      glColor3ubv(col[1]);
      glRasterPos2i(ix+y_x*60.0,iy-y_y*60.0);
      glBitmap(8,8,0.0,0.0,0.0,0.0,rasters[1]);
  
      glColor3ubv(col[2]);
      glRasterPos2i(ix+z_x*60.0,iy-z_y*60.0);
      glBitmap(8,8,0.0,0.0,0.0,0.0,rasters[2]);
   glPopMatrix();
 
  glLineWidth(1.0);
 
  return;
/* **************** ende zeichne_kreuz ******************************** */
}

/* ******************************************************************** */
void zeichne_rahmen(int stoffnr) /* zeichne Rahmen */
/* ******************************************************************** */
{
  t_koord R_max, R_min;
  t_koord Rahmen[8];
  int     colormap_index;
/* ******************************************************************** */
  R_max[0] = R_max[1] = R_max[2] = -10000.0; 
  R_min[0] = R_min[1] = R_min[2] =  10000.0; 
  
  if (g_Maske[stoffnr])
  {
    R_max[0] = MAX(R_max[0], g_stoffgebiete[stoffnr-1].max[0]);
    R_max[1] = MAX(R_max[1], g_stoffgebiete[stoffnr-1].max[1]);
    R_max[2] = MAX(R_max[2], g_stoffgebiete[stoffnr-1].max[2]);
    R_min[0] = MIN(R_min[0], g_stoffgebiete[stoffnr-1].min[0]);
    R_min[1] = MIN(R_min[1], g_stoffgebiete[stoffnr-1].min[1]);
    R_min[2] = MIN(R_min[2], g_stoffgebiete[stoffnr-1].min[2]);
  }

  if (g_geodat.npunkt == 0)
  {
    R_min[0] = -5;
    R_min[1] = -5;
    R_min[2] = -5;
    R_max[0] = 5;
    R_max[1] = 5;
    R_max[2] = 5;
  }

  Rahmen[0][0] = Rahmen[3][0] = Rahmen[4][0] = Rahmen[7][0] = g_stoffgebiete[stoffnr-1].min[0];
  Rahmen[2][1] = Rahmen[3][1] = Rahmen[6][1] = Rahmen[7][1] = g_stoffgebiete[stoffnr-1].min[1];
  Rahmen[0][2] = Rahmen[1][2] = Rahmen[2][2] = Rahmen[3][2] = g_stoffgebiete[stoffnr-1].min[2];
  Rahmen[1][0] = Rahmen[2][0] = Rahmen[5][0] = Rahmen[6][0] = g_stoffgebiete[stoffnr-1].max[0];
  Rahmen[0][1] = Rahmen[1][1] = Rahmen[4][1] = Rahmen[5][1] = g_stoffgebiete[stoffnr-1].max[1];
  Rahmen[4][2] = Rahmen[5][2] = Rahmen[6][2] = Rahmen[7][2] = g_stoffgebiete[stoffnr-1].max[2];

  if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
    colormap_index = STOFFARB_OFFSET +1 + stoffnr;
  else
    colormap_index = ACHT_BIT_OFFSET +1 + stoffnr;
  glIndexi(colormap_index);
  glBegin(GL_LINE_STRIP);
  glVertex3fv(Rahmen[0]);
  glVertex3fv(Rahmen[1]);
  glVertex3fv(Rahmen[2]);
  glVertex3fv(Rahmen[3]);
  glVertex3fv(Rahmen[0]);
  glVertex3fv(Rahmen[4]);
  glVertex3fv(Rahmen[5]);
  glVertex3fv(Rahmen[6]);
  glVertex3fv(Rahmen[7]);
  glVertex3fv(Rahmen[4]);
  glVertex3fv(Rahmen[5]);
  glVertex3fv(Rahmen[1]);
  glVertex3fv(Rahmen[2]);
  glVertex3fv(Rahmen[6]);
  glVertex3fv(Rahmen[7]);
  glVertex3fv(Rahmen[3]);
  glEnd();

  return;
/* **************** ende zeichne_rahmen ******************************* */
}

/* ******************************************************************** */
void zeichne_tabelle()
/* ******************************************************************** */
{
	t_koord p_koord[22];          /* Koordinaten des Farbfeldes -------- */
	float   diff;                 /* Unterteilung der Farbpalette ------ */
	float   t, t_min, t_max, 
		t_diff;                     /* Min. Max. und delta T fuer Intervall*/
	GLubyte minwert[3], 
		maxwert[3];                 /* minimaler u. maximaler Farbwert --- */
	GLubyte color[11][3];
	GLfloat texcoord[11];         /* texture coordinate ---------------- */
	int     near, far;            /* near- und far-Clippingplanes ------ */
	int     colmap_index;         /* Farbindex fuer Colormap ----------- */
	int     i;
	char    c_ausgabe[16];        /* Ausgabestring --------------------- */
	char    c_hilf[16];           /* Hilfsstring */
	char    disp_text[40];
	Display *dpy;
	GLXContext cx;                /* Grafik-Kontext -------------------- */
	int cindex;                   /* color-index------------------------ */
	XFontStruct* font;            /* font for drawing num. ------------- */
	int pixel_height, pixel_width;
	Arg       args[10];
	Cardinal  n;
	Dimension width, height; 
	int tcol;
	COLOUR c;                     /* value of the color r,g,b ---------- */
	t_bool    texture_on;
	float xpos, ypos;             /* position of display text ---------- */
	int numstripes;
	int support; 
        int legend_space, legend_width, legend_height;
        int left, bottom;
	char dummy;

	/* ******************************************************************** */
	/* Ruecksprung, falls kein Colormapfenster geoeffnet ------------------ */
	if (!g_tabelle.on) return;
  
        /*XXX setting values of colormap */
        g_tabelle.size = 256;
        g_tabelle.pos = C3D_TOP;
        g_tabelle.margin_x = 0;
	g_tabelle.margin_y = 0;
	C3D_win.font_height = 20;
	

	/* get the colormap */
	tcol = g_color_extra.color_name;

	/* Mache Colormapfenster aktuell -------------------------------------- */
	//GLwDrawingAreaMakeCurrent(CMAP_win.wid, CMAP_win.cx);
	GLwDrawingAreaMakeCurrent( C3D_win.wid, C3D_win.cx );
	/* Bestimme Breite und Hoehe des Tabellenfensters --------------------- */
	XtVaGetValues(widgetMain.S_colormap, XmNwidth, &width,
			XmNheight, &height, NULL);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	R_setGL2D();

	if ((!C3D_win.achtbit) && (g_tabelle.art == 1))
		colmap_index = STOFFARB_OFFSET;
	else
		colmap_index = ACHT_BIT_OFFSET;

	if ((!C3D_win.achtbit)&&(g_tabelle.art==1)) /* 24-Bit,Default-Belegung*/
	{
		for(i = 0; i < 3; i++)
			minwert[i] = g_tabelle.colortable[i+1];
		for(i = 0; i < 3; i++)
			maxwert[i] = g_tabelle.colortable[4*(COLORTABLE_LENGTH-1)+i+1];
	}
	else
	{
		for(i = 0; i < 3; i++)
			minwert[i] = g_tabelle.colortable[ACHT_BIT_TEMP_OFFSET];
		for(i = 0; i < 3; i++)
			maxwert[i] = 
				g_tabelle.colortable[ACHT_BIT_TEMP_OFFSET + ACHT_BIT_COLORTABLE_LENGTH-1];
	}

	/* decide about position of the color legend */
	if ( g_tabelle.pos == C3D_BOTTOM )
	{
		left = 50 + g_tabelle.margin_x;
		bottom = C3D_win.gl_height - 20 + g_tabelle.margin_y;
	}
	else if ( g_tabelle.pos == C3D_TOP )
	{
		left = 50 + g_tabelle.margin_x;
		bottom = g_tabelle.size + 5 + C3D_win.font_height + 25 + g_tabelle.margin_y;
	}
	else if ( g_tabelle.pos == C3D_RIGHT )
	{
		left = C3D_win.gl_width - (35 * g_tabelle.size / 128 ) - 5 * C3D_win.font_height + 
			g_tabelle.margin_x;
		bottom = g_tabelle.size + 5 + C3D_win.font_height + 50 + g_tabelle.margin_y;
	}
	else if ( g_tabelle.pos == C3D_LEFT ) 
	{
		left = 20 + g_tabelle.margin_x;
		bottom = g_tabelle.size + 5 + C3D_win.font_height + 100 + g_tabelle.margin_y;
	}
	else
	{
		SYS_error( "zeichne_tabelle: bad legend position\n" );
		return;
	}

	/* legend coordiantes */
	legend_height = g_tabelle.size;
	legend_width = (25 * g_tabelle.size ) / 128;
	legend_space = (20 * g_tabelle.size ) / 128;

	p_koord[0][0] = left;
	p_koord[0][1] = bottom;
	p_koord[0][2] = 0.0;
	p_koord[1][0] = left + legend_width;
	p_koord[1][1] = bottom;
	p_koord[1][2] = 0.0;
	p_koord[2][0] = left + legend_width;
	p_koord[2][1] = bottom - legend_height + 1;
	p_koord[2][2] = 0.0;
	p_koord[3][0] = left;
	p_koord[3][1] = bottom - legend_height + 1; 
	p_koord[3][2] = 0.0;


	//diff = 50.0;
	diff = legend_space;

	/* calculate temperature scale */
	t_min = g_loesung.Min;
	t_max = g_loesung.Max;
	t_diff = (float) (g_loesung.Max - g_loesung.Min) / 10.0;

	/* see if  texture is on*/
	texture_on = g_color_extra.texture_on;
	if (texture_on) 
	{
		t_texture_info *t = &g_tex_info;
		if(g_tabelle.art == 1) 
			glBindTexture(GL_TEXTURE_1D, t->texName[0]);
		else if(g_tabelle.art == 3) 
			glBindTexture(GL_TEXTURE_1D, t->texName[1]);

		glEnable(GL_TEXTURE_1D);
	} 

	/* assign colors to temp. values */
	for(i = 0; i <= 10; i++)
	{
		t = t_min + t_diff * i;
		cindex = (int)(t*COLORTABLE_LENGTH - t_min*COLORTABLE_LENGTH) /
			(t_max-t_min);

		/* get texture values */
		if (texture_on) 
		{
			texcoord[i] = CcolTex(t);
		} 
		else  /* get color values */
		{	
			if (tcol)
			{
				c = GetColour(t, g_loesung.Min, g_loesung.Max, tcol);
				color[i][0] = (GLubyte)(c.r * 255.0);
				color[i][1] = (GLubyte)(c.g * 255.0);
				color[i][2] = (GLubyte)(c.b * 255.0);
			}
			else
			{
				color[i][0] = (GLubyte) g_tabelle.colortable[4*cindex+1];
				color[i][1] = (GLubyte) g_tabelle.colortable[4*cindex+2];
				color[i][2] = (GLubyte) g_tabelle.colortable[4*cindex+3];
			}
		}
	}

	/* OpenGl-init ------------------------------------------------------ */
	//glViewport(0, 0, width, height);
	//glShadeModel(GL_SMOOTH);
	C3D_clearcolor();
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	near = -1;
	far  =  1;

	/* Lade Identitaetsmatrix auf Stack und gehe in Ortho-Darstellung -- */
	//glMatrixMode (GL_PROJECTION);
	//glPushMatrix();
	// glLoadIdentity();
	//glOrtho(0.0, 260.0, 0.0, 560.0, near, far);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glTranslatef(0.0,0.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* choose the raster or default */
	if(texture_on) 
	{
		t_texture_info *t = &g_tex_info;
		if(g_tabelle.art == 3)
			glBindTexture(GL_TEXTURE_1D, t->texName[1]);
		else 
			glBindTexture(GL_TEXTURE_1D, t->texName[0]);
	}

	glDisable(GL_LIGHTING);
	//glColor3ubv(c3d_foreground_color);

	// draw color values
	glBegin(GL_QUAD_STRIP);
	for(i = 0; i <= 10; i++)
	{
		/* calculate position of the point 
		 * in the color table.
		 */
		p_koord[i*2  ][0] = left;
		p_koord[i*2+1][0] = left + legend_width;
		p_koord[i*2  ][1] = bottom - legend_height + 1 + diff * i;
		p_koord[i*2+1][1] = bottom - legend_height + 1 + diff * i;
		p_koord[i*2  ][2] = 0.0;
		p_koord[i*2+1][2] = 0.0;
		/* set tex. coord. of temp. */
		if (texture_on) 
		{
			glColor3ubv(RGB_Weiss); 
			glTexCoord1f(texcoord[i]);
		}
		else
			/* else set color coord. of temp. */
			glColor3ubv(color[i]);

		glVertex3fv(p_koord[i*2  ]);
		glVertex3fv(p_koord[i*2+1]);
	}
	glEnd();

	disableTex();

	p_koord[0][2]  = 0.1;
	p_koord[1][2]  = 0.1;
	p_koord[20][2] = 0.1;
	p_koord[21][2] = 0.1;

	if((!C3D_win.achtbit)&&(g_tabelle.art==1))
	{
		glIndexi(FARBE_SCHWARZ);
	}
	else
	{
		glIndexi(FARBE_SCHWARZ_ACHT);
	}

	// draw lines around color-table.
	glColor3ubv(c3d_foreground_color);
	glLineWidth(LINE_WIDTH);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(p_koord[0]);
	glVertex3fv(p_koord[1]);
	glVertex3fv(p_koord[21]);
	glVertex3fv(p_koord[20]);
	glEnd();

	if (texture_on && g_tabelle.art == 3 )
		numstripes = g_tex_info.nstripes;
	else
		numstripes = 10;

	glBegin(GL_LINES);
	for (i = 0; i <= numstripes; i++)
	{
		p_koord[i*2  ][2] = 0.1;
		p_koord[i*2+1][2] = 0.1;
		glVertex3fv(p_koord[i*2]);
		glVertex3fv(p_koord[i*2+1]);
	}
	glEnd();

	p_koord[0][0] = left + legend_width;
	p_koord[0][1] = bottom - legend_height;
	/* query font */
	font = QueryFont(XtDisplay(CMAP_win.wid), FONT_HELVETICA_MEDIUM_14, CMAP_win.cx);
	pixel_width  = XTextWidth(font, "a" ,1);
	pixel_height = font->ascent + font->descent;

#ifdef DEBUG
	if(CMAP_win.wid == C3D_win.wid)
		printf("The graphic context is same, change it!\n");

#endif

	/* draw values of temp. on color table */
	for (i = 0; i <= 10; i++)
	{
		/* set color */
		glColor3ubv(c3d_foreground_color);
		DrawNumberAtPos(t_max-i*t_diff, CMAP_win.wid, font, 
				p_koord[0][0], p_koord[0][1]);
		p_koord[0][1] = p_koord[0][1] + diff;
	}

	strcpy(disp_text, g_loesdef[0].text);
	strcat(disp_text, "  ");
	strcat(disp_text, g_loesdef[0].einheit);

	ypos = bottom - legend_height - pixel_height - 10;
	xpos = left;

	/* display text and dimensions */
	DrawText(disp_text, font, C3D_win.wid, xpos, ypos);

	/* swap und clear double-Buffers -------------------------------------- */
	//GLwDrawingAreaSwapBuffers (CMAP_win.wid);
	//GLwDrawingAreaSwapBuffers( C3D_win.wid ); 
	/* setze Projektionsmatrix zurueck ------------------------------------ */ 
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	glPopMatrix();

	/* setzt Drawing Area auf Mainfenster zurueck ------------------------- */
	GLwDrawingAreaMakeCurrent(C3D_win.wid, C3D_win.cx);
	return;
	/* *************** ende zeichne_tabelle ******************************* */
}








