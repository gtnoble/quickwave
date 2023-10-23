#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "sinusoid.h"
#include "pll.h"
#include "test.h"
#include "constants.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846

void test_pll();
void test_vco();
double complex pll_filter(double complex input, void *filter);
double complex detected_phase;

int main(int , char **) {
    //test_vco();
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

    double test_signal[TEST_SIGNAL_LENGTH];
    Sinusoid pll_out[TEST_SIGNAL_LENGTH];

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "index,test,pll,pll_freq,detected_phase\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sin(i / 4.0);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(
            const_freq_csv, 
            "%d,%f,%f,%f,%f\n", 
            i, 
            test_signal[i], 
            -sinusoid_quadrature(pll_out[i]),
            complex_frequency_to_ordinary(pll->vco.complex_frequency),
            carg(detected_phase)
        );
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    pll_reset(initial_vco, pll);
    filter_reset_digital_filter(filter);

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    Sinusoid vco = sinusoid_make(0, 0.5);

    fprintf(sweep_csv, "index,test,pll,pll_freq,detected_phase,test_frequency\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = (((double) TEST_SIGNAL_LENGTH - i) / TEST_SIGNAL_LENGTH) / 2 / 2;
        vco = update_vco(angular_to_complex_frequency(ordinary_frequency_to_angular(frequency)), vco);
        test_signal[i] = sinusoid_inphase(vco);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(
            sweep_csv, 
            "%d,%f,%f,%f,%f,%f\n", 
            i, 
            test_signal[i], 
            -sinusoid_quadrature(pll_out[i]),
            complex_frequency_to_ordinary(pll->vco.complex_frequency),
            carg(detected_phase),
            complex_frequency_to_ordinary(vco.complex_frequency)
        );
    }

    fflush(sweep_csv);

    fclose(sweep_csv);

}

double complex pll_filter(double complex input, void *filter) {
    detected_phase = input;
    return filter_evaluate(input, filter) * 1 + input * 200;
}
