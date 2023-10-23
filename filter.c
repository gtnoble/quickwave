#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include "savgol.h"
#include "filter.h"
#include "constants.h"

double sinc(double x);

double complex filter_evaluate(double complex input, DigitalFilter *filter) {
    assert(filter != NULL);

    double _Complex accumulate = 0;
    assert(filter->previous_inputs != NULL);
    circbuf_shift(input, filter->previous_inputs);
    for (size_t i = 0; i < filter->n_feedforward; i++) {
        accumulate +=  *circbuf_element(-i, filter->previous_inputs) *
            filter->feedforward[i];
    }

    if (filter->n_feedback > 0) {
        assert(filter->previous_outputs != NULL);
        for (size_t i = 0; i < filter->n_feedback; i++) {
            accumulate += *circbuf_element(-i, filter->previous_outputs) *
                filter->feedback[i];
        }
        circbuf_shift(accumulate, filter->previous_outputs);
    }

    return accumulate;
}

DigitalFilter *filter_make_digital_filter(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
    ) {
    DigitalFilter *filter = malloc(sizeof(DigitalFilter));
    if (filter == NULL)
        return NULL;

    assert(n_feedforward > 0);

    filter->n_feedforward = n_feedforward;
    filter->n_feedback = n_feedback;
    filter->window_size = 0;

    filter->feedforward = NULL;
    filter->feedback = NULL;
    filter->previous_outputs = NULL;
    filter->previous_inputs = NULL;

    size_t size_feedback = sizeof(double complex) * n_feedback;
    if (n_feedback > 0) {
        if (
            (filter->feedback = malloc(size_feedback)) == NULL ||
            (filter->previous_outputs = circbuf_new(n_feedback)) == NULL
        )
            goto fail;
        
        if (feedback != NULL) {
            memcpy(filter->feedback, feedback, size_feedback);
        }
    }

    size_t size_feedforward = sizeof(double complex) * n_feedforward;
    if (
        (filter->feedforward = malloc(size_feedforward)) == NULL ||
        (filter->previous_inputs = circbuf_new(n_feedforward)) == NULL
    ) {
        goto fail;
    }

    if (feedforward != NULL) {
        memcpy(filter->feedforward, feedforward, size_feedforward);
    }

    return filter;

    fail: 
        filter_free_digital_filter(filter);
        return NULL;
}

void filter_reset_digital_filter(DigitalFilter *filter) {
    circbuf_reset(filter->previous_inputs);
    circbuf_reset(filter->previous_outputs);
}

void filter_free_digital_filter(DigitalFilter *filter) {
    free(filter->feedforward);
    free(filter->feedback);
    circbuf_free(filter->previous_outputs);
    circbuf_free(filter->previous_inputs);
    free(filter);
}

DigitalFilter *filter_make_savgol(
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
    complex double feedforward[filter_length];

    for (size_t i = 0; i < filter_length; i++) {
        feedforward[i] = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    return filter_make_digital_filter(filter_length, feedforward, 0, NULL);
}

DigitalFilter *filter_make_integrator() {
    complex double feedforward[] = {1.0};
    complex double feedback[] = {1.0};
    return filter_make_digital_filter(1, feedforward, 1, feedback);
}

DigitalFilter *filter_make_ewma(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    complex double feedforward[] = {alpha};
    complex double feedback[] = {1 - alpha};
    return filter_make_digital_filter(1, feedforward, 1, feedback);
}

DigitalFilter *filter_make_first_order_iir(double cutoff_frequency) {
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

DigitalFilter *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
) {
    assert(cutoff_frequency >= 0);

    if (window == NULL)
        window = window_rectangular;

    double complex feedforward[length];
    double kernel_shift = ((int) length - 1) / 2.0;

    for (size_t i = 0; i < length; i++) {
        feedforward[i] = 
            2 * 
            cutoff_frequency * 
            sinc(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
    }
    return filter_make_digital_filter(length, feedforward, 0, NULL);
}

MovingAverage *filter_make_moving_average(size_t length) {
    MovingAverage *filter = malloc(sizeof(MovingAverage));
    
    if (filter == NULL) {
        return NULL;
    }

    filter->previous_input = circbuf_new(length);
    if (filter->previous_input == NULL) {
        free(filter);
        return NULL;
    }

    filter->moving_sum = 0;

    return filter;
}

double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
) {
    filter->moving_sum += input;
    filter->moving_sum -= circbuf_shift(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements;
}

double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}