#include "pll.h"
#include <math.h>
#include <stdbool.h>
#include <complex.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#define PI 3.14159265358979323846

double fmod_euclidean(double x, double y);
double complex quadrature_demod(double frequency, double phase, CircularBuffer *lagged_input);

int pll_make(PhaseLockedLoop **pll, double loop_filter_cutoff, double initial_freq, double min_freq) {
    int status;
    *pll = sqlite3_malloc(sizeof(PhaseLockedLoop));
    if (*pll == NULL) {
        status = ALLOCATION_FAILURE;
        goto failure;
    }

    DigitalFilter *loop_filter;
    status = filter_make_first_order_iir(&loop_filter, loop_filter_cutoff);
    if (status != FILTER_OK) {
        goto cleanup_pll;
    }

    CircularBuffer *lagged_input;
    lagged_input = circbuf_new((int) round(1 / min_freq));
    if (lagged_input == NULL) {
        status = ALLOCATION_FAILURE;
        goto cleanup_filter;
    }

    loop_filter->previous_outputs->buffer[0] = initial_freq;
    (*pll)->loop_filter = loop_filter;
    (*pll)->phase_accumulator = 0;
    (*pll)->lagged_input = lagged_input;
    return FILTER_OK;

    cleanup_filter:
        filter_free(loop_filter);
    cleanup_pll:
        sqlite3_free(pll);
    failure:
        return status;
}

void pll_free(PhaseLockedLoop *pll) {
    filter_free(pll->loop_filter);
    circbuf_free(pll->lagged_input);
    sqlite3_free(pll);
}

double pll_update(double input, PhaseLockedLoop *pll) {
    circbuf_shift(input, pll->lagged_input);
    double complex iq = quadrature_demod(
        pll->loop_filter->current_output, 
        pll->phase_accumulator, 
        pll->lagged_input
    );
    double phase_shift = carg(iq);
    double amplitude = cabs(iq);
    return amplitude * update_vco(
        filter_evaluate(-phase_shift * 3, pll->loop_filter), 
        &(pll->phase_accumulator),
        true
    );
}

double update_vco(double angular_freq, double *phase_accumulator, bool allow_negative_freq) {
    if (angular_freq >= 0 || allow_negative_freq)
        *phase_accumulator = fmod_euclidean((*phase_accumulator + angular_freq), 2 * PI);
    return cos(*phase_accumulator);
}

double complex quadrature_demod(double frequency, double phase, CircularBuffer *lagged_input) {
    int quadrature_element_index = 0;
    int in_phase_element_index;
    if (frequency < 0)
        in_phase_element_index = 0;
    else {
        int last_element_index = -(lagged_input->n_elements) + 1;
        int quadrature_lag = (int) round(-(1 / frequency));
        if (quadrature_lag < last_element_index) {
            in_phase_element_index = last_element_index;
        }
        else {
            in_phase_element_index = quadrature_lag;
        }
    }
    return *circbuf_element(quadrature_element_index, lagged_input)  * sin(phase) * I + 
        *circbuf_element(in_phase_element_index, lagged_input) * cos(phase);
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