
#include <complex.h>
#include <math.h>

#include "assertions.h"
#include "sinusoid_fit.h"
#include "moving_average.h"
#include "oscillator.h"


SinusoidFitDouble *sinusoid_fit_make_double(size_t window_length, double frequency) {
    SinusoidFitDouble *model = malloc(sizeof(SinusoidFitDouble));
    if (model == NULL) {
        return NULL;
    }

    model->fit_window = moving_average_make_complex_double(window_length);
    if (model->fit_window == NULL) {
        free(model); 
        return NULL;
    }

    model->reference = oscillator_make_double(0.0, frequency);
    return model;
}

void sinusoid_fit_free_double(SinusoidFitDouble *model) {
    moving_average_free_complex_double(model->fit_window);
    free(model);
}

OscillatorDouble sinusoid_fit_evaluate_double(double input, SinusoidFitDouble *model) {
    assert_not_null(model);

    OscillatorDouble reference = oscillator_update_double(0.0, &model->reference);

    /**
     * @brief 
     * Compare the instantaneous amplitude and phase of the current input sample sinusoid
     * to the reference sinusoid.
     * Return a sinusoid that has an amplitude equal to the input sample, 
     * but with a phase that is relative to the phase of the input signal sinusoid.
     */
    double complex phase_and_amplitude_relative_to_reference = 
        conj(oscillator_phase_double(reference)) * input;

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
        oscillator_phase_double(reference) * average_relative_phase_and_amplitude;

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
    return (OscillatorDouble) {
        .phasor = absolute_positive_frequency_component + 
            absolute_negative_frequency_component,
        .complex_frequency = oscillator_frequency_double(reference)
    };
}



SinusoidFitFloat *sinusoid_fit_make_float(size_t window_length, float frequency) {
    SinusoidFitFloat *model = malloc(sizeof(SinusoidFitFloat));
    if (model == NULL) {
        return NULL;
    }

    model->fit_window = moving_average_make_complex_float(window_length);
    if (model->fit_window == NULL) {
        free(model); 
        return NULL;
    }

    model->reference = oscillator_make_float(0.0, frequency);
    return model;
}

void sinusoid_fit_free_float(SinusoidFitFloat *model) {
    moving_average_free_complex_float(model->fit_window);
    free(model);
}

OscillatorFloat sinusoid_fit_evaluate_float(float input, SinusoidFitFloat *model) {
    assert_not_null(model);

    OscillatorFloat reference = oscillator_update_float(0.0, &model->reference);

    /**
     * @brief 
     * Compare the instantaneous amplitude and phase of the current input sample sinusoid
     * to the reference sinusoid.
     * Return a sinusoid that has an amplitude equal to the input sample, 
     * but with a phase that is relative to the phase of the input signal sinusoid.
     */
    float complex phase_and_amplitude_relative_to_reference = 
        conjf(oscillator_phase_float(reference)) * input;

    /**
     * @brief 
     * Take the average of the relative sinusoids. This attenuates all sinusoids,
     * except those with the same frequency as the reference.
     */
    float complex average_relative_phase_and_amplitude = 
        moving_average_evaluate_complex_float(
            phase_and_amplitude_relative_to_reference, 
            model->fit_window
        );

    /**
     * @brief 
     * Recombining the average phase and amplitude differences with the reference
     * gives the positive frequency term of the DFT
     */
    float complex absolute_positive_frequency_component = 
        oscillator_phase_float(reference) * average_relative_phase_and_amplitude;

    /**
     * @brief 
     * The negative frequency term of the DFT is the conjugate of the positive frequency term (For real-valued signals)
     */
    float complex absolute_negative_frequency_component = 
        conjf(absolute_positive_frequency_component);


    /**
     * @brief 
     * We have to add the positive and negative frequencies together
     * to perform the inverse DFT
     */
    return (OscillatorFloat) {
        .phasor = absolute_positive_frequency_component + 
            absolute_negative_frequency_component,
        .complex_frequency = oscillator_frequency_float(reference)
    };
}
