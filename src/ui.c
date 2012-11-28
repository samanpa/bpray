#include <ui.h>
#include <bp_math.h>
#include <bp_memory.h>

static user_input_t inputs;

user_input_t *
get_user_input () {
	return &inputs;
}

void
ui_init (ui_t *ui, framebuffer_t *fb)
{
	window_create (ui, fb);

	glClearColor (1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(0);
	glDisable(GL_BLEND);

	/* 1:1 mapping between texels & pixels. */
	glViewport(0, bp_framebuffer_get_vertical_resolution (fb) - bp_framebuffer_get_canvas_height (fb),
		   bp_framebuffer_get_canvas_width (fb), bp_framebuffer_get_canvas_height (fb));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 

	/* draw */
	glGenTextures(1, &ui->textureId);
	glBindTexture(GL_TEXTURE_2D, ui->textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bp_framebuffer_get_canvas_width (fb), 
		     bp_framebuffer_get_canvas_height (fb), 0, GL_RGB, GL_FLOAT, 
		     bp_framebuffer_get_canvas (fb));

	ui->glCallList = glGenLists(1);
	glNewList(ui->glCallList, GL_COMPILE);
	{
		/* flipped up upside down. hmm. */
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1); glVertex3f(-1, -1, -0.5f);
			glTexCoord2f(1, 1); glVertex3f( 1, -1, -0.5f);
			glTexCoord2f(1, 0); glVertex3f( 1,  1, -0.5f);
			glTexCoord2f(0, 0); glVertex3f(-1,  1, -0.5f);
		}
		glEnd();
	}
	glEndList();
}

int
ui_update (ui_t *ui, framebuffer_t *fb)
{
	/* FIXME: find the actual screen coordinates and scale the drawing to fit */
	glViewport(0, bp_framebuffer_get_vertical_resolution (fb) - bp_framebuffer_get_canvas_height (fb),
		   bp_framebuffer_get_canvas_width (fb), bp_framebuffer_get_canvas_height (fb));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	window_update (ui);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, ui->textureId);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, bp_framebuffer_get_canvas_width (fb),
			bp_framebuffer_get_canvas_height (fb), GL_RGB, GL_FLOAT,
			bp_framebuffer_get_canvas(fb));

 	glCallList (ui->glCallList);
	window_swap_buffers (ui);

	return inputs.exit;
}

void
ui_destroy (ui_t *ui)
{
   	window_destroy (ui); 
}
