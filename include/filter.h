#ifndef QUICKWAVE_FILTER
#define QUICKWAVE_FILTER

#include <complex.h>
#include "vector.h"
#include "window.h"

/**
 * @brief 
 * Complex-valued linear filter. Can be IIR or FIR
 */
typedef struct {
    VectorComplexDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorComplexDouble *previous_input;
    VectorComplexDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorComplexDouble *previous_output;
} DigitalFilterComplex;

/**
 * @brief 
 * Real-valued linear filter. Can be IIR or FIR
 */
typedef struct {
    VectorRealDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorRealDouble *previous_input;
    VectorRealDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorRealDouble *previous_output;
} DigitalFilterReal;

/**
 * @brief 
 * Specifies the nature of the filter stop-band
 */
enum FilterType {
    LOW_PASS, /** Low pass filter */
    HIGH_PASS /** High pass filter */
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
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterComplex *filter_make_digital_filter_complex(
    const VectorComplexDouble *feedforward,
    const VectorComplexDouble *feedback
);

/**
 * @brief 
 * Makes and allocates a real linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterReal *filter_make_digital_filter_real(
    const VectorRealDouble *feedforward,
    const VectorRealDouble *feedback
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