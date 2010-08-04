/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "csv.h"

enum { NOMEM = -2 };          /* out of memory signal */

static char *line    = NULL;  /* input chars */
static char *sline   = NULL;  /* line copy used by split */
static int  maxline  = 0;     /* size of line[] and sline[] */
static char **field  = NULL;  /* field pointers */
static int  maxfield = 0;     /* size of field[] */
static int  nfield   = 0;     /* number of fields in field[] */

static char fieldsep[] = ","; /* field separator chars */

static char *advquoted(char *);
static int split(void);

/* endofline: check for and consume \r, \n, \r\n, or EOF */
static int endofline(FILE *fin, int c)
{
	int eol;

	eol = (c=='\r' || c=='\n');
	if (c == '\r') {
		c = getc(fin);
		if (c != '\n' && c != EOF)
			ungetc(c, fin);	/* read too far; put c back */
	}
	return eol;
}

/* reset: set variables back to starting values */
static void reset(void)
{
	FREE(line);	/* free(NULL) permitted by ANSI C */
	FREE(sline);
	FREE(field);
	line = NULL;
	sline = NULL;
	field = NULL;
	maxline = maxfield = nfield = 0;
}

/* csvgetline:  get one line, grow as needed */
/* sample input: "LU",86.25,"11/4/1998","2:19PM",+4.0625 */
char *csvgetline(FILE *fin)
{	
	int i, c;
	char *newl, *news;

	if (line == NULL) {			/* allocate on first call */
		maxline = maxfield = 1;
		line = (char *) ALLOC(maxline);
		sline = (char *) ALLOC(maxline);
		field = (char **) ALLOC(maxfield*sizeof(field[0]));
		if (line == NULL || sline == NULL || field == NULL) {
			reset();
			return NULL;		/* out of memory */
		}
	}
	for (i=0; ((c=fgetc(fin)) && !feof(fin) && !endofline(fin,c)); i++) {
		if (i >= maxline-1) {	/* grow line */
			maxline *= 2;		/* double current size */
			newl = (char *) RESIZE(line, maxline);
			if (newl == NULL) {
				reset();
				return NULL;
			}
			line = newl;
			news = (char *) RESIZE(sline, maxline);
			if (news == NULL) {
				reset();
				return NULL;
			}
			sline = news;


		}
		line[i] = c;
	}
	line[i] = '\0';
	if (split() == NOMEM) {
		reset();
		return NULL;			/* out of memory */
	}
	return (c == EOF && i == 0) ? NULL : line;
}

/* split: split line into fields */
static int split(void)
{
	char *p, **newf;
	char *sepp; /* pointer to temporary separator character */
	int sepc;   /* temporary separator character */

	nfield = 0;
	if (line[0] == '\0')
		return 0;
	strcpy(sline, line);
	p = sline;

	do {
		if (nfield >= maxfield) {
			maxfield *= 2;			/* double current size */
			newf = (char **) RESIZE(field, 
						maxfield * sizeof(field[0]));
			if (newf == NULL)
				return NOMEM;
			field = newf;
		}
		if (*p == '"')
			sepp = advquoted(++p);	/* skip initial quote */
		else
			sepp = p + strcspn(p, fieldsep);
		sepc = sepp[0];
		sepp[0] = '\0';				/* terminate field */
		field[nfield++] = p;
		p = sepp + 1;
	} while (sepc == ',');

	return nfield;
}

/* advquoted: quoted field; return pointer to next separator */
static char *advquoted(char *p)
{
	int i, j;

	for (i = j = 0; p[j] != '\0'; i++, j++) {
		if (p[j] == '"' && p[++j] != '"') {
			/* copy up to next separator or \0 */
			int k = strcspn(p+j, fieldsep);
			memmove(p+i, p+j, k);
			i += k;
			j += k;
			break;
		}
		p[i] = p[j];
	}
	p[i] = '\0';
	return p + j;
}

/* csvfield:  return pointer to n-th field */
char *csvfield(int n)
{
	if (n < 0 || n >= nfield)
		return NULL;
	return field[n];
}

/* csvnfield:  return number of fields */ 
int csvnfield(void)
{
	return nfield;
}

/* creates new item of struct csvline */
static csvline *newitem(char *line, int nstrings)
{
	csvline *newp;
    newp = (csvline *) ALLOC(sizeof(csvline));
    newp->line = (char*) ALLOC((strlen(line)+1)*sizeof(line[0]));
    strcpy(newp->line, line);
    newp->ncsvvalues = nstrings;
    newp->values = (char**) ALLOC(nstrings*sizeof(char*));
    newp->next = NULL;
    return newp;
}

/* addend: adds to end of linked list */
static csvline *addend(csvline *listp, csvline *elem)
{
  csvline *p;
   
  if (listp == NULL)
    return elem;
  for (p = listp; p->next != NULL; p = p->next) 
    ;
  p->next = elem;
  elem->next = NULL;
  return listp;
}

/* readcsv: reads csv in linked list */
csvline *readcsv(csvline *listp, char *csvname)
{
  FILE *fp;
  int i;
  char *line;
  csvline *csvl;

  fp = fopen(csvname,"r");
  if (fp == NULL)
    fprintf(stderr,"Can't open file %s\n", csvname);
  while((line = csvgetline(fp)) != NULL) {
    if (csvnfield() > 0)
      csvl = newitem(line, csvnfield());
    for (i = 0; i < csvnfield(); i++)
      csvl->values[i] = strdup(csvfield(i));
    listp = addend(listp, csvl);
  }
  fclose(fp);
  return listp;
}

static void printnv(csvline *p, void *arg)
{
  int i;
  char *fmt;
  fmt = (char*) arg;
  printf(fmt, p->line);
  for (i = 0; i < p->ncsvvalues; i++)
    printf(fmt, p->values[i]);
}

static void apply(csvline *listp, 
		  void (*fn)(csvline *, void *), void *arg)
{
  for ( ; listp != NULL; listp = listp->next) 
    (*fn)(listp, arg);
}

/*
int main(int argc, char *argv[])
{
  csvline *listp = NULL;
  if (argc < 2) {
    printf("Usage: csv [file]\n");
    exit(1);
  }
  if (argc == 2) {
    listp = readcsv(listp, argv[1]);
  }
  apply(listp, printnv, "%s\n");

  return 0;
}
*/

