#include "bp_math.h"
#include "bp_utils.h"

void
ray_box_intersect (const vector_t corner0, const vector_t corner1, const ray_t *ray, 
		   const int *ray_dir_negative, const vector_t inv_dir, float *t0, float *t1);

