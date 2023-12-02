
#include "moving_average.h"
#include "assertions.h"

MovingAverageComplexDouble *moving_average_make_complex_double(size_t length) {
    MovingAverageComplexDouble *filter = malloc(sizeof(MovingAverageComplexDouble)); 
    
    if (filter == NULL) { 
        return NULL; 
    } 
    
    filter->previous_input = vector_new_complex_double(length); 
    if (filter->previous_input == NULL) { 
        free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

double complex moving_average_evaluate_complex_double(
    double complex input, 
    MovingAverageComplexDouble *filter
) {
    assert_not_null(filter); 
    
    filter->moving_sum += input; 
    filter->moving_sum -= vector_shift_complex_double(input, filter->previous_input); 
    return filter->moving_sum / vector_length_complex_double(filter->previous_input); 
}

void moving_average_complex_reset_complex_double(MovingAverageComplexDouble *filter) {
    assert_not_null(filter); 
    
    vector_reset_complex_double(filter->previous_input); 
    filter->moving_sum = 0; 
}

void moving_average_free_complex_double(MovingAverageComplexDouble *filter) {
    assert_not_null(filter); 
    
    vector_free_complex_double(filter->previous_input); 
    free(filter); 
}



MovingAverageComplexFloat *moving_average_make_complex_float(size_t length) {
    MovingAverageComplexFloat *filter = malloc(sizeof(MovingAverageComplexFloat)); 
    
    if (filter == NULL) { 
        return NULL; 
    } 
    
    filter->previous_input = vector_new_complex_float(length); 
    if (filter->previous_input == NULL) { 
        free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

double complex moving_average_evaluate_complex_float(
    float complex input, 
    MovingAverageComplexFloat *filter
) {
    assert_not_null(filter); 
    
    filter->moving_sum += input; 
    filter->moving_sum -= vector_shift_complex_float(input, filter->previous_input); 
    return filter->moving_sum / vector_length_complex_float(filter->previous_input); 
}

void moving_average_complex_reset_complex_float(MovingAverageComplexFloat *filter) {
    assert_not_null(filter); 
    
    vector_reset_complex_float(filter->previous_input); 
    filter->moving_sum = 0; 
}

void moving_average_free_complex_float(MovingAverageComplexFloat *filter) {
    assert_not_null(filter); 
    
    vector_free_complex_float(filter->previous_input); 
    free(filter); 
}



MovingAverageRealDouble *moving_average_make_real_double(size_t length) {
    MovingAverageRealDouble *filter = malloc(sizeof(MovingAverageRealDouble)); 
    
    if (filter == NULL) { 
        return NULL; 
    } 
    
    filter->previous_input = vector_new_real_double(length); 
    if (filter->previous_input == NULL) { 
        free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

double complex moving_average_evaluate_real_double(
    double input, 
    MovingAverageRealDouble *filter
) {
    assert_not_null(filter); 
    
    filter->moving_sum += input; 
    filter->moving_sum -= vector_shift_real_double(input, filter->previous_input); 
    return filter->moving_sum / vector_length_real_double(filter->previous_input); 
}

void moving_average_complex_reset_real_double(MovingAverageRealDouble *filter) {
    assert_not_null(filter); 
    
    vector_reset_real_double(filter->previous_input); 
    filter->moving_sum = 0; 
}

void moving_average_free_real_double(MovingAverageRealDouble *filter) {
    assert_not_null(filter); 
    
    vector_free_real_double(filter->previous_input); 
    free(filter); 
}



MovingAverageRealFloat *moving_average_make_real_float(size_t length) {
    MovingAverageRealFloat *filter = malloc(sizeof(MovingAverageRealFloat)); 
    
    if (filter == NULL) { 
        return NULL; 
    } 
    
    filter->previous_input = vector_new_real_float(length); 
    if (filter->previous_input == NULL) { 
        free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

double complex moving_average_evaluate_real_float(
    float input, 
    MovingAverageRealFloat *filter
) {
    assert_not_null(filter); 
    
    filter->moving_sum += input; 
    filter->moving_sum -= vector_shift_real_float(input, filter->previous_input); 
    return filter->moving_sum / vector_length_real_float(filter->previous_input); 
}

void moving_average_complex_reset_real_float(MovingAverageRealFloat *filter) {
    assert_not_null(filter); 
    
    vector_reset_real_float(filter->previous_input); 
    filter->moving_sum = 0; 
}

void moving_average_free_real_float(MovingAverageRealFloat *filter) {
    assert_not_null(filter); 
    
    vector_free_real_float(filter->previous_input); 
    free(filter); 
}
