
#ifndef QUICKWAVE_FILTER
#define QUICKWAVE_FILTER

#include <complex.h>
#include "vector.h"
#include "window.h"


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
 * Linear time-invariant digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorComplexDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorComplexDouble *previous_input;
    VectorComplexDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorComplexDouble *previous_output;
} DigitalFilterComplexDouble;
/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double complex filter_evaluate_digital_filter_complex_double(double complex input, DigitalFilterComplexDouble *filter);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterComplexDouble *filter_make_digital_filter_complex_double(
    const VectorComplexDouble *feedforward,
    const VectorComplexDouble *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterComplexDouble *filter_make_ewma_complex_double(double alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterComplexDouble *filter_make_first_order_iir_complex_double(double cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterComplexDouble *filter_make_sinc_complex_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionDouble window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterComplexDouble *filter_make_savgol_complex_double(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex_double(DigitalFilterComplexDouble *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex_double(DigitalFilterComplexDouble *filter);




/**
 * @brief 
 * Linear time-invariant digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorComplexFloat *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorComplexFloat *previous_input;
    VectorComplexFloat *feedback; /** Feedback (IIR) terms of the filter */
    VectorComplexFloat *previous_output;
} DigitalFilterComplexFloat;
/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
float complex filter_evaluate_digital_filter_complex_float(float complex input, DigitalFilterComplexFloat *filter);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterComplexFloat *filter_make_digital_filter_complex_float(
    const VectorComplexFloat *feedforward,
    const VectorComplexFloat *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterComplexFloat *filter_make_ewma_complex_float(float alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterComplexFloat *filter_make_first_order_iir_complex_float(float cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterComplexFloat *filter_make_sinc_complex_float(
    float cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionFloat window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterComplexFloat *filter_make_savgol_complex_float(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex_float(DigitalFilterComplexFloat *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex_float(DigitalFilterComplexFloat *filter);




/**
 * @brief 
 * Linear time-invariant digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorRealDouble *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorRealDouble *previous_input;
    VectorRealDouble *feedback; /** Feedback (IIR) terms of the filter */
    VectorRealDouble *previous_output;
} DigitalFilterRealDouble;
/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double filter_evaluate_digital_filter_real_double(double input, DigitalFilterRealDouble *filter);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterRealDouble *filter_make_digital_filter_real_double(
    const VectorRealDouble *feedforward,
    const VectorRealDouble *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterRealDouble *filter_make_ewma_real_double(double alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterRealDouble *filter_make_first_order_iir_real_double(double cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterRealDouble *filter_make_sinc_real_double(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionDouble window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterRealDouble *filter_make_savgol_real_double(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real_double(DigitalFilterRealDouble *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real_double(DigitalFilterRealDouble *filter);




/**
 * @brief 
 * Linear time-invariant digital filter. Can be IIR or FIR
 */
typedef struct {
    VectorRealFloat *feedforward; /** Feedforward (FIR) terms of the filter */
    VectorRealFloat *previous_input;
    VectorRealFloat *feedback; /** Feedback (IIR) terms of the filter */
    VectorRealFloat *previous_output;
} DigitalFilterRealFloat;
/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
float filter_evaluate_digital_filter_real_float(float input, DigitalFilterRealFloat *filter);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterRealFloat *filter_make_digital_filter_real_float(
    const VectorRealFloat *feedforward,
    const VectorRealFloat *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterRealFloat *filter_make_ewma_real_float(float alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterRealFloat *filter_make_first_order_iir_real_float(float cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterRealFloat *filter_make_sinc_real_float(
    float cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunctionFloat window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterRealFloat *filter_make_savgol_real_float(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real_float(DigitalFilterRealFloat *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real_float(DigitalFilterRealFloat *filter);



#endif
