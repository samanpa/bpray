#ifndef BP_MATH_SSE_H
#define BP_MATH_SSE_H

#include "bp_types.h"

#ifdef SSE


#define simd4_get0(a) ( ((const float *)(void *)(&(a)))[0])
#define simd4_get1(a) ( ((const float *)(void *)(&(a)))[1])
#define simd4_get2(a) ( ((const float *)(void *)(&(a)))[2])
#define simd4_get3(a) ( ((const float *)(void *)(&(a)))[3])

#define simd4i_get0(a) ( ((const int *)(void *)(&(a)))[0])
#define simd4i_get1(a) ( ((const int *)(void *)(&(a)))[1])
#define simd4i_get2(a) ( ((const int *)(void *)(&(a)))[2])
#define simd4i_get3(a) ( ((const int *)(void *)(&(a)))[3])

#define simd4_vector_extract0(res, vec) do {\
        (res) [X_axis] = simd4_get0 ((vec) [X_axis]); \
        (res) [Y_axis] = simd4_get0 ((vec) [Y_axis]); \
        (res) [Z_axis] = simd4_get0 ((vec) [Z_axis]); \
} while (0)

#define simd4_vector_extract1(res, vec) do {\
        (res) [X_axis] = simd4_get1 ((vec) [X_axis]); \
        (res) [Y_axis] = simd4_get1 ((vec) [Y_axis]); \
        (res) [Z_axis] = simd4_get1 ((vec) [Z_axis]); \
} while (0)

#define simd4_vector_extract2(res, vec) do {\
        (res) [X_axis] = simd4_get2 ((vec) [X_axis]); \
        (res) [Y_axis] = simd4_get2 ((vec) [Y_axis]); \
        (res) [Z_axis] = simd4_get2 ((vec) [Z_axis]); \
} while (0)

#define simd4_vector_extract3(res, vec) do {\
        (res) [X_axis] = simd4_get3 ((vec) [X_axis]); \
        (res) [Y_axis] = simd4_get3 ((vec) [Y_axis]); \
        (res) [Z_axis] = simd4_get3 ((vec) [Z_axis]); \
} while (0)

#define simd4_INVERSE(res, src) do {\
        const simd4_t rcp = _mm_rcp_ps (src) ; \
        (res) = _mm_sub_ps (_mm_add_ps(rcp,rcp), \
			  _mm_mul_ps(_mm_mul_ps(rcp,rcp), src)); \
} while (0)

#define simd4_INVERSE_SQRT(res, src) do {\
        (res) = _mm_rsqrt_ps (src) ; \
} while (0)

#define simd4_float_gt(a,b) _mm_cmpgt_ps (a, b)
#define simd4_float_lt(a,b) _mm_cmplt_ps (a, b)
#define simd4_float_eq(a,b) _mm_cmpeq_ps (a, b)
#define simd4_from_float(a) _mm_set1_ps(a)

#define simd4_int_eq(a,b) _mm_cmpeq_epi32 (a, b)

#define simd4_extract_sign(a) _mm_movemask_ps(a)

#define simd4_Update(mask, a, b) ( \
        _mm_or_ps (_mm_and_ps (a, mask), \
                   _mm_andnot_ps(mask, b)))

#define simd4i_Update(mask, a, b) ( \
        _mm_or_si128 (_mm_and_si128 (a, mask), \
                      _mm_andnot_si128(mask, b)))

#define simd4_vector_SMUL(res, scal, vec) do { \
        (res) [0] = _mm_mul_ps (scal, (vec) [0]); \
        (res) [1] = _mm_mul_ps (scal, (vec) [1]); \
        (res) [2] = _mm_mul_ps (scal, (vec) [2]); \
} while (0)

#define simd4_vector_SCALE_ADD(res, scal, vec1, vec2) do { \
        (res) [X_axis] = _mm_add_ps (_mm_mul_ps (scal, (vec1) [X_axis]), (vec2) [X_axis]); \
        (res) [Y_axis] = _mm_add_ps (_mm_mul_ps (scal, (vec1) [Y_axis]), (vec2) [Y_axis]); \
        (res) [Z_axis] = _mm_add_ps (_mm_mul_ps (scal, (vec1) [Z_axis]), (vec2) [Z_axis]); \
} while (0)

#define simd4_vector_INVERSE(res, src) do {\
        simd4_INVERSE ((res) [X_axis], (src) [X_axis]); \
        simd4_INVERSE ((res) [Y_axis], (src) [Y_axis]); \
        simd4_INVERSE ((res) [Z_axis], (src) [Z_axis]); \
} while (0)

#define simd4_vector_SIZE(a) (\
  _mm_add_ps ( \
       _mm_add_ps ( \
            _mm_mul_ps ((a) [0], (a) [0]), \
            _mm_mul_ps ((a) [1], (a) [1])), \
       _mm_mul_ps ((a) [2], (a) [2])))

#define simd4_vector_ASSIGN1(res, scal) do {\
        (res) [X_axis] = _mm_set1_ps ((scal) [X_axis]); \
        (res) [Y_axis] = _mm_set1_ps ((scal) [Y_axis]); \
        (res) [Z_axis] = _mm_set1_ps ((scal) [Z_axis]); \
} while (0)

#define simd4_vector_ASSIGN(res, src) do {\
        (res) [X_axis] = (src) [X_axis]; \
        (res) [Y_axis] = (src) [Y_axis]; \
        (res) [Z_axis] = (src) [Z_axis]; \
} while (0)

#define simd4_vector_NORM(res, src) do {\
        const simd4_t mag = simd4_vector_SIZE (src); \
        simd4_t inv; \
        simd4_INVERSE_SQRT (inv, mag); \
\
        simd4_vector_SMUL (res, inv, src); \
} while (0)
  
#endif

#endif

