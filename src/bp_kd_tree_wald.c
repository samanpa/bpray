#include "bp_kd_tree.h"
#include "bp_utils.h"
#include "bp_box.h"
#include "glib.h"
#include "bp_io.h"

#define MAX_STACK_SIZE 50
static aabb_t scene_bounding_box;

typedef struct {
	kd_tree_node_t *node;
	float t_near;
	float t_far;
} ray_segment_stack_t;

void
bp_kd_tree_find_nearest_wald (const kd_tree_t *tree, const ray_t *ray, intersect_t *isect)
{
	kd_tree_node_t *curr_node;
	float t_near, t_far; /* entry/exit signed distance */
	uint32_t *leaves, i;
	int negative_dir [3];
	vector_t inv_dir;

	ray_segment_stack_t stack [MAX_STACK_SIZE];
	int stack_end = 1;

	vector3_inverse (inv_dir, ray->dir);

	negative_dir [0] = ray->dir [0] < 0;
	negative_dir [1] = ray->dir [1] < 0;
	negative_dir [2] = ray->dir [2] < 0;

	ray_box_intersect (scene_bounding_box.lower, scene_bounding_box.upper, ray, negative_dir,
			   inv_dir, &t_near, &t_far);

	if (t_near > t_far)
		return;

	curr_node = (kd_tree_node_t *)tree->nodes;

	stack [0].node = curr_node;
	stack [0].t_far = t_far;
	stack [0].t_near = t_near;

	while (1) {
		while (!kd_tree_node_is_leaf (curr_node)) {
			float split_val = curr_node->u.split_plane;
			int axis = kd_tree_node_get_axis(curr_node);
			/* signed distance to the splitting plane */
			float t = (split_val - ray->orig [axis]) * inv_dir [axis];

			int negative = negative_dir [axis];
			/* case 1: t <= t_near <= t_far  -> cull front side */
			int backside = (t <= t_near);
			/* case 2: t_near <= t_far <= t  -> cull back side */
			int frontside = (t >= t_far);
			
			int skip = backside | frontside;

			bp_profiler_add_to_counter ('a', 1);
			
			if (!skip) {
				if (negative) {
					stack [stack_end].node = kd_tree_node_get_left_child (curr_node);
					curr_node = kd_tree_node_get_right_child (curr_node);
				}
				else {
					stack [stack_end].node = kd_tree_node_get_right_child (curr_node);
					curr_node = kd_tree_node_get_left_child (curr_node);
				}
				stack [stack_end].t_near = t;
				stack [stack_end].t_far  = t_far;
				t_far  = t;
				stack_end ++;
			} else {
				if (negative ^ frontside)
					curr_node = kd_tree_node_get_left_child (curr_node);
				else
					curr_node = kd_tree_node_get_right_child (curr_node);
			}
		}

#define MIN_CONTRIB 0.03

		isect->t = stack [stack_end].t_far  + MIN_CONTRIB;
		leaves = kd_tree_node_get_primitives (curr_node, tree->prims);
		i = curr_node->u.num_prims;

		while (i --) {
			prim_ray_intersect (leaves [i], ray, isect);
		}

		if (isect->prim_id != -1)
			return;
		
		if (stack_end == 0)
			return;
		
		stack_end --;
		curr_node = stack [stack_end].node;
		t_near = stack [stack_end].t_near;
		t_far = stack [stack_end].t_far;
	}
}


int
bp_kd_tree_has_occluder_wald (const kd_tree_t *tree, const ray_t *ray, double t_max)
{
	float t_near, t_far; /* entry/exit signed distance */
	
	kd_tree_node_t *curr_node;
	unsigned int *leaves, i;
	int negative_dir [3];
	vector_t inv_dir;
	
	ray_segment_stack_t stack [MAX_STACK_SIZE];
	int stack_end = 1;

	vector3_inverse (inv_dir, ray->dir);

	negative_dir [0] = ray->dir [0] < 0;
	negative_dir [1] = ray->dir [1] < 0;
	negative_dir [2] = ray->dir [2] < 0;

	ray_box_intersect (scene_bounding_box.lower, scene_bounding_box.upper, ray, negative_dir,
			   inv_dir, &t_near, &t_far);

	if (t_near > t_far)
		return 0;

	curr_node = (kd_tree_node_t *)tree->nodes;

	stack [0].node = curr_node;
	stack [0].t_far = t_far;
	stack [0].t_near = t_near;

	while (1) {
		while (!kd_tree_node_is_leaf (curr_node)) {
			float split_val = curr_node->u.split_plane;
			int axis = kd_tree_node_get_axis(curr_node);
			/* signed distance to the splitting plane */
			float t = (split_val - ray->orig [axis]) * inv_dir [axis];

			int negative = negative_dir [axis];
			/* case 1: t <= t_near <= t_far  -> cull front side */
			int backside = (t <= t_near);
			/* case 2: t_near <= t_far <= t  -> cull back side */
			int frontside = (t >= t_far);
			
			int skip = backside | frontside;
			
			if (!skip) {
				if (negative) {
					stack [stack_end].node = kd_tree_node_get_left_child (curr_node);
					curr_node = kd_tree_node_get_right_child (curr_node);
				}
				else {
					stack [stack_end].node = kd_tree_node_get_right_child (curr_node);
					curr_node = kd_tree_node_get_left_child (curr_node);
				}
				stack [stack_end].t_near = t;
				stack [stack_end].t_far  = t_far;
				t_far  = t;
				stack_end ++;
			} else {
				if (negative ^ frontside)
					curr_node = kd_tree_node_get_left_child (curr_node);
				else
					curr_node = kd_tree_node_get_right_child (curr_node);
			}
		}

		leaves = kd_tree_node_get_primitives (curr_node, tree->prims);
		i = curr_node->u.num_prims;
		while (i --) {
			if (prim_ray_has_intersect (leaves [i], ray, t_max))
					return 1;
		}

		if (stack_end == 0)
			return 0;
		
		stack_end --;
		curr_node = stack [stack_end].node;
		t_near = stack [stack_end].t_near;
		t_far = stack [stack_end].t_far;
	}

	return 0;
	
}

