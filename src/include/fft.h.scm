(load-from-path "substitutions.scm")

(output-text

"
#ifndef QUICKWAVE_FFT
#define QUICKWAVE_FFT

#include \"vector.h\"
#include \"memory.h\"

"

    (generate-text
        fft-schema
"
typedef struct {
    ${number-type} *in_out_data;
    ${number-type} *wave_table;
    int length;
    int *bit_reversal_work_area;
    Deallocator *free;
} ${fft-type};

${fft-type} *fft_make_fft${function-tag}(int length, const MemoryManager *manager);
void fft_free_fft${function-tag}(${fft-type} *fft);
void fft_fft${function-tag}(${vector-type} *data, ${fft-type} *fft);
void fft_ifft${function-tag}(${vector-type} *data, ${fft-type} *fft);

")

"
#endif
")