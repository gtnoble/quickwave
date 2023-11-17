include(`vector.m4')
include(`code_generator.m4')
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

define(`M4_MAKE_VECTOR_ELEMENT_VALUE_PROTOTYPE',
`$1 vector_real_element_value(int index, const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_ELEMENT_VALUE_DECLARATION',
`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE($1);')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_ELEMENT_VALUE_DECLARATION')

define(`make_vector_shift',
`M4_MAKE_VECTOR_SHIFT_PROTOTYPE($1) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`make_vector_shift')

define(`M4_MAKE_VECTOR_ELEMENT', 
`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE($1) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_ELEMENT')

define(`M4_MAKE_VECTOR_ELEMENT_VALUE',
`M4_MAKE_VECTOR_ELEMENT_VALUE_PROTOTYPE($1) {
    return *vector_element_`'M4_FUNCTION_TYPE_TAG($1)`'(index, (M4_VECTOR_TYPE($1)*) buf);
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_ELEMENT_VALUE')

define(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT',
`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT_PROTOTYPE($1) {
    $1 fraction_between_elements = index - floor(index);
    return 
        M4_TAGGED_FUNCTION_NAME(vector_element_value, $1)((int) floor(index), buf) * (1 - fraction_between_elements) + 
        M4_TAGGED_FUNCTION_NAME(vector_element_value, $1)((int) ceil(index), buf) * fraction_between_elements;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT')

define(`M4_MAKE_VECTOR_DOT',
`M4_MAKE_VECTOR_DOT_PROTOTYPE($1) {
    assert(M4_TAGGED_FUNCTION_NAME(vector_length, $1)(a) == M4_TAGGED_FUNCTION_NAME(vector_length, $1)(b));
    double sum = 0;
    for (size_t i = 0; i < M4_TAGGED_FUNCTION_NAME(vector_length, $1)(a); i++) {
        sum += M4_TAGGED_FUNCTION_NAME(vector_element_value, $1)(i, a) * M4_TAGGED_FUNCTION_NAME(vector_element_value, $1)(i, b);
    }
    return sum;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_DOT')

define(`M4_MAKE_VECTOR_SCALE',
`M4_MAKE_VECTOR_SCALE_PROTOTYPE($1) {
    for (size_t i = 0; i < M4_TAGGED_FUNCTION_NAME(vector_length, $1)(vector); i++) {
        *M4_TAGGED_FUNCTION_NAME(vector_element, $1)(i, vector) *= scalar;
    }
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_SCALE')

define(`M4_MAKE_VECTOR_APPLY',
`M4_MAKE_VECTOR_APPLY_PROTOTYPE($1) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *M4_TAGGED_FUNCTION_NAME(vector_element, $1)(i, vector) = 
            operation(*M4_TAGGED_FUNCTION_NAME(vector_element, $1)(i, vector));
    }
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_APPLY')

define(`M4_MAKE_VECTOR_NEW',
`M4_MAKE_VECTOR_NEW_PROTOTYPE($1) {
    M4_VECTOR_TYPE($1) *circbuf = malloc(
        sizeof(M4_VECTOR_TYPE($1)) + sizeof($1) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    M4_TAGGED_FUNCTION_NAME(vector_reset, $1)(circbuf);
    return circbuf;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_NEW')

define(`M4_MAKE_VECTOR_FROM_ARRAY',
`M4_MAKE_VECTOR_FROM_ARRAY_PROTOTYPE($1) {
    M4_VECTOR_TYPE($1) *vector = M4_TAGGED_FUNCTION_NAME(vector_new, $1)(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *M4_TAGGED_FUNCTION_NAME(vector_element, $1)(i, vector) = elements[i];
    }

    return vector;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_FROM_ARRAY')

define(`M4_MAKE_VECTOR_DUPLICATE',
`M4_MAKE_VECTOR_DUPLICATE_PROTOTYPE($1) {
    size_t vector_length = M4_TAGGED_FUNCTION_NAME(vector_length, $1)(vector);
    M4_VECTOR_TYPE($1) *new_vector = M4_TAGGED_FUNCTION_NAME(vector_new, $1)(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof($1) * vector_length
    );
    return new_vector;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_DUPLICATE')

define(`M4_MAKE_VECTOR_LENGTH',
`M4_MAKE_VECTOR_LENGTH_PROTOTYPE($1) {
    return buf->n_elements;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_LENGTH')

define(`M4_MAKE_VECTOR_REVERSE',
`M4_MAKE_VECTOR_REVERSE_PROTOTYPE($1) {
    vector->is_reversed = ! vector->is_reversed;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_REVERSE')

define(`M4_MAKE_VECTOR_RESET',
`M4_MAKE_VECTOR_RESET_PROTOTYPE($1) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_RESET')

define(`M4_MAKE_VECTOR_FREE',
`M4_MAKE_VECTOR_FREE_PROTOTYPE($1) {
    free(buf);
}')

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