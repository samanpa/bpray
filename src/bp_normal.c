#include <bp_object.h>
#include <bp_memory.h>
#include <bp_scene.h>
#include <bp_noise.h>

/*
static void get_normal_noisy (intersect_t *i, vector_t normal)
{
	vector_t pnormal;
	double f0, fx, fy, fz;


	get_primitive_function (i->object)->get_normal (i, pnormal);

	f0 = perlin_noise (pnormal [0], pnormal [1], pnormal [2]);
 	fx = perlin_noise (pnormal [0] + 0.001, pnormal [1], pnormal [2]);
	fy = perlin_noise (pnormal [0], pnormal [1] + 0.001, pnormal [2]);
	fz = perlin_noise (pnormal [0], pnormal [1], pnormal [2] + 0.001);

	pnormal [0] -= (fx - f0) / 0.001;
 	pnormal [1] -= (fy - f0) / 0.001;
 	pnormal [2] -= (fz - f0) / 0.001;

	vector_normalize (pnormal, normal);
}
*/

static void get_normal_default (const intersect_t *i, vector_t normal)
{
	prim_get_normal (i, normal);
}

static void
get_normal_bump_mapped (const intersect_t *i, vector_t normal)
{
	double u, v;
	image_t *im;

 	prim_get_uv_coord (i, &u, &v);
	im = &get_material_from_primitive(i->prim_id)->texture->normal->u.bump_map;
	bp_image_get_color (im, u, v, 1, 1, normal);

	bp_matrix_vector_mult (normal, get_primitive_ts_matrix (i->prim_id), normal);
	vector_normalize (normal, normal);
}

normal_t *
bp_normal_new ()
{
	normal_t *normal = NEW_1 (normal_t, curr_scene->material_pool);
	normal->get_normal = get_normal_default;
	return normal;
}

normal_t *
bp_bump_map_new (image_t *im)
{
	normal_t *normal = NEW (normal_t);
	normal->u.bump_map = *im;
	normal->get_normal = get_normal_bump_mapped;
	return normal;
}

normal_t *
bp_normal_get_default ()
{
	static normal_t *normal;
	if (!normal) {
		normal = NEW_1 (normal_t, curr_scene->material_pool);
		normal->get_normal = get_normal_default;
	}
	return normal;
}
