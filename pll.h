#ifndef SQLDSP_PLL
#define SQLDSP_PLL

#include "filter.h"
#include "sinusoid.h"
#include <stdbool.h>
#include <complex.h>

typedef struct {
    double complex (*loop_filter)(double complex input, void *filter_context);
    void *filter_context;
    Sinusoid vco;
    CircularBuffer *lagged_input;
} PhaseLockedLoop;

PhaseLockedLoop *pll_make(
    Sinusoid vco_initial,
    double minimum_frequency,
    double complex (*loop_filter)(double complex input, void *filter_context),
    void *filter_context
);
Sinusoid pll_update(double input, PhaseLockedLoop *pll);
void pll_reset(Sinusoid vco_initial, PhaseLockedLoop *pll);
void pll_free(PhaseLockedLoop *pll);
Sinusoid update_vco(double _Complex complex_freq, Sinusoid vco);
Sinusoid quadrature_demodulate(Sinusoid reference, CircularBuffer *lagged_input);

#endif