#include "moving_average.h"
#include "assertions.h"

/**
 * @brief 
 * Makes and allocates a simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverage *moving_average_make(size_t length) {
    MovingAverage *filter = malloc(sizeof(MovingAverage));
    
    if (filter == NULL) {
        return NULL;
    }

    filter->previous_input = circbuf_real_new(length);
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
double moving_average_evaluate(
    double input, 
    MovingAverage *filter
) {
    assert_not_null(filter);

    filter->moving_sum += input;
    filter->moving_sum -= circbuf_real_shift(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements;
}

/**
 * @brief 
 * Resets moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset(MovingAverage *filter) {
    assert_not_null(filter);

    circbuf_real_reset(filter->previous_input);
    filter->moving_sum = 0;
}

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free(MovingAverage *filter) {
    assert_not_null(filter);

    free(filter->previous_input);
    free(filter);
}
