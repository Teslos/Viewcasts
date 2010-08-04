#ifndef __GLColorSliderManagerC_h_
#define __GLColorSliderManagerC_h_

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef __GLColorSliderGadgetC_h_
typedef struct GLColorSliderGadget *GLColorSliderGadgetP;
#endif

extern Widget SgGLColorSliderManagerCreate(Widget parent, char *name, 
					   Arg *args, Cardinal num_args);
/***
extern Widget SgGLColorSliderManagerCreate(Widget parent);
***/

extern void   SgGLColorSliderManagerAddSliders(Widget w, 
					       GLColorSliderGadgetP after,
					       GLColorSliderGadgetP *sliders, 
					       int num_sliders);
extern void   SgGLColorSliderManagerDeleteSliders(Widget w, 
						 GLColorSliderGadgetP *sliders,
						  int num_sliders);

extern void   SgGLColorSliderManagerClearArea(Widget w);

#ifdef __cplusplus
}
#endif 


#endif /* __GLColorSliderManagerC_h_ */
