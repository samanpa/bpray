#include <ui.h>
#include <config.h>

#ifdef PLATFORM_LINUX

int
window_create (ui_t *ui, framebuffer_t *fb)
{
	return 1;
}

void
window_update (ui_t *ui)
{
}

void
window_swap_buffers (ui_t *ui)
{
}

void
window_destroy (ui_t *ui)
{
}
#endif
