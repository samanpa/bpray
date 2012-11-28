#include "bp_scene.h"
#include "bp_utils.h"
#include "bp_object.h"
#include "shade.h"
#include <stdint.h>
#include <string.h>
#include <math_sse.h>
#include <pthread.h>

scene_t *curr_scene;

static vector_t background = {0, 0, 0};

static bp_settings_t initial_settings = {
	{                     /* atmosphere */
		{0, 0, 0},       /* background */
		{0, 0, 0}        /* ambient light */
	},
	5,                    /* max trace level */
        1,                    /* number of anti aliasing samples */
	0.0039,               /* adc bailout */
	2.2,                  /* gamma */
	".bp_script",         /* input filename */
	NULL,                 /* kd tree output filename */
	1                     /* num threads */
};

BP_EXPORT scene_t *
bp_scene_new ()
{
	scene_t *scene = NEW (scene_t);
	curr_scene = scene;

	memcpy (&scene->settings, &initial_settings, sizeof (bp_settings_t));
	memset (&scene->library_path, 0, MAX_LIBRARY_PATH * sizeof (char *));

	bp_scene_set_horizontal_resolution (scene, 800);
	bp_scene_set_vertical_resolution (scene, 600);

	scene->material_pool = bp_mempool_new ();

	bp_camera_init (&scene->camera);

	return curr_scene;
}

static void
framebuffer_init (framebuffer_t *fb)
{
	unsigned int i;
	const unsigned int v_res = fb->vertical_resolution;
	const unsigned int h_res = fb->horizontal_resolution;

	/* Textures need to have a height/width equal to a power of 2 for versions of OpenGL less than 1.5 */
	for (i  = (1 << 31) ; i != 0; i >>= 1)
		if (h_res & i) {
			int j;
			fb->canvas_width = (h_res & ~i) ? i << 1 : i;
			for (j = 0; ((1 << j) & fb->canvas_width) == 0; j++)
				;
			fb->log2_canvas_width = j;
			break;
		}

	for (i  = (1 << 31) ; i != 0; i >>= 1)
		if (v_res & i) {
			fb->canvas_height = (v_res & ~i) ? i << 1: i;
			break;
		}

	fb->canvas = NEW_0 (vector_t, fb->canvas_height * fb->canvas_width);
}

typedef struct {
	scene_t *scene;
	unsigned int start;
	unsigned int end;
} thread_args_t; 

static pthread_mutex_t line_id_mutex;
static pthread_t *threads;
static thread_args_t *thread_args;
static pthread_attr_t thread_attr;

static float gamma_inv;

static const simd4_t simd4f_minus_ones = {-1,-1,-1,-1};
static simd4i_t simd4i_minus_ones;
static const simd4_t simd4_zeros = {0, 0, 0, 0};
static const simd4_t simd4_ones = {1, 1, 1, 1};

void
bp_scene_init (scene_t *scene)
{
	int i;
	int parcel_size, parcel_start;
	const unsigned int vertical_resolution = bp_scene_get_vertical_resolution (scene);
	const unsigned int horizontal_resolution = bp_scene_get_horizontal_resolution (scene);

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	threads = (pthread_t *)calloc (sizeof (pthread_t), scene->settings.num_threads);
	thread_args = (thread_args_t *)calloc (sizeof (thread_args_t), scene->settings.num_threads);

	pthread_mutex_init (&line_id_mutex, NULL);

	gamma_inv = 1 / scene->settings.gamma;
	

	/* FIXME: this can be improved a lot */
	parcel_size = vertical_resolution / scene->settings.num_threads + 
		((vertical_resolution % scene->settings.num_threads) ? 1 : 0);
	
	for (parcel_start = 0, i = 0; parcel_start < vertical_resolution; i++) {
		thread_args [i].scene = scene;
		thread_args [i].start = parcel_start;
		parcel_start += parcel_size;
		thread_args [i].end = MIN (parcel_start, horizontal_resolution);
	}

	framebuffer_init (bp_scene_get_framebuffer (scene));

	simd4i_minus_ones = _mm_set1_epi32 (-1);
}

static inline void 
gamma_correct (vector_t res_color, vector_t color, float gamma)
{
	res_color [0] = pow (MIN (color [0], 1.0), gamma);
	res_color [1] = pow (MIN (color [1], 1.0), gamma);
	res_color [2] = pow (MIN (color [2], 1.0), gamma);
}
 

#ifdef PRODUCTION
static volatile float render_progress = 0.0;
#endif

void 
bp_ray_trace_packet (const ray4_t *ray, vector_t *colors, simd4i_t srcprim_id, int depth, simd4_t fdepth)
{
	unsigned int activeMask;
	
  	intersect4_t isect4;

	ASSIGN (colors [0], background);
	ASSIGN (colors [1], background);
	ASSIGN (colors [2], background);
	ASSIGN (colors [3], background);

 	if ((depth > curr_scene->settings.max_trace_level) 
	    | (simd4_extract_sign (simd4_float_lt (fdepth, simd4_from_float (curr_scene->settings.adc_bailout))) == 0xf)) {
		return;
	}

 	isect4.prim_id = simd4i_minus_ones;

 	bp_kd_tree_packet_find_nearest (curr_scene->kd_tree_root, ray, &isect4);

	activeMask = _mm_movemask_ps (simd4_float_eq (_mm_cvtepi32_ps (isect4.prim_id), simd4f_minus_ones));

 	/* If there was no intersection terminate early */
	if (activeMask == 0xF)
		return;

 	bp_shade_packet (curr_scene, &isect4, ray, colors, depth, fdepth, srcprim_id);
}


static void *
draw_scanlines (void *data)
{
	thread_args_t *thread_args = (thread_args_t *)data;
	scene_t *scene = thread_args->scene;
	int i, end = thread_args->end;

	const unsigned int h_resolution = bp_scene_get_horizontal_resolution (scene);


	for (i = thread_args->start; i < end; i += 2) {
		int j;
#ifdef PRODUCTION
		const unsigned int v_resolution = bp_scene_get_vertical_resolution (scene);
 		bp_report_render_progress (render_progress++ / v_resolution);
#endif
		for (j = 0; j < h_resolution; j+= 2) {
			ray4_t ray4;
			vector_t colors [4];

			bp_camera_get_ray2x2 (&scene->camera, i, j, &ray4);
			bp_ray_trace_packet (&ray4, colors, simd4i_minus_ones, 0, simd4_ones);

			gamma_correct (bp_framebuffer_get_color (&scene->fb, i, j), colors [0], gamma_inv);
			gamma_correct (bp_framebuffer_get_color (&scene->fb, i, (j + 1)), colors [1], gamma_inv);
			gamma_correct (bp_framebuffer_get_color (&scene->fb, (i + 1), j), colors [2], gamma_inv);
			gamma_correct (bp_framebuffer_get_color (&scene->fb, (i + 1), (j + 1)), colors [3], gamma_inv);
		}
	}
	pthread_exit (NULL);
	return NULL;
}

BP_EXPORT void
bp_scene_draw (scene_t * scene)
{
	unsigned int i;

#if 0
	ADD (background, scene->settings.atmosphere.background,
	     scene->settings.atmosphere.ambient_light);
#else
	ASSIGN (background, scene->settings.atmosphere.background);
#endif

	for (i = 0; i < scene->settings.num_threads; i++) {
 		int rc = pthread_create (&threads [i], &thread_attr, draw_scanlines, &thread_args [i]);
		if (rc) {
			printf("ERROR; thread %d return code from pthread_create() is %d\n",
			       i + 1,rc);
			exit(-1);
		}
	}

	for (i = 0; i < scene->settings.num_threads; i++) {
		int rc = pthread_join(threads[i], NULL);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
}

void 
bp_ray_trace (const ray_t *ray, vector_t color, unsigned int srcprim_id, int depth, float fdepth)
{
	intersect_t isect;

	if (depth > curr_scene->settings.max_trace_level
	       || fdepth < curr_scene->settings.adc_bailout) {
		ASSIGN (color, background);
		return;
	}

	isect.prim_id = -1;
	bp_kd_tree_find_nearest (curr_scene->kd_tree_root, ray, &isect);

	/* If there was no intersection terminate early */
	if (isect.prim_id == -1) {
		COPY (background, color);
		return;
	}

	bp_shade (curr_scene, &isect, ray, color, depth, fdepth, srcprim_id);
}


void
bp_scene_destroy (scene_t *scene)
{
	pthread_attr_destroy(&thread_attr);
	free (threads);
	free (thread_args);
	free (bp_scene_get_framebuffer (scene));
}
