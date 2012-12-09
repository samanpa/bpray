#include "bp_kd_tree.h"
#include "bp_utils.h"
#include "bp_box.h"
#include "glib.h"
#include "bp_io.h"

typedef struct {
	float pos;
	float cost;
	unsigned short axis;
	unsigned short side;
} split_t;

enum {
	Split_Start = 1,
	Split_End,
	Split_Planar
};

enum {
	Side_None,
	Side_LeftOnly,
	Side_RightOnly,
	Side_Both
};
	

typedef struct {
	float pos;
	int prim_id;
	short axis;
	short split_type;
} event_t;

typedef struct {
	event_t *events;
	unsigned int num_events;
} event_list_t;

static void
event_list_init (event_list_t *el, int size)
{
	el->num_events = 0;
	el->events = NEW_0 (event_t, size);
}

static void 
event_list_destroy (event_list_t *el)
{
	free (el->events);
}

static inline event_t *
event_list_get_event (event_list_t *el, unsigned int index)
{
	return el->events + index;
}

#if 0
static void 
event_list_print (event_list_t *el)
{
	int i;
	static const char axis_char [3] = {'x', 'y', 'z'};
	static const char split_char [4] = {'\0', '+', '-', '|'};

	for (i = 0; i < el->num_events; i++) {
		event_t *e = event_list_get_event (el, i);
		printf ("%f {%d,%c,%c},  ", e->pos, e->prim_id, axis_char [e->axis], split_char [e->split_type]);
	}
	printf ("\n"); fflush (stdout);
}
#endif

static void inline 
event_list_add (event_list_t *el, float pos, int prim_id, int axis, int split_type)
{
	int index = el->num_events;
	el->events [index].pos        = pos;
	el->events [index].prim_id    = prim_id;
	el->events [index].axis       = axis;
	el->events [index].split_type = split_type;
	el->num_events ++;
}

static void inline 
event_list_add_event (event_list_t *el, event_list_t *src, int idx)
{
	unsigned int index = el->num_events;
	event_t *ev = event_list_get_event (src, idx);
	el->events [index].pos        = ev->pos;
	el->events [index].prim_id    = ev->prim_id;
	el->events [index].axis       = ev->axis;
	el->events [index].split_type = ev->split_type;
	el->num_events ++;
}

static int inline
compare_events (const void *p1, const void *p2)
{
	event_t *ev1 = (event_t *)p1,
		*ev2 = (event_t *)p2;

	float f = ev1->pos - ev2->pos;

	if (f < -0)
		return -1;
	else if (f > 0)
		return 1;
	else if (ev1->axis < ev2->axis)
		return -1;
	else if (ev1->axis > ev2->axis)
		return 1;
	else if (ev1->split_type < ev2->split_type)
		return -1;
	else
		return ev1->split_type > ev2->split_type;
}


static void inline
event_list_sort (event_list_t *el) {
	qsort (el->events, el->num_events, sizeof (event_t), compare_events);
}


static inline void
kd_tree_init (kd_tree_t *tree)
{
	tree->num_nodes = 4;
  	tree->nodes = NEW_0 (kd_tree_node_t, tree->num_nodes);

	tree->num_prims = 0;
	tree->prims = NULL;
}

static void
kd_tree_load (const char *filename, kd_tree_t *tree)
{
	FILE *file = fopen (filename, "rb");
	int i;

	bp_io_readint32 (file, &tree->num_prims);
	bp_io_readint32 (file, &tree->num_nodes);

	tree->prims = NEW_0 (uint32_t, tree->num_prims);
	tree->nodes = NEW_0 (kd_tree_node_t, tree->num_nodes);
	
	for (i = 0; i < tree->num_prims; i++)
		bp_io_readint32 (file, &tree->prims [i]);

	for (i = 0; i < tree->num_nodes; i++) {
		bp_io_readint32 (file, &tree->nodes [i].u.num_prims);
		bp_io_readint32 (file, &tree->nodes [i].data);		
	}
}

static void
kd_tree_save (const char *filename, const kd_tree_t *tree)
{
	FILE *file = fopen (filename, "wb");
	int i;

	bp_io_writeint32 (file, tree->num_prims);
	bp_io_writeint32 (file, tree->num_nodes);

	for (i = 0; i < tree->num_prims; i++)
		bp_io_writeint32 (file, tree->prims [i]);

	for (i = 0; i < tree->num_nodes; i++) {
		/* Since floats and uint32_t we can safely either member of the union*/
		bp_io_writeint32 (file, tree->nodes [i].u.num_prims);
		bp_io_writeint32 (file, tree->nodes [i].data);
	}
	
	fclose (file);
}

static void
kd_tree_node_print (const kd_tree_node_t *node, uint32_t *items, const int ident)
{
	int i;
	static char axis_char [3] = {'x', 'y', 'z'};

	for (i = 0; i < ident; i++)
		fprintf (stderr, "\t");
	bp_report (BP_LOG_DEBUG, "Node :  %x", (long)node);
	if (node == NULL)
		return;
	if (kd_tree_node_is_leaf (node)) {
		unsigned int * leaves = kd_tree_node_get_primitives (node, items);
		int j = node->u.num_prims;
		
		while (j --) {
			for (i = 0 ; i <= ident; i++)
				fprintf (stderr, "\t");
			bp_report (BP_LOG_DEBUG, "Leaf : %d", (long) leaves [j]);
		}
	} else {
		 for (i = 0; i < ident; i++)
			 fprintf (stderr, "\t");
		 bp_report (BP_LOG_DEBUG, "        %c", axis_char [kd_tree_node_get_axis (node)]);
		 for (i = 0; i < ident; i++)
			 fprintf (stderr, "\t");
		 bp_report (BP_LOG_DEBUG, "        %f", node->u.split_plane);
		 
		 kd_tree_node_print (kd_tree_node_get_left_child (node), items, ident + 1);
		 kd_tree_node_print (kd_tree_node_get_right_child (node), items, ident + 1);
	}
}


static inline void
kd_tree_grow (kd_tree_t *tree, int pos)
{
	if (tree->num_nodes < pos + 1) {
		tree->num_nodes = pos + 1;
		tree->nodes = (kd_tree_node_t *)bp_alloc3 (tree->nodes, tree->num_nodes * sizeof (kd_tree_node_t));
	}
}

static inline void
leaf_node_new (kd_tree_t *tree, int pos, int num_prims, int *prims)
{
	static unsigned int curr_prim_index = 0;
	kd_tree_node_t *leaf ;
 	int i;
	uint32_t *dest_prim;

	kd_tree_grow (tree, pos);

	if (tree->num_prims < curr_prim_index + num_prims) {
		tree->num_prims += num_prims;
		tree->prims = (uint32_t *)bp_alloc3 (tree->prims, tree->num_prims * sizeof (uint32_t));
	}

	dest_prim = tree->prims + curr_prim_index;
	leaf = tree->nodes + pos;
   	leaf->u.num_prims = num_prims;
	leaf->data = ((uint32_t)(1 << 31) | (curr_prim_index << 2));

	curr_prim_index += num_prims;

	for (i = 0; i < num_prims; i++) {
		dest_prim [i] = prims [i];
	}
}

static inline void
internal_node_new (kd_tree_t *tree, int pos, int left_pos, float split, int axis)
{
	kd_tree_node_t *internal_node;
	kd_tree_node_t *left_node, *right_node;

	kd_tree_grow (tree, pos);

	internal_node = tree->nodes + pos;
	left_node     = tree->nodes + left_pos;
	right_node    = tree->nodes + left_pos + 1;
	
	internal_node->u.split_plane = split;
  	internal_node->data = (uint32_t)(((left_node - internal_node) << 3) | axis);
}


static inline float
get_area (aabb_t *voxel)
{
	float w, l, b;
	float area;
	w = voxel->upper [0] - voxel->lower [0];
	l = voxel->upper [1] - voxel->lower [1];
	b = voxel->upper [2] - voxel->lower [2];
	
	/* We don't multiply by 2 cos we only really use ratios of areas*/
	area = w * l + w * b + l * b;

	if (area < 0) {
		vector3_print (stdout, voxel->lower);
		vector3_print (stdout, voxel->upper);
	}
	assert (area >= 0);
	return area;
}

const int intersect_cost = 80;
static const int next_axes [3] = {1, 2, 0};
static const int prev_axes [3] = {2, 0, 1};

static inline void
split(aabb_t *voxel, float split, int axis, aabb_t *left, aabb_t *right)
{
	ASSIGN (left->lower, voxel->lower);
	ASSIGN (right->upper, voxel->upper);

	right->lower [next_axes [axis]] = voxel->lower [next_axes [axis]];
	right->lower [prev_axes [axis]] = voxel->lower [prev_axes [axis]];

	left->upper [next_axes [axis]] = voxel->upper [next_axes [axis]]; 
	left->upper [prev_axes [axis]] = voxel->upper [prev_axes [axis]]; 

	left->upper [axis] = right->lower [axis] = split;
}

static inline float
get_sah_cost (float prob_left, unsigned int num_left, float prob_right, unsigned int num_right)
{
	const int TRAVERSAL_COST = 1;
	float cost = TRAVERSAL_COST + (intersect_cost *
				       (prob_left * num_left + prob_right * num_right));
	
	if (num_right == 0 || num_left == 0)
		return 0.8 * cost;
	else
		return cost;
}

static void
find_best_plane (int num_objects, aabb_t *voxel, int num_prims, int *prims, event_list_t *events, split_t *best_split)
{
	unsigned int nleft [3], nright [3];
	unsigned int i;
	float inv_voxel_area = 1/get_area (voxel);
	unsigned int num_events = events->num_events;

 	best_split->cost  = intersect_cost * num_prims;
	best_split->axis = No_axis;

	for (i = 0; i < 3; i ++) {
		nleft [i] = 0;
		nright [i] = num_prims;
	}

	for (i = 0; i < num_events;) {
		unsigned int num_starting = 0, num_ending = 0, num_lying = 0;
		float pos = event_list_get_event (events, i)->pos;
		int axis = event_list_get_event (events, i)->axis;

		/* For the current position in the current plane get num prims startinng, ending and planar */
		while (i < num_events && axis == event_list_get_event (events, i)->axis
		       && event_list_get_event (events, i)->split_type == Split_End
		       && pos == event_list_get_event (events, i)->pos) {
			num_ending ++;
			i ++;
		}
		while (i < num_events && axis == event_list_get_event (events, i)->axis 
		       && event_list_get_event (events, i)->split_type == Split_Start
		       && pos == event_list_get_event (events, i)->pos) {
			num_starting ++;
			i ++;
		}
		while (i < num_events && axis == event_list_get_event (events, i)->axis
		       && event_list_get_event (events, i)->split_type == Split_Planar
		       && pos == event_list_get_event (events, i)->pos) {
			num_lying ++;
			i ++;
		}
		
		nright [axis] -= num_lying;
		nright [axis] -= num_ending;

		/* We dont want any flat voxels cos we don't traverse them correctly
		 *
		 * If we don't do this we have to deal with the case when area of the new voxel is 0,
		 * This can happen if we have several equivalent triangles lying
		 * together in a flat voxel (in the first split position the left half voxel is a line with 0 area)
		 */
		if (pos != voxel->lower [axis] && pos != voxel->upper [axis]) {
			float lcost, rcost;
			float lprob, rprob;
			aabb_t lvoxel, rvoxel; 
	
			if (!(voxel->lower [axis] <=  pos && voxel->upper [axis] >= pos)) {
				printf ("%.16f %d %d %d\n", pos, axis, voxel->lower [axis] <=  pos, voxel->upper [axis] >= pos);
				printf ("%d %.16f %.16f %.16f\n", i,
					(voxel->lower [axis] - pos) * 1000, 
					(pos - pos)* 1000, 
					(voxel->upper [axis] - pos) * 1000);
				vector3_print (stdout, voxel->lower);
				vector3_print (stdout, voxel->upper);
			}

			split (voxel, pos, axis, &lvoxel, &rvoxel);

			lprob = get_area (&lvoxel) * inv_voxel_area;
			rprob = get_area (&rvoxel) * inv_voxel_area;

			lcost = get_sah_cost (lprob, nleft [axis] + num_lying, rprob, nright [axis]);
			rcost = get_sah_cost (lprob, nleft [axis], rprob, nright [axis] + num_lying);
			
			if (lcost < rcost) {
				if (lcost < best_split->cost) {
					best_split->cost = lcost;
					best_split->pos  = pos;
					best_split->axis = axis;
					best_split->side = Side_LeftOnly;
				}
			} else {
				if (rcost < best_split->cost) {
					best_split->cost = rcost;
					best_split->pos  = pos;
					best_split->axis = axis;
					best_split->side = Side_RightOnly;
				}
			}
		}

		nleft [axis] += num_lying;
		nleft [axis] += num_starting;

		assert (num_lying + nright [axis] + nleft [axis] >= num_prims);
			

	}
}

static aabb_t scene_bounding_box;
static char *sides;

static unsigned int inline
classify (unsigned int num_prims, int *prims, event_list_t *events, float pos, int side, int axis)
{
	unsigned int i;
	int count = num_prims;
	unsigned int num_events = events->num_events;
	

	for (i = 0; i < num_prims; i++)
		sides [prims [i]] = Side_Both;

	for (i = 0; i < num_events; i++) {
		event_t *event = event_list_get_event (events, i);
		if (event->split_type == Split_End && event->axis == axis && event->pos < pos) {
			sides [event->prim_id] = Side_LeftOnly;
			count --;
		}
		else if (event->split_type == Split_Start && event->axis == axis && event->pos > pos) {
			sides [event->prim_id] = Side_RightOnly;
			count --;
		}
		else if (event->split_type == Split_Planar && event->axis == axis) {
			if (event->pos < pos || (side == Side_LeftOnly && event->pos == pos)) {
				sides [event->prim_id] = Side_LeftOnly;
				count --;
			}
			if (event->pos > pos || (side == Side_RightOnly && event->pos == pos)) {
				sides [event->prim_id] = Side_RightOnly;
				count --;
			}
		}
	}


	assert (count >= 0);
	return count;
}


static int
clip (int prim_id, aabb_t *voxel, event_list_t *events, int split_axis)
{
	vector_t lower, upper;
	vector_t bl, bu;
	int axis;

	int is_clipped = prim_clip_to_box (prim_id, voxel->lower, voxel->upper, lower, upper);

	if (!is_clipped)
		return 0;
	
	if (lower [split_axis] == upper [split_axis]) { 
		/* if clipping gave us a flat cell, we only add
		   the primitive if area (intersect (primitive, voxel)) > 0 i.e.
		   primitive is also flat on that axis
		*/
		prim_get_bounds (prim_id, bl, bu);
		if ((bu [split_axis] - bl [split_axis]) > SMALL_VAL) {
			return 0;
		}
	}

	for (axis = 0; axis < 3; axis ++) {
		if (lower [axis] == upper [axis]) {
			event_list_add (events, lower [axis], prim_id, axis, Split_Planar);
		}
		else {
			event_list_add (events, lower [axis], prim_id, axis, Split_Start);
			event_list_add (events, upper [axis], prim_id, axis, Split_End);
		}
	}
	return 1;
}

static void
merge (event_list_t *merged, event_list_t *non_overlaps, event_list_t *overlaps)
{
	unsigned int i, j;

	i = j = 0;
	for (; i < non_overlaps->num_events && j < overlaps->num_events;) {
		switch (compare_events (event_list_get_event (non_overlaps, i),
					event_list_get_event (overlaps, j))) {
		case 0: /* equal */
			event_list_add_event (merged, non_overlaps, i);
			i ++;
			/* intentionally fall through */
		case 1: /* overlaps less */
			event_list_add_event (merged, overlaps, j);
			j ++;
			break;
		case -1: /* non overlaps less */
			event_list_add_event (merged, non_overlaps, i);
			i ++;
			break;
		}
	}
	/* Merge any overapping events that were not merged */
	for (; j < overlaps->num_events; j++)
		event_list_add_event (merged, overlaps, j);

	/* Merge any non overapping events that were not merged */
	for (; i < non_overlaps->num_events; i++)
		event_list_add_event (merged, non_overlaps, i);
}

static const int MIN_TRIANGLES = 2;
static int MAX_DEPTH;

static void
kd_tree_construct (kd_tree_t *tree, int tree_pos, aabb_t voxel, int num_prims, int *prims,
		   event_list_t *events, int depth)
{
	static int curr_node_index;
	unsigned int num_left_prims, num_right_prims;
	unsigned int i, num_overlapping_prims;
	aabb_t lvoxel, rvoxel;
	int *left_prims, *right_prims;
	split_t best_split;
	int tree_index;
	event_list_t bleft_events, bright_events, oleft_events, oright_events, left_events, right_events;

	if (num_prims <= MIN_TRIANGLES) {
		leaf_node_new (tree, tree_pos, num_prims, prims);
		return;
	}
	else if (depth >= MAX_DEPTH) {
		leaf_node_new (tree, tree_pos, num_prims, prims);
		return;
	}
	
	find_best_plane (num_prims, &voxel, num_prims, prims, events, &best_split);
   	/* printf ("\tBest split %f  %d %d \n",best_split.pos, best_split.axis, best_split.side); */

	if (best_split.axis == No_axis) {
		leaf_node_new (tree, tree_pos, num_prims, prims);
		return;
	}

	/* classify the side primitives fall on as either left only, right only or both */
	num_overlapping_prims = classify (num_prims, prims, events, best_split.pos, best_split.side, best_split.axis);

	/* split the event list into left_events and right events */
	split (&voxel, best_split.pos, best_split.axis, &lvoxel, &rvoxel);

	event_list_init (&oleft_events, events->num_events - num_overlapping_prims * 3);
	event_list_init (&oright_events, events->num_events - num_overlapping_prims * 3);
	
	for (i = 0; i < events->num_events; i++) {
		int prim_id = event_list_get_event (events, i)->prim_id;
		int side = sides [prim_id];

		switch (side) {
		case Side_LeftOnly:
			event_list_add_event (&oleft_events, events, i);
			break;
		case Side_RightOnly:
			event_list_add_event (&oright_events, events, i);
			break;
		}
	}

	event_list_init (&bleft_events, num_overlapping_prims * 6);
	event_list_init (&bright_events, num_overlapping_prims * 6);

	for (i = 0; i < num_prims; i ++) {
		if (sides [prims [i]] == Side_Both) {
			if (!clip (prims [i], &lvoxel, &bleft_events, best_split.axis))
				sides [prims [i]] = Side_RightOnly;
			if (!clip (prims [i], &rvoxel, &bright_events, best_split.axis))
				sides [prims [i]] &= ~Side_RightOnly;
		}
	}

	event_list_sort (&bleft_events);
	event_list_sort (&bright_events);

	event_list_init (&left_events, oleft_events.num_events + bleft_events.num_events);
	merge (&left_events, &oleft_events, &bleft_events);

	event_list_destroy (&bleft_events);
	event_list_destroy (&oleft_events);

	event_list_init (&right_events, oright_events.num_events + bright_events.num_events);
	merge (&right_events, &oright_events, &bright_events);

	event_list_destroy (&bright_events);
	event_list_destroy (&oright_events);


	/* split the primitive list into two */
	num_left_prims = num_right_prims = 0;
	left_prims = NEW_0 (int, num_prims);
	right_prims = NEW_0 (int, num_prims);
	for (i = 0; i < num_prims; i++) {
		int prim_id = prims [i];
		if (sides [prim_id] == Side_LeftOnly)
			left_prims [num_left_prims ++] = prim_id;
		else if (sides [prim_id] == Side_RightOnly)
			right_prims [num_right_prims ++] = prim_id;
		else if (sides [prim_id] == Side_Both) {
			left_prims [num_left_prims ++] = prim_id;
			right_prims [num_right_prims ++] = prim_id;
		}
	}

/*
	if (num_prims == num_left_prims) {
		static int nkk;
		nkk++;
		printf ("left %d %d %d %d\n", nkk, num_prims, num_right_prims, depth);
		vector3_print (stdout, lvoxel.upper);
		vector3_print (stdout, voxel.upper);
	}

	if (num_prims == num_right_prims) {
		static int nll;
		nll++;
		printf ("right %d %d %d %d\n", nll, num_prims, num_left_prims, depth);
		vector3_print (stdout, rvoxel.lower);
		vector3_print (stdout, voxel.lower);
	}
*/
	tree_index = curr_node_index;
	curr_node_index += 2;
	kd_tree_construct (tree, tree_index + 1, lvoxel, num_left_prims, left_prims, &left_events, depth + 1);
 	free (left_prims);
	event_list_destroy (&left_events);

	kd_tree_construct (tree, tree_index + 2, rvoxel, num_right_prims, right_prims, &right_events, depth + 1);
	
	internal_node_new (tree, tree_pos, tree_index + 1, best_split.pos, best_split.axis);

 	free (right_prims);
	event_list_destroy (&right_events);
}


typedef struct {
	kd_tree_node_t *node;
	double t;
	vector_t pb;
	int prev;
} stack_elem_t;
typedef struct {
	kd_tree_node_t *node;
	simd4_t near4;
	simd4_t far4;
} simd4_stack_elem_t;
	

#define MAX_STACK_SIZE 50

void
bp_kd_tree_find_nearest (const kd_tree_t *tree, const ray_t *ray, intersect_t *isect)
{
	kd_tree_node_t *curr_node;
	float t_near, t_far; /* entry/exit signed distance */
	uint32_t *leaves, i;
	int negative_dir [3];
	vector_t inv_dir;

	kd_tree_node_t *far_child;
	stack_elem_t stack [MAX_STACK_SIZE];
	unsigned int entry_pt, exit_pt;

	vector3_inverse (inv_dir, ray->dir);

	negative_dir [0] = ray->dir [0] < 0;
	negative_dir [1] = ray->dir [1] < 0;
	negative_dir [2] = ray->dir [2] < 0;

	ray_box_intersect (scene_bounding_box.lower, scene_bounding_box.upper, ray, negative_dir,
			   inv_dir, &t_near, &t_far);

	if (t_near > t_far)
		return;

	curr_node = (kd_tree_node_t *)tree->nodes;

	entry_pt    = 0;
	stack [0].t = t_near;
	if (t_near >= 0.0)
		SCALE_ADD3 (stack [0].pb, t_near, ray->dir, ray->orig);
	else
		ASSIGN (stack [0].pb, ray->orig);

	
	exit_pt        = 1;
	stack [1].t    = t_far;
	stack [1].node = NULL;
	SCALE_ADD3 (stack [1].pb, t_far, ray->dir, ray->orig);
	

#define DO_QUICK_CHILD
	while (curr_node) {
		while (!kd_tree_node_is_leaf (curr_node)) {
			float split_val = curr_node->u.split_plane;
			int axis = kd_tree_node_get_axis(curr_node);
			/* signed distance to the splitting plane */
			float t = (split_val - ray->orig [axis]) * inv_dir [axis];
#ifdef DO_QUICK_CHILD
			kd_tree_node_t *left = kd_tree_node_get_left_child (curr_node);
#endif
			int next_axis, prev_axis;

			int tmp;
			bp_profiler_add_to_counter ('a', 1);
			
			if (stack [entry_pt].pb [axis] <= split_val) {
				if (stack [exit_pt].pb [axis] <= split_val) {
					/* case N1, N2, N3, P5, Z2 and Z3 */
#ifdef DO_QUICK_CHILD
					curr_node = left;
#else
					curr_node = kd_tree_node_get_left_child (curr_node);
#endif
					continue;
				}
				if (stack [exit_pt].pb [axis] == split_val) {
					/* case Z2 */
#ifdef DO_QUICK_CHILD
					curr_node = left + 1;
#else
					curr_node = kd_tree_node_get_right_child (curr_node);
#endif
					continue;
				}
				/* case N4 */
				far_child = kd_tree_node_get_right_child (curr_node);
				curr_node = kd_tree_node_get_left_child (curr_node);
			}
			else {
				if (stack [exit_pt].pb [axis] > split_val) {
					/* case P1, P2, P3 and P5 */
#ifdef DO_QUICK_CHILD
					curr_node = left + 1;
#else
 					curr_node = kd_tree_node_get_right_child (curr_node);
#endif
					continue;
				}
				/* case P4 */
#ifdef DO_QUICK_CHILD
				far_child = left;
				curr_node = left + 1;
#else
				far_child = kd_tree_node_get_left_child (curr_node);
				curr_node = kd_tree_node_get_right_child (curr_node);
#endif
			}


			tmp = exit_pt;
			++exit_pt;

			if (exit_pt == entry_pt)
				++exit_pt;

			assert (exit_pt < MAX_STACK_SIZE);

			stack [exit_pt].prev = tmp;
			stack [exit_pt].t    = t;
			stack [exit_pt].node = far_child;

			next_axis = next_axes [axis];
			prev_axis = prev_axes [axis];

			stack [exit_pt].pb [axis]      = split_val;
			stack [exit_pt].pb [next_axis] = ray->orig [next_axis] + t * ray->dir [next_axis];
			stack [exit_pt].pb [prev_axis] = ray->orig [prev_axis] + t * ray->dir [prev_axis];

		}

		isect->t = stack [exit_pt].t  + 0.001;
		leaves = kd_tree_node_get_primitives (curr_node, tree->prims);
		i = curr_node->u.num_prims;

		while (i --) {
			prim_ray_intersect (leaves [i], ray, isect);
		}

		if (isect->prim_id != -1)
			return;
		
		entry_pt  = exit_pt;
		curr_node = stack [exit_pt].node;
		exit_pt   = stack [entry_pt].prev;
	}
	
	return;
}

#include <math_sse.h>

void
bp_kd_tree_packet_find_nearest (const kd_tree_t *tree, const ray4_t *ray4, intersect4_t *isect4)
{
	simd4_t near4 = _mm_setzero_ps ();
	simd4_t far4  = _mm_set1_ps (HUGE_VAL);
	int activeMask;
	unsigned int ray_dir [3][2];
	simd4_vector_t inv_dir;
	simd4_t clip_min, clip_max, cmp;
	unsigned int dir_x, dir_y, dir_z;
	int stack_index = 0;
	int termination = 0;
	kd_tree_node_t *curr_node;
	simd4_stack_elem_t stack [MAX_STACK_SIZE];
	
	simd4_vector_INVERSE (inv_dir, ray4->dir);

	/* clipping in the X direction */
	clip_min = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.lower [X_axis]), 
					    ray4->orig [X_axis]),
				inv_dir [X_axis]);

	clip_max = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.upper [X_axis]),
					    ray4->orig [X_axis]),
				inv_dir [X_axis]);

	cmp = _mm_cmpgt_ps (inv_dir [X_axis], _mm_setzero_ps ());
	near4 = _mm_max_ps (near4, simd4_Update (cmp, clip_min, clip_max));
	far4  = _mm_min_ps (far4,  simd4_Update (cmp, clip_max, clip_min));

	/* clipping in the Y direction */
	clip_min = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.lower [Y_axis]), 
					    ray4->orig [Y_axis]),
				inv_dir [Y_axis]);

	clip_max = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.upper [Y_axis]),
					    ray4->orig [Y_axis]),
				inv_dir [Y_axis]);

	cmp = _mm_cmpgt_ps (inv_dir [Y_axis], _mm_setzero_ps ());
	near4 = _mm_max_ps (near4, simd4_Update (cmp, clip_min, clip_max));
	far4  = _mm_min_ps (far4,  simd4_Update (cmp, clip_max, clip_min));

	/* clipping in the Z direction */
	clip_min = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.lower [Z_axis]), 
					    ray4->orig [Z_axis]),
				inv_dir [Z_axis]);

	clip_max = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (scene_bounding_box.upper [Z_axis]),
					    ray4->orig [Z_axis]),
				inv_dir [Z_axis]);

	cmp = _mm_cmpgt_ps (inv_dir [Z_axis], _mm_setzero_ps ());
	near4 = _mm_max_ps (near4, simd4_Update (cmp, clip_min, clip_max));
	far4  = _mm_min_ps (far4,  simd4_Update (cmp, clip_max, clip_min));

	/* check if we completely miss the scene box */
	if (_mm_movemask_ps (_mm_cmpgt_ps (near4, far4)) == 0xF)
		return;

	/* To achieve a common traversal order the direction signs should be the same
	   for all directions.
	 */
	dir_x = _mm_movemask_ps (inv_dir [X_axis]);
	dir_y = _mm_movemask_ps (inv_dir [Y_axis]);
	dir_z = _mm_movemask_ps (inv_dir [Z_axis]);

	if (!(((dir_x == 0xF) | (dir_x == 0))
	      & ((dir_y == 0xF) | (dir_y == 0))
	      & ((dir_z == 0xF) | (dir_z == 0)))) {
		ray_t ray;
		intersect_t isect [4];

		/* use single ray traversal */
		simd4_vector_extract0 (ray.orig, ray4->orig);

		simd4_vector_extract0 (ray.dir, ray4->dir);
		isect[0].prim_id = simd4i_get0 (isect4->prim_id);
		bp_kd_tree_find_nearest (tree, &ray, isect);
		
		simd4_vector_extract1 (ray.dir, ray4->dir);
		isect[1].prim_id = simd4i_get1 (isect4->prim_id);
		bp_kd_tree_find_nearest (tree, &ray, isect + 1);
		
		simd4_vector_extract2 (ray.dir, ray4->dir);
		isect[2].prim_id = simd4i_get2 (isect4->prim_id);
		bp_kd_tree_find_nearest (tree, &ray, isect + 2);
		
		simd4_vector_extract3 (ray.dir, ray4->dir);
		isect[3].prim_id = simd4i_get3 (isect4->prim_id);
		bp_kd_tree_find_nearest (tree, &ray, isect + 3);
		
		isect4->prim_id = _mm_setr_epi32 (isect [0].prim_id, isect [1].prim_id, isect [2].prim_id, isect [3].prim_id);
		isect4->u = _mm_setr_ps (isect [0].u, isect [1].u, isect [2].u, isect [3].u);
		isect4->v = _mm_setr_ps (isect [0].v, isect [1].v, isect [2].v, isect [3].v);
		isect4->t = _mm_setr_ps (isect [0].t, isect [1].t, isect [2].t, isect [3].t);

		return;
	}
	/* precompute the traversal order (front/back child) for the bundle. 
	   The front and back children are found as ray_dir [axis] [0] and ray_dir [axis] [1] repectively */
	ray_dir [X_axis][0] = dir_x & 1;
	ray_dir [Y_axis][0] = dir_y & 1;
	ray_dir [Z_axis][0] = dir_z & 1;

	ray_dir [X_axis][1] = (dir_x & 1) ^ 1;
	ray_dir [Y_axis][1] = (dir_y & 1) ^ 1;
	ray_dir [Z_axis][1] = (dir_z & 1) ^ 1;

	curr_node = (kd_tree_node_t *)tree->nodes;
	activeMask = _mm_movemask_ps (_mm_cmple_ps (near4, far4));

	isect4->t = far4;
	do {
		unsigned int i;
		uint32_t *leaves;
		while (!kd_tree_node_is_leaf (curr_node)) {
			const int axis = kd_tree_node_get_axis(curr_node);
			/* signed distance to the splitting plane */
			const simd4_t d = _mm_mul_ps (_mm_sub_ps (_mm_set1_ps (curr_node->u.split_plane),
								  ray4->orig [axis]),
						      inv_dir [axis]);
			const kd_tree_node_t *left  = kd_tree_node_get_left_child (curr_node);
			const kd_tree_node_t *front = left + ray_dir [axis][0];
			const kd_tree_node_t *back  = left + ray_dir [axis][1];

			curr_node = (kd_tree_node_t *)back; /*  only back child traversal ? */
			if (!(_mm_movemask_ps(_mm_cmple_ps(near4, d)) & activeMask))
				continue;
			
			curr_node = (kd_tree_node_t *)front; /* only front child traversal ? */
			if (!(_mm_movemask_ps(_mm_cmpge_ps(far4,d )) & activeMask))
				continue;

			stack [stack_index].node  = (kd_tree_node_t *)back; /* push values on stack */
			stack [stack_index].near4 = _mm_max_ps(near4, d);
			stack [stack_index].far4  = far4;

			stack_index++;
			far4 = _mm_min_ps (far4,d); /*  update far values and the active mask */
			activeMask &= _mm_movemask_ps (_mm_cmple_ps (near4, far4));
		}

		leaves = kd_tree_node_get_primitives (curr_node, tree->prims);
		i = curr_node->u.num_prims;


		if (i) {
			while (i --) {
				unsigned int leaf = leaves [i];

				termination |= triangle_packet_intersect (leaf, ray4, isect4, activeMask)
					& _mm_movemask_ps(_mm_cmpge_ps(far4, isect4->t));
			}
			if (termination == 0xF)
				return;
		}

		if (stack_index == 0)
			return ;
		
		stack_index --;
		curr_node = stack [stack_index].node;
		near4 = stack [stack_index].near4;
		far4 = stack [stack_index].far4;


		activeMask = ~termination & _mm_movemask_ps (_mm_cmple_ps (near4,far4));
	} while (1);

}

int
bp_kd_tree_has_occluder (const kd_tree_t *tree, const ray_t *ray, double t_max)
{
	float t_near, t_far; /* entry/exit signed distance */
	
	kd_tree_node_t *curr_node;
	unsigned int *leaves, i;
	int negative_dir [3];
	vector_t inv_dir;
	
	kd_tree_node_t *far_child;
	stack_elem_t stack [MAX_STACK_SIZE];
	unsigned int entry_pt, exit_pt;

	vector3_inverse (inv_dir, ray->dir);

	negative_dir [0] = ray->dir [0] < 0;
	negative_dir [1] = ray->dir [1] < 0;
	negative_dir [2] = ray->dir [2] < 0;

	ray_box_intersect (scene_bounding_box.lower, scene_bounding_box.upper, ray, negative_dir,
			   inv_dir, &t_near, &t_far);

	if (t_near > t_far)
		return 0;

	curr_node = (kd_tree_node_t *)tree->nodes;

	entry_pt    = 0;
	stack [0].t = t_near;
	

	if (t_near >= 0.0)
		SCALE_ADD3 (stack [0].pb, t_near, ray->dir, ray->orig);
	else
		ASSIGN (stack [0].pb, ray->orig);

	
	exit_pt        = 1;
	stack [1].t    = t_far;
	stack [1].node = NULL;
	SCALE_ADD3 (stack [1].pb, t_far, ray->dir, ray->orig);

	while (curr_node) {
		while (!kd_tree_node_is_leaf (curr_node)) {
			int next_axis, prev_axis;
			float split_val = curr_node->u.split_plane;
			int axis = kd_tree_node_get_axis (curr_node);
#ifdef DO_QUICK_CHILD
 			kd_tree_node_t *left = kd_tree_node_get_left_child (curr_node);
#endif
			int tmp;
			float t;

			if (stack [entry_pt].pb [axis] <= split_val) {
				if (stack [exit_pt].pb [axis] <= split_val) {
#ifdef DO_QUICK_CHILD
					curr_node = left;
#else
					curr_node = kd_tree_node_get_left_child (curr_node);
#endif
					continue;
				}
				if (stack [exit_pt].pb [axis] == split_val) {
#ifdef DO_QUICK_CHILD
					curr_node = left + 1;
#else
					curr_node = kd_tree_node_get_right_child (curr_node);
#endif
					continue;
				}
#ifdef DO_QUICK_CHILD
				far_child = left + 1;
				curr_node = left;
#else
				far_child = kd_tree_node_get_right_child (curr_node);
				curr_node = kd_tree_node_get_left_child (curr_node);
#endif
			}
			else {
				if (stack [exit_pt].pb [axis] > split_val) {
#ifdef DO_QUICK_CHILD
					curr_node = left + 1;
#else
					curr_node = kd_tree_node_get_right_child (curr_node);
#endif
					continue;
				}
#ifdef DO_QUICK_CHILD
				far_child = left;
				curr_node = left + 1;
#else
				far_child = kd_tree_node_get_left_child (curr_node);
				curr_node = kd_tree_node_get_right_child (curr_node);
#endif
			}

			t = (split_val - ray->orig [axis]) / ray->dir [axis];

			tmp = exit_pt;
			++exit_pt;

			if (exit_pt == entry_pt)
				++exit_pt;
			
			stack [exit_pt].prev = tmp;
			stack [exit_pt].t    = t;
			stack [exit_pt].node = far_child;

			next_axis = next_axes [axis];
			prev_axis = prev_axes [axis];

			stack [exit_pt].pb [axis]      = split_val;
			stack [exit_pt].pb [next_axis] = ray->orig [next_axis] + t * ray->dir [next_axis];
			stack [exit_pt].pb [prev_axis] = ray->orig [prev_axis] + t * ray->dir [prev_axis];
		}

		i = curr_node->u.num_prims;
		leaves = kd_tree_node_get_primitives (curr_node, tree->prims);

		while (i--) {
			if (prim_ray_has_intersect (leaves [i], ray, t_max)) 
			    return 1;
		}
		entry_pt  = exit_pt;
		curr_node = stack [exit_pt].node;
		exit_pt   = stack [entry_pt].prev;
	}

	return 0;
	
}



kd_tree_t *
bp_kd_tree_init (int num_objects, object_t *objects, const char *kd_filename)
{
	unsigned int i;
	int *prims = NULL;
	unsigned int num_prims = 0;
	kd_tree_t *root;
	event_list_t events;
	static int load_from_file = 0;

	VSET_ALL (scene_bounding_box.lower, HUGE_VAL);
	VSET_ALL (scene_bounding_box.upper, -HUGE_VAL);


	if (kd_filename)
		load_from_file = !access (kd_filename, F_OK);

	/* find the number of primitives */
	for (i = 0; i < num_objects; i++)
		num_prims += objects [i].primitives_end - objects [i].primitives_start;

	/* optionally create data that is needed to build initial kd_tree */
	if (!load_from_file) {
		event_list_init (&events, num_prims * 6);
		prims = NEW_0 (int, num_prims);
		sides = NEW_0 (char, num_prims);

		/* FIXME:
		     read other Havran kd-tree paper (improving kd-trees for ray shooting)
		     to find a good setting for this. Or possibly using an Automatic 
		     Termination Criteria if we are computing the kd-tree structure ahead
		     of time.
		     
		   for now we use value suggested in "A comparison of acceleration structures
		      for GPU assisted raytracing"

		   The value as at now is meaningless. The value from the paper is
		      8 + 1.3 * log (num_prims)
		*/
		MAX_DEPTH = 8 + 1.3 * log (num_prims);
	}


	for (i = 0; i < num_objects; i++) {
		vector_t bl, bu;
		int j;

		for (j = objects [i].primitives_start; j < objects [i].primitives_end; j++) {
			prim_get_bounds (j, bl, bu);
			
			scene_bounding_box.lower [0] = MIN (bl [0], scene_bounding_box.lower [0]);
			scene_bounding_box.lower [1] = MIN (bl [1], scene_bounding_box.lower [1]);
			scene_bounding_box.lower [2] = MIN (bl [2], scene_bounding_box.lower [2]);
				
			scene_bounding_box.upper [0] = MAX (bu [0], scene_bounding_box.upper [0]);
			scene_bounding_box.upper [1] = MAX (bu [1], scene_bounding_box.upper [1]);
			scene_bounding_box.upper [2] = MAX (bu [2], scene_bounding_box.upper [2]);

			if (!load_from_file) {
				unsigned int axis;

				prims [j] = j;
				for (axis = 0; axis != 3; axis ++) {
					if (bl [axis] == bu [axis])
						event_list_add (&events, bl [axis], j, axis, Split_Planar);
					else {
						event_list_add (&events, bl [axis], j, axis, Split_Start);
						event_list_add (&events, bu [axis], j, axis, Split_End);
					}
				}
			}
		}
	}



	printf ("Scene bounding box %d\n", load_from_file);
	vector3_print (stdout, scene_bounding_box.lower);
	vector3_print (stdout, scene_bounding_box.upper);



	root = NEW (kd_tree_t);

	/* if we can have a serialized version of the kd tree we use that instead of building it */
	if (load_from_file) {
		kd_tree_load (kd_filename, root);
	}
	else {
		kd_tree_init (root);
		event_list_sort (&events);
		kd_tree_construct (root, 0, scene_bounding_box, num_prims, prims, &events, 0);

		if (kd_filename)
			kd_tree_save (kd_filename, root);
	}

	/* 	kd_tree_node_print (root->nodes, root->prims, 0); */
	return root;
}
