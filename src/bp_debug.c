#include <bp_debug.h>

#ifndef NDEBUG
#define MAX_NUM_COUNTERS 256
static struct {
	uint64_t val;
	int      num_times;
} performance_counters [MAX_NUM_COUNTERS];


void
bp_profiler_add_to_counter (uint8_t ch, uint64_t val)
{
	performance_counters [ch].val += val;
	performance_counters [ch].num_times ++;
}

uint64_t
bp_profiler_get_counter (uint8_t ch)
{
	return performance_counters [ch].val;
}

#endif

