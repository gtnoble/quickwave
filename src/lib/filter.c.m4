include(`types.m4')
include(`code_generator.m4')
include(`filter.m4')

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

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_RESET_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_FREE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_SAVGOL')

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