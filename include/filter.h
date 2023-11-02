#ifndef SQLDSP_FILTER
#define SQLDSP_FILTER

#include "buffer.h"
#include "window.h"
#include <complex.h>

/**
 * @brief 
 * Linear filter. Can be IIR or FIR
 */
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

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double complex moving_sum;
    CircularBuffer *previous_input;
} MovingAverage;

/**
 * @brief 
 * Proportional-integral-derivative filter.
 * Useful for control applications.
 */
typedef struct {
    double complex previous_input;
    double complex accumulated_input;
    double complex proportional_gain;
    double complex integral_gain;
    double complex derivative_gain;
} Pid;

double complex filter_evaluate_digital_filter(double complex input, DigitalFilter *filter);
DigitalFilter *filter_make_digital_filter(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
);
DigitalFilter *filter_make_ewma(double alpha);
DigitalFilter *filter_make_first_order_iir(double cutoff_frequency);
DigitalFilter *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
);
DigitalFilter *filter_make_savgol(size_t window_length, int deriv, int polyorder);
void filter_reset_digital_filter(DigitalFilter *filter);
void filter_free_digital_filter(DigitalFilter *filter);

Pid filter_make_pid(
    double complex proportional_gain, 
    double complex integral_gain, 
    double complex derivative_gain
);
void filter_reset_pid(Pid *pid);
double complex filter_evaluate_pid(double complex input, Pid *pid);

MovingAverage *filter_make_moving_average(size_t length);
double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
);
void filter_reset_moving_average(MovingAverage *filter);
void filter_free_moving_average(MovingAverage *filter);

#endif