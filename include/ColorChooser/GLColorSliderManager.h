#ifndef __GLColorSliderManager_h_
#define __GLColorSliderManager_h_

#include <X11/X.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/GLwMDrawA.h>
#include "GLColorSliderManagerC.h"
#include "SgList.h"


// -------------------------------------------------------------------------
// GL Manager that manages several slider gadgets
//

class GLColorSliderGadget;

class GLColorSliderManager : public SgColor {
  public:
    GLColorSliderManager(Widget parent, const char* instanceName, 
			 Arg *args, Cardinal num_args);
    ~GLColorSliderManager();

    virtual void render();
    void expose();
    virtual void processEvent(GLwDrawingAreaCallbackStruct *call_data);

    virtual void initSlider(Display *d, XVisualInfo *v);

    GLXContext getContext()   { return glx_context;}
    Widget getWidget()              { return gl_widget; }
    void   addCallback(XtCallbackProc callback, void *client_data); 
    // Add a gadget slider to the GL manager
    void   addGadget(GLColorSliderGadget *after, GLColorSliderGadget *gadget);
    void   deleteGadget(GLColorSliderGadget *gadget);
    void   addGadgets(GLColorSliderGadget *after,
		      GLColorSliderGadget **gadget, int num_gadgets);
    void   deleteGadgets(GLColorSliderGadget **gadget, int num_gadgets);
    Dimension getWidth()   { return(gl_widget->core.width); }
    
    void   winset()  { GLwDrawingAreaMakeCurrent(getWidget(), getContext()); }   

    void drawGLBackground();	// Draw the GL widget background
    void setWorkProcRun(Boolean val) { work_proc_run = val; }

  protected:
    void    resize();		// calculate and set children positions
    GLColorSliderGadget *calculateSliderGadget(XEvent* event);
    Boolean getDoubleBuf() { return(True); }
    Dimension getHeight()  { return(gl_widget->core.height); }
    GLColorSliderGadget *selected_slider; // Current events are is this slider
    Widget              gl_widget; // The slider is drawn on a Motif GL widget
    Boolean             rgbmode;
    XtCallbackProc      callback;
    void               *client_data;
    SgList             *gadgets; // List of slider gadgets
    Boolean             work_proc_run; // Flag whether the work proc is invoked

    short		r, g, b;
    short		minr, ming, minb;
    short		maxr, maxg, maxb;
    Bool		picking;
    float		value;		// current value of the V slider.
    GLXContext          glx_context;

};

#endif /* __GLColorSliderManager_h_ */
