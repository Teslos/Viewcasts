/*************************************************************************
 This routine cutts the all elements:
 hexaedars, pentaedars, tetraedars.
 By applying the algorithm for 
 polygonize the cube or tetraedar.

 Author: Toni Ivas & Helmut Vor
**************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include "paulslib.h"
#include "grafik.h"
#include "strukturen.h"
#include "memory.h"

/* num.of nodes hexa, penta, teta*/
enum { NNHEX=8, NNPEN=6, NNTET=4 }; 
enum { NOMEM=-1, INIT_VERT = 32000, VAL_PRO_TRI = 18, COL_PRO_TRI = 9,
       TEX_COORD_PRO_TRI = 3 };
/* local functions */
static void hex2cube(GRIDCELL *g, double *d, int ivol);
static void pen2tet(GRIDCELL *g, double *d, int ivol);
static void tet2tet(GRIDCELL *g, double *d, int ivol);
static int  alloc_mem(int t, t_cut *cut);
static int  alloc_vals(int t, t_cut *cut);
static int  alloc_tex_coords(int t, t_cut *cut);
/*************************************************************************
 This function draws the cut on the model.
 

 Parameters:
           cp - cliping plane vector

 Returns:

 Warnings:

 For now, triangles are immediately draw, but
 probably much faster way is to make the
 triangualar list and draw them all at once.
*************************************************************************/
int C3D_createcut(int stoffnr, t_isocut *tcut)
{
	float cp[4];
	float np[3];     /* normal to a plane */
	t_koord p[8];    /* points of the current element */
	double d[8];     /* distances of nodes of current elem. to plane */
	GRIDCELL g;      /* gridcell on which we calculate the cut */ 
	double iso;      /* isoline value for cutting algorithm */
	TRIANGLEF tri[5];/* result of cutting algorithm */
	FIELD field;     /* vals of the field */
	int i = 0, j = 0, k = 0;     /* counters */
	int ivol;        /* index of volume element */
	int ntri;        /* number of triangles to draw */
	double dp;       /* distance of plane from origin */
	t_cut *cut;      /* cut info and data */
	int t = 0;       /* point counter */
	GLubyte *scal0, *scal1, *scal2;
	t_loesung *res = NULL;


	if ( tcut == NULL )
		return 1;

	res = C3D_searchresult( 0, "temperature" );
	tcut->field = TRUE;

	cut = &(tcut->mat[stoffnr-1]);	
	if ( cut->vertices == NULL )
	{
		cut->max_vert = INIT_VERT;
		cut->max_vals = INIT_VERT;
		cut->max_tex_coords = INIT_VERT;
		cut->vertices = (float*) ALLOC(sizeof(cut->vertices[0]) * cut->max_vert);
		cut->vals = (GLubyte*) ALLOC(sizeof(cut->vals[0]) * cut->max_vals);
		cut->tex_coords = (GLfloat*) ALLOC(sizeof(cut->tex_coords[0]) * cut->max_tex_coords);
	}

	g_transform.schneiden_on = TRUE;

	/* get the normal of the current plane */
	cp[0] = tcut->cplane[0]; cp[1] = tcut->cplane[1];
	cp[2] = tcut->cplane[2]; cp[3] = tcut->cplane[3];
	NormalToPlane(np,cp);
	/* get isoline value */
	dp = -cp[3];

	/* go thru all points */
	/* calculate the distance from the 
	 * cutting plane
	 */

	/* for all hexaedars */
	for (ivol = g_stoffgebiete[stoffnr-1].anfang_hex;
			ivol < g_stoffgebiete[stoffnr-1].ende_hex; ivol++)
	{
		short sign[8];
		int flag = 0;
		int draw = 0;
		/* calculate distance from the cutting plane
		 * for all nodes of hexaedar
		 */
		for (i = 0; i < NNHEX; i++) 
		{
			p[i][0] = g_knoten[g_element[ivol].pn[i] - 1].koord[0];
			p[i][1] = g_knoten[g_element[ivol].pn[i] - 1].koord[1];
			p[i][2] = g_knoten[g_element[ivol].pn[i] - 1].koord[2];
		
			DisPointToPlane(d[i],p[i],np,dp);
			if (d[i] > 0.0) sign[i]  = 1;
			else if (ABS(d[i]) < EPSILON) sign[i] = 0;
			else sign[i] = -1;
		}

		/* transform our element in gridcell */
		hex2cube(&g, d, ivol);

		/* polygonise hexaedars */
		ntri = PolygoniseCube(g, 0.0, tri);
		alloc_mem( t, cut );
		alloc_vals( t, cut );
		alloc_tex_coords( t, cut );

		for(i = 0; i < ntri; i++) {
			calcnorm( &tri[i] );

			cut->vertices[t  ] = tri[i].n[0].x;
			cut->vertices[t+1] = tri[i].n[0].y;
			cut->vertices[t+2] = tri[i].n[0].z;

			cut->vertices[t+3] = tri[i].p[0].x;
			cut->vertices[t+4] = tri[i].p[0].y;
			cut->vertices[t+5] = tri[i].p[0].z;

			cut->vertices[t+6] = tri[i].n[0].x;
			cut->vertices[t+7] = tri[i].n[0].y;
			cut->vertices[t+8] = tri[i].n[0].z;

			cut->vertices[t+9 ] = tri[i].p[1].x;
			cut->vertices[t+10] = tri[i].p[1].y;
			cut->vertices[t+11] = tri[i].p[1].z;

			cut->vertices[t+12] = tri[i].n[0].x;
			cut->vertices[t+13] = tri[i].n[0].y;
			cut->vertices[t+14] = tri[i].n[0].z;

			cut->vertices[t+15] = tri[i].p[2].x;
			cut->vertices[t+16] = tri[i].p[2].y;
			cut->vertices[t+17] = tri[i].p[2].z;
			t += 18;  
		}
		cut->ntri += ntri;
		g_element[ivol].zeichnen = 0;
	}


	/* polygonise pentaedars 
	 * by decomposing pentaedar 
	 * in 3 tetraedars 
	 */
	for (ivol = g_stoffgebiete[stoffnr-1].anfang_pen;
			ivol < g_stoffgebiete[stoffnr-1].ende_pen; ivol++)
	{
		short sign[8];
		int flag = 0;
		int draw = 0;
		/* calculate distance from the cutting plane
		 * for all nodes of pentaedar
		 */
		for (i = 0; i < NNPEN; i++) 
		{
			p[i][0] = g_knoten[g_element[ivol].pn[i] - 1].koord[0];
			p[i][1] = g_knoten[g_element[ivol].pn[i] - 1].koord[1];
			p[i][2] = g_knoten[g_element[ivol].pn[i] - 1].koord[2];

			DisPointToPlane(d[i],p[i],np,dp);

			if (d[i] > 0.0) sign[i]  = 1;
			else if ( ABS(d[i]) < EPSILON ) sign[i] = 0;
			else sign[i] = -1;
		}

		/* transform our element in gridcell */
		for(j = 0; j < 3; j++)
		{
			pen2tet(&g, d, ivol);

			/* polygonise tetraedars */
			ntri = PolygoniseTri(g, iso, tri, 0,2,3,7);
			alloc_mem( t, cut );
			alloc_vals( t, cut );
			alloc_tex_coords( t, cut );

			/* draw the triangles */
			glBegin(GL_TRIANGLES);
			for(i = 0; i < ntri; i++) {
				glVertex3f(tri[i].p[0].x,tri[i].p[0].y,tri[i].p[0].z);
				glVertex3f(tri[i].p[1].x,tri[i].p[1].y,tri[i].p[1].z);
				glVertex3f(tri[i].p[2].x,tri[i].p[2].y,tri[i].p[2].z);
			}
			glEnd();
		}
	}

	/* polygonise tetraedars */
	for (ivol = g_stoffgebiete[stoffnr-1].anfang_tet;
			ivol < g_stoffgebiete[stoffnr-1].ende_tet; ivol++)
	{
		short sign[8];
		int flag = 0;
		int draw = 0;

		/* calculate distance from the cutting plane
		 * for all nodes of pentaedar
		 */
		for (i = 0; i < NNTET; i++) 
		{
			p[i][0] = g_knoten[g_element[ivol].pn[i] - 1].koord[0];
			p[i][1] = g_knoten[g_element[ivol].pn[i] - 1].koord[1];
			p[i][2] = g_knoten[g_element[ivol].pn[i] - 1].koord[2];
		
			DisPointToPlane(d[i],p[i],np,dp);

			if (d[i] > 0.0) sign[i]  = 1;
			else if ( ABS(d[i]) < EPSILON ) sign[i] = 0;
			else sign[i] = -1;
		}

		/* get the current field */	
		field.val[0] = res->temp[g_element[ivol].pn[0] - 1];
		field.val[2] = res->temp[g_element[ivol].pn[1] - 1];
		field.val[3] = res->temp[g_element[ivol].pn[2] - 1];
		field.val[7] = res->temp[g_element[ivol].pn[3] - 1];
	
		/* transform our element in gridcell */
		tet2tet(&g, d, ivol);

		/* polygonise tetraedar */
		ntri = PolygoniseTriField(g, 0.0, tri, field, 0, 2, 3, 7);
		alloc_mem( t, cut );
		alloc_vals( t, cut );
		alloc_tex_coords( t, cut );

		/* draw the triangles */
		for(i = 0; i < ntri; i++) {
			calcnorm( &tri[i] );

			cut->vertices[t  ] = tri[i].n[0].x;
			cut->vertices[t+1] = tri[i].n[0].y;
			cut->vertices[t+2] = tri[i].n[0].z;

			cut->vertices[t+3] = tri[i].p[0].x;
			cut->vertices[t+4] = tri[i].p[0].y;
			cut->vertices[t+5] = tri[i].p[0].z;

			cut->vertices[t+6] = tri[i].n[0].x;
			cut->vertices[t+7] = tri[i].n[0].y;
			cut->vertices[t+8] = tri[i].n[0].z;

			cut->vertices[t+9 ] = tri[i].p[1].x;
			cut->vertices[t+10] = tri[i].p[1].y;
			cut->vertices[t+11] = tri[i].p[1].z;

			cut->vertices[t+12] = tri[i].n[0].x;
			cut->vertices[t+13] = tri[i].n[0].y;
			cut->vertices[t+14] = tri[i].n[0].z;

			cut->vertices[t+15] = tri[i].p[2].x;
			cut->vertices[t+16] = tri[i].p[2].y;
			cut->vertices[t+17] = tri[i].p[2].z;
			t += 18; 

			scal0 = (GLubyte*)Ccolres( tri[i].val[0], res );
			scal1 = (GLubyte*)Ccolres( tri[i].val[1], res );
			scal2 = (GLubyte*)Ccolres( tri[i].val[2], res );

			cut->tex_coords[k  ] = (GLfloat) CcolTex( tri[i].val[0] );
			cut->tex_coords[k+1] = (GLfloat) CcolTex( tri[i].val[1] );
			cut->tex_coords[k+2] = (GLfloat) CcolTex( tri[i].val[2] );

			cut->vals[j  ] = scal0[0];
			cut->vals[j+1] = scal0[1];
			cut->vals[j+2] = scal0[2];
			cut->vals[j+3] = scal1[0];
			cut->vals[j+4] = scal1[1];
			cut->vals[j+5] = scal1[2];
			cut->vals[j+6] = scal2[0];
			cut->vals[j+7] = scal2[1];
			cut->vals[j+8] = scal2[2];

			j += 9;
			k += 3;
		}
		cut->ntri += ntri;
		g_element[ivol].zeichnen = 0;
	}
	return 0;
}

static int alloc_mem(int t, t_cut *cut)
{
	float *newt;

	while( t * VAL_PRO_TRI >= cut->max_vert )
	{
		cut->max_vert *= 2;
		newt = (float*) RESIZE( cut->vertices, 
				cut->max_vert * sizeof(cut->vertices[0]));
		if ( newt == NULL )
			return NOMEM;
		cut->vertices = newt;
	}
	return 0;
}

static int alloc_vals(int t, t_cut *cut)
{
	GLubyte *newt;

	while( t * COL_PRO_TRI >= cut->max_vals )
	{
		cut->max_vals *= 2;
		newt = (GLubyte *) RESIZE( cut->vals,
			cut->max_vals * sizeof( cut->vals[0] ));
		if ( newt == NULL )
			return NOMEM;
		cut->vals = newt; 
	}
	return 0;
}

static int alloc_tex_coords(int t, t_cut *cut)
{
	GLfloat *newt;

	while( t * TEX_COORD_PRO_TRI >= cut->max_tex_coords )
	{
		cut->max_tex_coords *= 2;
		newt = (GLubyte *) RESIZE( cut->tex_coords,
			cut->max_tex_coords * sizeof( cut->tex_coords[0] ));
		if ( newt == NULL )
			return NOMEM;
		cut->tex_coords = newt; 
	}
	return 0;
}

/*
=======================================
C3D_updatecut - update the cut
region in list of cuts.
=======================================
*/
void C3D_updatecut( struct list *lcut, t_bool fcreate )
{
	int stoffnr;
	t_isocut *tcut;
	struct listelem *cut;
	
	if ( lcut == NULL ) return;

	for ( cut = lcut->head; cut != NULL; cut = cut->next )
	{
		tcut = (t_isocut*) cut->data;
		if ( tcut->on )
		{
			for ( stoffnr = 1; stoffnr <= g_geodat.nstoff; stoffnr++ )
			{
				if ( g_stoffgebiete[stoffnr-1].zeichne_on )
				{	
					if ( fcreate )
						C3D_createcut( stoffnr, tcut );
					if( C3D_drawcut( tcut, stoffnr ) )
						SYS_error( "Error: Not valid cut structure in C3D_drawcut.\n" );
				}
			}
		}
	}
}

int C3D_drawcut( t_isocut *icut, int nr )
{
	if ( icut == NULL || (icut->mat[nr-1]).vertices == NULL )
		return 1;
	glDisable( GL_CULL_FACE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	shadeon();
	glDisableClientState( GL_EDGE_FLAG_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );

	glNormalPointer( GL_FLOAT, 6*sizeof(GLfloat), (icut->mat[nr-1]).vertices );
	glVertexPointer( 3, GL_FLOAT, 6*sizeof(GLfloat), &((icut->mat[nr-1]).vertices[3]) );
	if ( icut->field )
	{
		//glEnableClientState( GL_COLOR_ARRAY );
		//glColorPointer( 3, GL_UNSIGNED_BYTE, 0, (icut->mat[nr-1]).vals );
		glEnable( GL_TEXTURE_1D );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 1, GL_FLOAT, 0, (icut->mat[nr-1]).tex_coords );
	}
	glDrawArrays( GL_TRIANGLES, 0, (icut->mat[nr-1]).ntri * 3 );
	shadeoff();
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_CULL_FACE );
	return 0;
}

static void hex2cube(GRIDCELL *g, double *d, int ivol) 
{
	int i,j;
	int hex2cube[8] = {7,6,5,4,3,2,1,0};  /* connection between gridcell and hexa */

	for (i = 0; i < NNHEX; i++)
	{ 
		//j = hex2cube[i];
		j = i;
		g->p[j].x = g_knoten[g_element[ivol].pn[i]-1].koord[0];
		g->p[j].y = g_knoten[g_element[ivol].pn[i]-1].koord[1];
		g->p[j].z = g_knoten[g_element[ivol].pn[i]-1].koord[2];
		g->val[j] = d[i];
	}
}

static void pen2tet(GRIDCELL *g, double *d, int ivol)
{
	int i,j;
	int pen2tetra[8] = { 0,0,0,0,0,0,0,0 };  /* connection between gridcell and penta */
	
	for (i = 0; i < NNPEN; i++)
	{
		j = pen2tetra[i];
		g->p[j].x = g_knoten[g_element[ivol].pn[i]-1].koord[0];
		g->p[j].y = g_knoten[g_element[ivol].pn[i]-1].koord[1];
		g->p[j].z = g_knoten[g_element[ivol].pn[i]-1].koord[2];
		g->val[j] = d[i];
	}
}

static void tet2tet(GRIDCELL *g, double *d, int ivol)
{
	int i,j;
	int tet2tetra[8] = { 0,2,3,7,0,0,0,0 };  /* connection between gridcell and penta */

	for (i = 0; i < NNTET; i++)
	{
		j = tet2tetra[i];
		g->p[j].x = g_knoten[g_element[ivol].pn[i]-1].koord[0];
		g->p[j].y = g_knoten[g_element[ivol].pn[i]-1].koord[1];
		g->p[j].z = g_knoten[g_element[ivol].pn[i]-1].koord[2];
		g->val[j] = d[i];
	}
}
