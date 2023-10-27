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
void test_quadrature_mix();
double complex pll_filter(double complex input, void *filter);
double complex detected_phase;

int main(int , char **) {
    //test_vco();
    test_quadrature_mix();
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

void test_quadrature_mix() {
    FILE *iq_csv = fopen("tests/iq.csv", "w");

    char output_column_headers[] =
        "reference_i,"
        "reference_q,"
        "mixed_i,"
        "mixed_q,"
        "reference_phase,"
        "mixed_phase,"
        "\n";
    
    char output_column_format[] = "%f,%f,%f,%f,%f,%f,%f\n";

    fprintf(
        iq_csv, 
        output_column_headers
    );

    Sinusoid reference = sinusoid_make(0.0, 0.1);
    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        reference = update_vco(0, reference);
        Sinusoid mixed = quadrature_mix(reference, sinusoid_evaluate(reference));
        fprintf(
            iq_csv, 
            output_column_format, 
            sinusoid_inphase(reference), 
            sinusoid_quadrature(reference), 
            sinusoid_inphase(mixed), 
            sinusoid_quadrature(mixed),
            sinusoid_phase(reference),
            sinusoid_phase(mixed)
        );

        munit_assert_double(sinusoid_inphase(mixed), >=, 0);
        munit_assert_double(sinusoid_quadrature(mixed), >=, 0);

        fflush(iq_csv);
    }
}

void test_pll() {

    Pid filter = filter_make_pid(0.1, 0.1, 0.0);

    //munit_assert_not_null(filter);
    
    Sinusoid initial_vco = sinusoid_make(0.0, 0.01);

    PhaseLockedLoop *pll = pll_make(
        initial_vco, 
        pll_filter,
        &filter
    );

    munit_assert_not_null(pll);

    double test_signal[TEST_SIGNAL_LENGTH];
    Sinusoid pll_out[TEST_SIGNAL_LENGTH];

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "index,test,pll,pll-i,pll-q,pll_freq,detected_phase\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sin(i / 4.0);
        pll_out[i] = pll_update(test_signal[i], pll);
        double vco_frequency = complex_frequency_to_ordinary(pll->vco.complex_frequency);
        fprintf(
            const_freq_csv, 
            "%d,%f,%f,%f,%f,%f,%f\n", 
            i, 
            test_signal[i], 
            sinusoid_evaluate(pll_out[i]),
            sinusoid_inphase(pll_out[i]),
            sinusoid_quadrature(pll_out[i]),
            vco_frequency,
            creal(detected_phase) + cimag(detected_phase)
        );
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    pll_reset(initial_vco, pll);
    filter_reset_pid(&filter);

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    Sinusoid vco = sinusoid_make(0, 0.0);

    fprintf(sweep_csv, "index,test,pll,pll_freq,detected_phase,test_frequency\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = (((double) TEST_SIGNAL_LENGTH - i) / TEST_SIGNAL_LENGTH) / 2 / 2;
        vco = update_vco(angular_to_complex_frequency(ordinary_frequency_to_angular(frequency)), vco);
        test_signal[i] = sinusoid_inphase(vco);
        pll_out[i] = pll_update(test_signal[i], pll);
        double vco_frequency = complex_frequency_to_ordinary(pll->vco.complex_frequency);
        fprintf(
            sweep_csv, 
            "%d,%f,%f,%f,%f,%f\n", 
            i, 
            test_signal[i], 
            sinusoid_inphase(pll_out[i]),
            vco_frequency,
            creal(detected_phase) + cimag(detected_phase),
            complex_frequency_to_ordinary(vco.complex_frequency)
        );
    }

    fflush(sweep_csv);

    fclose(sweep_csv);

}

double complex pll_filter(double complex input, void *context) {
    detected_phase = input;
    double complex filtered = 
        angular_to_complex_frequency(
            creal(
                filter_update_pid(carg(input), context))); 
    return filtered; 
}
