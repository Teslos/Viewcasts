#ifndef __GLColorSliderGadget_h_
#define __GLColorSliderGadget_h_

#include <GL/GLwMDrawA.h>

// -------------------------------------------------------------------------
// GL gadget which provides a color value slider, for color selection.
//

class GLColorSliderManager;

class GLColorSliderGadget : public SgColor {
  public:
    GLColorSliderGadget(GLColorSliderManager *parent, 
			const char* instanceName);
    ~GLColorSliderGadget();

    virtual void render();
    virtual void processEvent(GLwDrawingAreaCallbackStruct *call_data);

    void setType(SliderType);
    SliderType getType();

    // Set color in RGB (Red, Green, Blue) color space
    void setColor(short r, short g, short b);
    // Set color in HSV (Hue, Saturation, Value) color space
    void setColor(float h, float s, float v);

    void setColor(SgColor* col);
    SgColor *getColor()		    { return SgColor::FindColor(r, g, b); }

    void getColor(short &red, short &green, short &blue)
				    { red = r; green = g; blue = b; }
    void setValue(float v);
    float getValue()		    { return value; }

    void   addCallback(XtCallbackProc callback, void *client_data); 
    void   setHeight(Dimension h)  { height = h; }
    Dimension getHeight()  { return(height); }
    void   setMarginHeight(Dimension h)  { margin_height = h; }
    Dimension getMarginHeight()  { return(margin_height); }
    void      setLocation(Position x_arg, Position y_arg)
      { x = x_arg; y = y_arg; }
    // Set whether to display the gadget or not
    void      setManaged(Boolean managed_arg) { managed = managed_arg; }
      

  protected:
    void setMinMaxColors();
    Boolean getDoubleBuf() { return(True); }
    Dimension getWidth()   { return(parent->getWidth()); }

    Boolean               managed;
    GLColorSliderManager *parent;
    Boolean               rgbmode;
    XtCallbackProc        callback;
    void                 *client_data;

    Position              x, y;
    Dimension             height;
    Dimension             margin_height;
    float		  h, s, v;
    short		  r, g, b;
    short		  minr, ming, minb;
    short		  maxr, maxg, maxb;
    SliderType	          type;
    Bool		  picking;
    float		  value; // current value of the V slider. 
    Boolean               sensitive; // If equal False can't set color or value

    void drawslider();
    void drawThumbUIRegion(short x1, short y1, short x2, short y2);
    void drawDownUIBorders(short x1, short y1, short x2, short y2, 
			   Bool blackLast);
    void calculateValue(XEvent* event);
};

#endif /* __GLColorSliderGadget_h_ */
