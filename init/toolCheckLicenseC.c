/*
ToolCheckLicenseC: routine to perform actual calls to FLEXlm library.
Written:
  07.11.02    PS, TI     Adapted from FLEXlm example.
Modifications:
  17.01.03    PS         Enable various 'features' (limited versions).
  16.10.03    PS, TI     'Multi-feature' license.
  04.11.03    PS         Set 'platform' attribute.
  10.11.03    PS         Perform manual 'heartbeats'.
  29.12.03    TI         Remove unnecessary parameters from C function.
*/
#if FLEXLM == EIN
#include "lmclient.h"
#include "lm_attr.h"
#endif /* FLEXlm */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"
/* Set 'Verbose' flag */
// #define VERBOSE
/* Set 'Debug' flag */
// #define DEBUG
/*
======================================================================
Check License
======================================================================
*/
/* Hand over strings as well as their length (necessary under Irix!),
1 to check in and 0 to check out, returns 0 if OK, 666 otherwise */
int toolCheckLicenseC( unsigned int Flag,
                         char *Feature,
                         char *Version,
                         char *License )
{
/* FLEXlm stuff */
#define CHECK_OUT       1
#define CHECK_HEARTBEAT 2
#define CHECK_IN        3
#if FLEXLM == EIN
    struct vendorcode7 code;
    LM_HANDLE *lm_job;
#define MAX_NUMBER_OF_LM_JOBS 64
    static int number_of_lm_jobs = 0;
    static LM_HANDLE *checked_out_lm_jobs[MAX_NUMBER_OF_LM_JOBS];
    int no_job = 0;
    int no_license = 0;
    int flag = 0;
    int heartbeat_ok, num_reconnects, num_minutes;
#define STRING_SIZE 256
    static char checked_out_features[MAX_NUMBER_OF_LM_JOBS][STRING_SIZE];
    int current_job = 0;
#ifdef VERBOSE
    CONFIG *pos = 0;
    CONFIG *conf;
    int days = 0;
#endif /* VERBOSE */
#endif /* FLEXlm */
/* Declare return value */
#define RETURN_ERROR 666
    int return_value = RETURN_ERROR;
/* Allocate and copy strings (...) */
    char *feature, *version, *license;
    feature = (char*)malloc(sizeof(char)*(strlen(Feature)+1));
    strcpy ( feature, Feature );
    version = (char*)malloc(sizeof(char)*(strlen(Version)+1));
    strcpy ( version, Version );
    license = (char*)malloc(sizeof(char)*(strlen(License)+1));
    strcpy ( license, License );
/* Check allocation */
    if ( ( feature == NULL ) || ( version == NULL ) || ( license == NULL ) ) {
        printf ( strcat( "\nError in routine 'toolCheckLicenseC',",
                         " unable to allocate buffers\n\n" ) );
        exit(RETURN_ERROR);
    }
/* Actual call */
#if FLEXLM == EIN
    flag = Flag;
/* Check out */
    if ( flag == CHECK_OUT ) {
        no_job = lc_new_job ( 0, 0, &code, &lm_job );
        if ( no_job ) {
            printf ( "\nUnable to allocate 'new job'\n\n%s\n\n",
                     lc_errstring(lm_job));
            lc_perror(lm_job,"lc_new_job failed" );
            exit(RETURN_ERROR);
        }
        lc_set_attr ( lm_job, LM_A_CHECK_INTERVAL, (LM_A_VAL_TYPE) -1 );
        lc_set_attr ( lm_job, LM_A_RETRY_INTERVAL, (LM_A_VAL_TYPE) -1 ); 
        lc_set_attr ( lm_job, LM_A_RETRY_COUNT, (LM_A_VAL_TYPE) 2 ); 
        lc_set_attr ( lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)license );
        no_license = lc_checkout ( lm_job, feature, version, 1,
                                   LM_CO_NOWAIT, &code, LM_DUP_NONE );
/* Store job specifics (for future use: 'heartbeat', and release) */
        if ( ! no_license ) {
            return_value = 0;
            if ( ( number_of_lm_jobs < MAX_NUMBER_OF_LM_JOBS ) &&
                 ( strlen(feature) < STRING_SIZE ) ) {
                checked_out_lm_jobs[number_of_lm_jobs] = lm_job;
                strcpy ( checked_out_features[number_of_lm_jobs], feature );
                number_of_lm_jobs++ ;
            } else {
                printf ( "\nWarning: dimensionality problem in routine toolCheckLicenseC!" );
                printf ( "\n Number of 'jobs': %d (maximum number: %d)",
                         number_of_lm_jobs, MAX_NUMBER_OF_LM_JOBS );
                printf ( "\n String length: %d (maximum length: %d)\n\n",
                         strlen(feature), STRING_SIZE );
            };
/* Get chatty */
#ifdef VERBOSE
            printf ( "\n%s\n%s\n%s\n%s\n\n", feature, version, platform,
                     license );
            conf = lc_next_conf ( lm_job, feature, &pos ); 
            days = lc_expire_days ( lm_job, conf );
            if ( days == LM_FOREVER ) {
                printf ( "\nPermanent license\n\n" );
            } else {
                printf ( "\nLicense expires in %d day(s)\n\n", days );
            }
#endif /* VERBOSE */
        } else { /* (NB: no license found!) */
#ifdef VERBOSE
            printf ( "\n%s\n", lc_errstring(lm_job) );
#endif /* VERBOSE */
            lc_free_job ( lm_job );
        }
/* 'Heartbeat' (i.e.: check whether the license is still available)*/
    } else if ( flag == CHECK_HEARTBEAT ) {
        return_value = 0;
        for ( current_job = 0; current_job < number_of_lm_jobs;
            current_job ++ ) {
#ifdef VERBOSE
            printf ( "\n'Heartbeat' for feature: %s\n\n",
                     checked_out_features[current_job] );
#endif /* VERBOSE */
            num_reconnects = 1;
            num_minutes = 60;
            heartbeat_ok = lc_heartbeat ( checked_out_lm_jobs[current_job],
                                          &num_reconnects, num_minutes );
            if ( heartbeat_ok != 0 ) {
                return_value = RETURN_ERROR;
#ifdef VERBOSE
                printf ( "\n%s\n",
                         lc_errstring(checked_out_lm_jobs[current_job]) );
#endif /* VERBOSE */
            };
        } 
/* Check out (i.e.: 'release the licenses') */
    } else if ( flag == CHECK_IN ) {
        return_value = 0;
        for ( current_job = 0; current_job < number_of_lm_jobs;
              current_job ++ ) {
#ifdef VERBOSE
            printf ( "\nChecked out feature: %s\n\n",
                     checked_out_features[current_job] );
#endif /* VERBOSE */
            lc_checkin ( checked_out_lm_jobs[current_job],
                         checked_out_features[current_job], 0 );
            lc_free_job ( checked_out_lm_jobs[current_job] );
        }
/* Wrong call parameter! */
    } else {
        return_value = RETURN_ERROR;
#ifdef VERBOSE
        printf ( strcat ( "\nRoutine 'toolCheckLicenseC' called",
                          " with unrecognised parameters\n\n" ) );
#endif /* VERBOSE */
    }
#ifdef DEBUG
    printf ( "\nWarning: debug!\n\n%s\n%s\n%s\n%s\n\n", feature, version,
                                                        platform, license );
    return_value = RETURN_ERROR;
#endif /* DEBUG */
#endif /* FLEXlm */
/* That's all folks! */
  return return_value;
}
