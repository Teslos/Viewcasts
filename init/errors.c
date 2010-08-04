/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Toni Ivas & Helmut Vor                           */
/* Datum    :   17.10.02                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/* error        : handles global errors                          */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */ 
#include <stdio.h>
#include "errs.h"

void error(int nmes);

void 
error(int  nmes)
{
  fprintf(stderr, "Color3d Error: %s\n", errs[nmes]);
}

/* machine generated do not edit */

char *errs[] = { 
	"Premission denied", /* Eprem */
	"I/O error", /* Eio */
	"File does not exist", /* Efile */
	"Memory limit reached", /* Enomem */
	"Out of file space", /* Espace */
	"Error in getting resources", /* Erlimit */
	"Can't write n-bytes", /* Ewriten */
	"Can't read n-bytes", /* Ereadn */
	"Resources does not exist", /* Elimit */
	"File can't be map in memory", /* Emmap */
	"File stat error", /* Estat */
};
