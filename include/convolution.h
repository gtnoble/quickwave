#ifndef QUICKWAVE_CONVOLUTION
#define QUICKWAVE_CONVOLUTION

#include <stdlib.h>
#include <complex.h>
#include "buffer.h"

/**
 * @brief 
 * Stores state for complex-valued streaming convolution.
 * The next convolved sample is calculated as input samples are added to `sliding elements`
 */
typedef struct {
    size_t n_stationary_elements; /** Number of stationary elements */
    CircularBufferComplex *sliding_elements; /** Stores sliding elements. Updates for each input sample. */
    double complex stationary_elements[]; /** Stores stationary elements */
} ConvolutionComplex;

/**
 * @brief 
 * Stores state for real-valued streaming convolution
 * The next convolved sample is calculated as input samples are added to `sliding elements`
 */
typedef struct {
    size_t n_stationary_elements; /** Number of stationary elements */
    CircularBufferReal *sliding_elements; /** Stores sliding elements. Updates for each input sample. */
    double stationary_elements[]; /** Stores stationary elements */
} ConvolutionReal;

/**
 * @brief 
 * Makes a complex-valued streaming convolution
 * @param n_stationary_elements Number of elements in stationary array
 * @param stationary_elements Stationary elemetn array
 * @return Complex-valued streaming convolution
 */
ConvolutionComplex *convolution_make_complex(
    size_t n_stationary_elements, 
    const double complex stationary_elements[]
);

/**
 * @brief 
 * Makes a real-valued streaming convolution
 * @param n_stationary_elements Number of elements in stationary array
 * @param stationary_elements Stationary elemetn array
 * @return Real-valued streaming convolution
 */
ConvolutionReal *convolution_make_real(
    size_t n_stationary_elements, 
    const double stationary_elements[]
);

/**
 * @brief 
 * Resets a complex-valued streaming convolution to its initial state
 * @param convolution Complex-valued streaming convolution
 */
void convolution_reset_complex(ConvolutionComplex *convolution);

/**
 * @brief 
 * Resets a real-valued streaming convolution to its initial state
 * @param convolution Real-valued streaming convolution
 */
void convolution_reset_real(ConvolutionReal *convolution);

/**
 * @brief 
 * Frees the memory associated with a complex-valued streaming convolution
 * @param convolution Complex-valued streaming convolution
 */
void convolution_free_complex(ConvolutionComplex *convolution);

/**
 * @brief 
 * Frees the memory associated with a real-valued streaming convolution
 * @param convolution real-valued streaming convolution
 */
void convolution_free_real(ConvolutionReal *convolution);

/**
 * @brief 
 * Convolves a complex-valued input sample.
 * @param input Input sample
 * @param convolution Complex-valued streaming convolution
 * @return Convolved sample
 */
double complex convolution_convolve_complex(
    double complex input, 
    ConvolutionComplex *convolution
);

/**
 * @brief 
 * Convolves a real-valued input sample.
 * @param input Input sample
 * @param convolution real-valued streaming convolution
 * @return Convolved sample
 */
double convolution_convolve_real(
    double input, 
    ConvolutionReal *convolution
);

#endif