#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
/* X11 header --------------------------------------------------------- */
#include <X11/cursorfont.h>

#include "color3d.h"
#include "motif.h"

/* ******************************************************************* */
void set_watch_cursor_handler(Widget widget)
/* ******************************************************************* */
{
  static Cursor watch_cursor = (Cursor)NULL;

  if(!watch_cursor)
  	watch_cursor = XCreateFontCursor(XtDisplay(widget), XC_watch);

  XDefineCursor(XtDisplay(widget), XtWindow(widget), watch_cursor);
  XmUpdateDisplay(widget);

  return;
/* ******************************************************************* */
}

/* ******************************************************************* */
void set_cursor_handler(Widget widget, int cursor)
/* ******************************************************************* */
{
  static Cursor translate_cursor = (Cursor)NULL;
  static Cursor rotate_cursor;
  static Cursor zoom_cursor;
  static Cursor pick_cursor;

  if (translate_cursor == (Cursor)NULL)
  {
    translate_cursor = XCreateFontCursor(XtDisplay(widget), XC_fleur);
    rotate_cursor =    XCreateFontCursor(XtDisplay(widget), XC_exchange);
    zoom_cursor =      XCreateFontCursor(XtDisplay(widget), XC_ul_angle);
    pick_cursor =      XCreateFontCursor(XtDisplay(widget), XC_crosshair);
  }

  switch(cursor)
  {
    case TRANSLATE_CURSOR:
    {
      XDefineCursor(XtDisplay(widget), XtWindow(widget), translate_cursor);
      break;
    }
    case ROTATE_CURSOR:
    {
      XDefineCursor(XtDisplay(widget), XtWindow(widget), rotate_cursor);
      break;
    }
    case ZOOM_CURSOR:
    {
      XDefineCursor(XtDisplay(widget), XtWindow(widget), zoom_cursor);
      break;
    }
    case PICK_CURSOR:
    {
      XDefineCursor(XtDisplay(widget), XtWindow(widget), pick_cursor);
      break;
    }
    case WAIT_CURSOR:
    {
      XDefineCursor(XtDisplay(widget), XtWindow(widget), pick_cursor);
      break;
    }
  }
  XmUpdateDisplay(widget);

  return;
/* ******************************************************************* */
}

/* ******************************************************************* */
void reset_cursor_handler(Widget widget)
/* ******************************************************************* */
{
  XUndefineCursor(XtDisplay(widget), XtWindow(widget));
  XmUpdateDisplay(widget);

  return;
/* ******************************************************************* */
}
