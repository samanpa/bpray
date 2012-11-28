#include <bp_light.h>
#include <bp_memory.h>
#include <bp_config.h>
#include <bp_utils.h>
#include <bp_kd_tree.h>

void
triangle_post_init (unsigned int tri_id, vector2f_t texture_coords [3]);

void
sphere_post_init (unsigned int id);


#define in_block() (curr_object.id > -1)

static struct {
	int type;
	int id;
	int last_prim_id;
} curr_object;

static struct {
	int vertex_id;
	int id;
	int options;
} curr_prim;

static vector_t curr_color = {0, 0, 0};

static vector2f_t default_coords [3] = {
	{0, 0},
	{1, 0},
	{0, 1}
};

static vector2f_t texture_coords [3];

bp_arraylist_t *vertices;
bp_arraylist_t *normals;
bp_arraylist_t *prim_transforms;
static bp_arraylist_t *uv_coords;

static bp_arraylist_t *object_list;
static object_t *objects;
static bp_arraylist_t *_primitives_desc;
static bp_arraylist_t *_primitives_accel;

acceleration_t *primitives_accel;
primitive_t    *primitives_desc;


static inline void
primitive_new ()
{
	static int max_num_accels;
/* 	bp_arraylist_alloc (_primitives_accel); */
/*         primitives_accel = (acceleration_t *)prim_transforms->array; */

	if (curr_object.last_prim_id == max_num_accels) {
		if (max_num_accels == 0)
			max_num_accels = 16;
		else
			max_num_accels += max_num_accels / 2;

		primitives_accel = (acceleration_t *)realloc (primitives_accel, 
							      max_num_accels * sizeof (acceleration_t));
	}
	bp_arraylist_alloc (prim_transforms);

	bp_arraylist_alloc (_primitives_desc);
 	primitives_desc = (primitive_t *)_primitives_desc->array;

	curr_prim.id = curr_object.last_prim_id;
	objects [curr_object.id].primitives_end = ++ curr_object.last_prim_id;
}

void
bp_vertex (vector_t vert)
{
	int num_vertices;
	if (!in_block ())
		return;

	if (curr_prim.options == 0)
		bp_texture_coord (default_coords [curr_prim.vertex_id]);
	
	num_vertices = bp_arraylist_get_size (vertices);
	bp_arraylist_alloc1 (vertices, vert);

	switch (curr_prim.vertex_id) {
	case 0:
		primitive_new ();
		
		primitives_desc [curr_prim.id].v0 = num_vertices;
		curr_prim.vertex_id = 1;
		break;
	case 1:
		primitives_desc [curr_prim.id].v1 = num_vertices;
		curr_prim.vertex_id = 2;
		break;
	case 2:
		primitives_desc [curr_prim.id].v2 = num_vertices;
		curr_prim.vertex_id = 3;

		if (curr_object.type == BP_TRIANGLE)
			triangle_post_init (curr_prim.id, texture_coords);
		else if (curr_object.type == BP_MESH) {
			triangle_post_init (curr_prim.id, texture_coords);
			curr_prim.vertex_id = 0;
		}
		break;
	default:
		assert (0);
	}
}

void
bp_mesh (bp_mesh_t * mesh) 
{
	int i;

 	unsigned int initial_num_vertices = bp_arraylist_get_size (vertices);
	unsigned int vertices_end = mesh->num_vertices + initial_num_vertices;

 	bp_arraylist_grow (vertices, mesh->num_vertices); 
 	bp_arraylist_grow (normals, mesh->num_vertices); 

	for (i = initial_num_vertices; i < vertices_end; i ++) {
		bp_arraylist_set (vertices, i, mesh->vertices [i]);
		bp_arraylist_set (normals,  i, mesh->normals [i]);
	}

	for (i = 0; i < mesh->num_faces; i ++) {
		primitive_new ();
		primitives_desc [curr_prim.id].v0 = mesh->faces [i][0] + initial_num_vertices;
		primitives_desc [curr_prim.id].v1 = mesh->faces [i][1] + initial_num_vertices;
		primitives_desc [curr_prim.id].v2 = mesh->faces [i][2] + initial_num_vertices;
		triangle_post_init (curr_prim.id, default_coords);
	}
}

void
bp_normal (vector_t norm)
{
	if (in_block ())
		bp_arraylist_alloc1 (normals, norm);
}

void
bp_texture_coord (vector2f_t uv)
{
	bp_texture_coord2f (uv [0], uv [1]);
}

void
bp_texture_coord2f (float s, float t)
{
	
	texture_coords [curr_prim.vertex_id] [0] = s;
	texture_coords [curr_prim.vertex_id] [1] = t;
}

void
bp_color (vector_t color)
{
	if (in_block ())
		ASSIGN (curr_color, color);
}

void
bp_objects_alloc ()
{
	/* Set the id of the current object to a 'null' value initially */
	curr_object.id = -1;

	if (!object_list) {
		object_list = bp_arraylist_new (sizeof (object_t), 256);
#define NUM_PRIMS 16
		_primitives_desc = bp_arraylist_new (sizeof (primitive_t), NUM_PRIMS);
		_primitives_accel = bp_arraylist_new (sizeof (acceleration_t) * 5, NUM_PRIMS);
		prim_transforms = bp_arraylist_new (sizeof (transform_t), NUM_PRIMS);
		vertices = bp_arraylist_new (sizeof (vector_t), NUM_PRIMS * 3);
		normals  = bp_arraylist_new (sizeof (vector_t), NUM_PRIMS * 3);
		uv_coords  = bp_arraylist_new (sizeof (vector2f_t), NUM_PRIMS * 3);
#undef NUM_PRIMS
	}
}


object_t *
bp_object_new ()
{
	object_t *object;

	object = (object_t *)bp_arraylist_alloc (object_list);
	object->primitives_start = curr_object.last_prim_id;
	object->primitives_end = curr_object.last_prim_id;

	objects = (object_t *)object_list->array;

	return object;
}

BP_EXPORT void
bp_objects_init (scene_t *scene)
{
	if (!object_list) {
		bp_report (BP_LOG_WARNING, "No objects created");
		return;
	}

	scene->kd_tree_root = bp_kd_tree_init (bp_arraylist_get_size (object_list), 
					       (object_t *)object_list->array,
					       scene->settings.kd_tree_filename);
}


void
bp_objects_destroy ()
{
	bp_arraylist_destroy (object_list);
	bp_arraylist_destroy (prim_transforms);
	bp_arraylist_destroy (_primitives_desc);
	bp_arraylist_destroy (_primitives_accel);
}

unsigned int
bp_object_generate_new ()
{
	unsigned int size = bp_arraylist_get_size (object_list);
	bp_object_new ();
	return size;
}

void
bp_begin (unsigned char type)
{
	bp_begin1 (bp_arraylist_get_size (object_list), type);
}

void
bp_begin1 (unsigned int id, unsigned char type)
{
	if (in_block ()) {
		bp_report (BP_LOG_ERROR, "ERROR: %d nested bp_begin", id);
	}

	curr_prim.vertex_id = 0;

	curr_object.id = id;
	curr_object.type = type;

	objects [id].primitives_start = curr_object.last_prim_id;
	objects [id].primitives_end = curr_object.last_prim_id;

	switch (type) {
	case BP_TRIANGLE:
		break;
	case BP_SPHERE:
		primitive_new ();
		break;
	default:
		break;
	}
}

void
bp_end ()
{

	switch (curr_object.type) {
	case BP_SPHERE:
		sphere_post_init (curr_prim.id);
	default:
		break;
	}

	curr_prim.vertex_id = curr_object.id = curr_object.type = -1;
}


void
bp_material (material_t *m)
{
	int i;
	assert (in_block ());


	for (i = objects [curr_object.id].primitives_start; i < objects [curr_object.id].primitives_end; i++)
		get_material (i) = m;
}

