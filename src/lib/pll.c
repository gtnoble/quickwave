#include "pll.h"
#include "sinusoid.h"
#include "constants.h"
#include <math.h>
#include <assert.h>
#include <complex.h>

/**
 * @brief 
 * Constructs a phase-locked loop.
 * @param nco_initial Initial numerically controlled oscillator (NCO) state
 * @param loop_filter Loop filter function
 * @param filter_context Context data to pass to loop filter when it is called. Can store filter state objects, etc.
 * @return PhaseLockedLoop Phase-locked loop
 */
PhaseLockedLoop pll_make(
    Sinusoid nco_initial,
    double complex (*loop_filter)(double complex input, void *filter_context),
    void *filter_context
) {
    assert(loop_filter != NULL);

    PhaseLockedLoop pll;
    pll.loop_filter = loop_filter;
    pll.filter_context = filter_context;
    pll.nco = nco_initial;

    return pll;
}

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset(Sinusoid nco_initial, PhaseLockedLoop *pll) {
    pll->nco = nco_initial;
}

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @return Sinusoid Numerically-controlled oscillator (NCO) state
 */
Sinusoid pll_evaluate(double input, PhaseLockedLoop *pll) {
    pll->input_iq = quadrature_mix(pll->nco, input);
    Sinusoid phase_error = sinusoid_negate_phase(pll->input_iq);

    double complex next_frequency = 
        pll->loop_filter(
            phase_error.phasor, 
            pll->filter_context
        );

    pll->nco = nco_update(next_frequency, pll->nco);
    Sinusoid output = pll->nco;
    return output;
}

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco NCO to update
 * @return Sinusoid Updated NCO
 */
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

/**
 * @brief 
 * Performs quadrature mixing. Converts a baseband signal to and from an I/Q (inphase and quadrature) signal.
 * @param reference Oscillator used as the reference for the mixing.
 * @param input Next input signal value to mix
 * @return Sinusoid Mixed value
 */
Sinusoid quadrature_mix(Sinusoid reference, double complex input) {

    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            reference.phasor * input / I +
            reference.phasor * input
    };
    return input_sinusoid;
}