/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   16.12.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/* zeichne_geometrie | zeichne Geometrie nach Grafikmaske        */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "grafik.h"
#include "strukturen.h"
#include "shade.h"
#include "ref.h"

/* Routine :                                                     */
/* ============================================================= */
/* zeichne_geometrie    | zeichnet die komplette Geometrie       */ 

/* Hilfs-Routinen :                                              */
/* ============================================================= */
/* build_oberflmat      | bildet Verschiebungsvektor fuer Netz   */          
/* build_zykl_oberflmat	| bildet zyklischen Verschiebungsvektor  */
/* verschiebe_werte_netz| verschiebt Netz zum Btrachter hin      */
/* verbiege_geo         | tauscht Org.-Geometrie mit Verspannung */
/* rueckbiege_geo       | stellt Org.-Geometrie wieder her       */
/*                                                               */
/*****************************************************************/

#define VERSCHIEBUNGS_QUOTIENT 60.0
/* functions */
static void setTransparent();
static void disableTransparent();

/* globale Variable :                                            */
/* ============================================================= */
t_knoten *org_knoten;          /* Zeiger auf original-Geometrie- */
static int Stip_available = 0; /* stipple pattern------ -------- */
static GLuint stipple[3][32];
static void init_graphics2( void );
/* Bitmap fuer gepickte Punkte: */
GLubyte pointmap[8] = {24,126,255,255,255,255,126,24};


build_zykl_oberflmat(float ax, float ay, float az, float *xn,
                     float *yn, float *zn)
/* ******************************************************************** */
{
  float um, wx, wy, wz,
    x,  y,  z,
    nx, ny, nz;
  float eps;                      /* Faktor fuer Netzverschiebung -- */
  float ausdehnung;
  float min_ausdehnung;
  float dx,dy,dz;
  int i;
  
  /* ******************************************************************** */
  um = PI / 180.0;

  wx = (um * (g_transform.rotx+90.0+ax));
  wy = (um * (g_transform.roty+ay));
  wz = (um * (g_transform.rotz+az));

  /* Drehung um die X-Achse : */
  ny = -cosf(wx);
  nz = sinf(wx);

  x = 0.0;
  y = ny;
  z = nz;

  /* Drehung um die Y-Achse : */
  nx = cosf(wy)*x-sinf(wy)*z;
  ny = y;
  nz = cosf(wy)*z + sinf(wy)*x;

  x = nx;
  y = ny;
  z = nz;

  /* Drehung um die Z-Achse : */
  nx = cosf(wz)*x + sinf(wz)*y;
  ny = cosf(wz)*y - sinf(wz)*x;
  nz = z;

  x = nx;
  y = ny;
  z = nz;
  
  min_ausdehnung = 1000000.0;
  for(i=0;i<g_geodat.nstoff;i++)
    {
      if(g_stoffgebiete[0].zeichne_on!=0)
	{
	  dx = g_stoffgebiete[i].max[0]-g_stoffgebiete[i].min[0];
	  dy = g_stoffgebiete[i].max[1]-g_stoffgebiete[i].min[1];
	  dz = g_stoffgebiete[i].max[2]-g_stoffgebiete[i].min[2];
	  ausdehnung = sqrt(dx*dx+dy*dy+dz*dz);
	}
      if(min_ausdehnung > ausdehnung) min_ausdehnung = ausdehnung;
    }
  
  eps = (min_ausdehnung/VERSCHIEBUNGS_QUOTIENT)/g_transform.scale;
  *xn = eps*x;
  *yn = eps*y;
  *zn = eps*z;

  return (0);
  /* ***************** ende build_zykl_oberflmat ************************** */
}

/* ********************************************************************* */
int verbiege_geo()
/* ********************************************************************* */
{
  /* Geometrie-Zeiger retten und auf Verzerrung setzen:*/
  org_knoten = g_knoten;
  g_knoten   = g_spannung.verzerrung;

  /* ***************** ende verbiege_geo   ******* *********************** */
  return (0);
}

/* ********************************************************************* */

int rueckbiege_geo()
/* ********************************************************************* */
{
  /* Zeiger wieder auf Originaldaten setzen */

  g_knoten = org_knoten;
  
  /* ***************** ende rueckbiege_geo   ***************************** */
  return (0);
}


/*****************************************************************/
void zeichne_geometrie()
/*****************************************************************/
{
  int    stoffnr;
  int    colormap_index;
  float  xn, yn, zn;
  float  xnz,ynz,znz;
  float  sgx,sgy,sgz;
  int    i;
  int    result;
  t_bool verbogen;
  t_bool g_draw_objects = TRUE;  
  t_bool bPolStipple = FALSE;
  t_bool texture_on;
  struct listelem *next_point;
  t_pickpunkt *pickpoint, *p_punkt;
  GLfloat raster_pos[4];
  GLint  viewport[4];
  GLuint *positions;
  GLubyte tcol[4];
  GLfloat edgeWidth = 1.0;                /* edge width */ 
  GLfloat edgeColor[] = { 1.0, 1.0, 1.0}; /* edge color */

  /*****************************************************************/
  /* loesche Grafikfenster und Z-Buffer -------------------------- */
  
  /* set texture */
  texture_on = g_color_extra.texture_on;

  if((!C3D_win.achtbit) && (g_tabelle.art==1))
    {
      colormap_index = STOFFARB_OFFSET+1;
      C3D_clearcolor();
    }
  else
    {
      colormap_index = ACHT_BIT_OFFSET+1;
      glClearIndex(FARBE_SCHWARZ_ACHT);
    }

  /* initialize the stipple pattern */
  if(Stip_available)
    init_graphics2(); 
  
  if(texture_on)
    {
      myinit(TEX_DEFAULT, FALSE);
      disableTex();
    }
  
 
  /* put the light on */  
  if (b_light_on && (!cutting_on))
    {
#ifdef DEBUG
      printf("Light on\n");
#endif
      //lighting();
      shadeon();
      setdiffuse(0.6,0.6,0.6);
      setspecular(0.,0.,0.);
    }
  
  //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  

  /* falls Eine Spannungsdarstellung: ------------------------------------ */
  /* Spannungs-Vektor lesen, Geometrie Verzerren und Verzerrung aktivieren:*/  
  
  verbogen = FALSE;
  
  if(g_spannung.on)
    {
      /* Falls Haupt-Modell verspannt gezeigt wird: */
      if(!(g_spannung.huelle_on&&(g_spannung.verspanne_solid_huelle==1)))
	{
	  verbiege_geo();
	  verbogen = TRUE;
	} 
    }     /* Ende Spannung an       */

  
  verschiebe_werte_netz(&xn,&yn,&zn);
  
  /* falls Geometrie abgesenkt werden soll bestimme Absenkung ------- */
  if (g_transform.absenken_on)
    bestimme_position();

  /* schalte backfaceremovel ein ------------------------------------ */
  if(!cutting_on) 
    {
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
    }

  for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
    {
      if (g_stoffgebiete[stoffnr-1].zeichne_on)
	{
	  switch (g_Maske[stoffnr])
	    {
	    case ZEICHNE_NICHT:
	      break;
	    case ZEICHNE_SOLID:
	      {
		glShadeModel(GL_FLAT);
		for(i = 0; i < 3; i++)
		  tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		glColor3ubv(tcol);
		if (!cutting_on)
		  glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		zeichne_stoff(stoffnr);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_stoff(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_stoff(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		break;
	      }
	    case ZEICHNE_GITTER:
	      {
		glShadeModel(GL_FLAT);
		for(i = 0; i < 3; i++)
		  {
		    tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		    /* draw with given intensity */
		    tcol[i] = (GLubyte) tcol[i] * g_tabelle.intensity;
		  }

		glColor3ubv(tcol);
		/*glIndexi(colormap_index + stoffnr);*/
		/* if rotation dont disable cull face*/
		if(g_transform.rotation_on)
		  {
		    glCullFace(GL_BACK);
		    glEnable(GL_CULL_FACE);
		  }
		else
		  glDisable(GL_CULL_FACE);
          
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		zeichne_gitterstoff(stoffnr,1);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_gitterstoff(stoffnr,1);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_gitterstoff(stoffnr,1);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
		      }
		  }
		if(g_transform.rotation_on)
		  glDisable(GL_CULL_FACE);
		break;
	      }
	    case ZEICHNE_TRANSPARENT:
	      {
		glShadeModel(GL_FLAT);
		for(i = 0; i < 3; i++)
		  tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		/* set transparecy */
		tcol[3] = (GLubyte)((g_color_extra.transparency*255.0 > 255) ? 255:
				    g_color_extra.transparency*255.0);

		if (g_color_extra.trans_color_on)
		  {
		    tcol[0] = g_color_extra.trans_color[0];
		    tcol[1] = g_color_extra.trans_color[1];
		    tcol[2] = g_color_extra.trans_color[2];
		  }

		glColor4ubv(tcol);
		/*glIndexi(colormap_index + stoffnr);*/
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(bPolStipple) 
		  {
		    glEnable(GL_POLYGON_STIPPLE);
		    switch(stoffnr % 4)
		      {
		      case 1 :
			glPolygonStipple(qtone1);
			break;
		      case 2 :
			glPolygonStipple(qtone2);
			break;
		      case 3 :
			glPolygonStipple(qtone3);
			break;
		      case 0 :
			glPolygonStipple(qtone4);
			break;
		      } 
		  } 
		else
		  {
		    setTransparent();  
		  }

		zeichne_stoff(stoffnr);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_stoff(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_stoff(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		if(bPolStipple)
		  glDisable(GL_POLYGON_STIPPLE);
		else
		  disableTransparent();
		break;
	      }
	    case ZEICHNE_NETZ:
	      {
		glShadeModel(GL_FLAT);
		if((!C3D_win.achtbit)&&(g_tabelle.art==1)){
		  for(i = 0; i < 3; i++)
		    tcol[i] = g_tabelle.colortable[4*FARBE_NETZ+i+1];
		  glColor3ubv(tcol);
		}
		else
		  glIndexi(FARBE_NETZ_ACHT); 
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);
		glTranslatef(xn,yn,zn);
		zeichne_gitterstoff(stoffnr,1);
		glTranslatef(-xn,-yn,-zn);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      build_zykl_oberflmat(i*g_transform.zyklus_ang,0.0,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      build_zykl_oberflmat(0.0,i*g_transform.zyklus_ang,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      build_zykl_oberflmat(0.0,0.0,i*g_transform.zyklus_ang,&xnz,&ynz,&znz);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			glTranslatef(xnz,ynz,znz);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-xnz,-ynz,-znz);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_gitterstoff(stoffnr,1);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
		      }
		  } 
		/*  glTranslatef(-xn, -yn, -zn);*/
		break;
	      }
	    case ZEICHNE_RESULT:
	      {
		glShadeModel(GL_SMOOTH);
		if (!cutting_on)
		  glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(texture_on)
		  {
		    zeichne_result_tex(stoffnr);
		    disableTex();
		  }
		else
		{	
		  zeichne_result(stoffnr);
		  /*disableLighting();*/
	    }	
	  
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt die gedrehte Loesung zeichnen: */
			if(texture_on)
			  {
			    zeichne_result_tex(stoffnr);
			    disableTex();
			  }
			else
			  zeichne_result(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			if(texture_on)
			  {
			    zeichne_result_tex(stoffnr);
			    disableTex();
			  }
			else
			  zeichne_result(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		break;
	      }
	    case ZEICHNE_SOLID_NETZ:
	      {
		glShadeModel(GL_FLAT);
		for(i = 0; i < 3; i++)
		  tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		glColor3ubv(tcol);
		/*glIndexi(colormap_index + stoffnr);*/
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		zeichne_stoff(stoffnr);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
 			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_stoff(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_stoff(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		/* Jetzt Netz zeichnen */
		if((!C3D_win.achtbit)&&(g_tabelle.art==1)){
		  for(i = 0; i < 3; i++)
		    tcol[i] = g_tabelle.colortable[4*FARBE_NETZ+i+1];
		  glColor3ubv(tcol);
		  /*glIndexi(FARBE_NETZ);*/
		}
		else
		  glIndexi(FARBE_NETZ_ACHT); 

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);
          
		/* Verschiebung um E : */
		glTranslatef(xn,yn,zn);
		zeichne_gitterstoff(stoffnr,1);
		/* Zurueck schieben um E */
		glTranslatef(-xn,-yn,-zn);
          
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      build_zykl_oberflmat(i*g_transform.zyklus_ang,0.0,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      build_zykl_oberflmat(0.0,i*g_transform.zyklus_ang,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      build_zykl_oberflmat(0.0,0.0,i*g_transform.zyklus_ang,&xnz,&ynz,&znz);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			glTranslatef(xnz,ynz,znz);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-xnz,-ynz,-znz);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			sgx = sgy = sgz = 1.0;
			/* Spiegeln: */
			if(g_transform.mirrorx) sgx = -1.0;
			if(g_transform.mirrory) sgy = -1.0;
			if(g_transform.mirrorz) sgz = -1.0;
			glScalef(sgx,sgy,sgz);
       
			/* Stoff zeichnen: */
			glTranslatef(sgx*xn,sgy*yn,sgz*zn);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-sgx*xn,-sgy*yn,-sgz*zn);
            
			/* Zuruecksetzen der Spiegelung */
			glScalef(sgx,sgy,sgz);
		      }
		  } 
		break;
	      }
	    case ZEICHNE_TRANSPARENT_NETZ:
	      {
		glShadeModel(GL_FLAT);
		for(i = 0; i < 3; i++)
		  tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		glColor3ubv(tcol);
		/*glIndexi(colormap_index + stoffnr);*/
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_STIPPLE);
		switch(stoffnr % 4)
		  {
		  case 1 :
                    glPolygonStipple(qtone1);
		    break;
		  case 2 :
                    glPolygonStipple(qtone2);
		    break;
		  case 3 :
                    glPolygonStipple(qtone3);
		    break;
		  case 0 :
                    glPolygonStipple(qtone4);
		    break;
		  }

		zeichne_stoff(stoffnr);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_stoff(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_stoff(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		glDisable(GL_POLYGON_STIPPLE);
		if((!C3D_win.achtbit)&&(g_tabelle.art==1)) {
		  for(i = 0; i < 3; i++)
		    tcol[i] = g_tabelle.colortable[4*FARBE_NETZ+i+1];
		  glColor3ubv(tcol);
		  /*glIndexi(FARBE_NETZ);*/
		}
		else
		  glIndexi(FARBE_NETZ_ACHT); 

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);
		glTranslatef(xn,yn,zn);
		zeichne_gitterstoff(stoffnr,1);
		glTranslatef(-xn,-yn,-zn);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      build_zykl_oberflmat(i*g_transform.zyklus_ang,0.0,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      build_zykl_oberflmat(0.0,i*g_transform.zyklus_ang,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      build_zykl_oberflmat(0.0,0.0,i*g_transform.zyklus_ang,&xnz,&ynz,&znz);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			glTranslatef(xnz,ynz,znz);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-xnz,-ynz,-znz);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			sgx = sgy = sgz = 1.0;
			/* Spiegeln: */
			if(g_transform.mirrorx) sgx = -1.0;
			if(g_transform.mirrory) sgy = -1.0;
			if(g_transform.mirrorz) sgz = -1.0;
			glScalef(sgx,sgy,sgz);
       
			/* Stoff zeichnen: */
			glTranslatef(sgx*xn,sgy*yn,sgz*zn);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-sgx*xn,-sgy*yn,-sgz*zn);
            
			/* Zuruecksetzen der Spiegelung */
			glScalef(sgx,sgy,sgz);
		      }
		  } 
		break;
	      }
	    case ZEICHNE_RESULT_NETZ:
	      {
		glShadeModel(GL_SMOOTH);
		if (!cutting_on)
		  glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(g_factor, g_bias);
		if(texture_on)
		  {
		    zeichne_result_tex(stoffnr);
		    disableTex();
		  }
		else
		  zeichne_result(stoffnr);
		glDisable(GL_POLYGON_OFFSET_FILL);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt die gedrehte Loesung zeichnen: */
			if(texture_on)
			  {
			    zeichne_result_tex(stoffnr);
			    disableTex();
			  }
			else
			  zeichne_result(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			glDisable(GL_CULL_FACE);
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			if(texture_on) 
			  {
			    zeichne_result_tex(stoffnr);
			    disableTex();
			  }
			else
			  zeichne_result(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			glEnable(GL_CULL_FACE);
		      }
		  } 
		if((!C3D_win.achtbit)&&(g_tabelle.art==1)){
		  for(i = 0; i < 3; i++)
		    tcol[i] = g_tabelle.colortable[4*FARBE_NETZ+i+1];
		  glColor3ubv(tcol);
		  /*glIndexi(FARBE_NETZ);*/
		}
		else
		  glIndexi(FARBE_NETZ_ACHT); 
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(edgeWidth);
		glColor3fv(edgeColor);
		zeichne_gitterstoff(stoffnr,1);
		glEnable(GL_LIGHTING);
		
	
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      build_zykl_oberflmat(i*g_transform.zyklus_ang,0.0,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      build_zykl_oberflmat(0.0,i*g_transform.zyklus_ang,0.0,&xnz,&ynz,&znz);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      build_zykl_oberflmat(0.0,0.0,i*g_transform.zyklus_ang,&xnz,&ynz,&znz);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			glTranslatef(xnz,ynz,znz);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-xnz,-ynz,-znz);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			sgx = sgy = sgz = 1.0;
			/* Spiegeln: */
			if(g_transform.mirrorx) sgx = -1.0;
			if(g_transform.mirrory) sgy = -1.0;
			if(g_transform.mirrorz) sgz = -1.0;
			glScalef(sgx,sgy,sgz);
       
			/* Stoff zeichnen: */
			glTranslatef(sgx*xn,sgy*yn,sgz*zn);
			zeichne_gitterstoff(stoffnr,1);
			glTranslatef(-sgx*xn,-sgy*yn,-sgz*zn);
            
			/* Zuruecksetzen der Spiegelung */
			glScalef(sgx,sgy,sgz);
		      }
		  } 
		break;
	      }
	    case ZEICHNE_RAHMEN:
	      {
		for(i = 0; i < 3; i++)
		  tcol[i] = g_tabelle.colortable[4*(colormap_index + stoffnr)+i+1];
		glColor3ubv(tcol);
		/*glIndexi(colormap_index + stoffnr);*/
		zeichne_rahmen(stoffnr);
		if(g_transform.zyklus_on)
		  {
		    for(i=1;i<=g_transform.zyklus_anz;i++)
		      {
			/* um einen Winkelschritt um die entspr. Achse drehen: */
			switch(g_transform.zyklus_achse)
			  {
			  case 'x':
			    {
			      glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
			      break;
			    }
			  case 'y':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
			      break;
			    }
			  case 'z':
			    {
			      glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
			      break;
			    }
			  }
			/* Jetzt gedrehten Stoff zeichnen: */
			zeichne_rahmen(stoffnr);
		      }
		    /* Zuruck drehen: */
		    switch(g_transform.zyklus_achse)
		      {
		      case 'x':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			  break;
			}
		      case 'y':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			  break;
			}
		      case 'z':
			{
			  glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			  break;
			}
		      }            
		  }
		else
		  {
		    if(g_transform.spiegel_on)
		      {
			/* Spiegeln: */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			/* Stoff zeichnen: */
			zeichne_rahmen(stoffnr);
            
			/* Zuruecksetzen der Spiegelung */
			if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
		      }
		  }
		break;
	      }
	    }
	}
    } /* Ender der Geometrie-Zeichnung */
  /*if (b_light_on)
  	disableLighting();*/
  
  glTranslatef(-xn, -yn, -zn);


  /* falls Spannungsdarstellung: */  
  if(g_spannung.on)
    {
      /* Falls Haupt-Modell verspannt gezeigt wurde: */
      if(verbogen)
	{
	  /* Koordinaten wieder herstellen: */
	  rueckbiege_geo();
	}
      verbogen = FALSE;
      /* Falls noch die Huelle gezeichnet wird : */   
      if(g_spannung.huelle_on)
	{
	  /* Diese ggf. verzerren */
	  if((g_spannung.verspanne_solid_huelle==1))
	    {
	      verbiege_geo();
	      verbogen = TRUE;
	    }
      
	  /* Zeichne Huelle */
	  /* Hier im Prinzip Gitterstoff mit Huellfarbe zeichnen */
	  /* ggf auch wie Netz verschieben */
      
	  for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
	    {
	      if (g_stoffgebiete[stoffnr-1].zeichne_on)
		{
		  /*glIndexi(g_spannung.huellfarbe);*/
		  for(i = 0; i < 3; i++)
		    tcol[i] = g_tabelle.colortable[4*FARBE_HUELLE+i+ 1];
		  glColor3ubv(tcol);
		  glDisable(GL_CULL_FACE);
		  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		  zeichne_gitterstoff(stoffnr,0);
		  if(g_transform.zyklus_on)
		    {
		      for(i=1;i<=g_transform.zyklus_anz;i++)
			{
			  /* um einen Winkelschritt um die entspr. Achse drehen: */
			  switch(g_transform.zyklus_achse)
			    {
			    case 'x':
			      {
				glRotatef(g_transform.zyklus_ang,1.0,0.0,0.0);
				break;
			      }
			    case 'y':
			      {
				glRotatef(g_transform.zyklus_ang,0.0,1.0,0.0);
				break;
			      }
			    case 'z':
			      {
				glRotatef(g_transform.zyklus_ang,0.0,0.0,1.0);
				break;
			      }
			    }
			  /* Jetzt gedrehten Stoff zeichnen: */
			  zeichne_gitterstoff(stoffnr,0);
			}
		      /* Zuruck drehen: */
		      switch(g_transform.zyklus_achse)
			{
			case 'x':
			  {
			    glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,-1.0,0.0,0.0);
			    break;
			  }
			case 'y':
			  {
			    glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,-1.0,0.0);
			    break;
			  }
			case 'z':
			  {
			    glRotatef(g_transform.zyklus_anz*g_transform.zyklus_ang,0.0,0.0,-1.0);
			    break;
			  }
			}            
		    }
		  else
		    {
		      if(g_transform.spiegel_on)
			{
			  /* Spiegeln: */
			  if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			  if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			  if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
            
			  /* Stoff zeichnen: */
			  zeichne_gitterstoff(stoffnr,0);
            
			  /* Zuruecksetzen der Spiegelung */
			  if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
			  if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
			  if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
			}
		    } 
		}
	    } /* ENDE HUELLE ZEICHNEN */
      
      
	  /* Diese ggf. wieder entzerren */
	  if(verbogen)
	    {
	      rueckbiege_geo();
	    }
	} /* Ende der Huellen-Darstellung */
    } /* Ende if Spannung */
  
  if ( g_draw_objects )
  {
	C3D_drawobjects();
  } 
  
  /* ggf. Stroemung zeichnen: */
  if(g_stroemung.on)
    {   
      /*g_stroemung.art = 2;*/
      zeichne_stroemung();
    }

  /* gepickte Punkte markieren: */
  if(g_picking.anzahl>0)
    {
#ifdef DEBUG
      printf("\n%d Gepickte Punkte: ",g_picking.anzahl);
#endif
      positions = (GLuint*)malloc(sizeof(GLuint)*2*g_picking.anzahl);
      if(positions==NULL) fprintf(stderr,"\nKein Speicher fuer 2D-Punktlist!");
      else
	{
	  next_point = g_picking.punkte->head;
	  for(i=0;i<g_picking.anzahl;i++)
	    {
	      pickpoint = (t_pickpunkt*) next_point->data;
	      /*printf("%d ",next_point->punkt_nr);*/
	      glRasterPos3fv(pickpoint->koord);
	      glGetFloatv(GL_CURRENT_RASTER_POSITION,raster_pos);
	      positions[2*i  ] = (GLuint)raster_pos[0]-4;
	      positions[2*i+1] = (GLuint)raster_pos[1]-4;
	      next_point = next_point->next;
	    }
	  glGetIntegerv(GL_VIEWPORT,viewport);
	  glMatrixMode(GL_PROJECTION);
	  glPushMatrix();
	  glLoadIdentity();
	  glOrtho(0,viewport[2],0,viewport[3],0.0,1.0);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glLoadIdentity();
	  /*glClearDepth(0.0);*/
	  glClear(GL_DEPTH_BUFFER_BIT);
		
	  if(C3D_win.achtbit)
	    {
	      glIndexi(13);
	    }
	  else
	    {
	    }
 
	  next_point = g_picking.punkte->head;	
	  for(i = 0; i < g_picking.anzahl; i++)
	    {
	      GLubyte r,g,b,a;
	      pickpoint = (t_pickpunkt*) next_point->data;
	      r = (GLubyte) UNPACK_RED(pickpoint->rgba);
	      g = (GLubyte) UNPACK_GREEN(pickpoint->rgba);
	      b = (GLubyte) UNPACK_BLUE(pickpoint->rgba);
	     
	      glColor3ub(r, g, b); 
	      glRasterPos2i(positions[2*i  ],positions[2*i+1]);
	      glBitmap(8,8,0.0,0.0,0.0,0.0,pointmap);
	      next_point = next_point->next;
	    }
			
	  glMatrixMode(GL_MODELVIEW);
	  glPopMatrix();
	  glMatrixMode(GL_PROJECTION);
	  glPopMatrix(); 
	  free(positions);
	} /* End Else */
    } /* End if g_picking.anzahl>0 */

  return;
} 


/* --------------- Ende zeichne_geometrie() ---------------------------- */

/* ********************************************************************* */
build_oberflmat(float *xn, float *yn, float *zn)
/* ********************************************************************* */
{
  float um;
  float wx, wy, wz,
    x,  y,  z,
    nx, ny, nz;
  /* ********************************************************************* */
  um = PI / 180.0;

  /* Rückdreh-Winkel im Bogenmaß: (für Sin/Cos) */
  wx = (um * (g_transform.rotx+90.0));
  wy = (um * g_transform.roty);
  wz = (um * g_transform.rotz);

  /* Drehung um die X-Achse : */
  ny = -cosf(wx);
  nz = sinf(wx);

  x = 0.0;
  y = ny;
  z = nz;

  /* Drehung um die Y-Achse : */
  nx = cosf(wy)*x-sinf(wy)*z;
  ny = y;
  nz = cosf(wy)*z + sinf(wy)*x;

  x = nx;
  y = ny;
  z = nz;

  /* Drehung um die Z-Achse : */
  nx = cosf(wz)*x + sinf(wz)*y;
  ny = cosf(wz)*y - sinf(wz)*x;
  nz = z;

  x = nx;
  y = ny;
  z = nz;

  *xn = x;
  *yn = y;
  *zn = z;
  /* ********************* ende build_oberflmat ************************** */
  return;
}

/* ********************************************************************* */
verschiebe_werte_netz(float *xn, float *yn, float *zn)
/* ********************************************************************* */ 
{
  float eps;                      /* Faktor fuer Netzverschiebung -- */
  float ausdehnung;
  float max_ausdehnung;
  float dx,dy,dz;
  int i;
  
  /* ********************************************************************* */
  build_oberflmat(xn,yn,zn);   /* Matrix fuer Oberflaechennetz -- */
  
  /* Alle Stoffe abklappern fuer verschiebungsfaktor */
  max_ausdehnung = 0.0;
  for(i=0;i<g_geodat.nstoff;i++)
    {
      dx = g_stoffgebiete[i].max[0]-g_stoffgebiete[i].min[0];
      dy = g_stoffgebiete[i].max[1]-g_stoffgebiete[i].min[1];
      dz = g_stoffgebiete[i].max[2]-g_stoffgebiete[i].min[2];
      ausdehnung = sqrt(dx*dx+dy*dy+dz*dz);
      if(ausdehnung>max_ausdehnung) max_ausdehnung = ausdehnung;
    }
  eps = (max_ausdehnung/VERSCHIEBUNGS_QUOTIENT)/g_transform.scale;
  *xn = eps * *xn;
  *yn = eps * *yn;
  *zn = eps * *zn;
  /* ********************* ende verschiebe_werte_netz ******************** */
  return;  
}

/* This function initialize stipple for 
 * screendoor transparency. It is called
 * only once.
 */
void init_graphics2( void )
{
  int i;

  Stip_available = 1;
   

  /* Setup stipples for screendoor transparency */
  /* stipple[0] = 25% opaque */
  for (i=0;i<32;i+=2) {
    stipple[0][i+0] = 0x88888888;
    stipple[0][i+1] = 0x22222222;
  }
  /* stipple[1] = 50% opaque */
  for (i=0;i<32;i+=2) {
    stipple[1][i+0] = 0xaaaaaaaa;
    stipple[1][i+1] = 0x55555555;
  }
  /* stipple[2] = 75% opaque */
  for (i=0;i<32;i+=2) {
    stipple[2][i+0] = 0x77777777;
    stipple[2][i+1] = 0xdddddddd;
  }
}
/*********************************************************
 setTrasparent - this function sets the transparency
 for polygons.
**********************************************************/
static void setTransparent()
{
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  /* set blending func */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}
/***********************************************************
 disableTransparent - this function disable the transparency
 for polygons.
************************************************************/
static void disableTransparent()
{
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

int C3D_drawobjects()
{

	if ( re.C3D_updateiso )
		re.C3D_updateiso(g_isosurfaces.isosurfaces, TRUE, FALSE);
	if ( re.C3D_updateiso )
		re.C3D_updateiso(g_isovolumes.isovolumes, FALSE, TRUE );
	if ( re.C3D_updatecut )
		re.C3D_updatecut(g_isocuts.cuts, FALSE);
	return 0;
}
