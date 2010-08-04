/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor / Stefan Kaesberg                     */
/* Datum    :   16.02.98   / 05.98                               */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname            | Funktion                            */
/* ------------------------------------------------------------- */
/* void SetColorMap2000()  | Belegt Default 24-Bit C-Map         */
/* void SetColorMap256()   | Belegt Default  8-Bit C-Map         */
/* void SetColorMapRaster()| Belegt C-Map mit 12 Rasterstufen    */
/* void SetColorMapRGrau() | Belegt C-Map mit Grauwertverlauf    */
/* void C3D_updateisolines()| Aktualisiert Iso-Linien in C-Map   */
/* mapcolor                | Hilfsprozedur                       */
/* make_stoff_farben       | Hilfsprozedur                       */
/* maptcolor               | Hilfsprozedur                       */
/* int save_colortable(int,char*) | schreibt C-Map in File       */
/* int read_colortable(int,char*) | liest C-Map aus File         */
/*                                                               */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
/*                                                               */
/* ************************************************************* */
#include <assert.h>
#include "grafik.h"
#include "color3d.h"
#include "iso.h"
#include "utilh.h"
#include "genlist.h"
#include "memory.h"
#include "../graphics/gl_local.h"

#define RED2UBYTE(r)  ((GLubyte)  ((r) * 255.0 > 255.0) ? 255 : (r) * 255.0 )
#define BLUE2UBYTE(b)  ((GLubyte)  ((b) * 255.0 > 255.0) ? 255 : (b) * 255.0 )
#define GREEN2UBYTE(g)  ((GLubyte)  ((g) * 255.0 > 255.0) ? 255 : (g) * 255.0 )

/*global variables*/
Tri *g_listp[MAX_ISO];

/* error code */
enum {
	ECURRENT = -2
};

/* functions */
static void makeiso_tex(float, float, int, int);
int makeTexImageSub(int subtexsize, t_linie *iso, int current);
int makeClipTexImage(int subtexsize, int above, int bellow);
static void resetIso();
static void reset_subtex();
extern int  cmp_int(const void *, const void *);

/* ===================================================================== */
void mapcolor(int index, short red, short green, short blue) /* Hilfe -- */
{
	XColor ColorX;
	FILE *fout;
	static flag=0;
/* --------------------------------------------------------------------- */
	g_tabelle.colortable[4*g_tabelle.anzahl_farben+0]=index;
	g_tabelle.colortable[4*g_tabelle.anzahl_farben+1]=red;
	g_tabelle.colortable[4*g_tabelle.anzahl_farben+2]=green;
	g_tabelle.colortable[4*g_tabelle.anzahl_farben+3]=blue;
	g_tabelle.anzahl_farben++;

	/* if overlay is active */
	if ( C3D_win.overlay && C3D_win.overlay->vi )
	{
		ColorX.pixel = index;
		ColorX.red   = red * 256;
		ColorX.green = green * 256;
		ColorX.blue  = blue * 256;
		ColorX.flags = DoRed | DoGreen | DoBlue;
		XStoreColor( XtDisplay( C3D_win.overlay->wid ),
		 C3D_win.overlay->cmap, &ColorX );
	}
	return;
} 

/* ===================================================================== */
void C3D_updateisolines() /* Traegt Isolinien in C-Map ein ---------------- */
{
	int anzahl;
	float start;
	int i,j;
	XColor Farbe;
	float faktor;
	int n;
	int stoffnr;
	int col;
	t_texture_info *t;
	struct listelem *isoline;
	t_linie *iso;


	static int b_btrilist = 1;
	/* --------------------------------------------------------------------- */
	assert(XtDisplay(C3D_win.wid) == dpy);
	assert(C3D_win.cmap == cmap);

	/* initalize the list */
	if(b_btrilist)
	{
		b_btrilist = 0;
		for(i = 0; i < MAX_ISO; i++)
			g_listp[i] = NULL;
	}

	/* Gibt es ueberhaupt ein Temperaturintervall ? ------------------------ */
	if((g_loesung.Max-g_loesung.Min)>0.0)
	{

		/*build list of triangles*/

		/*g_listp = btrilist(g_listp);*/
		b_btrilist = 1;

		/* Welchen Farbmodus haben wir ? --------------------------------------- */
		if((!C3D_win.achtbit)&&(g_tabelle.art==1))
		{
			faktor = ((float)COLORTABLE_LENGTH/(g_loesung.Max-g_loesung.Min));
		}
		else
		{
			faktor = ((float)ACHT_BIT_COLORTABLE_LENGTH/(g_loesung.Max-g_loesung.Min));
		}

		/* reset all isolines */
		t = &g_tex_info;
		//makeTexImageDef(t->tex_size[0], t->ntex);
		reset_subtex();

		/* Nacheinander die Isolinien einzeichnen: ----------------------------- */
		if (g_isolinien.linien == NULL) return;
		for (isoline = g_isolinien.linien->head; isoline != NULL; isoline = isoline->next)
		{
			iso = (t_linie *) isoline->data;
			if(iso->on)
			{
				anzahl = (int)(iso->breit);
				start  = (iso->wert-g_loesung.Min)- anzahl/2;

				/* put isoline on the texture map */
				makeiso_tex(start, anzahl, iso->nr, t->tex_size[0]);
			}
		}
	}
}

/*************************************************************************
This function change the texture map  so it can display the isolines.

Parameters:
-float vtemp  - value of the isoline (color index in texture map).
-float anzahl - width of isoline (the number of colors to change).
-int isonr - isoline number.
-int tex_size - max texture size

*************************************************************************/
static void makeiso_tex(float vtemp , float anzahl, int isonr, 
		int tex_size)
{
	int i, j, k;
	int cindex, cindex_end;
	int tcol = 1;
	int isowidth;
	t_texture_info *t;
	t_subtex       *st;
	t_linie *isoline;
	void *p;
	struct listelem *litem;
	int subtexsize;

	t  = &g_tex_info;


	/* clamp the starting value to 0 - texture size */
	cindex = (int) vtemp * tex_size / (g_loesung.Max - g_loesung.Min);
	cindex_end = (int) (vtemp + anzahl) * tex_size / (g_loesung.Max - g_loesung.Min);

	/* find id of isoline */
	p = glookup(g_isolinien.linien, (void *)&isonr, cmp_int);
	assert(p != NULL);
	litem = (struct listelem *) p;
	isoline = (t_linie *) litem->data;

	if (gl_config.renderer & ( GL_RENDERER_O2 | GL_RENDERER_IMPACT) )
	{  
		/* put isoline in the texture map */
		if(tcol == HOT_COLD)
		{
			k = 0;

			for(i = 0; i < tex_size; i++)
			{
				if(cindex <= i  && i <= cindex_end)
				{
					k++;
					t->texImage[0][4*(cindex + k)] = (GLubyte)
						((isoline->r * 255.0 > 255) ? 
						 255 : isoline->r * 255.0);
					t->texImage[0][4*(cindex + k) + 1] = 
						(GLubyte)((isoline->g * 255.0 > 255) ? 
						255 : isoline->g * 255.0);
					t->texImage[0][4*(cindex + k) + 2] = 
						(GLubyte)((isoline->b * 255.0 > 255) ? 
						255 : isoline->b * 255.0);
					t->texImage[0][4*(cindex + k) + 3] = (GLubyte) 255;

				}

			}
		}

		glBindTexture(GL_TEXTURE_1D, t->texName[0]);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage1D(GL_TEXTURE_1D,0,4, tex_size, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, &t->texImage[0]);
	}
	else
	{

		if (g_color_extra.texture_on)
		{
			subtexsize = cindex_end - cindex;
			if (makeTexImageSub(subtexsize, isoline, isonr-1)) 
			{
				printf("Error: not valid number of isoline\n");
			}

			st = g_tex_info.subtex;

			/* bind it to original texture */
			glBindTexture(GL_TEXTURE_1D, t->texName[0]);
			cglTexSubImage1DEXT(GL_TEXTURE_1D, 0, cindex,
					st->subtex_size[isonr-1], GL_RGBA,
					GL_UNSIGNED_BYTE, st->subTexImage[isonr-1]);

		}
	}  
}

static int maxsubtex = 0; /* maximal number of sub textures */

/*************************************************************
makeTexImageSub - sub texture image routine creates 
isolines as subtextures to main texture.

Parameters:
==========
int subtexsize - size of texture
t_linie *iso   - isoline 
int current    - current isoline
Returns:
========
returns 0 if succeced. 
*************************************************************/
int makeTexImageSub(int subtexsize, t_linie *iso, int current)
{
	t_subtex *st = NULL;
	GLubyte  **newf;
	GLuint   *newfs;
	GLubyte  *subTexImage;
	int i = 0;
	t_texture_info *t = &g_tex_info;

	if (t->subtex == NULL)
	{
		NEW(t->subtex);
		st = t->subtex;
		st->subTexImage = NULL;
		st->subtex_size = NULL;
		st->nsubtex     = 0;
	}

	if (t->subtex->subTexImage == NULL)          /* allocate on first call */
	{
		st = t->subtex;
		st->nsubtex = maxsubtex = 1;
		st->subtex_size = (GLuint *) ALLOC(sizeof(GLuint) * maxsubtex);
		st->subTexImage = (GLubyte **) ALLOC(sizeof(GLubyte*) * maxsubtex);
		if (st->subtex_size == NULL || st->subTexImage == NULL)
			reset_subtex();
	}


	if (t->subtex->nsubtex >= maxsubtex)         /* when we need more sub textures */
	{
		maxsubtex *= 2;
		newf = (GLubyte **) RESIZE(t->subtex->subTexImage, 
				sizeof(t->subtex->subTexImage[0]) * maxsubtex);

		newfs = (GLuint *) RESIZE(t->subtex->subtex_size, 
				sizeof(GLuint) * maxsubtex);
		t->subtex->subTexImage = newf;
		t->subtex->subtex_size = newfs;
	}

	if (current > t->subtex->nsubtex)
		return ECURRENT;


	subTexImage = (GLubyte *) ALLOC(sizeof(subTexImage[0]) * 4 * subtexsize);


	/* create sub texture image (isoline) */
	for (i = 0; i < subtexsize; i++)
	{
		subTexImage[4*i  ] = RED2UBYTE(iso->r);
		subTexImage[4*i+1] = GREEN2UBYTE(iso->g);
		subTexImage[4*i+2] = BLUE2UBYTE(iso->b); 
		subTexImage[4*i+3] = (GLubyte)255;
	}
	t->subtex->subTexImage[current] = subTexImage;
	t->subtex->nsubtex++;                         /* add new sub texture */
	t->subtex->subtex_size[current] = subtexsize; /* save the size of subtexture */
	return 0;
}

static void reset_subtex()
{
	if ( gl_config.renderer & (GL_RENDERER_O2 | GL_RENDERER_IMPACT) )
	{ 
		resetIso(); 
	}
	else
	{
		t_texture_info *t  = &g_tex_info;
		glBindTexture( GL_TEXTURE_1D, t->texName[0] );
		glTexImage1D( GL_TEXTURE_1D,0,4, t->tex_size[0], 0,
				GL_RGBA, GL_UNSIGNED_BYTE, &t->texImage[0] );
	}
}
  
/******************************************************************
  This function deletes the isolines from color-table.
******************************************************************/
static void resetIso()
{
	t_texture_info *t;

	/* get the texture map */
	t = &g_tex_info;
	assert (t != NULL);
	/* return texture map to default color-table */ 
	makeTexImageDef(t->tex_size[0], t->ntex);
	glBindTexture(GL_TEXTURE_1D, t->texName[0]);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D,0,4, t->tex_size[0], 0,
			GL_RGBA, GL_UNSIGNED_BYTE, &t->texImage[0]);
}

/*******************************************************************
  makeClipTexImage - this function makes part of the texture
  transparent by setting alpha value of the RGBA textel.
  
*******************************************************************/
int makeClipTexImage(int order, int above, int bellow)
{
	GLuint      subtexsize;   /* subtexture size  */
	t_texture_info *t;        /* tex. information */
	GLubyte *subTexImage;     /* subtexture image */
	int i;                    /* counter          */
	int start;                /* start of the tex.*/
	t = &g_tex_info;

	if (g_color_extra.texture_on)
	{
		if(order)
		{
			subtexsize = t->tex_size[0] - above;
			start      = above;
		}
		else
		{
			subtexsize = bellow;
			start      = 0;
		}

		subTexImage = (GLubyte *) ALLOC(sizeof(subTexImage[0]) * 4 * subtexsize);

		/* create texture with alpha value set to 0,
		 * we used it after to make texture transparent, by 
		 * using alpha test. I use white color for cliping (it not important anyhow).
		 */
		for (i = 0; i < subtexsize; i++)
		{
			subTexImage[4*i  ] = (GLubyte)255;
			subTexImage[4*i+1] = (GLubyte)255;
			subTexImage[4*i+2] = (GLubyte)255; 
			subTexImage[4*i+3] = (GLubyte)0;
		}

		/* bind it to original texture */
		glBindTexture(GL_TEXTURE_1D, t->texName[0]);
		glTexSubImage1D(GL_TEXTURE_1D, 0, start,
				subtexsize, GL_RGBA,
				GL_UNSIGNED_BYTE, subTexImage);
		FREE(subTexImage);
	}
}

/******************************************************************
  Purpose:
  ========
     This function clips results bellow and above certain value.
*******************************************************************/
void C3D_clip_tex(float above, float bellow)
{
	int ab, be;
	int tex_size = g_tex_info.tex_size[0];

	/* reset the old clip texture */
	C3D_clip_tex_reset();
	if(g_loesung.Min < bellow && bellow < g_loesung.Max) 
	{ 
		be = (int) ((bellow - g_loesung.Min) * tex_size / (g_loesung.Max - g_loesung.Min));
		makeClipTexImage(0, 0, be);
	}
	if(g_loesung.Min < above  && above  < g_loesung.Max)
	{
		ab = (int) ((above - g_loesung.Min) * tex_size / (g_loesung.Max - g_loesung.Min));
		makeClipTexImage(1, ab, 0);
	}
	glAlphaFunc( GL_GREATER, 0.1f );
	glEnable( GL_ALPHA_TEST );
}

void C3D_clip_tex_reset()
{
	resetIso();
	glDisable( GL_ALPHA_TEST );
}

/* ===================================================================== */
void SetColorMap2000() /* Belegt Default 24-Bit C-Map ------------------ */

{
	int          i, istart, iende;
	short        r, g, b;
	/* --------------------------------------------------------------------- */  
	/* Falls Colormap existiert, freigeben: -------------------------------- */
	if(g_tabelle.colortable!=NULL) free(g_tabelle.colortable);

	/* Information in Struktur speichern: ---------------------------------- */
	g_tabelle.art = 1;

	/* Dann Speicher fuer Farbpalette reservieren: 2-fach wg. Datei -------- */
	g_tabelle.colortable = (int*)malloc(8*MAX_COLORS*sizeof(int));
	if(g_tabelle.colortable==NULL)
	{
		printf("\nFehler: Kein Speicher fuer Colortable!");
	}
	g_tabelle.anzahl_farben = 0;

	/* set default intensity */
	g_tabelle.intensity = 1.0f;

	/* Belegung der Colormap 1024 - 3024 fuer die Darstellung der Temperatur */
	istart = 1024;
	iende  = 3024; 
	r = g = 0;
	b = 255;

	for (i = 1; i < 225; i++)
	{
		mapcolor(istart++, r++, g, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b--);
		mapcolor(istart++, r, g, b);
	}

	for (i = 1; i < 32; i++)
	{
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r++, g, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b--);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);

		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
	}

	for (i = 0; i < 100; i++)
	{
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g++, b);
		mapcolor(istart++, r, g, b);
		mapcolor(istart++, r, g, b);
	}

	for (i = 0; i < 100; i++)
	{
		b++;
		mapcolor(istart++, r, g, b++);
	}

	/* Belegung der Colormap 768 - 795 fuer die Darstellung der Stoffe ----- */
	istart = 768;
	mapcolor(istart++, (short)   0, (short)   0, (short)   0); /* schwarz  */
	mapcolor(istart++, (short) 255, (short) 255, (short) 255); /* weiss -- */
	mapcolor(istart++, (short) 255, (short) 0, (short) 0);     /* rot ---- */
	mapcolor(istart++, (short) 0, (short) 255, (short) 0);     /* gruen -- */
	mapcolor(istart++, (short) 0, (short) 0, (short) 255);     /* blau --- */
	mapcolor(istart++, (short) 255, (short) 255, (short) 0);   /* gelb --- */ 
	mapcolor(istart++, (short) 255, (short) 0, (short) 255);   /* pink --- */
	mapcolor(istart++, (short) 0, (short) 255, (short) 255);   /* cyan --- */
	mapcolor(istart++, (short) 240, (short) 195, (short) 240); /* hellrosa */
	mapcolor(istart++, (short) 100, (short) 190, (short) 150); /* hellblau */
	mapcolor(istart++, (short) 100, (short) 175, (short) 255); /* hellgruen*/
	mapcolor(istart++, (short) 150, (short) 125, (short) 0);
	mapcolor(istart++, (short) 200, (short) 225, (short) 0);
	mapcolor(istart++, (short) 0, (short) 150, (short) 50);
	mapcolor(istart++, (short) 200, (short) 200, (short) 200);
	mapcolor(istart++, (short) 255, (short) 0, (short) 100);  
	mapcolor(istart++, (short) 100, (short) 100, (short) 255);
	mapcolor(istart++, (short) 200, (short) 200, (short) 0);
	mapcolor(istart++, (short) 0, (short) 100, (short) 0);
	mapcolor(istart++, (short) 0, (short) 0, (short) 100);
	mapcolor(istart++, (short) 100, (short) 0, (short) 0);
	mapcolor(istart++, (short) 0, (short) 200, (short) 0);
	mapcolor(istart++, (short) 0, (short) 0, (short) 200);
	mapcolor(istart++, (short) 200, (short) 0, (short) 0);
	mapcolor(istart++, (short) 200, (short) 200, (short) 200);
	mapcolor(istart++, (short) 100, (short) 100, (short) 100);
	mapcolor(istart, (short) 0, (short) 200, (short) 200);
	// this colors are taken from rgb.txt
	mapcolor(istart, (short) 135, (short) 206, (short) 235);
	mapcolor(istart, (short) 135, (short) 206, (short) 250);
	mapcolor(istart, (short)  70, (short) 130, (short) 180);
	mapcolor(istart, (short) 176, (short) 196, (short) 222);
	mapcolor(istart, (short) 173, (short) 216, (short) 230);
	mapcolor(istart, (short) 176, (short) 224, (short) 230);
	mapcolor(istart, (short) 175, (short) 238, (short) 238);
	mapcolor(istart, (short)   0, (short) 206, (short) 209);
	mapcolor(istart, (short)  64, (short) 224, (short) 208);
	mapcolor(istart, (short) 224, (short) 255, (short) 255);
	mapcolor(istart, (short)  95, (short) 158, (short) 160);
	mapcolor(istart, (short) 102, (short) 205, (short) 170);
	mapcolor(istart, (short) 127, (short) 255, (short) 212);
	mapcolor(istart, (short)   0, (short) 100, (short)   0);
	mapcolor(istart, (short)  85, (short) 107, (short)  47);
	mapcolor(istart, (short) 143, (short) 188, (short) 143);
	mapcolor(istart, (short)  46, (short) 139, (short)  87);
	mapcolor(istart, (short)  60, (short) 179, (short) 113);
	mapcolor(istart, (short)  32, (short) 178, (short) 170);
	mapcolor(istart, (short) 152, (short) 251, (short) 152);
	mapcolor(istart, (short)   0, (short) 255, (short) 127);
	mapcolor(istart, (short) 127, (short) 255, (short)   0);
	mapcolor(istart, (short)   0, (short) 250, (short) 154);
	mapcolor(istart, (short) 173, (short) 255, (short)  47);
	mapcolor(istart, (short)  50, (short) 205, (short)  50);
	mapcolor(istart, (short)  34, (short) 139, (short)  34);
	mapcolor(istart, (short) 107, (short) 142, (short)  35);
	mapcolor(istart, (short) 189, (short) 183, (short) 107);
	mapcolor(istart, (short) 240, (short) 230, (short) 140);
	mapcolor(istart, (short) 238, (short) 232, (short) 170);
	mapcolor(istart, (short) 250, (short) 250, (short) 210);
	mapcolor(istart, (short) 255, (short) 255, (short) 224);
	mapcolor(istart, (short) 255, (short) 255, (short) 224);
	mapcolor(istart, (short) 255, (short) 215, (short)   0);
	mapcolor(istart, (short) 238, (short) 221, (short) 138);
	mapcolor(istart, (short) 218, (short) 165, (short)  32);
	mapcolor(istart, (short) 184, (short) 134, (short)  11);
        	
	/* Belegung der Stroemungs Farben: ----------------------------------- */
	istart = ROLL_OFFSET;
	r = 0;
	g = 25;
	b = 25;
	for(i=0;i<ROLL_LENGTH;i++)
	{
		mapcolor(istart++,(short)r,(short)g,(short)b);
		g = g + 50;
		b = b + 60;
	}
	/* Schaftfarbe */
	r = 250;
	g = 200;
	b = 0;
	mapcolor(FARBE_SCHAFT,(short)r,(short)g,(short)b);
	/* Spitzenfarbe: */
	r = 255;
	g = 0;
	b = 0;
	mapcolor(FARBE_SPITZE,(short)r,(short)g,(short)b);

	g_tabelle.start_index_datei = g_tabelle.anzahl_farben;
	g_tabelle.end_index_datei   = g_tabelle.anzahl_farben;
	g_tabelle.anz_bitplanes = 24;

}


void make_achtbit_stroemung() /* Hilfsprozedur ------------------------- */
{
	int istart;
	int i,r,g,b;
	/* Belegung der Stroemungs Farben: ----------------------------------- */
	istart = ACHT_BIT_ROLL_OFFSET;
	r = 0;
	g = 25;
	b = 50;
	for(i=0;i<ROLL_LENGTH;i++)
	{
		mapcolor(istart++,(short)r,(short)g,(short)b);
		g = g + 20;
		b = b + 25;
	}
	/* Schaftfarbe */
	r = 250;
	g = 200;
	b = 0;
	mapcolor(FARBE_SCHAFT_ACHT,(short)r,(short)g,(short)b);
	/* Spitzenfarbe: */
	r = 255;
	g = 0;
	b = 0;
	mapcolor(FARBE_SPITZE_ACHT,(short)r,(short)g,(short)b);

}
  
    

/* ===================================================================== */
void make_stoff_farben() /* Hilfsprozedur ------------------------------ */
{
	int istart;
	/* --------------------------------------------------------------------- */
	/* Belegung der Colormap  mit den Stoffarben --------------------------- */
	istart = ACHT_BIT_OFFSET;
	mapcolor(istart++, (short)   0, (short)   0, (short)   0);  /* Schwarz */
	mapcolor(istart++, (short) 255, (short) 255, (short) 255);  /* Weiss   */
	mapcolor(istart++, (short) 255, (short)   0, (short)   0);  /* Stoff 1 */
	mapcolor(istart++, (short)   0, (short) 255, (short)   0);  /* Stoff 2 */
	mapcolor(istart++, (short)   0, (short)   0, (short) 255);  /* Stoff 3 */
	mapcolor(istart++, (short) 255, (short) 255, (short)   0);  /* Stoff 4 */
	mapcolor(istart++, (short) 255, (short)   0, (short) 255);  /* Stoff 5 */
	mapcolor(istart++, (short)   0, (short) 255, (short) 255);  /* Stoff 6 */
	mapcolor(istart++, (short) 240, (short) 195, (short) 240);  /* Stoff 7 */
	mapcolor(istart++, (short) 100, (short) 190, (short) 150);  /* Stoff 8 */
	mapcolor(istart++, (short) 100, (short) 175, (short) 255);  /* Stoff 9 */
	mapcolor(istart++, (short) 150, (short) 125, (short)   0);  /* Stoff 10*/
	mapcolor(istart++, (short) 200, (short) 225, (short)   0);  /* Stoff 11*/
	mapcolor(istart++, (short)   0, (short) 150, (short) 50);   /* Stoff 12*/
	mapcolor(istart++, (short) 200, (short) 200, (short) 200);  /* Stoff 13*/
	mapcolor(istart++, (short) 255, (short)   0, (short) 100);  /* Stoff 14*/
	mapcolor(istart++, (short) 100, (short) 100, (short) 255);  /* Stoff 15*/
	mapcolor(istart++, (short) 200, (short) 200, (short)   0);  /* Stoff 16*/
	mapcolor(istart++, (short)   0, (short) 100, (short)   0);  /* Stoff 17*/
	mapcolor(istart++, (short)   0, (short)   0, (short) 100);  /* Stoff 18*/
	mapcolor(istart++, (short) 100, (short)   0, (short)   0);  /* Stoff 19*/
	mapcolor(istart++, (short)   0, (short) 200, (short)   0);  /* Stoff 20*/
	mapcolor(istart++, (short)   0, (short)   0, (short) 200);  /* Stoff 21*/
	mapcolor(istart++, (short) 200, (short)   0, (short)   0);  /* Stoff 22*/
	mapcolor(istart++, (short) 200, (short) 200, (short) 200);  /* Stoff 23*/
	mapcolor(istart++, (short) 100, (short) 100, (short) 100);  /* Stoff 24*/
	mapcolor(istart++, (short)   0, (short) 200, (short) 200);  /* Stoff 25*/
	mapcolor(istart++, (short) 255, (short)   0, (short) 200);  /* Stoff 26*/
	mapcolor(istart++, (short) 200, (short)   0, (short) 200);  /* Stoff 27*/
	mapcolor(istart++, (short) 150, (short)   0, (short) 200);  /* Stoff 28*/
	mapcolor(istart++, (short) 100, (short)   0, (short) 200);  /* Stoff 29*/
	mapcolor(istart++, (short)  50, (short)   0, (short) 200);  /* Stoff 30*/
	mapcolor(istart++, (short)   0, (short)   0, (short) 200);  /* Stoff 31*/
	mapcolor(istart++, (short)  50, (short)  50, (short) 100);  /* Stoff 32*/
	mapcolor(istart++, (short)  50, (short) 100, (short) 100);  /* Stoff 33*/
	mapcolor(istart++, (short)  50, (short) 150, (short) 100);  /* Stoff 34*/
	mapcolor(istart++, (short)  50, (short) 200, (short) 100);  /* Stoff 35*/
	mapcolor(istart++, (short)  50, (short) 250, (short) 100);  /* Stoff 36*/
	mapcolor(istart++, (short) 200, (short) 100, (short) 255);  /* Stoff 37*/
	mapcolor(istart++, (short) 200, (short) 100, (short) 200);  /* Stoff 38*/
	mapcolor(istart++, (short) 200, (short) 100, (short) 150);  /* Stoff 39*/

}


/* ===================================================================== */
void SetColorMap256()
{
	int          i, istart, iende;
	short        r, g, b;
	/* --------------------------------------------------------------------- */
	/* Falls Colormap existiert, freigeben: -------------------------------- */
	if(g_tabelle.colortable!=NULL) free(g_tabelle.colortable);

	/* Information in Struktur speichern: ---------------------------------- */
	g_tabelle.art = 1;

	/* Dann Speicher fuer Farbpalette reservieren: 2-fach wg. Datei -------- */
	printf("\nAllokiere Speicherplatz f. 8-Bit C-T");
	g_tabelle.colortable = (int*)malloc(8*ACHT_BIT_MAX_COLORS*sizeof(int));
	if(g_tabelle.colortable==NULL)
	{
		printf("\nFehler: Kein Speicher fuer Colortable!");
	}
	g_tabelle.anzahl_farben = 0;

	istart = ACHT_BIT_TEMP_OFFSET;
	r = -4;
	g = 0;
	b = 260;

	for (i = 0; i < 32; i++)
	{
		r+=6;
		b-=6;
		mapcolor(istart++, r, g, b);
	}

	for (i = 0; i < 33; i++)
	{
		r+=2;
		b-=2;
		mapcolor(istart++, r, g, b);
	}
	r = 255;
	b = 0;
	g = -3;
	mapcolor(istart++, r, 0, b);

	for (i = 0; i < 33; i++)
	{
		g+=4;
		mapcolor(istart++, r, g, b);
	}

	for (i = 0; i < 32; i++)
	{
		g+=4;
		b+=7;
		mapcolor(istart++, r, g, b);
	}
	r = g = b = 255;
	mapcolor(istart++, r, g, b);
	printf( "Number of colors: %i\n", istart - ACHT_BIT_TEMP_OFFSET );

	/* Belegung der Colormap  mit den Stoffarben --------------------------- */
	make_stoff_farben();
	make_achtbit_stroemung();

	g_tabelle.start_index_datei = g_tabelle.anzahl_farben;
	g_tabelle.end_index_datei   = g_tabelle.anzahl_farben;
	g_tabelle.anz_bitplanes = 8;

}


void SetColorMapGrau()            /* Colormap in Grauabstufungen ------- */
{
	int          i, istart, iende;
	short        r, g, b;
	/* --------------------------------------------------------------------- */
	/* Falls Colormap existiert, freigeben: -------------------------------- */
	if(g_tabelle.colortable!=NULL) free(g_tabelle.colortable);

	/* Information in Struktur speichern: ---------------------------------- */
	g_tabelle.art = 2;

	/* Dann Speicher fuer Farbpalette reservieren: 2-fach wg. Datei -------- */
	g_tabelle.colortable = (int*)malloc(8*ACHT_BIT_MAX_COLORS*sizeof(int));
	if(g_tabelle.colortable==NULL)
	{
		printf("\nFehler: Kein Speicher fuer Colortable!");
	}
	g_tabelle.anzahl_farben = 0;

	istart = ACHT_BIT_TEMP_OFFSET;
	r = g = b = 0;
	mapcolor(istart++,0,0,0);
	for (i = 1; i < ACHT_BIT_COLORTABLE_LENGTH-2; i++) 
	{
		mapcolor(istart++, r, b, g);
		r = b = g = (g+2); 
	}
	mapcolor(istart++,254,254,254);
	mapcolor(istart++,255,255,255);
	/* Belegung der Colormap  mit den Stoffarben --------------------------- */
	make_stoff_farben();
	make_achtbit_stroemung();

	g_tabelle.start_index_datei = g_tabelle.anzahl_farben;
	g_tabelle.end_index_datei   = g_tabelle.anzahl_farben;

}

/* ===================================================================== */
void SetColorMapRaster()          /* Colormap in grobem Raster   ------- */
{
	int          i, j,istart, iende;
	short        r, g, b;
	/* --------------------------------------------------------------------- */
	/* Falls Colormap existiert, freigeben: -------------------------------- */
	if(g_tabelle.colortable!=NULL) free(g_tabelle.colortable);

	/* Information in Struktur speichern: ---------------------------------- */
	g_tabelle.art = 3;

	/* Dann Speicher fuer Farbpalette reservieren: 2-fach wg. Datei -------- */
	g_tabelle.colortable = (int*)malloc(8*ACHT_BIT_MAX_COLORS*sizeof(int));
	if(g_tabelle.colortable==NULL)
	{
		printf("\nFehler: Kein Speicher fuer Colortable!");
	}
	g_tabelle.anzahl_farben = 0;

	istart = ACHT_BIT_TEMP_OFFSET;
	r = g = 0;
	b = 255;

	for (i = 0; i < 4; i++) 
	{
		for (j = 0; j < 13; j++)  mapcolor(istart++, r, g, b);
		r = r + 60;
		b = b - 60;
	}

	for (i = 0; i < 4; i++) 
	{
		for (j = 0; j < 13; j++)  mapcolor(istart++, r, g, b);
		g = g + 60;
	}
	b = b + 128;
	for (j = 0; j < 13; j++)  mapcolor(istart++,r,g,b);
	for (j = 0; j < 13; j++)  mapcolor(istart++,255,255,230);

	/* Belegung der Colormap  mit den Stoffarben --------------------------- */
	make_stoff_farben();
	make_achtbit_stroemung();

	g_tabelle.start_index_datei = g_tabelle.anzahl_farben;
	g_tabelle.end_index_datei   = g_tabelle.anzahl_farben;

}

/* ===================================================================== */
int save_colortable() /* Test Prozedur - */
{
	int i;
	FILE *fp;
	/* --------------------------------------------------------------------- */

	/* File oeffnen: ------------------------------------------------------- */
	fp = fopen(g_tabelle.dateiname,"w");

	/* Haeder schreiben ---------------------------------------------------- */
	fprintf(fp,"%d",g_tabelle.anz_bitplanes);

	/* Farben schreiben ---------------------------------------------------- */
	for(i=0;i<g_tabelle.anzahl_farben;i++)
	{
		fprintf(fp,"\nI:%4d R:%5d G:%5d B:%5d",g_tabelle.colortable[i*4],
				g_tabelle.colortable[i*4+1],
				g_tabelle.colortable[i*4+2],g_tabelle.colortable[i*4+3]);
	}
	fclose(fp);
	return(g_tabelle.anzahl_farben);
}

/* ===================================================================== */
int read_colortable() /* Liest C-Map ein */
{
	int i;
	FILE *fp;
	XColor Farbe;
	char zeile[34];
	char warnung[50];
	int z;
	int anz_Bitplanes;
	/* --------------------------------------------------------------------- */

	/* File oeffnen: ------------------------------------------------------- */
	fp = fopen(g_tabelle.dateiname,"r");
	/* Bei Fehler Ruecksprung ---------------------------------------------- */
	if(fp==NULL) 
	{
		strcpy(warnung,"File ");
		strcat(warnung,g_tabelle.dateiname);
		strcat(warnung," does not exist!");
		warning(warnung);
		return(-1);
	}

	/* Haeader lesen: ------------------------------------------------------ */
	if(fgets(zeile,31,fp)!=NULL)
	{
		sscanf(zeile,"%d",&anz_Bitplanes);
		if(anz_Bitplanes!=g_tabelle.anz_bitplanes)
		{
			printf("\nFehler! - Die Datei %s enthaelt Farbwerte fuer %d-Bitdarstellung.",
					g_tabelle.dateiname,anz_Bitplanes);
			printf(" Es wird aber %d-Bitdarstellung angezeigt.",g_tabelle.anz_bitplanes);
			fclose(fp);
			return(-1); 
		}
	}
	else
	{
		printf("\nFehler! - Kann g_tabelle.colortable-File %s nicht lesen!",
				g_tabelle.dateiname);
		fclose(fp);
		return(-1);
	}

	/* Farben Einlesen: ---------------------------------------------------- */
	z = g_tabelle.start_index_datei;
	printf("\nZ:%d",z);
	while(fgets(zeile,33,fp)!=NULL)
	{ 
		sscanf(zeile,"\nI:%4d R:%5d G:%5d B:%5d",&g_tabelle.colortable[z*4],
				&g_tabelle.colortable[z*4+1],&g_tabelle.colortable[z*4+2],
				&g_tabelle.colortable[z*4+3]);
		z++;
	}

	fclose(fp);

	/* Colormap belegen: --------------------------------------------------- */
	for(i=g_tabelle.start_index_datei;i<z;i++)
	{
		Farbe.pixel = g_tabelle.colortable[i*4];
		Farbe.red   = g_tabelle.colortable[i*4+1];
		Farbe.green = g_tabelle.colortable[i*4+2];
		Farbe.blue  = g_tabelle.colortable[i*4+3];
		Farbe.flags = (char)7;
		XStoreColor(dpy,cmap,&Farbe);
	}

	/* Info global vermerken ----------------------------------------------- */
	g_tabelle.end_index_datei = z;
	g_tabelle.art = 1;   

	return(z-g_tabelle.start_index_datei);
}

/***********************************************************
 This routine builds the true color table for temperature
 scale.
 Parameters:
 ===========

***********************************************************/
void true_cmp()
{
	int i;
	int istart;
	short r = 0;
	short g = 0;
	short b = 255;

	istart = 0;
	/* assign colors to temp. scale */
	for (i = 1; i < 225; i++)
	{
		maptcolor(istart++, r++, g, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b--);
		maptcolor(istart++, r, g, b);
	}

	for (i = 1; i < 32; i++)
	{
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r++, g, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b--);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);

		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
	}

	for (i = 0; i < 100; i++)
	{
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g++, b);
		maptcolor(istart++, r, g, b);
		maptcolor(istart++, r, g, b);
	}

	for (i = 0; i < 100; i++)
	{
		b++;
		maptcolor(istart++, r, g, b++);
	}
}

/*
 * map the true color to temp. 
 */
void maptcolor(int i, GLubyte red, GLubyte green, GLubyte blue)
{
	g_tcmap[i].color[0]   = red;
	g_tcmap[i].color[1]   = green;
	g_tcmap[i].color[2]   = blue;
}


/***********************************************************************
   Return a RGB colour value c given a scalar v in the range [vmin,vmax]
   In this case each colour component ranges from 0 (no contribution) to
   1 (fully saturated), modifications for other ranges is trivial.
   The colour is clipped at the end of the scales if v is outside
   the range [vmin,vmax]
   
************************************************************************/
/*
void GetColour(float v,float vmin,float vmax, t_color* c)
{
   float  dv;

   c->color[0] = 255;
   c->color[1] = 255;
   c->color[2] = 255;

   if (v < vmin)
      v = vmin;
   if (v > vmax)
      v = vmax;
   dv = vmax - vmin;

   if (v < (vmin + 0.25 * dv)) {
      c->color[0] = 0;
      c->color[1] =(GLubyte)(255.0*4 * (v - vmin) / dv);
   } else if (v < (vmin + 0.5 * dv)) {
      c->color[0] = 0;
      c->color[2] = (GLubyte) 255.0*(1 + 4 * (vmin + 0.25 * dv - v) / dv);
   } else if (v < (vmin + 0.75 * dv)) {
      c->color[0] = (GLubyte) 255.0 * 4 * (v - vmin - 0.5 * dv) / dv;
      c->color[2] = 0;
   } else {
      c->color[1] = (GLubyte)255.0*(1 + 4 * (vmin + 0.75 * dv - v) / dv);
      c->color[2] = 0;
   }

}
*/

t_colorub C3D_getcolor( float v, float vmin, float vmax, int type )
{
	float maxsize;
	int col;
	t_colorub color;

	maxsize = vmax - vmin;

	if (v < vmin)
		v = vmin;

	if (v > vmax)
		v = vmax;

	col = (int) (v * COLORTABLE_LENGTH / maxsize);
	color.r = (GLubyte)g_tabelle.colortable[4*col+1];
	color.g = (GLubyte)g_tabelle.colortable[4*col+2];
	color.b = (GLubyte)g_tabelle.colortable[4*col+3];

	return color;
}

