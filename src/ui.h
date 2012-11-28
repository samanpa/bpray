#ifndef BP_UI
#define BP_UI

#include <bp_types.h>
#include <bp_config.h>
#ifdef PLATFORM_WIN32
#include <windows.h>
#endif
#include <bp_scene.h>
#include <gd.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

typedef struct {
	unsigned int axis [3];
	unsigned int exit;
	unsigned int keys [30];
	struct {
		unsigned int shift;
		unsigned int control;
		unsigned int menu;
	} modifiers;
	
} user_input_t;

user_input_t *get_user_input ();

typedef struct {
#ifdef PLATFORM_WIN32
	HWND   wnd;  /* handle to our window */
	HGLRC  glrc;
	HDC    dc;

#endif
	unsigned int textureId; /* fixme: rename me */
	GLuint glCallList;

} ui_t;


BP_EXPORT void
ui_init (ui_t *ui, framebuffer_t *fb);

BP_EXPORT int
ui_update (ui_t *ui, framebuffer_t *fb);

void
ui_destroy (ui_t *ui);

int
window_create (ui_t *ui, framebuffer_t *fb);

void
window_update (ui_t *ui);

void window_destroy (ui_t *ui);

void
window_swap_buffers (ui_t *ui);
#endif
