#ifndef _hilfs_func_h
#define _hilfs_func_h

#include <stdio.h>
#include <Xm/Xm.h>

/******************************************************************************/
int file_exist(char *datei);
int file_read(char *datei);
int file_write(char *datei);
int file_2_string(char *datei, char **string);

Widget my_XtNameToWidget(Widget widget, char *widget_string, int stderr_out);
void my_XmToggleButtonSetState(Widget widget, int state, int notify);
int sensitive_and_on(Widget widget);
/******************************************************************************/
#endif   /* #ifndef _hilfs_func_h */
