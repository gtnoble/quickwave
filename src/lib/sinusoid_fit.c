#include <complex.h>
#include <math.h>

#include "assertions.h"
#include "sinusoid_fit.h"
#include "moving_average.h"
#include "oscillator.h"

SinusoidFit *sinusoid_fit_make(size_t window_length, double frequency) {
    SinusoidFit *model = malloc(sizeof(SinusoidFit));
    if (model == NULL) {
        return NULL;
    }

    model->fit_window = moving_average_make_complex_double(window_length);
    if (model->fit_window == NULL) {
        free(model); 
        return NULL;
    }

    model->reference = oscillator_make(0.0, frequency);
    return model;
}

void sinusoid_fit_free(SinusoidFit *model) {
    moving_average_free_complex_double(model->fit_window);
    free(model);
}

Oscillator sinusoid_fit_evaluate(double input, SinusoidFit *model) {
    assert_not_null(model);

    Oscillator reference = oscillator_update(0.0, &model->reference);

    /**
     * @brief 
     * Compare the instantaneous amplitude and phase of the current input sample sinusoid
     * to the reference sinusoid.
     * Return a sinusoid that has an amplitude equal to the input sample, 
     * but with a phase that is relative to the phase of the input signal sinusoid.
     */
    double complex phase_and_amplitude_relative_to_reference = 
        conj(oscillator_phase(reference)) * input;

    /**
     * @brief 
     * Take the average of the relative sinusoids. This attenuates all sinusoids,
     * except those with the same frequency as the reference.
     */
    double complex average_relative_phase_and_amplitude = 
        moving_average_evaluate_complex_double(
            phase_and_amplitude_relative_to_reference, 
            model->fit_window
        );

    /**
     * @brief 
     * Recombining the average phase and amplitude differences with the reference
     * gives the positive frequency term of the DFT
     */
    double complex absolute_positive_frequency_component = 
        oscillator_phase(reference) * average_relative_phase_and_amplitude;

    /**
     * @brief 
     * The negative frequency term of the DFT is the conjugate of the positive frequency term (For real-valued signals)
     */
    double complex absolute_negative_frequency_component = 
        conj(absolute_positive_frequency_component);


    /**
     * @brief 
     * We have to add the positive and negative frequencies together
     * to perform the inverse DFT
     */
    return (Oscillator) {
        .phasor = absolute_positive_frequency_component + 
            absolute_negative_frequency_component,
        .complex_frequency = oscillator_frequency(reference)
    };
}