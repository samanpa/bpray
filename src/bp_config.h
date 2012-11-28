#ifndef BP_CONFIG
#define BP_CONFIG

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WIN32
/* we should perhaps do a dllimport too but that might cause some 
   trouble when compiling with mingw*/
#define BP_EXPORT __declspec(dllexport)
/* for glib to know we are on windows */
#define G_OS_WIN32
#else
#define BP_EXPORT
#endif

/* we need to have this here cos of the G_OS_WIN32 */
#include <glib.h>

#endif
