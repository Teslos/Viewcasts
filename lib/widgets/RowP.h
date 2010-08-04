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
 * RowP.h: Private header file for the Row widget.
 ***************************************************/
#ifndef ROWP_H
#define ROWP_H

#include "Row.h"
#include <Xm/ManagerP.h>

typedef struct _XsRowClassPart {
    int    empty;
} XsRowClassPart;
        
typedef struct _XsRowClassRec {
    CoreClassPart        core_class;
    CompositeClassPart   composite_class;
    ConstraintClassPart  constraint_class;
    XmManagerClassPart   manager_class;
    XsRowClassPart       row_class;
}  XsRowClassRec;
    
typedef struct {
    int empty;
} XsRowPart;
    
typedef struct _XsRowRec {
    CorePart        core;
    CompositePart   composite;
    ConstraintPart  constraint;
    XmManagerPart   manager;
    XsRowPart       row;
}  XsRowRec;
#endif

