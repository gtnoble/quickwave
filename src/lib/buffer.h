#ifndef SQLDSP_BUFFER
#define SQLDSP_BUFFER

#include <stdlib.h>
#include <complex.h>

typedef struct {
    double complex *buffer;
    size_t n_elements;
    size_t index;
} CircularBuffer;

double complex circbuf_shift(double complex element, CircularBuffer *buf);
double complex *circbuf_element(int index, CircularBuffer *buf);
double complex circbuf_interpolated_element(double index, CircularBuffer *buf);
CircularBuffer *circbuf_new(size_t size);
void circbuf_reset(CircularBuffer *buf);
void circbuf_free(CircularBuffer *buf);

int modular_add(int a, int b, int max);
int modulo_Euclidean(int a, int b);

#endif