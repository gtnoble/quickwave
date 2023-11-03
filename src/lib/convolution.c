#include <string.h>
#include "convolution.h"
#include "assertions.h"

ConvolutionComplex *convolution_make_complex(
    size_t n_stationary_elements, 
    const double complex stationary_elements[]
) {
    CircularBufferComplex *sliding_elements = circbuf_complex_new(n_stationary_elements);
    if (sliding_elements == NULL) {
        return NULL;
    }

    size_t stationary_elements_size = sizeof(double complex) * n_stationary_elements;
    ConvolutionComplex *convolution = malloc(
        sizeof(ConvolutionComplex) + stationary_elements_size
    );

    if (convolution == NULL) {
        circbuf_complex_free(sliding_elements);
        return NULL;
    }

    convolution->n_stationary_elements = n_stationary_elements;
    convolution->sliding_elements = sliding_elements;
    memcpy(
        convolution->stationary_elements, 
        stationary_elements, 
        stationary_elements_size
    );

    return convolution;
}

ConvolutionReal *convolution_make_real(
    size_t n_stationary_elements, 
    const double stationary_elements[]
) {
    CircularBufferReal *sliding_elements = circbuf_real_new(n_stationary_elements);
    if (sliding_elements == NULL) {
        return NULL;
    }

    size_t stationary_elements_size = sizeof(double) * n_stationary_elements;
    ConvolutionReal *convolution = malloc(
        sizeof(ConvolutionReal) + stationary_elements_size
    );

    if (convolution == NULL) {
        circbuf_real_free(sliding_elements);
        return NULL;
    }

    convolution->n_stationary_elements = n_stationary_elements;
    convolution->sliding_elements = sliding_elements;
    memcpy(
        convolution->stationary_elements, 
        stationary_elements, 
        stationary_elements_size
    );

    return convolution;
}

void convolution_reset_complex(ConvolutionComplex *convolution) {
    assert_not_null(convolution);
    circbuf_complex_reset(convolution->sliding_elements);
}

void convolution_reset_real(ConvolutionReal *convolution) {
    assert_not_null(convolution);
    circbuf_real_reset(convolution->sliding_elements);
}

void convolution_free_complex(ConvolutionComplex *convolution) {
    assert_not_null(convolution);
    circbuf_complex_free(convolution->sliding_elements);
    free(convolution);
}

void convolution_free_real(ConvolutionReal *convolution) {
    assert_not_null(convolution);
    circbuf_real_free(convolution->sliding_elements);
    free(convolution);
}

double complex convolution_convolve_complex(
    double complex input, 
    ConvolutionComplex *convolution
) {
    assert_not_null(convolution);

    double complex accumulate = 0;
    assert(convolution->sliding_elements != NULL);
    circbuf_complex_shift(input, convolution->sliding_elements);
    for (size_t i = 0; i < convolution->n_stationary_elements; i++) {
        accumulate +=  *circbuf_complex_element(-i, convolution->sliding_elements) *
            convolution->stationary_elements[i];
    }

    return accumulate;
}

double convolution_convolve_real(
    double input, 
    ConvolutionReal *convolution
) {
    assert_not_null(convolution);

    double accumulate = 0;
    assert(convolution->sliding_elements != NULL);
    circbuf_real_shift(input, convolution->sliding_elements);
    for (size_t i = 0; i < convolution->n_stationary_elements; i++) {
        accumulate +=  *circbuf_real_element(-i, convolution->sliding_elements) *
            convolution->stationary_elements[i];
    }

    return accumulate;
}