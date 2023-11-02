#ifndef QUICKWAVE_WINDOW
#define QUICKWAVE_WINDOW

#include <stddef.h>

typedef double (WindowFunction)(size_t element_index, size_t window_size);

double window_rectangular(size_t element_index, size_t window_size);
double window_hamming(size_t element_index, size_t window_size);

#endif