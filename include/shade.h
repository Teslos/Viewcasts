#ifndef _SHADE_H_
#define _SHADE_H_

typedef struct vect {
  float x,y,z,w;
}vect;

typedef struct material {
  vect emission;
  vect ambient;
  vect diffuse;
  vect specular;
  float shininess;
  float alpha;
}material;

typedef struct lamp {
  struct lamp *next;
  vect ambient;
  vect color;
  vect position;
} lamp;

typedef struct lights {
  vect ambient;
  int localviewer;
  float attenconst, attenmult;
  struct lamp *lamps;
} lights;

void makelight(int i, float x, float y, float z);
void shadeoff();
void setdiffuse(float r, float g, float b);
void setspecular(float r, float g, float b);
void setambient(float r, float g, float b);
void setshininess(float s);
void shadeinit();
void shadeon();

/* file functions */
lights *newlights();
lights *lightsfromfile();
lights *lightsfromf();
lights *readlights();
lamp *newlamp();
material *newmat();
material *matfromfile();
material *matfromf();
material *readmaterial();


#endif

