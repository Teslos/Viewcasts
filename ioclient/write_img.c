/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Toni Ivas & Helmut Vor                           */
/* Datum    :   17.10.02                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/* read_img     : read img-Data                                  */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */ 

#include <stdio.h>
#include <stdlib.h>

#include "errs.h"
#include "csv.h"
#include "mem.h"
#include "img.h"
#include "grafik.h"


/***************************************************************
 Function:

 Parameters:
 ===========
        char *dataname - name of img data file
 Returns:
 ========
        returns 0 if success otherwise -1
****************************************************************/
int write_img(char* dataname)
{
  FILE *fp;

  /* open data */
  if (!file_read(dataname))
    //warning(errs[Eprem]);

  fp = fopen(dataname, "w");
  if (fp == NULL) 
    {
      error(Efile);
      return -1;
    }
  
  /* write line by line */
  setparams(fp);
  
  /* close file */
  fclose(fp);

  return(0);
}




/* machine generated: do not edit */
void setparams(FILE *fp)
{
	fprintf(fp,"\" result open \",%i\n",0);
	fprintf(fp,"\" 2nd result open \",%i\n",0);
	fprintf(fp,"\" 2nd result shown \",%i\n",0);
	fprintf(fp,"\" showing cell \",%i\n",0);
	fprintf(fp,"\" showing flow \",%i\n",g_animation.stroem_on);
	fprintf(fp,"\" showing stress \",%i\n",g_animation.spann_on);
	fprintf(fp,"\" showing temperature \",%i\n",g_animation.temp_on);
	fprintf(fp,"\" showing geometry \",%i\n",g_transform.geo_on);
	fprintf(fp,"\" showing result \",%i\n",g_transform.result_on);
	fprintf(fp,"\" showing movement of oven \",%i\n",g_transform.absenken_on);
	fprintf(fp,"\" showing material groups \",%i\n",g_transform.stoffgruppen_on);
	fprintf(fp,"\" showing centered geometry \",%i\n",g_transform.zentriert_on);
	fprintf(fp,"\" showing boundary conditions \",%i\n",g_transform.rand_on);
	fprintf(fp,"\" showing wireframe \",%i\n",0);
	fprintf(fp,"\" showing net \",%i\n",0);
	fprintf(fp,"\" table on \",%i\n",0);
	fprintf(fp,"\" cutting on \",%i\n",g_transform.schneiden_on);
	fprintf(fp,"\" actual img cutted \",%i\n",0);
	fprintf(fp,"\" showing oven \",%i\n",g_ofen.on);
	fprintf(fp,"\" actual position of oven \",%i\n",g_ofen.akt_position);
	fprintf(fp,"\" mirror on \",%i\n",g_transform.spiegel_on);
	fprintf(fp,"\" cycling mirror on \",%i\n",g_transform.zyklus_on);
	fprintf(fp,"\" shell for stress-part on \",%i\n",g_spannung.huelle_on);
	fprintf(fp,"\" axis on \",%i\n",0);
	fprintf(fp,"\" solid model on \",%i\n",0);
	fprintf(fp,"\" logo on \",%i\n",0);
	fprintf(fp,"\" transparent on \",%i\n",0);
	fprintf(fp,"\" texture on \",%i\n",g_color_extra.texture_on);
	fprintf(fp,"\" materials shown \",%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	fprintf(fp,"\" showing prespektive \",%i\n",g_projektion.perspektive);
	fprintf(fp,"\" edge on \",%i\n",edgeinfo.edge_on);
	fprintf(fp,"\" actual time \",%i\n",0);
	fprintf(fp,"\" vector time \",%i\n",0);
	fprintf(fp,"\" displacement for mirroring in x \",%i\n",0);
	fprintf(fp,"\" displacement for mirroring in y \",%i\n",0);
	fprintf(fp,"\" displacement for mirroring in z \",%i\n",0);
	fprintf(fp,"\" position x for cutaway zooming \",%i\n",0);
	fprintf(fp,"\" position y for cutaway zooming \",%i\n",0);
	fprintf(fp,"\" position z for cutaway zooming \",%i\n",0);
	fprintf(fp,"\" position in X Y Z direction \",%f,%f,%f \n",g_transform.move[0],g_transform.move[1],g_transform.move[2]);
	fprintf(fp,"\" position of panning in X Y Z \",%f,%f,%f \n",g_transform.pan[0],g_transform.pan[1],g_transform.pan[2]);
	fprintf(fp,"\" rotation in x direction \",%f\n",g_transform.rotx);
	fprintf(fp,"\" rotation in y direction \",%f\n",g_transform.roty);
	fprintf(fp,"\" rotation in z direction \",%f\n",g_transform.rotz);
	fprintf(fp,"\" near cliping plane \",%f\n",g_projektion.near);
	fprintf(fp,"\" far clipping plane \",%f\n",g_projektion.far);
	fprintf(fp,"\" top clipping plane \",%f\n",g_projektion.top);
	fprintf(fp,"\" bottom clipping plane \",%f\n",g_projektion.bottom);
	fprintf(fp,"\" left clipping plane \",%f\n",g_projektion.left);
	fprintf(fp,"\" right clipping plane \",%f\n",g_projektion.right);
	fprintf(fp,"\" zoom factor \",%f\n",g_transform.scale);
	fprintf(fp,"\" coordinate cross factor \",%f\n",g_transform.scale_k);
	fprintf(fp,"\" rotation angle for cycle \",%f\n",g_transform.zyklus_ang);
	fprintf(fp,"\" direction of rotation \",%i\n",0);
	fprintf(fp,"\" minimum of parameter interval \",%f\n",g_loesung.akt_min);
	fprintf(fp,"\" maximum of parameter interval \",%f\n",g_loesung.akt_max);
	fprintf(fp,"\" default transparency \",%f\n",g_color_extra.transparency);
	fprintf(fp,"\" factor of scaling \",%f\n",g_spannung.factor);
	fprintf(fp,"\" mirror on xz-plane \",%i\n",g_transform.mirrorx);
	fprintf(fp,"\" mirror on zy-plane \",%i\n",g_transform.mirrory);
	fprintf(fp,"\" mirror on xy-plane \",%i\n",g_transform.mirrorz);
	fprintf(fp,"\" count of cycle mirrors \",%i\n",g_transform.zyklus_anz);
	fprintf(fp,"\" number of cut point 1 \",%i\n",g_transform.schnittp1_nr);
	fprintf(fp,"\" number of cut point 2 \",%i\n",g_transform.schnittp2_nr);
	fprintf(fp,"\" number of cut point 3 \",%i\n",g_transform.schnittp3_nr);
	fprintf(fp,"\" count of isolines \",%i\n",g_isolinien.anzahl_isolinien);
	fprintf(fp,"\" current colortable name \",%i\n",g_color_extra.color_name);
	fprintf(fp,"\" deform solid or shell \",%i\n",g_spannung.verspanne_solid_huelle);
	fprintf(fp,"\" animation start \",%i\n",g_animation.anim_anfang);
	fprintf(fp,"\" animation end \",%i\n",g_animation.anim_ende);
	fprintf(fp,"\" every n-th frame \",%i\n",g_animation.anim_frame);
	fprintf(fp,"\" animation art \",%i\n",g_animation.art);
	fprintf(fp,"\" edge color \",%i\n",edgeinfo.edgecolor);
	fprintf(fp,"\" material number \",%i\n",0);
	fprintf(fp,"\" show matrial \",%i\n",0);
	fprintf(fp,"\" coordinate of cut point 1 \" ,%f,%f,%f\n",g_transform.schnittp1_koord[0],g_transform.schnittp1_koord[1],g_transform.schnittp1_koord[2]);
	fprintf(fp,"\" coordinate of cut point 2 \" ,%f,%f,%f\n",g_transform.schnittp2_koord[0],g_transform.schnittp2_koord[1],g_transform.schnittp2_koord[2]);
	fprintf(fp,"\" coordinate of cut point 3 \" ,%f,%f,%f\n",g_transform.schnittp3_koord[0],g_transform.schnittp3_koord[1],g_transform.schnittp3_koord[2]);
	fprintf(fp,"\" normal of cutting plane \" ,%f,%f,%f\n",g_transform.norm[0],g_transform.norm[1],g_transform.norm[2]);
	fprintf(fp,"\" vector for centring \" ,%f,%f,%f\n",g_transform.zentrier_vector[0],g_transform.zentrier_vector[1],g_transform.zentrier_vector[2]);
	fprintf(fp,"\" name of result file \",%c,%c \n",g_loesung.dateiname[0],g_loesung.dateiname[1]);
	fprintf(fp,"\" rotation axis for cycling \",%c\n",g_transform.zyklus_achse);
}
