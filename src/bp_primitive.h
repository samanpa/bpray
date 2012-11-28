#ifndef BP_PRIMITIVE
#define BP_PRIMITIVE

#define TRIANGLE_ONLY

typedef struct intersect_t intersect_t;
typedef struct intersect4_t intersect4_t;

#include <bp_material.h>
#include <bp_math.h>
#include <bp_memory.h>
#include <mesh.h>

#define get_primitive(prim_id) (primitives_desc +  prim_id)
#define get_function(prim_id) (get_primitive (prim_id)->functions)
#define get_primitive_uv_matrix(prim_id) (get_transform (prim_id)->uv_matrix)
#define get_primitive_ts_matrix(prim_id) (get_transform (prim_id)->tangent_space_matrix)
#define get_material_from_primitive(prim_id) (get_primitive (prim_id)->material)
#define get_transform(prim_id) ((transform_t *)(bp_arraylist_get (prim_transforms, prim_id)))
#define get_material(prim_id) (get_primitive (prim_id)->material)

#ifdef TRIANGLE_ONLY
int triangle_intersect (unsigned int prim_id, const ray_t *ray, intersect_t *isect);
int triangle_packet_intersect (unsigned int prim_id, const ray4_t *ray, intersect4_t *isect, int inactive);
int triangle_has_intersect (unsigned int prim_id, const ray_t *ray, double t_max);
void smooth_triangle_get_normal (const intersect_t *i, vector_t normal);

#define prim_ray_intersect(prim_id, ray, isect) (triangle_intersect (prim_id, ray, isect))
#define prim_ray4_intersect(prim_id, ray, isect) (triangle_packet_intersect (prim_id, ray, isect))
#define prim_ray_has_intersect(prim_id, ray, t_max) (triangle_has_intersect (prim_id, ray, t_max))
#define prim_get_normal(i, n) (smooth_triangle_get_normal (i, n))
#else
#define prim_ray_intersect(prim_id, ray, isect) (get_function (prim_id)->intersect (prim_id, ray, isect))
#define prim_ray_has_intersect(prim_id, ray, t_max) (get_function (prim_id)->has_intersection (prim_id, ray, t_max))
#define prim_get_normal(i, n) (get_function (i->prim_id)->get_normal (i, n))
#endif
#define prim_get_bounds(prim_id, lbb, rtf) (get_function (prim_id)->get_bounds (prim_id, lbb, rtf))
#define prim_get_uv_coord(i, u, v) (get_function (i->prim_id)->get_uv_coord (i, u, v))
#define prim_clip_to_box(i, c0, c1, l, u) (get_function (i)->clip_to_box (i, c0, c1, l, u))


typedef struct {
        matrix_t tangent_space_matrix;
        matrix_t uv_matrix;
} transform_t;

typedef struct {
	union {
		struct triangle_accel_t {
			float n_u;
			float n_v;
			float n_d;
			int k;
			
			float b_nu;
			float b_nv;
			float b_d;
			int pad;
			
			float c_nu;
			float c_nv;
			float c_d;
			int pad2;
		} triangle;
		struct {
			vector_t center;
			float radius;
		} sphere;
	} shape;
} acceleration_t;

struct intersect_t {
	int prim_id;
	vector_t pos;
	double u;
	double v;
	double t;
} ;

struct intersect4_t {
	simd4i_t prim_id;
	simd4_vector_t pos;
	simd4_t u;
	simd4_t v;
	simd4_t t;
} ;

typedef struct {
	int (*intersect) (unsigned int prim_id, const ray_t *ray, intersect_t *);
	int (*has_intersection) (unsigned int prim_id, const ray_t *ray, double t_max);
	void (*get_normal) (const intersect_t *, vector_t n);
	void (*get_uv_coord) (const intersect_t *, double *u, double *v);
	void (*get_bounds) (unsigned int prim_id, vector_t lbb, vector_t rtf);
	int (*clip_to_box) (unsigned int prim_id, const vector_t c0, const vector_t c1, vector_t lower, vector_t upper);
} object_vtable_t;

typedef struct {
/* 	int id; */
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
/* 	unsigned int shader_id; */
	material_t * material;
	object_vtable_t *functions;
} primitive_t;

extern bp_arraylist_t *prim_transforms;
extern bp_arraylist_t *vertices;
extern bp_arraylist_t *normals;
extern acceleration_t *primitives_accel;
extern primitive_t    *primitives_desc;

enum {
	BP_TRIANGLE,
	BP_SPHERE,
	BP_MESH,
	BP_KMESH
};


void bp_vertex (vector_t);

void bp_normal (vector_t);

void bp_texture_coord (vector2f_t);

void bp_texture_coord2f (float s, float t);

void bp_color (vector_t);

void bp_mesh (bp_mesh_t *);

void bp_begin (unsigned char);

void bp_begin1 (unsigned int, unsigned char);

void bp_end ();

#endif
