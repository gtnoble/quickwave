#include "pll.h"
#include "sinusoid.h"
#include "constants.h"
#include <math.h>
#include <assert.h>
#include <complex.h>

PhaseLockedLoop *pll_make(
    Sinusoid vco_initial,
    double complex (*loop_filter)(double complex input, void *filter_context),
    void *filter_context
) {
    assert(loop_filter != NULL);

    PhaseLockedLoop *pll = malloc(sizeof(PhaseLockedLoop));
    if (pll == NULL) {
        return NULL;
    }

    pll->loop_filter = loop_filter;
    pll->filter_context = filter_context;
    pll->vco = vco_initial;

    return pll;
}

void pll_reset(Sinusoid vco_initial, PhaseLockedLoop *pll) {
    pll->vco = vco_initial;
}

void pll_free(PhaseLockedLoop *pll) {
    free(pll);
}

Sinusoid pll_update(double input, PhaseLockedLoop *pll) {
    pll->input_iq = quadrature_mix(pll->vco, input);
    Sinusoid phase_error = sinusoid_negate_phase(pll->input_iq);

    double complex next_frequency = 
        pll->loop_filter(
            phase_error.phasor, 
            pll->filter_context
        );

    pll->vco = update_vco(next_frequency, pll->vco);
    Sinusoid output = pll->vco;
    return output;
}


Sinusoid update_vco(double _Complex update_frequency, Sinusoid vco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        vco.complex_frequency : 
        update_frequency / cabs(update_frequency);

    Sinusoid updated_vco = {
        .complex_frequency = next_frequency,
        .phasor = vco.phasor * next_frequency
    };
    return updated_vco;
}

Sinusoid quadrature_mix(Sinusoid reference, double complex input) {

    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            reference.phasor * input / I +
            reference.phasor * input
    };
    return input_sinusoid;
}