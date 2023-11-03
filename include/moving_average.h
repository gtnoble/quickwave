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

/**
 * @brief 
 * Makes and allocates a simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverage *moving_average_make(size_t length);

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double moving_average_evaluate(
    double input, 
    MovingAverage *filter
);

/**
 * @brief 
 * Resets moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset(MovingAverage *filter);

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free(MovingAverage *filter);

#endif