#include "c3dinit.h"

t_cvar *castsrootvar;
/*
============================
Initialize C3d 
variables from configuration.
============================
*/
void C3D_initvar()
{
	castsrootvar = CVAR_get("casts_root", "", CVAR_USERINFO );
}
