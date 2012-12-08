#ifndef BP_UI
#define BP_UI

#include <bp_scene.h>
#include <gd.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef struct {
	unsigned int textureId; /* fixme: rename me */
	GLuint glCallList;
} ui_t;


BP_EXPORT void
ui_init (ui_t *ui, scene_t *scene);

BP_EXPORT int
ui_draw (ui_t *ui);

#endif
