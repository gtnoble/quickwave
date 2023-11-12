#include <stdbool.h>
#include <math.h>
#include <complex.h>

#include "fftg.h"
#include "fft.h"
#include "assertions.h"
#include "vector.h"


static bool is_power_of_two(int n);

FftComplex *fft_make_fft_complex(int length) {
    assert(is_power_of_two(length));
    assert(length > 0);

    FftComplex *fft = malloc(sizeof(FftComplex));
    if (fft == NULL)
        goto fft_allocation_failure;
    
    fft->in_out_data = malloc(sizeof(double) * 2 * length);
    if (fft->in_out_data == NULL)
        goto data_allocation_failure;

    int bit_reversal_work_area_length = 2+(1<<(int)(log(length+0.5)/log(2))/2);
    fft->bit_reversal_work_area = malloc(sizeof(int) * bit_reversal_work_area_length);
    if (fft->bit_reversal_work_area == NULL)
        goto bit_reversal_allocation_failure;
    
    fft->wave_table = malloc(sizeof(double) * (length / 2));
    if (fft->wave_table == NULL)
        goto wave_table_allocation_failure;

    fft->length = length;

    return fft;

    wave_table_allocation_failure:
        free(fft->bit_reversal_work_area);
    bit_reversal_allocation_failure:
        free(fft->in_out_data);
    data_allocation_failure:
        free(fft);
    fft_allocation_failure:
        return NULL;
}

void fft_free_fft_complex(FftComplex *fft) {
    free(fft->bit_reversal_work_area);
    free(fft->in_out_data);
    free(fft->wave_table);
    free(fft);
}

void fft_fft(VectorComplex *data, FftComplex *fft) {
    assert_not_null(data);
    assert(is_power_of_two(vector_length_generic(data)));
    assert_not_null(fft);

    double *in_data = fft->in_out_data;
    double *out_data = fft->in_out_data; //+ fft->length / 2;

    for (size_t i = 0; i < vector_length_generic(data); i++) {
        double complex input_element = *vector_element_generic(i, data);
        in_data[i * 2] = creal(input_element);
        in_data[i * 2 + 1] = cimag(input_element);
    }

    cdft(
        fft->length * 2, 
        FORWARD_TRANSFORM, 
        fft->in_out_data, 
        fft->bit_reversal_work_area, 
        fft->wave_table
    );

    for (size_t i = 0; i < vector_length_generic(data); i++) {
        *vector_element_generic(i, data) = 
            CMPLX(out_data[2 * i], out_data[2 * i + 1]);
    }
}

void fft_ifft(VectorComplex *data, FftComplex *fft) {
    vector_complex_apply(conj, data);
    fft_fft(data, fft);
    vector_complex_apply(conj, data);
    vector_complex_scale(1.0 / vector_length_generic(data), data);
}

static bool is_power_of_two(int n) {
    return (n & (n - 1)) == 0;
}