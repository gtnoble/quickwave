#ifndef SQLDSP_BUFFER
#define SQLDSP_BUFFER

#include <stdlib.h>
#include <complex.h>

/**
 * @brief 
 * Circular buffer. Stores the n_elements most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements;
    size_t index;
    double complex buffer[];
} CircularBufferComplex;

/**
 * @brief 
 * Circular buffer. Stores the n_elements most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements;
    size_t index;
    double buffer[];
} CircularBufferReal;


/**
 * @brief 
 * Complex Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double complex circbuf_complex_shift(double complex element, CircularBufferComplex *buf);


/**
 * @brief 
 * Returns complex element at index. Negative indices represent previously inserted elements.
 * @param index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double complex *circbuf_complex_element(int index, CircularBufferComplex *buf);

/**
 * @brief 
 * Interpolates values of a complex circular buffer
 * @param index Generalized index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double complex circbuf_complex_interpolated_element(double index, CircularBufferComplex *buf);

/**
 * @brief 
 * Makes and allocates a new complex circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
CircularBufferComplex *circbuf_complex_new(size_t size);

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void circbuf_complex_reset(CircularBufferComplex *buf);

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void circbuf_complex_free(CircularBufferComplex *buf);


/**
 * @brief 
 * Real Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double circbuf_real_shift(double element, CircularBufferReal *buf);

/**
 * @brief 
 * Returns real element at index. Negative indices represent previously inserted elements.
 * @param index Index of element to be returned
 * @param buf Buffer to be accessed
 * @return Indexed element
 */
double *circbuf_real_element(int index, CircularBufferReal *buf);

/**
 * @brief 
 * Interpolates values of a real circular buffer
 * @param index Generalized index to interpolate at. Can be between integer indices.
 * @param buf Buffer to be accessed
 * @return Interpolated element
 */
double circbuf_real_interpolated_element(double index, CircularBufferReal *buf);

/**
 * @brief 
 * Makes and allocates a new real circular buffer
 * @param size Number of elements in the circular buffer
 * @return Circular buffer 
 */
CircularBufferReal *circbuf_real_new(size_t size);

/**
 * @brief 
 * Resets all values of the buffer to zero
 * @param buf Buffer to reset
 */
void circbuf_real_reset(CircularBufferReal *buf);

/**
 * @brief 
 * Frees circular buffer memory allocations
 * @param buf Buffer to be freed
 */
void circbuf_real_free(CircularBufferReal *buf);

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