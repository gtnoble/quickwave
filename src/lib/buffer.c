#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "buffer.h"

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return double Last element in the buffer, before being overwritten
 */
double complex circbuf_shift(double complex element, CircularBuffer *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double complex last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

/**
 * @brief 
 * Returns element at index. Negative indices represent previously inserted elements.
 * @param index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return double* Indexed element
 */
double complex *circbuf_element(int index, CircularBuffer *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

/**
 * @brief 
 * Interpolates values of a circular buffer
 * @param index Generalized index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return double Interpolated element
 */
double complex circbuf_interpolated_element(double index, CircularBuffer *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_element((int) floor(index), buf) * (1 - fraction_between_elements) +
        *circbuf_element((int) ceil(index), buf) * fraction_between_elements;
}

/**
 * @brief 
 * Makes and allocates a new circular buffer
 * @param size Number of elements in the circular buffer
 * @return CircularBuffer* 
 */
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

    circbuf_reset(circbuf);
    return circbuf;
}

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void circbuf_reset(CircularBuffer *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->buffer[ii] = 0.0;
    }
    buf->index = 0;
}

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void circbuf_free(CircularBuffer *buf) {
    if (buf == NULL)
        return;
    free(buf->buffer);
    free(buf);
}

/**
 * @brief 
 * Performs Euclidean modular addition
 * @param a Summand
 * @param b Summand
 * @param max Divisor
 * @return int 
 */
int modular_add(int a, int b, int max) {
    int sum = a + b;
    return modulo_euclidean(sum, max);
}

/**
 * @brief 
 * Performs Euclidean modulus
 * @param a Dividend
 * @param b Divisor
 * @return int Remainder
 */
int modulo_euclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}