#ifndef QUICKWAVE_BUFFER
#define QUICKWAVE_BUFFER

#include <stdlib.h>
#include <complex.h>
#include "assertions.h"
#include <stdbool.h>

#define assert_valid_vector(vector) assert(vector != NULL && vector->elements != NULL)

#define vector_free_generic(vector) \
    _Generic((vector), VectorReal*: vector_real_free, VectorComplex*: vector_complex_free)(vector)
#define vector_shift_generic(input, vector) \
    _Generic((vector), VectorReal*: vector_real_shift, VectorComplex*: vector_complex_shift)(input, vector)
#define vector_dot_generic(a, b) \
    _Generic((a), VectorReal*: vector_real_dot, VectorComplex*: vector_complex_dot)(a, b)
#define vector_element_generic(index, vector) \
    _Generic((vector), VectorReal*: vector_real_element, VectorComplex*: vector_complex_element)(index, vector)
#define vector_reverse_generic(vector) \
    _Generic((vector), VectorReal*: vector_real_reverse, VectorComplex*: vector_complex_reverse)(vector)
#define vector_duplicate_generic(vector) \
    _Generic((vector), \
        VectorReal*: vector_real_duplicate, \
        const VectorReal*: vector_real_duplicate, \
        VectorComplex*: vector_complex_duplicate, \
        const VectorComplex*: vector_complex_duplicate \
    )(vector)
#define vector_length_generic(vector) \
    _Generic((vector),  \
        VectorReal*: vector_real_length, \
        const VectorReal*: vector_real_length, \
        VectorComplex*: vector_complex_length, \
        const VectorComplex*: vector_complex_length \
    )(vector)
#define vector_reset_generic(vector) \
    _Generic((vector), VectorReal*: vector_real_reset, VectorComplex*: vector_complex_reset)(vector)

/**
 * @brief 
 * Complex-valued circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    double complex elements[]; /** Buffer elements */
} VectorComplex;

/**
 * @brief 
 * Real-valued circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elmeents in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    double elements[]; /** Buffer elements */
} VectorReal;

/**
 * @brief 
 * Complex Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double complex vector_complex_shift(double complex element, VectorComplex *buf);


/**
 * @brief 
 * Returns complex element at last_element_index. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double complex *vector_complex_element(int last_element_index, VectorComplex *buf);

/**
 * @brief 
 * Interpolates values of a complex circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double complex vector_complex_interpolated_element(double index, VectorComplex *buf);

double complex vector_complex_dot(VectorComplex *a, VectorComplex *b);

/**
 * @brief 
 * Makes and allocates a new complex circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
VectorComplex *vector_complex_new(size_t size);

VectorComplex *vector_complex_duplicate(const VectorComplex *vector);


size_t vector_complex_length(const VectorComplex *buf);

void vector_complex_reverse(VectorComplex *vector);

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void vector_complex_reset(VectorComplex *buf);

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void vector_complex_free(VectorComplex *buf);


/**
 * @brief 
 * Real Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double vector_real_shift(double element, VectorReal *buf);

/**
 * @brief 
 * Returns real element at last_element_index. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double *vector_real_element(int index, VectorReal *buf);

/**
 * @brief 
 * Interpolates values of a real circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double vector_real_interpolated_element(double index, VectorReal *buf);

double vector_real_dot(VectorReal *a, VectorReal *b);

/**
 * @brief 
 * Makes and allocates a new real circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
VectorReal *vector_real_new(size_t size);

VectorReal *vector_real_duplicate(const VectorReal *vector);

size_t vector_real_length(const VectorReal *buf);

void vector_real_reverse(VectorReal *vector);

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void vector_real_reset(VectorReal *buf);

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void vector_real_free(VectorReal *buf);

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