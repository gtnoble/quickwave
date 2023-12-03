(load-from-path "substitutions.scm")

(output-text
"
#include <assert.h>
#include <complex.h>

#include \"oscillator.h\"
#include \"vector.h\"
#include \"constants.h\"
#include \"phasor.h\"

"

    (generate-text
        oscillator-schema
"
${oscillator-type} *oscillator_make${function-tag}(
    ${number-base-type} phase_degrees, 
    ${number-base-type} normalized_frequency,
    const MemoryManager *manager
) {
    ${oscillator-type} *oscillator = manager->allocate(sizeof(${oscillator-type}));
    if (oscillator == NULL) {
        return NULL;
    }

    oscillator->complex_frequency = angular_to_complex_frequency${function-tag}(
        ordinary_frequency_to_angular${function-tag}(normalized_frequency)
    );
    oscillator->phasor = rotate_phasor${function-tag}(
        zero_phase_phasor${function-tag}, 
        degrees_to_radians${function-tag}(phase_degrees)
    );

    return oscillator;
}

void oscillator_free${function-tag}(${oscillator-type} *oscillator) {
    oscillator->free(oscillator);
}

${number-base-type} oscillator_inphase${function-tag}(const ${oscillator-type} *x) {
    return creal${math-function-suffix}(x->phasor);
}

${number-base-type} oscillator_quadrature${function-tag}(const ${oscillator-type} *x) {
    return cimag${math-function-suffix}(x->phasor);
}

${number-base-type} complex oscillator_phase${function-tag}(const ${oscillator-type} *osc) {
    return osc->phasor;
}


${number-base-type} oscillator_angular_phase${function-tag}(const ${oscillator-type} *x) {
    return carg${math-function-suffix}(x->phasor);
}

${number-base-type} complex oscillator_frequency${function-tag}(const ${oscillator-type} *osc) {
    return osc->complex_frequency;
}

${number-base-type} oscillator_angular_freq${function-tag}(const ${oscillator-type} *x) {
    return carg${math-function-suffix}(x->complex_frequency);
}

void oscillator_shift_phase${function-tag}(
    ${number-base-type} angle_radians, 
    ${oscillator-type} *shifted_output, 
    const ${oscillator-type} *x
) {
    shifted_output->complex_frequency = x->complex_frequency;
    shifted_output->phasor = rotate_phasor${function-tag}(x->phasor, angle_radians);
}

void oscillator_predict${function-tag}(
    ${number-base-type} delta_samples, 
    ${oscillator-type} *prediction, 
    const ${oscillator-type} *x
) {
    oscillator_shift_phase${function-tag}(
        oscillator_angular_freq${function-tag}(x) * delta_samples, 
        prediction,
        x
    );
}

void oscillator_update${function-tag}(${number-base-type} complex update_frequency, ${oscillator-type} *nco) {
    ${number-base-type} complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabs${math-function-suffix}(update_frequency);

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

}
"))