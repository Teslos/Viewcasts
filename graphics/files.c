#include "common.h"
#include "memory.h"

char	fs_sessiondir[MAX_OSPATH];
t_cvar	*fs_basedir;
t_cvar	*fs_cddir;
t_cvar  *fs_sessiondirvar;

typedef struct filelink_s
{
	struct filelink_s	*next;
	char	*from;
	int		fromlength;
	char	*to;
} t_filelink;

t_filelink	*fs_links;

typedef struct
{
	char	name[MAX_QPATH];
	int		filepos, filelen;
} t_packfile;

typedef struct pack_s
{
	char	filename[MAX_OSPATH];
	FILE	*handle;
	int		numfiles;
	t_packfile	*files;
} t_pack;

typedef struct searchpath_s
{
	char	filename[MAX_OSPATH];
	t_pack	*pack;		// only one of filename / pack will be used
	struct searchpath_s *next;
} t_searchpath;

t_searchpath	*fs_searchpaths;
t_searchpath	*fs_base_searchpaths;	// without gamedirs

int file_from_pak = 0;

int FS_filelength( FILE *f )
{
	int  pos;
	int  end;

	pos = ftell ( f );
	fseek( f, 0, SEEK_END );
	end = ftell(f);
	fseek ( f, pos, SEEK_SET );

	return end;
}

void FS_createpath( char *path )
{
	char *ofs;

	for (ofs = path+1; *ofs ; ofs++ )
	{
		if ( *ofs == '/' )
		{
			*ofs = 0;
			SYS_mkdir(path);
			*ofs = '/';
		}
	}
}

void FS_fclosefile( FILE *f )
{
	fclose ( f );
}

/* Finds the file in the search path.
 * returns file size and an open FILE *
 * Used for streaming data.
 */
int FS_fopenfile( char *filename, FILE **file )
{
	t_searchpath    *search;
	char            netpath[MAX_OSPATH];
	t_pack          *pak;
	int             i;
	t_filelink      *link;

	file_from_pak = 0;

	for ( link = fs_links; link ; link = link->next )
	{
		if ( !strncmp( filename, link->from, link->fromlength ) )
		{
			sprintf( netpath, "%s%s", link->to, filename+link->fromlength);
			*file = fopen(netpath, "rb");
			if (*file)
			{
				printf( "link file: %s\n", netpath );
				return FS_filelength( *file );
			}
			return -1;
		}
	}
	/* search through the path, one element at a time */
	for ( search = fs_searchpaths; search ; search = search->next )
	{
		/* is file a pak file */
		if ( search->pack )
		{
			/* look through all the pak file elements */
			pak = search->pack;
			for ( i = 0; i < pak->numfiles; i++ )
				if ( !strcasecmp(pak->files[i].name, filename ) )
				{
					file_from_pak = 1;
					printf(" packfile: %s : %s\n", pak->filename, filename );
					*file = fopen( pak->filename, "rb" );
					if ( !*file )
						COM_error( ERR_FATAL, "Couldn't reopen %s", pak->filename );
					fseek ( *file, pak->files[i].filepos, SEEK_SET );
					return pak->files[i].filelen;
				}
		}
		else
		{
			/* check file in the directory tree */
			sprintf( netpath, "%s/%s", search->filename, filename );

			*file = fopen( netpath, "rb" );
			if (!*file)
				continue;

			printf( "findfile: %s\n", netpath );
			return FS_filelength(*file);
		}
	}

	printf("findfile: can't find %s\n", filename );
	*file = NULL;
	return -1;
}

#define	MAX_READ	0x10000		// read in blocks of 64k
/*
 * Properly handles partial reads
 */
void FS_read( void *buffer, int len, FILE *f )
{
	int   block, remaining;
	int   read;
	byte  *buf;
	int   tries;

	buf = (byte *) buffer;

	/* read in chunks for progress bar */
	remaining = len;
	tries = 0;
	while( remaining )
	{
		block = remaining;
		if ( block > MAX_READ )
			block = MAX_READ;
		read = fread( buf, 1, block, f );
		if ( read == 0 )
		{
			if (!tries)
			{
				tries = 1;
			}
			else
				COM_error( ERR_FATAL, "FS_read: 0 bytes read" );
		}

		if (read == -1)
			COM_error( ERR_FATAL, "FS_read: -1 bytes read");

		/* do some progress bar thing here */
		remaining -= read;
		buf += read;
	}
}

/* Filename are relative to the search path
 * a null buffer will just return the file length without loading
 */
int FS_loadfile( char *path, void **buffer )
{
	FILE   *h;
	byte   *buf;
	int    len;

	buf = NULL;    

	/* look for it in the filesystem or pack files */
	len = FS_fopenfile(path, &h);
	if ( !h )
	{
		if (buffer)
			*buffer = NULL;
		return -1;
	}
	if (!buffer)
	{
		fclose (h);
		return len;
	}
	buf = (byte *) ALLOC(len);
	*buffer = buf;

	FS_read(buf, len, h);
	fclose(h);
	return len;
}

void FS_freefile( void *buffer )
{
  FREE(buffer);
}

void FS_addpathdirectory(char *dir)
{
	int  i;
	t_searchpath *search;
	
	strcpy( fs_sessiondir, dir );
	
	// 
	//  add directory to the search path
	//	
	search = ALLOC( sizeof( t_searchpath ));
	strcpy ( search->filename, dir );
	search->pack = NULL;
	search->next = fs_searchpaths;
	fs_searchpaths = search;
}
/*
 * Creates a t_filelink
 */
void FS_link_f(void)
{
	t_filelink   *l, **prev;

	if( CMD_argc() != 3 )
	{
		printf( "USAGE: link <from> <to> \n");
		return;
	}

	/* see if the link already exists */
	prev = &fs_links;
	for ( l = fs_links; l ; l = l->next )
	{
		if ( !strcmp(l->from, CMD_argv(1)))
		{
			FREE(l->to);
			if(!strlen(CMD_argv(2)))
			{
				/* delete it */
				*prev = l->next;
				FREE( l->from );
				FREE( l );
				return;
			}
			l->to = copystring(CMD_argv(2));
			return;
		}
		prev = &l->next;
	}
	/* create a new link */
	l = (t_filelink *) ALLOC(sizeof(*l));
	l->next = fs_links;
	fs_links = l;
	l->from = copystring(CMD_argv(1));
	l->fromlength = strlen(l->from);
	l->to = copystring(CMD_argv(2));
}

char **FS_listfiles( char *findname, int *numfiles, unsigned musthave, unsigned canthave )
{
	char *s;
	int nfiles = 0;
	char **list = 0;

	s = SYS_findfirst( findname, musthave, canthave );
	while ( s )
	{
		if ( s[strlen(s)-1] != '.' )
			nfiles++;
		s = SYS_findnext( musthave, canthave );
	}
	SYS_findclose ();

	if ( !nfiles )
		return NULL;

	nfiles++; // add space for a guard
	*numfiles = nfiles;

	list = malloc( sizeof( char * ) * nfiles );
	memset( list, 0, sizeof( char * ) * nfiles );

	s = SYS_findfirst( findname, musthave, canthave );
	nfiles = 0;
	while ( s )
	{
		if ( s[strlen(s)-1] != '.' )
		{
			list[nfiles] = strdup( s );
#ifdef _WIN32
			strlwr( list[nfiles] );
#endif
			nfiles++;
		}
		s = SYS_findnext( musthave, canthave );
	}
	SYS_findclose ();

	return list;
}


void FS_dir_f( void )
{
	char *path = NULL;
	char findname[1024];
	char wildcard[1024] = "*.*";
	char **dirnames;
	int  ndirs;

	if ( CMD_argc() != 1 )
	{
		strcpy( wildcard, CMD_argv( 1 ));
	}

	while ((path = FS_nextpath( path )) != NULL )
	{
		char *tmp = findname;
		sprintf( findname, "%s/%s", path, wildcard );

		while ( *tmp != 0 )
		{
			if ( *tmp == '\\' )
				*tmp = '/';
			tmp++;
		}
		printf( "Directory of %s\n", findname );
		printf( "---------\n" );

		if (( dirnames = FS_listfiles( findname, &ndirs, 0, 0 )) != 0)
		{
			int i;
			for ( i = 0; i < ndirs-1; i++ ) 
			{
				if ( strrchr( dirnames[i], '/'))
					printf( "%s\n", strrchr( dirnames[i], '/' ) + 1);
				else
					printf( "%s\n", dirnames[i]);
				free ( dirnames );
			}
			free( dirnames );
		}
		printf( "\n" );
	};
}

void FS_path_f ( void )
{
	t_searchpath   *s;
	t_filelink     *l;

	printf ( "Current search path:\n");
	for ( s = fs_searchpaths; s ; s = s->next )
	{
		if ( s == fs_base_searchpaths )
			printf( "--------------\n");
		if ( s->pack )
			printf( "%s (%i files)\n", s->pack->filename, s->pack->numfiles);
		else
			printf( "%s\n", s->filename );

	}
	printf( "\nLinks:\n");

	for ( l = fs_links; l ; l = l->next )
		printf( "%s : %s\n", l->from, l->to );
}

/*
 * Allows enumerating all of the directories in the search path
 */
char *FS_nextpath( char *prevpath )
{
	t_searchpath   *s;
	char           *prev;

	if (!prevpath)
		return fs_sessiondir;

	prev = fs_sessiondir;
	for ( s = fs_searchpaths; s ; s = s->next )
	{
		if ( s->pack )
			continue;
		if ( prevpath == prev )
			return s->filename;
		prev = s->filename;
	}
	return NULL;
}

void FS_initfilesystem( void )
{
	CMD_addcommand( "path", FS_path_f );
	CMD_addcommand( "link", FS_link_f );
	CMD_addcommand( "dir", FS_dir_f );

	/* allows color to run from outside the data tree */
	fs_basedir = CVAR_get( "basedir", ".", CVAR_NOSET );

	/* logically concatenates the cddir after the basedir for 
	 * allows the color to run from outside the data tree.
	 */
	fs_cddir = CVAR_get( "cddir", "./", CVAR_NOSET );
	if ( fs_cddir->string[0] )
		FS_addpathdirectory( va( "%s/"BASEDIRNAME, fs_basedir->string ));

	/* any set dirs will be freed up to here */
	fs_base_searchpaths = fs_searchpaths;

	fs_sessiondirvar = CVAR_get( "session", "", CVAR_LATCH | CVAR_SERVERINFO );
	if ( fs_sessiondirvar->string[0] )
		; /* do nothing */ 
}
