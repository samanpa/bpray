#include <bp_math.h>

vector_t x = {1, 0, 0};
vector_t y = {0, 1, 0};
vector_t z = {0, 0, 1};
vector_t origin = {0, 0, 0};

#ifdef USE_FAST_INV_SQRT
static float
fast_inverse_sqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; /* get bits for floating value */
	i = 0x5f3759df - (i>>1); /* gives initial guess y0 */
	x = *(float*)&i; /* convert bits back to float */
	x = x*(1.5f-xhalf*x*x); /* Newton step, repeating increases accuracy */
	return x;
}
#endif


void vector_normalize (vector_t src, vector_t dst) {
	float mag;
#ifndef USE_FAST_INV_SQRT
	mag = 1/MAG(src);
#else
	mag = fast_inverse_sqrt (src [0] * src [0] + 
				 src [1] * src [1] +
				 src [2] * src [2]);
#endif
	SMUL (dst, mag, src);
}

void vector3_print (FILE *file, const vector_t vec) {
	fprintf (file, "{%.12f, %.12f, %.12f}\n", vec [0], vec [1], vec [2]);
}

void vector4_print (FILE *file, const vector4_t vec) {
	fprintf (file, "{%f, %f, %f %f}\n", vec [0], vec [1], vec [2], vec [3]);
}

void vector_print (const vector_t vec) {
	vector3_print (stderr, vec);
}

void matrix_print (const matrix_t m)
{
	fprintf (stderr, "\t|%f,\t%f,\t%f\t|\n", m [0][0], m [0][1], m [0][2]);
	fprintf (stderr, "\t|%f,\t%f,\t%f\t|\n", m [1][0], m [1][1], m [1][2]);
	fprintf (stderr, "\t|%f,\t%f,\t%f\t|\n", m [2][0], m [2][1], m [2][2]);
}

void bp_matrix_vector_mult (vector_t result, matrix_t A, vector_t x)
{
	result [0] = DOT (A [0], x);
	result [1] = DOT (A [1], x);
	result [2] = DOT (A [2], x);
}

void bp_matrix_transpose (matrix_t res, matrix_t matrix)
{
#define FLIP(m, s, i, j) ((m)[i][j] = (s)[j][i])
	FLIP (res, matrix, 0, 0);
	FLIP (res, matrix, 0, 1);
	FLIP (res, matrix, 0, 2);
	FLIP (res, matrix, 1, 0);
	FLIP (res, matrix, 1, 1);
	FLIP (res, matrix, 1, 2);
	FLIP (res, matrix, 2, 0);
	FLIP (res, matrix, 2, 1);
	FLIP (res, matrix, 2, 2);
#undef FLIP	
}

