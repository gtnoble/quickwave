#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "sinusoid.h"
#include "pll.h"
#include "test.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846

void test_pll();
void test_vco();
double complex pll_filter(double complex input, void *filter);

int main(int , char **) {
    test_vco();
    test_pll();
    return 0;
}

void test_vco() {
    double complex complex_frequency = 1.0 * I;

    Sinusoid vco = {
        .complex_frequency = 1.0,
        .phasor = 1.0
    };

    vco = update_vco(complex_frequency, vco);
    assert_complex_equal(vco.complex_frequency, complex_frequency, 5);
    assert_complex_equal(vco.phasor, complex_frequency, 5);
    vco = update_vco(complex_frequency, vco);
    assert_complex_equal(vco.phasor, -1.0, 5);
    vco = update_vco(complex_frequency, vco);
    assert_complex_equal(vco.phasor, -complex_frequency, 5);
}

void test_pll() {

    DigitalFilter *filter = filter_make_integrator();
    
    Sinusoid initial_vco = sinusoid_make(0, 0.1);

    PhaseLockedLoop *pll = pll_make(
        initial_vco, 
        1.0 / TEST_SIGNAL_LENGTH, 
        pll_filter,
        filter
    );

    munit_assert_not_null(pll);

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "index,test,pll,pll_freq\n");

    double test_signal[TEST_SIGNAL_LENGTH];
    Sinusoid pll_out[TEST_SIGNAL_LENGTH];
    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sin(i / 3.0);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(
            const_freq_csv, 
            "%d,%f,%f,%f\n", 
            i, 
            test_signal[i], 
            -sinusoid_quadrature(pll_out[i]),
            complex_frequency_to_ordinary(pll->vco.complex_frequency)
        );
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    pll_reset(initial_vco, pll);
    filter_reset_digital_filter(filter);

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    fprintf(sweep_csv, "index,test,pll\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = ((double) i) / TEST_SIGNAL_LENGTH / 2;
        test_signal[i] = sin(i * frequency);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(
            sweep_csv, 
            "%d,%f,%f\n", 
            i, 
            test_signal[i], 
            -sinusoid_quadrature(pll_out[i]) 
        );
    }

    fflush(sweep_csv);

    munit_assert_double_equal(
        test_signal[TEST_SIGNAL_LENGTH - 1], 
        sinusoid_inphase(pll_out[TEST_SIGNAL_LENGTH - 1]),
        4
    );
    fclose(sweep_csv);

}

double complex pll_filter(double complex input, void *filter) {
    return filter_evaluate(input, filter) * 0.0005 + input * 0.01;
}
