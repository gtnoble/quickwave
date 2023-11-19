#ifndef QUICKWAVE_FFT
#define QUICKWAVE_FFT

#include "vector.h"

typedef struct {
    double *in_out_data;
    double *wave_table;
    int length;
    int *bit_reversal_work_area;
} FftComplex;

FftComplex *fft_make_fft_complex(int length);
void fft_free_fft_complex(FftComplex *fft);
void fft_fft(const VectorComplexDouble *data, VectorComplexDouble *spectrum, FftComplex *fft);
void fft_ifft(const VectorComplexDouble *spectrum, VectorComplexDouble *data, FftComplex *fft);

#endif