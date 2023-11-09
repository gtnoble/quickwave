#include <complex.h>
#include <math.h>

#include "assertions.h"
#include "sinusoid_fit.h"
#include "moving_average.h"
#include "sinusoid.h"

SinusoidFit *sinusoid_fit_make(size_t window_length, double frequency) {
    SinusoidFit *model = malloc(sizeof(SinusoidFit));
    if (model == NULL) {
        return NULL;
    }

    model->fit_window = moving_average_complex_make(window_length);
    if (model->fit_window == NULL) {
        free(model); 
        return NULL;
    }

    model->reference = sinusoid_make(0.0, frequency);
    return model;
}

void sinusoid_fit_free(SinusoidFit *model) {
    moving_average_complex_free(model->fit_window);
    free(model);
}

Sinusoid sinusoid_fit_evaluate(double input, SinusoidFit *model) {
    assert_not_null(model);

    Sinusoid difference = 
        quadrature_mix(sinusoid_negate_phase(model->reference), input);
    double complex smoothed_phase = 
        moving_average_complex_evaluate(difference.phasor, model->fit_window);
    Sinusoid positive_frequency = quadrature_mix(model->reference, smoothed_phase);
    Sinusoid negative_frequency = sinusoid_negate_phase(positive_frequency);
    model->reference = nco_update(0.0, model->reference);
    return sinusoid_add(positive_frequency, negative_frequency);
}