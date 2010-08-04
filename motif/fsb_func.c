/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:                                                    */
/*                                                               */
/* Routinenname      | Funktion                                  */
/* ------------------------------------------------------------- */
/*  fsb_cb           |  FSB Callback                             */
/*  fsb_help_cb      |  Aufruf d. Hilfeseite fuer FSB            */
/*  file_dir_auswahl |  Aufruf FSB                               */
/* ------------------------------------------------------------- */
/* ============================================================= */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Xm/Xm.h>

#include <Xm/FileSB.h>
#include <Xm/List.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <X11/Shell.h>
#include "Row.h"
#include "motif.h"
#include "memory.h"
#include "color3d.h"
#include "../graphics/common.h"

/*                                                               */
/* globale Variable / Definitionen                               */
/* ==================                                            */
#define FSB_OK      1
#define FSB_CANCEL -1
#define FSB_APPLY  -2
char *fsb_datei_verzeichnis;
static Widget fsb;
static char *ext_files[] = {"*.tmpp", "*.spxp", "*.spyp", "*.spzp", "*.vspp"};
static char *result_files[] = { "Temperature", "Stress-X", "Stress-Y",
                          "Stress-Z", "Von Misses Stress" };
static char *get_result_names( Widget w);
static char *get_extension_names( Widget w );
static XmString *get_xmstring_names( char *str, int *nf );

/*                                                               */
/* Routinen :                                                    */
/* ============================================================= */

/*************************************************************************/
/*
 *
**/
void fsb_cb(Widget widget, int *answer, XmFileSelectionBoxCallbackStruct *cbs)
{
  int width;

  if (cbs->reason == XmCR_OK)
  {
    if(fsb_datei_verzeichnis != NULL)
       free(fsb_datei_verzeichnis);
    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &fsb_datei_verzeichnis);
    *answer = FSB_OK;
  }
    else if (cbs->reason == XmCR_CANCEL)
  {
    *answer = FSB_CANCEL;
  }
  else if (cbs->reason == XmCR_APPLY)
  {
    /* notwendig, da FSB resize nicht ok */
    XtVaGetValues(widget, XmNwidth, &width, NULL);
    XtVaSetValues(widget, XmNwidth, width++, NULL);
  }

  return;
}
/*************************************************************************/
/*
 *
**/
void fsb_help_cb(Widget widget, int hilfe_nummer, XtPointer call_data)
{
  help_handler(hilfe_nummer);
  return;
}

static void BrowseCallback(Widget w, XtPointer clientData, XtPointer callData)
{
   XmListCallbackStruct *cb = ( XmListCallbackStruct* ) callData;
   char *text;
   int i, nf, nexten;
   XmString *xmstra, *xmext;
   char *extension_names;

   xmstra = (XmString *) clientData;
   XtVaGetValues ( w, XmNitemCount, &nf, NULL);
  
   extension_names = get_extension_names ( w );
   xmext           = get_xmstring_names ( extension_names, &nexten );
   if ( nexten != nf )
     warning ("Number of filenames and file extensions not the same.\n Please update resource file \n");
   for (i = 0; i < nf; i++) {
   	 if (XmStringCompare(xmstra[i], cb->item)) {
	   XtVaSetValues(fsb, XmNpattern, xmext[i], NULL);
     }
   }
 }

	
/*************************************************************************/
/*
 *		
**/
char *file_dir_auswahl(char *header, char *header1, char *dirmask, int dir_only, 
		       int create_only, int create_list)
{
  /*extern Widget toplevel;*/
  /*extern XtAppContext kontext;*/
  XmString *xmstr, *xmext;
  XmString xmstring;
  XmString cancel;
  XmString help;
  XmString directories;
  XmString files; 
  char* result_names, *extension_names;
  XmString selection;
  static Widget Sfilesel=NULL;
  static Widget l_header, r_header, row, list;
  Widget xmform;
  static int answer;
  int i;
  int nfield, nexten;
  char title[128];
  char *base;
 
  /* erzeuge Widgets */
  /*if (!XtIsTransientShell(Sfilesel))
  {*/
    Sfilesel = XtVaCreatePopupShell("S_filesel", transientShellWidgetClass,
                                      toplevel, XmNallowShellResize, TRUE,
                                      XmNdeleteResponse, XmUNMAP,
                                      XmNminWidth, 600, XmNminHeight, 300,
                                      XmNx, 180, XmNy, 180,
                                      0, 0);

    xmform = XtVaCreateManagedWidget("XmForm", xmFormWidgetClass, Sfilesel,
                                      0, 0);

    l_header = XtVaCreateManagedWidget("L_Header", xmLabelWidgetClass,
                                        xmform, XmNleftAttachment,
                                        XmATTACH_FORM, XmNtopAttachment, 
                                        XmATTACH_FORM, XmNleftOffset, 10,
                                        XmNtopOffset, 10,
                                        0, 0);
    if ( create_list )
      {
	row = XtVaCreateManagedWidget("XmRow", xsRowWidgetClass,
				      xmform, XmNleftAttachment, XmATTACH_FORM,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopWidget, l_header,
				      XmNleftAttachment, XmATTACH_FORM,
				      XmNrightAttachment, XmATTACH_FORM,
				      XmNbottomAttachment, XmATTACH_FORM,
				      NULL, 0);

	fsb = XtVaCreateManagedWidget("FSB", xmFileSelectionBoxWidgetClass,
				      row, XmNlistVisibleItemCount, 10,
				      0, 0);

	r_header = XtVaCreateManagedWidget("R_Header", xmLabelWidgetClass,
					   xmform, XmNrightAttachment,
					   XmATTACH_FORM, XmNtopAttachment, 
					   XmATTACH_FORM, XmNrightOffset, 10,
					   XmNtopOffset, 10,
					   0, 0);
	
	/* get resources from resultfiles */
	result_names = get_result_names( Sfilesel );
	
	xmstr = get_xmstring_names( result_names, &nfield );
	
	
	list = XtVaCreateManagedWidget("fileSelList", xmListWidgetClass,
				       row, XmNitems, xmstr,
				       XmNitemCount, nfield,
				       XmNvisibleItemCount, 15, 0);

	/* add Callbacks to list widget */
	XtAddCallback(list, XmNbrowseSelectionCallback, 
		      BrowseCallback, (XtPointer)xmstr);
      }
    else
      fsb = XtVaCreateManagedWidget("FSB", xmFileSelectionBoxWidgetClass,
				    xmform, XmNlistVisibleItemCount, 10,
				    XmNtopWidget, l_header,
				    XmNleftAttachment, XmATTACH_FORM,
				    XmNrightAttachment, XmATTACH_FORM,
				    XmNbottomAttachment, XmATTACH_FORM,
				    0, 0);

   
    /* add Callbacks */
    XtAddCallback(fsb, XmNokCallback, (XtCallbackProc)fsb_cb,
                  (XtPointer)&answer);
    XtAddCallback(fsb, XmNcancelCallback, (XtCallbackProc)fsb_cb,
                  (XtPointer)&answer);
    XtAddCallback(fsb, XmNapplyCallback, (XtCallbackProc)fsb_cb,
                  (XtPointer)&answer);

    
  /*}*/
 
 
  /* falls "create_only == TRUE" */
  if (create_only)
  {
    XtPopup(Sfilesel,XtGrabExclusive);
    XtPopdown(Sfilesel);
    return("");
  }

  if (dir_only)
  {
  /* unmanage DIALOG_LIST_LABEL und DIALOG_LIST_SW */
    XtUnmanageChild(XmFileSelectionBoxGetChild(fsb, XmDIALOG_LIST_LABEL));
    XtUnmanageChild(XtParent(XmFileSelectionBoxGetChild(fsb, XmDIALOG_LIST)));
    XtRemoveAllCallbacks(fsb, XmNhelpCallback);
    XtAddCallback(fsb, XmNhelpCallback, (XtCallbackProc)fsb_help_cb,
                  (XtPointer)1100000);
  }
  else
  {
  /* manage DIALOG_LIST_LABEL und DIALOG_LIST_SW */
    XtManageChild(XmFileSelectionBoxGetChild(fsb, XmDIALOG_LIST_LABEL));
    XtManageChild(XtParent(XmFileSelectionBoxGetChild(fsb, XmDIALOG_LIST)));
    XtRemoveAllCallbacks(fsb, XmNhelpCallback);
    XtAddCallback(fsb, XmNhelpCallback, (XtCallbackProc)fsb_help_cb,
                  (XtPointer)1100001);
  }

  /* setze Header */
  xmstring = XmStringCreateLtoR(header, XmFONTLIST_DEFAULT_TAG);
  XtVaSetValues(l_header, XmNlabelString, xmstring, 0, 0);
  XmStringFree(xmstring);

  if ( header1 != NULL ) 
    {
      xmstring = XmStringCreateLtoR(header1, XmFONTLIST_DEFAULT_TAG);
      XtVaSetValues(r_header, XmNlabelString, xmstring, 0, 0);
      XmStringFree(xmstring);
    } 

  /* setze Title */
  XtVaSetValues(Sfilesel, XmNtitle, header, 0, 0);

  /* setze dirMask */
  xmstring = XmStringCreateLtoR(dirmask, XmFONTLIST_DEFAULT_TAG);
  XtVaSetValues(fsb, XmNdirMask, xmstring, 0, 0);

  /* start a directory search */
  XmFileSelectionDoSearch(fsb, xmstring);

  XmStringFree(xmstring);

  answer = 0;
  assert(XtIsTransientShell(Sfilesel));
  XtPopup(Sfilesel, XtGrabExclusive);

  while (answer == 0)
  XtAppProcessEvent(kontext, XtIMAll);

  XtPopdown(Sfilesel);

  XSync(XtDisplay(Sfilesel), 0);
  XmUpdateDisplay(toplevel);

  /* update main window title */
  base = SYS_basename( fsb_datei_verzeichnis );
  sprintf( title, "%s", "Viewcasts: " );
  strcat( title, base );
  C3D_setwindowtitle( XtDisplay( toplevel ), XtWindow( toplevel ),
		     title );

  if (answer == FSB_OK)
  {
    return(fsb_datei_verzeichnis);
  }
  else
    return("");
}

static char *
get_result_names( Widget w )
{
  static char *returnval = NULL;
  XtResource  request;

  request.resource_name = "names";
  request.resource_class = "Names";
  request.resource_type  = XmRString;
  request.resource_size  = sizeof(char *);
  request.default_type   = XmRImmediate;
  request.resource_offset = 0;
  request.default_addr   = ( XtPointer ) NULL;
  
  XtGetSubresources( w, (XtPointer) &returnval,
		    "resultfiles", "Resultfiles",
		    &request, 1, NULL, 0 );
  return returnval;
}

static char *
get_extension_names( Widget w )
{
  static char *returnval = NULL;
  XtResource  request;

  request.resource_name = "extensions";
  request.resource_class = "Extensions";
  request.resource_type  = XmRString;
  request.resource_size  = sizeof(char *);
  request.default_type   = XmRImmediate;
  request.resource_offset = 0;
  request.default_addr   = ( XtPointer ) NULL;
  
  XtGetSubresources( w, (XtPointer) &returnval,
		    "resultfiles", "Resultfiles",
		    &request, 1, NULL, 0 );
  return returnval;
}

static XmString *
get_xmstring_names( char *line, int *nf )
{
    char *p, **newf;
    char *sepp;
    int sepc,s;
    int nfield, i;
    int maxfield = 2;
    char *str;
    static char fieldsep[] = ",";
    static XmString *xmarray = NULL;
    char **field  = NULL;
    Boolean result;
  
    field = (char **) ALLOC(maxfield*sizeof(field[0]));
    if ( field == NULL ) return NULL;

    nfield = 0; 
    str = strdup(line);
    if ( str[0] == '\0')
      return NULL;
    p = str;

    do {
      if (nfield >= maxfield ) {
	maxfield *= 2;
	newf = (char **) RESIZE(field, maxfield * sizeof(field[0]));
	if (newf == NULL)
	  return NULL;
	field = newf;
      }
      sepp = p + strcspn(p, fieldsep);
      sepc = sepp[0];
      sepp[0] = '\0';
      field[nfield++] = p;
      p = sepp + 1;
    } while (sepc == ',');

   
    xmarray = (XmString *) XtMalloc( sizeof( XmString ) * nfield );
    if ( xmarray == NULL ) return NULL; 
    for (i = 0; i < nfield; i++) {
		xmarray[i] = XmStringCreateLtoR( field[i], XmFONTLIST_DEFAULT_TAG );
    }
    
    *nf = nfield;
    FREE(field);
    free(str);
    return xmarray;
}
	
/*************************************************************************/
