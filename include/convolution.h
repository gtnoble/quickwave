#ifndef QUICKWAVE_CONVOLUTION
#define QUICKWAVE_CONVOLUTION

#include <stdlib.h>
#include <complex.h>
#include "buffer.h"


typedef struct {
    size_t n_stationary_elements;
    CircularBufferComplex *sliding_elements;
    double complex stationary_elements[];
} ConvolutionComplex;

typedef struct {
    size_t n_stationary_elements;
    CircularBufferReal *sliding_elements;
    double stationary_elements[];
} ConvolutionReal;


ConvolutionComplex *convolution_make_complex(
    size_t n_stationary_elements, 
    const double complex stationary_elements[]
);
ConvolutionReal *convolution_make_real(
    size_t n_stationary_elements, 
    const double stationary_elements[]
);

void convolution_reset_complex(ConvolutionComplex *convolution);
void convolution_reset_real(ConvolutionReal *convolution);
void convolution_free_complex(ConvolutionComplex *convolution);
void convolution_free_real(ConvolutionReal *convolution);

double complex convolution_convolve_complex(
    double complex input, 
    ConvolutionComplex *convolution
);
double convolution_convolve_real(
    double input, 
    ConvolutionReal *convolution
);

#endif