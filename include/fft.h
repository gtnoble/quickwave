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
void fft_fft(VectorComplexDouble *data, FftComplex *fft);
void fft_ifft(VectorComplexDouble *data, FftComplex *fft);

#endif