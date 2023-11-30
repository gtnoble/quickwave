
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
    PidDouble loop_filter;
    OscillatorDouble nco;
} PhaseLockedLoopDouble;

/**
 * @brief 
 * Constructs a phase-locked loop.
 * @param nco_initial Initial numerically controlled oscillator (NCO) state
 * @param loop_filter Loop filter function
 * @param filter_context Context data to pass to loop filter when it is called. Can store filter state objects, etc.
 * @return Phase-locked loop
 */
PhaseLockedLoopDouble pll_make_double(
    OscillatorDouble vco_initial,
    PidDouble loop_filter
);

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @return Numerically-controlled oscillator (NCO) state
 */
OscillatorDouble pll_evaluate_double(double input, PhaseLockedLoopDouble *pll);

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset_double(OscillatorDouble vco_initial, PhaseLockedLoopDouble *pll);

/**
 * @brief 
 * Creates a type 2 PLL loop filter
 * @param noise_bandwidth Normalized noise bandwidth
 * @param damping_coefficient Damping coefficient
 * @return loop filter
 */
PidDouble pll_loop_filter_make_double(double noise_bandwidth, double damping_coefficient);




/**
 * @brief 
 * Phase-locked loop.
 * A numerically controlled oscillator is iteratively adjusted until its phase and frequency are aligned with an input signal.
 * Can be used as a sort of continuous sinusoid fit.
 */
typedef struct {
    PidFloat loop_filter;
    OscillatorFloat nco;
} PhaseLockedLoopFloat;

/**
 * @brief 
 * Constructs a phase-locked loop.
 * @param nco_initial Initial numerically controlled oscillator (NCO) state
 * @param loop_filter Loop filter function
 * @param filter_context Context data to pass to loop filter when it is called. Can store filter state objects, etc.
 * @return Phase-locked loop
 */
PhaseLockedLoopFloat pll_make_float(
    OscillatorFloat vco_initial,
    PidFloat loop_filter
);

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @return Numerically-controlled oscillator (NCO) state
 */
OscillatorFloat pll_evaluate_float(float input, PhaseLockedLoopFloat *pll);

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset_float(OscillatorFloat vco_initial, PhaseLockedLoopFloat *pll);

/**
 * @brief 
 * Creates a type 2 PLL loop filter
 * @param noise_bandwidth Normalized noise bandwidth
 * @param damping_coefficient Damping coefficient
 * @return loop filter
 */
PidFloat pll_loop_filter_make_float(float noise_bandwidth, float damping_coefficient);


#endif
