#include <bp_primitive.h>
#include <bp_utils.h>
#include <bp_material.h>

#define get_vertex(vindex) ((float *)bp_arraylist_get (vertices, vindex))
#define get_tnormal(nindex) ((float *)bp_arraylist_get (normals, nindex))

#define get_vert0(tridesc) (get_vertex (tridesc->v0))
#define get_vert1(tridesc) (get_vertex (tridesc->v1))
#define get_vert2(tridesc) (get_vertex (tridesc->v2))
#define get_norm0(tridesc) (get_tnormal (tridesc->v0))
#define get_norm1(tridesc) (get_tnormal (tridesc->v1))
#define get_norm2(tridesc) (get_tnormal (tridesc->v2))

#define get_accel(prim_id) (primitives_accel [prim_id].shape.triangle)

static const int next_axes [3] = {1, 2, 0};
static const int prev_axes [3] = {2, 0, 1};

void bp_triangle_print (FILE *file, unsigned int prim_id)
{
	primitive_t *t = get_primitive (prim_id);	
 	vector3_print (file, get_vert0 (t));
 	vector3_print (file, get_vert1 (t));
 	vector3_print (file, get_vert2 (t));
}

#include "math_sse.h"
int
triangle_packet_intersect (unsigned int prim_id, const ray4_t *ray4, intersect4_t *isect4, int activeMask)
{
	static const float EPSILON =  0.03;
	simd4_t mask, hu, hv, lambda, mue;
	const struct triangle_accel_t *accel = &get_accel (prim_id);

	const int k = accel->k,
		ku = next_axes [k],
		kv = prev_axes [k];

	simd4_t nd = _mm_add_ps (ray4->dir [k],
				 _mm_add_ps (_mm_mul_ps (_mm_set1_ps (accel->n_u), ray4->dir [ku]),
					     _mm_mul_ps (_mm_set1_ps (accel->n_v), ray4->dir [kv])));

	simd4_t t = _mm_add_ps (ray4->orig [k],
				_mm_add_ps (_mm_mul_ps (_mm_set1_ps (accel->n_u), ray4->orig [ku]),
					    _mm_mul_ps (_mm_set1_ps (accel->n_v), ray4->orig [kv])));
	simd4_INVERSE (nd, nd);

	t = _mm_mul_ps (nd, _mm_sub_ps (_mm_set1_ps (accel->n_d), t));

	mask = _mm_cmpneq_ps(_mm_setr_ps (activeMask & 1,
					  activeMask & 2,
					  activeMask & 4,
					  activeMask & 8),
			     _mm_setzero_ps ());
	mask = _mm_and_ps (_mm_cmpgt_ps (t, _mm_set1_ps (EPSILON)),
			   _mm_cmpge_ps (isect4->t, t));

	if (_mm_movemask_ps (mask) == 0)
		return 0;

	hu = _mm_add_ps (ray4->orig [ku], _mm_mul_ps (t, ray4->dir [ku]));
	hv = _mm_add_ps (ray4->orig [kv], _mm_mul_ps (t, ray4->dir [kv]));

	lambda = _mm_add_ps (_mm_set1_ps (accel->b_d),
			     _mm_add_ps (_mm_mul_ps (hu, _mm_set1_ps (accel->b_nu)),
					 _mm_mul_ps (hv, _mm_set1_ps (accel->b_nv))));

	mask = _mm_and_ps (mask, _mm_cmpgt_ps (lambda, _mm_setzero_ps ()));
	if (_mm_movemask_ps (mask) == 0)
		return 0;
	
	mue = _mm_add_ps (_mm_set1_ps (accel->c_d),
			  _mm_add_ps (_mm_mul_ps (hu, _mm_set1_ps (accel->c_nu)),
				      _mm_mul_ps (hv, _mm_set1_ps (accel->c_nv))));

	mask = _mm_and_ps (mask, _mm_cmpgt_ps (mue, _mm_setzero_ps ()));
	if (_mm_movemask_ps (mask) == 0)
		return 0;
	
	mask = _mm_and_ps (mask, _mm_cmple_ps (_mm_add_ps (mue, lambda), _mm_set1_ps (1)));
	if (_mm_movemask_ps (mask) == 0)
		return 0;
	
	isect4->u       = simd4_Update (mask, lambda, isect4->u);
	isect4->v       = simd4_Update (mask, mue, isect4->v);
	isect4->t       = simd4_Update (mask, t, isect4->t);
	isect4->prim_id = simd4i_Update ((simd4i_t)mask, _mm_set1_epi32 (prim_id), isect4->prim_id);

	return _mm_movemask_ps (mask);
}

int triangle_intersect (unsigned int prim_id, const ray_t *ray, intersect_t *isect)
{
	static const float EPSILON =  0.03;

	float hu, hv, lambda, mue;
	const struct triangle_accel_t *accel = &get_accel (prim_id);

	const int k = accel->k,
		ku = next_axes [k],
		kv = prev_axes [k];

	const double nd = 1.0 / (ray->dir [k]
				+ accel->n_u * ray->dir [ku]
				+ accel->n_v * ray->dir [kv]);

	const float t = nd * (accel->n_d - ray->orig [k] 
		   - accel->n_u * ray->orig [ku] - accel->n_v * ray->orig [kv]);

 	if (t < EPSILON  || t > isect->t + 0.001 )
		return 0;

	hu = ray->orig [ku] + t * ray->dir [ku];
	hv = ray->orig [kv] + t * ray->dir [kv];

	lambda = hu * accel->b_nu + hv * accel->b_nv + accel->b_d;
	if (lambda < 0.0)
		return 0;

	mue    = hu * accel->c_nu + hv * accel->c_nv + accel->c_d;
	if (mue < 0.0)
		return 0;

	if (mue + lambda > 1.0f)
		return 0;

	isect->t = t;
	isect->u = lambda;
	isect->v = mue;
	isect->prim_id = prim_id;
	
	return 1;
}

int triangle_has_intersect (unsigned int prim_id, const ray_t *ray, double t_max)
{
	static const float EPSILON =  0.03;

	float hu, hv, t, lambda, mue;
	const struct triangle_accel_t  *accel = &get_accel (prim_id);

	const int k = accel->k,
		ku = next_axes [k],
		kv = prev_axes [k];

	const double nd = 1.0 / (ray->dir [k]
				+ accel->n_u * ray->dir [ku]
				+ accel->n_v * ray->dir [kv]);
	t = nd * (accel->n_d - ray->orig [k] 
		   - accel->n_u * ray->orig [ku] - accel->n_v * ray->orig [kv]);


 	if (t < EPSILON || t > t_max + 0.001)
		return 0;

	hu = ray->orig [ku] + t * ray->dir [ku];
	hv = ray->orig [kv] + t * ray->dir [kv];

	lambda = hu * accel->b_nu + hv * accel->b_nv + accel->b_d;
	if (lambda < 0.0)
		return 0;

	mue    = hu * accel->c_nu + hv * accel->c_nv + accel->c_d;
	if (mue < 0.0)
		return 0;
	
	if (mue + lambda > 1.0f)
		return 0;

	return 1;
}

enum {
	UPPER,
	LOWER
};

#define MAX_CLIPPED_VERTICES 48
static unsigned char polygon_end;
static vector_t clipped_polygon [MAX_CLIPPED_VERTICES];

static void
clipToPlane (unsigned int v1_index, unsigned int v2_index, float plane, int axis, int side) {
        float dir;
        float plane1, plane2;
        float *dest = clipped_polygon [polygon_end];
        float *v1 = clipped_polygon [v1_index];
        float *v2 = clipped_polygon [v2_index];

        plane2 = v2 [axis];
        plane1 = v1 [axis];
        dir = plane1 - plane2;

        /* if line from v1 to v2 lines on plane and v2*/
        if (dir > -SMALL_VAL && dir < SMALL_VAL) {
		if ((plane2 <= plane && side == LOWER)
                    || (plane2 >= plane && side == UPPER)) {
			ASSIGN (dest, v2);
			polygon_end ++;
			return;
		}
        }
        else {
                /* if v1 and v2 are on different sides of plane add the intersection point*/
                if ((plane2 < plane && plane1 > plane) ||
                    (plane2 > plane && plane1 < plane)) {
                        double t = (plane1 - plane) / dir;
                        int next_axis = next_axes [axis];
                        int prev_axis = prev_axes [axis];

                        dest [axis] = plane;
                        dest [next_axis] = v1 [next_axis] + t * (v2 [next_axis] - v1 [next_axis]);
                        dest [prev_axis] = v1 [prev_axis] + t * (v2 [prev_axis] - v1 [prev_axis]);

                        polygon_end ++;
                        dest = clipped_polygon [polygon_end];
                }

                /* if v2 is on the side we want we add it*/
                if ((plane2 <= plane && side == LOWER)
                    || (plane2 >= plane && side == UPPER)) {
                        ASSIGN (dest, v2);

			polygon_end ++;
                }

        }
}


static int
clip_to_box (unsigned int prim_id, const vector_t corner0, const vector_t corner1
	, vector_t bounding_box_lower, vector_t bounding_box_upper)
{
	int axis;
	unsigned int curr_poly_start, curr_poly_end, i;
	primitive_t *triangle = get_primitive (prim_id);

	ASSIGN (clipped_polygon [0], get_vert0 (triangle));
	ASSIGN (clipped_polygon [1], get_vert1 (triangle));
	ASSIGN (clipped_polygon [2], get_vert2 (triangle));

	curr_poly_start = 0;
	curr_poly_end = polygon_end = 3;
	for (axis = 0; axis < 3; axis ++) {
		clipToPlane (curr_poly_end - 1, curr_poly_start, corner0 [axis], axis, UPPER);
		for (; curr_poly_start < curr_poly_end - 1; curr_poly_start ++) {
			clipToPlane (curr_poly_start, curr_poly_start + 1, corner0 [axis], axis, UPPER);
		}
		curr_poly_start = curr_poly_end;
		curr_poly_end = polygon_end;

		if (curr_poly_start > curr_poly_end - 3)
			return 0;

		clipToPlane (curr_poly_end - 1, curr_poly_start, corner1 [axis], axis, LOWER);
		for (; curr_poly_start < curr_poly_end - 1; curr_poly_start ++) {
			clipToPlane (curr_poly_start, curr_poly_start + 1, corner1 [axis], axis, LOWER);
		}
		curr_poly_start = curr_poly_end;
		curr_poly_end = polygon_end;

		if (curr_poly_start > curr_poly_end - 3)
			return 0;
	}

	assert (polygon_end < MAX_CLIPPED_VERTICES);

	ASSIGN (bounding_box_lower, clipped_polygon [curr_poly_start]);
	ASSIGN (bounding_box_upper, clipped_polygon [curr_poly_start]);

	for (i = curr_poly_start + 1; i < curr_poly_end; i ++) {
		bounding_box_lower [0] = MIN (clipped_polygon [i] [0], bounding_box_lower [0]);
		bounding_box_lower [1] = MIN (clipped_polygon [i] [1], bounding_box_lower [1]);
		bounding_box_lower [2] = MIN (clipped_polygon [i] [2], bounding_box_lower [2]);

		bounding_box_upper [0] = MAX (clipped_polygon [i] [0], bounding_box_upper [0]);
		bounding_box_upper [1] = MAX (clipped_polygon [i] [1], bounding_box_upper [1]);
		bounding_box_upper [2] = MAX (clipped_polygon [i] [2], bounding_box_upper [2]);
	}
	return 1;
}

void triangle_get_normal (const intersect_t *i, vector_t normal)
{
	primitive_t *tri = get_primitive (i->prim_id);
	ASSIGN (normal, get_norm0 (tri));
}

static void 
get_2d_point (const intersect_t *i, double *u, double *v)
{
	matrix_t *m = & (get_primitive_uv_matrix (i->prim_id));
	(*u) = (*m) [0][0] * i->u + (*m) [0][1] * i->v + (*m) [0][2];
	(*v) = (*m) [1][0] * i->u + (*m) [1][1] * i->v + (*m) [1][2];;
}
static void
get_tri_bounds (unsigned int prim_id, vector_t lbb, vector_t rtf)
{
	primitive_t *tri = get_primitive (prim_id);
	
	VMIN3 (lbb, get_vert0 (tri), get_vert1 (tri), get_vert2 (tri));
	VMAX3 (rtf, get_vert0 (tri), get_vert1 (tri), get_vert2 (tri));
}

void
smooth_triangle_get_normal (const intersect_t *i, vector_t normal)
{
	double u, v;
	primitive_t *tri = get_primitive (i->prim_id);
	get_2d_point (i, &u, &v);
	LIN_COMB3  (normal, i->u, get_norm1 (tri), i->v, get_norm2 (tri));
 	SCALE_ADD3 (normal, 1 - i->u - i->v, get_norm0 (tri), normal);
	vector_normalize (normal, normal);
}


static object_vtable_t triangle_vtable = {
	triangle_intersect,
	triangle_has_intersect,
	triangle_get_normal,
	get_2d_point,
	get_tri_bounds,
	clip_to_box
};

static object_vtable_t smooth_triangle_vtable = {
	triangle_intersect,
	triangle_has_intersect,
	smooth_triangle_get_normal,
	get_2d_point,
	get_tri_bounds,
	clip_to_box
};

static void
make_accel (unsigned int tri_id)
{
	primitive_t *tri = get_primitive (tri_id);
	vector_t b, c, N;
	vector_t A;
	double inv, k_inv;
	int k, u, v;
	
	ASSIGN (A, get_vert0 (tri));

	SUB (b, get_vert2 (tri), A);
	SUB (c, get_vert1 (tri), A);

	CROSS (N, c, b);

	if (fabs (N [X_axis]) > fabs (N [Y_axis])) {
		if (fabs (N [X_axis]) > fabs (N [Z_axis]))
			k = X_axis;
		else
			k = Z_axis;
	}
	else {
		if (fabs (N [Y_axis]) > fabs (N [Z_axis]))
			k = Y_axis;
		else
			k = Z_axis;
	}
	u = next_axes [k];
	v = prev_axes [k];

	k_inv = 1 / N [k];
	inv  = 1.0 / (b [u] * c [v] - b [v] * c [u]);

	get_accel (tri_id).k = k;

	get_accel (tri_id).n_u = N [u] * k_inv;
	get_accel (tri_id).n_v = N [v] * k_inv;
	get_accel (tri_id).n_d = DOT (N, A) * k_inv;

	get_accel (tri_id).b_nu = -b [v] * inv;
	get_accel (tri_id).b_nv =  b [u] * inv;
	get_accel (tri_id).b_d =  (b [v] * A [u] - b [u] * A [v]) * inv;
	
	get_accel (tri_id).c_nu =  c [v] * inv;
	get_accel (tri_id).c_nv = -c [u] * inv;
	get_accel (tri_id).c_d  = (c [u] * A [v] - c [v] * A [u]) * inv;

}


void
triangle_post_init (unsigned int tri_id, const vector2f_t coords [3])
{
	primitive_t *tri = get_primitive (tri_id);

	/* transfrom from texture space to objectspace */
	SUB (get_primitive_ts_matrix (tri_id) [0], get_vert1 (tri), get_vert0 (tri));
	SUB (get_primitive_ts_matrix (tri_id) [1], get_vert2 (tri), get_vert0 (tri));

	vector_normalize (get_primitive_ts_matrix (tri_id) [0],
			  get_primitive_ts_matrix (tri_id) [0]);
	vector_normalize (get_primitive_ts_matrix (tri_id) [1], 
			  get_primitive_ts_matrix (tri_id) [1]);

	CROSS (get_primitive_ts_matrix (tri_id) [2], 
	       get_primitive_ts_matrix (tri_id) [0],
	       get_primitive_ts_matrix (tri_id) [1]);

	bp_matrix_transpose (get_primitive_ts_matrix (tri_id),
			     get_primitive_ts_matrix (tri_id));

	/* setup the uv matrix
	   M = [ s1 - s0,   s2 - s0,   s0]
               [ t1 - t0,   t2 - t0,   t0]
               [crap ]
	*/
	get_primitive_uv_matrix (tri_id) [0][0] = coords [1][0] - coords [0][0];
	get_primitive_uv_matrix (tri_id) [0][1] = coords [2][0] - coords [0][0];
	get_primitive_uv_matrix (tri_id) [0][2] = coords [0][0];
	get_primitive_uv_matrix (tri_id) [1][0] = coords [1][1] - coords [0][1];
	get_primitive_uv_matrix (tri_id) [1][1] = coords [2][1] - coords [0][1];
	get_primitive_uv_matrix (tri_id) [1][2] = coords [0][1];

	make_accel (tri_id);

	if (0) {
		tri->functions = &triangle_vtable;
	}
	else {
		tri->functions = &smooth_triangle_vtable;
	}

}

