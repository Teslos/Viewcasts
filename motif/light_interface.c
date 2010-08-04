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
/* initWidgetTree                       |                        */
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
#include "shade.h"
#include "motif.h"
#include "uil.h"
/* Definition of all callbacks */
/*static void fsdInitCB(Widget w, 
	       XtPointer clientData, 
	       XtPointer callData);
static void unmanageCB(Widget w, 
		       XtPointer clientData, 
		       XtPointer callData);
static void fsdFinishCB(Widget w, 
			XtPointer clientData, 
			XtPointer callData);*/


static String fallbackResources[] = {
  "*load.labelString: Load...",
  "*saveAs.labelString: Save As...",
  "*default.labelString: Default",
  "*mainForm.horizontalSpacing:    4",
  "*mainForm.verticalSpacing:      4",
  "*ambient.labelString:  Ambient",
  "*diffuse.labelString:  Diffuse",
  "*spot.labelString:     Spot",
  "*brightness.width:     50",
  "*red.width:     50",
  "*green.width:     50",
  "*blue.width:     50",
  "*okLight.labelString: OK"
  "*cancelLight.labelString: Cancel",
  "*applyLight.labelString:  Apply",
  NULL
};

static Widget lightMainWidget = NULL;
static float lmodel_ambient;
static int bright = FALSE;
static lights *llights;
static lamp *currentlight;
enum {
  AMBIENT,
  DIFFUSE,
  SPOT
};


/*****************************************************************
 Purpose:
 =======
       This function is interface to initialize light dialog box.
******************************************************************/
void shadingCB(Widget w,XtPointer clientData,
	       XtPointer callData)
{
  
}
/*****************************************************************
 Purpose:
 =======
       This function is interface to initialize light dialog box.
******************************************************************/
void shadingDialogCB(Widget w,XtPointer clientData,
	       XtPointer callData)
{
  MrmType  topWidgetClass;
 
  /* create main light widget */
  /* if ( lightMainWidget != NULL ) */
/*     { */
/*       assert(XtIsShell(lightMainWidget)); */
/*       XtPopup(lightMainWidget, XtGrabNone); */
/*       return; */
/*     } */

  if(/*(lightMainWidget == NULL) &&*/
     (MrmFetchWidget(hierarchy, "topWidgetLight", toplevel,
			  &lightMainWidget, &topWidgetClass)) != MrmSUCCESS )
  {
    fprintf(stderr, "Shading Dialog can't be loaded\n");
  }

  XtManageChild(lightMainWidget);
  
}

/****************************************************************
 Purpose:
 ========
      Initialize the dialog box for selecting files
*****************************************************************/
void fsdInitCB(Widget w, 
	       XtPointer clientData, 
	       XtPointer callData)
{
  fsdWidget = w;
  
  /* Der Help-Button wird nicht verwendet
   */
  XtUnmanageChild (XmFileSelectionBoxGetChild (fsdWidget,
					       XmDIALOG_HELP_BUTTON));
}
/****************************************************************
 Purpose:
 ========
       Close the dialog box for selecting files
*****************************************************************/
void unmanageCB(Widget w, 
		       XtPointer clientData, 
		       XtPointer callData)
{
  /* close dialog box */
  //XtUnmanageChild(w);
}

/****************************************************************
 Purpose:
 ========
       Finish selecting the files from dialog box
*****************************************************************/
void fsdFinishCB(Widget w, 
		       XtPointer clientData, 
		       XtPointer callData)
{
}

/*****************************************************************
 Purpose:
 =======
        This callback load file with data for lighting.
        It open file selection box.
******************************************************************/
void loadLightCB(Widget w, 
			XtPointer clientData, 
			XtPointer callData)
{
  char *name;

  /* load file selection dialog */
  XmFileSelectionDoSearch (fsdWidget, (XmString)NULL);
  XtManageChild (fsdWidget);
  llights = lightsfromfile( name );
}

/*****************************************************************
 Purpose:
 =======
        This callback save a current lighting properties in 
	the file, so we can later again use it.
******************************************************************/
void saveAsLightCB(Widget w, 
			XtPointer clientData, 
			XtPointer callData)
{
  /* load file selection box */
  XmFileSelectionDoSearch (fsdWidget, (XmString)NULL);
  XtManageChild (fsdWidget);

}

/*****************************************************************
 Purpose: 
 ========
        This callback save a current lighting as default, so
	every time we open new sesion of color3d we will
        have this lighting.
******************************************************************/
void defLightCB(Widget w, 
		XtPointer clientData, 
		XtPointer callData)
{

}

/*****************************************************************
 Purpose: 
 ========
        This callback choose lighting model for different
	lights.
******************************************************************/
void lightModelCB(Widget w, 
		  int value,
		  XmToggleButtonCallbackStruct *callData)
{
  float r,g,b;

  XmToggleButtonCallbackStruct *cbs = callData;
  switch ( value )
    {
    case AMBIENT:
      setambient(r,g,b);
      break;
    case DIFFUSE:
      setdiffuse(r,g,b);
      break;
    case SPOT:
      break;
    default:
      break;
    }
  
}
/*****************************************************************
 Purpose: 
 ========
        This callback set brightness of light.
       
******************************************************************/
void lightSetBrightCB(Widget w,
		      XtPointer clientData,
		      XtPointer callData)
{
  XmScaleCallbackStruct *cb = (XmScaleCallbackStruct *)callData;
  if (cb->value > 0 && cb->value <= 100) 
    {
      lmodel_ambient = ( float )cb->value / 100.0;
      bright = TRUE;
    }

}
/*****************************************************************
 Purpose: 
 ========
        This callback set  amount of red color in light.
       
******************************************************************/
void lightSetRedCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData)
{

}


/*****************************************************************
 Purpose: 
 ========
        This callback set  amount of green color in light.
       
******************************************************************/
void lightSetGreenCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData)
{

}


/*****************************************************************
 Purpose: 
 ========
        This callback set  amount of blue color in light.
       
******************************************************************/
void lightSetBlueCB(Widget w,
		    XtPointer clientData,
		    XtPointer callData)
{

}


/*****************************************************************
 Purpose: 
 ========
       This callback set diffrent lights, choose number of 
       light.
******************************************************************/
void setLightNumCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData)
{
  int i;
  lamp *li;
  int numli;
  
  XmToggleButtonCallbackStruct *cbs = ( XmToggleButtonCallbackStruct *) callData;
  numli = *(int *) clientData;
  for ( i = 0, li = llights; (li != NULL || i == numli); i++, li = li->next );
  currentlight = li;
}

/*****************************************************************
 Purpose: 
 ========
        This callback sets custom settings for light
	widget.
******************************************************************/
void customCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData)
{
  
}
/*****************************************************************
 Purpose: 
 ========
        This callback sets standard settings for light
	widget.
******************************************************************/
void standardCB(Widget w,
		XtPointer clientData,
		XtPointer callData)
{
   b_light_on = TRUE;
   shadeon();
}
/*****************************************************************
 Purpose: 
 ========
        This callback sets standard settings for light
	widget.
******************************************************************/
void eyeCB(Widget w,
	   XtPointer clientData,
	   XtPointer callData)
{
}

/*****************************************************************
 Purpose:
 ========
        This callback sets no light settings for light
    widget.
******************************************************************/
void nolightCB(Widget w,
       XtPointer clientData,
       XtPointer callData)
{
#ifdef DEBUG
	printf ("I'm in no light callback\n");	
#endif
    b_light_on = FALSE;
	shadeoff();
}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the expose event for OpenGL
	widget.
******************************************************************/
void exposeCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData)
{

}

void initOGLWidgetCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData)
{

}
/*****************************************************************
 Purpose: 
 ========
        This callback handle the init event for OpenGL
	widget.
******************************************************************/
void initCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData)
{

}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the ok button. It closes the 
	dialog window a set light to choose one.
******************************************************************/
void okLightCB(Widget w,
		 XtPointer clientData,
		 XtPointer callData)
{
#ifdef DEBUG
  printf("I'm in light ok callback\n");
#endif
  if (bright)
    setambient( lmodel_ambient, lmodel_ambient, lmodel_ambient );

  /* update screen */
  C3D_postredisplay();
  XtPopdown(lightMainWidget);
}

/*****************************************************************
 Purpose: 
 ========
        This callback handle the cancel button. It closes the 
	dialog window but don't set new lighting.
******************************************************************/
void cancelLightCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData)
{
#ifdef DEBUG
  printf("I'm in light cancel callback\n");
#endif
  /* close dialog box */
  XtPopdown(lightMainWidget);
}
/*****************************************************************
 Purpose: 
 ========
        This callback handle the apply button. It applies the 
	new lighting to the model.
******************************************************************/
void applyLightCB(Widget w,
		    XtPointer clientData,
		    XtPointer callData)
{
#ifdef DEBUG
  printf("I'm in light apply callback\n");
#endif
  C3D_postredisplay();
}




   





