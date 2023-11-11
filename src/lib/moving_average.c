#include "moving_average.h"
#include "assertions.h"

#define MOVING_AVERAGE_MAKE(moving_average_type, circbuf_constructor) \
    moving_average_type *filter = malloc(sizeof(moving_average_type)); \
    \
    if (filter == NULL) { \
        return NULL; \
    } \
    \
    filter->previous_input = circbuf_constructor(length); \
    if (filter->previous_input == NULL) { \
        free(filter); \
        return NULL; \
    } \
    \
    filter->moving_sum = 0; \
    \
    return filter;


MovingAverageReal *moving_average_real_make(size_t length) {
    MOVING_AVERAGE_MAKE(MovingAverageReal, vector_real_new)
}

MovingAverageComplex *moving_average_complex_make(size_t length) {
    MOVING_AVERAGE_MAKE(MovingAverageComplex, vector_complex_new)
}

size_t moving_average_complex_length(MovingAverageComplex *filter) {
    return vector_complex_length(filter->previous_input);
}

#define MOVING_AVERAGE_EVALUATE(circbuf_shifter) \
    assert_not_null(filter); \
    \
    filter->moving_sum += input; \
    filter->moving_sum -= circbuf_shifter(input, filter->previous_input); \
    return filter->moving_sum / filter->previous_input->n_elements; 

double moving_average_real_evaluate(
    double input, 
    MovingAverageReal *filter
) {
    MOVING_AVERAGE_EVALUATE(vector_real_shift)
}

double complex moving_average_complex_evaluate(
    double complex input, 
    MovingAverageComplex *filter
) {
    MOVING_AVERAGE_EVALUATE(vector_complex_shift)
}

#define MOVING_AVERAGE_RESET(circbuf_resetter) \
    assert_not_null(filter); \
    \
    circbuf_resetter(filter->previous_input); \
    filter->moving_sum = 0; 


void moving_average_real_reset(MovingAverageReal *filter) {
    MOVING_AVERAGE_RESET(vector_real_reset)
}

void moving_average_complex_reset(MovingAverageComplex *filter) {
    MOVING_AVERAGE_RESET(vector_complex_reset)
}

#define MOVING_AVERAGE_FREE \
    assert_not_null(filter); \
    \
    free(filter->previous_input); \
    free(filter); 


void moving_average_real_free(MovingAverageReal *filter) {
    MOVING_AVERAGE_FREE
}

void moving_average_complex_free(MovingAverageComplex *filter) {
    MOVING_AVERAGE_FREE
}
