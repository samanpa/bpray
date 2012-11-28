#include <bp_pigment.h>
#include <string.h>

static void get_solid_color (pigment_t *p, const intersect_t *i, vector_t v)
{
	ASSIGN (v, p->data.solid_color);
}

static void get_image_color (pigment_t *p, const intersect_t *i, vector_t vec)
{
	double u, v;

 	prim_get_uv_coord (i, &u, &v);
	bp_image_get_color (&p->data.image_map, u, v, 1, 1, vec);
}


void
bp_pigment_set_solid_color (pigment_t *pig, vector5_t v)
{
	/* FIXME: wont work if are doing sse */
	ASSIGN (pig->data.solid_color, v);
	pig->filter    = v [3];
	pig->transmit  = v [4];
	pig->get_color = get_solid_color;
}

void
bp_pigment_set_image_map (pigment_t *p, image_t *i)
{
	p->filter      = 1;
	p->transmit    = 0;
	memcpy (&p->data.image_map, i, sizeof (image_t));
	p->get_color   = get_image_color;
}
