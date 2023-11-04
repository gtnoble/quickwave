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

enum FilterType {
    LOW_PASS,
    HIGH_PASS
};

/**
 * @brief 
 * Evaluates a complex linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double complex filter_evaluate_digital_filter_complex(double complex input, DigitalFilterComplex *filter);

/**
 * @brief 
 * Evaluates a real linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double filter_evaluate_digital_filter_real(double input, DigitalFilterReal *filter);

/**
 * @brief 
 * Makes and allocates a complex linear digital filter.
 * @param n_feedforward Number of filter feedforward coefficients
 * @param feedforward Feedforward coefficient values
 * @param n_feedback Number of filter feedback coefficients
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterComplex *filter_make_digital_filter_complex(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
);

/**
 * @brief 
 * Makes and allocates a real linear digital filter.
 * @param n_feedforward Number of filter feedforward coefficients
 * @param feedforward Feedforward coefficient values
 * @param n_feedback Number of filter feedback coefficients
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterReal *filter_make_digital_filter_real(
    size_t n_feedforward, 
    const double feedforward[],
    size_t n_feedback,
    const double feedback[]
);

/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterComplex *filter_make_ewma(double alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterComplex *filter_make_first_order_iir(double cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterReal *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterReal *filter_make_savgol(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a complex linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex(DigitalFilterComplex *filter);

/**
 * @brief 
 * Resets a real linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real(DigitalFilterReal *filter);

/**
 * @brief 
 * Frees memory associated with a complex linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex(DigitalFilterComplex *filter);

/**
 * @brief 
 * Frees memory associated with a real linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real(DigitalFilterReal *filter);

#endif