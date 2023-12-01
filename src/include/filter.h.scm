(load-from-path "substitutions.scm")

(output-text

"
#ifndef QUICKWAVE_FILTER
#define QUICKWAVE_FILTER

#include <complex.h>
#include \"vector.h\"
#include \"window.h\"


/**
 * @brief 
 * Specifies the nature of the filter stop-band
 */
enum FilterType {
    LOW_PASS, /** Low pass filter */
    HIGH_PASS /** High pass filter */
};

"

(generate-text
    filter-schema

"
/**
 * @brief 
 * Linear time-invariant digital filter. Can be IIR or FIR
 */
typedef struct {
    ${vector-type} *feedforward; /** Feedforward (FIR) terms of the filter */
    ${vector-type} *previous_input;
    ${vector-type} *feedback; /** Feedback (IIR) terms of the filter */
    ${vector-type} *previous_output;
} ${filter-type};
/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
${number-type} filter_evaluate_digital_filter${function-tag}(${number-type} input, ${filter-type} *filter);

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
${filter-type} *filter_make_digital_filter${function-tag}(
    const ${vector-type} *feedforward,
    const ${vector-type} *feedback
);


/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
${filter-type} *filter_make_ewma${function-tag}(${number-base-type} alpha);

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
${filter-type} *filter_make_first_order_iir${function-tag}(${number-base-type} cutoff_frequency);

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */
${filter-type} *filter_make_sinc${function-tag}(
    ${number-base-type} cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    ${window-function-type} window
);

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
${filter-type} *filter_make_savgol${function-tag}(size_t window_length, int deriv, int polyorder);

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter${function-tag}(${filter-type} *filter);

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter${function-tag}(${filter-type} *filter);

")

"

#endif
"
)