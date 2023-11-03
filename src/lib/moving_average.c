#include "moving_average.h"
#include "assertions.h"

/**
 * @brief 
 * Makes and allocates a simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverage *filter_make_moving_average(size_t length) {
    MovingAverage *filter = malloc(sizeof(MovingAverage));
    
    if (filter == NULL) {
        return NULL;
    }

    filter->previous_input = circbuf_complex_new(length);
    if (filter->previous_input == NULL) {
        free(filter);
        return NULL;
    }

    filter->moving_sum = 0;

    return filter;
}

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
) {
    assert_not_null(filter);

    filter->moving_sum += input;
    filter->moving_sum -= circbuf_complex_shift(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements;
}

/**
 * @brief 
 * Resets moving average filter to initial state
 * @param filter Filter to reset
 */
void filter_reset_moving_average(MovingAverage *filter) {
    assert_not_null(filter);

    circbuf_complex_reset(filter->previous_input);
    filter->moving_sum = 0;
}

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void filter_free_moving_average(MovingAverage *filter) {
    assert_not_null(filter);

    free(filter->previous_input);
    free(filter);
}
