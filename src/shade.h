#ifndef BP_SHADE
#define BP_SHADE

#include "bp_scene.h"
#include "bp_utils.h"
#include "bp_script.h"
#include "bp_object.h"

void bp_shade (const scene_t * scene, intersect_t *isect, const ray_t *ray, 
  	       vector_t color, const int depth, const float fdepth, unsigned int);

void bp_shade_packet (const scene_t * scene, intersect4_t *isect, const ray4_t *ray, 
  	       vector_t *color, int depth, const simd4_t fdepth, const simd4i_t srcprim);

#endif
