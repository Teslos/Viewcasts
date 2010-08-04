#ifndef __GLColorSliderGadgetC_h_
#define __GLColorSliderGadgetC_h_

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef __GLColorSliderManagerC_h_
typedef struct GLColorSliderGadget *GLColorSliderGadgetP;
#endif

/* The range of values the slider visually represents in its background. */

typedef enum {
    Slider_None, 
    Slider_Red,
    Slider_Green, 
    Slider_Blue, 
    Slider_Hue, 
    Slider_Sat,
    Slider_Value
} SliderType;

extern GLColorSliderGadgetP 
  SgGLColorSliderGadgetCreate(SliderType type, Widget parent);
extern void   SgGLColorSliderGadgetSetColor(GLColorSliderGadgetP gadget, 
					    short r, short g, short b);
extern void   SgGLColorSliderGadgetSetHSVColor(GLColorSliderGadgetP gadget, 
					      float hue, float sat, float val);
extern void   SgGLColorSliderGadgetSetValue(GLColorSliderGadgetP gadget, 
					    float v);
extern float  SgGLColorSliderGadgetGetValue(GLColorSliderGadgetP gadget);
extern void   SgGLColorSliderGadgetAddCallback(GLColorSliderGadgetP gadget, 
					       XtCallbackProc callback,
					       void *client_data_arg);
extern void   SgGLColorSliderGadgetSetMarginHeight(GLColorSliderGadgetP gadget,
						   Dimension height);

#ifdef __cplusplus
}
#endif 


#endif /* __GLColorSliderGadgetC_h_ */
