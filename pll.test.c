#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "sinusoid.h"
#include "pll.h"
#include "test.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846

PhaseLockedLoop *setup_pll();
void test_pll();
void test_vco();

int main(int , char **) {
    test_vco();
    test_pll();
    return 0;
}

PhaseLockedLoop *setup_pll() {
    return pll_make(0.001, -1, 1.0 / TEST_SIGNAL_LENGTH);
}


void test_vco() {
    double complex complex_frequency = 1.0 * I;

    Sinusoid vco = {
        .complex_frequency = 1.0,
        .phasor = 1.0
    };

    vco = update_vco(complex_frequency, vco);
    munit_assert_complex_equal(vco.complex_frequency, complex_frequency, 5);
    munit_assert_complex_equal(vco.phasor, complex_frequency, 5);
    vco = update_vco(complex_frequency, vco);
    munit_assert_complex_equal(vco.phasor, -1.0, 5);
    vco = update_vco(complex_frequency, vco);
    munit_assert_complex_equal(vco.phasor, -complex_frequency, 5);
}

void test_pll() {

    PhaseLockedLoop *pll = setup_pll();
    munit_assert_not_null(pll);

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "index,test,pll,pll_freq\n");

    double test_signal[TEST_SIGNAL_LENGTH];
    Sinusoid pll_out[TEST_SIGNAL_LENGTH];
    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sin(i / 3.0);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(const_freq_csv, "%d,%f,%f,%f\n", i, test_signal[i], sinusoid_inphase(pll_out[i]), creal(pll->loop_filter->current_output));
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    pll_free(pll);
    pll = setup_pll();

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    fprintf(sweep_csv, "index,test,pll\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = ((double) i) / TEST_SIGNAL_LENGTH / 2;
        test_signal[i] = sin(i * frequency);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(sweep_csv, "%d,%f,%f,%f\n", i, test_signal[i], sinusoid_inphase(pll_out[i]), creal(pll->loop_filter->current_output));
    }

    fflush(sweep_csv);

    munit_assert_double_equal(
        test_signal[TEST_SIGNAL_LENGTH - 1], 
        sinusoid_inphase(pll_out[TEST_SIGNAL_LENGTH - 1]),
        4
    );
    fclose(sweep_csv);

}
