
#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <TextTable.h>
#include <TableMan.h>

#include "uil.h"

/* UID - data */
static char *data[] = {
  "light_ui.uid",
  "fileSel.uid",
  "colors_ui.uid",
  "main_mod.uid",
  "cut.uid"
};

/* callbacks */
static MRMRegisterArg callbacks[] = {
  {"loadLightCB", (XtPointer)loadLightCB},
  {"saveAsLightCB", (XtPointer)saveAsLightCB},
  {"defLightCB", (XtPointer)defLightCB},
  {"lightModelCB", (XtPointer)lightModelCB},
  {"lightSetBrightCB", (XtPointer)lightSetBrightCB},
  {"lightSetRedCB", (XtPointer)lightSetRedCB},
  {"lightSetGreenCB", (XtPointer)lightSetGreenCB},
  {"lightSetBlueCB", (XtPointer)lightSetBlueCB},
  {"setLightNumCB", (XtPointer)setLightNumCB},
  {"eyeCB", (XtPointer) eyeCB},
  {"nolightCB",(XtPointer) nolightCB},
  {"customCB", (XtPointer) customCB},
  {"standardCB", (XtPointer) standardCB},
  {"initOGLWidgetCB", (XtPointer)initOGLWidgetCB},
  {"exposeCB", (XtPointer)exposeCB},
  {"initCB", (XtPointer)initCB},
  {"okLightCB",(XtPointer)okLightCB},
  {"cancelLightCB", (XtPointer) cancelLightCB},
  {"applyLightCB", (XtPointer) applyLightCB},
  
  /* from fileSel.uil*/
  {"fsdInitCB", (XtPointer) fsdInitCB},
  {"fsdFinishCB", (XtPointer) fsdFinishCB},
  {"unmanageCB",  (XtPointer)unmanageCB},
  
  /* from colors_ui.uil*/
  {"loadColorCB", (XtPointer)loadColorCB},
  {"defColorCB", (XtPointer)defColorCB},
  {"colorNameCB", (XtPointer)colorNameCB},
  {"colorSetTexCB", (XtPointer)colorSetTexCB},
  {"colorSetTranspCB", (XtPointer)colorSetTranspCB},
  {"chooseColorCB", (XtPointer)chooseColorCB},
  {"initSetTranspCB", (XtPointer)initSetTranspCB},
  {"okColorsCB", (XtPointer)okColorsCB},
  {"cancelColorsCB", (XtPointer)cancelColorsCB},
  {"applyColorsCB", (XtPointer)applyColorsCB},
  {"initColorWidget", (XtPointer)initColorWidget},
  {"initColorScale", (XtPointer)initColorScale},
  {"exposeColorScale",(XtPointer)exposeColorScale},
  {"setTexCB", (XtPointer) setTexCB},
  {"setOffTexCB", (XtPointer) setOffTexCB},
  {"onTex",(XtPointer) &onTex},
  {"offTex",(XtPointer) &offTex},
  {"initSetClipCB",(XtPointer) initSetClipCB},
  {"clipAboveTexCB",(XtPointer) clipAboveTexCB},
  {"clipBellowTexCB",(XtPointer) clipBellowTexCB},
 
  /* from main_mod.uil */
  {"colorsDialogCB", (XtPointer) colorsDialogCB},
  {"shadingCB", (XtPointer) shadingCB},
  {"shadingDialogCB", (XtPointer) shadingDialogCB},
  {"create_cutShellCB", (XtPointer) create_cutShellCB},
  
  /* from cut.uil */
  {"setsliceCB", (XtPointer) setsliceCB},
  {"addsliceCB", (XtPointer) addsliceCB},
  {"deletesliceCB", (XtPointer) deletesliceCB},
  {"setoutlineCB", (XtPointer) setoutlineCB},
  {"changeoutcolorCB", (XtPointer) changeoutcolorCB},
  {"setmaterialsCB", (XtPointer) setmaterialsCB},
  {"setcolorsCB", (XtPointer) setcolorsCB},
  {"getmaterialCB", (XtPointer) getmaterialCB},
  {"cutapplyCB", (XtPointer) cutapplyCB},
  {"cutcloseCB", (XtPointer) cutcloseCB},
  {"cuthelpCB", (XtPointer) cuthelpCB},
  {"initrotxCB", (XtPointer) initrotxCB},
  {"initrotyCB", (XtPointer) initrotyCB},
  {"initrotzCB", (XtPointer) initrotzCB},
  {"changeXRCB", (XtPointer) changeXRCB},
  {"changeYRCB", (XtPointer) changeYRCB},
  {"changeZRCB", (XtPointer) changeZRCB}
};

MrmHierarchy  hierarchy;

/*******************************************************************
 Purpose:
 ========
        This routine initialize the Widget tree

 Parameters:
 ==========
        Widget parent - widget which is the parent.

 Returns:
 ========
       This routine returns 0 if success, -1 if failure.

*******************************************************************/

int initWidgetTree(Widget parent)
{
  /* local variables */
  Widget topWidget, cutWidget;              /* top widget from UIL */
  MrmType topWidgetClass, cutWidgetClass;   /* class of topWidget */
  Cardinal status;                          /* return status */
 
  /* MRM initialization */
  MrmInitialize();
    
  /* open MRM-hierarchy with UID-data */
  status = MrmOpenHierarchyPerDisplay(XtDisplay(parent),
				      XtNumber(data), data,
				      (MrmOsOpenParamPtr*) NULL,
				      &hierarchy);

  if(status != MrmSUCCESS)
    {
      fprintf(stderr, "UID-Data can't be read\n");
      return(-1);
    }
  
  /* register the callbacks for UIL   */
  status = MrmRegisterNamesInHierarchy(hierarchy,
				       callbacks, XtNumber(callbacks));
  if(status != MrmSUCCESS)
    {
      fprintf(stderr, 
	      "Callbacks can't be register for UIL widgets.\n");
      return(-1);
    }
  
  /* make widget hierarchy from topwidget */
  topWidget = NULL;
  status = MrmFetchWidget(hierarchy, "extra", parent,
			  &topWidget, &topWidgetClass);
  if(status != MrmSUCCESS)
    {
      fprintf(stderr, 
	      "Widget hierarchy can't be build for UIL widgets.\n");
      return(-1);
    }

  /* register userdefined widgets */
  if (MrmRegisterClass(MrmwcUnknown, "BXmTexTable" , "BXmCreateTextTable",
                            (Widget(*)())BXmCreateTextTable,
                            (WidgetClass)&bxmTextTableWidgetClass) != MrmSUCCESS)
      fprintf(stderr, "User-defined widget can't be registered!\n");

  /* make widget cut */
  status = MrmFetchWidget(hierarchy, "cut", parent,
              &cutWidget, &cutWidgetClass);

  if(status != MrmSUCCESS)
    {
      fprintf(stderr, "Widget hierarchy can't be build for cut widgets.\n");
      return(-1);
    } 
  
  /* manage topwidget */
  XtManageChild(topWidget);
  XtManageChild(cutWidget);
  return 0;
}         
   
