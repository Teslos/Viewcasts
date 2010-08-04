#ifndef _read_unv_h
#define _read_unv_h

/******************************************************************************/

#include <stdio.h>

/******************************************************************************/
extern int n_datasets;		/* Anzahl Universalfile-Datasets */
int read_unv(char *dateiname);
int unv_rd_dskennung(FILE *datei);
int unv_rd_skip(FILE *datei);
int unv_rd_skip_nlines(FILE *datei, int nskip);
char *get_datatyp_name(int typ_nr);

int unv_rd_unit(FILE *datei);

int unv_rd_node(FILE *datei, int ds_id);
int unv_rd_node_dp(FILE *datei, int ds_id);

int unv_rd_element(FILE *datei, int ds_id);
int unv_rd_pid(FILE *datei, int ds_id);
int unv_rd_mid(FILE *datei, int ds_id);

/******************************************************************************/
#endif   /* #ifndef _read_unv_h */
