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
    lagged_input = circbuf_new((int) ceil(1 / minimum_frequency / 4.0));
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
    pll->input_iq = detect_iq(pll->vco, pll->lagged_input);
    Sinusoid phase_error = 
        sinusoid_mult(
            sinusoid_negate_phase(pll->vco),
            pll->input_iq
        );
    double complex next_frequency = pll->loop_filter(phase_error.phasor, pll->filter_context);
    pll->vco = update_vco(next_frequency, pll->vco);
    return pll->vco;
}


Sinusoid update_vco(double _Complex update_frequency, Sinusoid vco) {
    double complex next_frequency =
        cimag(update_frequency) <= 0.0 ? 
        vco.complex_frequency : 
        update_frequency / cabs(update_frequency);

    Sinusoid updated_vco = {
        .complex_frequency = next_frequency,
        .phasor = vco.phasor * next_frequency
    };
    return updated_vco;
}

Sinusoid detect_iq(Sinusoid reference, CircularBuffer *lagged_input) {
    double reference_normal_frequency = angular_frequency_to_ordinary(
        sinusoid_angular_freq(reference)
    );
    assert(reference_normal_frequency > 0);
    int last_element_index = -(lagged_input->n_elements) + 1;
    double quadrature_lag = -(1 / reference_normal_frequency) / 4.0;
    double quadrature_measurement_lag = 
        quadrature_lag > last_element_index ? quadrature_lag : last_element_index;

    int in_phase_element_index = 0;
    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            circbuf_interpolated_element(quadrature_measurement_lag, lagged_input) * I +
            *circbuf_element(in_phase_element_index, lagged_input) 
    };
    return input_sinusoid;
}