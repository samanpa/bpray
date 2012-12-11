#include <ui.h>
#include <bp_math.h>
#include <bp_memory.h>

void draw_scene (void)
{
	GLuint texid = 1;
	framebuffer_t* fb;
	unsigned char* image;
	int h, w;
	fb = &(curr_scene->fb);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bp_scene_draw (curr_scene);

	image = NEW_0(unsigned char, fb->canvas_width * fb->canvas_height * 4);
	for (w = 0; w < bp_framebuffer_get_canvas_width(fb); w++) {
		for (h = 0; h < bp_framebuffer_get_canvas_height(fb); h++) {
			vector_t color;
			int idx = 4 * (h * bp_framebuffer_get_canvas_width(fb) + w);
			ASSIGN (color, bp_framebuffer_get_color (fb, h, w));
			image[idx + 0] = (unsigned char)(255 * color [0]);
			image[idx + 1] = (unsigned char)(255 * color [1]);
			image[idx + 2] = (unsigned char)(255 * color [2]);
			//image[idx + 3] = (unsigned char)(128);
		}
	}

	
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   

	glTexImage2D (GL_TEXTURE_2D, 0
		      , GL_RGBA, fb->canvas_width, fb->canvas_height
		      , 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	/* release data, its been uploaded */
	free (image);

	glPushMatrix();
	glEnable (GL_TEXTURE_2D); /* enable texture mapping */
	glBindTexture (GL_TEXTURE_2D, texid); /* bind to our texture, has id of 13 */
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glBegin (GL_QUADS);
	glTexCoord2f (0,1);
        glVertex3f (-5, -5, 5);

        glTexCoord2f (1, 1);
        glVertex3f (5, -5, 5);

        glTexCoord2f (1, 0);
        glVertex3f (5, 5, 5);

        glTexCoord2f (0, 0);
        glVertex3f (-5, 5, 5);

	glEnd ();
	glPopMatrix();
	glDisable (GL_TEXTURE_2D); /* disable texture mapping */
	glutSwapBuffers();
}

void glutResize (int w, int h)
{
	static float eye = 100.0;
	static float near = 90.0;
	static float far = 100.0;
	static float bord = 5.0;
	float eye_pos[] = { 0, 0, eye};

	//prevent bad aspect ratios
	glViewport( 0, 0, w, h );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-bord, bord, -bord, bord, near, far);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt( eye_pos[0], eye_pos[1], eye_pos[2], 0, 0 , 0, 0, 1, 0 );

	glutPostRedisplay ();
}

void glutKeyboard (unsigned char key, int x, int y)
{
	const static float DELTA_DIR = 0.001;
	switch (key)
	{
	case 'a':
		curr_scene->camera.location[X_axis] += DELTA_DIR;
		break;
	case 'd':
		curr_scene->camera.location[X_axis] -= DELTA_DIR;
		break;
	case 'w':
		curr_scene->camera.location[Y_axis] += DELTA_DIR;
		break;
	case 's':
		curr_scene->camera.location[Y_axis] -= DELTA_DIR;
		break;
        case 'q':
        case 'Q':
		exit (1);
		break;
	}
}

void
ui_init (ui_t *ui, scene_t *scene)
{
	int argc = 0;
	char** argv = NULL;

	curr_scene = scene;

	glutInit(&argc, argv);
	glClearColor(1, 1, 1, 1);
	glEnable (GL_DEPTH_TEST);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (scene->fb.horizontal_resolution
			    , scene->fb.vertical_resolution);
	glutCreateWindow (PACKAGE_STRING);
	glutKeyboardFunc (glutKeyboard);
	glutDisplayFunc (draw_scene);
	glutReshapeFunc (glutResize);
	glutIdleFunc(draw_scene);

	glEnable( GL_SMOOTH );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL);
}

int
ui_draw (ui_t *ui)
{
	glutMainLoop(); // we never return...
}

