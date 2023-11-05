#ifndef QUICKWAVE_MOVING_AVERAGE
#define QUICKWAVE_MOVING_AVERAGE

#include <complex.h>
#include "buffer.h"

/**
 * @brief 
 * Simple real-valued moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double moving_sum;
    CircularBufferReal *previous_input;
} MovingAverageReal;

/**
 * @brief 
 * Simple complex-valued moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double complex moving_sum;
    CircularBufferComplex *previous_input;
} MovingAverageComplex;

/**
 * @brief 
 * Makes and allocates a real-valued simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageReal *moving_average_real_make(size_t length);

/**
 * @brief 
 * Makes and allocates a complex-valued simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageComplex *moving_average_complex_make(size_t length);

/**
 * @brief 
 * Evaluates a real-valued moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double moving_average_real_evaluate(
    double input, 
    MovingAverageReal *filter
);

/**
 * @brief 
 * Evaluates a complex-valued moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double complex moving_average_complex_evaluate(
    double complex input, 
    MovingAverageComplex *filter
);

/**
 * @brief 
 * Resets real-valued moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_real_reset(MovingAverageReal *filter);

/**
 * @brief 
 * Resets complex-valued moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_complex_reset(MovingAverageComplex *filter);

/**
 * @brief 
 * Frees the memory assicated with a real-valued moving average filter
 * @param filter Filter to be freed
 */
void moving_average_real_free(MovingAverageReal *filter);

/**
 * @brief 
 * Frees the memory assicated with a complex-valued moving average filter
 * @param filter Filter to be freed
 */
void moving_average_complex_free(MovingAverageComplex *filter);

#endif