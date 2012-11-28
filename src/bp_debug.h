#ifndef BP_DEBUG
#define BP_DEBUG

#include <assert.h>
/* FIXME */
#define x86

#ifdef x86
#define GET_TIME(x) __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x))
#else
#define GET_TIME(x) {assert (1);}
#endif

#ifndef NDEBUG
#include <stdint.h>
void
bp_profiler_add_to_counter (uint8_t ch, uint64_t val);

uint64_t
bp_profiler_get_counter (uint8_t ch);
#endif


#endif
