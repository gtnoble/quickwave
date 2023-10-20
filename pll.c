#include "pll.h"
#include "sinusoid.h"
#include <math.h>
#include <stdbool.h>
#include <complex.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#define PI 3.14159265358979323846

PhaseLockedLoop *pll_make(double loop_filter_cutoff, double initial_freq, double min_freq) {
    PhaseLockedLoop *pll = sqlite3_malloc(sizeof(PhaseLockedLoop));
    if (pll == NULL) {
        goto failure;
    }

    DigitalFilter *loop_filter = filter_make_first_order_iir(loop_filter_cutoff);
    if (loop_filter == NULL) {
        goto cleanup_pll;
    }

    CircularBuffer *lagged_input;
    lagged_input = circbuf_new((int) ceil(1 / min_freq));
    if (lagged_input == NULL) {
        goto cleanup_filter;
    }

    double complex complex_initial_freq = real_to_complex_frequency(initial_freq * 2 * PI);
    loop_filter->previous_outputs->buffer[0] = complex_initial_freq;
    pll->loop_filter = loop_filter;

    pll->vco.complex_frequency = complex_initial_freq;
    pll->vco.phasor = 1.0 * I;

    pll->lagged_input = lagged_input;
    return pll;

    cleanup_filter:
        filter_free(loop_filter);
    cleanup_pll:
        sqlite3_free(pll);
    failure:
        return NULL;
}

void pll_free(PhaseLockedLoop *pll) {
    filter_free(pll->loop_filter);
    circbuf_free(pll->lagged_input);
    sqlite3_free(pll);
}

Sinusoid pll_update(double input, PhaseLockedLoop *pll) {
    circbuf_shift(input, pll->lagged_input);
    Sinusoid phase_error = sinusoid_negate_phase(
        quadrature_demodulate(
            pll->vco, 
            pll->lagged_input
        )
    );
    double complex next_frequency = filter_evaluate(phase_error.phasor, pll->loop_filter);
    pll->vco = update_vco(next_frequency / cabs(next_frequency), pll->vco);
    return pll->vco;
}

double sign(double x) {
    if (x > 0) 
        return 1.0;
    else if (x < 0)
        return -1.0;
    else
        return 0.0;
}