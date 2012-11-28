#include <bp_utils.h>
#include <bp_config.h>
#include <stdlib.h>

static bp_report_func_t report;

BP_EXPORT void
bp_report_func_set (bp_report_func_t r)
{
	report = r;
}

BP_EXPORT void
bp_report (int type, const char *fmt, ...)
{
	if (report) {
		va_list ap;
		va_start (ap, fmt);
		bp_reportv (type, fmt, ap);
		va_end (ap);
	}
}

void bp_reportv (int type, const char *fmt, va_list ap)
{
	gchar *ret;
	if (report) {
		ret = g_strdup_vprintf (fmt, ap);
		report (type, ret);
		free (ret);
	}
}

static bp_report_progress_func_t report_progress;

BP_EXPORT void 
bp_report_set_render_progress_func (bp_report_progress_func_t f)
{
	report_progress = f;
}

void
bp_report_render_progress (float f)
{
	if (report_progress)
		report_progress (f);
}



