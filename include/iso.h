/* header file for isoline construction */
#ifndef __ISO_H__
#define __ISO_H__

typedef struct Iso
{
  float x, y;
  int niso;
}Iso;

typedef struct Vert
{
  float *xyz[3];
  float *val;
}Vert;

typedef struct Tri 
{
  struct Tri *next;
  Vert   vert[3];
  float  norm[3];
  long   id;
}Tri;

/* function interfaces */
void applyiso( Tri *listp,
	       void(*fn)(Tri*, float, Iso*), float tempg, int isol);
void interpvert(Tri *tri, float tempg, Iso *line);
Tri *btrilist(Tri*, float, int);
void printnv(Tri *p, void *arg);
void apply(Tri *listp, void (*fn)(Tri*, void*), void *arg);
void inccounter(Tri *listp, void *arg);
#endif
