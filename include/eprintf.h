/* eprintf.h: error wrapper functions */
#include <stdio.h>
#include <stdlib.h>
#ifndef __EPRINTF_H__
#define __EPRINTF_H__
extern void eprintf(char *,...);
extern void weprintf(char *,...);
extern char *estrdup(char *);
extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern char *progname(void);
extern void setprogname(char *); 
#endif
