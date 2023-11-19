#include <stdbool.h>

#include "stft.h"

ShortTimeFourierTransform *stft_make(int segment_length, int segment_overlap) {
    ShortTimeFourierTransform *stft = malloc(sizeof(ShortTimeFourierTransform));
    if (stft == NULL)
        goto stft_allocation_failure;

    stft->edge_overlap = segment_overlap;
    stft->sample_count = 0;
    stft->fft = fft_make_fft_complex(segment_length);
    if (stft->fft == NULL)
        goto fft_allocation_failure;

    stft->segment = vector_new_complex_double(segment_length);
    if (stft->segment == NULL)
        goto segment_allocation_failure;

    return stft;

    segment_allocation_failure:
        fft_free_fft_complex(stft->fft);
    fft_allocation_failure:
        free(stft);
    stft_allocation_failure:
        return NULL;
}

void stft_free(ShortTimeFourierTransform *stft) {
    fft_free_fft_complex(stft->fft);
    vector_free_complex_double(stft->segment);
    free(stft);
}

bool stft_evaluate(
    double complex input, 
    VectorComplexDouble *spectrum,
    ShortTimeFourierTransform *stft
) {
    vector_shift_complex_double(input, stft->segment);
    stft->sample_count++;
    if (stft->sample_count % stft->edge_overlap == 0) {
        fft_fft(stft->segment, spectrum, stft->fft);
        return true;
    }
    return false;
}