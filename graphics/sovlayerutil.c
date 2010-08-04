
/* Copyright (c) Mark J. Kilgard, 1996. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include "sovLayerUtil.h"
#include "grafik.h"
#include "grafik_func.h"
#include "strukturen.h"
#include "memory.h"
#include "glut.h"
#include "glutint.h"

/* Frame buffer capability macros and types. */
#define BUFFER_SIZE             0
#define RGBA                    1
#define DOUBLEBUFFER            2
#define STEREO                  3
#define AUX_BUFFERS             4
#define RED_SIZE                5  /* Used as mask bit for "color selected". */
#define GREEN_SIZE              6
#define BLUE_SIZE               7
#define ALPHA_SIZE              8
#define DEPTH_SIZE              9
#define STENCIL_SIZE            10
#define ACCUM_RED_SIZE          11  /* Used as mask bit for "acc selected". */
#define ACCUM_GREEN_SIZE        12
#define ACCUM_BLUE_SIZE         13
#define ACCUM_ALPHA_SIZE        14
#define LEVEL                   15

#define NUM_GLXCAPS             LEVEL + 1

#define XVISUAL                 NUM_GLXCAPS + 0
#define TRANSPARENT             NUM_GLXCAPS + 1
#define SAMPLES                 NUM_GLXCAPS + 2
#define XSTATICGRAY             NUM_GLXCAPS + 3  /* Used as mask bit for "any visual type selected". */
#define XGRAYSCALE              NUM_GLXCAPS + 4
#define XSTATICCOLOR            NUM_GLXCAPS + 5
#define XPSEUDOCOLOR            NUM_GLXCAPS + 6
#define XTRUECOLOR              NUM_GLXCAPS + 7
#define XDIRECTCOLOR            NUM_GLXCAPS + 8
#define SLOW                    NUM_GLXCAPS + 9
#define CONFORMANT              NUM_GLXCAPS + 10

#define NUM_CAPS                NUM_GLXCAPS + 11

/* Frame buffer capablities that don't have a
   corresponding FrameBufferMode entry.  These get
   used as mask bits. */
#define NUM                     NUM_CAPS + 0
#define RGBA_MODE               NUM_CAPS + 1
#define CI_MODE                 NUM_CAPS + 2

#define NONE			0
#define EQ			1
#define NEQ			2
#define LTE			3
#define GTE			4
#define GT			5
#define LT			6
#define MIN			7

static Criterion requiredOverlayCriteria[] = {
	{ LEVEL, EQ, 1 },
	{ TRANSPARENT, EQ, 1 },
	{ XPSEUDOCOLOR, EQ, 1 },
	{ RGBA, EQ, 0 },
	{ BUFFER_SIZE, GTE, 1 }
};

static int numRequiredOverlayCriteria = sizeof( requiredOverlayCriteria ) / sizeof( Criterion );
static int requiredOverlayCriteriaMask =
	( 1 << LEVEL ) | (1 << TRANSPARENT ) | (1 << XSTATICGRAY ) | ( 1 << RGBA ) | ( 1 << CI_MODE );


/* SGI optimization introduced in IRIX6.3 to avoid X server
   round trips for interning common X atoms. */
#include <X11/Xatom.h>
#if defined(_SGI_EXTRA_PREDEFINES) && !defined(NO_FAST_ATOMS)
#include<X11/SGIFastAtom.h>
#else
#define XSGIFastInternAtom(dpy,string,fast_name,how) XInternAtom(dpy,string,how)
#endif

static Bool layersRead;
static Atom overlayVisualsAtom;
static sovOverlayInfo **overlayInfoPerScreen;
static unsigned long *numOverlaysPerScreen;
//unsigned int displayMode;

sovVisualInfo *
sovGetVisualInfo(Display *display, long lvinfo_mask,
  sovVisualInfo *lvinfo_template, int *nitems_return);

static int
checkOverlayAcceptability( XVisualInfo *vi, unsigned int mode )
{
	t_winstruct *window = &C3D_win;
	int value;
	
	/* Must support OpenGL */
	glXGetConfig( window->dpy, vi, GLX_USE_GL, &value );
	if ( !value )
		return 1;

	/* Must be color-index .*/
	glXGetConfig( window->dpy, vi, GLX_RGBA, &value );
	if ( value )
		return 1;
	
	
	/* Must match single/double buffering request. */
	glXGetConfig( window->dpy, vi, GLX_DOUBLEBUFFER, &value );
	if ( C3D_WIND_IS_DOUBLE( window->display_mode) != ( value != 0 ))
		return 1;
	
	/* Must match mono/stereo request */
	glXGetConfig( window->dpy, vi, GLX_STEREO, &value );
	if ( C3D_WIND_IS_STEREO(mode) != (value != 0))
		return 1;


	return 0;
}

static void
findServerOverlayVisualsInfo(Display *dpy)
{
	static Atom overlayVisualsAtom;
	Atom actualType;
	Status status;
	unsigned long sizeData, bytesLeft;
	Window root;
	int actualFormat, numScreens, i;

	if ( layersRead == False ) 
	{
		overlayVisualsAtom = XSGIFastInternAtom(dpy,
				"SERVER_OVERLAY_VISUALS", SGI_XA_SERVER_OVERLAY_VISUALS, True);
		if ( overlayVisualsAtom != None ) 
		{
			numScreens = ScreenCount(dpy);
			overlayInfoPerScreen = (sovOverlayInfo**) ALLOC(numScreens * sizeof(sovOverlayInfo*));
			numOverlaysPerScreen = ( unsigned long * ) ALLOC(numScreens * sizeof(unsigned long));
			if ( overlayInfoPerScreen != NULL && numOverlaysPerScreen != NULL )
			{
				for ( i = 0; i < numScreens; i++ )
				{
					root = RootWindow(dpy, i);
					status = XGetWindowProperty(dpy, root,
							overlayVisualsAtom, 0L, (long)10000, False,
							overlayVisualsAtom, &actualType, &actualFormat,
							&sizeData, &bytesLeft,
							(unsigned char **) &overlayInfoPerScreen[i] );
					if ( status != Success ||
							actualType != overlayVisualsAtom ||
							actualFormat != 32 || sizeData < 4 )
						numOverlaysPerScreen[i] = 0;
					else
						/* Four 32-bit quantities per SERVER_OVERLAY_VISUALS entry . */
						numOverlaysPerScreen[i] = sizeData / 4;
				}
				layersRead = True;
			}
			else
			{
				if ( overlayInfoPerScreen != NULL )
					FREE( overlayInfoPerScreen );
				if ( numOverlaysPerScreen != NULL )
					FREE( numOverlaysPerScreen );
			}
		}
	}
}

static XVisualInfo *
getOverlayVisualInfoCI( unsigned int mode )
{
	t_winstruct *window = &C3D_win;
	sovVisualInfo *vi;
	sovVisualInfo template;
	XVisualInfo *goodVisual, *returnVisual;
	int nitems, i,j, bad;
	
	for ( i = 1; i <= 3; i ++ )
	{
		template.vinfo.screen = *((int*)XtScreen( window->wid ));
		template.vinfo.class = PseudoColor;
		template.layer       = i;
		template.type        = TransparentPixel;
		vi = sovGetVisualInfo( window->dpy, VisualTransparentType | VisualScreenMask |
			VisualClassMask | VisualLayerMask, &template, &nitems );
		
		if ( vi )
		{
			/* Check list for acceptable visual meeting requirements
			   of requested display mode */
			for ( j = 0; j < nitems; j++ )
			{
               			bad = checkOverlayAcceptability ( &vi[j].vinfo, mode );
				if ( bad )
					vi[j].vinfo.visual = NULL;
			}
			
			goodVisual = NULL;
			for( j = 0; j < nitems; j++ )
			{
				if ( vi[j].vinfo.visual )
				{
					if ( goodVisual == NULL )
						goodVisual = &vi[j].vinfo;
					else
					{
						if ( goodVisual->depth < vi[j].vinfo.depth )
							goodVisual = &vi[j].vinfo;
					}
				}
			}
			if ( goodVisual )
			{
				returnVisual = ( XVisualInfo *) ALLOC( sizeof(XVisualInfo ));
				if ( returnVisual )
					*returnVisual = *goodVisual;
				XFree(vi);
				return returnVisual;
			}
			XFree(vi);
		}
	}
	return NULL;
}


static XVisualInfo *
getOverlayVisualInfoRGB(unsigned int mode )
{
}

static XVisualInfo *
getOverlayVisualInfo( unsigned int mode )
{
	t_bool is_rgb = FALSE;
	if ( is_rgb )
		return getOverlayVisualInfoRGB( mode );
	else
		return getOverlayVisualInfoCI( mode );
}
	
int 
C3D_get_transparent_pixel( Display *dpy, XVisualInfo *vinfo )
{
	int i, screen = vinfo->screen;
	sovOverlayInfo *overlayInfo;
	
	findServerOverlayVisualsInfo( dpy );
	if ( layersRead )
	{
		for ( i = 0; i < numOverlaysPerScreen[screen]; i++ )
		{
			overlayInfo = &overlayInfoPerScreen[screen][i];
			if ( vinfo->visualid == overlayInfo->overlay_visual )
			{
				if ( overlayInfo->transparent_type == TransparentPixel )
				{
					return (int) overlayInfo->value;
				}
			}
			else
			{
				return -1;
			}
		}
	}
	return -1;
}	

sovVisualInfo *
sovGetVisualInfo(Display *display, long lvinfo_mask,
  sovVisualInfo *lvinfo_template, int *nitems_return)
{
	XVisualInfo *vinfo;
	sovVisualInfo *layerInfo;
	Window root;
	Status status;
	Atom actualType;
	unsigned long sizeData, bytesLeft;
	int actualFormat, numVisuals, numScreens, count, i, j;

	vinfo = XGetVisualInfo(display, lvinfo_mask & VisualAllMask,
			&lvinfo_template->vinfo, nitems_return);
	if (vinfo == NULL)
		return NULL;
	numVisuals = *nitems_return;
	findServerOverlayVisualsInfo( display );
	layerInfo = (sovVisualInfo*) ALLOC( numVisuals * sizeof(sovVisualInfo));	
	
	count = 0;
	for (i = 0; i < numVisuals; i++) {
		XVisualInfo *pVinfo;
		int screen;
		sovOverlayInfo *overlayInfo;

		pVinfo = &vinfo[i];
		screen = pVinfo->screen;
		overlayInfo = NULL;
		if (layersRead) {
			for (j = 0; j < numOverlaysPerScreen[screen]; j++)
				if (pVinfo->visualid == 
						overlayInfoPerScreen[screen][j].overlay_visual) {
					overlayInfo = &overlayInfoPerScreen[screen][j];
					break;
				}
		}
#if 1
		if ( overlayInfo )
		{
			printf( "Visual ID: %x\n", overlayInfo->overlay_visual );
			printf( "  transparent type : %s\n", overlayInfo->transparent_type );
			printf( "  pixel value:   %i\n", overlayInfo->value );
			printf( "  layer  : %i\n", overlayInfo->layer );
		}
#endif

		if (lvinfo_mask & VisualLayerMask)
			if (overlayInfo == NULL) {
				if (lvinfo_template->layer != 0)
					continue;
			} else if (lvinfo_template->layer != overlayInfo->layer)
				continue;
		if (lvinfo_mask & VisualTransparentType)
			if (overlayInfo == NULL) {
				if (lvinfo_template->type != None)
					continue;
			} else if (lvinfo_template->type !=
					overlayInfo->transparent_type)
				continue;
		if (lvinfo_mask & VisualTransparentValue)
			if (overlayInfo == NULL)
				/* non-overlay visuals have no sense of
				   TransparentValue */
				continue;
			else if (lvinfo_template->value != overlayInfo->value)
				continue;
		layerInfo[count].vinfo = *pVinfo;
		if (overlayInfo == NULL) {
			layerInfo[count].layer = 0;
			layerInfo[count].type = None;
			layerInfo[count].value = 0;  /* meaningless */
		} else {
			layerInfo[count].layer = overlayInfo->layer;
			layerInfo[count].type = overlayInfo->transparent_type;
			layerInfo[count].value = overlayInfo->value;
		}
		count++;
	}
	XFree(vinfo);
	*nitems_return = count;
	if (count == 0) {
		XFree(layerInfo);
		return NULL;
	} else
		return layerInfo;
}

Status
sovMatchVisualInfo(Display *display, int screen,
  int depth, int class, int layer, sovVisualInfo *lvinfo_return)
{
	sovVisualInfo *lvinfo;
	sovVisualInfo lvinfoTemplate;
	int nitems;

	lvinfoTemplate.vinfo.screen = screen;
	lvinfoTemplate.vinfo.depth = depth;
	lvinfoTemplate.vinfo.class = class;
	lvinfoTemplate.layer = layer;
	lvinfo = sovGetVisualInfo(display,
			VisualScreenMask|VisualDepthMask|VisualClassMask|VisualLayerMask,
			&lvinfoTemplate, &nitems);
	if (lvinfo != NULL && nitems > 0) {
		*lvinfo_return = *lvinfo;
		return 1;
	} else
		return 0;
}

void
C3D_use_layer( GLenum layer, t_winstruct *window )
{

	switch( layer )
	{
		case C3D_NORMAL:
			window->render_win = window->win;
			window->render_cx = window->cx;
			break;
		case C3D_OVERLAY:
			window->render_win = window->overlay->win;
			window->render_cx = window->overlay->cx;
			break;
		default:
			break;
	}
	/* make a current window */
}

XVisualInfo*
C3D_determine_visual( unsigned int displayMode,
		t_bool *treatAsSingle,
                XVisualInfo *(getVisualInfo) (unsigned int ))
{
	t_winstruct *window = &C3D_win;
	XVisualInfo *vi;
	*treatAsSingle = C3D_WIND_IS_SINGLE( displayMode );
	vi = getVisualInfo( displayMode );
	return vi;
}

XVisualInfo*
C3D_determine_visual_from_string(char *string, t_bool * treatAsSingle,
  Criterion * requiredCriteria, int nRequired, int requiredMask )
{
	Bool *single = (Bool*) treatAsSingle;
/*
	return __glutDetermineVisualFromString( string, single,
	requiredCriteria, nRequired, requiredMask );
*/
	return NULL;
}

static XVisualInfo *
determine_overlay_visual ( int *treatAsSingle, t_bool *visAlloced )
{
	XVisualInfo *vi;
	int i;
	t_winstruct *window = &C3D_win;	
	*visAlloced = FALSE;
	for ( i = 1; i < 3; i++ )
	{
		requiredOverlayCriteria[0].value = i;
		vi = C3D_determine_visual_from_string( window->display_string, treatAsSingle,
			requiredOverlayCriteria, numRequiredOverlayCriteria, requiredOverlayCriteriaMask );
		if ( vi ) 
			return vi;
	}
	*visAlloced = TRUE;
	return C3D_determine_visual( window->display_mode, treatAsSingle, getOverlayVisualInfo );
}

void setupColormap( XVisualInfo *vi, Colormap *colormap, Colormap *cmap, int is_rgb )
{
}

void
C3D_establish_overlay( t_winstruct *window )
{
	int is_rgb;
	t_overlay *overlay;
	XSetWindowAttributes wa;

	overlay = (t_overlay*) ALLOC(sizeof(t_overlay));
	overlay->vi = determine_overlay_visual(&overlay->treatAsSingle,
			&overlay->visAlloced );
	if ( !overlay->vi )
		printf( "out of memory\n" );
	overlay->cx = glXCreateContext( window->dpy, overlay->vi, None, True);
	overlay->isDirect = glXIsDirect( window->dpy, overlay->cx );
	setupColormap( overlay->vi, &overlay->colormap, &overlay->cmap,
			is_rgb );
	overlay->transparentPixel = C3D_get_transparent_pixel( window->dpy, overlay->vi );

	wa.colormap = overlay->colormap;
	wa.background_pixel = overlay->transparentPixel;
	wa.border_pixel = 0;
	overlay->win = XCreateWindow( window->dpy, window->win,
			0, 0, window->gl_width, window->gl_height,
			0, overlay->vi->depth, InputOutput, overlay->vi->visual,
			CWBackPixel | CWBorderPixel | CWEventMask | CWColormap,
			&wa );
	XMapWindow( window->dpy, overlay->win );
	overlay->showState = 1;
	//overlay->display = NULL;
	window->overlay = overlay;
	C3D_use_layer( C3D_OVERLAY, window );
}
static int ov_attribs[] = { GLX_BUFFER_SIZE, 2,
			    GLX_LEVEL, 1,
                            GLX_TRANSPARENT_TYPE_EXT, GLX_TRANSPARENT_INDEX_EXT,
			    None };
static void
ov_expose( Widget w, XtPointer client_data, XtPointer call_data )
{
	int i;
	GLwDrawingAreaCallbackStruct *call;
	call = ( GLwDrawingAreaCallbackStruct *) call_data;
	GLwDrawingAreaMakeCurrent( w, C3D_win.overlay->cx );
	glLoadIdentity();
	glOrtho( 0., 11., 0. ,11.,-1.,1. );
	glClearIndex( 0. );
	glClear( GL_COLOR_BUFFER_BIT );
	glIndexi(1);

	/*XXX test for overlay
	glBegin(GL_LINES);
	for ( i = 0; i < 10; i++ )
	{
		glVertex2f( 0.5+i,1.0 );
		glVertex2f( 0.5+i,10.0 );
		glVertex2f( 1.0, 0.5+i );
		glVertex2f( 10.0, 0.5+i );

	}
	glEnd();
	*/

	GLwDrawingAreaMakeCurrent( C3D_win.wid, C3D_win.cx );
}	

void
C3D_create_overlay( Widget w, t_winstruct *window )
{
	int n;
	Arg args[20];
	XColor col;
	GLboolean isDirect = GL_TRUE;
	Display *dpy;
	unsigned long pixel;	
	unsigned long pixels[128];
	unsigned long plane_masks[1];

	t_overlay *overlay = (t_overlay *) ALLOC(sizeof( t_overlay ));
	dpy = XtDisplay(w);

	/* create overlay widget */
	overlay->vi = glXChooseVisual( dpy, DefaultScreen( dpy ), ov_attribs ); 

	if ( !overlay->vi )
		SYS_error( "No valid overlay!\n" );
	n = 0;
	XtSetArg( args[n], XtNx, 0 ); n++;
	XtSetArg( args[n], XtNy, 0 ); n++;
	XtSetArg( args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg( args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg( args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	XtSetArg( args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg( args[n], GLwNvisualInfo, overlay->vi ); n++;
	overlay->wid = XtCreateManagedWidget( "overlay", glwMDrawingAreaWidgetClass,
		w /*toplevel*/, args, n );

	XtAddCallback( overlay->wid, GLwNexposeCallback, ov_expose, NULL ); 
	XtAddCallback( overlay->wid, GLwNresizeCallback, c3d_resize, NULL );
	XtAddCallback( overlay->wid, GLwNinputCallback, c3d_input, NULL ); 
	overlay->cx = glXCreateContext( dpy, overlay->vi, 0, isDirect );
	XRaiseWindow( dpy, XtWindow( overlay->wid ));
        GLwDrawingAreaMakeCurrent( window->wid, window->cx );
	XtVaGetValues( overlay->wid, XmNcolormap, &(overlay->cmap), NULL );
	if ( !XAllocColorCells( dpy, overlay->cmap, True, plane_masks, 0, pixels, 128 ))
		SYS_error( "Can't alloc overlay color cell \n" );
/*
	col.pixel = pixel;
	col.red = 65535;
	col.green = 65535;
	col.blue = 65535;
	col.flags = DoRed | DoGreen | DoBlue;
*/
	
	window->overlay = overlay;
//	SetColorMapGrau();
//	XStoreColor( dpy, overlay->cmap, &col );
}

int
C3D_layer_get( GLenum param, t_winstruct *window )
{
	switch( param )
	{
		case C3D_OVERLAY_POSSIBLE:
                {
			XVisualInfo *vi;
			t_bool dummy, visAlloced;
			vi = determine_overlay_visual( &dummy, &visAlloced );
			if ( vi )
			{
				if ( visAlloced )
					XFree( vi );
				return 1;
			}
			return 0;
                }
		case C3D_LAYER_IN_USE:
			return window->render_win != window->win;
		case C3D_HAS_OVERLAY:
			return window->overlay != NULL;
		case C3D_TRANSPARENT_INDEX:
			if( window->overlay )
			{
				return window->overlay->transparentPixel;
			}
			else
			{
				return -1;
			}
		default:
			return -1;
	}
}
 
void
C3D_init_display_mode( unsigned int mask )
{
	C3D_win.display_mode = mask;
}
