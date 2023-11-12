#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

double complex vector_complex_shift(double complex element, VectorComplex *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

double vector_real_shift(double element, VectorReal *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

double complex *vector_complex_element(int index, VectorComplex *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

double *vector_real_element(int index, VectorReal *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

double complex vector_complex_interpolated_element(double index, VectorComplex *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *vector_element_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        *vector_element_generic((int) ceil(index), buf) * fraction_between_elements;
}

double vector_real_interpolated_element(double index, VectorReal *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *vector_element_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        *vector_element_generic((int) ceil(index), buf) * fraction_between_elements;
}

double vector_real_dot(VectorReal *a, VectorReal *b) {
    assert(vector_length_generic(a) == vector_length_generic(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_generic(a); i++) {
        sum += *vector_element_generic(i, a) * *vector_element_generic(i, b);
    }
    return sum;
}

double complex vector_complex_dot(VectorComplex *a, VectorComplex *b) {
    assert(vector_length_generic(a) == vector_length_generic(b));
    double complex sum = 0;
    for (size_t i = 0; i < vector_length_generic(a); i++) {
        sum += *vector_element_generic(i, a) * *vector_element_generic(i, b);
    }
    return sum;
}

VectorComplex *vector_complex_new(size_t size) {
    VectorComplex *circbuf = malloc(
        sizeof(VectorComplex) + sizeof(double complex) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_generic(circbuf);
    return circbuf;
}

VectorReal *vector_real_new(size_t size) {
    VectorReal *circbuf = malloc(
        sizeof(VectorReal) + sizeof(double) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_generic(circbuf);
    return circbuf;
}

VectorComplex *vector_complex_duplicate(const VectorComplex *vector) {
    size_t vector_length = vector_length_generic(vector);
    VectorComplex *new_vector = vector_complex_new(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(double complex) * vector_length
    );
    return new_vector;
}

VectorReal *vector_real_duplicate(const VectorReal *vector) {
    size_t vector_length = vector_length_generic(vector);
    VectorReal *new_vector = vector_real_new(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(double) * vector_length
    );
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

void vector_complex_reset(VectorComplex *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_real_reset(VectorReal *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
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