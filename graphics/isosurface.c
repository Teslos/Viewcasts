/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Ivas Toni & Helmut Vor                           */
/* Datum    :   26.08.04                                         */
/* Dateiname:   isosurface.c                                     */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*      C3D_drawisosurface | Drawing isosurface                  */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "grafik.h"
#include "ref.h"
#include "paulslib.h"
#include "memory.h"
#ifdef DEBUG
	#define DEBUGOUT(str) printf(str)	
#else
	#define DEBUGOUT(str) 
#endif
		
enum{
	ZERO = 3,
	ONE  = 2,
	TWO  = 1,
	THREE = 0
};
enum{
	NOMEM = -1,
	VAL_PRO_TRI = 18,
	INIT_VERT = 320000
};

refexport_t re;


GLubyte *Ccolres( float tmp, t_loesung *res )
{
	GLubyte *pcol;
	int col, i;
	int tcol;
	static flag = 0;
	static GLubyte *color = NULL;
	COLOUR c;			/* value of the color r,g,b */
	int icolmap = 1;		/* index of the colormap */
	FILE *fd;

	/* ********************************************************************* */
	tcol = g_color_extra.color_name;
	/* Allocate space for color ******************************************** */
	if (color == NULL)
		color = (GLubyte *) malloc (sizeof (GLubyte) * 3);
	/* Welche Farbdarstellung haben wir ? ---------------------------------- */
	g_tabelle.art = 1;

	if ((!C3D_win.achtbit) && (g_tabelle.art == 1))	/* 24-Bit + Default-Belegung */
	{
		if (tmp < res->Min)
		{
			color[0] = (GLubyte) g_tabelle.colortable[1];
			color[1] = (GLubyte) g_tabelle.colortable[2];
			color[2] = (GLubyte) g_tabelle.colortable[3];
			return (color);
		}
		if (tmp > res->Max)
		{
			color[0] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 1];
			color[1] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 2];
			color[2] = (GLubyte) g_tabelle.colortable[4 * (COLORTABLE_LENGTH - 1) + 3];
			return (color);
		}

		col = (int) ((tmp - res->Min) * ((float) COLORTABLE_LENGTH /
					(res->Max - res->Min)));


		color[0] = (GLubyte) g_tabelle.colortable[4 * col + 1];
		color[1] = (GLubyte) g_tabelle.colortable[4 * col + 2];
		color[2] = (GLubyte) g_tabelle.colortable[4 * col + 3];

		/* if color map is hot to cold */
		if (tcol)
		{
			c = GetColour (tmp, res->Min, res->Max, tcol);
			color[0] = (GLubyte) (c.r * 255);
			color[1] = (GLubyte) (c.g * 255);
			color[2] = (GLubyte) (c.b * 255);
		}

	}
	else		/* Alle anderen Faelle haben Farbdaten in 128 Pixeln ---------- */
	{
		col = (int) ((tmp - res->Min) *
				((float) ACHT_BIT_COLORTABLE_LENGTH / (res->Max -
								       res->Min)) +
				ACHT_BIT_TEMP_OFFSET);

	}
	return (color);
}

int cmp_isosurfaces( const void *cmpdata, const void *data )
{
	int *nr,nr1;
	t_iso *isosurf;
	nr = (int*) cmpdata;
	isosurf = (t_iso*) data;
	nr1 = isosurf->nr;
	if ( *nr > nr1 )
		return 1;
	else if ( *nr < nr1 )
		return -1;
	else
		return 0;
}

int C3D_createisosurface( int mat, t_iso *tiso, int numisol )
{
	TRIANGLEF tri[5];
	GRIDCELL gcell,tcell;
	FIELD field;
	t_loesung *res = NULL;
	t_isomat *isomat = NULL;
	struct listelem *elem;
	int ntri, t;
	int ivol;
	int tc;
	int i = 0;
	int j = 0;
	float *newt;
	float *newf;
	int nr = 0;
	
	isomat = &(tiso->mat[mat-1]);
	
	if ( tiso == NULL )
		return 1;
	/* allocate iso_vertices */
	if ( isomat->iso_vertices == NULL )
	{
		isomat->max_vert = INIT_VERT;
		isomat->iso_vertices = (float*) ALLOC( sizeof(isomat->iso_vertices[0]) * isomat->max_vert );
		isomat->iso_colors   = (GLubyte *) ALLOC( sizeof(isomat->iso_colors[0]) * isomat->max_vert );
	}

	isomat->ntri = 0;

	/* get field which is displayed on isosurface */
	res = C3D_searchresult( 0, "temperature" );
	
	/* create first isosurface for hexaedars */
	for ( ivol = g_stoffgebiete[mat - 1].anfang_hex;
              ivol < g_stoffgebiete[mat - 1].ende_hex; ivol++ )
	{
		tc = g_element[ivol].pn[0] - 1;
		gcell.p[0].x = g_knoten[tc].koord[0];
		gcell.p[0].y = g_knoten[tc].koord[1];
		gcell.p[0].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[1] - 1;
		gcell.p[1].x = g_knoten[tc].koord[0];
		gcell.p[1].y = g_knoten[tc].koord[1];
		gcell.p[1].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[2] - 1;
		gcell.p[2].x = g_knoten[tc].koord[0];
		gcell.p[2].y = g_knoten[tc].koord[1];
		gcell.p[2].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[3] - 1;
		gcell.p[3].x = g_knoten[tc].koord[0];
		gcell.p[3].y = g_knoten[tc].koord[1];
		gcell.p[3].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[4] - 1;
		gcell.p[4].x = g_knoten[tc].koord[0];
		gcell.p[4].y = g_knoten[tc].koord[1];
		gcell.p[4].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[5] - 1;
		gcell.p[5].x = g_knoten[tc].koord[0];
		gcell.p[5].y = g_knoten[tc].koord[1];
		gcell.p[5].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[6] - 1;
		gcell.p[6].x = g_knoten[tc].koord[0];
		gcell.p[6].y = g_knoten[tc].koord[1];
		gcell.p[6].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[7] - 1;
		gcell.p[7].x = g_knoten[tc].koord[0];
		gcell.p[7].y = g_knoten[tc].koord[1];
		gcell.p[7].z = g_knoten[tc].koord[2];

		/* Temperaturen der Punkte -- */
		gcell.val[0] = g_loesung.temp[g_element[ivol].pn[0] - 1];
		gcell.val[1] = g_loesung.temp[g_element[ivol].pn[1] - 1];
		gcell.val[2] = g_loesung.temp[g_element[ivol].pn[2] - 1];
		gcell.val[3] = g_loesung.temp[g_element[ivol].pn[3] - 1];
		gcell.val[4] = g_loesung.temp[g_element[ivol].pn[4] - 1];
		gcell.val[5] = g_loesung.temp[g_element[ivol].pn[5] - 1];
		gcell.val[6] = g_loesung.temp[g_element[ivol].pn[6] - 1];
		gcell.val[7] = g_loesung.temp[g_element[ivol].pn[7] - 1];

		/* calculate iso-surface for particular cell */
		ntri = PolygoniseCube( gcell, tiso->wert, tri );
		while ( i * VAL_PRO_TRI >= isomat->max_vert )
		{
			isomat->max_vert *= 2;
			newt = (float *) RESIZE( isomat->iso_vertices,
				 isomat->max_vert * sizeof(isomat->iso_vertices[0]));
			if ( newt == NULL )
				return NOMEM;
			isomat->iso_vertices = newt;
		}

		for ( t = 0; t < ntri; t++ )
		{
			calcnorm( &tri[t] );
			isomat->iso_vertices[i  ] = tri[t].n[0].x; 
			isomat->iso_vertices[i+1] = tri[t].n[0].y; 
			isomat->iso_vertices[i+2] = tri[t].n[0].z;

			isomat->iso_vertices[i+3] = tri[t].p[0].x; 
			isomat->iso_vertices[i+4] = tri[t].p[0].y; 
			isomat->iso_vertices[i+5] = tri[t].p[0].z;

			isomat->iso_vertices[i+6] = tri[t].n[0].x;
			isomat->iso_vertices[i+7] = tri[t].n[0].y;
			isomat->iso_vertices[i+8] = tri[t].n[0].z;

			isomat->iso_vertices[i+9 ] = tri[t].p[1].x;
			isomat->iso_vertices[i+10] = tri[t].p[1].y;
			isomat->iso_vertices[i+11] = tri[t].p[1].z;

			isomat->iso_vertices[i+12] = tri[t].n[0].x;
			isomat->iso_vertices[i+13] = tri[t].n[0].y;
			isomat->iso_vertices[i+14] = tri[t].n[0].z;

			isomat->iso_vertices[i+15] = tri[t].p[2].x;
			isomat->iso_vertices[i+16] = tri[t].p[2].y;
			isomat->iso_vertices[i+17] = tri[t].p[2].z;
			
			i += 18;
		}
		isomat->ntri += ntri;
	}

	/* create isosurface for tetraedars */
	for ( ivol = g_stoffgebiete[mat - 1].anfang_tet;
              ivol < g_stoffgebiete[mat - 1].ende_tet; ivol++ )
	{
		/* Coordinates of elem. */
		tc = g_element[ivol].pn[0] - 1;
		tcell.p[0].x = g_knoten[tc].koord[0];
		tcell.p[0].y = g_knoten[tc].koord[1];
		tcell.p[0].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[1] - 1;
		tcell.p[2].x = g_knoten[tc].koord[0];
		tcell.p[2].y = g_knoten[tc].koord[1];
		tcell.p[2].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[2] - 1;
		tcell.p[3].x = g_knoten[tc].koord[0];
		tcell.p[3].y = g_knoten[tc].koord[1];
		tcell.p[3].z = g_knoten[tc].koord[2];

		tc = g_element[ivol].pn[3] - 1;
		tcell.p[7].x = g_knoten[tc].koord[0];
		tcell.p[7].y = g_knoten[tc].koord[1];
		tcell.p[7].z = g_knoten[tc].koord[2];

		tcell.val[0]  = g_loesung.temp[g_element[ivol].pn[0] - 1];
		tcell.val[2]  = g_loesung.temp[g_element[ivol].pn[1] - 1];
		tcell.val[3]  = g_loesung.temp[g_element[ivol].pn[2] - 1];
		tcell.val[7]  = g_loesung.temp[g_element[ivol].pn[3] - 1];

		if ( res != NULL )
		{
			field.val[0]  = res->temp[g_element[ivol].pn[0] - 1];
			field.val[2]  = res->temp[g_element[ivol].pn[1] - 1];
			field.val[3]  = res->temp[g_element[ivol].pn[2] - 1];
			field.val[7]  = res->temp[g_element[ivol].pn[3] - 1];
		}

		/* calculate iso-surface for particular cell */
		ntri = PolygoniseTriField( tcell, tiso->wert, tri, field, 0,2,3,7 );
		while ( i * VAL_PRO_TRI >= isomat->max_vert )
		{
			isomat->max_vert *= 2;
			newt = (float *) RESIZE( isomat->iso_vertices, 
			isomat->max_vert * sizeof(isomat->iso_vertices[0]));
			if ( newt == NULL )
				return NOMEM;
			isomat->iso_vertices = newt;
		}
		for( t = 0; t < ntri; t++ )
		{
			calcnorm( &tri[t] );
			isomat->iso_vertices[i  ] = tri[t].n[0].x; 
			isomat->iso_vertices[i+1] = tri[t].n[0].y; 
			isomat->iso_vertices[i+2] = tri[t].n[0].z;

			isomat->iso_vertices[i+3] = tri[t].p[0].x; 
			isomat->iso_vertices[i+4] = tri[t].p[0].y; 
			isomat->iso_vertices[i+5] = tri[t].p[0].z;

			isomat->iso_vertices[i+6] = tri[t].n[0].x;
			isomat->iso_vertices[i+7] = tri[t].n[0].y;
			isomat->iso_vertices[i+8] = tri[t].n[0].z;

			isomat->iso_vertices[i+9 ] = tri[t].p[1].x;
			isomat->iso_vertices[i+10] = tri[t].p[1].y;
			isomat->iso_vertices[i+11] = tri[t].p[1].z;

			isomat->iso_vertices[i+12] = tri[t].n[0].x;
			isomat->iso_vertices[i+13] = tri[t].n[0].y;
			isomat->iso_vertices[i+14] = tri[t].n[0].z;

			isomat->iso_vertices[i+15] = tri[t].p[2].x;
			isomat->iso_vertices[i+16] = tri[t].p[2].y;
			isomat->iso_vertices[i+17] = tri[t].p[2].z;

			i += 18;

			if ( res != NULL )
			{
				GLubyte *scal0, *scal1, *scal2;
				scal0 = (GLubyte*)Ccolres( tri[t].val[0], res );
				scal1 = (GLubyte*)Ccolres( tri[t].val[1], res );
				scal2 = (GLubyte*)Ccolres( tri[t].val[2], res );
				isomat->iso_colors[j  ] = scal0[0];
				isomat->iso_colors[j+1] = scal0[1];
				isomat->iso_colors[j+2] = scal0[2];
				isomat->iso_colors[j+3] = scal1[0];
				isomat->iso_colors[j+4] = scal1[1];
				isomat->iso_colors[j+5] = scal1[2];
				isomat->iso_colors[j+6] = scal2[0];
				isomat->iso_colors[j+7] = scal2[1];
				isomat->iso_colors[j+8] = scal2[2];
				
				tiso->field = TRUE;
				j += 9;
			}
		}
		isomat->ntri += ntri;
	}

}

int C3D_drawisosurface( t_iso *tisosurface, int nr )
{
	if ( tisosurface == NULL || (tisosurface->mat[nr-1]).iso_vertices == NULL ) 
		return 1;

	glDisable( GL_CULL_FACE );	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	shadeon();	
	glDisableClientState( GL_EDGE_FLAG_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );

	glNormalPointer( GL_FLOAT, 6 * sizeof(GLfloat), (tisosurface->mat[nr-1]).iso_vertices );
	glVertexPointer( 3, GL_FLOAT, 6 * sizeof(GLfloat), &((tisosurface->mat[nr-1]).iso_vertices[3]) );
	if ( tisosurface->field )
	{
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( 3, GL_UNSIGNED_BYTE, 0, (tisosurface->mat[nr-1]).iso_colors );
	}
	else
		glColor3f( tisosurface->r, tisosurface->g, tisosurface->b );
	glDrawArrays( GL_TRIANGLES, 0, (tisosurface->mat[nr-1]).ntri*3 );

	shadeoff();
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
	glEnable( GL_CULL_FACE );
	return 0;
}

void  C3D_drawisovolume( t_iso *tiso, int nr )
{
	float iso;
	iso = tiso->wert;

	/* first clip the texture on iso value */
	C3D_clip_tex( iso, 0.0 );
	
	/* then close the clip texture with isosurface */
	C3D_drawisosurface( tiso, nr );
}

void C3D_updateiso( struct list *liso,  t_bool isosurface, t_bool fcreate )
{
	t_iso *tiso;
	static int nl = 0;
	int stoffnr;
	struct listelem *iso;

	/* show the iso one after each other */
	if ( liso == NULL ) return;
	for ( iso = liso->head; iso != NULL; iso = iso->next )
	{
		tiso = ( t_iso * ) iso->data;

		if ( tiso->on )
		{ 
			for (stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++)
			{
				if (g_stoffgebiete[stoffnr-1].zeichne_on)
				{
					if ( fcreate )
					{
						nl++;
						C3D_createisosurface( stoffnr, tiso, nl );
						DEBUGOUT("Creating isosurface\n");
					}

					DEBUGOUT( "\tDrawing isosurface\n");
					if ( isosurface )
						C3D_drawisosurface( tiso, stoffnr );
					else
						C3D_drawisovolume( tiso, stoffnr );
				}
			}
		}
	}
}

void C3D_isoinit()
{
	CMD_addcommand( "drawisosurface", C3D_drawisosurface );
	CMD_addcommand( "drawisovol", C3D_drawisovolume );
}

refexport_t GetRefIsoAPI( refimport_t rimp )
{
	refexport_t re;
	refimport_t ri;
	ri = rimp;
	re.C3D_updateiso = C3D_updateiso;
	re.C3D_updatecut = C3D_updatecut;
	re.C3D_drawisosurface = C3D_drawisosurface;	
	re.C3D_drawisovolume  = C3D_drawisovolume;
	re.C3D_drawcut        = C3D_drawcut;

	return re;
}

void calcnorm( TRIANGLE *tri )
{
	float ax,ay,az,bx,by,bz;
	float l;

	/* calculate normal */
	ax=(tri->p[1].x-tri->p[0].x);
	ay=(tri->p[1].y-tri->p[0].y);
	az=(tri->p[1].z-tri->p[0].z);
	bx=(tri->p[2].x-tri->p[0].x);
	by=(tri->p[2].y-tri->p[0].y);
	bz=(tri->p[2].z-tri->p[0].z);

	tri->n[0].x = (by*az - bz*ay);
	tri->n[0].y = (bx*az - bz*ax);
	tri->n[0].z = (bx*ay - by*ax);
	Normalise(&(tri->n[0]));
}
