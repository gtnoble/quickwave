#include "pll.h"
#include "sinusoid.h"
#include "constants.h"
#include <math.h>
#include <assert.h>
#include <complex.h>

PhaseLockedLoop *pll_make(
    Sinusoid vco_initial,
    double minimum_frequency,
    double complex (*loop_filter)(double complex input, void *filter_context),
    void *filter_context
) {
    assert(loop_filter != NULL);
    assert(minimum_frequency > 0);
    assert(minimum_frequency < 0.5);
    assert(
        complex_frequency_to_ordinary(vco_initial.complex_frequency) > 
        minimum_frequency
    );

    PhaseLockedLoop *pll = malloc(sizeof(PhaseLockedLoop));
    if (pll == NULL) {
        goto failure;
    }

    CircularBuffer *lagged_input;
    lagged_input = circbuf_new((int) ceil(1 / minimum_frequency));
    if (lagged_input == NULL) {
        goto cleanup_pll;
    }

    pll->loop_filter = loop_filter;
    pll->filter_context = filter_context;
    pll->vco = vco_initial;
    pll->lagged_input = lagged_input;

    return pll;

    cleanup_pll:
        free(pll);
    failure:
        return NULL;
}

void pll_reset(Sinusoid vco_initial, PhaseLockedLoop *pll) {
    pll->vco = vco_initial;
    circbuf_reset(pll->lagged_input);
}

void pll_free(PhaseLockedLoop *pll) {
    circbuf_free(pll->lagged_input);
    free(pll);
}

Sinusoid pll_update(double input, PhaseLockedLoop *pll) {
    circbuf_shift(input, pll->lagged_input);
    Sinusoid phase_error = sinusoid_negate_phase(
        quadrature_demodulate(
            pll->vco, 
            pll->lagged_input
        )
    );
    double complex next_frequency = pll->loop_filter(phase_error.phasor, pll->filter_context);
    pll->vco = update_vco(next_frequency, pll->vco);
    return pll->vco;
}