#include <stdio.h>
#include "linear_model.h"
#include "assertions.h"

const size_t estimator_length = 11;
const size_t signal_length = 100;

const double signal_slope = 1.0;
const double signal_y_intercept = 1.0;

const char test_linear_fit_csv_filename[] = "tests/linear_fit.csv";
const char test_linear_fit_csv_header[] = "actual value, fit intercept, fit slope\n";
const char test_linear_fit_csv_format[] = "%f,%f,%f\n";

void test_linear_fit();

int main() {
    test_linear_fit();
}

void test_linear_fit() {
    LinearEstimator *estimator = linear_estimator_new(estimator_length);

    FILE *linear_model_csv = fopen(test_linear_fit_csv_filename, "w");
    assert_not_null(linear_model_csv);

    fprintf(linear_model_csv, test_linear_fit_csv_header);


    for (size_t i = 0; i < signal_length; i++) {
        double value = signal_slope * i + signal_y_intercept;
        LinearModel fitted = linear_estimator_estimate(value, estimator);

        fprintf(linear_model_csv, test_linear_fit_csv_format, value, fitted.intercept, fitted.slope);
    }

    fclose(linear_model_csv);
}