include(`vector.m4')
include(`code_generator.m4')
#ifndef QUICKWAVE_BUFFER
#define QUICKWAVE_BUFFER

#include <stdlib.h>
#include <complex.h>
#include "assertions.h"
#include <stdbool.h>

#define assert_valid_vector(vector) assert(vector != NULL && vector->elements != NULL)

define(`M4_MAKE_VECTOR_STRUCTURE',
/**
 * @brief 
 * General fixed-length vector.
 */
`typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    $1 elements[]; /** Buffer elements */
} M4_VECTOR_TYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_VECTOR_STRUCTURE')dnl

define(`M4_MAKE_DECLARE_VECTOR_SHIFT',
`/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
M4_MAKE_VECTOR_SHIFT_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_SHIFT')dnl

define(`M4_MAKE_DECLARE_VECTOR_ELEMENT',
`/**
 * @brief 
 * Returns vector element. Negative indices index backwards from the end of the vector.
 * @param index Index of element to be returned
 * @param buf vector to be accessed
 * @return Indexed element
 */
M4_MAKE_VECTOR_ELEMENT_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_ELEMENT')dnl

M4_MAKE_FUNCTION_DECLARATIONS(`M4_MAKE_VECTOR_ELEMENT_VALUE_PROTOTYPE')

define(`M4_MAKE_DECLARE_VECTOR_INTERPOLATED_ELEMENT',
`/**
 * @brief 
 * Interpolates between the values of a vector
 * @param index generalized index to interpolate at. Can be between integer indices.
 * @param buf vector to be accessed
 * @return Interpolated element
 */
M4_MAKE_VECTOR_INTERPOLATED_ELEMENT_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_INTERPOLATED_ELEMENT')dnl

define(`M4_MAKE_DECLARE_VECTOR_DOT',
`M4_MAKE_VECTOR_DOT_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_DOT')dnl

define(`M4_MAKE_DECLARE_VECTOR_SCALE',
`M4_MAKE_VECTOR_SCALE_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_SCALE')dnl

define(`M4_MAKE_DECLARE_VECTOR_APPLY',
`M4_MAKE_VECTOR_APPLY_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_APPLY')

define(`M4_MAKE_DECLARE_VECTOR_FROM_ARRAY',
`M4_MAKE_VECTOR_FROM_ARRAY_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_FROM_ARRAY')dnl

define(`M4_MAKE_DECLARE_VECTOR_NEW',
`/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the vector
 * @return vector
 */
M4_MAKE_VECTOR_NEW_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_NEW')dnl

define(`M4_MAKE_DECLARE_VECTOR_LENGTH',
`M4_MAKE_VECTOR_LENGTH_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_LENGTH')dnl

define(`M4_MAKE_DECLARE_VECTOR_REVERSE',
`M4_MAKE_VECTOR_REVERSE_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_REVERSE')dnl

define(`M4_MAKE_DECLARE_VECTOR_RESET',
`/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
M4_MAKE_VECTOR_RESET_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_RESET')dnl

define(`M4_MAKE_DECLARE_VECTOR_FREE',
`/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
M4_MAKE_VECTOR_FREE_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_FREE')dnl

define(`M4_MAKE_DECLARE_VECTOR_DUPLICATE',
`M4_MAKE_VECTOR_DUPLICATE_PROTOTYPE($1);')dnl

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_DECLARE_VECTOR_DUPLICATE')dnl

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
