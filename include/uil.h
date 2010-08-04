/******************************************* 
This header contains the definitons
   of functions wich are used for
   UIL parts.
******************************************/
#ifndef __UIL_H__
#define __UIL_H__

#include "Mrm/MrmPublic.h"

void fsdInitCB(Widget w, 
	       XtPointer clientData, 
	       XtPointer callData);
void unmanageCB(Widget w, 
		XtPointer clientData, 
		XtPointer callData);
void fsdFinishCB(Widget w, 
		 XtPointer clientData, 
		 XtPointer callData);
int initWidgetTree(Widget parent); /* light_interface.c, colors_interface.c */

Widget fsdWidget;                  /* widget for dialog boxes */
extern MrmHierarchy hierarchy;            /* Mrm hierarchy-ID */

void loadLightCB(Widget w, 
		 XtPointer clientData, 
		 XtPointer callData);
void saveAsLightCB(Widget w, 
		   XtPointer clientData, 
		   XtPointer callData);
void defLightCB(Widget w, 
		XtPointer clientData, 
		XtPointer callData);
void lightModelCB(Widget w, 
		  int value,
		  XmToggleButtonCallbackStruct *callData);
void lightSetBrightCB(Widget w,
		      XtPointer clientData,
		      XtPointer callData);

void lightSetRedCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData);
void lightSetGreenCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData);
void lightSetBlueCB(Widget w,
		    XtPointer clientData,
		    XtPointer callData);
void setLightNumCB(Widget w,
		   XtPointer clientData,
		   XtPointer callData);
void customCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData);
void standardCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData);
void eyeCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData);
void nolightCB(Widget w,
          XtPointer clientData,
          XtPointer callData);
void exposeCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData);
void initOGLWidgetCB(Widget w,
		     XtPointer clientData,
		     XtPointer callData);
void initCB(Widget w,
	    XtPointer clientData,
	    XtPointer callData);
void okLightCB(Widget w,
	  XtPointer clientData,
	  XtPointer callData);
void cancelLightCB(Widget w,
	      XtPointer clientData,
	      XtPointer callData);
void applyLightCB(Widget w,
	     XtPointer clientData,
	     XtPointer callData);

/* identifiers from colors_ui.uil */
extern Boolean onTex, offTex;

void loadColorCB(Widget w,XtPointer clientData,
		 XtPointer callData);
void defColorCB(Widget w,XtPointer clientData,
		XtPointer callData);
void colorNameCB(Widget w, XtPointer clientData,
		 XtPointer callData);
void colorSetTexCB(Widget w,XtPointer clientData,
		   XtPointer callData);
void setTexCB(Widget w,XtPointer clientData,
	    XtPointer callData);
void setOffTexCB(Widget w,XtPointer clientData,
	    XtPointer callData);
void colorSetTranspCB(Widget w,XtPointer clientData,
		      XtPointer callData);
void chooseColorCB(Widget w,XtPointer clientData,
		      XtPointer callData);
void initSetTranspCB(Widget w,XtPointer clientData,
              XtPointer callData);
void initSetClipCB(Widget w,XtPointer clientData,
              XtPointer callData);
void clipAboveTexCB(Widget w,XtPointer clientData,
              XtPointer callData);
void clipBellowTexCB(Widget w,XtPointer clientData,
	      XtPointer callData);
void okColorsCB(Widget w,XtPointer clientData,
		XtPointer callData);
void cancelColorsCB(Widget w,XtPointer clientData,
		    XtPointer callData);
void applyColorsCB(Widget w,XtPointer clientData,
		   XtPointer callData);
void initColorScale(Widget w,XtPointer clientData,
		    XtPointer callData);
void exposeColorScale(Widget w,XtPointer clientData,
		      XtPointer callData);
void initColorWidget(Widget w,XtPointer clientData,
		     XtPointer callData);
void colorsDialogCB(Widget w,XtPointer clientData,
		     XtPointer callData);
void shadingCB(Widget w,XtPointer clientData,
	       XtPointer callData);
void shadingDialogCB(Widget w,XtPointer clientData,
	       XtPointer callData); 

/* callbacks from cut_interface.c */
void create_cutShellCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void cutapplyCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void cutcloseCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void cuthelpCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void setsliceCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void addsliceCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void deletesliceCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void setoutlineCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void changeoutcolorCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void setmaterialsCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void setcolorsCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void getmaterialCB(Widget parent, 
	XtPointer clientData, XtPointer callData);
void initrotxCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void initrotyCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void initrotzCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void changeXRCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void changeYRCB(Widget parent,
	XtPointer clientData, XtPointer callData);
void changeZRCB(Widget parent,
	XtPointer clientData, XtPointer callData);

#endif
