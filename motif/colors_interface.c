/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor & Toni Ivas                           */
/* Datum    :   29.06.01                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname                         | Funktion               */
/* ------------------------------------------------------------- */
/*                                      |                        */
/*                                      |                        */
/*                                      |                        */
/*                                      |                        */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */ 


#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>

/* Header data for diffrent widget classes */
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/Scale.h>
#include <X11/Shell.h>

/* Header data of program */
#include "color3d.h"
#include "grafik.h"
#include "motif.h"
#include "uil.h"
#include "strukturen.h"
//#define DEBUG 1
#ifdef DEBUG
  #define DEBUGOUT(fmt, args...) \
	 fprintf(stderr, "%s(%d)"fmt"\n", __FILE__, __LINE__, ##args)
#else
  #define DEBUGOUT(str) 
#endif

/* Definitions of all callbacks */
extern t_color chcolor;
t_color_extra g_color_extra;
Boolean onTex=True, offTex;
static t_bool clip;
static Widget colorMainWidget;
GC gc;

/*****************************************************************
 Purpose:
 =======
       This function is interface to initialize color dialog box.
******************************************************************/
/* int init_extra_colors() */
/* { */
   /* local variables */ 
/*   Widget w, w1; */

/*   colorMainWidget = XtVaCreateWidget( */
/* 			     "S_color", */
/* 			     transientShellWidgetClass, toplevel, */
/* 			     XmNminWidth, 230, XmNminHeight, 500, */
/* 			     XmNx, 45, XmNy, 190,NULL , 0); */
  
/*   XtManageChild(colorMainWidget); */
/*   return 0; */
/* } */

void colorsDialogCB(Widget w,XtPointer clientData,
		     XtPointer callData)
{
  MrmType  topWidgetClass;

  /* create main color-extra widget */
  if ( colorMainWidget != NULL )
    {
      XtPopup(colorMainWidget, XtGrabNone);
      return;
    }

  if((colorMainWidget == NULL) &&
     (MrmFetchWidget(hierarchy, "topWidget", toplevel,
			  &colorMainWidget, &topWidgetClass)) != MrmSUCCESS )
    {
      fprintf(stderr, "Colors Dialog can't be loaded\n");
    }
  
  XtManageChild(colorMainWidget);
}

/*****************************************************************
 Purpose:
 =======
        This callback load file with data for colors.
        It open file selection box.
******************************************************************/
void loadColorCB(Widget w, 
			XtPointer clientData, 
			XtPointer callData)
{
  /* load file selection dialog */
  XmFileSelectionDoSearch (fsdWidget, (XmString)NULL);
  XtManageChild (fsdWidget);
}

/*****************************************************************
 Purpose: 
 ========
        This callback save a current colors  as default, so
	every time we open new sesion of color3d we will
        have this colors.
******************************************************************/
void defColorCB(Widget w, 
		XtPointer clientData, 
		XtPointer callData)
{

}

/*****************************************************************
 Purpose: 
 ========
        This callback choose color table.

******************************************************************/
void colorNameCB(Widget w, 
		 XtPointer clientData,
		 XtPointer callData)
{
  int col_table;
  col_table = *((int*) clientData);
  g_color_extra.color_name = col_table;

#ifdef DEBUG
  printf("Color table number: %i\n", col_table);
#endif

  /* make new color-table if texture is on */
  if (g_color_extra.texture_on) myinit(TEX_DEFAULT, TRUE); 

  /* update screen */
  C3D_postredisplay();
  /* update color-table */
  if (g_tabelle.on) zeichne_tabelle();
}

/*****************************************************************
 Purpose: 
 ========
        This callback set the texture mode.

******************************************************************/
void colorSetTexCB(Widget w,
                          XtPointer clientData,
			  XtPointer callData)
{
  XmToggleButtonCallbackStruct *cbs =
    (XmToggleButtonCallbackStruct *) callData;
  
  /* report the state of button */
  if(cbs->set)
    {
      onTex = *((int*)clientData);
      if(onTex)
	g_color_extra.texture_on = TRUE;
      else 
	{
	  offTex = !onTex;
	  g_color_extra.texture_on = FALSE;
	}
    }
#ifdef DEBUG
  printf("%s: %s\n", XtName(w), cbs->set ? "set":"unset");
#endif
}

/*****************************************************************
 Purpose: 
 ========
        This callback set the texture mode.

******************************************************************/
void setTexCB(Widget w, XtPointer clientData,
			     XtPointer callData)
{
  XtVaSetValues(w, XmNset, g_color_extra.texture_on, NULL);
}

/*****************************************************************
 Purpose: 
 ========
        This callback initialize default texture value.
	
******************************************************************/
void setOffTexCB(Widget w, XtPointer clientData,
			     XtPointer callData)
{
  XtVaSetValues(w, XmNset, !g_color_extra.texture_on, NULL);
}

/*****************************************************************
 Purpose: 
 ========
        This callback initialize transparency value.
	
******************************************************************/
void initSetTranspCB(Widget w,
		      XtPointer clientData,
		      XtPointer callData)
{
  XmScaleCallbackStruct *cbs = ( XmScaleCallbackStruct * )callData;
  g_color_extra.transparency = (float) cbs->value / 100.0;
}

/*****************************************************************
 Purpose: 
 ========
        This callback choose transparency value.
	
******************************************************************/
void colorSetTranspCB(Widget w,
		      XtPointer clientData,
		      XtPointer callData)
{
  XmScaleCallbackStruct *cbs = ( XmScaleCallbackStruct * )callData;
  g_color_extra.transparency = (float) cbs->value / 100.0;
}

/*****************************************************************
 Purpose: 
 ========
        This callback choose transparency color.
	
******************************************************************/
void chooseColorCB(Widget w,
		      XtPointer clientData,
		      XtPointer callData)
{
  Arg args[1];

#ifdef HAVE_SGI
  /* raise color-widget */
  Widget  colChooser = CreateColorChooser("S_trans_chooser", w, args, 1);
#endif
}

/*****************************************************************
 Purpose: 
 ========
        This function sets the transparency color.
	
******************************************************************/
int change_transp_col()
{

#ifdef HAVE_SGI
  /* take the color and make it color of the trans. */
  g_color_extra.trans_color[0] = chcolor.color[0];
  g_color_extra.trans_color[1] = chcolor.color[1];
  g_color_extra.trans_color[2] = chcolor.color[2];
  g_color_extra.trans_color_on = TRUE;
#endif

  return 1;
}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the ok button. It closes the 
	dialog window a set colors to choose ones.
******************************************************************/
void okColorsCB(Widget w,
		XtPointer clientData,
		XtPointer callData)
{
	applyColorsCB(w, clientData, callData);
	XtPopdown(colorMainWidget);
}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the cancel button. It closes the 
	dialog window but don't set new colors.
******************************************************************/
void cancelColorsCB(Widget w,
		    XtPointer clientData,
		    XtPointer callData)
{
	if(clip)
	{
		C3D_clip_tex_reset();
		clip = FALSE;
	}
	/* close dialog box */
	XtPopdown(colorMainWidget);
}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the apply button. It applies the 
	new colors to the model.
******************************************************************/
void applyColorsCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData)
{

	if (clip) 
	{
		C3D_clip_tex(g_color_extra.clipabove, g_color_extra.clipbellow);
	}
	/* update screen */
	C3D_postredisplay();

	/* update colortable */
	if (g_tabelle.on) zeichne_tabelle();
}

/****************************************************************
 Purpose:
 ========
        This callback sets the above clipping value.
*****************************************************************/ 
void clipAboveTexCB(Widget w, XtPointer clientData,
                           XtPointer callData)
{
	XmScaleCallbackStruct *cbs = ( XmScaleCallbackStruct * ) callData;
	DEBUGOUT("Clipping above callback\n");
	g_color_extra.clipabove = cbs->value;
	clip = TRUE;
}

/****************************************************************
 Purpose:
 ========
        This callback sets the bellow clipping value.
*****************************************************************/ 
void clipBellowTexCB(Widget w, XtPointer clientData,
                           XtPointer callData)
{
	XmScaleCallbackStruct *cbs = ( XmScaleCallbackStruct * ) callData;
	DEBUGOUT("Clipping bellow callback\n");
	g_color_extra.clipbellow = cbs->value;
	clip = TRUE;
}

/****************************************************************
 Purpose:
 ========
        This callback initialize clipping values.
*****************************************************************/ 
void initSetClipCB(Widget w, XtPointer clientData,
                           XtPointer callData)
{
	DEBUGOUT("Clipping init callback\n");	
	XtVaSetValues(w, XmNmaximum, (int)g_loesung.Max,
	                 XmNminimum, (int)g_loesung.Min,
                   NULL);
}

void initColorScale(Widget w, XtPointer clientData,
			   XtPointer callData)
{
}

void initColorWidget(Widget w, XtPointer clientData,
			   XtPointer callData)
{
  
  Pixel fg;
  XGCValues gcv;
  XColor screen_def, exact_def;
  Colormap cmap;
  Window root;
  unsigned long mask;
  unsigned long colors[MAX_COLORS];
  int ncolors = MAX_COLORS;
  unsigned long plane_masks[1];

  cmap = XDefaultColormapOfScreen(XtScreen(w));
  XtVaGetValues(w,
		XmNforeground, &fg,
		NULL);
  root = RootWindowOfScreen(XtScreen(w));
 /*
  while(1) {
    if (XAllocColorCells(XtDisplay(w), cmap, 
		       False, plane_masks, 0, colors, ncolors))
      break;
    ncolors--;
    if (ncolors == 0)
      fprintf(stderr, "Couldn't allocate Read / Write colors\n");
  }
  printf("colors_interface: allocated %d read/write color cells\n", ncolors);
  */
  gcv.cap_style = CapRound;
  gcv.fill_style = FillSolid;
  gcv.foreground = screen_def.pixel;
  mask = GCForeground | GCCapStyle | GCFillStyle;
  
  gc = XCreateGC(XtDisplay(w), root, mask, &gcv);

  XtAddCallback(w, XmNexposeCallback, 
		exposeColorScale, (XtPointer) gc);
}

void exposeColorScale(Widget w, XtPointer clientData,
			     XtPointer callData)
{
  XmDrawingAreaCallbackStruct *evp;
  Dimension width, height;
  int x, y, xold;

  evp = (XmDrawingAreaCallbackStruct*) callData;
  if (evp->event->xexpose.count != 0)
    return;
  /* get the size of color scale */
  XtVaGetValues(w, 
		XmNwidth, &width,
		XmNheight, &height,
		NULL);
  
  
  /* draw lines test */
  xold = 0;
  for (x = 0; x < width; x += 10)
    {
      XFillRectangle(XtDisplay(w), XtWindow(w),
		     (GC)gc, xold, 0, x, height);
      xold += 11;
    }
  
  for (y = 10; y < height; y += 10)
    XDrawLine(XtDisplay(w), XtWindow(w),
	      (GC)gc, 0, height, width, y);

}







