#include <stdlib.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3
#include "buffer.h"

double circbuf_shift(double element, CircularBuffer *buf) {
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

double *circbuf_element(int index, CircularBuffer *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

CircularBuffer *circbuf_new(size_t size) {
    CircularBuffer *circbuf = sqlite3_malloc(sizeof(CircularBuffer));
    if (circbuf == NULL) 
        return NULL;

    circbuf->buffer = sqlite3_malloc(sizeof(double) * size);
    if (circbuf->buffer == NULL) {
        sqlite3_free(circbuf);
        return NULL;
    }

    circbuf->n_elements = size;
    circbuf->index = 0;

    for (int ii = 0; ii < size; ii++) {
        circbuf->buffer[ii] = 0.0;
    }
    return circbuf;
}

void circbuf_free(CircularBuffer *buf) {
    if (buf == NULL)
        return;
    sqlite3_free(buf->buffer);
    sqlite3_free(buf);
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