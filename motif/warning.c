#ifndef NO_MOTIF

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include <Xm/DialogS.h>
/************************************************************************/
#define WARNING_OK 1
/************************************************************************/
/*
*
*/
void warning_response(Widget widget, int *answer, XmAnyCallbackStruct *cbs)
{
  if (cbs->reason == XmCR_OK)
    *answer = WARNING_OK;

  return;
}
/************************************************************************/
/*
*
*/
void warning(char *message_string)
{
  extern Widget toplevel;
  extern XtAppContext kontext;
  static Widget dialog = NULL;
  static int answer;
  XmString xm_message_string;

  /* falls Hilfe noch nicht aufgerufen wurde Box erzeugen */
  if (dialog == NULL)
  {
    dialog = XmCreateWarningDialog(toplevel, "dialog", NULL, 0);
    XtVaSetValues(dialog, XmNdialogStyle, 
                  XmDIALOG_FULL_APPLICATION_MODAL, NULL);

    XtAddCallback(dialog, XmNokCallback, (XtCallbackProc)warning_response,
                  (XtPointer)&answer);

    XtSetSensitive(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON),
                   FALSE);
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
  }

  answer = 0;
  xm_message_string = XmStringCreateLocalized(message_string);
  XtVaSetValues(dialog, XmNmessageString, xm_message_string, NULL);
  XmStringFree(xm_message_string);

  XtManageChild(dialog);
  XtPopup(XtParent(dialog), XtGrabExclusive);

  while (answer == 0) XtAppProcessEvent(kontext, XtIMAll);

  XtPopdown(XtParent(dialog));

  XSync(XtDisplay(dialog), 0);
  XmUpdateDisplay(toplevel);

  return;
}
/************************************************************************/
#endif /* #ifndef NO_MOTIF */
