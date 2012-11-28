#ifndef BP_UTILS
#define BP_UTILS

#include <strings.h>
#include <stdlib.h>
#include <bp_config.h>
#include <bp_memory.h>
#include <bp_math.h>
#include <bp_debug.h>

typedef void (*bp_report_func_t) (int, const char *);
typedef void (*bp_report_progress_func_t) (float p);

void
bp_report_func_set (bp_report_func_t r);

void
bp_report (int, const char *, ...);

void
bp_reportv (int, const char *, va_list);

enum {
	BP_LOG_WARNING,
	BP_LOG_ERROR,
	BP_LOG_MESSAGE,
	BP_LOG_DEBUG 
};

void bp_report_set_render_progress_func (bp_report_progress_func_t f);
void bp_report_render_progress (float);

#endif
