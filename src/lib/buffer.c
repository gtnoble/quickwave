#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "buffer.h"

#define MAKE_CIRCBUF_SHIFT(element_type) \
    assert(buf != NULL); \
    assert(buf->buffer != NULL); \
    buf->index = modular_add(buf->index, 1, buf->n_elements); \
    element_type last_element = buf->buffer[buf->index]; \
    buf->buffer[buf->index] = element; \
    return last_element;


double complex circbuf_complex_shift(double complex element, CircularBufferComplex *buf) {
    MAKE_CIRCBUF_SHIFT(double complex)
}

double circbuf_real_shift(double element, CircularBufferReal *buf) {
    MAKE_CIRCBUF_SHIFT(double)
}

#define MAKE_CIRCBUF_ELEMENT \
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];

double complex *circbuf_complex_element(int index, CircularBufferComplex *buf) {
    MAKE_CIRCBUF_ELEMENT
}

double *circbuf_real_element(int index, CircularBufferReal *buf) {
    MAKE_CIRCBUF_ELEMENT
}

#define MAKE_CIRCBUF_INTERPOLATED_ELEMENT(circbuf_element_getter) \
    double fraction_between_elements = index - floor(index); \
    return  \
        *circbuf_element_getter((int) floor(index), buf) * (1 - fraction_between_elements) + \
        *circbuf_element_getter((int) ceil(index), buf) * fraction_between_elements;

double complex circbuf_complex_interpolated_element(double index, CircularBufferComplex *buf) {
    MAKE_CIRCBUF_INTERPOLATED_ELEMENT(circbuf_complex_element)
}

double circbuf_real_interpolated_element(double index, CircularBufferReal *buf) {
    MAKE_CIRCBUF_INTERPOLATED_ELEMENT(circbuf_real_element)
}

#define MAKE_CIRCBUF_NEW(buffer_type, element_type, buffer_resetter) \
    buffer_type *circbuf = malloc( \
        sizeof(buffer_type) + sizeof(element_type) * size); \
    if (circbuf == NULL)  \
        return NULL; \
    \
    circbuf->n_elements = size; \
    \
    buffer_resetter(circbuf); \
    return circbuf;


CircularBufferComplex *circbuf_complex_new(size_t size) {
    MAKE_CIRCBUF_NEW(CircularBufferComplex, double complex, circbuf_complex_reset)
}

CircularBufferReal *circbuf_real_new(size_t size) {
    MAKE_CIRCBUF_NEW(CircularBufferReal, double, circbuf_real_reset)
}


size_t circbuf_complex_length(CircularBufferComplex *buf) {
    return buf->n_elements;
}

#define MAKE_CIRCBUF_RESET \
    if (buf == NULL) \
        return; \
    for (size_t ii = 0; ii < buf->n_elements; ii++) { \
        buf->buffer[ii] = 0.0; \
    } \
    buf->index = 0;


void circbuf_complex_reset(CircularBufferComplex *buf) {
    MAKE_CIRCBUF_RESET
}

void circbuf_real_reset(CircularBufferReal *buf) {
    MAKE_CIRCBUF_RESET
}

void circbuf_complex_free(CircularBufferComplex *buf) {
    free(buf);
}

void circbuf_real_free(CircularBufferReal *buf) {
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