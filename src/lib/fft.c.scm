(load-from-path "substitutions.scm")

(output-text

"
#include <stdbool.h>
#include <math.h>
#include <complex.h>

#include \"fftg.h\"
#include \"fft.h\"
#include \"assertions.h\"
#include \"vector.h\"


static bool is_power_of_two(int n);

"

    (generate-text
        fft-schema
"
${fft-type} *fft_make_fft${function-tag}(int length) {
    assert(is_power_of_two(length));
    assert(length > 0);

    ${fft-type} *fft = malloc(sizeof(${fft-type}));
    if (fft == NULL)
        goto fft_allocation_failure;
    
    fft->in_out_data = malloc(sizeof(${number-type}) * 2 * length);
    if (fft->in_out_data == NULL)
        goto data_allocation_failure;

    int bit_reversal_work_area_length = 2+(1<<(int)(log(length+0.5)/log(2))/2);
    fft->bit_reversal_work_area = malloc(sizeof(int) * bit_reversal_work_area_length);
    if (fft->bit_reversal_work_area == NULL)
        goto bit_reversal_allocation_failure;
    
    fft->wave_table = malloc(sizeof(${number-type}) * (length / 2));
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

void fft_free_fft${function-tag}(${fft-type} *fft) {
    free(fft->bit_reversal_work_area);
    free(fft->in_out_data);
    free(fft->wave_table);
    free(fft);
}

void fft_fft${function-tag}(${vector-type} *data, ${fft-type} *fft) {
    assert_not_null(data);
    assert(is_power_of_two(vector_length${complex-function-tag}(data)));
    assert_not_null(fft);

    ${number-type} *in_data = fft->in_out_data;
    ${number-type} *out_data = fft->in_out_data; //+ fft->length / 2;

    for (size_t i = 0; i < vector_length${complex-function-tag}(data); i++) {
        ${complex-number-type} input_element = *vector_element${complex-function-tag}(i, data);
        in_data[i * 2] = creal${math-function-suffix}(input_element);
        in_data[i * 2 + 1] = cimag${math-function-suffix}(input_element);
    }

    cdft${function-tag}(
        fft->length * 2, 
        FORWARD_TRANSFORM, 
        fft->in_out_data, 
        fft->bit_reversal_work_area, 
        fft->wave_table
    );

    for (size_t i = 0; i < vector_length${complex-function-tag}(data); i++) {
        *vector_element${complex-function-tag}(i, data) = 
            CMPLX${math-macro-suffix}(out_data[2 * i], out_data[2 * i + 1]);
    }
}

void fft_ifft${function-tag}(${vector-type} *data, ${fft-type} *fft) {
    vector_apply${complex-function-tag}(conj${math-function-suffix}, data);
    fft_fft${function-tag}(data, fft);
    vector_apply${complex-function-tag}(conj${math-function-suffix}, data);
    vector_scale${complex-function-tag}(1.0 / vector_length${complex-function-tag}(data), data);
}

")

"
static bool is_power_of_two(int n) {
    return (n & (n - 1)) == 0;
}
"
)


