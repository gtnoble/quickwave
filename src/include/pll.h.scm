(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_PLL
#define QUICKWAVE_PLL

#include <stdbool.h>
#include <complex.h>

#include \"filter.h\"
#include \"oscillator.h\"
#include \"pid.h\"
#include \"memory.h\"


"

(generate-text
    pll-schema
"
/**
 * @brief 
 * Phase-locked loop.
 * A numerically controlled oscillator is iteratively adjusted until its phase and frequency are aligned with an input signal.
 * Can be used as a sort of continuous sinusoid fit.
 */
typedef struct {
    ${pid-type} loop_filter;
    ${oscillator-type} nco;
    Deallocator *free;
} ${pll-type};

/**
 * @brief 
 * Constructs a phase-locked loop.
 * @param nco_initial Initial numerically controlled oscillator (NCO) state
 * @param loop_filter Loop filter function
 * @param filter_context Context data to pass to loop filter when it is called. Can store filter state objects, etc.
 * @return Phase-locked loop
 */
${pll-type} *pll_make${function-tag}(
    const ${oscillator-type} *vco_initial,
    const ${pid-type} *loop_filter,
    const MemoryManager *manager
);

/**
 * @brief 
 * Evaluates a phase-locked loop (PLL)
 * @param input Next input signal value
 * @param pll PLL to evaluate
 * @poaram output Numerically-controlled oscillator (NCO) state
 */
void pll_evaluate${function-tag}(
    ${number-type} input, 
    ${oscillator-type} *output,
    ${pll-type} *pll
);

/**
 * @brief 
 * Resets phase-locked loop (PLL) to an initial state
 * @param nco_initial State to reset NCO to
 * @param pll PLL to reset
 */
void pll_reset${function-tag}(${oscillator-type} vco_initial, ${pll-type} *pll);

void pll_free${function-tag}(${pll-type} *pll);

/**
 * @brief 
 * Creates a type 2 PLL loop filter
 * @param noise_bandwidth Normalized noise bandwidth
 * @param damping_coefficient Damping coefficient
 * @return loop filter
 */
${pid-type} *pll_loop_filter_make${function-tag}(
    ${number-type} noise_bandwidth, 
    ${number-type} damping_coefficient,
    const MemoryManager *manager);

")

"
#endif
")