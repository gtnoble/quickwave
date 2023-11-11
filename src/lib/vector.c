#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

#define MAKE_VECTOR_SHIFT(element_type) \
    assert(buf != NULL); \
    assert(buf->elements != NULL); \
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements); \
    element_type last_element = buf->elements[buf->last_element_index]; \
    buf->elements[buf->last_element_index] = element; \
    return last_element;


double complex vector_complex_shift(double complex element, VectorComplex *buf) {
    MAKE_VECTOR_SHIFT(double complex)
}

double vector_real_shift(double element, VectorReal *buf) {
    MAKE_VECTOR_SHIFT(double)
}

#define MAKE_VECTOR_ELEMENT \
    return &buf->elements[ \
        modular_add( \
            index * (buf->is_reversed ? -1 : 1),  \
            buf->last_element_index + ! buf->is_reversed, \
            buf->n_elements) \
    ];

double complex *vector_complex_element(int index, VectorComplex *buf) {
    MAKE_VECTOR_ELEMENT
}

double *vector_real_element(int index, VectorReal *buf) {
    MAKE_VECTOR_ELEMENT
}

#define MAKE_VECTOR_INTERPOLATED_ELEMENT(circbuf_element_getter) \
    double fraction_between_elements = index - floor(index); \
    return  \
        *circbuf_element_getter((int) floor(index), buf) * (1 - fraction_between_elements) + \
        *circbuf_element_getter((int) ceil(index), buf) * fraction_between_elements;

double complex vector_complex_interpolated_element(double index, VectorComplex *buf) {
    MAKE_VECTOR_INTERPOLATED_ELEMENT(vector_complex_element)
}

double vector_real_interpolated_element(double index, VectorReal *buf) {
    MAKE_VECTOR_INTERPOLATED_ELEMENT(vector_real_element)
}

#define MAKE_VECTOR_DOT(vector_length_getter, vector_element_getter, element_type) \
    assert(vector_length_getter(a) == vector_length_getter(b)); \
    element_type sum = 0; \
    for (size_t i = 0; i < vector_length_getter(a); i++) { \
        sum += *vector_element_getter(i, a) * *vector_element_getter(i, b); \
    } \
    return sum;


double vector_real_dot(VectorReal *a, VectorReal *b) {
    MAKE_VECTOR_DOT(vector_real_length, vector_real_element, double)
}

double complex vector_complex_dot(VectorComplex *a, VectorComplex *b) {
    MAKE_VECTOR_DOT(vector_complex_length, vector_complex_element, double complex)
}

#define MAKE_VECTOR_NEW(buffer_type, element_type, buffer_resetter) \
    buffer_type *circbuf = malloc( \
        sizeof(buffer_type) + sizeof(element_type) * size); \
    if (circbuf == NULL)  \
        return NULL; \
    \
    circbuf->n_elements = size; \
    \
    buffer_resetter(circbuf); \
    return circbuf;


VectorComplex *vector_complex_new(size_t size) {
    MAKE_VECTOR_NEW(VectorComplex, double complex, vector_complex_reset)
}

VectorReal *vector_real_new(size_t size) {
    MAKE_VECTOR_NEW(VectorReal, double, vector_real_reset)
}

VectorComplex *vector_complex_duplicate(const VectorComplex *vector) {
    size_t vector_length = vector_complex_length(vector);
    VectorComplex *new_vector = vector_complex_new(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(new_vector->elements, vector->elements, vector_length);
    return new_vector;
}

VectorReal *vector_real_duplicate(const VectorReal *vector) {
    size_t vector_length = vector_real_length(vector);
    VectorReal *new_vector = vector_real_new(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(new_vector->elements, vector->elements, vector_length);
    return new_vector;
}


size_t vector_complex_length(const VectorComplex *buf) {
    return buf->n_elements;
}

size_t vector_real_length(const VectorReal *buf) {
    return buf->n_elements;
}

void vector_complex_reverse(VectorComplex *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_real_reverse(VectorReal *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

#define MAKE_VECTOR_RESET \
    if (buf == NULL) \
        return; \
    for (size_t ii = 0; ii < buf->n_elements; ii++) { \
        buf->elements[ii] = 0.0; \
    } \
    buf->last_element_index = 0; \
    buf->is_reversed = false;


void vector_complex_reset(VectorComplex *buf) {
    MAKE_VECTOR_RESET
}

void vector_real_reset(VectorReal *buf) {
    MAKE_VECTOR_RESET
}

void vector_complex_free(VectorComplex *buf) {
    free(buf);
}

void vector_real_free(VectorReal *buf) {
    free(buf);
}

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