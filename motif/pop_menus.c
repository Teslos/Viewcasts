/*******************************************
 This routine makes handling of popup menus 
 for color3d.

 Author: Toni Ivas & Helmut Vor
 =======
 
 Warnings:
 ========
*******************************************/
#include <X11/keysym.h>
#include <Xm/Text.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <unistd.h>
#include "color3d.h"
#include "motif.h"
#include "grafik_func.h"
#include "grafik.h"
#include "sovLayerUtil.h"


static Widget popup;            /* popup-menu              */
static WidgetList menuWidgets;  /* list of the popup items */
int g_ClearColor;               /* sets background color   */
t_bool cutting_on = FALSE;

enum {
  DEFAULT_AASIZE = 9
};
	
enum {
  ZOOM_IN = 0,
  ZOOM_OUT,
  PERSPEKTIVE_VIEW,
  RESET,
  SAVE_PICTURE,
  SLICE,
  
  CROSS_DRAW = 7,
  SMOOTH_SCENE,
  AUTORANGE,
  BLEND_CONST,
  BLEND_ARB,
  BLEND_NONE,
  INTENSITY_FUL,
  INTENSITY_INC,
  INTENSITY_DEC,
  POLYGON_FACTOR_INC,
  POLYGON_FACTOR_DEC,
  POLYGON_BIAS_INC,
  POLYGON_BIAS_DEC,
  BACKGROUND_BLACK,
  BACKGROUND_WHITE,
  BACKGROUND_BLUEG
};

enum {
  AA_BLEND_CONST = 2,
  AA_BLEND_ARB,
  AA_OFF,
  INTENSITY_FULL = 6,
  INTENSITY_INCREASE,
  INTENSITY_DECREASE,
  
};

/* Functions */
void processMenuUse(Widget w, XtPointer clientData,
		    XtPointer callData);
void activateMenu(Widget w, XtPointer clientData, XEvent *event,
		  Boolean *cont);
void createCasPane(Widget parent);
void createCasPolygonOffset(Widget parent);
void createCasBackground( Widget parent );
/**********************************************************
 This file include the sources for popup menus.

***********************************************************/

void popMenuCreate(Widget parent)
{
  int n;                   /* number of args */
  Arg args[10];
  Widget submenu;
  static XmButtonType buttonTypes[] = {
    XmPUSHBUTTON, XmPUSHBUTTON, XmPUSHBUTTON, XmPUSHBUTTON, 
    XmPUSHBUTTON, XmCHECKBUTTON,
    XmSEPARATOR,  XmCHECKBUTTON, XmCHECKBUTTON, XmPUSHBUTTON, XmPUSHBUTTON};
  XmString buttonLabels[XtNumber(buttonTypes)];

  /* Create pop-up menu. */
  buttonLabels[0] = XmStringCreateLocalized("Zoom In");
  buttonLabels[1] = XmStringCreateLocalized("Zoom Out");
  buttonLabels[2] = XmStringCreateLocalized("Perspective");
  buttonLabels[3] = XmStringCreateLocalized("Reset");
  buttonLabels[4] = XmStringCreateLocalized("Save As Image");
  buttonLabels[5] = XmStringCreateLocalized("Slices");
  buttonLabels[6] = NULL;
 
  buttonLabels[7] = XmStringCreateLocalized("Pick");
  buttonLabels[8] = XmStringCreateLocalized("Show Origin");
  buttonLabels[9] = XmStringCreateLocalized("Smooth Scene");
  buttonLabels[10] = XmStringCreateLocalized("Auto-range"); 
 
  
  n = 0;
  XtSetArg(args[n], XmNbuttonCount, XtNumber(buttonTypes)); n++;
  XtSetArg(args[n], XmNbuttons, buttonLabels); n++;
  XtSetArg(args[n], XmNbuttonType, buttonTypes); n++;
  XtSetArg(args[n], XmNbuttonSet, 4); n++;
  XtSetArg(args[n], XmNsimpleCallback, processMenuUse); n++;
  if(C3D_win.overlayVisual) 
    {
      XtSetArg(args[n], XmNvisual, C3D_win.overlayVisual); n++;
      XtSetArg(args[n], XmNdepth, C3D_win.overlayDepth); n++;
      XtSetArg(args[n], XmNcolormap, C3D_win.overlayColormap); n++;
    }
  
  popup = XmCreateSimplePopupMenu(parent, "popup", args, n);
  XtAddEventHandler(parent, ButtonPressMask, False, activateMenu, &popup);
  
  createCasPane(popup);
  createCasBackground(popup);
  createCasPolygonOffset(popup);

  XmStringFree(buttonLabels[0]);
  XmStringFree(buttonLabels[1]);
  XmStringFree(buttonLabels[2]);
  XmStringFree(buttonLabels[3]);
  XmStringFree(buttonLabels[4]);
  XmStringFree(buttonLabels[5]);
  XmStringFree(buttonLabels[6]);
  XmStringFree(buttonLabels[7]);
  XmStringFree(buttonLabels[8]);
  XmStringFree(buttonLabels[9]);
 

  XtVaGetValues(popup, XmNchildren, &menuWidgets, NULL);
  
  /* set popup-menu to default values */
  XmToggleButtonSetState(menuWidgets[5], False, False);
//  XmToggleButtonSetState(menuWidgets[7], False, False);
  XmToggleButtonSetState(menuWidgets[8], True, False);
 /*
  XtSetSensitive( menuWidgets[5], False );
  XtSetSensitive( menuWidgets[7], False );
  XtSetSensitive( menuWidgets[9], False );
 */
}

/*******************************************************************
 This function ensures that overlay colormap is installed.		
*******************************************************************/
void activateMenu(Widget w, XtPointer clientData, XEvent *event,
	     Boolean *cont)
{
  if(C3D_win.overlayVisual)
    {
      XInstallColormap(C3D_win.dpy,C3D_win.overlayColormap);
    }
  XmMenuPosition(popup, &event->xbutton);
  XtManageChild(popup);
}

void processMenuUse(Widget w, XtPointer clientData, XtPointer callData)
{
  char *file_name;
  char num[3];
  static int numPicture=0;
  int button = (int) clientData;
    
  switch(button) {
    
  case ZOOM_IN:
    {
      g_transform.scale = g_transform.scale * 1.4;
      C3D_postredisplay();    /* neuzeichnen ---------------------- */
      break;
    }
  case ZOOM_OUT:
    {
      g_transform.scale = g_transform.scale * 0.7;
      C3D_postredisplay();    /* neuzeichnen ---------------------- */
      break;
    }
  case PERSPEKTIVE_VIEW:
    {
      g_projektion.perspektive = !g_projektion.perspektive;
      C3D_postredisplay();
      break;
    }
  case RESET:
    {
      reset();
      C3D_postredisplay();    /* neuzeichnen ---------------------- */
      break;
    }
  case SAVE_PICTURE:
    {
      size_t len_base;
      char *temp;
      /* make new file name */
      temp = strdup(g_loesung.dateiname);
      file_name = strtok(temp, ".");
      printf("File name: %s\n", file_name);
      ClearBuffers(2);
      C3D_postredisplay();    /* neuzeichnen ---------------------- */
      glFinish();
      glXWaitX();
      /* dump the picture in the file */
      if (!WindowDump(C3D_win.gl_width, C3D_win.gl_height, 0, -5, file_name))
	warning("This directory already contains tiff file(s) under the same name(s).\n To save the new file(s), first delete the existing one(s)");
      glDrawBuffer(GL_BACK);
      numPicture++;
      free(temp);
      break;
    }
  case SLICE:
    {
      printf("Capping in progress\n");
      cutting_on = !cutting_on;
      C3D_postredisplay();
      break;
    }
  case BACKGROUND_BLACK:
    {  
      g_ClearColor = 0;
      C3D_postredisplay();
      break;
    }
  case BACKGROUND_WHITE:
    {  
      g_ClearColor = 1;
      C3D_postredisplay();
      break;
    }
  case BACKGROUND_BLUEG:
    {  
      g_ClearColor = 3;
      C3D_postredisplay();
      break;
    }  
  case SMOOTH_SCENE:
    {
      glClearAccum(0.0, 0.0, 0.0, 0.0);
					
      /* antialiansing of the scene */
      displayAccum(DEFAULT_AASIZE);
      break;
    }
  case AUTORANGE:
    {
      C3D_localminmax(&g_loesung.Max, &g_loesung.Min);
      break;
    }
  case CROSS_DRAW:
    {
      g_transform.kreuz_on = !g_transform.kreuz_on;
      C3D_postredisplay();
      break;
    }
  case BLEND_CONST:
    {
      setmode(AA_BLEND_CONST);
      C3D_postredisplay();
      break;
    }
  case BLEND_ARB:
    {
      setmode(AA_BLEND_ARB);
      C3D_postredisplay();
      break;
    }
  case BLEND_NONE:
    {
      setmode(AA_OFF);
      C3D_postredisplay();
      break;
    }
  case INTENSITY_FUL:
    {
      setmode(INTENSITY_FULL);
      C3D_postredisplay();
      break;
    }
  case INTENSITY_INC:
    {
      setmode(INTENSITY_INCREASE);
      C3D_postredisplay();
      break;
    }
  case INTENSITY_DEC:
    {
      setmode(INTENSITY_DECREASE);
      C3D_postredisplay();
      break;
    }
  case POLYGON_FACTOR_INC:
    {
      g_factor += 0.1;
      break;
    }
  case POLYGON_FACTOR_DEC:
    {
      g_factor -= 0.1;
      break;
    }
  case POLYGON_BIAS_INC:
    {
      g_bias += 0.001;
      break;
    }
  case POLYGON_BIAS_DEC:
    {
      g_bias -= 0.001;
      break;
    } 
  default:
    break;
  }
}

void createCasBackground( Widget parent )
{
  Arg args[10];
  int n = 0;
  XmString buttonLabels[5];
  Widget submenu, cascade, bbackwhite, bbackblack,
  bbackblueg;
  
  submenu = XmCreatePulldownMenu( parent, "subBackground", NULL, 0 );
  
  buttonLabels[0] = XmStringCreateLocalized( "Background" );
  XtSetArg( args[n], XmNlabelString, buttonLabels[0] ); n++;
  XtSetArg( args[n], XmNsubMenuId, submenu ); n++;
  cascade = XtCreateManagedWidget( "SubBackground",
				  xmCascadeButtonWidgetClass,
				  parent,
				  args, n);
  
  buttonLabels[1] = XmStringCreateLocalized( "Black" );
  buttonLabels[2] = XmStringCreateLocalized( "White" );
  buttonLabels[3] = XmStringCreateLocalized( "Blue to white" );
  
  XtSetArg( args[n], XmNlabelString, buttonLabels[1] ); n++;
  bbackblack = XtCreateManagedWidget( "PB_black_background",
				     xmPushButtonWidgetClass,
				     submenu, args, n );
  
  XtSetArg( args[n], XmNlabelString, buttonLabels[2] ); n++;
  bbackwhite = XtCreateManagedWidget( "PB_white_background",
				     xmPushButtonWidgetClass,
				     submenu, args, n );
  
  XtSetArg( args[n], XmNlabelString, buttonLabels[3] ); n++;
  bbackblueg = XtCreateManagedWidget( "PB_blueg_background",
				     xmPushButtonWidgetClass,
				     submenu, args, n );
  XtAddCallback( bbackblack, XmNactivateCallback,
		processMenuUse, (XtPointer) BACKGROUND_BLACK );
  XtAddCallback( bbackwhite, XmNactivateCallback,
		processMenuUse, (XtPointer) BACKGROUND_WHITE );
  XtAddCallback( bbackblueg, XmNactivateCallback,
		processMenuUse, (XtPointer) BACKGROUND_BLUEG );	
}

void createCasPolygonOffset( Widget parent )
{
  Arg args[10];
  int n = 0;
  XmString buttonLabels[5];
  Widget submenu, cascade, bfactor_inc, bfactor_dec,
    bbias_inc, bbias_dec;
  
  submenu = XmCreatePulldownMenu(parent, "subPolygonOffset", NULL, 0);
  
  buttonLabels[0] = XmStringCreateLocalized("Polygon Offset");
  XtSetArg(args[n], XmNlabelString, buttonLabels[0]); n++;
  XtSetArg(args[n], XmNsubMenuId, submenu); n++;
  cascade = XtCreateManagedWidget("SubPolygonOffset",
				  xmCascadeButtonWidgetClass,
				  parent,
				  args,n);
  
  buttonLabels[1] = XmStringCreateLocalized("Increase factor");
  buttonLabels[2] = XmStringCreateLocalized("Decrease factor");
  buttonLabels[3] = XmStringCreateLocalized("Increase bias");
  buttonLabels[4] = XmStringCreateLocalized("Decrease bias");

  XtSetArg(args[n], XmNlabelString, buttonLabels[1]); n++;
  bfactor_inc = XtCreateManagedWidget("PB_factor_inc",
				     xmPushButtonWidgetClass,
				     submenu, args, n);
  
  XtSetArg(args[n], XmNlabelString, buttonLabels[2]); n++;
  bfactor_dec = XtCreateManagedWidget("PB_factor_dec",
				     xmPushButtonWidgetClass,
				     submenu, args, n);
  
  XtSetArg(args[n], XmNlabelString, buttonLabels[3]); n++;
  bbias_inc   = XtCreateManagedWidget("PB_bias_inc",
				     xmPushButtonWidgetClass,
				     submenu, args, n);
  
  XtSetArg(args[n], XmNlabelString, buttonLabels[4]); n++;
  bbias_dec   = XtCreateManagedWidget("PB_bias_dec",
				     xmPushButtonWidgetClass,
				     submenu, args, n);
  
  XtAddCallback(bfactor_inc, XmNactivateCallback,
		processMenuUse, (XtPointer) POLYGON_FACTOR_INC);
  XtAddCallback(bfactor_dec, XmNactivateCallback,
		processMenuUse, (XtPointer) POLYGON_FACTOR_DEC);
  XtAddCallback(bbias_inc, XmNactivateCallback,
		processMenuUse, (XtPointer) POLYGON_BIAS_INC);
  XtAddCallback(bbias_dec, XmNactivateCallback,
		processMenuUse, (XtPointer) POLYGON_BIAS_DEC);
  
}

void createCasPane( Widget parent )
{
  Arg args[10];
  int n = 0;
  XmString buttonLabels[8];
  Widget cascade, submenu, baa_const, baa_arb, baa_none,
    binten_full,binten_incr, binten_decr;
  
  submenu = XmCreatePulldownMenu(parent, "submenu", NULL, 0);
  
  buttonLabels[0] = XmStringCreateLocalized("Line Drawing");
  XtSetArg(args[n], XmNlabelString, buttonLabels[0]); n++;
  XtSetArg(args[n], XmNsubMenuId, submenu); n++;
  cascade = XtCreateManagedWidget("Submenu",
				  xmCascadeButtonWidgetClass,
				  parent,
				  args, n);
  
  buttonLabels[1] = XmStringCreateLocalized("Antialiansing, Blend Constant");
  buttonLabels[2] = XmStringCreateLocalized("Antialiansing, Blend Arbitrary");
  buttonLabels[3] = XmStringCreateLocalized("Disable Antialiansing ");
  buttonLabels[4] = XmStringCreateLocalized("Intensity full");
  buttonLabels[5] = XmStringCreateLocalized("Increase intensity");
  buttonLabels[6] = XmStringCreateLocalized("Decrease intensity");

  XtSetArg(args[n], XmNlabelString, buttonLabels[1]); n++;
  baa_const = XtCreateManagedWidget("PB_aa_const", 
				    xmPushButtonWidgetClass,
				    submenu, args, n);

  XtSetArg(args[n], XmNlabelString, buttonLabels[2]); n++;
  baa_arb   = XtCreateManagedWidget("PB_aa_arb",
				    xmPushButtonWidgetClass,
				    submenu, args, n);

  XtSetArg(args[n], XmNlabelString, buttonLabels[3]); n++;
  baa_none  = XtCreateManagedWidget("PB_aa_none",
				    xmPushButtonWidgetClass,
				    submenu, args, n);
  
  XtSetArg(args[n], XmNlabelString, buttonLabels[4]); n++;
  binten_full = XtCreateManagedWidget("PB_inten_full", 
				    xmPushButtonWidgetClass,
				    submenu, args, n);

  XtSetArg(args[n], XmNlabelString, buttonLabels[5]); n++;
  binten_incr   = XtCreateManagedWidget("PB_inten_incr",
				    xmPushButtonWidgetClass,
				    submenu, args, n);

  XtSetArg(args[n], XmNlabelString, buttonLabels[6]); n++;
  binten_decr  = XtCreateManagedWidget("PB_inten_decr",
				    xmPushButtonWidgetClass,
				    submenu, args, n);

  XtAddCallback(baa_const, XmNactivateCallback,
		processMenuUse, (XtPointer) BLEND_CONST);
  XtAddCallback(baa_arb, XmNactivateCallback,
		processMenuUse, (XtPointer) BLEND_ARB);
  XtAddCallback(baa_none, XmNactivateCallback,
		processMenuUse, (XtPointer) BLEND_NONE);
  XtAddCallback(binten_full, XmNactivateCallback,
		processMenuUse, (XtPointer)INTENSITY_FUL);
  XtAddCallback(binten_incr, XmNactivateCallback,
		processMenuUse, (XtPointer)INTENSITY_INC);
  XtAddCallback(binten_decr, XmNactivateCallback,
		processMenuUse, (XtPointer)INTENSITY_DEC);
}
