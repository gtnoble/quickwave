#ifndef QUICKWAVE_MOVING_AVERAGE
#define QUICKWAVE_MOVING_AVERAGE

#include <complex.h>
#include "buffer.h"

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double moving_sum;
    CircularBufferReal *previous_input;
} MovingAverage;

MovingAverage *moving_average_make(size_t length);
double moving_average_evaluate(
    double input, 
    MovingAverage *filter
);
void moving_average_reset(MovingAverage *filter);
void moving_average_free(MovingAverage *filter);

#endif