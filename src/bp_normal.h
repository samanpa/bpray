#ifndef BP_NORMAL
#define BP_NORMAL

typedef struct normal_t normal_t;

#include <bp_image.h>

struct normal_t {
	union {
		image_t bump_map;
	} u;
	void (*get_normal) (const intersect_t *, vector_t n);
};


normal_t *
bp_normal_new ();

normal_t *
bp_bump_map_new (image_t *im);

normal_t *
bp_normal_get_default ();

#endif
