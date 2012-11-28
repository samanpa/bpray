#include "bp_scene.h"
#include "bp_script.h"
#include "bp_io.h"
#include "bp_noise.h"
#include "bp_object.h"
#include <string.h>

static void io_init (const char *appname)
{
	char *path, *dir;

	/* add the current directory */
	path = getcwd (NULL, 0);
	bp_library_path_add1 (curr_scene, path);
	free (path);

	/* add the standard include dir to the libray path */
	dir = g_path_get_dirname (appname);
	path = g_strdup_printf ("%s" G_DIR_SEPARATOR_S ".." G_DIR_SEPARATOR_S "etc"
				G_DIR_SEPARATOR_S "banna" G_DIR_SEPARATOR_S, dir);
	bp_library_path_add1 (curr_scene, path);
	free (path);
	free (dir);

	/* add every thing in the library path*/
	bp_library_path_add (curr_scene, getenv ("Library_Path"));
}

static void ini_init (const char *filename)
{
	unsigned int len;
	char *dir;

	/* add the directory of the ini file */
	dir = g_path_get_dirname (filename);
	bp_library_path_add1 (curr_scene, dir);
	free (dir);

	len = strlen (filename);

	if (len > 4 && filename [--len] == 'i' 
	       && filename [--len] == 'n'
	       && filename [--len] == 'i'
	       && filename [len - 1] == '.') {
#define repl(file, a, b, c) { \
	file = strdup (filename);\
        file [len] = a; \
        file [len + 1] = b; \
        file [len + 2] = c; \
}
		repl (curr_scene->settings.input_filename,  'b', 'p', 'r');
#undef repl
	}
}


BP_EXPORT void bp_ray_init (const char *appname, const char *ini_filename, scene_t *scene)
{
	perlin_init ();

	io_init (appname);

	if (ini_filename) {
		ini_init (ini_filename);
		if (bp_ini_parse_file (scene, ini_filename))
			exit (1);
	}
	bp_scene_init (scene);
}


BP_EXPORT void bp_ray_destroy (scene_t *scene)
{
 	bp_lights_destroy ();
 	bp_objects_destroy ();
  	bp_scene_destroy (scene);
}
