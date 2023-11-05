#include "pll.h"
#include "sinusoid.h"
#include "constants.h"
#include "phasor.h"
#include "pid.h"
#include <math.h>
#include <assert.h>
#include <complex.h>

PhaseLockedLoop pll_make(
    Sinusoid nco_initial,
    double (*loop_filter)(double input, void *filter_context),
    void *filter_context
) {
    assert(loop_filter != NULL);

    PhaseLockedLoop pll;
    pll.loop_filter = loop_filter;
    pll.filter_context = filter_context;
    pll.nco = nco_initial;

    return pll;
}

void pll_reset(Sinusoid nco_initial, PhaseLockedLoop *pll) {
    pll->nco = nco_initial;
}

Sinusoid pll_evaluate(double input, PhaseLockedLoop *pll) {
    Sinusoid phase_error = quadrature_mix(sinusoid_negate_phase(pll->nco), input);

    double complex next_frequency = 
        angular_to_complex_frequency(
            pll->loop_filter(
                sinusoid_phase(phase_error), 
                pll->filter_context
            )
        );

    Sinusoid output = pll->nco;
    pll->nco = nco_update(next_frequency, pll->nco);
    return output;
}

Sinusoid nco_update(double complex update_frequency, Sinusoid nco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        nco.complex_frequency : 
        update_frequency / cabs(update_frequency);

    Sinusoid updated_nco = {
        .complex_frequency = next_frequency,
        .phasor = nco.phasor * next_frequency
    };
    return updated_nco;
}

Sinusoid quadrature_mix(Sinusoid reference, double complex input) {

    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            input * reference.phasor
    };
    return input_sinusoid;
}

Pid pll_loop_filter_make(double noise_bandwidth, double damping_coefficient) {
    return pid_make(
        4.0 * damping_coefficient * noise_bandwidth / 
        (
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient)
        ),
        4.0 * pow(noise_bandwidth, 2.0) /
        pow(
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient),
            2.0
        ),
        0.0
    );
}