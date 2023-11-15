include(`vector.m4')
#ifndef QUICKWAVE_BUFFER
#define QUICKWAVE_BUFFER

#include <stdlib.h>
#include <complex.h>
#include "assertions.h"
#include <stdbool.h>

#define assert_valid_vector(vector) assert(vector != NULL && vector->elements != NULL)

define(`macro_make_vector_structure',
/**
 * @brief 
 * General fixed-length vector.
 */
`typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    $1 elements[]; /** Buffer elements */
} macro_vector_type($1);')dnl

macro_make_for_numeric_types(`macro_make_vector_structure')dnl

define(`macro_make_declare_vector_shift',
`/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
macro_make_vector_shift_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_shift')dnl

define(`macro_make_declare_vector_element',
`/**
 * @brief 
 * Returns vector element. Negative indices index backwards from the end of the vector.
 * @param index Index of element to be returned
 * @param buf vector to be accessed
 * @return Indexed element
 */
macro_make_vector_element_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_element')dnl

define(`macro_make_declare_vector_interpolated_element',
`/**
 * @brief 
 * Interpolates between the values of a vector
 * @param index generalized index to interpolate at. Can be between integer indices.
 * @param buf vector to be accessed
 * @return Interpolated element
 */
macro_make_vector_interpolated_element_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_interpolated_element')dnl

define(`macro_make_declare_vector_dot',
`macro_make_vector_dot_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_dot')dnl

define(`macro_make_declare_vector_scale',
`macro_make_vector_scale_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_scale')dnl

define(`macro_make_declare_vector_apply',
`macro_make_vector_scale_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_apply')

define(`macro_make_declare_vector_from_array',
`macro_make_vector_from_array_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_from_array')dnl

define(`macro_make_declare_vector_new',
`/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the vector
 * @return vector
 */
macro_make_vector_new_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_new')dnl

define(`macro_make_declare_vector_length',
`macro_make_vector_length_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_length')dnl

define(`macro_make_declare_vector_reverse',
`macro_make_vector_reverse_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_reverse')dnl

define(`macro_make_declare_vector_reset',
`/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
macro_make_vector_reset_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_reset')dnl

define(`macro_make_declare_vector_free',
`/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
macro_make_vector_free_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_free')dnl

define(`macro_make_declare_vector_duplicate',
`macro_make_vector_duplicate_prototype($1);')dnl

macro_make_for_numeric_types(`macro_make_declare_vector_duplicate')dnl

/**
 * @brief 
 * Performs Euclidean modular addition
 * @param a Summand
 * @param b Summand
 * @param max Divisor
 * @return Result
 */
int modular_add(int a, int b, int max);

/**
 * @brief 
 * Performs Euclidean modulus
 * @param a Dividend
 * @param b Divisor
 * @return Remainder
 */
int modulo_euclidean(int a, int b);

#endif
