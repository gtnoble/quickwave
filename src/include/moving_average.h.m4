include(`moving_average.m4')
include(`code_generator.m4')
#ifndef QUICKWAVE_MOVING_AVERAGE
#define QUICKWAVE_MOVING_AVERAGE

#include <complex.h>
#include "vector.h"

define(`M4_DEFINE_DIGITAL_FILTER_STRUCT',
`/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    $1 moving_sum;
    M4_VECTOR_TYPE($1) *previous_input;
} M4_MOVING_AVERAGE_TYPE($1);')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_DEFINE_DIGITAL_FILTER_STRUCT')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_MOVING_AVERAGE_MAKE_PROTOTYPE',
`/**
 * @brief 
 * Makes and allocates a moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */')


M4_MAKE_FUNCTION_DECLARATIONS(`M4_MAKE_MOVING_AVERAGE_LENGTH_PROTOTYPE')

M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_MOVING_AVERAGE_EVALUATE_PROTOTYPE',
`/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */')


M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_MOVING_AVERAGE_RESET_PROTOTYPE',
`/**
 * @brief 
 * Resets a moving average filter to initial state
 * @param filter Filter to reset
 */')


M4_MAKE_FUNCTION_DECLARATIONS(
`M4_MAKE_MOVING_AVERAGE_FREE_PROTOTYPE',
`/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */')

#endif
