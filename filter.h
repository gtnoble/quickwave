#ifndef SQLDSP_FILTER
#define SQLDSP_FILTER

#include "buffer.h"
#include <complex.h>

enum filter_errors {
    FILTER_OK,
    FILTER_ERROR,
    ALLOCATION_FAILURE,
    IMPROPER_PARAMS
};
typedef struct {
    double complex *feedforward;
    size_t n_feedforward;
    CircularBuffer *previous_inputs;

    double complex *feedback;
    size_t n_feedback;
    CircularBuffer *previous_outputs;
    double complex current_output;

    int window_size;
} DigitalFilter;



double complex filter_evaluate(double complex input, DigitalFilter *filter);
DigitalFilter *filter_make(size_t n_feedforward, size_t n_feedback);
void filter_free(DigitalFilter *filter);
double complex filter_current_value(DigitalFilter *filter);
DigitalFilter *filter_make_savgol(size_t window_length, int deriv, int polyorder);
DigitalFilter *filter_make_first_order_iir(double cutoff_frequency);
DigitalFilter *filter_make_sinc(double cutoff_frequency, size_t length);

#endif