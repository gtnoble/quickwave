#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include "savgol.h"
#include "filter.h"

#define PI 3.14159265358979323846

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
    filter->current_output = accumulate;

    return accumulate;
}

double complex filter_current_value(DigitalFilter *filter) {
    assert(filter != NULL);
    return filter->current_output;
}



DigitalFilter *filter_make(size_t n_feedforward, size_t n_feedback) {
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

    if (n_feedback > 0)
        if (
            (filter->feedback = malloc(sizeof(double complex) * n_feedback)) == NULL ||
            (filter->previous_outputs = circbuf_new(n_feedback)) == NULL
        )
            goto fail;

    if (
        (filter->feedforward = malloc(sizeof(double complex) * n_feedforward)) == NULL ||
        (filter->previous_inputs = circbuf_new(n_feedforward)) == NULL
    ) {
        goto fail;
    }

    return filter;

    fail: 
        filter_free(filter);
        return NULL;
}

void filter_free(DigitalFilter *filter) {
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

    //size_t center = filter_length / 2 + 1;
    int center = 0;
    DigitalFilter *filter = filter_make(filter_length, 0);
    if (filter == NULL) {
        return NULL;
    }

    assert(filter->feedforward != NULL);
    for (size_t i = 0; i < filter_length; i++) {
        filter->feedforward[i] = savgol_weight(i, center, filter_length, polynomial_order, derivative);
    }
    return FILTER_OK;
}

DigitalFilter *filter_make_first_order_iir(double cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);

    double angular_frequency = cutoff_frequency * 2 * PI;
    double alpha = 
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        );

    DigitalFilter *filter = filter_make(1, 1);
    if (filter == NULL) {
        return NULL;
    }
    filter->feedforward[0] = alpha;
    filter->feedback[0] = 1 - alpha;
    return filter;
}

DigitalFilter *filter_make_sinc(double cutoff_frequency, size_t length) {
    assert(cutoff_frequency >= 0);

    DigitalFilter *filter = filter_make(length, 0);
    if (filter == NULL) {
        return NULL;
    }

    double kernel_shift = ((int) length - 1) / 2.0;
    for (size_t i = 0; i < length; i++) {
        filter->feedforward[i] = 2 * cutoff_frequency * sinc(2 * cutoff_frequency * ((double) i - kernel_shift));
    }
    return filter;
}

double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(PI * x) / (PI * x);
}