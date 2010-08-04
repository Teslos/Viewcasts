/**********************************************
 Purpose:  
 ========
    This function builds the  list 
    of triangles for isoline algorithm 

 Author:
 =======
         Toni Ivas & Helmut Vor
**********************************************/
#include <stdlib.h>
#include <stdio.h>
#include "iso.h"
#include "utilh.h"
#include "eprintf.h"
typedef struct Trilist {
  Tri *first, *last;
}Trilist;

Trilist list;

/**********************************************
 Add new triangle to list of the triangles.
**********************************************/					       
Tri *newitem(long id, Vert *nvert)
{
  Tri *ntri;
  int i;

  ntri = (Tri *) emalloc(sizeof(Tri));
  for(i = 0; i < 3; i++)
    ntri->vert[i] = nvert[i];
  ntri->id   = id;
  ntri->next = NULL;
  return ntri;
}
/**********************************************
 Add new triangles to list of elements. With
 normals. This function is similar to above fun.
***********************************************/					       
Tri *newelem(long id, Vert *nvert, float *norm)
{
  Tri *ntri;
  int i;

  ntri = (Tri *) emalloc(sizeof(Tri));
  for(i = 0; i < 3; i++)
    ntri->vert[i] = nvert[i];
  ntri->id   = id;
  for(i = 0; i < 3; i++)
    ntri->norm[i] = norm[i]; 
  ntri->next = NULL;
  return ntri;
}

/* addfront: add new triangle to front of list*/
Tri *addfront(Tri *listp, Tri *ntri)
{
  ntri->next = listp;
  list.first = listp;
  return ntri;
}


/* addend: add new triangle to end of list */
Tri *addend(Tri *listp, Tri *ntri)
{
  Tri *p;
  
  if(listp == NULL) {
    list.last = ntri;
    return ntri;
  }
  p = list.last;
  p->next = ntri;
  list.last = ntri;
  list.last->next = NULL;
  
  return listp;
}

/* lookup: sequential search for triangle in listp */
Tri *lookup(Tri *listp, long id)
{
  for( ;listp != NULL; listp= listp->next)
    if(id == listp->id)
      return listp;
      
  return NULL;   /* no match */
}

/* inccounter: count all elements of list */
void inccounter(Tri *p, void *arg)
{
  int *ip;
  
  /* p is unused */
  ip = (int *) arg;
  (*ip)++;
}

/* freeall: free all elements of the listp */
void freeall(Tri *listp)
{
  Tri *next;
  
  for( ; listp != NULL; listp = next)
    {
      next = listp->next;
      /* assumes name is freed elsewhere */
      free(listp);
    }
}

/* apply: execute fn for each element of listp */
void apply( Tri *listp,
	    void(*fn)(Tri*, void*), void *arg)
{
  for( ; listp != NULL; listp = listp->next)
    (*fn)(listp, arg); /* call function */
}

void printnv(Tri *p, void *arg)
{
  char *fmt;
  int i,j;

  fmt = (char *) arg;
  for (i = 0; i < 3; i++)
	for(j = 0; j < 3; j++)
    printf(fmt, *p->vert[i].xyz[j]);
}

/************************************************************
 Purpose:
 ========
       This function apply isoline algorithm to all
       triangles in the list.
 Parameters:
 ==========
       listp - list pointer
       fn    - pointer to function which contains algorithm 
               for isolines
       float tempg - argument to function which is called
                     (isoterm value)
       isol - isoline number.
************************************************************/
void applyiso( Tri *listp,
	    void(*fn)(Tri*, float, Iso*), float tempg, int isol)
{
  Iso iso;
  iso.niso = isol;
  for( ; listp != NULL; listp = listp->next)
    (*fn)(listp, tempg, &iso); 
}

/*small code for testing*/
#ifdef TEST
void main()
{
  Tri *listp=NULL, *ntri=NULL, *tmptri=NULL;
  int i,j,id;

  float values[] = {
    1.0,1.0,0.0,
    3.0,1.0,0.0,
    5.0,1.0,0.0,
    7.0,1.0,0.0,
    2.0,2.0,0.0,
    4.0,2.0,0.0,
    6.0,2.0,0.0
  };

  float temp[] = {
    100.0,
    100.0,
    100.0,
    100.0,
    200.0,
    200.0,
    200.0};

  int faces[][3] = { {0,1,4},
		 {1,5,4},
		 {1,2,5},
		 {2,6,5},
		 {2,3,6}};

  Vert trivert[3];
  Vert verts[7];
  
  
  for(i=0; i<7; i++)
    {
      verts[i].val = &temp[i];
      for(j=0; j<3; j++)
	{
	  verts[i].xyz[j] = &values[i*3+j];
	 
	}
    }

  id = 3;
  /*
  for(i = 0; i < 5; i++)
    for( j = 0; j < 3; j++)
    printf("faces[%i][%i]= %i\n", i,j,faces[i][j]);*/

  /* add all triangles */
  for(i = 0; i < 5; i++)
    {
      for( j = 0; j < 3; j++)
	trivert[j] = verts[faces[i][j]];
      ntri = newitem(i, trivert);
      listp = addend(listp, ntri);
    }
  
  apply(listp, printnv, "%i, vert num: %i\n");
  /*applyiso(listp, interpvert);*/
  tmptri = lookup(listp, id);
  for(i=0; i<3; i++)
    for(j=0; j<3;j++)
      {
	printf("vert value  %f\n", *(tmptri->vert[i].xyz[j]));
	
      }
   
 }

#endif






