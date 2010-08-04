#include "keys.h"
#include "../graphics/ref.h"
#include "memory.h"
#include "console.h"
#include "client.h"

#define MAXCMDLINE 256
/* XXX */
t_console con;

char key_lines[32][MAXCMDLINE];
int  key_linepos;
t_bool shift_down = FALSE;
int  anykeydown;
int  edit_line = 0;
int  history_line = 0;

int key_waiting;
char *keybindings[256];
t_bool  consolekeys[256];  // if true can not be rebound while in console
int   keyshift[256];       // key to map to if shift held down in console
int   key_repeats[256];   // if  > 1, it is autorepeating
t_bool keydown[256];

typedef struct
{
	char *name;
	int  keynum;
} t_keyname;

t_keyname keynames[] =
{
	{"TAB", K_TAB},
	{"ENTER", K_ENTER},
	{"ESCAPE", K_ESCAPE},
	{"SPACE", K_SPACE},
	{"BACKSPACE", K_BACKSPACE},
	{"UPARROW", K_UPARROW},
	{"DOWNARROW", K_DOWNARROW},
	{"LEFTARROW", K_LEFTARROW},
	{"RIGHTARROW", K_RIGHTARROW},

	{"ALT", K_ALT},
	{"CTRL", K_CTRL},
	{"SHIFT", K_SHIFT},

	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"INS", K_INS},
	{"DEL", K_DEL},
	{"PGDN", K_PGDN},
	{"PGUP", K_PGUP},
	{"HOME", K_HOME},
	{"END", K_END},

	{"MOUSE1", K_MOUSE1},
	{"MOUSE2", K_MOUSE2},
	{"MOUSE3", K_MOUSE3},

	{"JOY1", K_JOY1},
	{"JOY2", K_JOY2},
	{"JOY3", K_JOY3},
	{"JOY4", K_JOY4},

	{"AUX1", K_AUX1},
	{"AUX2", K_AUX2},
	{"AUX3", K_AUX3},
	{"AUX4", K_AUX4},
	{"AUX5", K_AUX5},
	{"AUX6", K_AUX6},
	{"AUX7", K_AUX7},
	{"AUX8", K_AUX8},
	{"AUX9", K_AUX9},
	{"AUX10", K_AUX10},
	{"AUX11", K_AUX11},
	{"AUX12", K_AUX12},
	{"AUX13", K_AUX13},
	{"AUX14", K_AUX14},
	{"AUX15", K_AUX15},
	{"AUX16", K_AUX16},
	{"AUX17", K_AUX17},
	{"AUX18", K_AUX18},
	{"AUX19", K_AUX19},
	{"AUX20", K_AUX20},
	{"AUX21", K_AUX21},
	{"AUX22", K_AUX22},
	{"AUX23", K_AUX23},
	{"AUX24", K_AUX24},
	{"AUX25", K_AUX25},
	{"AUX26", K_AUX26},
	{"AUX27", K_AUX27},
	{"AUX28", K_AUX28},
	{"AUX29", K_AUX29},
	{"AUX30", K_AUX30},
	{"AUX31", K_AUX31},
	{"AUX32", K_AUX32},

	{"KP_HOME",			K_KP_HOME },
	{"KP_UPARROW",		K_KP_UPARROW },
	{"KP_PGUP",			K_KP_PGUP },
	{"KP_LEFTARROW",	K_KP_LEFTARROW },
	{"KP_5",			K_KP_5 },
	{"KP_RIGHTARROW",	K_KP_RIGHTARROW },
	{"KP_END",			K_KP_END },
	{"KP_DOWNARROW",	K_KP_DOWNARROW },
	{"KP_PGDN",			K_KP_PGDN },
	{"KP_ENTER",		K_KP_ENTER },
	{"KP_INS",			K_KP_INS },
	{"KP_DEL",			K_KP_DEL },
	{"KP_SLASH",		K_KP_SLASH },
	{"KP_MINUS",		K_KP_MINUS },
	{"KP_PLUS",			K_KP_PLUS },

	{"MWHEELUP", K_MWHEELUP },
	{"MWHEELDOWN", K_MWHEELDOWN },

	{"PAUSE", K_PAUSE},

	{"SEMICOLON", ';'},	// because a raw semicolon seperates commands

	{NULL,0}
};
/*
==============================================================================
			LINE TYPING INTO THE CONSOLE
==============================================================================
*/

void CompleteCommand( void )
{
	char *cmd, *s;
	
	s = key_lines[edit_line] + 1;
	if ( *s == '\\' || *s == '/' )
		s++;
	
	cmd = CMD_completecommand( s );
	if ( !cmd )
		cmd = CVAR_completevariable( s );
	if ( cmd )
	{
		key_lines[edit_line][1] = '/';
		strcpy( key_lines[edit_line]+2, cmd );
		key_linepos = strlen(cmd) + 2;
		key_lines[edit_line][key_linepos] = ' ';
		key_linepos++;
		key_lines[edit_line][key_linepos] = 0;
		return;
	}
}

/*
===========================
KEY_console

Interactive line editing and console scrollback
===========================
*/

void KEY_console( int key )
{

	switch ( key )
	{
		case K_KP_SLASH:
			key = '/';
			break;
		case K_KP_MINUS:
			key = '-';
			break;
		case K_KP_PLUS:
			key = '+';
			break;
		case K_KP_HOME:
			key = '7';
			break;
		case K_KP_UPARROW:
			key = '8';
			break;
		case K_KP_PGUP:
			key = '9';
			break;
		case K_KP_LEFTARROW:
			key = '4';
			break;
		case K_KP_5:
			key = '5';
			break;
		case K_KP_RIGHTARROW:
			key = '6';
			break;
		case K_KP_END:
			key = '1';
			break;
		case K_KP_DOWNARROW:
			key = '2';
			break;
		case K_KP_PGDN:
			key = '3';
			break;
		case K_KP_INS:
			key = '0';
			break;
		case K_KP_DEL:
			key = '.';
			break;
	}

	if ( ( toupper( key ) == 'V' && keydown[K_CTRL] ) ||
			( ( ( key == K_INS ) || ( key == K_KP_INS ) ) && keydown[K_SHIFT] ) )
	{
		char *cbd;

		if ( ( cbd = SYS_getclipboarddata() ) != 0 )
		{
			int i;

			strtok( cbd, "\n\r\b" );

			i = strlen( cbd );
			if ( i + key_linepos >= MAXCMDLINE)
				i= MAXCMDLINE - key_linepos;

			if ( i > 0 )
			{
				cbd[i]=0;
				strcat( key_lines[edit_line], cbd );
				key_linepos += i;
			}
			free( cbd );
		}

		return;
	}

	if ( key == 'l' ) 
	{
		if ( keydown[K_CTRL] )
		{
			CBUF_addtext ("clear\n");
			return;
		}
	}

	if ( key == K_ENTER || key == K_KP_ENTER )
	{	// backslash text are commands, else chat
		if (key_lines[edit_line][1] == '\\' || key_lines[edit_line][1] == '/')
			CBUF_addtext (key_lines[edit_line]+2);	// skip the >
		else
			CBUF_addtext (key_lines[edit_line]+1);	// valid command

		CBUF_addtext ("\n");
		printf ("%s\n",key_lines[edit_line]);
		edit_line = (edit_line + 1) & 31;
		history_line = edit_line;
		key_lines[edit_line][0] = ']';
		key_linepos = 1;
		//SCR_updatescreen ();	// force an update, because the command
		// may take some time
		return;
	}

	if (key == K_TAB)
	{	// command completion
		CompleteCommand ();
		return;
	}

	if ( ( key == K_BACKSPACE ) || ( key == K_LEFTARROW ) || 
	( key == K_KP_LEFTARROW ) || ( ( key == 'h' ) && ( keydown[K_CTRL] ) ) )
	{
		if (key_linepos > 1)
			key_linepos--;
		return;
	}

	if ( ( key == K_UPARROW ) || ( key == K_KP_UPARROW ) ||
			( ( key == 'p' ) && keydown[K_CTRL] ) )
	{
		do
		{
			history_line = (history_line - 1) & 31;
		} while (history_line != edit_line
				&& !key_lines[history_line][1]);
		if (history_line == edit_line)
			history_line = (edit_line+1)&31;
		strcpy(key_lines[edit_line], key_lines[history_line]);
		key_linepos = strlen(key_lines[edit_line]);
		return;
	}

	if ( ( key == K_DOWNARROW ) || ( key == K_KP_DOWNARROW ) ||
			( ( key == 'n' ) && keydown[K_CTRL] ) )
	{
		if (history_line == edit_line) return;
		do
		{
			history_line = (history_line + 1) & 31;
		}
		while (history_line != edit_line
				&& !key_lines[history_line][1]);
		if (history_line == edit_line)
		{
			key_lines[edit_line][0] = ']';
			key_linepos = 1;
		}
		else
		{
			strcpy(key_lines[edit_line], key_lines[history_line]);
			key_linepos = strlen(key_lines[edit_line]);
		}
		return;
	}

	if (key == K_PGUP || key == K_KP_PGUP )
	{
		con.display -= 2;
		return;
	}

	if (key == K_PGDN || key == K_KP_PGDN ) 
	{
		con.display += 2;
		if (con.display > con.current)
			con.display = con.current;
		return;
	}

	if (key == K_HOME || key == K_KP_HOME )
	{
		con.display = con.current - con.totallines + 10;
		return;
	}

	if (key == K_END || key == K_KP_END )
	{
		con.display = con.current;
		return;
	}

	if (key < 32 || key > 127)
		return;	// non printable

	if (key_linepos < MAXCMDLINE-1)
	{
		key_lines[edit_line][key_linepos] = key;
		key_linepos++;
		key_lines[edit_line][key_linepos] = 0;
	}

}
	
/*
========================
KEY_stringtokeynum

Returns a key number to be used to index keybibdings[] by looking at
the given string. Single ascii characters return themselves, while
the K_* names are matched up.
========================
*/

int KEY_stringtokeynum( char *str )
{
	t_keyname *kn;
	
	if ( !str || !str[0] )
		return -1;
	if ( !str[1] )
		return str[0];
	
	for ( kn = keynames; kn->name; kn++ )
	{
		if ( strcasecmp( str, kn->name ))
			return kn->keynum;
	}
	return -1;
} 

/*
=============================
KEY_keynumtostring

Returns a string (either a single ascii char, or a K_* name) for
the keynum.
=============================
*/

char *KEY_keynumtostring( int keynum )
{
	t_keyname *kn;
	static char  tinystr[2];
	
	if ( keynum == -1 )
		return "<KEY NOT FOUND>";
	if ( keynum > 32 && keynum < 127 )
	{
		// printable ascii
		tinystr[0] = keynum;
		tinystr[1] = 0;
		return tinystr;
	}
	for ( kn = keynames; kn->name; kn++ )
		if ( keynum == kn->keynum )
			return kn->name;
	return "<UNKNOWN KEYNUM>";
}

/*
======================
KEY_setbinding
======================
*/
void KEY_setbinding( int keynum, char *binding )
{
	char *new;
	
	int  l;
	
	if ( keynum == -1 )
		return;
		
	// free old bindings
	if ( keybindings[keynum] )
	{
		FREE( keybindings[keynum] );
		keybindings[keynum] = NULL;
	}

	// allocate memory for new binding
	l = strlen( binding );
	new = ALLOC( l + 1 );
	strcpy( new, binding );
	new[l] = 0;
	keybindings[keynum] = new;

}
	
/*
=========================
KEY_unbind_f
=========================
*/

void KEY_unbind_f( void )
{
	int   b;
	
	if ( CMD_argc() != 2 )
	{
		printf( "unbind <key> : remove commands from a key\n ");
		return;
	}
	
	b = KEY_stringtokeynum( CMD_argv(1) );
	if ( b == -1)
	{
		printf( "\"%s\" isn't a valid key\n", CMD_argv(1));
		return;
	}
	KEY_setbinding(b, "");
}

void KEY_unbindall_f( void )
{
	int   i;
	for ( i = 0; i < 256; i++ )
		if ( keybindings[i] )
			KEY_setbinding( i, "" );
}

/*
=========================
KEY_bind_f
=========================
*/
void KEY_bind_f( void )
{
	int    i,c,b;
	char   cmd[1024];

	c = CMD_argc();
	if ( c < 2 )
	{
		printf( "bind <key> [command] : attach a command to a key \n");
		return;
	}
	b = KEY_stringtokeynum( CMD_argv(1) );
	if ( b == -1 )
	{
		printf( "\"%s\" isn't a valid key\n", CMD_argv(1));
		return;
	} 
	if( c == 2 )
	{
		if ( keybindings[b] )
			printf( "\"%s\" = \"%s\"\n", CMD_argv(1), keybindings[b] );
		else
			printf( "\"%s\" is not bound\n", CMD_argv(1) );
		return;
	}
	// copy the rest of the command line
	cmd[0] = 0;
	for ( i = 2; i < c; i ++ )
	{
		strcat ( cmd, CMD_argv(i) );
		if ( i != (c - 1))
			strcat(cmd, " " );	
	}

	KEY_setbinding(b, cmd);
}	
/*
=====================
KEY_writebindings

Writes lines containing "bind key value"
=====================
*/
void KEY_writebindings( FILE *f )
{
	int  i;
	
	for ( i = 0; i < 256; i++ )
		if ( keybindings[i] && keybindings[i][0] )
			fprintf( f, "bind %s \"%s\"\n", KEY_keynumtostring(i), keybindings[i] );
}

/*
=================
KEY_bindlist_f

=================
*/
void  KEY_bindlist_f( void )
{
	int    i;
	for ( i = 0; i < 256; i ++ )
	{
		if ( keybindings[i] && keybindings[i][0] )
			printf( " %s \"%s \" \n", KEY_keynumtostring(i), keybindings[i] );
	} 
}	

void KEY_init()
{
	int i;

	for ( i = 0; i < 32; i++ )
	{
		key_lines[i][0] = ']';
		key_lines[i][1] = 0;
	}
	key_linepos = 1;

	//
	// init ascii characters in console mode
	//

	for( i = 32; i < 128; i++ )
		consolekeys[i] = TRUE;

	consolekeys[K_ENTER] = TRUE;
	consolekeys[K_KP_ENTER] = TRUE;
	consolekeys[K_TAB] = TRUE;
	consolekeys[K_LEFTARROW] = TRUE;
	consolekeys[K_KP_LEFTARROW] = TRUE;
	consolekeys[K_RIGHTARROW] = TRUE;
	consolekeys[K_KP_RIGHTARROW] = TRUE;
	consolekeys[K_UPARROW] = TRUE;
	consolekeys[K_KP_UPARROW] = TRUE;
	consolekeys[K_DOWNARROW] = TRUE;
	consolekeys[K_KP_DOWNARROW] = TRUE;
	consolekeys[K_BACKSPACE] = TRUE;
	consolekeys[K_HOME] = TRUE;
	consolekeys[K_KP_HOME] = TRUE;
	consolekeys[K_END] = TRUE;
	consolekeys[K_KP_END] = TRUE;
	consolekeys[K_PGUP] = TRUE;
	consolekeys[K_KP_PGUP] = TRUE;
	consolekeys[K_PGDN] = TRUE;
	consolekeys[K_KP_PGDN] = TRUE;
	consolekeys[K_SHIFT] = TRUE;
	consolekeys[K_INS] = TRUE;
	consolekeys[K_KP_INS] = TRUE;
	consolekeys[K_KP_DEL] = TRUE;
	consolekeys[K_KP_SLASH] = TRUE;
	consolekeys[K_KP_PLUS] = TRUE;
	consolekeys[K_KP_MINUS] = TRUE;
	consolekeys[K_KP_5] = TRUE;

	consolekeys['`'] = FALSE;
	consolekeys['~'] = FALSE;

	for (i=0 ; i<256 ; i++)
		keyshift[i] = i;
	for (i='a' ; i<='z' ; i++)
		keyshift[i] = i - 'a' + 'A';
	keyshift['1'] = '!';
	keyshift['2'] = '@';
	keyshift['3'] = '#';
	keyshift['4'] = '$';
	keyshift['5'] = '%';
	keyshift['6'] = '^';
	keyshift['7'] = '&';
	keyshift['8'] = '*';
	keyshift['9'] = '(';
	keyshift['0'] = ')';
	keyshift['-'] = '_';
	keyshift['='] = '+';
	keyshift[','] = '<';
	keyshift['.'] = '>';
	keyshift['/'] = '?';
	keyshift[';'] = ':';
	keyshift['\''] = '"';
	keyshift['['] = '{';
	keyshift[']'] = '}';
	keyshift['`'] = '~';
	keyshift['\\'] = '|';

	//menubound[K_ESCAPE] = TRUE;
	//for (i=0 ; i<12 ; i++)
	//	menubound[K_F1+i] = TRUE;

	//
	// register our functions
	//
	CMD_addcommand ("bind",KEY_bind_f);
	CMD_addcommand ("unbind",KEY_unbind_f);
	CMD_addcommand ("unbindall",KEY_unbindall_f);
	CMD_addcommand ("bindlist",KEY_bindlist_f);
}

/*
=========================
KEY_event

Called by the system between frames for both key up and key down events
Should NOT be called during interrupt!
=========================
*/
void KEY_event( int key, t_bool down, unsigned time )
{
	char   *kb;
	char   cmd[1024];

	// hack for modal presses
	if ( key_waiting == -1 )
	{
		if ( down )
			key_waiting = key;
		return;
	}

	//update auto-repeat status
	if ( down )
	{
		key_repeats[key]++;
		if ( key != K_BACKSPACE
			&& key != K_PAUSE
			&& key != K_PGUP
			&& key != K_KP_PGUP
			&& key != K_PGDN
			&& key != K_KP_PGDN
			&& key_repeats[key] > 1 )
			return;  // ignore most autorepeats

		if ( key >= 200 && !keybindings[key] )
			printf( "%s is unbound, hit F4 to set.\n", KEY_keynumtostring(key));
	}
	else
	{
		key_repeats[key] = 0;
	}

	// console key is hardcoded, so the user can never unbind it
	if ( key == '`' || key == '~' )
	{
		if(!down)
			return;
		//CON_toggleconsole_f();
		return;
	}
	
	// track if any key during is down for BUTTON_ANY  
	keydown[key] = down;
	if ( down )
	{
		if ( key_repeats[key] == 1)
			anykeydown++;
	}
	else
	{
		anykeydown--;
		if (anykeydown < 0 )
			anykeydown = 0;
	}

	//	
	// key up event only generate commands if key binding is
	// a button command ( leading + sign ). These will occur even in console mode.
	// to keep the characte from continuing an action started before a console
	// switch. Button commands include the keynum as a parameter, so multiple
	// downs can be matched with ups.
	//
	if ( !down )
	{
		kb = keybindings[key];
		if ( kb && kb[0] == '+')
		{
			sprintf( cmd,"-%s %i %i\n", kb+1, key, time );
			CBUF_addtext( cmd );
		}
		if ( keyshift[key] != key )
		{
			kb = keybindings[keyshift[key]];
			if ( kb && kb[0] == '+' )
			{
				sprintf( cmd, "-%s %i  %i\n", kb+1, key, time );
				CBUF_addtext( cmd );
			}
		}
		return;
	}

	//
	// if not a consolekey, send to the interpreter no matter what mode is
	//
	if ( key_console  && !consolekeys[key] )
	{
		kb = keybindings[ key ];
		if ( kb )
		{
			if ( kb[0] == '+' )
			{
				//button commands add keynum and time as a parm
				sprintf( cmd, "%s %i %i\n", kb, key, time );
				CBUF_addtext( cmd );
			}
			else
			{
				CBUF_addtext( kb );
				CBUF_addtext( "\n" );
			}
		}
		return;
	}
	if ( !down )
		return;

	if ( shift_down )
		key = keyshift[key];
/*	switch( cls.key_dest )
	{
		case key_console:
		case key_session:
			KEY_console(key);
			break;
		default:
			printf( "Bad key dest\n");
	}
*/
}

/*
=============================
KEY_clearstates

=============================
*/

void KEY_clearstates( void )
{
	int i;
	
	anykeydown = FALSE;

	for ( i = 0; i < 256; i ++ )
	{
		if ( keydown[i] || key_repeats[i] )
			KEY_event( i, FALSE, 0 );
		keydown[i] = 0;
		key_repeats[i] = 0;
	}
}

int KEY_getkey( void )
{
	key_waiting = -1;
	
	while( key_waiting == -1)
		SYS_sendkeyevents();
	return key_waiting;
}	
