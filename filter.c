#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3
#include "savgol.h"
#include "filter.h"

#define PI 3.14159265358979323846

double fmod_euclidean(double x, double y);

double filter_evaluate(double input, DigitalFilter *filter) {
    assert(filter != NULL);

    double accumulate = 0;
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

double filter_current_value(DigitalFilter *filter) {
    assert(filter != NULL);
    return filter->current_output;
}



DigitalFilter *filter_make(size_t n_feedforward, size_t n_feedback) {
    DigitalFilter *filter = sqlite3_malloc(sizeof(DigitalFilter));
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
            (filter->feedback = sqlite3_malloc(sizeof(double) * n_feedback)) == NULL ||
            (filter->previous_outputs = circbuf_new(n_feedback)) == NULL
        )
            goto fail;

    if (
        (filter->feedforward = sqlite3_malloc(sizeof(double) * n_feedforward)) == NULL ||
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
    sqlite3_free(filter->feedforward);
    sqlite3_free(filter->feedback);
    circbuf_free(filter->previous_outputs);
    circbuf_free(filter->previous_inputs);
    sqlite3_free(filter);
}

int filter_make_savgol(
    DigitalFilter **filter, 
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    if ((filter_length & 0x1) != 1 ||
        filter_length == 0 ||
        derivative < 0 ||
        derivative > polynomial_order) {
        return IMPROPER_PARAMS;
    }
    //size_t center = filter_length / 2 + 1;
    int center = 0;
    *filter = filter_make(filter_length, 0);
    if (filter == NULL) {
        return ALLOCATION_FAILURE;
    }

    assert((*filter)->feedforward != NULL);
    for (size_t i = 0; i < filter_length; i++) {
        (*filter)->feedforward[i] = savgol_weight(i, center, filter_length, polynomial_order, derivative);
    }
    return FILTER_OK;
}

int filter_make_first_order_iir(
    DigitalFilter **filter,
    double cutoff_frequency
) {
    if (cutoff_frequency > 0.5 || cutoff_frequency < 0) {
        return IMPROPER_PARAMS;
    }
    double angular_frequency = cutoff_frequency * 2 * PI;
    double alpha = 
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        );
    *filter = filter_make(1, 1);
    if (filter == NULL) {
        return ALLOCATION_FAILURE;
    }
    (*filter)->feedforward[0] = alpha;
    (*filter)->feedback[0] = 1 - alpha;
    return FILTER_OK;
}

int pll_make(PhaseLockedLoop **pll, double loop_filter_cutoff, double initial_freq) {
    DigitalFilter *loop_filter;
    int status = filter_make_first_order_iir(&loop_filter, loop_filter_cutoff);
    if (status != FILTER_OK) {
        return status;
    }
    loop_filter->previous_outputs->buffer[0] = initial_freq;

    *pll = sqlite3_malloc(sizeof(PhaseLockedLoop));
    if (*pll == NULL) {
        filter_free(loop_filter);
        return ALLOCATION_FAILURE;
    }
    (*pll)->loop_filter = loop_filter;
    (*pll)->phase_accumulator = 0;
    return FILTER_OK;
}

void pll_free(PhaseLockedLoop *pll) {
    filter_free(pll->loop_filter);
    sqlite3_free(pll);
}

double pll_update(double input, PhaseLockedLoop *pll) {
    double frequency_velocity = sin(pll->phase_accumulator) * input * 10;
    return update_vco(
        filter_evaluate(frequency_velocity, pll->loop_filter), 
        &(pll->phase_accumulator)
    );
}

double update_vco(double angular_freq, double *phase_accumulator) {
    *phase_accumulator = fmod_euclidean((*phase_accumulator + angular_freq), 2 * PI);
    return sin(*phase_accumulator);
}

double sign(double x) {
    if (x > 0) 
        return 1.0;
    else if (x < 0)
        return -1.0;
    else
        return 0.0;
}

double fmod_euclidean(double x, double y) {
    double m = fmod(x, y);
    if (m < 0)
    {
        m = (y < 0) ? m - y : m + y;
    }
    return m;
}