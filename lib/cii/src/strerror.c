#include <string.h>
#include <stdio.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/*lint -e613
strerror is defined here because some vendors don't provide it.
*/
#ifndef HAVE_STRERROR
char *strerror(int errnum) {
	extern int sys_nerr;
	/*extern const char *sys_errlist[];*/

	if (errnum <= 0)
		return "";
	else if (errnum >= sys_nerr) {
		static char errmsg[50];
		sprintf(errmsg, "error %d", errnum);
		return errmsg;
	} else
		return sys_errlist[errnum];
}
#endif
