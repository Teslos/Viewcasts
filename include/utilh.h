#ifndef __UTIL_H__
#define __UTIL_H__

enum{NSIDES=6, NPOINTS=4, NTRI=2, NPPTRI=3};
enum{NINTV=1, NVGROW=2};
enum{NPHEX=8, NPPEN=6, NPTET=4};
enum{FREE=1, ALLOC=0};
struct NVert{
  int nv;         /* number of verticies */
  int maxv;       /* allocated numbers of values */
  Vert *vert;     /* array of verticies */
}nvert;

/*declarations of the functions */
Tri *newitem(long id, Vert *vert);
Tri *addfront(Tri *listp, Tri *ntri);
Tri *addend(Tri *listp, Tri *ntri);
Tri *lookup(Tri *listp, long id);
void freeall(Tri *listp);
Tri *newelem(long id, Vert *nvert, float *norm);
int rtri(int f);
#endif
