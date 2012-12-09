#include "bp_shade.h"

static const float AIR_IOR   = 1.0003;

#define REFLECTED_RAY(res, dot, t, normal, dir, calc_dot) \
                if (calc_dot) \
                       dot = DOT (normal, dir); \
		(t) = - 2 * (dot); \
		SCALE_ADD3 (res, t, normal, dir);

static void 
get_color (const scene_t *scene, const intersect_t *i, const vector_t raydir
	   , vector_t normal, int srcprim_id, int depth, float fdepth
	   , vector_t color ,vector_t objcolor)
{
	float refl, tmp1, tmp2;
	float cos_inc, cos_refr = 0;
	float mult = 1.;
	int do_refraction = 0;
	ray_t ray;
	vector_t ncolor;
	float rel_ior = 0;
	float ior_from, ior_to;
	vector_t neg_ray_dir;
	texture_t *texture;
	float transmit;
	material_t *intersect_material = get_material (i->prim_id);

	
	texture  = intersect_material->texture;
	transmit = texture->pigment->transmit;

	NEG (neg_ray_dir, raydir);
	cos_inc = DOT (normal, neg_ray_dir);

	/* test to see if we are coming from inside an object or outside */
	if (cos_inc >= 0) {
		ior_to = intersect_material->ior;
		ior_from = (srcprim_id != -1) ? get_material (srcprim_id)->ior : AIR_IOR;
	}
	else {
 		if (ZERO1 (transmit)) {
 			return;
		}
		mult = -1;
		/*
		  since we are inside the object, angle of incidence is 
		  different -angle is faster than doing another dot product
		*/
		cos_inc = -cos_inc;
		ior_from = intersect_material->ior;
		/*
		  FIXME : find the ior of the object that we might be entering
		*/
		ior_to = AIR_IOR;
	}


	if (NOT_ZERO1 (ior_to)) {
		vector_t filtered_color;
		SMUL (filtered_color, texture->pigment->filter, objcolor);
		rel_ior = ior_from / ior_to;
		tmp1 = 1 - rel_ior * rel_ior *  (1.0 - cos_inc * cos_inc);
		
		if (tmp1 > 0.0 ) {

			cos_refr = sqrt (tmp1);
		
			if (texture->finish->options & (OPTION_METALLIC | OPTION_FRESNEL) ) {
				/* do fresnel */
			
				tmp1 = (ior_from * cos_inc - ior_to * cos_refr) / 
					(ior_from * cos_inc + ior_to * cos_refr);
				tmp1 = tmp1 * tmp1;
		
				tmp2 = (ior_to * cos_inc - ior_from * cos_refr) /
					(ior_to * cos_inc + ior_from * cos_refr);
				tmp2 = tmp2 * tmp2;
			
				refl = (tmp1 + tmp2) /2;

				assert (refl <= 1.0);
			} else
				refl = texture->finish->reflection;
		
		
			/* do refraction */
			if (transmit > SMALL_VAL) {
				do_refraction = 1;
			}
		}
		else { /* total internal reflection happens */ /* FIXME: is this true */
			ASSIGN (ray.orig, i->pos);
			
			refl = texture->finish->reflection;
                        REFLECTED_RAY (ray.dir, tmp2, tmp1, normal, raydir, 1);
			bp_ray_trace (&ray, ncolor, i->prim_id, depth + 1, transmit * fdepth);
#if 0
  			LIN_COMB3 (color, transmit, ncolor, 1 - transmit, filtered_color);
#else
			SCALE_ADD3 (color, transmit, ncolor, color);
#endif
		}
	} else {
		refl = texture->finish->reflection;
	}

	
	/* do reflection :
	      the color variable holds the color due to refraction
	*/
	if (refl > SMALL_VAL) {
		ASSIGN (ray.orig, i->pos);
		REFLECTED_RAY (ray.dir, tmp2, tmp1, normal, raydir, 1);
		bp_ray_trace (&ray, ncolor, i->prim_id, depth + 1, refl * fdepth);
 		SCALE_ADD3 (color, refl, ncolor, color);
	}

	if (do_refraction) {
		ASSIGN (ray.orig, i->pos);
		tmp1 = mult * (rel_ior * cos_inc - cos_refr);
		LIN_COMB3 (ray.dir, tmp1, normal, rel_ior, raydir);
		bp_ray_trace (&ray, ncolor, i->prim_id, depth + 1, transmit * fdepth);
#if 0
  		SCALE_ADD3 (color, transmit, ncolor, color);
#else
		LIN_COMB3 (color, transmit, ncolor, 1 - transmit, color);
#endif
	}
}


static void
phong_shade (const scene_t *scene, const intersect_t *isect,
	     const vector_t raydir, vector_t normal, vector_t color,
	     vector_t objcolor)
{
	light_t *light_itr;
	texture_t *texture;
	vector_t neg_raydir;
	vector_t phong_i;
	int i = 0;

	NEG (neg_raydir, raydir);
	texture = get_material (isect->prim_id)->texture;

        /* FIXME: There are too many access to the material
	 *         and this compounded by the indirection involved. */ 
	texture->normal->get_normal (isect, normal);

	VSET_ALL (phong_i, 0.0);
	for (light_itr = scene->lights [i]; light_itr; light_itr = scene->lights [++i]) {
		vector_t light_dir, refl_light;
		ray_t ray;
		
		/* find a unit vector from the object to the light */
		SUB (light_dir, light_itr->pos, isect->pos);
 		double t = MAG (light_dir);
		SMUL (light_dir, 1/t, light_dir);
		
		/* see if object is in the shade */
		ASSIGN (ray.orig, isect->pos);
		ASSIGN (ray.dir, light_dir);
		if (bp_kd_tree_has_occluder (scene->kd_tree_root, &ray, t))
			continue;

		float dot = DOT (normal, light_dir);
		/* FIXME: do fabs because normal may turn inwards */
 		float diffuse = fabs(dot)* texture->finish->diffuse;
		float phong_f = diffuse;

		/* specular lighting */
		if (texture->finish->shininess != 0.f) {
			SCALE_SUB3 (refl_light, 2 * dot, normal, light_dir);
			dot = DOT (neg_raydir, refl_light);
 			if (dot > 0) { 
				vector_t scolor;
				float specular = texture->finish->specular *
					pow (dot, texture->finish->shininess);
				phong_f += specular;
			}
		}
		SCALE_ADD3(phong_i, phong_f, light_itr->color, phong_i); 
	}

	texture->pigment->get_color (texture->pigment, isect, objcolor);
	MUL (color, phong_i, objcolor);
	ADD (color, color, scene->settings.atmosphere.ambient_light);
}

#include "math_sse.h"

#define get_isect(i, i4, id) do { \
        (i).u = simd4_get ## id ((i4)->u); \
        (i).v = simd4_get ## id ((i4)->v); \
        (i).t = simd4_get ## id ((i4)->t); \
        (i).pos [X_axis] = simd4_get ## id ((i4)->pos [X_axis]); \
        (i).pos [Y_axis] = simd4_get ## id ((i4)->pos [Y_axis]); \
        (i).pos [Z_axis] = simd4_get ## id ((i4)->pos [Z_axis]); \
        (i).prim_id = simd4i_get ## id ((i4)->prim_id); \
} while (0)

#define get_ray(r, r4, id) do { \
        r.orig [X_axis] = simd4_get ## id (r4->orig [X_axis]); \
	r.orig [Y_axis] = simd4_get ## id (r4->orig [Y_axis]); \
	r.orig [Z_axis] = simd4_get ## id (r4->orig [Z_axis]); \
							       \
	r.dir [X_axis] = simd4_get ## id (r4->dir [X_axis]); \
	r.dir [Y_axis] = simd4_get ## id (r4->dir [Y_axis]); \
	r.dir [Z_axis] = simd4_get ## id (r4->dir [Z_axis]); \
} while (0)

#define shade(isect4, ray4, n) do {					\
 	vector_t objcolor;					        \
	vector_t normal;						\
	intersect_t isect;						\
	get_isect (isect, isect4, n);					\
	if (isect.prim_id != -1) {					\
		ray_t ray;						\
		get_ray (ray, ray4, n);					\
		/* do phong shading */					\
		phong_shade (scene, &isect, ray.dir, normal,		\
			     colors[n], objcolor);			\
		get_color (scene, &isect, ray.dir, normal,		\
			   simd4i_get##n (srcprim), depth,		\
			   simd4_get##n (fdepth), colors[n], objcolor);	\
	}								\
} while(0)
	

void bp_shade_packet (const scene_t * scene, intersect4_t *isect4
		      , const ray4_t *ray4, vector_t *colors, int depth
		      , const simd4_t fdepth, const simd4i_t srcprim)
{
 	/* find the intersection point */
	simd4_vector_SCALE_ADD (isect4->pos, isect4->t, ray4->dir, ray4->orig);

	/*-------------*/
	shade(isect4, ray4, 0);
	shade(isect4, ray4, 1);
	shade(isect4, ray4, 2);
	shade(isect4, ray4, 3);
}

void bp_shade (const scene_t * scene, intersect_t *isect, const ray_t *ray, 
	       vector_t color, const int depth, const float fdepth, unsigned int srcprim_id)
{
	vector_t objcolor;
	vector_t normal;

	/* find the intersection point */
 	SCALE_ADD3 (isect->pos, isect->t, ray->dir, ray->orig);

	/* do phong shading */
   	phong_shade (scene, isect, ray->dir, normal, color, objcolor);

    	get_color (scene, isect, ray->dir, normal, srcprim_id
		   , depth, fdepth, color, objcolor);
}
