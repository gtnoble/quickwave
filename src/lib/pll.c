#include "pll.h"
#include "oscillator.h"
#include "constants.h"
#include "phasor.h"
#include "pid.h"
#include <math.h>
#include <assert.h>
#include <complex.h>

PhaseLockedLoop pll_make(
    Oscillator nco_initial,
    Pid loop_filter
) {
    PhaseLockedLoop pll;
    pll.loop_filter = loop_filter;
    pll.nco = nco_initial;

    return pll;
}

void pll_reset(Oscillator nco_initial, PhaseLockedLoop *pll) {
    pll->nco = nco_initial;
}

Oscillator pll_evaluate(double input, PhaseLockedLoop *pll) {
    double complex phase_error = conj(oscillator_phase(pll->nco)) * input;

    double complex next_frequency = 
        angular_to_complex_frequency(
            pid_evaluate(carg(phase_error), &pll->loop_filter)
        );

    return oscillator_update(next_frequency, &pll->nco);
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