#ifndef SQLDSP_FILTER
#define SQLDSP_FILTER

#include "buffer.h"
#include "window.h"
#include <complex.h>

typedef struct {
    size_t n_stationary_elements;
    CircularBufferComplex *sliding_elements;
    double complex stationary_elements[];
} ConvolutionComplex;
typedef struct {
    size_t n_stationary_elements;
    CircularBufferReal *sliding_elements;
    double stationary_elements[];
} ConvolutionReal;

/**
 * @brief 
 * Linear filter. Can be IIR or FIR
 */
typedef struct {
    ConvolutionComplex *feedforward;
    ConvolutionComplex *feedback;
    double complex previous_output;
} DigitalFilterComplex;

/**
 * @brief 
 * Linear filter. Can be IIR or FIR
 */
typedef struct {
    ConvolutionReal *feedforward;
    ConvolutionReal *feedback;
    double previous_output;
} DigitalFilterReal;

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double complex moving_sum;
    CircularBufferComplex *previous_input;
} MovingAverage;

/**
 * @brief 
 * Proportional-integral-derivative filter.
 * Useful for control applications.
 */
typedef struct {
    double previous_input;
    double accumulated_input;
    double proportional_gain;
    double integral_gain;
    double derivative_gain;
} Pid;

ConvolutionComplex *filter_make_convolution_complex(
    size_t n_stationary_elements, 
    const double complex stationary_elements[]
);
void filter_reset_convolution_complex(ConvolutionComplex *convolution);
void filter_free_convolution_complex(ConvolutionComplex *convolution);
double complex filter_convolve_complex(
    double complex input, 
    ConvolutionComplex *convolution
);
double filter_convolve_real(
    double input, 
    ConvolutionReal *convolution
);

double complex filter_evaluate_digital_filter_complex(double complex input, DigitalFilterComplex *filter);
double filter_evaluate_digital_filter_real(double input, DigitalFilterReal *filter);
DigitalFilterComplex *filter_make_digital_filter_complex(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
);
DigitalFilterComplex *filter_make_ewma(double alpha);
DigitalFilterComplex *filter_make_first_order_iir(double cutoff_frequency);
DigitalFilterComplex *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
);
DigitalFilterComplex *filter_make_savgol(size_t window_length, int deriv, int polyorder);
void filter_reset_digital_filter_complex(DigitalFilterComplex *filter);
void filter_free_digital_filter_complex(DigitalFilterComplex *filter);

Pid filter_make_pid(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain
);
void filter_reset_pid(Pid *pid);
double complex filter_evaluate_pid(double input, Pid *pid);

MovingAverage *filter_make_moving_average(size_t length);
double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
);
void filter_reset_moving_average(MovingAverage *filter);
void filter_free_moving_average(MovingAverage *filter);

#endif