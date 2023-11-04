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

double complex filter_evaluate_digital_filter_complex(double complex input, DigitalFilterComplex *filter) {
    assert_not_null(filter);

    double complex accumulate = 0;
    accumulate += convolution_convolve_complex(input, filter->feedforward);
    if (filter->feedback != NULL) {
        accumulate += convolution_convolve_complex(filter->previous_output, filter->feedback);
        filter->previous_output = accumulate;
    }

    return accumulate;
}

double filter_evaluate_digital_filter_real(double input, DigitalFilterReal *filter) {
    assert_not_null(filter);

    double accumulate = 0;
    accumulate += convolution_convolve_real(input, filter->feedforward);
    if (filter->feedback != NULL) {
        accumulate += convolution_convolve_real(filter->previous_output, filter->feedback);
        filter->previous_output = accumulate;
    }

    return accumulate;
}

DigitalFilterComplex *filter_make_digital_filter_complex(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
    ) {
    DigitalFilterComplex *filter = malloc(sizeof(DigitalFilterComplex));
    if (filter == NULL)
        return NULL;

    assert(n_feedforward > 0);
    assert(feedforward != NULL);
    filter->feedforward = convolution_make_complex(n_feedforward, feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }

    if (n_feedback > 0) {
        assert(feedback != NULL);
        filter->feedback = convolution_make_complex(n_feedback, feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
    }
    else {
        filter->feedback = NULL;
    }

    filter->previous_output = 0;

    return filter;

    fail_allocate_feedback: 
        convolution_reset_complex(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;
}

DigitalFilterReal *filter_make_digital_filter_real(
    size_t n_feedforward, 
    const double feedforward[],
    size_t n_feedback,
    const double feedback[]
    ) {
    DigitalFilterReal *filter = malloc(sizeof(DigitalFilterReal));
    if (filter == NULL)
        return NULL;

    assert(n_feedforward > 0);
    assert(feedforward != NULL);
    filter->feedforward = convolution_make_real(n_feedforward, feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }

    if (n_feedback > 0) {
        assert(feedback != NULL);
        filter->feedback = convolution_make_real(n_feedback, feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
    }
    else {
        filter->feedback = NULL;
    }

    filter->previous_output = 0;

    return filter;

    fail_allocate_feedback: 
        convolution_free_real(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;
}

void filter_reset_digital_filter_complex(DigitalFilterComplex *filter) {
    assert_not_null(filter);

    convolution_reset_complex(filter->feedforward);
    convolution_reset_complex(filter->feedback);
    filter->previous_output = 0;
}

void filter_reset_digital_filter_real(DigitalFilterReal *filter) {
    assert_not_null(filter); 

    convolution_reset_real(filter->feedforward);
    convolution_reset_real(filter->feedback);
    filter->previous_output = 0;
}

void filter_free_digital_filter_complex(DigitalFilterComplex *filter) {
    assert_not_null(filter);

    convolution_reset_complex(filter->feedforward);
    if (filter->feedback != NULL) {
        convolution_reset_complex(filter->feedback);
    }
    free(filter);
}

void filter_free_digital_filter_real(DigitalFilterReal *filter) {
    assert_not_null(filter);

    convolution_free_real(filter->feedforward);
    if (filter->feedback != NULL) {
        convolution_free_real(filter->feedback);
    }
    free(filter);
}

DigitalFilterReal *filter_make_savgol(
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
    double feedforward[filter_length];

    for (size_t i = 0; i < filter_length; i++) {
        size_t last_index = filter_length - 1;
        feedforward[last_index - i] = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    return filter_make_digital_filter_real(filter_length, feedforward, 0, NULL);
}

DigitalFilterComplex *filter_make_ewma(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    complex double feedforward[] = {alpha};
    complex double feedback[] = {1 - alpha};
    return filter_make_digital_filter_complex(1, feedforward, 1, feedback);
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

    double feedforward[length];
    double kernel_shift = ((int) length - 1) / 2.0;

    for (size_t i = 0; i < length; i++) {
        double filter_coefficient = 
            2 * 
            cutoff_frequency * 
            sinc(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);

        feedforward[i] = 
            filter_type == LOW_PASS ? 
            filter_coefficient : 
            dirac_delta(i - length / 2) - filter_coefficient;
    }
    return filter_make_digital_filter_real(length, feedforward, 0, NULL);
}

double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

double dirac_delta(double x) {
    return x == 0.0 ? 1.0 : 0.0;
}