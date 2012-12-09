#include "bp_box.h"

#define X 0
#define Y 1
#define Z 2


void
ray_box_intersect (const vector_t corner0, const vector_t corner1, const ray_t *ray, 
		   const int *ray_dir_negative, const vector_t inv_dir, float *t0, float *t1)
{
	float txmin, txmax;
	float tymin, tymax;
	float tzmin, tzmax;

	if (ray_dir_negative [X_axis]) {
		txmin = (corner1 [X_axis] - ray->orig [X_axis]) * inv_dir [X_axis];
		txmax = (corner0 [X_axis] - ray->orig [X_axis]) * inv_dir [X_axis];
	} else {
		txmin = (corner0 [X_axis] - ray->orig [X_axis]) * inv_dir [X_axis];
		txmax = (corner1 [X_axis] - ray->orig [X_axis]) * inv_dir [X_axis];
	}

	if (ray_dir_negative [Y_axis]) {
		tymin = (corner1 [Y_axis] - ray->orig [Y_axis]) * inv_dir [Y_axis];
		tymax = (corner0 [Y_axis] - ray->orig [Y_axis]) * inv_dir [Y_axis];
	} else {
		tymin = (corner0 [Y_axis] - ray->orig [Y_axis]) * inv_dir [Y_axis];
		tymax = (corner1 [Y_axis] - ray->orig [Y_axis]) * inv_dir [Y_axis];
	} 

	if (ray_dir_negative [Z_axis]) {
		tzmin = (corner1 [Z_axis] - ray->orig [Z_axis]) * inv_dir [Z_axis];
		tzmax = (corner0 [Z_axis] - ray->orig [Z_axis]) * inv_dir [Z_axis];
	} else {
		tzmin = (corner0 [Z_axis] - ray->orig [Z_axis]) * inv_dir [Z_axis];
		tzmax = (corner1 [Z_axis] - ray->orig [Z_axis]) * inv_dir [Z_axis];
	} 
	
	/* Find the biggest of txmin, tymin, tzmin */
	*t0 = MAX3 (txmin, tymin, tzmin);

	/* Find the smallest of txmax, tymax, tzmax */
	*t1 = MIN3 (txmax, tymax, tzmax);
}
