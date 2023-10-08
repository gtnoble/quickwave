#include "buffer.h"

enum filter_errors {
    FILTER_OK,
    FILTER_ERROR,
    ALLOCATION_FAILURE,
    IMPROPER_PARAMS
};
typedef struct {
    double* feedforward;
    size_t n_feedforward;
    CircularBuffer *previous_inputs;

    double* feedback;
    size_t n_feedback;
    CircularBuffer *previous_outputs;
    double current_output;

    int window_size;
} DigitalFilter;

typedef struct {
    DigitalFilter *loop_filter;
    double phase_accumulator;
} PhaseLockedLoop;

double filter_evaluate(double input, DigitalFilter *filter);
DigitalFilter *filter_make(size_t n_feedforward, size_t n_feedback);
void filter_free(DigitalFilter *filter);
double filter_current_value(DigitalFilter *filter);
int filter_make_savgol(DigitalFilter **filter, size_t window_length, int deriv, int polyorder);
int filter_make_first_order_iir(DigitalFilter **filter, double cutoff_frequency);
int pll_make(PhaseLockedLoop **pll, double loop_filter_cutoff);
double pll_update(double input, PhaseLockedLoop *pll);
void pll_free(PhaseLockedLoop *pll);