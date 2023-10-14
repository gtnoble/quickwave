#include "filter.h"
#include <stdbool.h>

typedef struct {
    DigitalFilter *loop_filter;
    double phase_accumulator;
    CircularBuffer *lagged_input;
} PhaseLockedLoop;

int pll_make(PhaseLockedLoop **pll, double loop_filter_cutoff, double initial_freq, double min_freq);
double pll_update(double input, PhaseLockedLoop *pll);
void pll_free(PhaseLockedLoop *pll);

double sign(double x);
double update_vco(double angular_freq, double *phase_accumulator, bool allow_negative_freqs);