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

PhaseLockedLoop pll_make(
    Sinusoid vco_initial,
    double (*loop_filter)(double input, void *filter_context),
    void *filter_context
);
Sinusoid pll_evaluate(double input, PhaseLockedLoop *pll);
void pll_reset(Sinusoid vco_initial, PhaseLockedLoop *pll);
Sinusoid nco_update(double complex complex_freq, Sinusoid nco);
Sinusoid quadrature_mix(Sinusoid reference, double complex input);

#endif