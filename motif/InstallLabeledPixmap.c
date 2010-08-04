#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <X11/xpm.h>
#include "motif.h"

#define MAX(a,b) (a>b ? a : b)
#define PAD -2
void InstallLabeledPixmap(Widget w, char **xpmDescription)
{
	XmString  label;
    XmFontList fontlist;
    GC gc;
    GC inverseGc;
    Dimension width, height;
    unsigned int pixmapWidth, pixmapHeight;
    XGCValues values;
    int junk, depth;
    Display *display;
    unsigned char alignment;
    XpmAttributes attributes;
    XpmColorSymbol symbols[5];
    int totalWidth, totalHeight;
    int status;
    Pixmap labelPixmap,
           xpmPixmap;
    Colormap cmap;

    display = XtDisplay( w );
  
  /* Retirive the values used by the given widget */
    XtVaGetValues (w,
                   XmNlabelString, &label,
                   XmNfontList, &fontlist,
                   XmNforeground, &values.foreground,
                   XmNbackground, &values.background,
                   XmNdepth, &depth,
                   XmNalignment, &alignment,
                   XmNcolormap, &cmap,
                   NULL);

   /* Create two GCs, one to draw the text and copy the pixmaps
    * and another that can be used ti erase a pixmap by filling 
    * it with the background color of the label widget. Beacuse
    * the font attribute of the GC, allocate this GC using 
    * XtAllocateGC() and specify GCFont  as modifiable.
    */

    gc = XtAllocateGC( w, depth,
                    GCForeground | GCBackground,
                    &values, GCFont, 0);
    
    values.foreground = values.background;
    
    inverseGc = XtGetGC( w,
                          GCForeground | GCBackground,
                         &values);
    
    /* Set up the XpmColorSymbol array, binding the name "background"
     * to the actual background color of the widget.
     */
     
    symbols[0].name = "background";
    symbols[0].value = NULL;
    symbols[0].pixel = values.background;
    symbols[1].name = NULL;
    symbols[1].value = "none";
    symbols[1].pixel = values.background;
     
     /*
      * Set the resulting information in the attributes structure
      */
     
    attributes.colorsymbols = symbols;
    attributes.numsymbols   = 2;

     /*
      * Specify the visual, colormap and depth
      * to be used and set the XpmAttributes mask.
      */
    
    attributes.colormap = cmap;
    attributes.depth = depth;
    attributes.closeness = 40000;
    attributes.visual = DefaultVisual(display,
                                       DefaultScreen(display));
    
    attributes.valuemask = XpmColorSymbols | XpmDepth |
                           XpmColormap | XpmVisual | XpmCloseness;
    /*
     * Create the pixmap of the given image
     */
 
    status = XpmCreatePixmapFromData ( display,
                                       DefaultRootWindow ( display ), 
                                       xpmDescription,  &xpmPixmap,
                                       NULL, &attributes );

   /*
    * Compute the size of the label string and the given pixmap 
    */
 
   XmStringExtent ( fontlist, label, &width, &height );
 
   XGetGeometry ( display, xpmPixmap, ( Window * ) &junk,   
                  (int *) &junk, (int *) &junk, 
                  &pixmapWidth, &pixmapHeight,    
                  ( unsigned int *) &junk, ( unsigned int *) &junk );

  /*
   * Compute the sum of the label and pixmap sizes.
   */

   totalWidth = MAX ( pixmapWidth, width );
   totalHeight = pixmapHeight + height + PAD;
 
  /*
   * Create the final pixmap using the combined size and 
   * fill the pixmap with the background color of the widget 
   */
 

   labelPixmap = XCreatePixmap ( display, 
                               RootWindowOfScreen ( XtScreen ( w ) ),
                               totalWidth, totalHeight, depth );
  
   XFillRectangle ( display, labelPixmap, 
                    inverseGc, 0, 0, 
                    totalWidth, totalHeight );
  
  /*
   * Copy the Xpm-created pixmap into the larger pixmap and
   * then draw the string below the pixmap.
   */
  
   XCopyArea ( XtDisplay ( w ), xpmPixmap, labelPixmap, 
               gc, 0, 0, pixmapWidth, pixmapHeight, 
               ( totalWidth - pixmapWidth ) / 2, 
               0 );
  
   XmStringDraw ( display, labelPixmap, fontlist, label,
                  gc, 0, pixmapHeight + PAD, totalWidth, 
                  alignment, XmSTRING_DIRECTION_L_TO_R, NULL );

  /*
   * Install the final pixmap in the widget.
   */

   XtVaSetValues ( w,
                   XmNlabelPixmap, labelPixmap,
                   XmNlabelType,   XmPIXMAP,
                   NULL );
                   
  /*
   * Free the GCs, the initial pixmap, and the string retrieved
   * from the label widget.
   */

   XFreePixmap ( display, xpmPixmap );

   XtReleaseGC ( w, gc);
   XtReleaseGC ( w, inverseGc );
   XmStringFree ( label );
}
      


