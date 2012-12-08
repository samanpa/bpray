#include "bp_ray.h"
#include <gd.h>
#include <config.h>
#include <ui.h>

static void report_progress (float progress)
{
	static int last = -1;
	const int TOTAL = 80;
	const char *STAR = "*";
	const char *DASH = "_";

	int num_stars  = rint (progress * TOTAL);
	int num_dashes = TOTAL - num_stars;
	int i;
	
	if (num_stars == last)
		return;

	printf ("\r");/* %d%% ", (int)rint (progress * 100)); */
	last = num_stars;
	for (i = 0; i < num_stars; i++)
		printf (STAR);
	
	for (i = 0; i < num_dashes; i++)
		printf (DASH);

	fflush (stdout);
}

static void report (int type, const char *msg)
{
	switch (type) {
	case BP_LOG_WARNING:
		fprintf (stderr, msg);
		fprintf (stderr, "\n");
		break;
	case BP_LOG_ERROR:
	case BP_LOG_DEBUG:
		fprintf (stderr, msg);
		fprintf (stderr, "\n");
		fflush (stderr);
		break;
	case BP_LOG_MESSAGE:
	default:
		fprintf (stderr, msg);
		fprintf (stderr, "\n");
		break;
	}
}

static void usage (int flags)
{
	fprintf (stderr, ""
		 "Usage:\n"
		 "   bp_ray [options] ini_file\n"
		 "Options:\n"
		 "   --num-threads n --num-samples n --script-file <file> --output-file <file>  \n"
		);
	if (flags == 1)
		exit (1);
}

static void report_error (char *str)
{
	g_error ("ERROR: %s\n", str);
	usage (1);
}

static char *ini_file;
static char *script_file;
static char *output_filename;

static void parse_args (int argc, char **argv, scene_t *scene)
{
	int i;
	for (i = 1; i < argc; ++i) {
		if (!strcmp (argv [i], "--help")) {
			usage (0);
			exit (0);
		}
		else if (!strcmp (argv [i], "--output-file")) {
			++i;
			if (i < argc)
				output_filename = strdup (argv [i]);
			else
				report_error ("no parameter given to --output-file");
		}
		else if (!strcmp (argv [i], "--script-file")) {
			++i;
			if (i < argc)
				script_file = argv [i];
			else
				report_error ("no parameter given to --script-file");
		}
		else if (!strcmp (argv [i], "--num-threads")) {
			++i;
			if (i < argc) {
				scene->settings.num_threads = atoi (argv [i]);
				if (scene->settings.num_threads <= 0)
					report_error ("num threads less than 0");
			} else
				report_error ("no parameter given to --num-threads");
		}
		else if (!strcmp (argv [i], "--num-samples")) {
			++i;
			if (i < argc) {
				scene->settings.num_samples = atoi (argv [i]);
				if (scene->settings.num_samples <= 0)
					report_error ("num samples less than 0");
			}
			else
				report_error ("no parameter to --num-samples");
		}
		else if (!strcmp (argv [i], "--kd-tree-file")) {
			++i;
			if (i < argc) {
				scene->settings.kd_tree_filename = argv [i];
			}
			else
				report_error ("no parameter to --kd-tree-file");
		}
		else if (!strcmp (argv [i], "--gl")) {
			++i;
			scene->use_gl = 1;
		}
		else if (argv [i][0] == '-') {
			bp_report (BP_LOG_ERROR, "ERROR: unknown option %s\n", argv [i]);
			exit (0);
		}
		else {
			ini_file = argv [i];
		}
	}
}


void post_init (scene_t *scene)
{
	if (script_file) {
		unsigned int len;

		scene->settings.input_filename = script_file;

		len = strlen (script_file);

		if (len > 4 && script_file [len - 4] == '.') {
			output_filename = strdup (script_file);
			output_filename [len - 3] = 'p';
			output_filename [len - 2] = 'n';
			output_filename [len - 1] = 'g';
		}
	}
}

void
save_file (scene_t *scene, unsigned int height, unsigned int width)
{
	gdImagePtr image = gdImageCreateTrueColor (width, height);
	FILE * file  = fopen (output_filename, "wb");
	unsigned int i, j;
	framebuffer_t *fb = bp_scene_get_framebuffer(scene);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			vector_t color;
			float color_val;
			ASSIGN (color, bp_framebuffer_get_color (fb, i, j));
			color_val = gdImageColorExact(image
						      , (int) (255 * color [0])
						      , (int) (255 * color [1])
						      , (int) (255 * color [2]));
			gdImageSetPixel (image, j, i, color_val);
		}
	}
 	gdImagePng(image, file);
	gdImageDestroy(image);
	fclose (file);
}

static inline void
report_perf (char *str)
{
	char *pstr = str;

	while (*pstr) {
		printf ("performace counter %c:\t %llu\n", *pstr,
			(unsigned long long)bp_profiler_get_counter (*pstr));
		pstr ++;
	}

}

int main (int argc, char **argv)
{
	scene_t *scene  = bp_scene_new ();
	GTimer *timer;
	framebuffer_t *fb = bp_scene_get_framebuffer (scene);
	gulong microseconds; /* not really usefully but needed by the timing code */
	

	bp_report_func_set (report);

	if (argc == 1) {
		fprintf (stderr, "No script filename name given\n");
		return 1;
	}
	
	parse_args (argc, argv, scene);
	bp_ray_init (argv [0], ini_file, scene);
	post_init (scene);
	timer = g_timer_new ();
	g_timer_start (timer);
      	if (bp_script_parse_file (scene))
		return 1;

	g_timer_stop (timer);
	printf ("Parsing took %f.\n", g_timer_elapsed (timer, &microseconds));
	g_timer_reset (timer);

	g_timer_start (timer);
	bp_lights_init (scene);
	bp_objects_init (scene);
	g_timer_stop (timer);
	printf ("kd tree construction time %f.\n", g_timer_elapsed (timer, &microseconds));

	if (scene->use_gl) {
		ui_t ui;
		ui_init (&ui, scene);
		ui_draw (&ui);
	}
	else {
		bp_report_set_render_progress_func (report_progress);
		bp_scene_draw (scene);
		save_file (scene
			   , bp_framebuffer_get_vertical_resolution (fb)
			   , bp_framebuffer_get_horizontal_resolution (fb));
	}

/*   	bp_ray_destroy (scene); */
  	g_timer_destroy (timer);

	
 	report_perf ("abcd");

	return 0;

}
