#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#include <stdio.h>
#include <math.h>
#include "pll.h"
#include "ext/munit/munit.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846

static int setup_pll(PhaseLockedLoop **pll);
void test_pll();
void test_vco();

int main(int argc, char *argv[]) {
    test_vco();
    test_pll();
    return 0;
}

static int setup_pll(PhaseLockedLoop **pll) {
    *pll = sqlite3_malloc(sizeof(PhaseLockedLoop *));
    return pll_make(pll, 0.0002, -1, 1.0 / TEST_SIGNAL_LENGTH);
}


void test_vco() {
    double phase_accumulator = 0.0;
    update_vco(PI, &phase_accumulator, true);
    munit_assert_double_equal(phase_accumulator, PI, 5);
    update_vco(2 * PI, &phase_accumulator, true);
    munit_assert_double_equal(phase_accumulator, PI, 5);
    update_vco(-2 * PI, &phase_accumulator, true);
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
        test_signal[i] = sin(i / 3.0);
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
