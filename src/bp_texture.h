#ifndef BP_TEXTURE
#define BP_TEXTURE

typedef struct texture_t texture_t;

#include <bp_pigment.h>
#include <bp_normal.h>

enum {
	OPTION_FRESNEL = 1,
	OPTION_METALLIC = 2
};

typedef struct {
	vector_t ambient;
	float reflection;
	float diffuse;
	float phong;
	int   phong_size;
	int   options;
} finish_t;

struct texture_t {
	pigment_t *pigment;
	normal_t  *normal;
	finish_t  *finish;
};

finish_t *
bp_finish_get_default ();

void
bp_finish_init (finish_t *);

texture_t *
bp_texture_new ();

void
bp_texture_set_pigment (texture_t *, pigment_t *);

void
bp_texture_set_finish (texture_t *, finish_t *);

void
bp_texture_set_normal (texture_t *, normal_t *);

#endif
