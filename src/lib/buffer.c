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
 * @return Last element in the buffer, before being overwritten
 */
double complex circbuf_complex_shift(double complex element, CircularBufferComplex *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double complex last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double circbuf_real_shift(double element, CircularBufferReal *buf) {
    assert(buf != NULL);
    assert(buf->buffer != NULL);
    buf->index = modular_add(buf->index, 1, buf->n_elements);
    double last_element = buf->buffer[buf->index];
    buf->buffer[buf->index] = element;
    return last_element;
}

/**
 * @brief 
 * Returns element at index. Negative indices represent previously inserted elements.
 * @param index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double complex *circbuf_complex_element(int index, CircularBufferComplex *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

/**
 * @brief 
 * Returns element at index. Negative indices represent previously inserted elements.
 * @param index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double *circbuf_real_element(int index, CircularBufferReal *buf) {
    return &buf->buffer[modular_add(index, buf->index, buf->n_elements)];
}

/**
 * @brief 
 * Interpolates values of a circular buffer
 * @param index Generalized index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double complex circbuf_complex_interpolated_element(double index, CircularBufferComplex *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_complex_element((int) floor(index), buf) * (1 - fraction_between_elements) +
        *circbuf_complex_element((int) ceil(index), buf) * fraction_between_elements;
}

/**
 * @brief 
 * Interpolates values of a circular buffer
 * @param index Generalized index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double circbuf_real_interpolated_element(double index, CircularBufferReal *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        *circbuf_real_element((int) floor(index), buf) * (1 - fraction_between_elements) +
        *circbuf_real_element((int) ceil(index), buf) * fraction_between_elements;
}

/**
 * @brief 
 * Makes and allocates a new circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
CircularBufferComplex *circbuf_complex_new(size_t size) {
    CircularBufferComplex *circbuf = malloc(
        sizeof(CircularBufferComplex) + sizeof(double complex) * size);
    if (circbuf == NULL) 
        return NULL;

    circbuf->n_elements = size;

    circbuf_complex_reset(circbuf);
    return circbuf;
}

/**
 * @brief 
 * Makes and allocates a new circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
CircularBufferReal *circbuf_real_new(size_t size) {
    CircularBufferReal *circbuf = malloc(
        sizeof(CircularBufferReal) + sizeof(double) * size);
    if (circbuf == NULL) 
        return NULL;

    circbuf->n_elements = size;

    circbuf_real_reset(circbuf);
    return circbuf;
}

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void circbuf_complex_reset(CircularBufferComplex *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->buffer[ii] = 0.0;
    }
    buf->index = 0;
}

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void circbuf_real_reset(CircularBufferReal *buf) {
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
void circbuf_complex_free(CircularBufferComplex *buf) {
    free(buf);
}

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void circbuf_real_free(CircularBufferReal *buf) {
    free(buf);
}

/**
 * @brief 
 * Performs Euclidean modular addition
 * @param a Summand
 * @param b Summand
 * @param max Divisor
 * @return Result
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
 * @return Remainder
 */
int modulo_euclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}