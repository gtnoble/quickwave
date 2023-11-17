


#ifndef QUICKWAVE_FILTER
#define QUICKWAVE_FILTER

#include <complex.h>
#include "vector.h"
#include "window.h"



/**
 * @brief 
 * Linear digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorComplexDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorComplexDouble *previous_input;
    VectorComplexDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorComplexDouble *previous_output;
} FilterComplexDouble;

/**
 * @brief 
 * Linear digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorComplexFloat *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorComplexFloat *previous_input;
    VectorComplexFloat *feedback; /** Feedback (IIR) terms of the filter */
    VectorComplexFloat *previous_output;
} FilterComplexFloat;

/**
 * @brief 
 * Linear digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorRealDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorRealDouble *previous_input;
    VectorRealDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorRealDouble *previous_output;
} FilterRealDouble;

/**
 * @brief 
 * Linear digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorRealFloat *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorRealFloat *previous_input;
    VectorRealFloat *feedback; /** Feedback (IIR) terms of the filter */
    VectorRealFloat *previous_output;
} FilterRealFloat;

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
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
filter_evaluate_digital_filter_complex_double(
    double complex input, 
    FilterComplexDouble *filter);

/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
filter_evaluate_digital_filter_complex_float(
    float complex input, 
    FilterComplexFloat *filter);

/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
filter_evaluate_digital_filter_real_double(
    double input, 
    FilterRealDouble *filter);

/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
filter_evaluate_digital_filter_real_float(
    float input, 
    FilterRealFloat *filter);


/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
FilterComplexDouble *filter_make_digital_filter_complex_double(
    const VectorComplexDouble *feedforward,
    const VectorComplexDouble *feedback
);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
FilterComplexFloat *filter_make_digital_filter_complex_float(
    const VectorComplexFloat *feedforward,
    const VectorComplexFloat *feedback
);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
FilterRealDouble *filter_make_digital_filter_real_double(
    const VectorRealDouble *feedforward,
    const VectorRealDouble *feedback
);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
FilterRealFloat *filter_make_digital_filter_real_float(
    const VectorRealFloat *feedforward,
    const VectorRealFloat *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
FilterComplexDouble *filter_make_ewma_complex_double(double complex alpha);

/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
FilterComplexFloat *filter_make_ewma_complex_float(float complex alpha);

/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
FilterRealDouble *filter_make_ewma_real_double(double alpha);

/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
FilterRealFloat *filter_make_ewma_real_float(float alpha);


/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
FilterComplexDouble *filter_make_first_order_iir_complex_double(double complex cutoff_frequency);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
FilterComplexFloat *filter_make_first_order_iir_complex_float(float complex cutoff_frequency);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
FilterRealDouble *filter_make_first_order_iir_real_double(double cutoff_frequency);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
FilterRealFloat *filter_make_first_order_iir_real_float(float cutoff_frequency);


/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
FilterComplexDouble *filter_make_sinc_complex_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
FilterComplexFloat *filter_make_sinc_complex_float(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
FilterRealDouble *filter_make_sinc_real_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
FilterRealFloat *filter_make_sinc_real_float(
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
FilterComplexDouble *filter_make_savgol_complex_double(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
FilterComplexFloat *filter_make_savgol_complex_float(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
FilterRealDouble *filter_make_savgol_real_double(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
FilterRealFloat *filter_make_savgol_real_float(size_t window_length, int deriv, int polyorder);


/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex_double(FilterComplexDouble *filter);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex_float(FilterComplexFloat *filter);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real_double(FilterRealDouble *filter);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real_float(FilterRealFloat *filter);


/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex_double(FilterComplexDouble *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex_float(FilterComplexFloat *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real_double(FilterRealDouble *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real_float(FilterRealFloat *filter);

#endif
