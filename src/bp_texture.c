#include <bp_texture.h>
#include <bp_scene.h>

finish_t *
bp_finish_get_default ()
{
	static finish_t *finish;
	if (!finish) {
		finish = NEW_1 (finish_t, curr_scene->material_pool);
		bp_finish_init (finish);
	}
	return finish;
}


void
bp_finish_init (finish_t *finish)
{
	ASSIGN (finish->ambient, origin);
	finish->reflection = 0.0;
	finish->diffuse    = 0.6;
	finish->specular   = 0.2;
	finish->shininess  = 40;
	finish->options    = 0;

}

texture_t *
bp_texture_new ()
{
	return NEW_1 (texture_t, curr_scene->material_pool);
}

void
bp_texture_set_pigment (texture_t *t, pigment_t *p)
{
	t->pigment = p;
}

void
bp_texture_set_finish (texture_t *t, finish_t *f)
{
	t->finish = f;
}

void
bp_texture_set_normal (texture_t *t, normal_t *n)
{
	t->normal = n;
}
