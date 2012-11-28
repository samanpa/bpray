#ifndef BP_SCENE
#define BP_SCENE

typedef struct scene_t scene_t;

extern scene_t *curr_scene;

#include "bp_config.h"
#include "bp_types.h"
#include "bp_light.h"
#include "bp_camera.h"
#include "bp_kd_tree.h"
#include "bp_memory.h"

enum {
	MAX_LIBRARY_PATH = 22,
	bp_PNG           = 1,
	bp_JPEG,
	bp_GIF
};

typedef struct {
	vector_t *canvas;

	unsigned int horizontal_resolution;
	unsigned int vertical_resolution;

	/**
	 * The canvas and image are different sizes because versions of OpenGL b4 1.4 require the dimensions of
	 *  textures to be powers of 2
	 */
	unsigned int canvas_width;
	unsigned int canvas_height;
	
	unsigned int log2_canvas_width;
	
} framebuffer_t;

#define bp_framebuffer_set_color(fb, h, w, color) do { \
     const int index = ((h) << (fb)->log2_canvas_width) + (w); \
     ASSIGN ((fb)->canvas [index], color); \
}while (0)

#define bp_framebuffer_get_color(fb, h, w) ((fb)->canvas [((h) << (fb)->log2_canvas_width) + (w)])
#define bp_framebuffer_get_canvas(fb) ((fb)->canvas)
#define bp_framebuffer_get_canvas_width(fb) ((fb)->canvas_width)
#define bp_framebuffer_get_canvas_height(fb) ((fb)->canvas_height)
#define bp_framebuffer_get_horizontal_resolution(fb) ((fb)->horizontal_resolution)
#define bp_framebuffer_get_vertical_resolution(fb) ((fb)->vertical_resolution)

#define bp_scene_get_framebuffer(scene) (&((scene)->fb))
#define bp_scene_set_horizontal_resolution(scene, res) (bp_scene_get_framebuffer(scene)->horizontal_resolution = (res))
#define bp_scene_set_vertical_resolution(scene, res) (bp_scene_get_framebuffer(scene)->vertical_resolution = (res))
#define bp_scene_get_horizontal_resolution(scene) (bp_scene_get_framebuffer(scene)->horizontal_resolution)
#define bp_scene_get_vertical_resolution(scene) (bp_scene_get_framebuffer(scene)->vertical_resolution)

typedef struct _bp_settings_t {
	struct {
		vector_t background;
		vector_t ambient_light;
#if 0
		struct {
			vector_t up;
			float fog_offset;
			float fog_alt;
			float distance;
			char type;
		} fog;
#endif
	} atmosphere;
	unsigned short max_trace_level;
	unsigned char num_samples;
	float adc_bailout;
	float gamma;
	char *input_filename;
	char *kd_tree_filename;
	unsigned int num_threads;
} bp_settings_t; 

 
struct scene_t {
	camera_t       camera;

	light_t      **lights;

 	bp_mempool_t  *material_pool;

	char          *library_path [MAX_LIBRARY_PATH];

	bp_settings_t  settings;
	
	kd_tree_t *kd_tree_root;

	framebuffer_t fb;
};

BP_EXPORT scene_t *
bp_scene_new ();

void 
bp_scene_init (scene_t *scene);

BP_EXPORT void 
bp_scene_draw (scene_t *scene);

void 
bp_scene_destroy (scene_t *scene);

void 
bp_ray_trace (const ray_t *ray, vector_t color, unsigned int prim_id, int depth, float fdepth);

#endif
