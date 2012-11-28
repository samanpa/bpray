#ifndef BP_CAMERA
#define BP_CAMERA

#include <bp_types.h>

typedef struct _camera_t {
	vector_t location;
	vector_t direction;
	vector_t right;
	vector_t up;
	vector_t sky;
} camera_t;


void bp_camera_init (camera_t *camera);
void bp_camera_set_direction (camera_t *camera, vector_t direction);
void bp_camera_set_angle (camera_t *, float);
void bp_camera_set_look_at (camera_t *, vector_t);

void
bp_camera_get_ray (camera_t *camera, int i, int j, ray_t *ray);

void
bp_camera_get_ray2x2 (camera_t *camera, int i, int j, ray4_t*ray);
#endif
