#include "sinusoid_fit.h"
#include "test.h"
#include "stdio.h"

const size_t n_test_points = 1000;
const size_t window_length = 75;
const double signal_frequency = 1.0 / 10.0;
const double noise_frequency = 1.0 / 7.0;

const char output_filename[] = "./tests/sinusoid_fit.csv";
const char output_file_headers[] = "signal, noise, sum, fit\n";
const char output_file_format[] = "%f,%f,%f,%f\n";

void test_sinusoid_fit();

int main() {
    test_sinusoid_fit();
}

void test_sinusoid_fit() {
    Sinusoid signal = sinusoid_make(0.0, signal_frequency);
    Sinusoid noise = sinusoid_make(0.0, noise_frequency);

    SinusoidFit *sinusoid_fit_model = 
        sinusoid_fit_make(window_length, signal_frequency);
    assert_not_null(sinusoid_fit_model);

    FILE *csv_output = fopen(output_filename, "w");
    assert_not_null(csv_output);

    fprintf(csv_output, output_file_headers);

    for (size_t i = 0; i < n_test_points; i++) {
        double signal_part = 
            sinusoid_inphase(signal = nco_update(0.0, signal));
        double noise_part =
            sinusoid_inphase(noise = nco_update(0.0, noise));
        double sum =
            signal_part + noise_part;
        
        Sinusoid fit = sinusoid_fit_evaluate(sum, sinusoid_fit_model);
        double evaluated_fit = 
            sinusoid_inphase(fit);

        fprintf(csv_output, output_file_format, signal_part, noise_part, sum, evaluated_fit);
    }
}