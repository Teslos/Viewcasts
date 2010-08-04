/**************************************************************************
 This file contains routines from toolCheckLicenseF.f90
 file, rewritten in C.
 
 Author: TI & PS
***************************************************************************/
/* ====================================================================== */
/* Changes                                                                */
/* ====================================================================== */
/* Written:                                                               */
/* 07.11.02    PS                                                         */
/* Modifications:                                                         */
/* 17.01.03    PS    Enable various 'features' (limited versions)         */
/* 28.02.03    PS    MODULE_IO: Ausgabe                                   */
/* 17.07.03    PS    'MICRESS_' in feature 'hidden' in                    */
/*                   'toolCheckLicenseFCallC' to try and mask the string  */
/*                   (for safety reasons: so as to make it more difficult */
/*                   to change it in the binary).                         */
/* 16.10.03    PS    'Multi-feature' license.                             */ 
/* 04.11.03    PS    Use FLEXlm 'PLATFORMS' feature.                      */
/* 10.11.03    PS    Implement FLEXlm 'manual heartbeat'.                 */
/* 01.12.03    PS    Simplified for CASTS implementation.                 */

#include "toolCheckLicense.h"
#include "../config.h"
#define TRUE   1
#define FALSE  0

int toolCheckLicenseF ( int flag )
{
#if FLEXLM == EIN
  int License_Found;
  int Exit_Code;

  char Feature[] = "CASTS_PROCESS";
  char Version[] = "14.1";

  /* Check if a license is available */
  switch ( flag )
    {
      /* Check default license paths */
    case 1:
      License_Found = toolCheckLicense( 1, Feature, Version );
      if ( !License_Found )
	{
	  printf("Unable to find a valid CASTS license\n");
	  printf("Please contact your ACCESS partner\n");
	  exit(1);
	}
      break;
   
    case 2:
      if ( !toolCheckLicense( 2, Feature, Version ) ) 
	{
	  printf("Unable to re-check CASTS license \n");
	  printf("Please contact your ACCESS partner \n");
	  exit(1);
	}
      break;
    
    case 3:
      Exit_Code = toolCheckLicense( 3, Feature, Version );
      break;
      
    default:
      printf ("Coding error: toolCheckLicenseF\n called with unrecognised parameters\n");
      exit(1);
    }
#endif
  return 0;
}

#if FLEXLM == EIN

int toolCheckLicense( int Flag, char *Feature, char *Version )
{

  int Exit_Code;
  int Length;
  
  char Path[] = "@localhost";

  /* Check out */
  if ( Flag == 1 )
    {
      if ( Feature != NULL && Version != NULL ) 
	{
	  Exit_Code = toolCheckLicenseC( 1, Feature, Version, Path );
	}
      else
	{
	  printf("Warning!: routine toolCheckLicense \n called with unrecognised parameters\n");
	}
    }
  else  /* Check in Heatbeat */
    {
      Exit_Code = toolCheckLicenseC( Flag, Feature, Version, Path );
    }
    
  return (( Exit_Code == 0 ) ? TRUE : FALSE);  
}

#endif
