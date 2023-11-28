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
${oscillator-type} oscillator_make${function-tag}(${number-base-type} phase_degrees, ${number-base-type} normalized_frequency) {
    ${oscillator-type} result = {
        .complex_frequency = angular_to_complex_frequency${function-tag}(
            ordinary_frequency_to_angular${function-tag}(normalized_frequency)
        ),
        .phasor = rotate_phasor${function-tag}(zero_phase_phasor, degrees_to_radians${function-tag}(phase_degrees))
    };
    return result;
}

${number-base-type} oscillator_inphase${function-tag}(${oscillator-type} x) {
    return creal${math-function-suffix}(x.phasor);
}

${number-base-type} oscillator_quadrature${function-tag}(${oscillator-type} x) {
    return cimag${math-function-suffix}(x.phasor);
}

${number-base-type} complex oscillator_phase${function-tag}(${oscillator-type} osc) {
    return osc.phasor;
}


${number-base-type} oscillator_angular_phase${function-tag}(${oscillator-type} x) {
    return carg${math-function-suffix}(x.phasor);
}

${number-base-type} complex oscillator_frequency${function-tag}(${oscillator-type} osc) {
    return osc.complex_frequency;
}

${number-base-type} oscillator_angular_freq${function-tag}(${oscillator-type} x) {
    return carg${math-function-suffix}(x.complex_frequency);
}

${oscillator-type} nco_shift_phase${function-tag}(${number-base-type} angle_radians, ${oscillator-type} x) {
    ${oscillator-type} result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor${function-tag}(x.phasor, angle_radians)
    };
    return result;
}

${oscillator-type} oscillator_predict${function-tag}(${number-base-type} delta_samples, ${oscillator-type} x) {
    return nco_shift_phase${function-tag}(oscillator_angular_freq${function-tag}(x) * delta_samples, x);
}

${oscillator-type} oscillator_update${function-tag}(${number-base-type} complex update_frequency, ${oscillator-type} *nco) {
    ${number-base-type} complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabs${math-function-suffix}(update_frequency);

    ${oscillator-type} current_nco = *nco;

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

    return current_nco;
}
"))