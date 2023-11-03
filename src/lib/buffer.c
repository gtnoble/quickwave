#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "buffer.h"

double complex circbuf_complex_shift(double complex element, CircularBufferComplex *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double complex last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

double circbuf_real_shift(double element, CircularBufferReal *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

double complex *circbuf_complex_element(int index, CircularBufferComplex *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

double *circbuf_real_element(int index, CircularBufferReal *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

double complex circbuf_complex_interpolated_element(double index, CircularBufferComplex *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_complex_element((int) floor(index), buf) * (1 - fraction_between_elements) +
        *circbuf_complex_element((int) ceil(index), buf) * fraction_between_elements;
}

double circbuf_real_interpolated_element(double index, CircularBufferReal *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_real_element((int) floor(index), buf) * (1 - fraction_between_elements) +
        *circbuf_real_element((int) ceil(index), buf) * fraction_between_elements;
}

CircularBufferComplex *circbuf_complex_new(size_t size) {
    CircularBufferComplex *circbuf = malloc(
        sizeof(CircularBufferComplex) + sizeof(double complex) * size);
    if (circbuf == NULL) 
        return NULL;

    circbuf->n_elements = size;

    circbuf_complex_reset(circbuf);
    return circbuf;
}

CircularBufferReal *circbuf_real_new(size_t size) {
    CircularBufferReal *circbuf = malloc(
        sizeof(CircularBufferReal) + sizeof(double) * size);
    if (circbuf == NULL) 
        return NULL;

    circbuf->n_elements = size;

    circbuf_real_reset(circbuf);
    return circbuf;
}

void circbuf_complex_reset(CircularBufferComplex *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->buffer[ii] = 0.0;
    }
    buf->index = 0;
}

void circbuf_real_reset(CircularBufferReal *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->buffer[ii] = 0.0;
    }
    buf->index = 0;
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