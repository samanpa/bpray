#include <bp_light.h>
#include <bp_config.h>
#include <bp_memory.h>
#include <bp_utils.h>

static bp_mempool_t *light_pool;
static unsigned int num_lights = 0;

light_t *
bp_light_new (vector_t pos, vector_t color)
{
	light_t *light;

	if (!light_pool)
		light_pool = bp_mempool_new ();
	
	light = NEW_1 (light_t, light_pool);
	num_lights++;
	
	ASSIGN (light->pos, pos);
	ASSIGN (light->color, color);
	
	return light;
}

BP_EXPORT void
bp_lights_init (scene_t *scene)
{
	light_t *light_ptr;
	bp_mempool_node_t *node;
	unsigned int i = 0;
	const unsigned int light_size = sizeof (light_t);

	/* FIXME: allocate these in the light_pool */
	scene->lights     = NEW_0 (light_t *, num_lights + 1);
	
	/* if there are no lights in the scene*/
	if (!light_pool) {
		scene->lights [0] = NULL;
		return;
	}

	for (node = light_pool->curr; node; node = node->next) {
		unsigned int num_in_pool;

		num_in_pool = (node->size - node->rest)/ light_size;
		for (light_ptr = (light_t *)node->start; num_in_pool;
		     light_ptr++, num_in_pool --, i++) {
			scene->lights [i] = light_ptr;
		}
	}
	scene->lights [i] = NULL;
}


void
bp_lights_destroy ()
{
	num_lights = 0;
	bp_mempool_destroy (light_pool);
}
