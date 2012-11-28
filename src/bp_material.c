#include <bp_material.h>
#include <bp_scene.h>

material_t *
bp_material_new ()
{
	return NEW_1 (material_t, curr_scene->material_pool);
}

void
bp_material_init (material_t *m, texture_t *t, float ior)
{
	m->texture = t;
	m->ior     = ior;
}

