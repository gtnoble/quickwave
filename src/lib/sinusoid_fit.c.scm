(load-from-path "substitutions.scm")

(output-text
"
#include <complex.h>
#include <math.h>

#include \"assertions.h\"
#include \"sinusoid_fit.h\"
#include \"moving_average.h\"
#include \"oscillator.h\"

"

    (generate-text
        sinusoid-fit-schema
"
${sinusoid-fit-type} *sinusoid_fit_make${function-tag}(size_t window_length, ${number-type} frequency) {
    ${sinusoid-fit-type} *model = malloc(sizeof(${sinusoid-fit-type}));
    if (model == NULL) {
        return NULL;
    }

    model->fit_window = moving_average_complex_make${function-tag}(window_length);
    if (model->fit_window == NULL) {
        free(model); 
        return NULL;
    }

    model->reference = oscillator_make${function-tag}(0.0, frequency);
    return model;
}

void sinusoid_fit_free${function-tag}(${sinusoid-fit-type} *model) {
    moving_average_free_complex${function-tag}(model->fit_window);
    free(model);
}

${oscillator-type} sinusoid_fit_evaluate${function-tag}(${number-type} input, ${sinusoid-fit-type} *model) {
    assert_not_null(model);

    ${oscillator-type} reference = oscillator_update${function-tag}(0.0, &model->reference);

    /**
     * @brief 
     * Compare the instantaneous amplitude and phase of the current input sample sinusoid
     * to the reference sinusoid.
     * Return a sinusoid that has an amplitude equal to the input sample, 
     * but with a phase that is relative to the phase of the input signal sinusoid.
     */
    ${number-type} complex phase_and_amplitude_relative_to_reference = 
        conj${math-function-suffix}(oscillator_phase${function-tag}(reference)) * input;

    /**
     * @brief 
     * Take the average of the relative sinusoids. This attenuates all sinusoids,
     * except those with the same frequency as the reference.
     */
    ${number-type} complex average_relative_phase_and_amplitude = 
        moving_average_evaluate_complex${function-tag}(
            phase_and_amplitude_relative_to_reference, 
            model->fit_window
        );

    /**
     * @brief 
     * Recombining the average phase and amplitude differences with the reference
     * gives the positive frequency term of the DFT
     */
    ${number-type} complex absolute_positive_frequency_component = 
        oscillator_phase${function-tag}(reference) * average_relative_phase_and_amplitude;

    /**
     * @brief 
     * The negative frequency term of the DFT is the conjugate of the positive frequency term (For real-valued signals)
     */
    ${number-type} complex absolute_negative_frequency_component = 
        conj${math-function-suffix}(absolute_positive_frequency_component);


    /**
     * @brief 
     * We have to add the positive and negative frequencies together
     * to perform the inverse DFT
     */
    return (${oscillator-type}) {
        .phasor = absolute_positive_frequency_component + 
            absolute_negative_frequency_component,
        .complex_frequency = oscillator_frequency${function-tag}(reference)
    };
}
"))