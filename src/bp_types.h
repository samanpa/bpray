#ifndef BP_TYPES
#define BP_TYPES

#include "config.h"

typedef float vector2f_t [2];
typedef float vector_t  [3];
typedef float vector4_t [4];
typedef float vector5_t [5];

#ifdef SSE
#include <emmintrin.h>
typedef __m128 simd4_t;
typedef __m128i simd4i_t;

#else
typedef struct {
	float f [4];
} simd4_t;
#endif

typedef struct {
	vector_t dir;
	vector_t orig;
} ray_t;

typedef simd4_t simd4_vector_t [3];
typedef simd4i_t simd4i_vector_t [3];

typedef struct {
	simd4_vector_t dir;
	simd4_vector_t orig;
} ray4_t;

enum Axes_t {X_axis, Y_axis, Z_axis, No_axis};

#endif
