/* ============================================================= */
/* COLOR3D - OpenGl-Version 0.1                                  */
/* ============================================================= */
/*                                                               */
/* Verfasser:   Helmut Vor                                       */
/* Datum    :   26.08.96                                         */
/* Dateiname:   color3d.c                                        */
/*                                                               */
/* Routinenname | Funktion                                       */
/* ------------------------------------------------------------- */
/*     main     |                                                */
/* ------------------------------------------------------------- */
/*                                                               */
/* Includes :                                                    */
/* ==========                                                    */       
#include "color3d.h"
#include "motif.h"
#include "uil.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>
#include <Xm/Xm.h>

#ifdef USE_EDITRES
#include <X11/Xmu/Editres.h>
#endif
#include "../graphics/gl_local.h"
#include "../init/toolCheckLicense.h"
#include "c3dinit.h"

/*                                                               */
/* globale Variable :                                            */
/* ==================                                            */
Widget shell; 
char *castsroot;
char help_dir_buffer[1000];
/* Liste eigener Aktionen ---------------------------------------------- */

static XtActionsRec aktionen[] =
{
  { "traverse_current", traverse_current },
  { "focus_out_TF_in_TT", focus_out_TF_in_TT },
  { "start_move", start_move },
  { "end_move", end_move },
  { "start_rot", start_rot },
  { "end_rot", end_rot },
  { "start_zoom", start_zoom },
  { "end_zoom", end_zoom },
  { "popup_menue_ausgeben", popup_menue_ausgeben },
  { "start_pick", start_pick },
};

/* Ressource-Liste ----------------------------------------------------- */

static XtResource resources[] =
{
  {	"version", "Version", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, version), XmRString, (XtPointer)"1.0"},
  {	"debug", "Debug", XmRBoolean, sizeof(XmRBoolean),
    	XtOffsetOf(ApplResTyp, debug), XmRString, (XtPointer)"False"},
  {	"nosubmit", "Nosubmit", XmRBoolean, sizeof(XmRBoolean),
    	XtOffsetOf(ApplResTyp, nosubmit), XmRString, (XtPointer)"False"},
  {	"help_dir", "Help_dir", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, help_dir), XmRString, (XtPointer)""},
  {	"print_string", "Print_string", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, print_string), XmRString, (XtPointer)""},
  {	"unv_dir", "Unv_dir", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, unv_dir), XmRString, (XtPointer)""},
  {	"casts_geo_dir", "Casts_geo_dir", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, casts_geo_dir), XmRString, (XtPointer)""},
  {	"casts_loes_dir", "Casts_loes_dir", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp, casts_loes_dir), XmRString, (XtPointer)""},
  {	"homogen_color", "Homogen_color", XmRString, sizeof(XmRString),
    	XtOffsetOf(ApplResTyp,homogen_color),XmRString,(XtPointer)"green"},
  {	"inhomogen_color", "Inhomogen_color", XmRString,sizeof(XmRString),
    	XtOffsetOf(ApplResTyp,inhomogen_color),XmRString,(XtPointer)"red"},
  {	"debug_gl", "Debug_gl", XmRBoolean, sizeof(XmRBoolean),
    	XtOffsetOf(ApplResTyp, debug_gl), XmRString, (XtPointer)"False"},
  {	"texture", "Texture", XmRBoolean, sizeof(XmRBoolean),
    	XtOffsetOf(ApplResTyp,texture),XmRString,(XtPointer)"False"}
};

/* fallback-Recourcen -------------------------------------------------- */

static String fallbackResourcen[] =
{
  "*sgiMode:              true",
  "*useSchemes:           all",
  "*allowShellResize:     true",
  "*defaultFontList:      *helvetica-bold-r-*-14-*",
  "*XmList.fontList:      *helvetica-bold-r-*-14-*",
  "*XmToggleButton.selectColor:       black",
  "*XmToggleButtonGadget.selectColor: black",
  (String)NULL
};
/*****************************************************************/

/* FUNCTION MAIN ------------------------------------------------------- */

int main(int argc, char **argv)
{
	/*char *color3d_install;*//* Umgebungsvariable Color3d_Install */
	/*char *color3d_icons;*/  /* Umgebungsvariable Color3d_Icons */
	char *sprache, sprache_buffer[1000]; 	/* Umgebungsvariable LANG */
	char *applresdir, applresdir_buffer[1000]; /* application resources */
	char castsroot_buffer[1000];   /* casts root directory */
	static char help_str[2048];
	int result;
	XKeyboardControl values;
	t_bool open_gl_verfuegbar;

	/* Setze XUSERFILESEARCHPATH und XBMLANGPATH --------------------------- */
	/*
	   if ((color3d_install = getenv("Color3d_Install"))) 
	   {
	   sprintf(help_str, "XUSERFILESEARCHPATH=$XUSERFILESEARCHPATH:%s/%%N", color3d_install);
	   putenv(help_str);
	   fprintf(stderr, "%s", help_str);
	   }
	   else
	   {
	   fprintf(stderr, "Die Umgebungsvariable >%s< ist nicht definiert.\n", "Color3d_Install");
	   }

	   if ((color3d_icons = getenv("Color3d_ICONS")))
	   {
	   sprintf(help_str, "XBMLANGPATH=$XBMLANGPATH:%s/%%B", color3d_icons);
	   putenv(help_str);
	   }
	   else
	   {
	   fprintf(stderr, "Die Umgebungsvariable >%s< ist nicht definiert.\n", "Color3d_ICONS");
	   sprintf(help_str, "XBMLANGPATH=$XBMLANGPATH:./icons/%%B");
	   putenv(help_str);
	   }
	 */

	/* test for FLEXlm license */
	//toolCheckLicenseF( 1 );
	/* initialize Color3d */
	C3D_init();
	sprintf(help_str, ":./icons/%%B");
	putenv(help_str);

	if (getenv("CASTSLANG"))
	{
		sprache = getenv("CASTSLANG");
		strcpy(sprache_buffer, sprache);
		strlwr(sprache_buffer);
	}
	else
		strcpy(sprache_buffer, "en");

	if ( strstr(sprache_buffer, "en") )
		g_sprache = SPRACHE_ENGLISCH;
	else
		g_sprache = SPRACHE_DEUTSCH;

	if (getenv("XAPPLRESDIR"))
	{
		applresdir = getenv("XAPPLRESDIR");
		strcpy(applresdir_buffer, applresdir);
	}

	C3D_initvar();
	if ( castsrootvar )
	{
		if ( strcmp(castsrootvar->string, "") )
		{
			strcpy( help_dir_buffer, castsrootvar->string );
			castsroot = strdup( castsrootvar->string );
		}
		else
		{
			if (getenv("CASTSROOT"))
			{
				castsroot = getenv("CASTSROOT");
				strcpy(castsroot_buffer, castsroot);
				strcpy(help_dir_buffer, castsroot_buffer);
				strcat(help_dir_buffer, "/help/viewcasts/help_text");
			}
		}
	}

#ifdef DEBUG
	fprintf(stderr, "sprache   = >%s<\n", sprache);
	fprintf(stderr, "g_sprache = >%d<\n", g_sprache);
#endif

	/* Initialisiere user-interface ---------------------------------------- */

	toplevel = XtVaAppInitialize(	&kontext, APPL_KLASSE,
			(XrmOptionDescRec*)NULL, (Cardinal)NULL,
			&argc, argv, fallbackResourcen, NULL);

	/* Aktionen registrieren */
	XtAppAddActions(kontext, aktionen, XtNumber(aktionen));

	/* read the programmer app default */
	XtGetApplicationResources(toplevel, &applRessourcen, resources,
			XtNumber(resources), NULL, 0);
#ifdef DEBUG
	if (applRessourcen.debug)
	{
		fprintf(stdout, "applRessourcen.version         = >%s<\n", applRessourcen.version);
		fprintf(stdout, "applRessourcen.debug           = >%d<\n", applRessourcen.debug);
		fprintf(stdout, "applRessourcen.nosubmit        = >%d<\n", applRessourcen.nosubmit);
		fprintf(stdout, "applRessourcen.help_dir        = >%s<\n", applRessourcen.help_dir);
		fprintf(stdout, "applRessourcen.print_string    = >%s<\n", applRessourcen.print_string);
		fprintf(stdout, "applRessourcen.unv_dir         = >%s<\n", applRessourcen.unv_dir);
		fprintf(stdout, "applRessourcen.casts_geo_dir   = >%s<\n", applRessourcen.casts_geo_dir);
		fprintf(stdout, "applRessourcen.casts_loes_dir  = >%s<\n", applRessourcen.casts_loes_dir);
		fprintf(stdout, "applRessourcen.homogen_color   = >%s<\n", applRessourcen.homogen_color);
		fprintf(stdout, "applRessourcen.inhomogen_color = >%s<\n", applRessourcen.inhomogen_color);
		fprintf(stdout, "applRessourcen.debug_gl        = >%d<\n", applRessourcen.debug_gl);
		fprintf(stdout, "applRessourcen.texture         = >%d<\n", applRessourcen.texture );
	}
#endif

	toplevel  = Createcolor3d_interface("color3d_interface",toplevel,NULL,0);
	if ((result = init_mainW()))
	{
		fprintf(stderr, "init_mainW() return = >%d<\n", result);
		exit(1);
	}




	/* File-Dialog erzeugen ------------------------------------------------ */
	file_dir_auswahl("", "","", FALSE, TRUE, FALSE);

	/* DrawingArea fuer Grafikausgabe erzeugen ----------------------------- */
	/*if (create_da(toplevel, &open_gl_verfuegbar))
	  {
	  if (g_sprache == SPRACHE_DEUTSCH)
	  warning("** Color3d ist auf dieser Maschine nicht lauffaehig **");
	  else
	  warning("*** Color3d does not run on this workstation ***");
	  exit(0);
	  }

	  if (!open_gl_verfuegbar)
	  {
	  if (g_sprache == SPRACHE_DEUTSCH)
	  warning("** OpenGL ist auf diesem Display nicht verfuegbar **");
	  else
	  warning("*** OpenGL is not implemented on this workstation ***");
	  XtSetSensitive(widgetMain.XmForm_r, FALSE);
	  exit(0);
	  }
	 */

#ifdef USE_EDITRES
	XtAddEventHandler(toplevel, (EventMask)NULL, True,
			_XEditResCheckMessages, NULL);
#endif

	assert(XtIsObject(toplevel)&&(XtIsShell(toplevel)));
	//XtRealizeWidget(toplevel);

	/* OpenGL stuff initialization */
	GL_init(toplevel,0);
	GL_strings();

	/* Daten initialisieren; muss nach init_mainW() aufgerufen werden ------ */
	if ((result = init_data(TRUE)))
	{
		fprintf(stderr, "init_data() return = >%d<\n", result);
		exit(1);
	}

	/* Fenster fuer Colormap erzeugen ------------------------------------- */
	if (make_fenster_cmap(toplevel, &CMAP_win))
	{
		g_tabelle.ex = TRUE;
		g_tabelle.on = FALSE;
	}

	g_color_extra.texture_on = applRessourcen.texture;

	/* turn off Keyboard-AutoRepeat --------------------------------------- */
	values.auto_repeat_mode = AutoRepeatModeDefault;
	values.auto_repeat_mode = AutoRepeatModeOff;
	values.key = XKeysymToKeycode(XtDisplay(toplevel), XStringToKeysym("1"));
	XChangeKeyboardControl(XtDisplay(toplevel), (KBAutoRepeatMode | KBKey), &values);
	values.key = XKeysymToKeycode(XtDisplay(toplevel), XStringToKeysym("2"));
	XChangeKeyboardControl(XtDisplay(toplevel), (KBAutoRepeatMode | KBKey), &values);
	values.key = XKeysymToKeycode(XtDisplay(toplevel), XStringToKeysym("3"));
	XChangeKeyboardControl(XtDisplay(toplevel), (KBAutoRepeatMode | KBKey), &values);

	/* start eventloop ----------------------------------------------------- */
	XtAppMainLoop(kontext);

	exit(0);
}











