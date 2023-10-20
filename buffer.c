#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "buffer.h"

double complex circbuf_shift(double complex element, CircularBuffer *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double complex last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

double complex *circbuf_element(int index, CircularBuffer *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

double complex circbuf_interpolated_element(double index, CircularBuffer *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_element((int) floor(index), buf) * fraction_between_elements +
        *circbuf_element((int) ceil(index), buf) * (1 - fraction_between_elements);
}

CircularBuffer *circbuf_new(size_t size) {
    CircularBuffer *circbuf = malloc(sizeof(CircularBuffer));
    if (circbuf == NULL) 
        return NULL;

    circbuf->buffer = malloc(sizeof(double complex) * size);
    if (circbuf->buffer == NULL) {
        free(circbuf);
        return NULL;
    }

    circbuf->n_elements = size;
    circbuf->index = 0;

    for (size_t ii = 0; ii < size; ii++) {
        circbuf->buffer[ii] = 0.0;
    }
    return circbuf;
}

void circbuf_free(CircularBuffer *buf) {
    if (buf == NULL)
        return;
    free(buf->buffer);
    free(buf);
}

int modular_add(int a, int b, int max) {
    int sum = a + b;
    return modulo_Euclidean(sum, max);
}

int modulo_Euclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}