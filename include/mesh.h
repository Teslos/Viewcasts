/*
 * Copyright 1991-94 Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABLIITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * In no event shall Silicon Graphics be liable for any special, incidental,
 * indirect or consequential damages of any kind, or any damages whatsoever
 * resulting from loss of use, data or profits, whether or not advised of the
 * possibility of damage, and on any theory of liability, arising out of or
 * in connection with the possession, use or performance of this software.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND
 * Use, duplication, or disclosure by the Government is subject to restrictions
 * set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013 and/or in
 * similar or successor clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the United States.
 * Contractor/manufacturer is Silicon Graphics, Inc., 2011 N. Shoreline Blvd.,
 * Mountain View, CA 94039-7311.
 */

#ifndef MESHDEF
#define MESHDEF

typedef struct Meshobj {
    int			connectcount;
    int			independentcount;
    int			npolys;
    int			vertcount;
    int			vertno;
    struct Edge *	edgearray;
    struct Edge *	freeedges;
    struct Edge **	edgehashlist;
    struct Tri *	curtri;
    struct Tri **	trihashlist;
    struct Trilist *	adjtrilist[4];
    struct Trilist *	donetrilist;
    struct Trilist *	newtrilist;
    struct Trilist *	trilist;
    struct Vert *	tmpvert;
    struct Vert **	verthashlist;
    struct Vertlist *	vertlist;
    void		(*ambegin)( int, int );
    void		(*amend)( void );
    int			(*amhashvert)( long );
    int			(*amvertsame)( long, long );
    void		(*amvertdata)( long );
    void		(*ambgntmesh)( void );
    void		(*amendtmesh)( void );
    void		(*amswaptmesh)( void );
    void		(*amvert)( long );
} Meshobj;

extern	Meshobj * 	newMeshobj ( 
    void (*)( int, int ), void (*)( void ),
    int	 (*)( long ),     int  (*)( long, long ),
    void (*)( long ),     void (*)( void ),
    void (*)( void ),     void (*)( void ),
    void (*)( long ) );
extern	void		freeMeshobj( Meshobj * );
extern	void		in_ambegin( Meshobj * );
extern	void		in_amnewtri( Meshobj * );
extern	void		in_amvert( Meshobj *, long );
extern	void		in_amend( Meshobj * );

#endif
