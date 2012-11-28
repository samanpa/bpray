#include <bp_image.h>
#include <bp_scene.h>
#include <bp_io.h>
#include <bp_utils.h>
#include <math.h>


image_t *
bp_image_new (int type, char *filename)
{
	FILE *file = bp_file_open (curr_scene, filename, "rb");
	if (file) {
		image_t *image = NEW_1 (image_t, curr_scene->material_pool);
		switch (type) {
		case bp_PNG:
			image->im = gdImageCreateFromPng (file);
			break;
/*		case BMP:*/
/* 			image->im = gdImageCreateFromBmp (file); */
/* 			break; */
 		case bp_JPEG:
 			image->im = gdImageCreateFromJpeg (file); 
			break;
		case bp_GIF:
			image->im = gdImageCreateFromGif (file);
			break;
		}
		if (image->im == NULL) {
			bp_report (BP_LOG_ERROR, "%s declared with wrong type\n", filename);
			return NULL;
		}
		return image;
	}
	return NULL;
}

/* FIXME: anti aliasing and blending */

void
bp_image_get_color (image_t *image, double u, double v, float scalex, float scaley,
		    vector_t color)
{
	int c;
	int x, y;
	gdImagePtr im;

	im = image->im;

	x = rint (gdImageSX (im) * u);
	x = ((int) rint (x * scalex)) % gdImageSX (im);

	y = rint (gdImageSY (im) * (1 - v));
	y = ((int) rint (y * scaley)) % gdImageSY (im);
	
	c = gdImageGetPixel (im, x, y);
	
	color [0] = gdImageRed (im, c) / 255.0;
	color [1] = gdImageGreen (im, c) / 255.0;
	color [2] = gdImageBlue(im, c) / 255.0;
}

