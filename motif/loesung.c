/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/* ******************************************************************** */
/* Vordefinition d. Loesungsparameter einer Auswahl moeglicher Loesungen */
/* Author : Helmut Vor, Access eV, Inzestrasse 5, 52070 Aachen ----- */
/* ******************************************************************** */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   10.03.97                                         */
/* Dateiname:   loesung.c                                        */
/*                                                               */
/* Routinenname    | Funktion                                    */
/* ------------------------------------------------------------- */
/* loesung_define  | setzt vordef. Parameter fuer Loesungen      */
/* init_loesstruct | setzt Loesungsstruktur nach einlesen        */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <malloc.h>
#include "errs.h"
#include "str.h"
#include "../graphics/common.h"
#include "csv.h"
#include "color3d.h"

enum{
  MAXFIELD = 1000, 
  CSVMAX = 100
};

enum{
  ENDUNG = 0,
  DESC,
  PARAMETER,
  EINHEIT
};
    
/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
static csvline *lresult = NULL;
/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */
char *result_strings( int ik, int jk )
{
  int i;
  csvline *p;
  for (p = lresult, i = 0; (p->next != NULL && i < ik); p = p->next, i++)
    ;
  return p->values[jk];
}

/* ******************************************************************** */
int loesung_define(void)
/* ******************************************************************** */
{
	int anzloesung;
	int nlines;
	char instdir[512];
	anzloesung = 5;
	anzloesungen = 62;

	/* Platz allockieren fuer Loesungsdefinition ---------------------- */
	if (g_loesdef != NULL) FREE (g_loesdef);
	if ((g_loesdef=(t_loesdef*) ALLOC(anzloesung * sizeof(t_loesdef)))==NULL)
		return -2;             /* Kein Platz ---------------------- */

	if (loesungen != NULL) FREE (loesungen);
	if ((loesungen = (t_loesdef *)ALLOC 
				(anzloesungen * sizeof(t_loesdef))) == NULL)
		return -2;             /* Kein Platz ---------------------- */

	/* read result strings from language file */
	if ( !castsroot )
		SYS_error( "Could not open CASTSROOT.\nSet CASTSROOT to your instalation.\n");
	else
	{
		sprintf( instdir,"%s", castsroot );

		strcat( instdir, "/resources/result_strings.En" );
		lresult = readcsv( lresult, instdir );
		if ( lresult == NULL ) {
			SYS_error( "Couldn't read result strings.\n Check if you have resources/result_strings.En in your distribution.\n" );
		}
	}

	strcpy(g_loesdef[0].dateiende,result_strings(0, ENDUNG));
	strcpy(g_loesdef[0].text,result_strings(0,DESC));
	strcpy(g_loesdef[0].parameter,result_strings(0, PARAMETER));
	strcpy(g_loesdef[0].einheit,result_strings(0,EINHEIT));
	g_loesdef[0].Min     = 0.0;
	g_loesdef[0].Max     = 2000.0;
	g_loesdef[0].schritt = 1.0;

	strcpy(loesungen[0].dateiende,result_strings(0,ENDUNG));
	strcpy(loesungen[0].text,result_strings(0,DESC));
	strcpy(loesungen[0].parameter,result_strings(0,PARAMETER));
	strcpy(loesungen[0].einheit,result_strings(0,EINHEIT));
	loesungen[0].Min     = 0.0;
	loesungen[0].Max     = 2000.0;
	loesungen[0].schritt = 1.0;
	strcpy(loesungen[1].dateiende,result_strings(1,ENDUNG));
	strcpy(loesungen[1].text,result_strings(1,DESC));
	strcpy(loesungen[1].parameter,result_strings(1,PARAMETER));
	strcpy(loesungen[1].einheit,result_strings(1,EINHEIT));
	loesungen[1].Min = -10.0;
	loesungen[1].Max = 100.0;
	loesungen[1].schritt = 0.05;
	strcpy(loesungen[2].dateiende,result_strings(2,ENDUNG));
	strcpy(loesungen[2].text,result_strings(2,DESC));
	strcpy(loesungen[2].parameter,result_strings(2,PARAMETER));
	strcpy(loesungen[2].einheit,result_strings(2,EINHEIT));
	loesungen[2].Min = -10.0;
	loesungen[2].Max = 100.0;
	loesungen[2].schritt = 0.05;
	strcpy(loesungen[3].dateiende,result_strings(3,ENDUNG));
	strcpy(loesungen[3].text,result_strings(3,DESC));
	strcpy(loesungen[3].parameter,result_strings(3,PARAMETER));
	strcpy(loesungen[3].einheit,result_strings(3,EINHEIT));
	loesungen[3].Min = 0.0;
	loesungen[3].Max = 100.0;
	loesungen[3].schritt = 0.1;
	strcpy(loesungen[4].dateiende,result_strings(4,ENDUNG));
	strcpy(loesungen[4].text,result_strings(4,DESC));
	strcpy(loesungen[4].parameter,result_strings(4,PARAMETER));
	strcpy(loesungen[4].einheit,result_strings(4,EINHEIT));
	loesungen[4].Min = 0.0;
	loesungen[4].Max = 100.0;
	loesungen[4].schritt = 0.1;
	strcpy(loesungen[5].dateiende,result_strings(5,ENDUNG));
	strcpy(loesungen[5].text,result_strings(5,DESC));
	strcpy(loesungen[5].parameter,result_strings(5,PARAMETER));
	strcpy(loesungen[5].einheit,result_strings(5,EINHEIT));
	loesungen[5].Min = 0.0;
	loesungen[5].Max = 100.0;
	loesungen[5].schritt = 0.1;
	strcpy(loesungen[6].dateiende,result_strings(6,ENDUNG));
	strcpy(loesungen[6].text,result_strings(6,DESC));
	strcpy(loesungen[6].parameter,result_strings(6,PARAMETER));
	strcpy(loesungen[6].einheit,result_strings(6,EINHEIT));
	loesungen[6].Min = -10.0;
	loesungen[6].Max = 100.0;
	loesungen[6].schritt = 0.05;
	strcpy(loesungen[7].dateiende,result_strings(7,ENDUNG));
	strcpy(loesungen[7].text,result_strings(7,DESC));
	strcpy(loesungen[7].parameter,result_strings(7,PARAMETER));
	strcpy(loesungen[7].einheit,result_strings(7,EINHEIT));
	loesungen[7].Min = 0.0;
	loesungen[7].Max = 2000.0;
	loesungen[7].schritt = 2.0;
	strcpy(loesungen[8].dateiende,result_strings(8,ENDUNG));
	strcpy(loesungen[8].text,result_strings(8,DESC));
	strcpy(loesungen[8].parameter,result_strings(8,PARAMETER));
	strcpy(loesungen[8].einheit,result_strings(8,EINHEIT));
	loesungen[8].Min = 0.0;
	loesungen[8].Max = 100.0;
	loesungen[8].schritt = 0.1;
	strcpy(loesungen[9].dateiende,result_strings(9,ENDUNG));
	strcpy(loesungen[9].text,result_strings(9,DESC));
	strcpy(loesungen[9].parameter,result_strings(9,PARAMETER));
	strcpy(loesungen[9].einheit,result_strings(9,EINHEIT));
	loesungen[9].Min = 0.0;
	loesungen[9].Max = 100000.0;
	loesungen[9].schritt = 10.0;
	strcpy(loesungen[10].dateiende,result_strings(10,ENDUNG));
	strcpy(loesungen[10].text,result_strings(10,DESC));
	strcpy(loesungen[10].parameter,result_strings(10,PARAMETER));
	strcpy(loesungen[10].einheit,result_strings(10,EINHEIT));
	loesungen[10].Min = 0.0;
	loesungen[10].Max = 100.0;
	loesungen[10].schritt = 0.1;
	strcpy(loesungen[11].dateiende,result_strings(11,ENDUNG));
	strcpy(loesungen[11].text,result_strings(11,DESC));
	strcpy(loesungen[11].parameter,result_strings(11,PARAMETER));
	strcpy(loesungen[11].einheit,result_strings(11,EINHEIT));
	loesungen[11].Min = -100.0;
	loesungen[11].Max = 100.0;
	loesungen[11].schritt = 0.1;
	strcpy(loesungen[12].dateiende,result_strings(12,ENDUNG));
	strcpy(loesungen[12].text,result_strings(12,DESC));
	strcpy(loesungen[12].parameter,result_strings(12,PARAMETER));
	strcpy(loesungen[12].einheit,result_strings(12,EINHEIT));
	loesungen[12].Min = -100.0;
	loesungen[12].Max = 100.0;
	loesungen[12].schritt = 0.1;
	strcpy(loesungen[13].dateiende,result_strings(13,ENDUNG));
	strcpy(loesungen[13].text,result_strings(13,DESC));
	strcpy(loesungen[13].parameter,result_strings(13,PARAMETER));
	strcpy(loesungen[13].einheit,result_strings(13,EINHEIT));
	loesungen[13].Min = -100.0;
	loesungen[13].Max = 100.0;
	loesungen[13].schritt = 0.1;
	strcpy(loesungen[14].dateiende,result_strings(14,ENDUNG));
	strcpy(loesungen[14].text,result_strings(14,DESC));
	strcpy(loesungen[14].parameter,result_strings(14,PARAMETER));
	strcpy(loesungen[14].einheit,result_strings(14,EINHEIT));
	loesungen[14].Min = -20000.0;
	loesungen[14].Max = 0.0;
	loesungen[14].schritt = 50;
	strcpy(loesungen[15].dateiende,result_strings(15,ENDUNG));
	strcpy(loesungen[15].text,result_strings(15,DESC));
	strcpy(loesungen[15].parameter,result_strings(15,PARAMETER));
	strcpy(loesungen[15].einheit,result_strings(15,EINHEIT));
	loesungen[15].Min = 0.0;
	loesungen[15].Max = 100.0;
	loesungen[15].schritt = 0.1;
	strcpy(loesungen[16].dateiende,result_strings(16,ENDUNG));
	strcpy(loesungen[16].text,result_strings(16,DESC));
	strcpy(loesungen[16].parameter,result_strings(16,PARAMETER));
	strcpy(loesungen[16].einheit,result_strings(16,EINHEIT));
	loesungen[16].Min = 0.0;
	loesungen[16].Max = 100.0;
	loesungen[16].schritt = 0.1;
	strcpy(loesungen[17].dateiende,result_strings(17,ENDUNG));
	strcpy(loesungen[17].text,result_strings(17,DESC));
	strcpy(loesungen[17].parameter,result_strings(17,PARAMETER));
	strcpy(loesungen[17].einheit,result_strings(17,EINHEIT));
	loesungen[17].Min = 0.0;
	loesungen[17].Max = 1.0;
	loesungen[17].schritt = 0.05;
	strcpy(loesungen[18].dateiende,result_strings(18,ENDUNG));
	strcpy(loesungen[18].text,result_strings(18,DESC));
	strcpy(loesungen[18].parameter,result_strings(18,PARAMETER));
	strcpy(loesungen[18].einheit,result_strings(18,EINHEIT));
	loesungen[18].Min = 0.0;
	loesungen[18].Max = 10000.0;
	loesungen[18].schritt = 10.0;
	strcpy(loesungen[19].dateiende,result_strings(19,ENDUNG));
	strcpy(loesungen[19].text,result_strings(19,DESC));
	strcpy(loesungen[19].parameter,result_strings(19,PARAMETER));
	strcpy(loesungen[19].einheit,result_strings(19,EINHEIT));
	loesungen[19].Min = 0.0;
	loesungen[19].Max = 250.0;
	loesungen[19].schritt = 1.0;
	strcpy(loesungen[20].dateiende,result_strings(20,ENDUNG));
	strcpy(loesungen[20].text,result_strings(20,DESC));
	strcpy(loesungen[20].parameter,result_strings(20,PARAMETER));
	strcpy(loesungen[20].einheit,result_strings(20,EINHEIT));
	loesungen[20].Min = -250.0;
	loesungen[20].Max = 250.0;
	loesungen[20].schritt = 1.0;
	strcpy(loesungen[21].dateiende,result_strings(21,ENDUNG));
	strcpy(loesungen[21].text,result_strings(21,DESC));
	strcpy(loesungen[21].parameter,result_strings(21,PARAMETER));
	strcpy(loesungen[21].einheit,result_strings(21,EINHEIT));
	loesungen[21].Min = -250.0;
	loesungen[21].Max = 250.0;
	loesungen[21].schritt = 1.0;
	strcpy(loesungen[22].dateiende,result_strings(22,ENDUNG));
	strcpy(loesungen[22].text,result_strings(22,DESC));
	strcpy(loesungen[22].parameter,result_strings(22,PARAMETER));
	strcpy(loesungen[22].einheit,result_strings(22,EINHEIT));
	loesungen[22].Min = -250.0;
	loesungen[22].Max = 250.0;
	loesungen[22].schritt = 1.0;
	strcpy(loesungen[23].dateiende,result_strings(23,ENDUNG));
	strcpy(loesungen[23].text,result_strings(23,DESC));
	strcpy(loesungen[23].parameter,result_strings(23,PARAMETER));
	strcpy(loesungen[23].einheit,result_strings(23,EINHEIT));
	loesungen[23].Min = 0.0;
	loesungen[23].Max = 250.0;
	loesungen[23].schritt = 1.0;
	strcpy(loesungen[24].dateiende,result_strings(24,ENDUNG));
	strcpy(loesungen[24].text,result_strings(24,DESC));
	strcpy(loesungen[24].parameter,result_strings(24,PARAMETER));
	strcpy(loesungen[24].einheit,result_strings(24,EINHEIT));
	loesungen[24].Min = -250.0;
	loesungen[24].Max = 250.0;
	loesungen[24].schritt = 1.0;
	strcpy(loesungen[25].dateiende,result_strings(25,ENDUNG));
	strcpy(loesungen[25].text,result_strings(25,DESC));
	strcpy(loesungen[25].parameter,result_strings(25,PARAMETER));
	strcpy(loesungen[25].einheit,result_strings(25,EINHEIT));
	loesungen[25].Min = -250.0;
	loesungen[25].Max = 250.0;
	loesungen[25].schritt = 1.0;
	strcpy(loesungen[26].dateiende,result_strings(26,ENDUNG));
	strcpy(loesungen[26].text,result_strings(26,DESC));
	strcpy(loesungen[26].parameter,result_strings(26,PARAMETER));
	strcpy(loesungen[26].einheit,result_strings(26,EINHEIT));
	loesungen[26].Min = -250.0;
	loesungen[26].Max = 250.0;
	loesungen[26].schritt = 1.0;
	strcpy(loesungen[27].dateiende,result_strings(27,ENDUNG));
	strcpy(loesungen[27].text,result_strings(27,DESC));
	strcpy(loesungen[27].parameter,result_strings(27,PARAMETER));
	strcpy(loesungen[27].einheit,result_strings(27,EINHEIT));
	loesungen[27].Min = 0.0;
	loesungen[27].Max = 100.0;
	loesungen[27].schritt = 0.1;
	strcpy(loesungen[28].dateiende,result_strings(28,ENDUNG));
	strcpy(loesungen[28].text,result_strings(28,DESC));
	strcpy(loesungen[28].parameter,result_strings(28,PARAMETER));
	strcpy(loesungen[28].einheit,result_strings(28,EINHEIT));
	loesungen[28].Min = -20.0;
	loesungen[28].Max = 10.0;
	loesungen[28].schritt = 0.05;
	strcpy(loesungen[29].dateiende,result_strings(29,ENDUNG));
	strcpy(loesungen[29].text,result_strings(29,DESC));
	strcpy(loesungen[29].parameter,result_strings(29,PARAMETER));
	strcpy(loesungen[29].einheit,result_strings(29,EINHEIT));
	loesungen[29].Min = 0.0;
	loesungen[29].Max = 100.0;
	loesungen[29].schritt = 0.1;
	strcpy(loesungen[30].dateiende,result_strings(30,ENDUNG));
	strcpy(loesungen[30].text,result_strings(30,DESC));
	strcpy(loesungen[30].parameter,result_strings(30,PARAMETER));
	strcpy(loesungen[30].einheit,result_strings(30,EINHEIT));
	loesungen[30].Min = -100.0;
	loesungen[30].Max = 100.0;
	loesungen[30].schritt = 0.1;
	strcpy(loesungen[31].dateiende,result_strings(31,ENDUNG));
	strcpy(loesungen[31].text,result_strings(31,DESC));
	strcpy(loesungen[31].parameter,result_strings(31,PARAMETER));
	strcpy(loesungen[31].einheit,result_strings(31,EINHEIT));
	loesungen[31].Min = -100.0;
	loesungen[31].Max = 100.0;
	loesungen[31].schritt = 0.01;
	strcpy(loesungen[32].dateiende,result_strings(32,ENDUNG));
	strcpy(loesungen[32].text,result_strings(32,DESC));
	strcpy(loesungen[32].parameter,result_strings(32,PARAMETER));
	strcpy(loesungen[32].einheit,result_strings(32,EINHEIT));
	loesungen[32].Min = -100.0;
	loesungen[32].Max = 100.0;
	loesungen[32].schritt = 0.1;
	strcpy(loesungen[33].dateiende,result_strings(33,ENDUNG));
	strcpy(loesungen[33].text,result_strings(33,DESC));
	strcpy(loesungen[33].parameter,result_strings(33,PARAMETER));
	strcpy(loesungen[33].einheit,result_strings(33,EINHEIT));
	loesungen[33].Min = -2.0;
	loesungen[33].Max = 10.0;
	loesungen[33].schritt = 0.05;
	strcpy(loesungen[34].dateiende,result_strings(34,ENDUNG));
	strcpy(loesungen[34].text,result_strings(34,DESC));
	strcpy(loesungen[34].parameter,result_strings(34,PARAMETER));
	strcpy(loesungen[34].einheit,result_strings(34,EINHEIT));
	loesungen[34].Min = 0.0;
	loesungen[34].Max = 1000.0;
	loesungen[34].schritt = 0.1;
	strcpy(loesungen[35].dateiende,result_strings(35,ENDUNG));
	strcpy(loesungen[35].text,result_strings(35,DESC));
	strcpy(loesungen[35].parameter,result_strings(35,PARAMETER));
	strcpy(loesungen[35].einheit,result_strings(35,EINHEIT));
	loesungen[35].Min = -10.0;
	loesungen[35].Max = 100.0;
	loesungen[35].schritt = 0.05;
	strcpy(loesungen[36].dateiende,result_strings(36,ENDUNG));
	strcpy(loesungen[36].text,result_strings(36,DESC));
	strcpy(loesungen[36].parameter,result_strings(36,PARAMETER));
	strcpy(loesungen[36].einheit,result_strings(36,EINHEIT));
	loesungen[36].Min = -2000.0;
	loesungen[36].Max = 2000.0;
	loesungen[36].schritt = 0.5;
	strcpy(loesungen[37].dateiende,result_strings(37,ENDUNG));
	strcpy(loesungen[37].text,result_strings(37,DESC));
	strcpy(loesungen[37].parameter,result_strings(37,PARAMETER));
	strcpy(loesungen[37].einheit,result_strings(37,EINHEIT));
	loesungen[37].Min = -2000.0;
	loesungen[37].Max = 2000.0;
	loesungen[37].schritt = 0.5;
	strcpy(loesungen[38].dateiende,result_strings(38,ENDUNG));
	strcpy(loesungen[38].text,result_strings(38,DESC));
	strcpy(loesungen[38].parameter,result_strings(38,PARAMETER));
	strcpy(loesungen[38].einheit,result_strings(38,EINHEIT));
	loesungen[38].Min = -2000.0;
	loesungen[38].Max = 2000.0;
	loesungen[38].schritt = 0.5;
	strcpy(loesungen[39].dateiende,result_strings(39,ENDUNG));
	strcpy(loesungen[39].text,result_strings(39,DESC));
	strcpy(loesungen[39].parameter,result_strings(39,PARAMETER));
	strcpy(loesungen[39].einheit,result_strings(39,EINHEIT));
	loesungen[39].Min = -20000.0;
	loesungen[39].Max = 20000.0;
	loesungen[39].schritt = 0.5;
	strcpy(loesungen[40].dateiende,result_strings(40,ENDUNG));
	strcpy(loesungen[40].text,result_strings(40,DESC));
	strcpy(loesungen[40].parameter,result_strings(40,PARAMETER));
	strcpy(loesungen[40].einheit,result_strings(40,EINHEIT));
	loesungen[40].Min = -20000.0;
	loesungen[40].Max = 20000.0;
	loesungen[40].schritt = 0.5;
	strcpy(loesungen[41].dateiende,result_strings(41,ENDUNG));
	strcpy(loesungen[41].text,result_strings(41,DESC));
	strcpy(loesungen[41].parameter,result_strings(41,PARAMETER));
	strcpy(loesungen[41].einheit,result_strings(41,EINHEIT));
	loesungen[41].Min = -20000.0;
	loesungen[41].Max = 20000.0;
	loesungen[41].schritt = 0.5;
	strcpy(loesungen[42].dateiende,result_strings(42,ENDUNG));
	strcpy(loesungen[42].text,result_strings(42,DESC));
	strcpy(loesungen[42].parameter,result_strings(42,PARAMETER));
	strcpy(loesungen[42].einheit,result_strings(42,EINHEIT));
	loesungen[42].Min = -20000.0;
	loesungen[42].Max = 20000.0;
	loesungen[42].schritt = 0.5;
	strcpy(loesungen[43].dateiende,result_strings(43,ENDUNG));
	strcpy(loesungen[43].text,result_strings(43,DESC));
	strcpy(loesungen[43].parameter,result_strings(43,PARAMETER));
	strcpy(loesungen[43].einheit,result_strings(43,EINHEIT));
	loesungen[43].Min = 0.0;
	loesungen[43].Max = 10000000.0;
	loesungen[43].schritt = 2500.0;
	strcpy(loesungen[44].dateiende,result_strings(44,ENDUNG));
	strcpy(loesungen[44].text,result_strings(44,DESC));
	strcpy(loesungen[44].parameter,result_strings(44,PARAMETER));
	strcpy(loesungen[44].einheit,result_strings(44,EINHEIT));
	loesungen[44].Min = 0.0;
	loesungen[44].Max = 10.0;
	loesungen[44].schritt = 0.05;
	strcpy(loesungen[45].dateiende,result_strings(45,ENDUNG));
	strcpy(loesungen[45].text,result_strings(45,DESC));
	strcpy(loesungen[45].parameter,result_strings(45,PARAMETER));
	strcpy(loesungen[45].einheit,result_strings(45,EINHEIT));
	loesungen[45].Min = 0.0;
	loesungen[45].Max = 40.0;
	loesungen[45].schritt = 1;
	strcpy(loesungen[46].dateiende,result_strings(46,ENDUNG));
	strcpy(loesungen[46].text,result_strings(46,DESC));
	strcpy(loesungen[46].parameter,result_strings(46,PARAMETER));
	strcpy(loesungen[46].einheit,result_strings(46,EINHEIT));
	loesungen[46].Min = -1.0;
	loesungen[46].Max = 2.0;
	loesungen[46].schritt = 1;
	strcpy(loesungen[47].dateiende,result_strings(47,ENDUNG));
	strcpy(loesungen[47].text,result_strings(47,DESC));
	strcpy(loesungen[47].parameter,result_strings(47,PARAMETER));
	strcpy(loesungen[47].einheit,result_strings(47,EINHEIT));
	loesungen[47].Min = -10.0;
	loesungen[47].Max = 30000.0;
	loesungen[47].schritt = 1;
	strcpy(loesungen[48].dateiende,result_strings(48,ENDUNG));
	strcpy(loesungen[48].text,result_strings(48,DESC));
	strcpy(loesungen[48].parameter,result_strings(48,PARAMETER));
	strcpy(loesungen[48].einheit,result_strings(48,EINHEIT));
	loesungen[48].Min = 0.0;
	loesungen[48].Max = 100.0;
	loesungen[48].schritt = 0.01;
	strcpy(loesungen[49].dateiende,result_strings(49,ENDUNG));
	strcpy(loesungen[49].text,result_strings(49,DESC));
	strcpy(loesungen[49].parameter,result_strings(49,PARAMETER));
	strcpy(loesungen[49].einheit,result_strings(49,EINHEIT));
	loesungen[49].Min = 0.0;
	loesungen[49].Max = 10.0;
	loesungen[49].schritt = 0.01;
	strcpy(loesungen[50].dateiende,result_strings(50,ENDUNG));
	strcpy(loesungen[50].text,result_strings(50,DESC));
	strcpy(loesungen[50].parameter,result_strings(50,PARAMETER));
	strcpy(loesungen[50].einheit,result_strings(50,EINHEIT));
	loesungen[50].Min = 0.0;
	loesungen[50].Max = 100.0;
	loesungen[50].schritt = 0.1;
	strcpy(loesungen[51].dateiende,result_strings(51,ENDUNG));
	strcpy(loesungen[51].text,result_strings(51,DESC));
	strcpy(loesungen[51].parameter,result_strings(51,PARAMETER));
	strcpy(loesungen[51].einheit,result_strings(51,EINHEIT));
	loesungen[51].Min = 0.0;
	loesungen[51].Max = 0.0;
	loesungen[51].schritt = 0.0;
	strcpy(loesungen[52].dateiende,result_strings(52,ENDUNG));
	strcpy(loesungen[52].text,result_strings(52,DESC));
	strcpy(loesungen[52].parameter,result_strings(52,PARAMETER));
	strcpy(loesungen[52].einheit,result_strings(52,EINHEIT));
	loesungen[52].Min = 0.0;
	loesungen[52].Max = 0.0;
	loesungen[52].schritt = 0.0;
	strcpy(loesungen[53].dateiende,result_strings(53,ENDUNG));
	strcpy(loesungen[53].text,result_strings(53,DESC));
	strcpy(loesungen[53].parameter,result_strings(53,PARAMETER));
	strcpy(loesungen[53].einheit,result_strings(53,EINHEIT));
	loesungen[53].Min = 0.0;
	loesungen[53].Max = 0.0;
	loesungen[53].schritt = 0.0;
	strcpy(loesungen[54].dateiende,result_strings(54,ENDUNG));
	strcpy(loesungen[54].text,result_strings(54,DESC));
	strcpy(loesungen[54].parameter,result_strings(54,PARAMETER));
	strcpy(loesungen[54].einheit,result_strings(54,EINHEIT));
	loesungen[54].Min = 0.0;
	loesungen[54].Max = 0.0;
	loesungen[54].schritt = 0.0;
	strcpy(loesungen[55].dateiende,result_strings(55,ENDUNG));
	strcpy(loesungen[55].text,result_strings(55,DESC));
	strcpy(loesungen[55].parameter,result_strings(55,PARAMETER));
	strcpy(loesungen[55].einheit,result_strings(55,EINHEIT));
	loesungen[55].Min = 0.0;
	loesungen[55].Max = 0.0;
	loesungen[55].schritt = 0.0;
	strcpy(loesungen[56].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[56].text,result_strings(56,DESC));
	strcpy(loesungen[56].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[56].einheit,result_strings(56,EINHEIT));
	loesungen[56].Min = 0.0;
	loesungen[56].Max = 0.0;
	loesungen[56].schritt = 0.0;
	strcpy(loesungen[57].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[57].text,result_strings(56,DESC));
	strcpy(loesungen[57].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[57].einheit,result_strings(56,EINHEIT));
	loesungen[57].Min = 0.0;
	loesungen[57].Max = 0.0;
	loesungen[57].schritt = 0.0;
	strcpy(loesungen[58].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[58].text,result_strings(56,DESC));
	strcpy(loesungen[58].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[58].einheit,result_strings(56,EINHEIT));
	loesungen[58].Min = 0.0;
	loesungen[58].Max = 0.0;
	loesungen[58].schritt = 0.0;
	strcpy(loesungen[59].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[59].text,result_strings(56,DESC));
	strcpy(loesungen[59].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[59].einheit,result_strings(56,EINHEIT));
	loesungen[59].Min = 0.0;
	loesungen[59].Max = 0.0;
	loesungen[59].schritt = 0.0;
	strcpy(loesungen[60].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[60].text,result_strings(56,DESC));
	strcpy(loesungen[60].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[60].einheit,result_strings(56,EINHEIT));
	loesungen[60].Min = 0.0;
	loesungen[60].Max = 0.0;
	loesungen[60].schritt = 0.0;
	strcpy(loesungen[61].dateiende,result_strings(56,ENDUNG));
	strcpy(loesungen[61].text,result_strings(56,DESC));
	strcpy(loesungen[61].parameter,result_strings(56,PARAMETER));
	strcpy(loesungen[61].einheit,result_strings(56,EINHEIT));
	loesungen[61].Min = 0.0;
	loesungen[61].Max = 0.0;
	loesungen[61].schritt = 0.0;

	return 1;
}

/* ********************************************************************* */
void init_loesstruct(void)
/* ********************************************************************* */
{
  char hilfstr[256];
  char endung[10];
  int loesindex, string_laenge, i;
  char *pos_punkt;
  int anzloesung;

  anzloesung = 5; 
  loesindex = -1;

  strcpy(hilfstr, g_loesung.dateiname);
  if ((pos_punkt = strrchr(hilfstr, '.')) != NULL)
  {
    strncpy(endung, pos_punkt, 10);
    
    for (i = 0; i < anzloesung; i++)
      if (!strcmp(g_loesdef[i].dateiende, endung)) loesindex = i; 
    if ((loesindex > -1) && (loesindex < anzloesung))
    {
      strcpy(g_loesung.ausgabe, g_loesdef[loesindex].text);
      strcpy(g_loesung.parameter, g_loesdef[loesindex].parameter);
      strcpy(g_loesung.einheit, g_loesdef[loesindex].einheit);
      g_loesung.Min     = g_loesdef[loesindex].Min;
      g_loesung.Max     = g_loesdef[loesindex].Max;
      g_loesung.schritt = g_loesdef[loesindex].schritt;
      g_loesung.typ_gesetzt = TRUE;
    }
  }
  return;
}










