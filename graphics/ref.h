#ifndef __REF_H
#define __REF_H

#define API_VERSION 1

#include "common.h"
#include "strukturen.h"

//
// these are the functions imported by the refresh module
//
typedef struct
{
	void	(*SYS_error) (int err_level, char *str, ...);

	void	(*CMD_addcommand) (char *name, void(*cmd)(void));
	void	(*CMD_removecommand) (char *name);
	int	(*CMD_argc) (void);
	char	*(*CMD_argv) (int i);
	void	(*CMD_executetext) (int exec_when, char *text);

	void	(*printf) (int print_level, char *str, ...);

	// files will be memory mapped read only
	// the returned buffer may be part of a larger pak file,
	// or a discrete file from anywhere in the quake search path
	// a -1 return means the file does not exist
	// NULL can be passed for buf to just determine existance
	int		(*FS_loadfile) (char *name, void **buf);
	void	(*FS_freefile) (void *buf);

	// gamedir will be the current directory that generated
	// files should be stored to, ie: "f:\quake\id1"
	char	*(*FS_gamedir) (void);

	t_cvar	*(*CVAR_get) (char *name, char *value, int flags);
	t_cvar	*(*CVAR_set)( char *name, char *value );
	void	 (*CVAR_setvalue)( char *name, float value );

} refimport_t;

typedef struct
{
	int x, y, widht, height;
	float fov_x, fov_y;
	float vieworg[3];
	float viewangles[3];
	float blend[3];
	float time;
	
}refdef_t;
typedef  struct
{
	int api_version;
	void (*C3D_updateiso) ( struct list *liso, t_bool isosurface, t_bool fcreate );
	void (*C3D_updatecut) ( struct list *lcut, t_bool fcreate );
	int (*C3D_drawisosurface) ( t_iso *tiso, int nr );
	void (*C3D_drawisovolume) ( t_iso *tiso, int nr );
	int (*C3D_drawcut) ( t_isocut *tcut, int nr );
	void (*RenderFrame) ( refdef_t *fd );
	void (*DrawGetPicSize) ( int *w, int *h, char *name );
	void (*DrawPic) (int x, int y, char *name );
	void (*DrawStretchPic) ( int x, int y, int w, int h, char *name );
	void (*DrawChar) ( int x, int y, int c );
	void (*DrawTileClear) ( int x, int y, int w, int h, char *name );
	void (*DrawFill) ( int x, int y, int w, int h, char *name );
	void (*DrawRectangle) ( int x, int y, int w, int h, char *name );
	void (*DrawFadeScreen) (void);
	
	void (*DrawStretchRaw) ( int x, int y, int w, int h, int cols, int rows, byte *data );

	void (*BeginFrame) ( float camera_separation );
	void (*EndFrame) ( void );
} refexport_t;

refexport_t GetRefAPI( refimport_t rimp );
refexport_t GetRefIsoAPI( refimport_t rimp );

#endif
