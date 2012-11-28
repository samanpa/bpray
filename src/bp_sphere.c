#include <bp_object.h>
#include <bp_debug.h>
#include <bp_material.h>

#define get_sphere(prim_id) (primitives_accel [prim_id].shape.sphere)
#define get_sphere_center(prim_id) ((get_sphere (prim_id)).center)
#define get_sphere_radius(prim_id) ((get_sphere (prim_id)).radius)

static int intersect (unsigned int prim_id , const ray_t *ray, intersect_t *isect) {
	vector_t dest;
	double B, C, D;
	const float EPSILON = 0.003;

	SUB (dest, ray->orig, get_sphere_center (prim_id));
	B = DOT (dest, ray->dir);
	C = DOT (dest, dest);
	C -= get_sphere_radius (prim_id) * get_sphere_radius (prim_id);

	D = B*B  - C;

	if (D > 0) {
		const double sqrt_D = sqrt (D),
			t0 = -B - sqrt_D,
			t1 = -B + sqrt_D;

		if (t0 > EPSILON && t0 <= isect->t) {
			isect->t = t0;
			return 1;
		}
		else if (t1 > EPSILON && t1 <= isect->t) {
			isect->t = t1;
			return 1;
		}
		return 0;
	}
	return 0;
}

static int has_intersect (unsigned int prim_id , const ray_t *ray, double t_max) {
	vector_t dest;
	double B, C, D;
	const float EPSILON = 0.003;

	SUB (dest, ray->orig, get_sphere_center (prim_id));
	B = DOT (dest, ray->dir);
	C = DOT (dest, dest);
	C -= get_sphere_radius (prim_id) * get_sphere_radius (prim_id);

	D = B*B  - C;

	if (D > 0) {
		const double sqrt_D = sqrt (D);
		const double t0 = -B - sqrt_D,
			t1 = -B + sqrt_D;

		if (t0 > EPSILON && t0 <= t_max)
			return 1;
		else if (t1 > EPSILON && t1 <= t_max)
			return 1;
		return 0;
	}
	return 0;
}

static void get_sphere_normal (const intersect_t *i, vector_t normal)
{
	float r_inv = 1 / get_sphere_radius (i->prim_id);
	SUB  (normal, i->pos, get_sphere_center (i->prim_id));

	/* normalize the normal vector */
	SMUL (normal, r_inv, normal);
}

static void
get_2d_point (const intersect_t *i, double *u, double *v)
{
	vector_t orig_sphere;
	double r_inv;

	r_inv = 1/get_sphere_radius (i->prim_id);
 	SUB (orig_sphere, i->pos, get_sphere_center (i->prim_id));
	SMUL (orig_sphere, r_inv, orig_sphere);

	*v = acos (orig_sphere [0]) / M_PI;
	*u = acos (orig_sphere [2] / sin (M_PI * *v))/ (2 * M_PI);
}

static void
get_sphere_bounds (unsigned int prim_id, vector_t lbb, vector_t rtf)
{
	vector_t r_vec;
	VSET_ALL (r_vec, get_sphere_radius (prim_id));
	SUB (lbb, get_sphere_center (prim_id), r_vec);
	ADD (rtf, get_sphere_center (prim_id), r_vec);
}

static int
sphere_clip (unsigned int prim_id, const vector_t corner0, const vector_t corner1,
	     vector_t lower, vector_t upper)
{
	get_sphere_bounds (prim_id, lower, upper);

	if (lower [0] < corner0 [0]) lower [0] = corner0 [0];
	if (lower [1] < corner0 [1]) lower [1] = corner0 [1];
	if (lower [2] < corner0 [2]) lower [2] = corner0 [2];
	if (upper [0] > corner1 [0]) upper [0] = corner1 [0];
	if (upper [1] > corner1 [1]) upper [1] = corner1 [1];
	if (upper [2] > corner1 [2]) upper [2] = corner1 [2];

	return 1;
}

static object_vtable_t sphere_vtable = {
	intersect,
	has_intersect,
	get_sphere_normal,
	get_2d_point,
	get_sphere_bounds,
	sphere_clip
};

static vector_t curr_center;
static float curr_radius;

void
sphere_post_init (unsigned int prim_id)
{
	ASSIGN (get_sphere_center (prim_id), curr_center);
	get_sphere_radius (prim_id) = curr_radius;
	get_primitive (prim_id)->functions = &sphere_vtable;
}

void
bp_sphere (vector_t center, float r, material_t *mat)
{
	unsigned int obj_id = bp_object_generate_new ();

	ASSIGN (curr_center, center);
	curr_radius = r;

	bp_begin1 (obj_id, BP_SPHERE);
	{
		bp_material (mat);
	}
	bp_end ();
}
