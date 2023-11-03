#include "moving_average.h"
#include "assertions.h"

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

double moving_average_evaluate(
    double input, 
    MovingAverage *filter
) {
    assert_not_null(filter);

    filter->moving_sum += input;
    filter->moving_sum -= circbuf_real_shift(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements;
}

void moving_average_reset(MovingAverage *filter) {
    assert_not_null(filter);

    circbuf_real_reset(filter->previous_input);
    filter->moving_sum = 0;
}

void moving_average_free(MovingAverage *filter) {
    assert_not_null(filter);

    free(filter->previous_input);
    free(filter);
}
