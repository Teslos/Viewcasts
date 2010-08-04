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
#include "str.h"
//#define DEBUG 1
enum {MAXFIELD = 1000, CSVMAX = 100};
t_image g_image, *img;		/* global img structure */
char *imgfield(int ik, int jk);
csvline *listp = NULL;

/***************************************************************
 Function:

 Parameters:
 ===========
        char *dataname - name of img data file
 Returns:
 ========
        returns 0 if success otherwise -1
****************************************************************/
int read_img(char* dataname)
{
  FILE *fp;                     /* file pointer */
  int i=0, j=0;                 /* counter */
  int nl=0;                     /* number of lines in img file */ 
  char *line;                   /* one line of img file */
  char ***newf, **newm;		/* new pointer to field */
  int maxfield = 0, csvmax = 0;	/* maximum field */
  

  /* open data */
  if (!file_read(dataname))
    error(Eprem);
  
  listp = readcsv(listp, dataname);
		     
  /* Transform imgfield list 
   * in structure for easier 
   * accessing. 
   */
  get_imgval(&g_image);

  /* import values in global structures */
  getparams(&g_image);

  return 0;
}

char *imgfield(int ik, int jk)
{
  int i;
  csvline *p;
  for (p = listp, i = 0; (p != NULL && i < ik); p = p->next, i++)
    ;
  return p->values[jk];    
}

void loadimg() {

}



/* machine generated: do not edit */
void getparams(struct img *img)
{
	g_animation.stroem_on = img->Istroem_on;
	g_animation.spann_on = img->Ispann_on;
	g_animation.temp_on = img->Itemp_on;
	g_transform.geo_on = img->Igeo_on;
	g_transform.result_on = img->Iresult_on;
	g_transform.absenken_on = img->Iabsenken_on;
	g_transform.stoffgruppen_on = img->Istoffgruppen_on;
	g_transform.zentriert_on = img->Izentriert_on;
	g_transform.rand_on = img->Irand_on;
	g_transform.schneiden_on = img->Ischneiden_on;
	g_ofen.on = img->Ion;
	g_ofen.akt_position = img->Iakt_position;
	g_transform.spiegel_on = img->Ispiegel_on;
	g_transform.zyklus_on = img->Izyklus_on;
	g_spannung.huelle_on = img->Ihuelle_on;
	g_color_extra.texture_on = img->Itexture_on;
	g_projektion.perspektive = img->Iperspektive;
	edgeinfo.edge_on = img->Iedge_on;
	g_transform.move[0] = img->Imove[0];
	g_transform.move[1] = img->Imove[1];
	g_transform.move[2] = img->Imove[2];
	g_transform.pan[0] = img->Ipan[0];
	g_transform.pan[1] = img->Ipan[1];
	g_transform.pan[2] = img->Ipan[2];
	g_transform.rotx = img->Irotx;
	g_transform.roty = img->Iroty;
	g_transform.rotz = img->Irotz;
	g_projektion.near = img->Inear;
	g_projektion.far = img->Ifar;
	g_projektion.top = img->Itop;
	g_projektion.bottom = img->Ibottom;
	g_projektion.left = img->Ileft;
	g_projektion.right = img->Iright;
	g_transform.scale = img->Iscale;
	g_transform.scale_k = img->Iscale_k;
	g_transform.zyklus_ang = img->Izyklus_ang;
	g_color_extra.transparency = img->Itransparency;
	g_spannung.factor = img->Ifactor;
	g_transform.mirrorx = img->Imirrorx;
	g_transform.mirrory = img->Imirrory;
	g_transform.mirrorz = img->Imirrorz;
	g_transform.zyklus_anz = img->Izyklus_anz;
	g_transform.schnittp1_nr = img->Ischnittp1_nr;
	g_transform.schnittp2_nr = img->Ischnittp2_nr;
	g_transform.schnittp3_nr = img->Ischnittp3_nr;
	g_isolinien.anzahl_isolinien = img->Ianzahl_isolinien;
	g_color_extra.color_name = img->Icolor_name;
	g_spannung.verspanne_solid_huelle = img->Iverspanne_solid_huelle;
	g_animation.anim_anfang = img->Ianim_anfang;
	g_animation.anim_ende = img->Ianim_ende;
	g_animation.anim_frame = img->Ianim_frame;
	g_animation.art = img->Iart;
	edgeinfo.edgecolor = img->Iedgecolor;
	memcpy(g_transform.schnittp1_koord, img->Ischnittp1_koord, sizeof(img->Ischnittp1_koord));
	memcpy(g_transform.schnittp2_koord, img->Ischnittp2_koord, sizeof(img->Ischnittp2_koord));
	memcpy(g_transform.schnittp3_koord, img->Ischnittp3_koord, sizeof(img->Ischnittp3_koord));
	memcpy(g_transform.norm, img->Inorm, sizeof(img->Inorm));
	memcpy(g_transform.zentrier_vector, img->Izentrier_vector, sizeof(img->Izentrier_vector));
	g_transform.zyklus_achse = img->Izyklus_achse;
}



/* machine-generated; do not edit */

void get_imgval(struct img *img) 
{
	 img->Ires_op = atoi(imgfield(0,1));
	 img->Ires2_op = atoi(imgfield(1,1));
	 img->Ires2_on = atoi(imgfield(2,1));
	 img->Ikorn_on = atoi(imgfield(3,1));
	 img->Istroem_on = atoi(imgfield(4,1));
	 img->Ispann_on = atoi(imgfield(5,1));
	 img->Itemp_on = atoi(imgfield(6,1));
	 img->Igeo_on = atoi(imgfield(7,1));
	 img->Iresult_on = atoi(imgfield(8,1));
	 img->Iabsenken_on = atoi(imgfield(9,1));
	 img->Istoffgruppen_on = atoi(imgfield(10,1));
	 img->Izentriert_on = atoi(imgfield(11,1));
	 img->Irand_on = atoi(imgfield(12,1));
	 img->Iwire_on = atoi(imgfield(13,1));
	 img->Inet_on = atoi(imgfield(14,1));
	 img->Itable_on = atoi(imgfield(15,1));
	 img->Ischneiden_on = atoi(imgfield(16,1));
	 img->Iactcut = atoi(imgfield(17,1));
	 img->Ion = atoi(imgfield(18,1));
	 img->Iakt_position = atoi(imgfield(19,1));
	 img->Ispiegel_on = atoi(imgfield(20,1));
	 img->Izyklus_on = atoi(imgfield(21,1));
	 img->Ihuelle_on = atoi(imgfield(22,1));
	 img->Iaxis_on = atoi(imgfield(23,1));
	 img->Isolid_on = atoi(imgfield(24,1));
	 img->Ilogo_on = atoi(imgfield(25,1));
	 img->Itrans_on = atoi(imgfield(26,1));
	 img->Itexture_on = atoi(imgfield(27,1));
	 img->Imat_on[0] = atoi(imgfield(28,1));
	 img->Imat_on[1] = atoi(imgfield(28,2));
	 img->Imat_on[2] = atoi(imgfield(28,3));
	 img->Imat_on[3] = atoi(imgfield(28,4));
	 img->Imat_on[4] = atoi(imgfield(28,5));
	 img->Imat_on[5] = atoi(imgfield(28,6));
	 img->Imat_on[6] = atoi(imgfield(28,7));
	 img->Imat_on[7] = atoi(imgfield(28,8));
	 img->Imat_on[8] = atoi(imgfield(28,9));
	 img->Imat_on[9] = atoi(imgfield(28,10));
	 img->Imat_on[10] = atoi(imgfield(28,11));
	 img->Imat_on[11] = atoi(imgfield(28,12));
	 img->Imat_on[12] = atoi(imgfield(28,13));
	 img->Imat_on[13] = atoi(imgfield(28,14));
	 img->Imat_on[14] = atoi(imgfield(28,15));
	 img->Imat_on[15] = atoi(imgfield(28,16));
	 img->Imat_on[16] = atoi(imgfield(28,17));
	 img->Imat_on[17] = atoi(imgfield(28,18));
	 img->Imat_on[18] = atoi(imgfield(28,19));
	 img->Imat_on[19] = atoi(imgfield(28,20));
	 img->Imat_on[20] = atoi(imgfield(28,21));
	 img->Imat_on[21] = atoi(imgfield(28,22));
	 img->Imat_on[22] = atoi(imgfield(28,23));
	 img->Imat_on[23] = atoi(imgfield(28,24));
	 img->Imat_on[24] = atoi(imgfield(28,25));
	 img->Imat_on[25] = atoi(imgfield(28,26));
	 img->Imat_on[26] = atoi(imgfield(28,27));
	 img->Imat_on[27] = atoi(imgfield(28,28));
	 img->Imat_on[28] = atoi(imgfield(28,29));
	 img->Imat_on[29] = atoi(imgfield(28,30));
	 img->Imat_on[30] = atoi(imgfield(28,31));
	 img->Imat_on[31] = atoi(imgfield(28,32));
	 img->Imat_on[32] = atoi(imgfield(28,33));
	 img->Imat_on[33] = atoi(imgfield(28,34));
	 img->Imat_on[34] = atoi(imgfield(28,35));
	 img->Imat_on[35] = atoi(imgfield(28,36));
	 img->Imat_on[36] = atoi(imgfield(28,37));
	 img->Imat_on[37] = atoi(imgfield(28,38));
	 img->Imat_on[38] = atoi(imgfield(28,39));
	 img->Imat_on[39] = atoi(imgfield(28,40));
	 img->Imat_on[40] = atoi(imgfield(28,41));
	 img->Imat_on[41] = atoi(imgfield(28,42));
	 img->Imat_on[42] = atoi(imgfield(28,43));
	 img->Imat_on[43] = atoi(imgfield(28,44));
	 img->Imat_on[44] = atoi(imgfield(28,45));
	 img->Iperspektive = atoi(imgfield(29,1));
	 img->Iedge_on = atoi(imgfield(30,1));
	 img->Itime = atof(imgfield(31,1));
	 img->Ivec_time = atof(imgfield(32,1));
	 img->Idisp_x = atof(imgfield(33,1));
	 img->Idisp_y = atof(imgfield(34,1));
	 img->Idisp_z = atof(imgfield(35,1));
	 img->Ipos_x = atof(imgfield(36,1));
	 img->Ipos_y = atof(imgfield(37,1));
	 img->Ipos_z = atof(imgfield(38,1));
	 img->Imove[0] = atof(imgfield(39,1));
	 img->Imove[1] = atof(imgfield(39,2));
	 img->Imove[2] = atof(imgfield(39,3));
	 img->Ipan[0] = atof(imgfield(40,1));
	 img->Ipan[1] = atof(imgfield(40,2));
	 img->Ipan[2] = atof(imgfield(40,3));
	 img->Irotx = atof(imgfield(41,1));
	 img->Iroty = atof(imgfield(42,1));
	 img->Irotz = atof(imgfield(43,1));
	 img->Inear = atof(imgfield(44,1));
	 img->Ifar = atof(imgfield(45,1));
	 img->Itop = atof(imgfield(46,1));
	 img->Ibottom = atof(imgfield(47,1));
	 img->Ileft = atof(imgfield(48,1));
	 img->Iright = atof(imgfield(49,1));
	 img->Iscale = atof(imgfield(50,1));
	 img->Iscale_k = atof(imgfield(51,1));
	 img->Izyklus_ang = atof(imgfield(52,1));
	 img->Irot_direc = atof(imgfield(53,1));
	 img->Iakt_min = atof(imgfield(54,1));
	 img->Iakt_max = atof(imgfield(55,1));
	 img->Itransparency = atof(imgfield(56,1));
	 img->Ifactor = atof(imgfield(57,1));
	 img->Imirrorx = atoi(imgfield(58,1));
	 img->Imirrory = atoi(imgfield(59,1));
	 img->Imirrorz = atoi(imgfield(60,1));
	 img->Izyklus_anz = atoi(imgfield(61,1));
	 img->Ischnittp1_nr = atoi(imgfield(62,1));
	 img->Ischnittp2_nr = atoi(imgfield(63,1));
	 img->Ischnittp3_nr = atoi(imgfield(64,1));
	 img->Ianzahl_isolinien = atoi(imgfield(65,1));
	 img->Icolor_name = atoi(imgfield(66,1));
	 img->Iverspanne_solid_huelle = atoi(imgfield(67,1));
	 img->Ianim_anfang = atoi(imgfield(68,1));
	 img->Ianim_ende = atoi(imgfield(69,1));
	 img->Ianim_frame = atoi(imgfield(70,1));
	 img->Iart = atoi(imgfield(71,1));
	 img->Iedgecolor = atoi(imgfield(72,1));
	 img->Inr = atoi(imgfield(73,1));
	 img->Izeichne_on = atoi(imgfield(74,1));
	 img->Ischnittp1_koord[0] = atof(imgfield(75,1));
	 img->Ischnittp1_koord[1] = atof(imgfield(75,2));
	 img->Ischnittp1_koord[2] = atof(imgfield(75,3));
	 img->Ischnittp2_koord[0] = atof(imgfield(76,1));
	 img->Ischnittp2_koord[1] = atof(imgfield(76,2));
	 img->Ischnittp2_koord[2] = atof(imgfield(76,3));
	 img->Ischnittp3_koord[0] = atof(imgfield(77,1));
	 img->Ischnittp3_koord[1] = atof(imgfield(77,2));
	 img->Ischnittp3_koord[2] = atof(imgfield(77,3));
	 img->Inorm[0] = atof(imgfield(78,1));
	 img->Inorm[1] = atof(imgfield(78,2));
	 img->Inorm[2] = atof(imgfield(78,3));
	 img->Izentrier_vector[0] = atof(imgfield(79,1));
	 img->Izentrier_vector[1] = atof(imgfield(79,2));
	 img->Izentrier_vector[2] = atof(imgfield(79,3));
	 sprintf(img->Idateiname[0],"%s", imgfield(80,1));
	 sprintf(img->Idateiname[1],"%s", imgfield(80,2));
	 img->Izyklus_achse = (char)(imgfield(81,1));
}
