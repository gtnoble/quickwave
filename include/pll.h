#ifndef SQLDSP_PLL
#define SQLDSP_PLL

#include "filter.h"
#include "sinusoid.h"
#include <stdbool.h>
#include <complex.h>

/**
 * @brief 
 * Phase-locked loop.
 * A numerically controlled oscillator is iteratively adjusted until its phase and frequency are aligned with an input signal.
 * Can be used as a sort of continuous sinusoid fit.
 */
typedef struct {
    double (*loop_filter)(double input, void *filter_context);
    void *filter_context;
    Sinusoid input_iq;
    Sinusoid nco;
} PhaseLockedLoop;

/**
 * @brief 
 * Constructs a phase-locked loop.
 * @param nco_initial Initial numerically controlled oscillator (NCO) state
 * @param loop_filter Loop filter function
 * @param filter_context Context data to pass to loop filter when it is called. Can store filter state objects, etc.
 * @return Phase-locked loop
 */
PhaseLockedLoop pll_make(
    Sinusoid vco_initial,
    double (*loop_filter)(double input, void *filter_context),
    void *filter_context
);

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @return Numerically-controlled oscillator (NCO) state
 */
Sinusoid pll_evaluate(double input, PhaseLockedLoop *pll);

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset(Sinusoid vco_initial, PhaseLockedLoop *pll);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco NCO to update
 * @return Updated NCO
 */
Sinusoid nco_update(double complex complex_freq, Sinusoid nco);

/**
 * @brief 
 * Performs quadrature mixing. Converts a baseband signal to and from an I/Q (inphase and quadrature) signal.
 * @param reference Oscillator used as the reference for the mixing.
 * @param input Next input signal value to mix
 * @return Mixed value
 */
Sinusoid quadrature_mix(Sinusoid reference, double complex input);

#endif