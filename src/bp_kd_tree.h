#ifndef BP_KD_TREE
#define BP_KD_TREE

typedef struct kd_tree_node_t kd_tree_node_t;
typedef struct kd_tree_t kd_tree_t;

#include "bp_types.h"
#include "bp_primitive.h"
#include "bp_object.h"
#include <stdint.h>

struct kd_tree_node_t {
	uint32_t data;
	/*for a non leaf bits
	 *   0..1   axis
	 *   2..30  prims
	 *   31     is leaf
	 * for leaf bits
	 *   0..30  first child
	 *   31
	 */
	union {
		float  split_plane;
		uint32_t num_prims;
	} u;
} ;

struct kd_tree_t {
	kd_tree_node_t *nodes;
	uint32_t *prims;
	uint32_t num_prims;
	uint32_t num_nodes;
};

#define kd_tree_node_is_leaf(node) (node->data & (unsigned int) (1 << 31))
#define kd_tree_node_get_axis(node) (node->data & 0x3)
#define kd_offset(node) (node->data & 0x7FFFFFC)
#define kd_tree_node_get_left_child(node) (kd_tree_node_t*)((unsigned long)node + kd_offset (node))
#define kd_tree_node_get_right_child(node) (kd_tree_node_t*)((unsigned long)node + kd_offset (node) + 8)
#define kd_tree_node_get_primitives(node,  item_list) (unsigned int *)((unsigned long)item_list + kd_offset(node))

kd_tree_t *
bp_kd_tree_init (int num_objects, object_t *objects, const char *filename);

void
bp_kd_tree_find_nearest (const kd_tree_t *tree, const ray_t *ray, intersect_t *isect);

void
bp_kd_tree_packet_find_nearest (const kd_tree_t *tree, const ray4_t *ray, intersect4_t *isect);

int
bp_kd_tree_has_occluder (const kd_tree_t *tree, const ray_t *ray, double t_min);


typedef struct {
	vector_t lower;
	vector_t upper;
} aabb_t;

#endif
