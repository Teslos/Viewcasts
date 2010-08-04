/* ******************************************************************** */
/* This header contains all standard errors  in COLOR3D                 */
/* ******************************************************************** */
/* Dateiname: errs.h                                                    */
/* ******************************************************************** */
#ifndef _ERRS_H_
#define _ERRS_H_
extern void error(int nmes);  /* prints simple error message */
extern char *errs[];
enum {
  Eprem,       /* Premission denied */
  Eio,         /* I/O error */
  Efile,       /* File does not exist */
  Enomem,      /* Memory limit reached */
  Espace,      /* Out of file space */
  Erlimit,     /* Error in getting resources */
  Ewriten,     /* Can't write n-bytes */
  Ereadn,      /* Can't read n-bytes */
  Elimit,      /* Resources does not exist */
  Emmap,       /* File can't be map in memory */
  Estat,       /* File stat error */
};  
#endif
