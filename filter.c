#include <stdlib.h>
#include <assert.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3
#include "savgol.h"
#include "filter.h"


double filter_evaluate(double input, DigitalFilter *filter) {
    double accumulate = 0;
    circbuf_shift(input, filter->previous_inputs);
    for (size_t i = 0; i < filter->n_feedforward; i++) {
        accumulate +=  *circbuf_element(-i, filter->previous_inputs) *
            filter->feedforward[i];
    }

    if (filter->n_feedback > 0) {
        for (size_t i = 0; i < filter->n_feedback; i++) {
            accumulate += *circbuf_element(-i, filter->previous_outputs) *
                filter->feedback[i];
        }
        circbuf_shift(accumulate, filter->previous_outputs);
    }

    return accumulate;
}

double filter_current_value(DigitalFilter *filter) {
    return filter->previous_outputs->buffer[0];
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

DigitalFilter *filter_make_savgol(size_t filter_length, int deriv, int polyorder) {
    // Window length must be odd
    if ((filter_length & 0x1) != 0) {
        return NULL;
    }
    size_t center = filter_length / 2 + 1;
    DigitalFilter *filter = filter_make(filter_length, 0);
    if (filter == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < filter_length; i++) {
        filter->feedforward[i] = savgol_weight(i, center, filter_length, polyorder, deriv);
    }
    return filter;
}