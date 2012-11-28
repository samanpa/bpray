#ifndef BP_MATERIAL
#define BP_MATERIAL

typedef struct material_t material_t;
#include <bp_texture.h>

struct material_t {
	float ior;
	texture_t *texture;
};

material_t *
bp_material_new ();

void
bp_material_init (material_t *, texture_t *, float);

void
bp_material (material_t *);

#endif
