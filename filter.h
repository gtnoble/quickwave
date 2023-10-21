#ifndef SQLDSP_FILTER
#define SQLDSP_FILTER

#include "buffer.h"
#include "window.h"
#include <complex.h>

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

typedef struct {
    double complex moving_sum;
    CircularBuffer *previous_input;
} MovingAverage;

double complex filter_evaluate(double complex input, DigitalFilter *filter);
DigitalFilter *filter_make_digital_filter(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
);
void filter_free_digital_filter(DigitalFilter *filter);
DigitalFilter *filter_make_savgol(size_t window_length, int deriv, int polyorder);
DigitalFilter *filter_make_ewma(double alpha);
DigitalFilter *filter_make_first_order_iir(double cutoff_frequency);
DigitalFilter *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
);

#endif