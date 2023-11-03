#ifndef SQLDSP_BUFFER
#define SQLDSP_BUFFER

#include <stdlib.h>
#include <complex.h>

/**
 * @brief 
 * Circular buffer. Stores the n_elements most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements;
    size_t index;
    double complex buffer[];
} CircularBufferComplex;

/**
 * @brief 
 * Circular buffer. Stores the n_elements most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements;
    size_t index;
    double buffer[];
} CircularBufferReal;

double complex circbuf_complex_shift(double complex element, CircularBufferComplex *buf);
double complex *circbuf_complex_element(int index, CircularBufferComplex *buf);
double complex circbuf_complex_interpolated_element(double index, CircularBufferComplex *buf);
CircularBufferComplex *circbuf_complex_new(size_t size);
void circbuf_complex_reset(CircularBufferComplex *buf);
void circbuf_complex_free(CircularBufferComplex *buf);

double circbuf_real_shift(double element, CircularBufferReal *buf);
double *circbuf_real_element(int index, CircularBufferReal *buf);
double circbuf_real_interpolated_element(double index, CircularBufferReal *buf);
CircularBufferReal *circbuf_real_new(size_t size);
void circbuf_real_reset(CircularBufferReal *buf);
void circbuf_real_free(CircularBufferReal *buf);

int modular_add(int a, int b, int max);
int modulo_euclidean(int a, int b);

#endif