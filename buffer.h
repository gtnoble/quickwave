#include <stdlib.h>

typedef struct {
    double *buffer;
    size_t n_elements;
    size_t index;
} CircularBuffer;

double circbuf_shift(double element, CircularBuffer *buf);
double *circbuf_element(int index, CircularBuffer *buf);
CircularBuffer *circbuf_new(size_t size);
void circbuf_free(CircularBuffer *buf);

int modular_add(int a, int b, int max);
int modulo_Euclidean(int a, int b);