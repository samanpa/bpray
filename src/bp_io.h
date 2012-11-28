#ifndef BP_IO
#define BP_IO

#include <bp_scene.h>
#include <stdlib.h>
#include <unistd.h>

FILE *bp_file_open (scene_t *scene, const char *filename, const char *mode);
BP_EXPORT void bp_library_path_add1 (scene_t *scene, char const*path);
BP_EXPORT void bp_library_path_add (scene_t *scene, char *path);

int bp_io_readint32 (FILE *file, unsigned int *input);

int bp_io_readfloat (FILE *file, float *input);

void bp_io_writeint32 (FILE *file, unsigned int input);

void bp_io_writefloat (FILE *file, float finput);

#endif
