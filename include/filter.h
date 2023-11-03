#ifndef SQLDSP_FILTER
#define SQLDSP_FILTER

#include <complex.h>
#include "buffer.h"
#include "window.h"
#include "convolution.h"

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

double complex filter_evaluate_digital_filter_complex(double complex input, DigitalFilterComplex *filter);
double filter_evaluate_digital_filter_real(double input, DigitalFilterReal *filter);

DigitalFilterComplex *filter_make_digital_filter_complex(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
);
DigitalFilterReal *filter_make_digital_filter_real(
    size_t n_feedforward, 
    const double feedforward[],
    size_t n_feedback,
    const double feedback[]
);

DigitalFilterComplex *filter_make_ewma(double alpha);
DigitalFilterComplex *filter_make_first_order_iir(double cutoff_frequency);
DigitalFilterReal *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
);
DigitalFilterReal *filter_make_savgol(size_t window_length, int deriv, int polyorder);

void filter_reset_digital_filter_complex(DigitalFilterComplex *filter);
void filter_reset_digital_filter_real(DigitalFilterReal *filter);
void filter_free_digital_filter_complex(DigitalFilterComplex *filter);
void filter_free_digital_filter_real(DigitalFilterReal *filter);

#endif