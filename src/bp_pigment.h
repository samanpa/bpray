#ifndef BP_PIGMENT
#define BP_PIGMENT

typedef struct _pigment_t pigment_t;

#include <bp_image.h>
#include <bp_object.h>

struct _pigment_t {
	union {
		vector_t solid_color;
		image_t  image_map;
	} data;
	float filter;
	float transmit; 

	void (*get_color) (pigment_t *, const intersect_t *, vector_t);
};

void
bp_pigment_set_solid_color (pigment_t *pig, vector5_t v);

void
bp_pigment_set_image_map (pigment_t *p, image_t *i);

/*
vector5_t *bp_solid_color_new (float r, float g, float b, float f, float t);
vector5_t *bp_solid_color_get_default ();
*/

#endif
