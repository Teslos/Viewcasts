/* ******************************************************************** */
/* Header file containing structure definition for image saving         */
/* ******************************************************************** */
/* Dataname: img.h                                                      */
/* ******************************************************************** */

#ifndef _IMG_H_
#define _IMG_H_

#include "strukturen.h"
#define MAXSTOFF 45

typedef struct img {
  t_bool  
          Ires_op,           /* result open */
          Ires2_op,          /* 2nd result open */
          Ires2_on,          /* 2nd result shown */
          Ikorn_on,          /* showing cell */
          Istroem_on,        /* showing flow */ /* animation */
          Ispann_on,         /* showing stress */ /* animation */
	  Itemp_on,          /* showing temperature */ /* animation */
          Igeo_on,           /* showing geometry */ /* transform */
          Iresult_on,        /* showing result */ /* transform */
          Iabsenken_on,      /* showing movement of oven */ /* transform */
          Istoffgruppen_on,  /* showing material groups */ /* transform */
          Izentriert_on,     /* showing centered geometry */ /* transform */
          Irand_on,          /* showing boundary conditions */ /* transform */
          Iwire_on,          /* showing wireframe */
          Inet_on,           /* showing net */
          Itable_on,         /* table on */
          Ischneiden_on,     /* cutting on */ /* transform */
          Iactcut,           /* actual img cutted */
          Ion,               /* showing oven */ /* ofen */
          Iakt_position,     /* actual position of oven */ /* ofen */
          Ispiegel_on,       /* mirror on */ /* transform */
          Izyklus_on,        /* cycling mirror on */ /* transform */
          Ihuelle_on,        /* shell for stress-part on */ /* spannung */
          Iaxis_on,          /* axis on */
          Isolid_on,         /* solid model on */
          Ilogo_on,          /* logo on */
          Itrans_on,         /* transparent on */
	  Itexture_on,       /* texture on */ /* color_extra */
          Imat_on[MAXSTOFF], /* materials shown */
          Iperspektive,      /* showing prespektive */ /* projektion */
          Iedge_on;          /* edge on */ /* edge */

  float   
          Itime,     /* actual time */
          Ivec_time, /* vector time */
          Idisp_x,   /* displacement for mirroring in x */
          Idisp_y,   /* displacement for mirroring in y */
          Idisp_z,   /* displacement for mirroring in z */
          Ipos_x,    /* position x for cutaway zooming */
          Ipos_y,    /* position y for cutaway zooming */
          Ipos_z,    /* position z for cutaway zooming */
          Imove[3],  /* position in X Y Z direction */ /* transform */
          Ipan[3],   /* position of panning in X Y Z */ /* transform */
          Irotx,     /* rotation in x direction */ /* transform */
          Iroty,     /* rotation in y direction */ /* transform */
          Irotz,     /* rotation in z direction */ /* transform */
          Inear,     /* near cliping plane */ /* projektion */
          Ifar,      /* far clipping plane */ /* projektion */
          Itop,      /* top clipping plane */ /* projektion */
          Ibottom,   /* bottom clipping plane */ /* projektion */
          Ileft,     /* left clipping plane */ /* projektion */
          Iright,    /* right clipping plane */ /* projektion */
          Iscale,    /* zoom factor */ /* transform */
          Iscale_k,  /* coordinate cross factor */ /* transform */
          Izyklus_ang,/* rotation angle for cycle */ /* transform */
          Irot_direc,/* direction of rotation */
          Iakt_min,      /* minimum of parameter interval */ /* loesung */
          Iakt_max,      /* maximum of parameter interval */ /* loesung */
	  Itransparency, /* default transparency */ /* color_extra */
	  Ifactor;   /* factor of scaling */ /* spannung */
         
  int     
          Imirrorx,      /* mirror on xz-plane */ /* transform */
          Imirrory,      /* mirror on zy-plane */ /* transform */
          Imirrorz,      /* mirror on xy-plane */ /* transform */
          Izyklus_anz,   /* count of cycle mirrors */ /* transform */
          Ischnittp1_nr, /* number of cut point 1 */ /* transform */
          Ischnittp2_nr, /* number of cut point 2 */ /* transform */
          Ischnittp3_nr, /* number of cut point 3 */ /* transform */
          Ianzahl_isolinien, /* count of isolines */ /* isolinien */
	  Icolor_name,   /* current colortable name */ /* color_extra */
	  Iverspanne_solid_huelle, /* deform solid or shell */ /* spannung */
          Ianim_anfang,  /* animation start */ /* animation */
          Ianim_ende,    /* animation end */ /* animation */
          Ianim_frame,   /* every n-th frame */ /* animation */
          Iart,          /* animation art */ /* animation */
          Iedgecolor,    /* edge color */ /* edge */
          Inr,               /* material number */ /* stoffgebiet */
          Izeichne_on;       /* show matrial */ /* stoffgebiet */
  t_koord
          Ischnittp1_koord, /* coordinate of cut point 1 */ /* transform */
          Ischnittp2_koord, /* coordinate of cut point 2 */ /* transform */
          Ischnittp3_koord, /* coordinate of cut point 3 */ /* transform */
          Inorm,            /* normal of cutting plane */ /* transform */
          Izentrier_vector; /* vector for centring */ /* transform */
  char    
          Idateiname[2][256],  /* name of result file */ /* loesung */
          Izyklus_achse;    /* rotation axis for cycling */ /* transform */
 
}t_image;

extern t_image g_image;

/* exported functions */
int read_img(char *dataname);
int write_img(char *dataname);
void get_imgval(struct img *img);
void getparams(struct img *img);
void setparams(FILE *fp);

#endif          

           
