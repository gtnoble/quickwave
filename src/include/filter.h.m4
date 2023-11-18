include(filter.m4)
include(code_generator.m4)

#ifndef QUICKWAVE_FILTER
#define QUICKWAVE_FILTER

#include <complex.h>
#include "vector.h"
#include "window.h"

define(`M4_DEFINE_DIGITAL_FILTER_STRUCT',
`/**
 * @brief 
 * Linear digital filter. Can be IIR or FIR
 */
typedef struct {
    M4_VECTOR_TYPE($1) *feedforward; /** Feedforward (FIR) terms of the filter */
    M4_VECTOR_TYPE($1) *previous_input;
    M4_VECTOR_TYPE($1) *feedback; /** Feedback (IIR) terms of the filter */
    M4_VECTOR_TYPE($1) *previous_output;
} M4_DIGITAL_FILTER_TYPE($1);')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_DEFINE_DIGITAL_FILTER_STRUCT')

/**
 * @brief 
 * Specifies the nature of the filter stop-band
 */
enum FilterType {
    LOW_PASS, /** Low pass filter */
    HIGH_PASS /** High pass filter */
};

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER_PROTOTYPE',
`/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to evaluate
 * @return Filtered value
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER_PROTOTYPE',
`/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param feedforward Feedforward coefficient values
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_MAKE_EWMA_PROTOTYPE',
`/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR_PROTOTYPE',
`/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_MAKE_SINC_PROTOTYPE',
`/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param filter_type The type of the filter. Can be low-pass or high-pass
 * @param window Windowing function
 * @return Constructed filter
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_MAKE_SAVGOL_PROTOTYPE',
`/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_RESET_DIGITAL_FILTER_PROTOTYPE',
`/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_FILTER_FREE_DIGITAL_FILTER_PROTOTYPE',
`/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */')

#endif
