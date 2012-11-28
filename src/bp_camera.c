#include "bp_camera.h"
#include "bp_math.h"
#include "bp_scene.h"

#include "math_sse.h"

void bp_camera_init (camera_t *camera)
{
	COPY (origin, camera->location);
	COPY (z, camera->direction);
	COPY (y, camera->up);
	COPY (y, camera->sky);
	SMUL (camera->right, 4.0/3.0, x);
}

void
bp_camera_set_direction (camera_t *camera, vector_t direction)
{
	ASSIGN (camera->direction, direction);
}

void
bp_camera_set_angle (camera_t *camera, float angle)
{
	float direction_length;

	if (angle >= 180)
		angle = 179;

	direction_length  = 0.5 * MAG (camera->right) / tan (angle / 360 * M_PI);
	VRESIZE (camera->direction, direction_length);
}

void
bp_camera_set_look_at (camera_t *camera, vector_t look_at)
{
	float dir_length, up_length, right_length;
	vector_t dir;     /* direction vector not normalized */

	dir_length   = MAG (camera->direction);
	up_length    = MAG (camera->up);
	right_length = MAG (camera->right);

	SUB   (dir, look_at, camera->location);

	CROSS   (camera->right, camera->sky, dir);
	VRESIZE (camera->right, right_length);

	CROSS   (camera->up, dir, camera->right);
	VRESIZE (camera->up, up_length);

	VSET_SIZE (camera->direction, dir, dir_length);
}


static void
sample ()
{
	sample ();
}


static int inited;
static vector_t left_top, right_step, down_step;

static inline void init_ray_computer (const camera_t *camera)
{
 	if (!inited) {
		vector_t right, down;
		float mag;
		inited = 1;

		mag = SIZE (camera->right);

		/* find the left top corner of the screen */
		ADD (left_top, camera->location, camera->direction);
		SUB (left_top, left_top, camera->right);
		ADD (left_top, left_top, camera->up);

		SMUL (right, 2, camera->right);
		SMUL (down, -2, camera->up);

		/* find the vector corresponding to a movement to the left & right */
		mag = 1.0 / (bp_scene_get_horizontal_resolution (curr_scene) - 1);
		SMUL (right_step, mag, right);
		
		mag = 1.0 / (bp_scene_get_vertical_resolution (curr_scene) - 1);
		SMUL (down_step, mag, down);
	}

}
	
void
bp_camera_get_ray (camera_t *camera, int height, int width, ray_t *ray)
{
	vector_t pos;

	init_ray_computer (camera);

	SCALE_ADD3 (pos, height, down_step, left_top);
	SCALE_ADD3 (pos, width, right_step, pos);

	ASSIGN (ray->orig, camera->location);
	SUB    (ray->dir, pos, camera->location);
	NORM   (ray->dir, ray->dir);
}

/* 
 * height, width : represents the top left pixel
 */
void
bp_camera_get_ray2x2 (camera_t *camera, int height, int width, ray4_t *ray)
{
	vector_t pos;

	init_ray_computer (camera);

	SCALE_ADD3 (pos, height, down_step, left_top);
	SCALE_ADD3 (pos, width, right_step, pos);

	simd4_vector_ASSIGN1 (ray->orig, camera->location);

	ray->dir [X_axis] = 
		_mm_sub_ps (_mm_setr_ps (pos [X_axis], 
					 pos [X_axis] + right_step [X_axis],
					 pos [X_axis] +  down_step [X_axis], 
					 pos [X_axis] + right_step [X_axis] + down_step [X_axis]),
			    ray->orig [X_axis]);

	ray->dir [Y_axis] =
		_mm_sub_ps (_mm_setr_ps (pos [Y_axis],
					pos [Y_axis] + right_step [Y_axis],
					pos [Y_axis] +  down_step [Y_axis], 
					pos [Y_axis] + right_step [Y_axis] + down_step [Y_axis]),
			    ray->orig [Y_axis]);
	ray->dir [Z_axis] =
		_mm_sub_ps (_mm_setr_ps (pos [Z_axis],
					pos [Z_axis] + right_step [Z_axis],
					pos [Z_axis] +  down_step [Z_axis], 
					pos [Z_axis] + right_step [Z_axis] + down_step [Z_axis]),
			    ray->orig [Z_axis]);


	simd4_vector_NORM (ray->dir, ray->dir);
}
