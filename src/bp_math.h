#ifndef BP_MATH
#define BP_MATH

#include <bp_types.h>
#include <math.h>
#include <stdio.h>

#include <bp_math_generic.h>

#define SMALL_VAL 0.0000001

#define M_PI 3.14159265358979323846264338327

#define ZERO1(f) (f < SMALL_VAL)
#define NOT_ZERO1(f) (f > SMALL_VAL)
#define NOT_ZERO(f) (fabs (f) > SMALL_VAL)

typedef vector_t matrix_t [3];

void bp_matrix_transpose (matrix_t res, matrix_t src);
void bp_matrix_vector_mult (vector_t result, matrix_t A, vector_t x);
void vector_normalize (vector_t src, vector_t dst);
void vector3_print (FILE *file, const vector_t vec);
void vector4_print (FILE *file, const vector4_t vec);
void vector_print (const vector_t vec);
void matrix_print (const matrix_t m);

#define NORM(dst, src) (vector_normalize (src, dst))
#define MIN3(a, b, c) ((((a)<(b)) ? (((a)<(c)) ? (a) : (c)) : (((b)<(c)) ? (b) : (c))))
#define MAX3(a, b, c) ((((a)>(b)) ? (((a)>(c)) ? (a) : (c)) : (((b)>(c)) ? (b) : (c))))
#define VMIN3(r, a, b, c) {\
       r [0] = MIN3 (a [0], b [0], c [0]); \
       r [1] = MIN3 (a [1], b [1], c [1]); \
       r [2] = MIN3 (a [2], b [2], c [2]); \
}
#define VMAX3(r, a, b, c) {\
       r [0] = MAX3 (a [0], b [0], c [0]); \
       r [1] = MAX3 (a [1], b [1], c [1]); \
       r [2] = MAX3 (a [2], b [2], c [2]); \
}
 

extern vector_t x;
extern vector_t y;
extern vector_t z;
extern vector_t origin;

#endif
