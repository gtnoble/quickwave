#include <complex.h>
#include "buffer.h"

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double complex moving_sum;
    CircularBufferComplex *previous_input;
} MovingAverage;

MovingAverage *filter_make_moving_average(size_t length);
double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
);
void filter_reset_moving_average(MovingAverage *filter);
void filter_free_moving_average(MovingAverage *filter);
