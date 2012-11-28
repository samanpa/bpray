#ifndef BP_RAY
#define BP_RAY

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <bp_config.h>
#include <bp_types.h>
#include <bp_scene.h>
#include <bp_script.h>
#include <bp_noise.h>
#include <bp_io.h>
#include <bp_utils.h>
#include <bp_object.h>

void bp_ray_init (const char *appname, const char *inifile, scene_t *scene);
void bp_ray_destroy (scene_t *scene);

#endif
