#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "oscillator.h"
#include "phasor.h"
#include "pll.h"
#include "test.h"
#include "constants.h"
#include "pid.h"

#define TEST_SIGNAL_LENGTH 10000

void test_pll();
void test_vco();
void test_quadrature_mix();

int main() {
    //test_vco();
    test_quadrature_mix();
    test_pll();
    return 0;
}

void test_vco() {
    double complex complex_frequency = 1.0 * I;

    Oscillator nco = {
        .complex_frequency = 1.0,
        .phasor = 1.0
    };

    nco = oscillator_update(complex_frequency, &nco);
    assert_complex_equal(nco.complex_frequency, complex_frequency, 5);
    assert_complex_equal(nco.phasor, complex_frequency, 5);
    nco = oscillator_update(complex_frequency, &nco);
    assert_complex_equal(nco.phasor, -1.0, 5);
    nco = oscillator_update(complex_frequency, &nco);
    assert_complex_equal(nco.phasor, -complex_frequency, 5);
}

void test_quadrature_mix() {
    FILE *iq_csv = fopen("tests/iq.csv", "w");

    const char output_column_headers[] =
        "reference,"
        "mixed_i,"
        "mixed_q,"
        "reference_phase,"
        "mixed_phase,"
        "\n";
    
    const char output_column_format[] = "%f,%f,%f,%f,%f\n";

    fprintf(
        iq_csv, 
        output_column_headers
    );

    Oscillator reference = oscillator_make(0.0, 0.02);
    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        oscillator_update(0, &reference);
        double complex mixed = oscillator_phase(reference) * oscillator_inphase(reference);
        fprintf(
            iq_csv, 
            output_column_format, 
            oscillator_inphase(reference), 
            creal(mixed), 
            cimag(mixed),
            oscillator_angular_phase(reference),
            carg(mixed)
        );

        //munit_assert_double_equal(sinusoid_evaluate(mixed), sinusoid_evaluate(reference), 4);
        //munit_assert_double(oscillator_quadrature(mixed), >=, -1);
        //munit_assert_double(oscillator_quadrature(mixed), <=, 1);

        //munit_assert_double(oscillator_inphase(mixed), >=, 0);
        //munit_assert_double(oscillator_inphase(mixed), <=, 1);

        fflush(iq_csv);
    }
}

void test_pll() {

    Pid filter = pid_make(0.001, 0.01, 0.0);

    //munit_assert_not_null(filter);
    
    Oscillator initial_vco = oscillator_make(0.0, 0.01);

    PhaseLockedLoop pll = pll_make(
        initial_vco, 
        filter
    );

    double test_signal[TEST_SIGNAL_LENGTH];
    Oscillator pll_out[TEST_SIGNAL_LENGTH];

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "test,pll-i,pll-q,pll_freq,detected_phase\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sin(i / 4.0);
        pll_out[i] = pll_evaluate(test_signal[i], &pll);
        double vco_frequency = complex_frequency_to_ordinary(pll.nco.complex_frequency);
        fprintf(
            const_freq_csv, 
            "%f,%f,%f,%f\n", 
            test_signal[i], 
            oscillator_inphase(pll_out[i]),
            oscillator_quadrature(pll_out[i]),
            vco_frequency
        );
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    filter = pid_make(0.1, 0.1, 0.0);
    pll = pll_make(initial_vco, filter);

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    Oscillator nco = oscillator_make(0, 0.0);

    fprintf(sweep_csv, "test,pll in-phase,pll frequency,test frequency\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = (((double) TEST_SIGNAL_LENGTH - i) / TEST_SIGNAL_LENGTH) / 2 / 2;
        Oscillator current_test_osc = oscillator_update(ordinary_to_complex_frequency(frequency), &nco);
        test_signal[i] = oscillator_inphase(current_test_osc);
        pll_out[i] = pll_evaluate(test_signal[i], &pll);
        fprintf(
            sweep_csv, 
            "%f,%f,%f,%f\n", 
            test_signal[i], 
            oscillator_inphase(pll_out[i]),
            complex_frequency_to_ordinary(pll.nco.complex_frequency),
            complex_frequency_to_ordinary(oscillator_angular_freq(current_test_osc))
        );
    }

    fflush(sweep_csv);

    fclose(sweep_csv);

}