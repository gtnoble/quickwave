#ifndef SQLDSP_PLL
#define SQLDSP_PLL

#include "filter.h"
#include "sinusoid.h"
#include <stdbool.h>

typedef struct {
    DigitalFilter *loop_filter;
    Sinusoid vco;
    CircularBuffer *lagged_input;
} PhaseLockedLoop;

PhaseLockedLoop *pll_make(double loop_filter_cutoff, double initial_freq, double min_freq);
Sinusoid pll_update(double input, PhaseLockedLoop *pll);
void pll_free(PhaseLockedLoop *pll);

double sign(double x);

#endif