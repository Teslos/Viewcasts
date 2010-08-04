/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */
/* Adding memory allocation from cii library */
/* csv.h: interface for csv library */
#include "mem.h"                  /* saffer memory functions */
typedef struct csvline csvline;
struct csvline {
   int ncsvvalues;                /* number of values */
   char **values;                 /* values in csv */
   char *line;                    /* complete line */   
   csvline *next;                 /* to next line */
};
extern char *csvgetline(FILE *f); /* read next input line */
extern char *csvfield(int n);	  /* return field n */
extern int csvnfield(void);	  /* return number of fields */
extern csvline *readcsv(csvline *listp, char *csvname);


