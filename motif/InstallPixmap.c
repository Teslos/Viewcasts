#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <X11/xpm.h>
#include "motif.h"


void InstallPixmap(Widget w, char **xpmDescription)
{
	Pixmap pixmap, armPixmap;
    XpmAttributes attributes;
    XpmColorSymbol symbols[2];
    Pixel color;
    Display *display;
    XpmImage xpmimage;
    int status;
  
    display = XtDisplay(w);

    /* Retrive the values used by the given widget */
    XtVaGetValues (w,
                   XmNbackground, &color,
                   NULL);

    symbols[0].name = NULL;
    symbols[0].value = "none";
    symbols[0].pixel = color;

    attributes.colorsymbols = symbols;
    attributes.numsymbols = 1;
    attributes.valuemask = XpmColorSymbols;
    status = XpmCreatePixmapFromData( display,
                                      DefaultRootWindow(display),
                                      xpmDescription, &pixmap,
                                      NULL, &attributes);

     if (status != XpmSuccess) {
         fprintf(stderr, "XpmError: %s\n", XpmGetErrorString(status));
     }

     XtVaSetValues( w,
                    XmNlabelType, XmPIXMAP,
                    XmNlabelPixmap, pixmap,
                    XmNmarginWidth, 0,
                    XmNmarginHeight, 0,
                    NULL);
 
}
