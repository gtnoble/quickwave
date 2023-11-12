#ifndef QUICKWAVE_PLL
#define QUICKWAVE_PLL

#include <stdbool.h>
#include <complex.h>

#include "filter.h"
#include "oscillator.h"
#include "pid.h"


/**
 * @brief 
 * Phase-locked loop.
 * A numerically controlled oscillator is iteratively adjusted until its phase and frequency are aligned with an input signal.
 * Can be used as a sort of continuous sinusoid fit.
 */
typedef struct {
    Pid loop_filter;
    Oscillator nco;
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
    Oscillator vco_initial,
    Pid loop_filter
);

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @return Numerically-controlled oscillator (NCO) state
 */
Oscillator pll_evaluate(double input, PhaseLockedLoop *pll);

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset(Oscillator vco_initial, PhaseLockedLoop *pll);

/**
 * @brief 
 * Creates a type 2 PLL loop filter
 * @param noise_bandwidth Normalized noise bandwidth
 * @param damping_coefficient Damping coefficient
 * @return loop filter
 */
Pid pll_loop_filter_make(double noise_bandwidth, double damping_coefficient);

#endif