/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Stefan Kaesberg                                  */
/* Datum    :   20.11.98                                         */
/* Dateiname:   zeichne_stroemung.c                              */
/*                                                               */
/* Routinenname         | Funktion                               */
/* ------------------------------------------------------------- */
/* zeichne_pfeil        | zeichnet einen Pfeil abh. von Norm&cyc */
/* zeichne_stroemung    | zeichnet Stroemungs-Darstellung        */
/* sortiere_knoten      | bildet eine Liste d. Knoten zu Stoff X */
/* reduziere_gitter     | legt regulaeres Gitter ueber Geometrie */
/* zeichne_reduced      | zeichnet Stroemung auf red. Gitter     */
/* zeichne_simpel       | zeichnet Stroemung auf org. Knoten     */
/* zeichne_rolling      | zeichnet Farbroll-Stroemung            */
/* trace_stroemung      | berechnet Partikelbahn                 */
/* zeichne_partikel     | zeichnet Partikel eines Zeitschritts   */
/* zeichne_stromlinie   | zeichnet Farbrolling-Stroemungslinie   */
/* cycle_color          | rollt die Farben einen Schritt         */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <GL/gl.h>
#include <GL/glu.h>
#include "grafik.h"
#include "color3d.h"
#include "paulslib.h"
#include "memory.h"

#define COLOR_SCHAFT   2047
#define COLOR_SPITZE   2048

/* Typdeklarationen: : */

typedef struct partikelset
{
	int n_partikel;
	t_koord *pos;
} t_partikelset;


#define SCHAFT 0.6 /* Anteil des Schaftes vom gesamten Pfeil */

/* Abstand des Netzes */
#define DIST 0.8

/* Globale Variablen: ------------------------------------------ */
static int farbe_schaft,farbe_spitze,roll_offset,roll_length;

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/* ============================================================= */
/* Determinates color value for drawing vector.                  */
/* ============================================================= */
GLubyte *ColVec(t_koord vector)
{
	float len,Max,Min;
	int tcol,col;
	static GLubyte *color = NULL;
	COLOUR c;

	tcol = g_color_extra.color_name;
	Veclen(len, vector);
	Veclen(Max,g_stroemung.akt_max);
	Veclen(Min,g_stroemung.akt_min);

	if (color == NULL)
		color = (GLubyte *) ALLOC(sizeof(GLubyte) * 3);

	if ( (!C3D_win.achtbit) && (g_tabelle.art == 1) )
	{
		if (len < Min)
		{
			color[0] = (GLubyte) g_tabelle.colortable[1];
			color[1] = (GLubyte) g_tabelle.colortable[2];
			color[2] = (GLubyte) g_tabelle.colortable[3];
		}
		if (len > Max)
		{
			color[0] = (GLubyte) g_tabelle.colortable[4*(COLORTABLE_LENGTH-1)+1];
			color[1] = (GLubyte) g_tabelle.colortable[4*(COLORTABLE_LENGTH-1)+2];
			color[2] = (GLubyte) g_tabelle.colortable[4*(COLORTABLE_LENGTH-1)+3];
		}

		col = (int) ((len - Min) * 
				((float) COLORTABLE_LENGTH / (Max - Min)));

		color[0] = (GLubyte) g_tabelle.colortable[4*col+1];
		color[1] = (GLubyte) g_tabelle.colortable[4*col+2];
		color[2] = (GLubyte) g_tabelle.colortable[4*col+3];

		/* if color map is hot to cold */
		if (tcol)
		{
			c = GetColour( len, Min, Max, tcol );
			color[0] = (GLubyte) (c.r * 255);
			color[1] = (GLubyte) (c.g * 255);
			color[2] = (GLubyte) (c.b * 255);
		}
	}
	else
	{
		col = (int) ((len - Min) * 
				((float) ACHT_BIT_COLORTABLE_LENGTH / 
				 (Max - Min)) + ACHT_BIT_TEMP_OFFSET);
	}

	return (color);
}

/* ============================================================= */
/* zeichne_pfeil zeichnet einen Pfeil abh. v. Normierung und Art.*/
/* ============================================================= */
void zeichne_pfeil(int nummer,t_koord k,t_koord v)
{
	t_bool cycling;
	GLubyte tcol[4]; /* color of the arrow         */   
	float lx,ly,lz;  /* darzustellender Vektors    */
	float e[3];      /* Endkoordinaten des Vektors */
	float s[3];      /* Startkoordinate der Spitze */
	float a[3];      /* 1.Rolling-Zwischenwert     */
	float b[3];      /* 2.Rolling-Zwischenwert     */
	float c[3];      /* 3.Rolling-Zwischenwert     */
	int i;           /* counters                   */
	t_bool arrow_style = FALSE;

	/* Is it right ???*/
	roll_offset = 2024;

	/*
	   if(g_stroemung.cycling) cycling = TRUE;
	   else cycling = FALSE;
	 */

	cycling = FALSE;

	lx = v[0];
	ly = v[1];
	lz = v[2];

	/*
	   lx = 1.0;
	   ly = 1.0;
	   lz = 1.0;
	 */
	if(!((lx==0.0)&&(ly==0.0)&&(lz==0.0)))
	{
		e[0] = k[0]+lx;
		e[1] = k[1]+ly;
		e[2] = k[2]+lz;

		if(cycling)
		{ 
			a[0] = k[0]+0.25*lx;
			a[1] = k[1]+0.25*ly;
			a[2] = k[2]+0.25*lz;

			b[0] = k[0]+0.5 *lx;
			b[1] = k[1]+0.5 *ly;
			b[2] = k[2]+0.5 *lz;

			c[0] = k[0]+0.75*lx;
			c[1] = k[1]+0.75*ly;
			c[2] = k[2]+0.75*lz;

			/*glIndexi(roll_offset);*/
			for(i=0;i<3;i++)
				tcol[i] = 
					g_tabelle.colortable[4*(roll_offset)+i+1];
			glColor3ubv(tcol);
			glBegin(GL_LINES);
			glVertex3fv(k);
			glVertex3fv(a);
			glEnd();
			for(i=0;i<3;i++)
				tcol[i] = 
					g_tabelle.colortable[4*(roll_offset+1)+i+1];
			glColor3ubv(tcol);
			/*glIndexi(roll_offset+1);*/
			glBegin(GL_LINES);
			glVertex3fv(a);
			glVertex3fv(b);
			glEnd();
			for(i=0;i<3;i++)
				tcol[i] = 
					g_tabelle.colortable[4*(roll_offset+2)+i+1];
			glColor3ubv(tcol);
			/*glIndexi(roll_offset+2);*/
			glBegin(GL_LINES);
			glVertex3fv(b);
			glVertex3fv(c);
			glEnd();
			for(i=0;i<3;i++)
				tcol[i] = g_tabelle.colortable[4*(roll_offset+3)+i+1];
			glColor3ubv(tcol);
			/*glIndexi(roll_offset+3);*/
			glBegin(GL_LINES);
			glVertex3fv(c);
			glVertex3fv(e);
			glEnd();
		}
		else
		{
			s[0] = k[0];
			s[1] = k[1];
			s[2] = k[2];
			
			//for(i=0;i<3;i++)
			//	tcol[i] = g_tabelle.colortable[4*(farbe_schaft)+i+1];
			glColor3ubv(ColVec(v));
			/*glIndexi(farbe_schaft);*/
			//glVertex3fv(k);
			//glVertex3fv(s);

			if ( arrow_style )
				darrowfv(s,e, 1.0,1.0);
			else
				darrow2d(s,e, 1.0,1.0);
		}
	}
	return;
}

/* ============================================================= */
/* dehne_vektoren multipliziert die loesungsvekt. mit dem faktor */
/* ============================================================= */
void dehne_vektoren()
{
	int i;         
	for(i=0;i<g_geodat.npunkt;i++)
	{
		g_stroemung.vektor[i][0]= 		
			g_stroemung.vektor[i][0]*g_stroemung.factor/g_stroemung.old_factor;
		g_stroemung.vektor[i][1]= 
			g_stroemung.vektor[i][1]*g_stroemung.factor/g_stroemung.old_factor;
		g_stroemung.vektor[i][2]= 
			g_stroemung.vektor[i][2]*g_stroemung.factor/g_stroemung.old_factor;
	}
	return;
}

/* ============================================================= */
/* sortiere_knoten bildet zu jedem Stoff die dazugeh. Punktliste */
/* ============================================================= */
void sortiere_knoten()
{
	int i,k,j,z,s;
	unsigned char* punkte_besucht; /* Flag-Feld fuer die Knoten ---*/

	/* Erst mal ein zweites Feld der Punkte holen */
	punkte_besucht = (unsigned char*)malloc((g_geodat.npunkt+1)*sizeof(unsigned char));
	if(punkte_besucht==NULL) fprintf(stderr,"\nKein Speicher fuer Punktefeld!");
	else
	{
		/* Stoffgebietsweise durchlaufen und Punkte suchen: */
		for(s=0;s<g_geodat.nstoff;s++)
		{ 
			/* Jetzt alle Punkte demarkieren : */   
			for(k=0;k<g_geodat.npunkt;k++) punkte_besucht[k]=(unsigned char)0;
			k = 0;
			/* Zuerst die Hexaeder: */
			for(j=g_stoffgebiete[s].anfang_hex;j<g_stoffgebiete[s].ende_hex;j++)
			{
				/*for(i=0;i<g_element[j].anz_knoten;i++)*/
				for(i=0;i<8;i++)
				{
					z = g_element[j].pn[i]-1;
					if(punkte_besucht[z]==(unsigned char)0)
					{
						punkte_besucht[z]=(unsigned char)1;
						k++;
					}
				}
			}
			/* Dann die Pentaeder */
			for(j=g_stoffgebiete[s].anfang_pen;j<g_stoffgebiete[s].ende_pen;j++)
			{
				/*for(i=0;i<g_element[j].anz_knoten;i++)*/
				for(i=0;i<6;i++)
				{
					z = g_element[j].pn[i]-1;
					if(punkte_besucht[z]==(unsigned char)0)
					{
						punkte_besucht[z]=(unsigned char)1;
						k++;
					}
				}
			}
			/* Und die Tetraeder */
			for(j=g_stoffgebiete[s].anfang_tet;j<g_stoffgebiete[s].ende_tet;j++)
			{
				/*for(i=0;i<g_element[j].anz_knoten;i++)*/
				for(i=0;i<4;i++)
				{
					z = g_element[j].pn[i]-1;
					if(punkte_besucht[z]==(unsigned char)0)
					{
						punkte_besucht[z]=(unsigned char)1;
						k++;
					}
				}
			}
			printf("\nZum Stoffgebiet %d gehoeren %d Knoten. ",s,k);  

			/* So, jetzt muessen alle Punkte, die Besucht sind, kopiert werden */

			g_stroemung.stoff[s].n_org_punkt   = k;

			/* Liste allokieren: */

			if(g_stroemung.stoff[s].punktlist != NULL) 
				free(g_stroemung.stoff[s].punktlist);      
			g_stroemung.stoff[s].punktlist = (int*)malloc(sizeof(int)*k);

			if(g_stroemung.stoff[s].punktlist == NULL) 
				fprintf(stderr,"\nKein Speicher fuer Stoffgebietspunktliste!");
			else
			{
				z = 0;
				for(k=0;k<g_geodat.npunkt;k++)
				{
					if(punkte_besucht[k]==1) 
					{
						g_stroemung.stoff[s].punktlist[z]=k;
						z++;
					}
				}
			} /* End Else 2*/
		} /* End Schleife ueber Stoffe */   
		g_stroemung.knoten_sortiert = TRUE;
		if(punkte_besucht!=NULL) free(punkte_besucht);
	} /* End Else 1*/

	return;
}


/* ============================================================= */
/* zeichne_simpel zeichnet an jeden Knoten einen Vektor-Pfeil    */
/* ============================================================= */
void zeichne_simpel()
{
	int i,s;
	glLineWidth(2.0);
	for(s=0;s<g_geodat.nstoff;s++)
	{ 
		if(g_stoffgebiete[s].zeichne_on)
		{
			for(i=0;i<g_stroemung.stoff[s].n_org_punkt;i++)
			{
				zeichne_pfeil(i,g_knoten[g_stroemung.stoff[s].punktlist[i]].koord,
						g_stroemung.vektor[g_stroemung.stoff[s].punktlist[i]]);
			}
		}
	}
	return;
}

/* ============================================================= */
/* reduziere_gitter bildet ein regulaeres 3D Gitter ueber ------ */
/* der Geometrie mit gegebenem Punktabstand -------------------- */
/* ============================================================= */
void reduziere_gitter()
{
	int step_x,step_y,step_z;      /* Schrittweiten b. reg. Gitter */
	float max_x,max_y,max_z,min_x,min_y,min_z; /* Min.& Max-Koor.  */
	float k_x,k_y,k_z;		 /* Knotenkoord. relativ         */
	int   n_x,n_y,n_z;		 /* Koord. in Schritten          */
	int   i,j,k,adr,p,t,s,pos;     /* Hilfsvariablen & Zaehler     */
	unsigned char *punkte_besucht; /* Liste der besuchten Knoten   */
	float diff = DIST;		 /* Gitterpunkt-Abstand          */
	float d;	                 /* Abstand zum Gitter-Punk.     */
	int   anz_gitterpunkte;        /* Zaehler fuer die tats. G.Pkt.*/
	t_gitterpunkt *startpunkte;    /* Liste der Gitterpunkte       */

	printf("\nBilde reduziertes Gitter. ");

	/* ggf. erst einmal Alten Speicher freigeben: */
	for(s=0;s<g_geodat.nstoff;s++)
	{
		if(g_stroemung.stoff[s].vektor!=NULL) 
		{
			free(g_stroemung.stoff[s].vektor);
			g_stroemung.stoff[s].vektor = NULL;
		}
		if(g_stroemung.stoff[s].startpunkt!=NULL) 
		{
			free(g_stroemung.stoff[s].startpunkt);
			g_stroemung.stoff[s].startpunkt = NULL;
		}
	}


	/* Zuerst maximale + min. XYZ-Ausdehnung pro Stoff bestimmen: */
	/*max_x = max_y = max_z = 0.0;
	  min_x = min_y = min_z = 65000;*/
	for(s=0;s<g_geodat.nstoff;s++)
	{
		printf("\nTeste Stoffgebiet %d.",s);
		max_x = g_stoffgebiete[s].max[0];
		max_y = g_stoffgebiete[s].max[1];
		max_z = g_stoffgebiete[s].max[2];
		min_x = g_stoffgebiete[s].min[0];
		min_y = g_stoffgebiete[s].min[1];
		min_z = g_stoffgebiete[s].min[2];  

		step_x = (int)((float)(max_x-min_x)/diff);
		step_y = (int)((float)(max_y-min_y)/diff);
		step_z = (int)((float)(max_z-min_z)/diff); 

		step_x++;
		step_y++;
		step_z++;

		printf("\nDer %d. Stoff geht von (%f|%f|%f) bis (%f|%f|%f)."
				,s,min_x,min_y,min_z,max_x,max_y,max_z);
		printf("\nDas red. Netz hat %dx%dx%d Punkte.",step_x,step_y,step_z);

		/* Jetzt die Struktur des reduzierten Gitters aufbauen: */
		g_stroemung.stoff[s].n_punkt = step_x*step_y*step_z;
		g_stroemung.stoff[s].startpunkt = NULL;
		g_stroemung.stoff[s].vektor     = NULL;
		g_stroemung.stoff[s].startpunkt = 
			(t_gitterpunkt*)malloc(g_stroemung.stoff[s].n_punkt*sizeof(t_gitterpunkt));
		if(g_stroemung.stoff[s].startpunkt==NULL) 
		{
			fprintf(stderr,"\nKein Speicher fuer regulaeres Gitter !");
		}
		else
		{ 
			/* Verschub berechnen: */
			k_x = ((step_x)*diff-(max_x - min_x));
			k_y = ((step_y)*diff-(max_y - min_y));
			k_z = ((step_z)*diff-(max_z - min_z));

			g_stroemung.v_x = (k_x - diff)/2.0;
			g_stroemung.v_y = (k_y - diff)/2.0;
			g_stroemung.v_z = (k_z - diff)/2.0;

			printf("\nDie Verschubwerte: %f %f %f",
					g_stroemung.v_x,g_stroemung.v_y,g_stroemung.v_z);
			for(k=0;k<step_z;k++)
			{
				for(j=0;j<step_y;j++)
				{
					for(i=0;i<step_x;i++)
					{
						adr = k*(step_x*step_y)+(j*step_x)+i;                 
						g_stroemung.stoff[s].startpunkt[adr].koo[0]=
							-g_stroemung.v_x+min_x+i*diff; 
						g_stroemung.stoff[s].startpunkt[adr].koo[1]=
							-g_stroemung.v_y+min_y+j*diff;
						g_stroemung.stoff[s].startpunkt[adr].koo[2]=
							-g_stroemung.v_z+min_z+k*diff;
						g_stroemung.stoff[s].startpunkt[adr].dis[0]=DIST;
						g_stroemung.stoff[s].startpunkt[adr].dis[1]=DIST;
						g_stroemung.stoff[s].startpunkt[adr].dis[2]=DIST;
						g_stroemung.stoff[s].startpunkt[adr].dis[3]=DIST;
						g_stroemung.stoff[s].startpunkt[adr].pnr[0]=-1;
						g_stroemung.stoff[s].startpunkt[adr].pnr[1]=-1;
						g_stroemung.stoff[s].startpunkt[adr].pnr[2]=-1;
						g_stroemung.stoff[s].startpunkt[adr].pnr[3]=-1;
					}
				}
			}
			/* Jetzt werden die Knoten auf die Gitterpunkte verteilt: */

			/* Gitterpunkt-Zaehler nullsetzen: */
			anz_gitterpunkte = 0;

			/* Schleife ueber alle Knoten von Stoff s: */
			for(i=0;i<g_stroemung.stoff[s].n_org_punkt;i++)
			{
				/* Index des aktuellen Knotens holen: */
				p = g_stroemung.stoff[s].punktlist[i];

				/* Koordinaten relativ zum Startpunkt des Stoffes s holen: */
				k_x = g_knoten[p].koord[0]+(diff/2.0)+g_stroemung.v_x-min_x-0.0001;
				k_y = g_knoten[p].koord[1]+(diff/2.0)+g_stroemung.v_y-min_y-0.0001;
				k_z = g_knoten[p].koord[2]+(diff/2.0)+g_stroemung.v_z-min_z-0.0001;

				/* Adresse des zugeh. regulaeren Gitterpunktes holen: */
				n_x = (int)(k_x/diff);
				if(n_x<0) n_x = 0;
				n_y = (int)(k_y/diff);
				if(n_y<0) n_y = 0;
				n_z = (int)(k_z/diff);
				if(n_z<0) n_z = 0;

				adr = n_z*((step_x)*(step_y))+(n_y*(step_x))+n_x;
				if(adr>=g_stroemung.stoff[s].n_punkt)
				{
					printf("\nadr(%d|%d|%d) = %d zu gross !",n_x,n_y,n_z,adr);
					exit(0);
				}

				/* Abstand zum Gitterpunkt berechnen: */
				k_x = g_knoten[p].koord[0]-g_stroemung.stoff[s].startpunkt[adr].koo[0];
				k_y = g_knoten[p].koord[1]-g_stroemung.stoff[s].startpunkt[adr].koo[1];
				k_z = g_knoten[p].koord[2]-g_stroemung.stoff[s].startpunkt[adr].koo[2];
				d = (float) sqrt(k_x*k_x+k_y*k_y+k_z*k_z);

				/* Punktnummer sortiert in die Liste der 4 naechsten einordnen: */
				/* Neue Position der Distanz berechnen: */
				pos = -1;
				if(d < g_stroemung.stoff[s].startpunkt[adr].dis[2])
				{
					if(d < g_stroemung.stoff[s].startpunkt[adr].dis[3]) pos = 3;
					else pos = 2;
				}
				else if(d < g_stroemung.stoff[s].startpunkt[adr].dis[0])
				{
					if(d < g_stroemung.stoff[s].startpunkt[adr].dis[1]) pos = 1;
					else pos = 0;
				}
				else
				{
					/* printf("\nKnoten %d an Gitterpunkt %d mit Abstand %f faellt raus.",
					   p,adr,d); */
				}
				if(pos>=0)
				{
					/* Distanzen und Vektoren , die groesser sind, umkopieren: */
					for(j=0;j<pos;j++)
					{
						g_stroemung.stoff[s].startpunkt[adr].dis[j]=
							g_stroemung.stoff[s].startpunkt[adr].dis[j+1];
						g_stroemung.stoff[s].startpunkt[adr].pnr[j]=
							g_stroemung.stoff[s].startpunkt[adr].pnr[j+1];
					}
					/* Neuen Punkt dazufuegen: */

					g_stroemung.stoff[s].startpunkt[adr].pnr[pos]=p;
					g_stroemung.stoff[s].startpunkt[adr].dis[pos]=d;
				}
				else
				{
					/* printf("\nVektor %d mit Abstand %f wird nicht zugeordnet.",p,d);*/
				}
				/* Falls Vektoren dazugezaehlt wurden, Zaehler erhoehen: */
				if(g_stroemung.stoff[s].startpunkt[adr].pnr[3]!=-1) anz_gitterpunkte++;
			}

			/* Hier koennten jetzt alle Gitterknoten, die keine zugeordneten Vek-
			   toren bzw. Punkte haben, aussordiert werden. Im Seben Schritt 
			   bei den Uebriggebliebenen getestet werden, ob sie ueberhaupt in der 
			   Geometrie liegen. (nur im otimierten Fall) */

			/* Umkopieren der tatsaechlich benoetigten Knoten , */
			printf("\nAnzahl der resulterenden Gitterpunkte: %d",anz_gitterpunkte);
			startpunkte = 
				(t_gitterpunkt*)malloc(anz_gitterpunkte*sizeof(t_gitterpunkt));
			if(startpunkte==NULL) 
			{
				fprintf(stderr,"\nZuwenig Speicher fuer regulaeres Gitter !");
			}   
			k = 0;
			for(i=0;i<g_stroemung.stoff[s].n_punkt;i++)
			{
				/* Falls dem Gitterpunkt i Vektoren zugeordnet wurden: */
				if(g_stroemung.stoff[s].startpunkt[i].pnr[3]!=-1)
				{
					/* Werte umkopieren: */
					for(j=0;j<4;j++)
					{
						startpunkte[k].dis[j]= g_stroemung.stoff[s].startpunkt[i].dis[j];
						startpunkte[k].pnr[j]= g_stroemung.stoff[s].startpunkt[i].pnr[j];
					}
					startpunkte[k].koo[0]    = g_stroemung.stoff[s].startpunkt[i].koo[0];
					startpunkte[k].koo[1]    = g_stroemung.stoff[s].startpunkt[i].koo[1];
					startpunkte[k].koo[2]    = g_stroemung.stoff[s].startpunkt[i].koo[2];
					k++;
				}
			}
			/* Nun alte liste freigeben und neue anhaengen */
			free(g_stroemung.stoff[s].startpunkt);
			g_stroemung.stoff[s].startpunkt=startpunkte;
			g_stroemung.stoff[s].n_punkt = k;


			/* Jetzt noch Testen, ob Knoten wirklich ind der Geometrie  */
			/* ... */


			/* Jetzt Speicher fuer die Vektoren allokieren: */


			/* Feld fuer Vektoren holen: */   
			g_stroemung.stoff[s].vektor = 
				(t_koord*)malloc(sizeof(t_koord)* g_stroemung.stoff[s].n_punkt);
			if(g_stroemung.stoff[s].vektor==NULL)
			{
				fprintf(stderr,
						"\nKein Speicher fuer interpolierte Stroemungsvektoren!");
			}

		} /* End Schleife ueber Stoffe */
	} /* End Else */
	g_stroemung.gitter_reduced = TRUE;      
	return;
}

/* ============================================================= */
/* interpoliere_vektoren interpoliert auf dem gefunden Gitter -- */
/* die (max. 4)  Vektoren, die am naechsten liegen. ------------ */
/* ============================================================= */
void interpoliere_vektoren()
{
  int s,i,t,pos,p;
  printf("\nInterpoliere die Vektoren des %d Zeitschritts."
					,g_stroemung.akt_index);
   
  /* Jetzt an jedem Gitterpunkt die bis zu 4 naechsten Vektoren mitteln: */
  for(s=0;s<g_geodat.nstoff;s++)
  {
    /* Zuerst die alten Vektoren loeschen: */
    for(i=0;i<g_stroemung.stoff[s].n_punkt;i++)
    {
      g_stroemung.stoff[s].vektor[i][0]=0.0;
      g_stroemung.stoff[s].vektor[i][1]=0.0;
      g_stroemung.stoff[s].vektor[i][2]=0.0;
    }
    /* Jetzt ggf bis zu 4 Vektoren pro Gitterpunkt sammeln */
    for(i=0;i<g_stroemung.stoff[s].n_punkt;i++)
    {
      if(g_stroemung.stoff[s].startpunkt[i].pnr[3]!=-1)
      {
        t   = 0;
        for(pos=3;pos>=0;pos--)
        { 
          if(g_stroemung.stoff[s].startpunkt[i].pnr[pos]==-1) break;
          else
          {     
            p = g_stroemung.stoff[s].startpunkt[i].pnr[pos]; 
            g_stroemung.stoff[s].vektor[i][0]+=g_stroemung.vektor[p][0];
            g_stroemung.stoff[s].vektor[i][1]+=g_stroemung.vektor[p][1];
            g_stroemung.stoff[s].vektor[i][2]+=g_stroemung.vektor[p][2];
            t++;
          }
        }
        if(t==0)
        {
          printf("\nInterpoliere Vektoren: Versuche durch 0 zu teilen!");
          exit(0);
        }
        /* Und nun mitteln (durch Anzahl teilen): */
        g_stroemung.stoff[s].vektor[i][0]/=(float)t;
        g_stroemung.stoff[s].vektor[i][1]/=(float)t;
        g_stroemung.stoff[s].vektor[i][2]/=(float)t;
      }
    }
  } /* Ende Schleife ueber die Stoffe */    
  
  return;
}

/* ============================================================= */
/* zeichne_reduced zeichnet die Vektoren auf dem reduz. Gitter.  */
/* ============================================================= */
void zeichne_reduced()
{
	int i,s;
	//glBegin(GL_LINES);
	//glLineWidth(2.0);
	for(s=0;s<g_geodat.nstoff;s++)
	{ 
		if(g_stoffgebiete[s].zeichne_on)
		{
			for(i=0;i<g_stroemung.stoff[s].n_punkt;i++)
			{
				zeichne_pfeil(i,g_stroemung.stoff[s].startpunkt[i].koo,g_stroemung.stoff[s].vektor[i]);
			}
		}
	}
	//glEnd();
	return;
}


/* ==============================================================*/
/* Zeichne_Stroemung ist die Schnittstelle fuer zeichne_geom.    */
/* ==============================================================*/

void zeichne_stroemung()
{
  int i;
  /* Welche Farbdarstellung haben wir ? */
  if((!C3D_win.achtbit)&&(g_tabelle.art==1))
  {
    /* 24-Bit Darstellung */
    farbe_schaft = COLOR_SCHAFT;
    farbe_spitze = COLOR_SPITZE;
    roll_offset  = ROLL_OFFSET;
  }
  else
  { 
    /* 8-Bit */
    farbe_schaft = FARBE_SCHAFT_ACHT;
    farbe_spitze = FARBE_SPITZE_ACHT;
    roll_offset  = ACHT_BIT_ROLL_OFFSET;
  }
  roll_length  = ROLL_LENGTH;
  
  /* Erst mal normal zeichnen */
  switch(g_stroemung.art)
  {
    case SIMPEL:
    {
     /*
     dehne_vektoren();
     if(!g_stroemung.knoten_sortiert) sortiere_knoten();
     printf("\nKnoten sortiert!");
     */
     zeichne_simpel();
     break;
    }
    case REDUZIERT:
    {
      /*
      dehne_vektoren();
      if(!g_stroemung.knoten_sortiert) sortiere_knoten();
      */
      if(!g_stroemung.gitter_reduced )
      {
        reduziere_gitter();
        interpoliere_vektoren();
      }
      zeichne_reduced();
      break;
    }
    case TRACER:
    {
      break;
    }
    case ROLLING:
    {
      break;
    }
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
      /* Jetzt gedrehte Stroemung zeichnen: */
      switch(g_stroemung.art)
      {
    	case SIMPEL:
        {
          zeichne_simpel();
          break;
        }
        case TRACER:
        {
          break;
        }
        case REDUZIERT:
        {
          zeichne_reduced();
          break;
        }
        case ROLLING:
        {
          break;
        }
      }
    }
    /* Zurueck drehen: */
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
      if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
      if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
      if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);

      /* Jetzt gespiegelte Stroemung zeichnen: */
      switch(g_stroemung.art)
      {
    	case SIMPEL:
        {
          zeichne_simpel();
          break;
        }
        case REDUZIERT:
        {
          zeichne_reduced();
          break;
        }
        case TRACER:
        {
          break;
        }
        case ROLLING:
        {
          break;
        }
      }           
            
      /* Zuruecksetzen der Spiegelung */
      if(g_transform.mirrorx) glScalef(-1.0,1.0,1.0);
      if(g_transform.mirrory) glScalef(1.0,-1.0,1.0);
      if(g_transform.mirrorz) glScalef(1.0,1.0,-1.0);
           
    }
  }
  return;
}

/***************************************************************

  This function draws the arrow in specified direction.

  Parameter:
  ==========
          - direction rx, ry, rz.
          - color 
          
  Returns:
  ========
          -nothing

****************************************************************/

void arrow_dir()
{

}
