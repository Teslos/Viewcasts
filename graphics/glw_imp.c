#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XShm.h>
#include <Xm/MwmUtil.h>

#include <GL/glx.h>
#include <stdio.h>
#include "gl_local.h"
#include "../ioclient/keys.h"
#include "motif.h"
#include "shared.h"
#include "grafik.h"
#include "keyboard.h"
#include "ref.h"

viddef_t vid;
extern refimport_t ri;

GLXContext				gl_cx;

static t_bool			doShm;
static Display			*x_disp;
static Colormap			x_cmap;
static Window			x_win;
static GC			x_gc;
static Visual			*x_vis;
static XVisualInfo		*x_visinfo;

static int StudlyRGBattributes[] =
{
	GLX_DOUBLEBUFFER,
	GLX_RGBA,
	GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 1,
	GLX_SAMPLES_SGIS, 4, /* for better AA */
	None,
};
static int RGB_attributes[] =
{
	GLX_DOUBLEBUFFER,
	GLX_RGBA,
	GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 1,
	None,
};
#define STD_EVENT_MASK (StructureNotifyMask | KeyPressMask \
		| KeyReleaseMask | ExposureMask | PointerMotionMask | \
		ButtonPressMask | ButtonReleaseMask)
int current_framebuffer;
static int    x_shmeventtype;

static t_bool oktodraw = FALSE;
static t_bool  X11_active = FALSE;

struct
{
	int key;
	int down;
}keyq[64];

int keyq_head = 0;
int keyq_tail = 0;

static int mx, my;
static int p_mouse_x, p_mouse_x;
static t_cvar *_windowed_mouse;

static t_cvar *sensitivity;

int config_notify = 0;
int config_notify_width;
int config_notify_height;

typedef unsigned short PIXEL;

/*
================================================================================
  MOUSE
================================================================================
*/

static t_bool mouse_avail;
static int    mouse_buttonstate;
static int    mouse_oldbuttonstate;
static int    mouse_x, mouse_y;
static int    old_mouse_x, old_mouse_y;
static float  old_windowed_mouse;
static int    p_mouse_x, p_mouse_y;

static t_bool  mlooking;

static t_in_state  *in_state;

int XShmQueryExtension(Display *);
int XShmGetEventBase( Display *);
void GLIMP_shutdown( void );

static void signal_handler( int sig) 
{
	fprintf( stderr, "recived signal %d, exiting...\n", sig);
	GLIMP_shutdown();
	_exit(0);
}

static void InitSig( void )
{
	struct sigaction sa;
	sigaction(SIGINT, 0, &sa);
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
}

void GLIMP_shutdown( void )
{
	fprintf( stderr, "GLIMP_shutdwon\n" );
	
	if ( !x_disp )
		return;
	
	XSynchronize( x_disp, True );
	XAutoRepeatOn( x_disp );
	XCloseDisplay( x_disp );
	x_disp = NULL;
}

int XLateKey(XKeyEvent *ev)
{

	int key;
	char buf[64];
	KeySym keysym;

	key = 0;

	XLookupString(ev, buf, sizeof buf, &keysym, 0);

	switch(keysym)
	{
		case XK_KP_Page_Up:	 key = K_KP_PGUP; break;
		case XK_Page_Up:	 key = K_PGUP; break;

		case XK_KP_Page_Down: key = K_KP_PGDN; break;
		case XK_Page_Down:	 key = K_PGDN; break;

		case XK_KP_Home: key = K_KP_HOME; break;
		case XK_Home:	 key = K_HOME; break;

		case XK_KP_End:  key = K_KP_END; break;
		case XK_End:	 key = K_END; break;

		case XK_KP_Left: key = K_KP_LEFTARROW; break;
		case XK_Left:	 key = K_LEFTARROW; break;

		case XK_KP_Right: key = K_KP_RIGHTARROW; break;
		case XK_Right:	key = K_RIGHTARROW;		break;

		case XK_KP_Down: key = K_KP_DOWNARROW; break;
		case XK_Down:	 key = K_DOWNARROW; break;

		case XK_KP_Up:   key = K_KP_UPARROW; break;
		case XK_Up:		 key = K_UPARROW;	 break;

		case XK_Escape: key = K_ESCAPE;		break;

		case XK_KP_Enter: key = K_KP_ENTER;	break;
		case XK_Return: key = K_ENTER;		 break;

		case XK_Tab:		key = K_TAB;			 break;

		case XK_F1:		 key = K_F1;				break;

		case XK_F2:		 key = K_F2;				break;

		case XK_F3:		 key = K_F3;				break;

		case XK_F4:		 key = K_F4;				break;

		case XK_F5:		 key = K_F5;				break;

		case XK_F6:		 key = K_F6;				break;

		case XK_F7:		 key = K_F7;				break;

		case XK_F8:		 key = K_F8;				break;

		case XK_F9:		 key = K_F9;				break;

		case XK_F10:		key = K_F10;			 break;

		case XK_F11:		key = K_F11;			 break;

		case XK_F12:		key = K_F12;			 break;

		case XK_BackSpace: key = K_BACKSPACE; break;

		case XK_KP_Delete: key = K_KP_DEL; break;
		case XK_Delete: key = K_DEL; break;

		case XK_Pause:	key = K_PAUSE;		 break;

		case XK_Shift_L:
		case XK_Shift_R:	key = K_SHIFT;		break;

		case XK_Execute: 
		case XK_Control_L: 
		case XK_Control_R:	key = K_CTRL;		 break;

		case XK_Alt_L:	
		case XK_Meta_L: 
		case XK_Alt_R:	
		case XK_Meta_R: key = K_ALT;			break;

		case XK_KP_Begin: key = K_KP_5;	break;

		case XK_Insert:key = K_INS; break;
		case XK_KP_Insert: key = K_KP_INS; break;

		case XK_KP_Multiply: key = '*'; break;
		case XK_KP_Add:  key = K_KP_PLUS; break;
		case XK_KP_Subtract: key = K_KP_MINUS; break;
		case XK_KP_Divide: key = K_KP_SLASH; break;

#if 0
		case 0x021: key = '1';break;/* [!] */
		case 0x040: key = '2';break;/* [@] */
		case 0x023: key = '3';break;/* [#] */
		case 0x024: key = '4';break;/* [$] */
		case 0x025: key = '5';break;/* [%] */
		case 0x05e: key = '6';break;/* [^] */
		case 0x026: key = '7';break;/* [&] */
		case 0x02a: key = '8';break;/* [*] */
		case 0x028: key = '9';;break;/* [(] */
		case 0x029: key = '0';break;/* [)] */
		case 0x05f: key = '-';break;/* [_] */
		case 0x02b: key = '=';break;/* [+] */
		case 0x07c: key = '\'';break;/* [|] */
		case 0x07d: key = '[';break;/* [}] */
		case 0x07b: key = ']';break;/* [{] */
		case 0x022: key = '\'';break;/* ["] */
		case 0x03a: key = ';';break;/* [:] */
		case 0x03f: key = '/';break;/* [?] */
		case 0x03e: key = '.';break;/* [>] */
		case 0x03c: key = ',';break;/* [<] */
#endif

		default:
			key = *(unsigned char*)buf;
			if (key >= 'A' && key <= 'Z')
				key = key - 'A' + 'a';
			break;
	} 

	return key;
}


void GetEvent(void)
{
	XEvent x_event;
	int b;
   
	XNextEvent(x_disp, &x_event);
	switch(x_event.type) {
	case KeyPress:
		keyq[keyq_head].key = XLateKey(&x_event.xkey);
		keyq[keyq_head].down = TRUE;
		keyq_head = (keyq_head + 1) & 63;
		break;
	case KeyRelease:
		keyq[keyq_head].key = XLateKey(&x_event.xkey);
		keyq[keyq_head].down = FALSE;
		keyq_head = (keyq_head + 1) & 63;
		break;

	case MotionNotify:
		if (_windowed_mouse->value) {
			mx += ((int)x_event.xmotion.x - (int)(vid.width/2));
			my += ((int)x_event.xmotion.y - (int)(vid.height/2));

			/* move the mouse to the window center again */
			XSelectInput(x_disp,x_win, STD_EVENT_MASK & ~PointerMotionMask);
			XWarpPointer(x_disp,None,x_win,0,0,0,0, 
				(vid.width/2),(vid.height/2));
			XSelectInput(x_disp,x_win, STD_EVENT_MASK);
		} else {
			mx = ((int)x_event.xmotion.x - (int)p_mouse_x);
			my = ((int)x_event.xmotion.y - (int)p_mouse_y);
			p_mouse_x=x_event.xmotion.x;
			p_mouse_y=x_event.xmotion.y;
		}
		break;

	case ButtonPress:
		b=-1;
		if (x_event.xbutton.button == 1)
			b = 0;
		else if (x_event.xbutton.button == 2)
			b = 2;
		else if (x_event.xbutton.button == 3)
			b = 1;
		if (b>=0)
			mouse_buttonstate |= 1<<b;
		break;

	case ButtonRelease:
		b=-1;
		if (x_event.xbutton.button == 1)
			b = 0;
		else if (x_event.xbutton.button == 2)
			b = 2;
		else if (x_event.xbutton.button == 3)
			b = 1;
		if (b>=0)
			mouse_buttonstate &= ~(1<<b);
		break;
	
	case ConfigureNotify:
		config_notify_width = x_event.xconfigure.width;
		config_notify_height = x_event.xconfigure.height;
		config_notify = 1;
		break;

	default:
		if (doShm && x_event.type == x_shmeventtype)
			oktodraw = TRUE;
	}
   
	if (old_windowed_mouse != _windowed_mouse->value) {
		old_windowed_mouse = _windowed_mouse->value;

		if (!_windowed_mouse->value) {
			/* ungrab the pointer */
			XUngrabPointer(x_disp,CurrentTime);
		} else {
			/* grab the pointer */
			XGrabPointer(x_disp,x_win,True,0,GrabModeAsync,
				GrabModeAsync,x_win,None,CurrentTime);
		}
	}
}

/*
===================================================================================
	KEYBOARD
===================================================================================
*/
Key_Event_fp_t Key_Event_fp;

void KBD_init( Key_Event_fp_t fp)
{
	Key_Event_fp = fp;
}

void KBD_update( void )
{
	if ( x_disp )
	{
		while( XPending( x_disp ))
			GetEvent();
		while( keyq_head != keyq_tail )
		{
			Key_Event_fp( keyq[keyq_tail].key, keyq[keyq_tail].down);
			keyq_tail = (keyq_tail + 1) & 63;
		}
	}
}

void KBD_close()
{
}

t_bool GLIMP_init(Widget parent, void *dpy )
{
  //signal handler
    return TRUE;
}
/*
=======================
GLIMP_beginframe
=======================
*/
void GLIMP_beginframe( float camera )
{
}

/*
=======================
GLIMP_endframe

Responsible for doing a swapbuffers and
possible for other stuff.
=======================
*/
void GLIMP_endframe( void )
{
	glFlush();
	glXSwapBuffers( x_disp, x_win );
}

t_bool GLIMP_initgraphics( Widget parent, t_bool fullscreen )
{
  t_bool has_opengl;
  Display  *dpy;

  dpy = XtDisplay(parent);

  // wait for first exposure event
    {
        XEvent event;
        do
        {
            XNextEvent(dpy, &event);
            if (event.type == Expose && !event.xexpose.count)
                oktodraw = TRUE;
        } while (!oktodraw);
    }

  /* if fullscreen */
  C3D_fullscreen( dpy, XtWindow( parent ), fullscreen );

  /* create display */
  create_da(parent, &has_opengl); 
  if ( !has_opengl )
    {
      if ( getenv ("DISPLAY") )
	SYS_error ( "Could not open display [%s]\n",
		   getenv("DISPLAY") );
      else
	SYS_error ( "Could not open local display\n" );
    }
  else 
    fprintf(stderr, "Opened display %s\n", getenv("DISPLAY"));
  XtRealizeWidget(parent);
  //XRaiseWindow( dpy, XtWindow( C3D_win.overlay->wid ));
  return TRUE;
}

void C3D_fullscreen( Display *x_disp, Window x_win, t_bool fullscreen )
{

	if (fullscreen) {
		MotifWmHints    wmhints;
		Atom aHints;
		XSizeHints      sizehints;
		XWindowChanges  changes;

		aHints = XInternAtom( x_disp, "_MOTIF_WM_HINTS", 0 );
		if (aHints == None)
		{
			printf( "Could not intern X atom for _MOTIF_WM_HINTS." );
			/*                 return( false ); */
		}
		else {
			wmhints.flags = MWM_HINTS_DECORATIONS;
			wmhints.decorations = 0; // Absolutely no decorations.
			XChangeProperty(x_disp, x_win, aHints, aHints, 32,
					PropModeReplace, (unsigned char *)&wmhints,
					4 );

			sizehints.flags = USPosition | USSize;
			sizehints.x = 0;
			sizehints.y = 0;
			sizehints.width = vid.width;
			sizehints.height = vid.height;
			XSetWMNormalHints( x_disp, x_win, &sizehints );

			changes.x = 0;
			changes.y = 0;
			changes.width = vid.width;
			changes.height = vid.height;
			changes.stack_mode = TopIf;
			XConfigureWindow( x_disp, x_win,
					CWX | CWY | CWWidth | CWHeight | CWStackMode,
					&changes);
		}
	}
}

t_bool VID_loadrefresh( char *name )
{
  ri.CMD_addcommand = CMD_addcommand;
  ri.CMD_removecommand = CMD_removecommand;
  ri.CMD_argc = CMD_argc;
  ri.CMD_argv = CMD_argv;
  ri.CMD_executetext = CBUF_executetext;
  //ri.Con_Printf = VID_Printf;
  ri.SYS_error = SYS_error;
  //ri.FS_loadfile = FS_loadfile;
  //ri.FS_freefile = FS_freefile;
  //ri.FS_Gamedir = FS_gamedir;
  ri.CVAR_get = CVAR_get;
  ri.CVAR_set = CVAR_set;
  ri.CVAR_setvalue = CVAR_setvalue;
/*
  ri.Vid_GetModeInfo = VID_GetModeInfo;
  ri.Vid_MenuInit = VID_MenuInit;
  ri.Vid_NewWindow = VID_NewWindow;
*/
  re = GetRefAPI( ri );	
  return TRUE;
}

void C3D_setwindowtitle( Display *dpy, Window win, const char *title )
{
  XTextProperty text;
  
  text.value = (unsigned char *) title;
  text.encoding = XA_STRING;
  text.format   = 8;
  text.nitems   = strlen( title );
  XSetWMName( dpy, win, &text );
  XFlush( dpy );
}
