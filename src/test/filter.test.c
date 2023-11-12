#include <math.h>
#include <stdio.h>
#include <complex.h>
#include "filter.h"
#include "test.h"
#include "window.h"
#include "constants.h"

#define TEST_SIGNAL_LENGTH 10000

void test_iir();
void test_sinc();
int test_filter(
    char *output_filename, 
    double input[], 
    double output[], 
    DigitalFilterReal *filter
);

int main() {
    test_iir();
    test_sinc();
    return 0;
}


void test_iir() {
    DigitalFilterComplex *iir = filter_make_first_order_iir(0.01);
    munit_assert_not_null(iir);

    FILE *iir_response_csv = fopen("tests/iir_response.csv", "w");
    munit_assert_not_null(iir_response_csv);

    fprintf(iir_response_csv, "value\n");

    double complex filtered[TEST_SIGNAL_LENGTH];
    for (int i = 1; i < TEST_SIGNAL_LENGTH; i++) {
        filtered[i] = filter_evaluate_digital_filter_complex(1.0, iir);
        munit_assert_double(creal(filtered[i]), >=, creal(filtered[i-1]));
        fprintf(iir_response_csv, "%f\n", creal(filtered[i]));
    }
    fflush(iir_response_csv);
    assert_complex_equal(filtered[TEST_SIGNAL_LENGTH - 1], 1.0, 2);
    fclose(iir_response_csv);

    filter_free_digital_filter_complex(iir);
}

void test_sinc() {
    DigitalFilterReal *sinc_filter = filter_make_sinc(0.25, 101, LOW_PASS, window_hamming);
    double filtered[TEST_SIGNAL_LENGTH];
    double input[TEST_SIGNAL_LENGTH];

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        input[i] = sin(2 * M_PI * 0.2 * i) + cos(2 * M_PI * 0.3 * i);
    }

    test_filter("tests/test_sinc.csv", input, filtered, sinc_filter);

    filter_free_digital_filter_real(sinc_filter);
}

int test_filter(
    char *output_filename, 
    double input[], 
    double output[], 
    DigitalFilterReal *filter
) {
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
        return 0;

    fprintf(output_file, "input,output\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        output[i] = filter_evaluate_digital_filter_real(input[i], filter);
        fprintf(output_file, "%f,%f\n", input[i], output[i]);
    }

    fclose(output_file);
    return 1;
}


