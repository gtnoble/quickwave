
#include "pll.h"
#include "oscillator.h"
#include "constants.h"
#include "phasor.h"
#include "pid.h"
#include <math.h>
#include <assert.h>
#include <complex.h>


PhaseLockedLoopDouble *pll_make_double(
    const OscillatorDouble *nco_initial,
    const PidDouble *loop_filter,
    const MemoryManager *manager
) {
    PhaseLockedLoopDouble *pll = manager->allocate(sizeof(PhaseLockedLoopDouble));
    pll->loop_filter = *loop_filter;
    pll->nco = *nco_initial;
    pll->free = manager->deallocate;

    return pll;
}

void pll_reset_double(OscillatorDouble nco_initial, PhaseLockedLoopDouble *pll) {
    pll->nco = nco_initial;
}

void pll_free_double(PhaseLockedLoopDouble *pll) {
    pll->free(pll);
}

void pll_evaluate_double(
    double input, 
    OscillatorDouble *output,
    PhaseLockedLoopDouble *pll
) {
    double complex phase_error = conj(oscillator_phase_double(&pll->nco)) * input;

    double complex next_frequency = 
        angular_to_complex_frequency_double(
            pid_evaluate_double(carg(phase_error), &pll->loop_filter)
        );

    *output = pll->nco;
    oscillator_update_double(next_frequency, &pll->nco);
}

PidDouble *pll_loop_filter_make_double(
    double noise_bandwidth, 
    double damping_coefficient, 
    const MemoryManager *manager
) {
    return pid_make_double(
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
        0.0,
        manager
    );
}



PhaseLockedLoopFloat *pll_make_float(
    const OscillatorFloat *nco_initial,
    const PidFloat *loop_filter,
    const MemoryManager *manager
) {
    PhaseLockedLoopFloat *pll = manager->allocate(sizeof(PhaseLockedLoopFloat));
    pll->loop_filter = *loop_filter;
    pll->nco = *nco_initial;
    pll->free = manager->deallocate;

    return pll;
}

void pll_reset_float(OscillatorFloat nco_initial, PhaseLockedLoopFloat *pll) {
    pll->nco = nco_initial;
}

void pll_free_float(PhaseLockedLoopFloat *pll) {
    pll->free(pll);
}

void pll_evaluate_float(
    float input, 
    OscillatorFloat *output,
    PhaseLockedLoopFloat *pll
) {
    float complex phase_error = conjf(oscillator_phase_float(&pll->nco)) * input;

    float complex next_frequency = 
        angular_to_complex_frequency_float(
            pid_evaluate_float(cargf(phase_error), &pll->loop_filter)
        );

    *output = pll->nco;
    oscillator_update_float(next_frequency, &pll->nco);
}

PidFloat *pll_loop_filter_make_float(
    float noise_bandwidth, 
    float damping_coefficient, 
    const MemoryManager *manager
) {
    return pid_make_float(
        4.0 * damping_coefficient * noise_bandwidth / 
        (
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient)
        ),
        4.0 * powf(noise_bandwidth, 2.0) /
        powf(
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient),
            2.0
        ),
        0.0,
        manager
    );
}
