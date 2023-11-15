include(`vector.m4')
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

define(`macro_make_vector_element_value_prototype',
`$1 vector_real_element_value(int index, const macro_vector_type($1) *buf)')

define(`macro_make_vector_element_value_declaration',
`macro_make_vector_element_prototype($1);')

macro_make_for_numeric_types(`macro_make_vector_element_value_declaration')

define(`make_vector_shift',
`macro_make_vector_shift_prototype($1) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}')

macro_make_for_numeric_types(`make_vector_shift')

define(`macro_make_vector_element', 
`macro_make_vector_element_prototype($1) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}')

macro_make_for_numeric_types(`macro_make_vector_element')

define(`macro_make_vector_element_value',
`macro_make_vector_element_value_prototype($1) {
    return *vector_element_`'macro_function_type_tag($1)`'(index, (macro_vector_type($1)*) buf);
}')

macro_make_for_numeric_types(`macro_make_vector_element_value')

define(`macro_make_vector_interpolated_element',
`macro_make_vector_interpolated_element_prototype($1) {
    $1 fraction_between_elements = index - floor(index);
    return 
        macro_tagged_function_name(vector_element_value, $1)((int) floor(index), buf) * (1 - fraction_between_elements) + 
        macro_tagged_function_name(vector_element_value, $1)((int) ceil(index), buf) * fraction_between_elements;
}')

macro_make_for_numeric_types(`macro_make_vector_interpolated_element')

define(`macro_make_vector_dot',
`macro_make_vector_dot_prototype($1) {
    assert(macro_tagged_function_name(vector_length, $1)(a) == macro_tagged_function_name(vector_length, $1)(b));
    double sum = 0;
    for (size_t i = 0; i < macro_tagged_function_name(vector_length, $1)(a); i++) {
        sum += macro_tagged_function_name(vector_element_value, $1)(i, a) * macro_tagged_function_name(vector_element_value, $1)(i, b);
    }
    return sum;
}')

macro_make_for_numeric_types(`macro_make_vector_dot')

define(`macro_make_vector_scale',
`macro_make_vector_scale_prototype($1) {
    for (size_t i = 0; i < macro_tagged_function_name(vector_length, $1)(vector); i++) {
        *macro_tagged_function_name(vector_element, $1)(i, vector) *= scalar;
    }
}')

macro_make_for_numeric_types(`macro_make_vector_scale')

define(`macro_make_vector_apply',
`macro_make_vector_apply_prototype($1) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *macro_tagged_function_name(vector_element, $1)(i, vector) = 
            operation(*macro_tagged_function_name(vector_element, $1)(i, vector));
    }
}')

macro_make_for_numeric_types(`macro_make_vector_apply')

define(`macro_make_vector_new',
`macro_make_vector_new_prototype($1) {
    macro_vector_type($1) *circbuf = malloc(
        sizeof(macro_vector_type($1)) + sizeof($1) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    macro_tagged_function_name(vector_reset, $1)(circbuf);
    return circbuf;
}')

macro_make_for_numeric_types(`macro_make_vector_new')

define(`macro_make_vector_from_array',
`macro_make_vector_from_array_prototype($1) {
    macro_vector_type($1) *vector = macro_tagged_function_name(vector_new, $1)(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *macro_tagged_function_name(vector_element, $1)(i, vector) = elements[i];
    }

    return vector;
}')

macro_make_for_numeric_types(`macro_make_vector_from_array')

define(`macro_make_vector_duplicate',
`macro_make_vector_duplicate_prototype($1) {
    size_t vector_length = macro_tagged_function_name(vector_length, $1)(vector);
    macro_vector_type($1) *new_vector = macro_tagged_function_name(vector_new, $1)(vector_length);
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

macro_make_for_numeric_types(`macro_make_vector_duplicate')

define(`macro_make_vector_length',
`macro_make_vector_length_prototype($1) {
    return buf->n_elements;
}')

macro_make_for_numeric_types(`macro_make_vector_length')

define(`macro_make_vector_reverse',
`macro_make_vector_reverse_prototype($1) {
    vector->is_reversed = ! vector->is_reversed;
}')

macro_make_for_numeric_types(`macro_make_vector_reverse')

define(`macro_make_vector_reset',
`macro_make_vector_reset_prototype($1) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}')

macro_make_for_numeric_types(`macro_make_vector_reset')

define(`macro_make_vector_free',
`macro_make_vector_free_prototype($1) {
    free(buf);
}')

macro_make_for_numeric_types(`macro_make_vector_free')

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