#ifndef BP_IMAGE
#define BP_IMAGE

#include <bp_types.h>
#include <gd.h>

typedef struct _image_t image_t;

struct _image_t {
	gdImagePtr im;
};

image_t *
bp_image_new (int type, char *filename);

void
bp_image_get_color (image_t *, double u, double v, float , float, vector_t);

#endif
