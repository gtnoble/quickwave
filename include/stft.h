#include <complex.h>

#include "vector.h"
#include "fft.h"

typedef struct {
    int sample_count;
    int edge_overlap;
    VectorComplexDouble *segment;
    FftComplex *fft;
} ShortTimeFourierTransform;

ShortTimeFourierTransform *stft_make(int segment_length, int segment_overlap);

void stft_free(ShortTimeFourierTransform *stft);

bool stft_evaluate(
    double complex input, 
    VectorComplexDouble *spectrum, 
    ShortTimeFourierTransform *stft
);