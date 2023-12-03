
#ifndef QUICKWAVE_FFT
#define QUICKWAVE_FFT

#include "vector.h"
#include "memory.h"


typedef struct {
    double *in_out_data;
    double *wave_table;
    int length;
    int *bit_reversal_work_area;
    Deallocator *free;
} FftComplexDouble;

FftComplexDouble *fft_make_fft_double(int length, const MemoryManager *manager);
void fft_free_fft_double(FftComplexDouble *fft);
void fft_fft_double(VectorComplexDouble *data, FftComplexDouble *fft);
void fft_ifft_double(VectorComplexDouble *data, FftComplexDouble *fft);




typedef struct {
    float *in_out_data;
    float *wave_table;
    int length;
    int *bit_reversal_work_area;
    Deallocator *free;
} FftComplexFloat;

FftComplexFloat *fft_make_fft_float(int length, const MemoryManager *manager);
void fft_free_fft_float(FftComplexFloat *fft);
void fft_fft_float(VectorComplexFloat *data, FftComplexFloat *fft);
void fft_ifft_float(VectorComplexFloat *data, FftComplexFloat *fft);


#endif
