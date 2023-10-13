#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#include <math.h>
#include <stdio.h>
#include "filter.h"
#include "ext/munit/munit.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846

static int setup_pll(PhaseLockedLoop **pll);
void test_pll();
void test_iir();
void test_vco();

int main(int argc, char *argv[]) {
    test_iir();
    test_vco();
    test_pll();
    return 0;
}

static int setup_pll(PhaseLockedLoop **pll) {
    *pll = sqlite3_malloc(sizeof(PhaseLockedLoop *));
    return pll_make(pll, 0.001, 0.5);
}

void test_iir() {
    DigitalFilter *iir;
    int status = filter_make_first_order_iir(&iir, 0.01);
    munit_assert_int(status, ==, FILTER_OK);

    FILE *iir_response_csv = fopen("tests/iir_response.csv", "w");
    munit_assert_not_null(iir_response_csv);

    fprintf(iir_response_csv, "index,value\n");

    double filtered[TEST_SIGNAL_LENGTH];
    for (int i = 1; i < TEST_SIGNAL_LENGTH; i++) {
        filtered[i] = filter_evaluate(1.0, iir);
        munit_assert_double(filtered[i], >=, filtered[i-1]);
        fprintf(iir_response_csv, "%d,%f\n", i, filtered[i]);
    }
    munit_assert_double_equal(filtered[TEST_SIGNAL_LENGTH - 1], 1.0, 2);
    fclose(iir_response_csv);
}

void test_vco() {
    double phase_accumulator = 0.0;
    update_vco(PI, &phase_accumulator);
    munit_assert_double_equal(phase_accumulator, PI, 5);
    update_vco(2 * PI, &phase_accumulator);
    munit_assert_double_equal(phase_accumulator, PI, 5);
    update_vco(-2 * PI, &phase_accumulator);
    munit_assert_double_equal(phase_accumulator, PI, 5);
}

void test_pll() {

    PhaseLockedLoop *pll;
    munit_assert_int(setup_pll(&pll), ==, FILTER_OK);

    munit_assert_not_null(pll);

    FILE *const_freq_csv = fopen("tests/const_freq.csv", "w");
    munit_assert_not_null(const_freq_csv);

    fprintf(const_freq_csv, "index,test,pll,pll_freq\n");

    double test_signal[TEST_SIGNAL_LENGTH];
    double pll_out[TEST_SIGNAL_LENGTH];
    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        test_signal[i] = sign(sin(i / 3.0));
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(const_freq_csv, "%d,%f,%f,%f\n", i, test_signal[i], pll_out[i], pll->loop_filter->current_output);
    }

    fflush(const_freq_csv);

    fclose(const_freq_csv);

    pll_free(pll);
    setup_pll(&pll);

    FILE *sweep_csv  = fopen("tests/sweep.csv", "w");
    munit_assert_not_null(sweep_csv);

    fprintf(sweep_csv, "index,test,pll\n");

    for (int i = 0; i < TEST_SIGNAL_LENGTH; i++) {
        double frequency = ((double) i) / TEST_SIGNAL_LENGTH / 2;
        test_signal[i] = sin(i * frequency);
        pll_out[i] = pll_update(test_signal[i], pll);
        fprintf(sweep_csv, "%d,%f,%f,%f\n", i, test_signal[i], pll_out[i], pll->loop_filter->current_output);
    }

    fflush(sweep_csv);

    munit_assert_double_equal(
        test_signal[TEST_SIGNAL_LENGTH - 1], 
        pll_out[TEST_SIGNAL_LENGTH - 1],
        4
    );
    fclose(sweep_csv);

}


