/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname        | Funktion                                */
/* ------------------------------------------------------------- */
/* lese_feld_ein       | prueft welche Datei gelesen werden muss */
/* lese_feld           | liest ein Loesungsfeld aus Loesungsdatei*/
/* lese_loesung        |                                         */
/* scann_loesung       | scannt <Loesungsdatei>                  */
/* lese_vektor         | liest ein Loesungsfeld aus Vektorloesung*/
/* lese_vektorloesung  |                                         */
/* scann_vektorloesung | scannt Vektorloesung                    */
/* lese_absenkung      | Datei hflap lesen                       */
/* belege_punktwerte   | belege Loesungsfeld f. gepickten Punkt  */
/* belege_zeitenfeld   | belege Zeitenfeld f. Animation u. Result*/
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "color3d.h"
#include "grafik.h"
#include "strukturen.h"
#include "motif.h"
#include "memory.h"
#include "mem.h"
#include "errs.h"

#ifndef MAP_FILE
#define MAP_FILE 0 /* to compile on other than 4.3+BSD */
#endif
/*                                                               */
/* globale Funktionen :                                          */
/* ==================                                            */
extern t_bool bigendien;

/* Routinen :                                                    */
/* ============================================================= */
int maxmin_vektor( t_koord *vektor, int npunkt, float *Max, float *Min );
void destroy_results(void*);

/*
=========================
 C3D_initresults
 Initializes result file info and
 global structures for reading results.
=========================
*/
void C3D_initresults(void)
{
	if ( g_results == NULL )
	{
		NEW( g_results );
		glistinit( g_results, destroy_results );
	}
}

/*
=========================
 C3D_searchresult
 Searching current valid result.
=========================
*/
t_loesung *C3D_searchresult( int numsol, char *str )
{
	int n = 0;
	struct listelem *re;
	t_loesung *loes;

	if ( g_results == NULL )
		return;
	if ( str == NULL )
	{
		for ( re = g_results->head, n = 0; re != NULL; re = re->next, n++ )
		{
			loes = ( t_loesung *) re->data;
			if ( n == numsol )
				return loes;
		}
	}
	else
	{
		for ( re = g_results->head; re != NULL; re = re->next )
		{
			loes = ( t_loesung* ) re->data;
			if ( !stricmp( loes->definition.text, str ) )
				return loes;	
		}
	}
	return NULL;
}
		 
/*****************************************************************/
void verzerre_geometrie()
/*****************************************************************/
{
	int i;
	for (i = 0; i < g_geodat.npunkt; i++)
	{
		g_spannung.verzerrung[i].koord[0] = g_knoten[i].koord[0]
			+g_spannung.factor*g_spannung.vektor[i][0];
		g_spannung.verzerrung[i].koord[1] = g_knoten[i].koord[1]
			+g_spannung.factor*g_spannung.vektor[i][1];
		g_spannung.verzerrung[i].koord[2] = g_knoten[i].koord[2]
			+g_spannung.factor*g_spannung.vektor[i][2];
	}
	return;
}

/*****************************************************************/
int lese_loesung(char* dateiname)
/*****************************************************************/
/*  RETURNS: int 0   - kein Fehler                               */
/*              -1   - Fehler beim oeffnen der Datei             */
/*              -2   - Fehler beim allozieren von Speicherplatz  */
/*              -3   - <file>  ist keine regulaere Datei         */
/*              -4   - <file>  ist leer oder nicht lesbar        */
/*              -5   - <file>  gehoert nicht zur Geometrie       */
{
	struct stat stbuf;  /* Struktur zur Ueberpruefung des Namens - */
	int  reg_file;      /* Variable z.Ueberpruefung d. Dateistatus */
	int  anzahl_felder; /* Anzahl Felder in Loesungsdatei -------- */
	FILE *fd;           /* Filepointer --------------------------- */
	float *ntemp;       /* Temp. pointer ------------------------- */
	t_loesung *res;

#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf (stdout, "Loesungsdatei = >%s<\n", dateiname);
#endif
	NEW( res );
	res->temp = NULL;
	res->mmap = FALSE;
	if ( !file_read(dateiname) ) return (-3);

	/* File info --------------------------------------------------- */  
	file_info( dateiname, res );

	/* Datei oeffnen ----------------------------------------------- */
	if ((res->loes_in = fopen(dateiname,"r")) == NULL) return (-1);

	res->offen = TRUE;
	anzahl_felder = scann_loesung( res );
	if (anzahl_felder <= 0)
	{
		fclose(res->loes_in);
		fprintf(stderr, "Datei konnte nicht gelesen werden\n");
	}

	if (anzahl_felder == 0) return (-4);
	if (anzahl_felder == -1) return (-5);
	if (anzahl_felder == -2) return (-2);

	res->anzahl_felder = anzahl_felder;
	res->gescannt = TRUE; 

	if ( res->temp == NULL )
	{
		res->temp = 
			(float*) ALLOC((g_geodat.npunkt)*sizeof(float));
	}
	else
	{
		ntemp = 
		(float*) RESIZE(res->temp, (g_geodat.npunkt)*sizeof(float));
		res->temp = ntemp;
	}

	strcpy(res->dateiname, dateiname);
	res->anim_anfang = 0;
	res->anim_ende   = res->anzahl_felder - 1;

	g_results->head = gaddend( g_results, gnewitem( (void*) res ));
	g_loesung = *res;
	if (belege_zeitenfeld(1))
		warning( "Error: No struct for animation available! " );

	return (0);
}
/*************** ende lese_loesung *******************************/

/*****************************************************************
  This function reads scalar result file.

  Parameters:
  ===========
         -char *dateiname - name of data file

  Returns:
  ========
*****************************************************************/
int mmread_res_scalar(char* dateiname)
{
	struct rlimit limit;
	struct stat statbuf;
	int fd;
	char *src;

	if ( (fd = open(dateiname, O_RDONLY)) < 0)
		error(Eio);

	/* check file permission, info data */
	if ( fstat(fd, &statbuf) < 0)
		error(Estat);

	/* ask machine how many memory is at use */
	if (pr_limits("RLIMIT_STACK", RLIMIT_STACK, &limit) < 0)
		error(Elimit);

	if (statbuf.st_size > limit.rlim_cur * 0.5)
	{
		lese_loesung(dateiname);
		g_loesung.mmap = FALSE;
		return (0);
	}

	/* allocate memory in mem arena */
	if (g_mem.arena == NULL)
	{
		g_mem.arena = Arena_new();
		g_mem.scalars = ARENA_ALLOC(g_mem.arena, statbuf.st_size);
	}

	if ( (src = mmap (NULL, statbuf.st_size, PROT_READ,
					MAP_FILE | MAP_SHARED, fd, 0)) == (caddr_t) -1) 
	{
		error(Emmap);
		perror("Explanation: ");
	}

	memcpy(g_mem.scalars, src, statbuf.st_size);
	return (0);
}

/*****************************************************************
  This function reads one time-step of scalar solution.

  Parameters:
  ===========
         -int schritt - time step

  Returns:
  ========
          0 - success
         -1 - error
*****************************************************************/
int mmlese_feld (int schritt)
{
	char *ptr;
	float dummy;
	float zeit;
	char rec_len[4];

	if (g_mem.scalars == NULL) /* no memory ? */
		return (-1);

	ptr = g_mem.scalars;
	ptr += (schritt) * (g_geodat.npunkt + 4) * 4;
	memcpy(rec_len, ptr, sizeof(rec_len));
	ptr += sizeof(rec_len);
	memcpy(&zeit, ptr, sizeof(zeit));
	ptr += sizeof(zeit);
	ptr += sizeof(dummy); /* jump over dummy */

	/* read scalar solution (temperature) */
	g_loesung.temp = (float *)ptr;

	g_loesung.gelesen = TRUE;
	return(0);
}

/*****************************************************************/
int lese_feld_ein(int schritt)
/*****************************************************************/
{
	/*  RETURNS: int 0   - kein Fehler ----------------------------- */
	/*              -1   - keine Loesungsdatei eingelesen ---------- */
	/*              -2   - Fehler in Leseroutine ------------------- */

	if (g_animation.temp_on && g_loesung.gescannt)
	{
		if (g_loesung.mmap)
			mmlese_feld(schritt);
		else
		{
			if (lese_feld(schritt)) 
				return (-2);
		}
	}
	else if (g_animation.spann_on && g_spannung.vektor_gescannt)
	{
		if (lese_vektor(schritt, 2)) 
			return (-2);
		else
			verzerre_geometrie();
	}
	else if (g_animation.stroem_on && g_stroemung.vektor_gescannt)
		if (lese_vektor(schritt, 3)) return (-2);
	else 	
		return -1;
	return (0);
/*************** ende lese_feld_ein ******************************/
}

/*****************************************************************/
int lese_feld(int schritt)
/*****************************************************************/
{
	/*  RETURNS: int 0   - kein Fehler                               */
	/*              -2   - Fehler beim allozieren von Speicherplatz  */

	int   i,j;           /* Zaehler ------------------------------ */
	float dummy;         /* Dummy-variable ----------------------- */
	int tmp_i;
	float tmp_f;	
	float zeit;          /* Dummy-variable ----------------------- */
	float Min, Max;      /* Minimum- und Maximum des Loesungsfeldes*/
	t_bool  min_max;
	char  ftn_rec_len[4];/* Recordlaenge fuer unformatierte Datei  */

	/* *********************************************************** */
	/* Ruecksprung falls keine Loesungsdatei geoeffnet ----------- */

	/* set the flag for auto-range */
	min_max = g_loesung.min_max;

	if (!g_loesung.gescannt)
		return -1;

	if ( bigendien )
	{
		/* einlesen des Zeitschrittes 'schritt' - */
		i = fseek(g_loesung.loes_in, (long)(schritt) * (g_geodat.npunkt + 4) * 4, 0);
		i = fread((char*) ftn_rec_len, sizeof(ftn_rec_len), 1, g_loesung.loes_in);
		i = fread((char*) &zeit, sizeof(float), 1, g_loesung.loes_in);
		i = fread((char*) &dummy, sizeof(dummy), 1, g_loesung.loes_in);
		i = fread((char*) g_loesung.temp, sizeof (float),
				(int) g_geodat.npunkt, g_loesung.loes_in);
	}
	else
	{
		i = fseek( g_loesung.loes_in, (long)(schritt) * (g_geodat.npunkt + 4) * 4, 0 );
		i = fread(( char* )ftn_rec_len, sizeof( ftn_rec_len ), 1, g_loesung.loes_in );
		i = fread(( char* )&tmp_f, sizeof( tmp_f ), 1, g_loesung.loes_in );
		i = fread(( char* )&dummy, sizeof( dummy ), 1, g_loesung.loes_in );
		i = fread(( char* )g_loesung.temp, sizeof( g_loesung.temp[0] ),
			(int) g_geodat.npunkt, g_loesung.loes_in );

		zeit        = LittleLong( tmp_f );

		/* change to little endien */
		for ( j = 0; j < g_geodat.npunkt; j ++ )
		{
			g_loesung.temp[j] = LittleFloat( g_loesung.temp[j] );
		} 	
	}

	/* bestimme Parameterintervall ---------- */
	if (min_max)
	{
		Max = -100000;
		Min = -Max;

		for (i = 0; i < g_geodat.npunkt; i++)
		{
			Max = MAX(g_loesung.temp[i+1],Max);
			Min = MIN(g_loesung.temp[i+1],Min);
		}

		/* add min and max to result info */
		g_loesung.akt_min = Min;
		g_loesung.akt_max = Max;

		/* update global min and max */
		g_loesung.Min = MIN(g_loesung.Min, g_loesung.akt_min);
		g_loesung.Max = MAX(g_loesung.Max, g_loesung.akt_max);
	}

#ifdef DEBUG
	printf("Min: %f, Max: %f\n", Min, Max);
#endif

	g_loesung.gelesen = TRUE; 
	return (0);
}
/**************** ende lese_feld *********************************/

/*****************************************************************/
int lese_vektorloesung(char* dateiname, int art)
/*****************************************************************/
/*  RETURNS: int 0   - kein Fehler                               */
/*              -1   - Fehler beim oeffnen der Datei             */
/*              -2   - Fehler beim allozieren von Speicherplatz  */
/*              -3   - <file>  ist keine regulaere Datei         */
/*              -4   - <file>  ist leer oder nicht lesbar        */
/*              -5   - <file>  gehoert nicht zur Geometrie       */
{
	struct stat stbuf;  /* Struktur zur Ueberpruefung des Namens - */
	int  reg_file;      /* Variable z.Ueberpruefung d. Dateistatus */
	int  anzahl_felder; /* Anzahl Felder in Loesungsdatei -------- */
	FILE *fopen();      /* Filepointer --------------------------- */
	t_loesung *res;
#ifdef DEBUG
	if (applRessourcen.debug)
		fprintf (stdout, "Vektor-Loesungsdatei = >%s<\n", dateiname);
#endif
	if (!file_read(dateiname)) return (-3);

	file_info( dateiname,res );

	if (art == 2)      /* Spannung ------------------------------- */
	{
		/* Datei oeffnen ----------------------------------------------- */
		if ((g_spannung.vektor_in = fopen(dateiname,"r")) == NULL)
			return (-1);
		g_spannung.vektor_offen = TRUE;
		anzahl_felder = scann_vektorloesung(g_spannung.vektor_in, art);
		if (anzahl_felder == 0) return (-4);
		if (anzahl_felder == -1) return (-4);
		if (anzahl_felder == -2) return (-5);

		strcpy(g_spannung.dateiname, dateiname);
		g_spannung.anim_anfang   = 0;
		g_spannung.anim_ende     = g_spannung.anzahl_felder - 1;
		g_spannung.akt_index     = 0;

		if (belege_zeitenfeld(art))
			warning("Error: No struct for animation available ! ");
		else
			lese_vektor(0, 2);
	}
	else if (art == 3)     /* Stroemung -------------------------- */
	{
		/* Datei oeffnen ----------------------------------------------- */
		if ((g_stroemung.vektor_in = fopen(dateiname,"r")) == NULL)
			return (-1);

		g_stroemung.vektor_offen = TRUE;
		anzahl_felder = scann_vektorloesung(g_stroemung.vektor_in, art);
		if (anzahl_felder == 0) return (-4);
		if (anzahl_felder == -1) return (-4);
		if (anzahl_felder == -2) return (-5);

		strcpy(g_stroemung.dateiname, dateiname);
		g_stroemung.anim_anfang   = 0;
		g_stroemung.anim_ende     = g_stroemung.anzahl_felder - 1;
		g_stroemung.akt_index     = 1;
		switch(g_stroemung.art)
		{
			case SIMPEL:
			{
				if(!g_stroemung.knoten_sortiert) sortiere_knoten();
				break;
			}
			case REDUZIERT:
			{
				if(!g_stroemung.gitter_reduced )
				{
					if(!g_stroemung.knoten_sortiert) sortiere_knoten();
						reduziere_gitter();
				}
				break;
			}
		}
		if (belege_zeitenfeld(art))
			warning("Error: No struct for animation available ! ");
		else
			lese_vektor(0, 3);
	}
	else return (-1);
	return (0);
}
/******************** ende lese_vektorloesung ********************/

/*****************************************************************/
int lese_vektor(int index, int art)
/*****************************************************************/
{
	/*  RETURNS: int 0   - kein Fehler                             */
	/*              -2   - Fehler beim allozieren von Speicherplatz*/
	int   i;             /* Zaehler ------------------------------ */
	int   schritt;       /* aktueller Zeitschritt ---------------- */
	long  pos;
	float vektor_zeit,   /* akt. Zeit aus Spannungs/Stroemungsloesg*/
	      dummy;         /* Dummy-Variable ----------------------- */
	t_bool  min_max;     /* TRUE:= Minimum bzw Maximum bestimmt -- */
	t_koord Min, Max, 
		norvec;      /* Minimum- und Maximum der X-,Y- und Z-  */
	/* Komponenten des Vektor-Loesungsfeldes  */
	char  ftn_rec_len[4];/* Recordlaenge fuer unformatierte Datei  */
	/* ************************************************************* */
	schritt = index;
	min_max = TRUE;
	norvec[0]=1.0; norvec[1]=1.0; norvec[2]=1.0;   
	if (art == 2 && g_spannung.vektor_gescannt)  /* Spannungsfeld lesen - */
	{
		/*
		   schritt = g_spannung.akt_index;
		 */
		/* einlesen des aktuellen Zeitschrittes aus file.dispp --------- */
		pos = schritt * (g_geodat.npunkt * 3 + 4) * sizeof(float);
		fseek(g_spannung.vektor_in, pos, 0);
		fread((char*)ftn_rec_len, sizeof (ftn_rec_len),1,g_spannung.vektor_in);
		fread((char*) &vektor_zeit,sizeof(float), 1, g_spannung.vektor_in);

		fread((char*) &dummy,sizeof(float),1,g_spannung.vektor_in);

		fread((char*) g_spannung.vektor,sizeof(t_koord),
				(int)g_geodat.npunkt, g_spannung.vektor_in);
		/* bestimme Parameterintervall ---------- */
		g_spannung.vektor_gelesen = TRUE;
		verzerre_geometrie();
	}
	else if (art == 3 && g_stroemung.vektor_gescannt)
	{                                    /* Stroemungsfeld lesen - */
		/* einlesen des aktuellen Zeitschrittes aus file.strom --------- */
		pos = schritt * (g_geodat.npunkt * 3 + 4) * sizeof(float);
		fseek(g_stroemung.vektor_in, pos, 0);
		fread((char*)ftn_rec_len, sizeof(ftn_rec_len),1,g_stroemung.vektor_in);
		fread((char*) &vektor_zeit,sizeof(float), 1, g_stroemung.vektor_in);

		fread((char*)ftn_rec_len, sizeof(ftn_rec_len),1,g_stroemung.vektor_in);
		fread((char*)g_stroemung.vektor, sizeof(t_koord),
				(int)g_geodat.npunkt, g_stroemung.vektor_in);
		g_stroemung.vektor_gelesen = TRUE;
		dehne_vektoren();

		if (g_stroemung.art == REDUZIERT) interpoliere_vektoren();
	}

	if (min_max)
	{
		Max[0] = 0.0;
		Max[1] = 0.0;
		Max[2] = 0.0;
		Min[0] = 10000;
		Min[1] = 10000;
		Min[2] = 10000;
		if ( art == 2 && g_spannung.vektor_gescannt ) 
		{	
			maxmin_vektor( g_spannung.vektor, g_geodat.npunkt, Max, Min );
			Veccpy(g_spannung.akt_min, Min);
			Veccpy(g_spannung.akt_max, Max);
			/* get global minimum and maximum */
			if ( MINVEC( g_spannung.akt_min, g_spannung.Min ) )
				Veccpy( g_spannung.Min, g_spannung.akt_min ); 
			if ( MAXVEC( g_spannung.akt_max, g_spannung.Max ) )
				Veccpy( g_spannung.Max, g_spannung.akt_max );
		}
		else if ( art == 3 && g_stroemung.vektor_gescannt )
		{
			maxmin_vektor( g_stroemung.vektor, g_geodat.npunkt, Max, Min );
			Veccpy(g_stroemung.akt_min, Min);
			Veccpy(g_stroemung.akt_max, Max);
			if ( MINVEC( g_stroemung.Min, g_stroemung.akt_min ) )
				Veccpy( g_stroemung.Min, g_stroemung.akt_min );
			if ( MAXVEC( g_stroemung.Max, g_stroemung.akt_max ) )
				Veccpy( g_stroemung.Max, g_stroemung.akt_max );
		}

	} 
	return (0);
}

int 
maxmin_vektor( t_koord *vektor, int npunkt, float *Max, float *Min )
{
	int i;
	for (i = 0; i < npunkt; i++)
	{
		if ( MAXVEC(vektor[i], Max) )
			Veccpy(Max, vektor[i]);

		if ( MINVEC(vektor[i], Min) )
			Veccpy(Min, vektor[i]);
	}
	return 0;
}

/* ***************************************************************/
int scann_loesung( t_loesung *res )
/* ***************************************************************/

/* Routine oeffnet Loeungsdatei, sammelt Zeiten der Loesungsfelder*/
/* und gibt die Anzahl der Felder zurueck  --------------------- */

/* Returnwerte : return >  0 : Anzahl der Loesungsfelder in Datei*/
/*               return =  0 : kein vollstaendiges Feld in Datei */
/*               return = -2 : Fehler beim allocieren v. Speicher*/
/*               return = -1 : Datei passt nicht zur Geometrie   */
{
	int  nitem,           /* Anzahl zu lesender Daten ------------ */
	     anzahl;          /* Anzahl Zeitschritte in Loesung ------ */
	char ftn_rec_len[4];  /* Recordlaenge fuer unformatierte Datei */
	float zeit;           /* Zeit des aktuellen Loesungsfeldes --- */
	float *zeitenfeld;    /* Feld mit Zeiten aus der Loesungsdatei */
	long idummy;          /* Dummy-variable ---------------------- */
	/* ************************************************************* */
	res->gescannt = TRUE; /* Flagge: Loesung ist gescannt --- */
	anzahl = 0;
	nitem  = 1;
	zeitenfeld = NULL;

	/* sammeln der Zeitschritte aus Loesungsdatei ------------------ */
	while (fread((char*)ftn_rec_len,
				sizeof(ftn_rec_len), nitem, res->loes_in) == nitem)
	{
		if ( zeitenfeld == NULL ) 
			zeitenfeld = (float*)
					ALLOC(sizeof(zeitenfeld[0]));
		else
			zeitenfeld = (float*)
					RESIZE( zeitenfeld, (anzahl+1) * sizeof(zeitenfeld[0]) );

		fread( (char*)&zeitenfeld[anzahl], sizeof(float), nitem, res->loes_in );
		fread( (char*)&idummy, sizeof(idummy), nitem, res->loes_in );
		/* Teste ob im 1. Feld Eintrag nach <Zeit> = #Punkte ----------- */
		if (!anzahl)
		{
			if (idummy != g_geodat.npunkt)
			{
				FREE (zeitenfeld);
				return (-1);
			}
		}  
		anzahl ++;
		fseek(res->loes_in, (long)g_geodat.npunkt * sizeof(float) +
				sizeof(ftn_rec_len), 1);
	}
	if (anzahl > 0) res->zeiten = zeitenfeld;
	return (anzahl);
/* ********************** ende scann_loesung  ************************ */
}

/* ******************************************************************* */
int scann_vektorloesung(FILE *file_in, int art)
/* ******************************************************************* */

/* Routine oeffnet Loeungsdatei, sammelt Zeiten der Loesungsfelder --- */
/* und gibt die Anzahl der Felder zurueck  ------------------ -------- */

/* Returnwerte : return >  0 : Anzahl der VektorLoesungsfelder in Datei*/
/*               return =  0 : kein vollstaendiges Feld in Datei ----- */
/*               return = -1 : falsches Format der Datei ------------- */
/*               return = -2 : Fehler beim allocieren v. Speicher ---- */
{
	char  ftn_rec_len[4];       /* Recordlaenge fuer unformatierte Datei- */
	int   nitem,                /* Anzahl zu lesender Daten ------------- */
	      anzahl_felder;        /* Anzahl Zeitschritte in Vektorloesung-- */
	float *vektorzeitenfeld;    /* Feld mit Zeiten aus der Vektorlsgsdatei*/
	long  idummy;               /* Dummy-variable ----------------------- */
	/* ******************************************************************** */
	anzahl_felder = 0;
	nitem  = 1;
	vektorzeitenfeld = NULL;

	/* sammeln der Zeitschritte in file.dispp bzw file.strom ----------- */
	while (fread((char*)ftn_rec_len, sizeof(ftn_rec_len),
				nitem, file_in) == nitem)
	{
		if ((vektorzeitenfeld = (float*)realloc(vektorzeitenfeld,
						(anzahl_felder+1) * sizeof(float))) == NULL)
			return(-2); 
		/* Zeiten der Parameterfelder einlesen ------------------------------ */
		fread((char*)&vektorzeitenfeld[anzahl_felder],sizeof(float),nitem,
				file_in);
		fread((char*) &idummy,sizeof(idummy),nitem,file_in);
		anzahl_felder ++;
		if (fseek(file_in, (long)g_geodat.npunkt * 3 * sizeof(float) +
					sizeof(ftn_rec_len), 1))
			warning(" Error while reading vector solution");
	} /* ende WHILE */
	if (anzahl_felder > 0)
	{
		if (art == 2)
		{
			g_spannung.anzahl_felder = anzahl_felder;
			g_spannung.zeiten = vektorzeitenfeld;
			g_spannung.vektor_gescannt = TRUE;
		}
		else if (art == 3)
		{
			g_stroemung.anzahl_felder = anzahl_felder;
			g_stroemung.zeiten = vektorzeitenfeld;
			g_stroemung.vektor_gescannt = TRUE;
		}
	}  
	return (anzahl_felder);
	/* **************** ende scann_vektorloesung ************************** */  
}

/* ******************************************************************** */
int lese_absenkung(char *file)
/* ******************************************************************** */
{
	/* Routine liest Stuetzwerte und Stoffe die abgesenkt werden ein --- */
	/* Returns : 0 OK -------------------------------------------------- */
	/*           1 Fehler, oder Datei existiert nicht ------------------ */
	/*           2 Datei passt nicht zur eingelesenen Geometrie -------- */
	int   anz_stuetz, anz_stofen, nstoff;
	int   i, j, index;
	int   dummy;
	int   zeit, pos;
	FILE  *fd, *fopen();  /* Filediscriptor -------------------------- */
	/* ******************************************************************** */
	nstoff = g_geodat.nstoff;
	/* Oeffnen der Datei fuer Verschiebung der Stoffe ------------------ */
	if ((fd = fopen(file,"r")) == NULL) return (1);

	i = fscanf(fd, "%i%i%i%i%i%i%i%i",/* lese #Stuetzstellen, #Stoffe  */
			&dummy, &dummy, &anz_stuetz, &dummy, &dummy, &dummy, &dummy,
			&anz_stofen);
	if (anz_stofen != 0)
	{
		/* allociere Platz fuer Stoffe die abgesenkt werden ---------------- */
		if ((g_ofen.stofen = (int*) ALLOC((nstoff) * sizeof(int))) == NULL)
			return (1);         /* Kein Platz ---------------------- */
		/* allociere Platz fuer Stuetzstellen ----------------------- */
		if ((g_ofen.pos_zeiten =
					(float*) ALLOC((anz_stuetz)*sizeof(float)))==NULL)
			return (1);         /* Kein Platz ---------------------- */
		/* allociere Platz fuer Positionen--- ----------------------- */
		if ((g_ofen.positionen =
					(t_koord*) ALLOC((anz_stuetz)*sizeof(t_koord)))==NULL)
			return (1);         /* Kein Platz ---------------------- */

		for (j = 0; j < g_geodat.nstoff; j++) g_ofen.stofen[j] = 0;

		for (j = 0; j < anz_stofen; j++)  /* markiere Stoffe ---------- */
		{
			i = fscanf(fd, "%i", &index);
			if (index < 0 || index > g_geodat.nstoff) return (2);
			g_ofen.stofen[index-1] = 1;
		}

		for (j = 0; j < anz_stuetz; j++)  /* markiere Stoffe ---------- */
		{
			g_ofen.positionen[j][0] = 0.0;
			g_ofen.positionen[j][1] = 0.0;
			g_ofen.positionen[j][2] = 0.0;
			i = fscanf(fd, "\n%i%i", &zeit, &pos);
			g_ofen.positionen[j][2] = pos / 100.0;
			g_ofen.pos_zeiten[j] = zeit / 100.0;
		}
	}

	g_ofen.anz_stofen = anz_stofen;
	g_ofen.anz_stuetz = anz_stuetz;

	fclose (fd);

	return (0);
	/* *************** ende lese_absenkung ******************************** */
}

/* ********************************************************************* */
int belege_punktwerte(t_pickpunkt *pickpunkt)
/* ********************************************************************* */
{
	int   refpkt1, refpkt2, pn; /* Referenz-Punkt --------------------- */
	int   i, j;                 /* Zaehler ---------------------------- */
	float t_max, t_min;         /* min. u. max. Parameterwert --------- */
	char  ftn_rec_len[4];       /* Recordlaenge fuer unformatierte Datei*/
	char *ptr;                  /* Pointer to memory mapped file ------ */
	float zeit;
	char rec_len[4];
	float dummy;
	/* ********************************************************************* */
	if (pickpunkt->punkt_nr <= g_geodat.npunkt)
	{
		refpkt1 = pickpunkt->punkt_nr;
	}
	else
	{
		pn = pickpunkt->punkt_nr - g_geodat.npunkt;

		refpkt1 = g_schnittpunktliste[pn-1].p1;
		refpkt2 = g_schnittpunktliste[pn-1].p2;
	}

	if ((pickpunkt->werte = (float*) ALLOC
				(g_loesung.anzahl_felder * sizeof(float))) == NULL)
		return (1);

	if ((pickpunkt->werte1 = (float*) ALLOC
				(g_loesung.anzahl_felder * sizeof(float))) == NULL)
		return (1);

	/* if memory maped file */
	if (g_loesung.mmap)
	{
		ptr = g_mem.scalars;
		memcpy(rec_len, ptr, sizeof(rec_len));
		ptr += sizeof(rec_len);
		memcpy(&zeit, ptr, sizeof(zeit));
		ptr += sizeof(zeit);


		/* jump to reference point */
		ptr += (refpkt1 + 1) * sizeof(float);

		memcpy(&pickpunkt->werte[0], ptr, sizeof(float));

		for (j = 1; j < g_loesung.anzahl_felder; j++)
		{
			ptr += ((g_geodat.npunkt + 4) * sizeof(float));
			memcpy(&pickpunkt->werte[j], ptr, sizeof(float));
		}
	}
	else
	{

		i = fseek(g_loesung.loes_in, (refpkt1 + 1) * sizeof(float) +
				sizeof(ftn_rec_len), SEEK_SET);


		/* 1. Wert einlesen --------------- */
		i = fread((char*)&pickpunkt->werte[0],sizeof(float),1,g_loesung.loes_in);

		/* Schleife ueber Anzahl Felder --- */
		for (j = 1; j < g_loesung.anzahl_felder; j++)
		{ 
			/* positioniere auf Punkt --------- */
			i = fseek(g_loesung.loes_in, (g_geodat.npunkt + 1) * sizeof(float) +
					2 * sizeof(ftn_rec_len), SEEK_CUR);
			/* lese Wert ein ------------------ */
			i = fread((char*)&pickpunkt->werte[j],
					sizeof(float),1,g_loesung.loes_in);
		}



		if (pickpunkt->punkt_nr > g_geodat.npunkt) /* if the point is cutted */
		{
			/* rewind the file */
			rewind(g_loesung.loes_in);

			i = fseek(g_loesung.loes_in, (refpkt2 + 1) * sizeof(float) +
					sizeof(ftn_rec_len), SEEK_SET);
			/* 2. Wert einlesen -------------- */
			i = fread((char*)&pickpunkt->werte1[0],sizeof(float),1,g_loesung.loes_in);

			for (j = 1; j < g_loesung.anzahl_felder; j++)
			{
				i = fseek(g_loesung.loes_in, (g_geodat.npunkt + 1) * sizeof(float) +
						2 * sizeof(ftn_rec_len), SEEK_CUR);
				i = fread((char*)&pickpunkt->werte1[j],
						sizeof(float),1,g_loesung.loes_in);

				/* interpolate on this cutted point */
				pickpunkt->werte1[j] = (float)interpolate2(pickpunkt->werte[j],
						pickpunkt->werte1[j], g_schnittpunktliste[pn-1].verhaeltnis);

			}
		}
	}

	t_min = 10000;                 /* init max- u. min-werte ---------- */
	t_max = -t_min;
	for (j = 0; j < g_loesung.anzahl_felder; j++)
	{        
		/* bestimmen der Max. u. Min.-werte  */
		t_max = MAX(pickpunkt->werte[j],t_max);
		t_min = MIN(pickpunkt->werte[j],t_min);
	}
	pickpunkt->min = t_min;
	pickpunkt->max = t_max;

	return (0);
	/* ********************************************************************* */
}

/* ********************************************************************* */
int belege_zeitenfeld(int art)
/* ********************************************************************* */
{
	int    i, save_art;
	t_bool gleiche_felder;
	char   c_hilf1[64],
	       c_hilf2[64],
	       c_hilf3[64];
	/* --------------------------------------------------------------------- */
	gleiche_felder = TRUE;
	save_art = g_animation.art;
	switch (art)
	{
		case 1:        /* Temperatur --------------------------------------- */
			if (g_animation.zeiten == NULL)
			{
				/* es ist noch kein Zeitenfeld eingetragen ----------------------------- */
				if (g_loesung.zeiten != NULL & g_loesung.anzahl_felder > 0)
				{
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_loesung.anzahl_felder*sizeof(float)))
					{
						g_animation.anzahl_felder = g_loesung.anzahl_felder;
						/* Korrektur des Listenaufbaus, Aendernung der Schleife 17.07.03 HV ---- */
						for (i = 0; i < g_loesung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_loesung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = g_animation.anzahl_felder-1;
						g_animation.art = 1;
					} 
					else return (1);  /* kein Speicherplatz frei ----------------- */
				}
				else return (2);    /* kein Zeitenfeld in Loesungsstruktur ----- */
			}
			else
			{
				/* es ist schon ein Zeitenfeld eingetragen ----------------------------- */
				/* teste ob beide Zeitenfelder gleich, falls nicht : ------------------- */
				/* Abfrage nach welchem Feld animiert werden soll ---------------------- */
				if (g_animation.art == 1) /* Es ist schon eine Tmp-lsg eingelesen*/
				{
					if(g_animation.zeiten != NULL)
						FREE(g_animation.zeiten);
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_loesung.anzahl_felder*sizeof(int)))
					{
						g_animation.anzahl_felder = g_loesung.anzahl_felder;
						for (i = 0; i < g_loesung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_loesung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = i-1;
					}
					else return (1);  /* kein Speicherplatz frei ----------------- */
				}
				else
				{
					if (g_loesung.anzahl_felder == g_animation.anzahl_felder)
					{
						for (i = 0; i < g_loesung.anzahl_felder; i++)
							if (g_loesung.zeiten[i] != g_animation.zeiten[i])
								gleiche_felder = FALSE;
					}
					else
					{
						gleiche_felder = FALSE;
					}
					if (!gleiche_felder)
					{
						/* setze Abfrage zusammen ---------------------------------------------- */
						if (g_sprache == SPRACHE_DEUTSCH)
						{
							strcpy(c_hilf1, "Nach welcher Loesung soll animiert werden ?");
							strcpy(c_hilf2, "Temperaturloesung");
							if (g_animation.art == 2)
								strcpy(c_hilf3, "Spannungsloesung");
							else if (g_animation.art == 3)
								strcpy(c_hilf3, "Stroemungsloesung");
						}
						else
						{
							strcpy(c_hilf1, "Select which solution you wish for the animation:");
							strcpy(c_hilf2, "Temperature");
							if (g_animation.art == 2)
								strcpy(c_hilf3, " or Stress/Strain");
							else if (g_animation.art == 3)
								strcpy(c_hilf3, " or Fluid Flow");
						}

						if (g_sprache == SPRACHE_DEUTSCH)
						{
							if (!strcmp(ask_user(c_hilf1, c_hilf2, c_hilf3, NULL, 1),
										"Temperaturloesung"))
							{
								g_animation.art = 1;
								if (g_animation.anzahl_felder != g_loesung.anzahl_felder)
								{
									if (g_animation.zeiten = (float*)
											realloc(g_animation.zeiten,g_loesung.anzahl_felder * 
												sizeof(int)))
									{
										g_animation.anzahl_felder = g_loesung.anzahl_felder;
										for (i = 0; i < g_loesung.anzahl_felder; i++)
											g_animation.zeiten[i] = g_loesung.zeiten[i];
										g_animation.anim_anfang = 0;
										g_animation.anim_ende   = i-1;
									}
									else g_animation.art = save_art;
								}
							}
						}
						else
						{
							if (!strcmp(ask_user(c_hilf1, c_hilf2, c_hilf3, NULL, 1),
										"Temperature"))
							{   
								g_animation.art = 1;
								if (g_animation.anzahl_felder != g_loesung.anzahl_felder)
								{ 
									if (g_animation.zeiten = (float*)
											realloc(g_animation.zeiten,g_loesung.anzahl_felder *
												sizeof(int)))
									{
										g_animation.anzahl_felder = g_loesung.anzahl_felder;
										for (i = 0; i < g_loesung.anzahl_felder; i++)
											g_animation.zeiten[i] = g_loesung.zeiten[i];
										g_animation.anim_anfang = 0;
										g_animation.anim_ende   = i-1;
									}
									else g_animation.art = save_art;
								}
							}
						}
					}
				}
			}
			g_animation.temp_on = TRUE;
			break;
		case 2:        /* Spannung ----------------------------------------- */
			if (g_animation.zeiten == NULL)
			{
				/* es ist noch kein Zeitenfeld eingetragen ----------------------------- */
				if (g_spannung.zeiten != NULL & g_spannung.anzahl_felder > 0)
				{
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_spannung.anzahl_felder*sizeof(int)))
					{
						g_animation.anzahl_felder = g_spannung.anzahl_felder;
						for (i = 0; i < g_spannung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_spannung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = i-1;
						g_animation.art = 2;
					}
					else return (1);  /* kein Speicherplatz frei ----------------- */
				}
				else return (2);    /* kein Zeitenfeld in Loesungsstruktur ----- */
			}
			else
			{
				/* es ist schon ein Zeitenfeld eingetragen ----------------------------- */
				/* teste ob beide Zeitenfelder gleich, falls nicht : ------------------- */
				/* Abfrage nach welchem Feld animiert werden soll ---------------------- */
				if (g_animation.art == 2) /*Es ist schon eine Span-lsg eingelesen*/
				{
					FREE(g_animation.zeiten);
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_spannung.anzahl_felder*sizeof(int)))
					{
						g_animation.anzahl_felder = g_spannung.anzahl_felder;
						for (i = 0; i < g_spannung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_spannung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = i-1;
					}
					else return (1);
				}
				else
				{
					if (g_stroemung.anzahl_felder == g_animation.anzahl_felder)
					{
						for (i = 0; i < g_spannung.anzahl_felder; i++)
							if (g_stroemung.zeiten[i] != g_animation.zeiten[i])
								gleiche_felder = FALSE;
					}
					else
					{
						gleiche_felder = FALSE;
					}
					if (!gleiche_felder)
					{
						/* setze Abfrage zusammen ---------------------------------------------- */
						strcpy(c_hilf1, "Nach welcher Loesung soll animiert werden ?");
						strcpy(c_hilf2, "Spannungsloesung");
						if (g_animation.art == 1)
							strcpy(c_hilf3, "Temperaturloesung");
						else if (g_animation.art == 3)
							strcpy(c_hilf3, "Stroemungsloesung");
						if (!strcmp(ask_user(c_hilf1, c_hilf2, c_hilf3, NULL, 1),
									"Spannungsloesung"))
						{
							g_animation.art = 2;
							if (g_animation.anzahl_felder != g_spannung.anzahl_felder)
							{
								if (g_animation.zeiten = (float*)
										realloc(g_animation.zeiten,g_spannung.anzahl_felder *
											sizeof(int)))
								{
									g_animation.anzahl_felder = g_spannung.anzahl_felder;
									for (i = 0; i < g_spannung.anzahl_felder; i++)
										g_animation.zeiten[i] = g_spannung.zeiten[i];
									g_animation.anim_anfang = 0;
									g_animation.anim_ende   = i-1;
								}
								else g_animation.art = save_art;
							}
						}
					}
				}
			}
			g_animation.spann_on = TRUE;
			break;
		case 3:        /* Stroemung --------------------------------------- */
			if (g_animation.zeiten == NULL)
			{
				/* es ist noch kein Zeitenfeld eingetragen ----------------------------- */
				if (g_stroemung.zeiten != NULL & g_stroemung.anzahl_felder > 0)
				{
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_stroemung.anzahl_felder*sizeof(int)))
					{
						g_animation.anzahl_felder = g_stroemung.anzahl_felder;
						for (i = 0; i < g_stroemung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_stroemung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = i-1;
						g_animation.art = 3;
					}
					else return (1);  /* kein Speicherplatz frei ----------------- */
				}
				else return (2);    /* kein Zeitenfeld in Loesungsstruktur ----- */
			}
			else
			{
				/* es ist schon ein Zeitenfeld eingetragen ----------------------------- */
				/* teste ob beide Zeitenfelder gleich, falls nicht : ------------------- */
				/* Abfrage nach welchem Feld animiert werden soll ---------------------- */
				if (g_animation.art == 3) /* Es ist schon eine Str-lsg eingelesen*/
				{
					FREE(g_animation.zeiten);
					/* alloziere Platz fuer Zeitenfeld ------------------------------------- */
					if (g_animation.zeiten =
							(float*)ALLOC(g_stroemung.anzahl_felder*sizeof(int)))
					{
						g_animation.anzahl_felder = g_stroemung.anzahl_felder;
						for (i = 0; i < g_stroemung.anzahl_felder; i++)
							g_animation.zeiten[i] = g_stroemung.zeiten[i];
						g_animation.anim_anfang = 0;
						g_animation.anim_ende   = i-1;
					}
					else return (1);
				}
				else
				{
					if (g_stroemung.anzahl_felder == g_animation.anzahl_felder)
					{
						for (i = 0; i < g_stroemung.anzahl_felder; i++)
							if (g_stroemung.zeiten[i] != g_animation.zeiten[i])
								gleiche_felder = FALSE;
					}
					else
					{
						gleiche_felder = FALSE;
					}
					if (!gleiche_felder)
					{
						/* setze Abfrage zusammen ---------------------------------------------- */
						strcpy(c_hilf1, "Nach welcher Loesung soll animiert werden ?");
						strcpy(c_hilf2, "Stroemungsloesung");
						if (g_animation.art == 1)
							strcpy(c_hilf3, "Temperaturloesung");
						else if (g_animation.art == 2)
							strcpy(c_hilf3, "Spannungsloesung");
						if (!strcmp(ask_user(c_hilf1, c_hilf2, c_hilf3, NULL, 1),
									"Stroemungsloesung"))
						{
							g_animation.art = 3;
							if (g_animation.anzahl_felder != g_stroemung.anzahl_felder)
							{
								if (g_animation.zeiten = (float*)
										realloc(g_animation.zeiten,g_stroemung.anzahl_felder *
											sizeof(int)))
								{
									g_animation.anzahl_felder = g_stroemung.anzahl_felder;
									for (i = 0; i < g_stroemung.anzahl_felder; i++)
										g_animation.zeiten[i] = g_stroemung.zeiten[i];
									g_animation.anim_anfang = 0;
									g_animation.anim_ende   = i-1;
								}
								else g_animation.art = save_art;
							}
						}
					}
				}
			}
			g_animation.stroem_on = TRUE;
			break;
	}
	return (0);
/* ********************************************************************* */
}

/*
================================
 C3D_localminmax
 Searches local minimum and maximum 
 inside solution.
================================
*/
int C3D_localminmax(float *max, float *min)
{
	int i;
	*max = -INFINITE;
	*min =  INFINITE;
	for ( i = 0; i < g_geodat.npunkt; i++ )
	{
		*max = MAX(g_loesung.temp[i+1], *max);
		*min = MIN(g_loesung.temp[i+1], *min);
	}

	return (1);
}

/*
================================
 C3D_globminmax
 Searches global minimum and maximum
 inside the solution.
================================
*/
int C3D_globminmax(float *max, float *min)
{
	int i;
	float lmin, lmax;

	for ( i = 0; i < g_loesung.anzahl_felder; i++ )
	{
		C3D_localminmax(&lmax, &lmin);
		*max = MAX (lmax, *max);
		*min = MIN (lmin, *min);
	}

	return (1);
}

void destroy_results(void* list)
{
}

/*
===============================
 C3D_listresults
 List all read in results.
===============================
*/
void C3D_listresults_f(void)
{
	int i = 0;
	struct listelem *re;
	t_loesung *loes;
	t_loesdef *def;
	for ( re = g_results->head; re != NULL; re = re->next, i++ )
	{
		loes = (t_loesung*) re->data;
		def  = &(loes->definition); 
		printf( " \%s \"%s\"\n", def->text, def->einheit );
	}
}
