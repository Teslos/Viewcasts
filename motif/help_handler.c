
#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>

#include "color3d.h"
#include "motif.h"
/*
 *		forward declarations
**/
void pb_help_ok_CB(Widget widget, Widget call_data, XtPointer cbs);
/*************************************************************************/
/*
 *	Fuellt einen String mit dem Hilfetext
**/
void help_handler(int hilfe_nummer)
{
  extern Widget toplevel;
  static Widget hilfe_shell = NULL;
  static Widget form, pb_ok, scrolled_text;
  Arg args[5];
  int nargs;
  Atom wm_delete_window;
  static char dateiname[1024];
  static char hilfstr[4096];
  int result;
  static char *string = (char *)NULL;

  if (!hilfe_shell)
  {
    hilfe_shell = XtVaCreatePopupShell("hilfe_shell",
                     transientShellWidgetClass, toplevel,
		     XmNminWidth, 800, XmNminHeight, 500,
		     XmNx, 120, XmNy, 120, NULL);

    form = XtVaCreateWidget("form", xmFormWidgetClass, hilfe_shell,
		     XmNhorizontalSpacing, 5,
		     XmNverticalSpacing, 5, NULL);

    pb_ok = XtVaCreateManagedWidget("pb_ok", xmPushButtonGadgetClass, form,
		     XmNleftAttachment, XmATTACH_FORM,
		     XmNrightAttachment, XmATTACH_FORM,
		     XmNbottomAttachment, XmATTACH_FORM,
		     XmNmarginHeight, 4, NULL);

    XtAddCallback(pb_ok, XmNactivateCallback, 
           (XtCallbackProc)pb_help_ok_CB, (XtPointer)hilfe_shell);

    nargs = 0;
    XtSetArg(args[nargs], XmNeditable, False); nargs++;
    XtSetArg(args[nargs], XmNeditMode, XmMULTI_LINE_EDIT); nargs++;
    scrolled_text = XmCreateScrolledText(form,"scrolled_text",args,nargs);

    XtManageChild(scrolled_text);

    XtVaSetValues(XtParent(scrolled_text),
    	             XmNleftAttachment, XmATTACH_FORM,
		     XmNtopAttachment, XmATTACH_FORM,
		     XmNbottomAttachment, XmATTACH_WIDGET,
		     XmNbottomWidget, pb_ok,
		     XmNrightAttachment, XmATTACH_FORM, NULL);

/* window-manager delete-button */
    XtVaSetValues(hilfe_shell,XmNdeleteResponse, XmDO_NOTHING, NULL, NULL);
    wm_delete_window = XmInternAtom(XtDisplay(hilfe_shell), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(hilfe_shell, wm_delete_window,
    	(XtCallbackProc)pb_help_ok_CB, (XtPointer)hilfe_shell);
  }


  sprintf(dateiname, "%s/help_text.%d.%d", help_dir_buffer,
          g_sprache, hilfe_nummer);
  if ((result = file_2_string(dateiname, &string)))
  {
    sprintf(hilfstr, "file_2_string(>%s<, ...) returns value = >%d<\n",
          dateiname, result);
    warning(hilfstr);
    return;
  }

  XmTextSetString(scrolled_text, string);
  XtFree(string);

  XtManageChild(form);
  XtPopup(hilfe_shell, XtGrabExclusive);

  return;
}
/************************************************************************/
/*
 *
**/
void pb_help_ok_CB(Widget widget, Widget call_data, XtPointer cbs)
{
  XtPopdown(call_data);
  return;
}
/************************************************************************/
