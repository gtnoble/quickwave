
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "savgol.h"
#include "filter.h"
#include "constants.h"
#include "assertions.h"


/**
 * @brief 
 * Normalized sinc function
 * @param x input
 * @return result
 */
double sinc_double(double x);

/**
 * @brief 
 * Dirac delta function
 * @param x Input
 * @return Result 
 */
double dirac_delta_double(double x);

double sinc_double(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

double dirac_delta_double(double x) {
    return x == 0.0 ? 1.0 : 0.0;
}



/**
 * @brief 
 * Normalized sinc function
 * @param x input
 * @return result
 */
float sinc_float(float x);

/**
 * @brief 
 * Dirac delta function
 * @param x Input
 * @return Result 
 */
float dirac_delta_float(float x);

float sinc_float(float x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

float dirac_delta_float(float x) {
    return x == 0.0 ? 1.0 : 0.0;
}


double complex filter_evaluate_digital_filter_complex_double(double complex input, DigitalFilterComplexDouble *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double complex accumulate = 0.0;
    vector_shift_complex_double(input, filter->previous_input);
    accumulate +=
        vector_dot_complex_double(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += vector_dot_complex_double(filter->feedback, filter->previous_output);
        vector_shift_complex_double(accumulate, filter->previous_output);
    }
    return accumulate;
}

DigitalFilterComplexDouble *filter_make_digital_filter_complex_double(
    const VectorComplexDouble *feedforward,
    const VectorComplexDouble *feedback
) {
    DigitalFilterComplexDouble *filter = malloc(sizeof(DigitalFilterComplexDouble));
    if (filter == NULL)
        return NULL;
    
    filter->feedforward = vector_duplicate_complex_double(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_complex_double(vector_length_complex_double(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        filter->feedback = vector_duplicate_complex_double(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_new_complex_double(vector_length_complex_double(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        vector_free_complex_double(filter->feedback);
    fail_allocate_feedback:
        vector_free_complex_double(filter->previous_input);
    fail_allocate_previous_input:
        vector_free_complex_double(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}


void filter_reset_digital_filter_complex_double(DigitalFilterComplexDouble *filter) {
    assert_not_null(filter);
    
    vector_reset_complex_double(filter->previous_input);
    if (filter->previous_output)
        vector_reset_complex_double(filter->previous_output);
}


void filter_free_digital_filter_complex_double(DigitalFilterComplexDouble *filter) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    vector_free_complex_double(filter->feedforward);

    assert_not_null(filter->previous_input);
    vector_free_complex_double(filter->previous_input);

    if (filter->feedback != NULL) {
        vector_free_complex_double(filter->feedback);
        assert_not_null(filter->previous_output);
        vector_free_complex_double(filter->previous_output);
    }
    free(filter);
}

DigitalFilterComplexDouble *filter_make_savgol_complex_double(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    VectorComplexDouble *feedforward = vector_new_complex_double(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *vector_element_complex_double(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    DigitalFilterComplexDouble *filter = filter_make_digital_filter_complex_double(feedforward, NULL);
    vector_free_complex_double(feedforward);
    return filter;
}

DigitalFilterComplexDouble *filter_make_ewma_complex_double(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    VectorComplexDouble *feedforward = vector_new_complex_double(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_element_complex_double(0, feedforward) = alpha;

    VectorComplexDouble *feedback = vector_new_complex_double(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_element_complex_double(0, feedback) = 1 - alpha;

    DigitalFilterComplexDouble *filter = 
        filter_make_digital_filter_complex_double(feedforward, feedback);

    vector_free_complex_double(feedforward);
    vector_free_complex_double(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_free_complex_double(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

DigitalFilterComplexDouble *filter_make_first_order_iir_complex_double(double cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    double angular_frequency = ordinary_frequency_to_angular_double(cutoff_frequency);
    return filter_make_ewma_complex_double(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

DigitalFilterComplexDouble *filter_make_sinc_complex_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionDouble window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular_double;

    VectorComplexDouble *filter_coefficients = vector_new_complex_double(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    double kernel_shift = ((int) length - 1) / 2.0;
    double complex uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_element_complex_double(i, filter_coefficients) = 
            sinc_double(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_element_complex_double(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        double complex dc_corrected_coefficient = 
            *vector_element_complex_double(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_element_complex_double(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta_double(i - length / 2) - dc_corrected_coefficient;
    }
    DigitalFilterComplexDouble *filter = 
        filter_make_digital_filter_complex_double(filter_coefficients, NULL);
    
    vector_free_complex_double(filter_coefficients);

    return filter;
}





float complex filter_evaluate_digital_filter_complex_float(float complex input, DigitalFilterComplexFloat *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    float complex accumulate = 0.0;
    vector_shift_complex_float(input, filter->previous_input);
    accumulate +=
        vector_dot_complex_float(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += vector_dot_complex_float(filter->feedback, filter->previous_output);
        vector_shift_complex_float(accumulate, filter->previous_output);
    }
    return accumulate;
}

DigitalFilterComplexFloat *filter_make_digital_filter_complex_float(
    const VectorComplexFloat *feedforward,
    const VectorComplexFloat *feedback
) {
    DigitalFilterComplexFloat *filter = malloc(sizeof(DigitalFilterComplexFloat));
    if (filter == NULL)
        return NULL;
    
    filter->feedforward = vector_duplicate_complex_float(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_complex_float(vector_length_complex_float(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        filter->feedback = vector_duplicate_complex_float(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_new_complex_float(vector_length_complex_float(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        vector_free_complex_float(filter->feedback);
    fail_allocate_feedback:
        vector_free_complex_float(filter->previous_input);
    fail_allocate_previous_input:
        vector_free_complex_float(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}


void filter_reset_digital_filter_complex_float(DigitalFilterComplexFloat *filter) {
    assert_not_null(filter);
    
    vector_reset_complex_float(filter->previous_input);
    if (filter->previous_output)
        vector_reset_complex_float(filter->previous_output);
}


void filter_free_digital_filter_complex_float(DigitalFilterComplexFloat *filter) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    vector_free_complex_float(filter->feedforward);

    assert_not_null(filter->previous_input);
    vector_free_complex_float(filter->previous_input);

    if (filter->feedback != NULL) {
        vector_free_complex_float(filter->feedback);
        assert_not_null(filter->previous_output);
        vector_free_complex_float(filter->previous_output);
    }
    free(filter);
}

DigitalFilterComplexFloat *filter_make_savgol_complex_float(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    VectorComplexFloat *feedforward = vector_new_complex_float(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *vector_element_complex_float(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    DigitalFilterComplexFloat *filter = filter_make_digital_filter_complex_float(feedforward, NULL);
    vector_free_complex_float(feedforward);
    return filter;
}

DigitalFilterComplexFloat *filter_make_ewma_complex_float(float alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    VectorComplexFloat *feedforward = vector_new_complex_float(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_element_complex_float(0, feedforward) = alpha;

    VectorComplexFloat *feedback = vector_new_complex_float(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_element_complex_float(0, feedback) = 1 - alpha;

    DigitalFilterComplexFloat *filter = 
        filter_make_digital_filter_complex_float(feedforward, feedback);

    vector_free_complex_float(feedforward);
    vector_free_complex_float(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_free_complex_float(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

DigitalFilterComplexFloat *filter_make_first_order_iir_complex_float(float cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    float angular_frequency = ordinary_frequency_to_angular_float(cutoff_frequency);
    return filter_make_ewma_complex_float(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

DigitalFilterComplexFloat *filter_make_sinc_complex_float(
    float cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionFloat window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular_float;

    VectorComplexFloat *filter_coefficients = vector_new_complex_float(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    float kernel_shift = ((int) length - 1) / 2.0;
    float complex uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_element_complex_float(i, filter_coefficients) = 
            sinc_float(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_element_complex_float(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        float complex dc_corrected_coefficient = 
            *vector_element_complex_float(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_element_complex_float(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta_float(i - length / 2) - dc_corrected_coefficient;
    }
    DigitalFilterComplexFloat *filter = 
        filter_make_digital_filter_complex_float(filter_coefficients, NULL);
    
    vector_free_complex_float(filter_coefficients);

    return filter;
}





double filter_evaluate_digital_filter_real_double(double input, DigitalFilterRealDouble *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double accumulate = 0.0;
    vector_shift_real_double(input, filter->previous_input);
    accumulate +=
        vector_dot_real_double(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += vector_dot_real_double(filter->feedback, filter->previous_output);
        vector_shift_real_double(accumulate, filter->previous_output);
    }
    return accumulate;
}

DigitalFilterRealDouble *filter_make_digital_filter_real_double(
    const VectorRealDouble *feedforward,
    const VectorRealDouble *feedback
) {
    DigitalFilterRealDouble *filter = malloc(sizeof(DigitalFilterRealDouble));
    if (filter == NULL)
        return NULL;
    
    filter->feedforward = vector_duplicate_real_double(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_real_double(vector_length_real_double(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        filter->feedback = vector_duplicate_real_double(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_new_real_double(vector_length_real_double(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        vector_free_real_double(filter->feedback);
    fail_allocate_feedback:
        vector_free_real_double(filter->previous_input);
    fail_allocate_previous_input:
        vector_free_real_double(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}


void filter_reset_digital_filter_real_double(DigitalFilterRealDouble *filter) {
    assert_not_null(filter);
    
    vector_reset_real_double(filter->previous_input);
    if (filter->previous_output)
        vector_reset_real_double(filter->previous_output);
}


void filter_free_digital_filter_real_double(DigitalFilterRealDouble *filter) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    vector_free_real_double(filter->feedforward);

    assert_not_null(filter->previous_input);
    vector_free_real_double(filter->previous_input);

    if (filter->feedback != NULL) {
        vector_free_real_double(filter->feedback);
        assert_not_null(filter->previous_output);
        vector_free_real_double(filter->previous_output);
    }
    free(filter);
}

DigitalFilterRealDouble *filter_make_savgol_real_double(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    VectorRealDouble *feedforward = vector_new_real_double(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *vector_element_real_double(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    DigitalFilterRealDouble *filter = filter_make_digital_filter_real_double(feedforward, NULL);
    vector_free_real_double(feedforward);
    return filter;
}

DigitalFilterRealDouble *filter_make_ewma_real_double(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    VectorRealDouble *feedforward = vector_new_real_double(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_element_real_double(0, feedforward) = alpha;

    VectorRealDouble *feedback = vector_new_real_double(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_element_real_double(0, feedback) = 1 - alpha;

    DigitalFilterRealDouble *filter = 
        filter_make_digital_filter_real_double(feedforward, feedback);

    vector_free_real_double(feedforward);
    vector_free_real_double(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_free_real_double(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

DigitalFilterRealDouble *filter_make_first_order_iir_real_double(double cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    double angular_frequency = ordinary_frequency_to_angular_double(cutoff_frequency);
    return filter_make_ewma_real_double(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

DigitalFilterRealDouble *filter_make_sinc_real_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionDouble window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular_double;

    VectorRealDouble *filter_coefficients = vector_new_real_double(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    double kernel_shift = ((int) length - 1) / 2.0;
    double uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_element_real_double(i, filter_coefficients) = 
            sinc_double(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_element_real_double(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        double dc_corrected_coefficient = 
            *vector_element_real_double(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_element_real_double(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta_double(i - length / 2) - dc_corrected_coefficient;
    }
    DigitalFilterRealDouble *filter = 
        filter_make_digital_filter_real_double(filter_coefficients, NULL);
    
    vector_free_real_double(filter_coefficients);

    return filter;
}





float filter_evaluate_digital_filter_real_float(float input, DigitalFilterRealFloat *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    float accumulate = 0.0;
    vector_shift_real_float(input, filter->previous_input);
    accumulate +=
        vector_dot_real_float(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += vector_dot_real_float(filter->feedback, filter->previous_output);
        vector_shift_real_float(accumulate, filter->previous_output);
    }
    return accumulate;
}

DigitalFilterRealFloat *filter_make_digital_filter_real_float(
    const VectorRealFloat *feedforward,
    const VectorRealFloat *feedback
) {
    DigitalFilterRealFloat *filter = malloc(sizeof(DigitalFilterRealFloat));
    if (filter == NULL)
        return NULL;
    
    filter->feedforward = vector_duplicate_real_float(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_real_float(vector_length_real_float(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        filter->feedback = vector_duplicate_real_float(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_new_real_float(vector_length_real_float(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        vector_free_real_float(filter->feedback);
    fail_allocate_feedback:
        vector_free_real_float(filter->previous_input);
    fail_allocate_previous_input:
        vector_free_real_float(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}


void filter_reset_digital_filter_real_float(DigitalFilterRealFloat *filter) {
    assert_not_null(filter);
    
    vector_reset_real_float(filter->previous_input);
    if (filter->previous_output)
        vector_reset_real_float(filter->previous_output);
}


void filter_free_digital_filter_real_float(DigitalFilterRealFloat *filter) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    vector_free_real_float(filter->feedforward);

    assert_not_null(filter->previous_input);
    vector_free_real_float(filter->previous_input);

    if (filter->feedback != NULL) {
        vector_free_real_float(filter->feedback);
        assert_not_null(filter->previous_output);
        vector_free_real_float(filter->previous_output);
    }
    free(filter);
}

DigitalFilterRealFloat *filter_make_savgol_real_float(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    VectorRealFloat *feedforward = vector_new_real_float(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *vector_element_real_float(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    DigitalFilterRealFloat *filter = filter_make_digital_filter_real_float(feedforward, NULL);
    vector_free_real_float(feedforward);
    return filter;
}

DigitalFilterRealFloat *filter_make_ewma_real_float(float alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    VectorRealFloat *feedforward = vector_new_real_float(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_element_real_float(0, feedforward) = alpha;

    VectorRealFloat *feedback = vector_new_real_float(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_element_real_float(0, feedback) = 1 - alpha;

    DigitalFilterRealFloat *filter = 
        filter_make_digital_filter_real_float(feedforward, feedback);

    vector_free_real_float(feedforward);
    vector_free_real_float(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_free_real_float(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

DigitalFilterRealFloat *filter_make_first_order_iir_real_float(float cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    float angular_frequency = ordinary_frequency_to_angular_float(cutoff_frequency);
    return filter_make_ewma_real_float(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

DigitalFilterRealFloat *filter_make_sinc_real_float(
    float cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionFloat window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular_float;

    VectorRealFloat *filter_coefficients = vector_new_real_float(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    float kernel_shift = ((int) length - 1) / 2.0;
    float uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_element_real_float(i, filter_coefficients) = 
            sinc_float(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_element_real_float(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        float dc_corrected_coefficient = 
            *vector_element_real_float(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_element_real_float(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta_float(i - length / 2) - dc_corrected_coefficient;
    }
    DigitalFilterRealFloat *filter = 
        filter_make_digital_filter_real_float(filter_coefficients, NULL);
    
    vector_free_real_float(filter_coefficients);

    return filter;
}

