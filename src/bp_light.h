#ifndef BP_LIGHT
#define BP_LIGHT

#include <bp_types.h>

typedef struct light_t light_t;

#include <bp_scene.h>

struct light_t {
	vector_t pos;
	vector_t color;
/* 	float radius; */
};

light_t *
bp_light_new (vector_t, vector_t);

void
bp_lights_destroy ();

void
bp_lights_init (scene_t *scene);

#endif
