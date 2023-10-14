#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#include <math.h>
#include <stdio.h>
#include "filter.h"
#include "ext/munit/munit.h"

#define TEST_SIGNAL_LENGTH 10000
#define PI 3.14159265358979323846


void test_iir();

int main(int argc, char *argv[]) {
    test_iir();
    return 0;
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


