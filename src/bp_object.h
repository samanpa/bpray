#ifndef BP_OBJECT
#define BP_OBJECT

typedef struct object_t object_t;

/* #include <bp_primitive.h> */
#include <bp_scene.h>

typedef struct {
	int transform_id;
	int object_id;
} instance_t;

struct object_t {
	unsigned int primitives_start;
	unsigned int primitives_end;
	unsigned int vertices_start;
	unsigned int vertices_end;
};


object_t * bp_object_new ();

void bp_objects_alloc ();

void bp_objects_init (scene_t *);

unsigned int bp_object_generate_new ();

void bp_objects_destroy ();

#endif
