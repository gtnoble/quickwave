



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
double sinc(double x);

/**
 * @brief 
 * Dirac delta function
 * @param x Input
 * @return Result 
 */
double dirac_delta(double x);

filter_evaluate_digital_filter_complex_double(
    double complex input, 
    FilterComplexDouble *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double accumulate = 0.0;
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

filter_evaluate_digital_filter_complex_float(
    float complex input, 
    FilterComplexFloat *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double accumulate = 0.0;
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

filter_evaluate_digital_filter_real_double(
    double input, 
    FilterRealDouble *filter) {
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

filter_evaluate_digital_filter_real_float(
    float input, 
    FilterRealFloat *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double accumulate = 0.0;
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

FilterComplexDouble *filter_make_digital_filter_complex_double(
    const VectorComplexDouble *feedforward,
    const VectorComplexDouble *feedback
) {
    FilterComplexDouble *filter = malloc(sizeof(FilterComplexDouble));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = vector_duplicate_complex_double(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_complex_double(vector_length_complex_double(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = vector_duplicate_complex_double(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_complex_new(vector_length_complex_double(feedback));
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

FilterComplexFloat *filter_make_digital_filter_complex_float(
    const VectorComplexFloat *feedforward,
    const VectorComplexFloat *feedback
) {
    FilterComplexFloat *filter = malloc(sizeof(FilterComplexFloat));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = vector_duplicate_complex_float(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_complex_float(vector_length_complex_float(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = vector_duplicate_complex_float(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_complex_new(vector_length_complex_float(feedback));
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

FilterRealDouble *filter_make_digital_filter_real_double(
    const VectorRealDouble *feedforward,
    const VectorRealDouble *feedback
) {
    FilterRealDouble *filter = malloc(sizeof(FilterRealDouble));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = vector_duplicate_real_double(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_real_double(vector_length_real_double(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = vector_duplicate_real_double(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_complex_new(vector_length_real_double(feedback));
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

FilterRealFloat *filter_make_digital_filter_real_float(
    const VectorRealFloat *feedforward,
    const VectorRealFloat *feedback
) {
    FilterRealFloat *filter = malloc(sizeof(FilterRealFloat));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = vector_duplicate_real_float(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new_real_float(vector_length_real_float(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = vector_duplicate_real_float(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_complex_new(vector_length_real_float(feedback));
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

void filter_reset_digital_filter_complex_double(FilterComplexDouble *filter) {
    assert_not_null(filter);
    
    vector_reset_complex_double(filter->previous_input);
    if (filter->previous_output)
        vector_reset_complex_double(filter->previous_output);
}

void filter_reset_digital_filter_complex_float(FilterComplexFloat *filter) {
    assert_not_null(filter);
    
    vector_reset_complex_float(filter->previous_input);
    if (filter->previous_output)
        vector_reset_complex_float(filter->previous_output);
}

void filter_reset_digital_filter_real_double(FilterRealDouble *filter) {
    assert_not_null(filter);
    
    vector_reset_real_double(filter->previous_input);
    if (filter->previous_output)
        vector_reset_real_double(filter->previous_output);
}

void filter_reset_digital_filter_real_float(FilterRealFloat *filter) {
    assert_not_null(filter);
    
    vector_reset_real_float(filter->previous_input);
    if (filter->previous_output)
        vector_reset_real_float(filter->previous_output);
}

void filter_free_digital_filter_complex_double(FilterComplexDouble *filter) {
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

void filter_free_digital_filter_complex_float(FilterComplexFloat *filter) {
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

void filter_free_digital_filter_real_double(FilterRealDouble *filter) {
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

void filter_free_digital_filter_real_float(FilterRealFloat *filter) {
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

FilterComplexDouble *filter_make_savgol_complex_double(size_t filter_length, int derivative, int polynomial_order) {
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

    FilterComplexDouble *filter = filter_make_digital_filter_complex_double(feedforward, NULL);
    vector_free_complex_double(feedforward);
    return filter;
}

FilterComplexFloat *filter_make_savgol_complex_float(size_t filter_length, int derivative, int polynomial_order) {
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

    FilterComplexFloat *filter = filter_make_digital_filter_complex_float(feedforward, NULL);
    vector_free_complex_float(feedforward);
    return filter;
}

FilterRealDouble *filter_make_savgol_real_double(size_t filter_length, int derivative, int polynomial_order) {
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

    FilterRealDouble *filter = filter_make_digital_filter_real_double(feedforward, NULL);
    vector_free_real_double(feedforward);
    return filter;
}

FilterRealFloat *filter_make_savgol_real_float(size_t filter_length, int derivative, int polynomial_order) {
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

    FilterRealFloat *filter = filter_make_digital_filter_real_float(feedforward, NULL);
    vector_free_real_float(feedforward);
    return filter;
}

DigitalFilterComplex *filter_make_ewma(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    VectorComplexDouble *feedforward = vector_complex_new(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_complex_element(0, feedforward) = alpha;

    VectorComplexDouble *feedback = vector_complex_new(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_complex_element(0, feedback) = 1 - alpha;

    DigitalFilterComplex *filter = 
        filter_make_digital_filter_complex(feedforward, feedback);

    vector_complex_free(feedforward);
    vector_complex_free(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_complex_free(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

DigitalFilterComplex *filter_make_first_order_iir(double cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    double angular_frequency = ordinary_frequency_to_angular(cutoff_frequency);
    return filter_make_ewma(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

DigitalFilterReal *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular;

    VectorRealDouble *filter_coefficients = vector_real_new(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    double kernel_shift = ((int) length - 1) / 2.0;
    double uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_real_element(i, filter_coefficients) = 
            sinc(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_real_element(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        double dc_corrected_coefficient = 
            *vector_real_element(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_real_element(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta(i - length / 2) - dc_corrected_coefficient;
    }
    DigitalFilterReal *filter = 
        filter_make_digital_filter_real(filter_coefficients, NULL);
    
    vector_real_free(filter_coefficients);

    return filter;
}

double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

double dirac_delta(double x) {
    return x == 0.0 ? 1.0 : 0.0;
}