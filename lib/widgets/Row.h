/********************************************************************
 *         This example code is from the book:
 *
 *           The X Window System: Programming and Applications with Xt
 *           Second OSF/Motif Edition
 *         by
 *           Douglas Young
 *           Prentice Hall, 1994
 *
 *         Copyright 1994 by Prentice Hall
 *         All Rights Reserved
 *
 *  Permission to use, copy, modify, and distribute this software for 
 *  any purpose except publication and without fee is hereby granted, provided 
 *  that the above copyright notice appear in all copies of the software.
 * *****************************************************************************/

/***************************************************
 * Row.h: Public header file for the Row widget.
 ***************************************************/
#ifndef ROW_H
#define ROW_H

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass xsRowWidgetClass;
typedef struct _XsRowClassRec *XsRowWidgetClass;
typedef struct _XsRowRec      *XsRowWidget;

#ifdef _NO_PROTO
Widget XsCreateRow();
#else
Widget XsCreateRow ( Widget   parent,
                     char    *name,
                     ArgList  arglist,
                     Cardinal argcount);
#endif

#ifdef __cplusplus
}
#endif

#endif
        
