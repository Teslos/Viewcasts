#ifndef NO_MOTIF

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include <Xm/DialogS.h>
/* ********************************************************************* */
#define ASK_USER_YES    1
#define ASK_USER_NO     2
#define ASK_USER_CANCEL 3
/* ********************************************************************* */

/* ********************************************************************* */
void ask_user_response(Widget widget, int *answer, XmAnyCallbackStruct *cbs)
/* ********************************************************************* */
{
  if (cbs->reason == XmCR_OK)
    *answer = ASK_USER_YES;
  else if (cbs->reason == XmCR_CANCEL)
    *answer = ASK_USER_NO;
  else if (cbs->reason == XmCR_HELP)
    *answer = ASK_USER_CANCEL;

  return;
}

/* ********************************************************************* */
char *ask_user(char *text, char *ans1, char *ans2, char *ans3,
               int default_ans)
/* ********************************************************************* */
{
  extern Widget toplevel;
  extern XtAppContext kontext;
  static Widget dialog = NULL;
  static int answer;
  XmString xm_text, xm_ans1, xm_ans2, xm_ans3;

  if (dialog == NULL)
  {
    dialog = XmCreateQuestionDialog(toplevel, "dialog", NULL, 0);
    XtVaSetValues(dialog, XmNdialogStyle,
                  XmDIALOG_FULL_APPLICATION_MODAL, NULL);

    XtAddCallback(dialog, XmNokCallback,
                  (XtCallbackProc)ask_user_response, (XtPointer)&answer);
    XtAddCallback(dialog, XmNcancelCallback,
                  (XtCallbackProc)ask_user_response, (XtPointer)&answer);
    XtAddCallback(dialog, XmNhelpCallback,
                  (XtCallbackProc)ask_user_response, (XtPointer)&answer);
  }
  answer = 0;

  xm_text = XmStringCreateLtoR(text, XmSTRING_DEFAULT_CHARSET);
  xm_ans1 = XmStringCreateSimple(ans1);
  xm_ans2 = XmStringCreateSimple(ans2);
  xm_ans3 = XmStringCreateSimple(ans3);

  XtVaSetValues(dialog, XmNmessageString, xm_text, XmNokLabelString,
                xm_ans1, XmNcancelLabelString, xm_ans2, XmNhelpLabelString,
                xm_ans3, NULL);

/* setze Defaultbutton ------------------------------------------------- */
  if (default_ans == 1)
    XtVaSetValues(dialog, XmNdefaultButtonType, XmDIALOG_OK_BUTTON, NULL);
  else if (default_ans == 2)
    XtVaSetValues(dialog,XmNdefaultButtonType,XmDIALOG_CANCEL_BUTTON,NULL);
  else if (default_ans == 3)
    XtVaSetValues(dialog,XmNdefaultButtonType,XmDIALOG_HELP_BUTTON,NULL);

  XmStringFree(xm_text);
  XmStringFree(xm_ans1);
  XmStringFree(xm_ans2);
  XmStringFree(xm_ans3);

  if ( (ans2 == NULL) || (strlen(ans2) == 0) )
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
  if ( (ans3 == NULL) || (strlen(ans3) == 0) )
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));

  XtManageChild(dialog);
  XtPopup(XtParent(dialog), XtGrabExclusive);

  while (answer == 0)
    XtAppProcessEvent(kontext, XtIMAll);

  XtPopdown(XtParent(dialog));

  XSync(XtDisplay(dialog), 0);
  XmUpdateDisplay(toplevel);

  if (answer == ASK_USER_YES)
    return(ans1);
  else if (answer == ASK_USER_NO)
    return(ans2);
  else if (answer == ASK_USER_CANCEL)
    return(ans3);
  else
    return("");
}
/* ********************************************************************* */
#endif /* #ifndef NO_MOTIF */
