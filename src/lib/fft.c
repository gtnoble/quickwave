
#include <stdbool.h>
#include <math.h>
#include <complex.h>

#include "fftg.h"
#include "fft.h"
#include "assertions.h"
#include "vector.h"


static bool is_power_of_two(int n);


FftComplexDouble *fft_make_fft_double(int length) {
    assert(is_power_of_two(length));
    assert(length > 0);

    FftComplexDouble *fft = malloc(sizeof(FftComplexDouble));
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

void fft_free_fft_double(FftComplexDouble *fft) {
    free(fft->bit_reversal_work_area);
    free(fft->in_out_data);
    free(fft->wave_table);
    free(fft);
}

void fft_fft_double(VectorComplexDouble *data, FftComplexDouble *fft) {
    assert_not_null(data);
    assert(is_power_of_two(vector_length_complex_double(data)));
    assert_not_null(fft);

    double *in_data = fft->in_out_data;
    double *out_data = fft->in_out_data; //+ fft->length / 2;

    for (size_t i = 0; i < vector_length_complex_double(data); i++) {
        double complex input_element = *vector_element_complex_double(i, data);
        in_data[i * 2] = creal(input_element);
        in_data[i * 2 + 1] = cimag(input_element);
    }

    cdft_double(
        fft->length * 2, 
        FORWARD_TRANSFORM, 
        fft->in_out_data, 
        fft->bit_reversal_work_area, 
        fft->wave_table
    );

    for (size_t i = 0; i < vector_length_complex_double(data); i++) {
        *vector_element_complex_double(i, data) = 
            CMPLX(out_data[2 * i], out_data[2 * i + 1]);
    }
}

void fft_ifft_double(VectorComplexDouble *data, FftComplexDouble *fft) {
    vector_apply_complex_double(conj, data);
    fft_fft_double(data, fft);
    vector_apply_complex_double(conj, data);
    vector_scale_complex_double(1.0 / vector_length_complex_double(data), data);
}




FftComplexFloat *fft_make_fft_float(int length) {
    assert(is_power_of_two(length));
    assert(length > 0);

    FftComplexFloat *fft = malloc(sizeof(FftComplexFloat));
    if (fft == NULL)
        goto fft_allocation_failure;
    
    fft->in_out_data = malloc(sizeof(float) * 2 * length);
    if (fft->in_out_data == NULL)
        goto data_allocation_failure;

    int bit_reversal_work_area_length = 2+(1<<(int)(log(length+0.5)/log(2))/2);
    fft->bit_reversal_work_area = malloc(sizeof(int) * bit_reversal_work_area_length);
    if (fft->bit_reversal_work_area == NULL)
        goto bit_reversal_allocation_failure;
    
    fft->wave_table = malloc(sizeof(float) * (length / 2));
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

void fft_free_fft_float(FftComplexFloat *fft) {
    free(fft->bit_reversal_work_area);
    free(fft->in_out_data);
    free(fft->wave_table);
    free(fft);
}

void fft_fft_float(VectorComplexFloat *data, FftComplexFloat *fft) {
    assert_not_null(data);
    assert(is_power_of_two(vector_length_complex_float(data)));
    assert_not_null(fft);

    float *in_data = fft->in_out_data;
    float *out_data = fft->in_out_data; //+ fft->length / 2;

    for (size_t i = 0; i < vector_length_complex_float(data); i++) {
        float complex input_element = *vector_element_complex_float(i, data);
        in_data[i * 2] = crealf(input_element);
        in_data[i * 2 + 1] = cimagf(input_element);
    }

    cdft_float(
        fft->length * 2, 
        FORWARD_TRANSFORM, 
        fft->in_out_data, 
        fft->bit_reversal_work_area, 
        fft->wave_table
    );

    for (size_t i = 0; i < vector_length_complex_float(data); i++) {
        *vector_element_complex_float(i, data) = 
            CMPLXF(out_data[2 * i], out_data[2 * i + 1]);
    }
}

void fft_ifft_float(VectorComplexFloat *data, FftComplexFloat *fft) {
    vector_apply_complex_float(conjf, data);
    fft_fft_float(data, fft);
    vector_apply_complex_float(conjf, data);
    vector_scale_complex_float(1.0 / vector_length_complex_float(data), data);
}


static bool is_power_of_two(int n) {
    return (n & (n - 1)) == 0;
}
