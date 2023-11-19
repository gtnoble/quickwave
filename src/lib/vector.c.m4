include(`vector.m4')
include(`code_generator.m4')
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

M4_MAKE_FOR_NUMERIC_TYPES(`make_vector_shift')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_ELEMENT')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_ELEMENT_VALUE')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_DOT')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_SCALE')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_APPLY')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_NEW')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_FROM_ARRAY')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_DUPLICATE')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_LENGTH')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_REVERSE')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_RESET')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_FREE')

int modular_add(int a, int b, int max) {
    int sum = a + b;
    return modulo_euclidean(sum, max);
}

int modulo_euclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}