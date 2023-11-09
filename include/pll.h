#ifndef QUICKWAVE_PLL
#define QUICKWAVE_PLL

#include <stdbool.h>
#include <complex.h>

#include "filter.h"
#include "sinusoid.h"
#include "pid.h"


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
 * Creates a type 2 PLL loop filter
 * @param noise_bandwidth Normalized noise bandwidth
 * @param damping_coefficient Damping coefficient
 * @return loop filter
 */
Pid pll_loop_filter_make(double noise_bandwidth, double damping_coefficient);

#endif