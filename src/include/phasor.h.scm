(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_PHASOR
#define QUICKWAVE_PHASOR

#include <complex.h>
#include \"constants.h\"

"

    (generate-text
        number-schema
"
/**
 * @brief 
 * Phasor corresponding to zero ordinary or angular frequency
 */
static const ${number-type} complex zero_complex_frequency${function-tag} = 1.0;

/**
 * @brief 
 * Phasor corresponding to zero angular phase
 */
static const ${number-type} complex zero_phase_phasor${function-tag} = 1.0;

/**
 * @brief 
 * Rotates a phasor
 * @param phasor Phasor to be rotated
 * @param angle_radians Angle to rotate phasor by, in radians
 * @return Rotated phasor
 */
static inline ${number-type} complex rotate_phasor${function-tag}(${number-type} complex phasor, ${number-type} angle_radians) {
    return phasor * cexp${math-function-suffix}(I * angle_radians);
}

/**
 * @brief 
 * Converts an angular frequency to a complex frequency
 * @param angular_frequency Angular frequency, in radians per sample
 * @return Complex frequency
 */
static inline ${number-type} complex angular_to_complex_frequency${function-tag}(${number-type} angular_frequency) {
    return zero_complex_frequency${function-tag} * cexp${math-function-suffix}(I * angular_frequency);
}

static inline ${number-type} complex ordinary_to_complex_frequency${function-tag}(${number-type} ordinary_frequency){
    return angular_to_complex_frequency${function-tag}(
        ordinary_frequency_to_angular${function-tag}(ordinary_frequency)
    );
}

/**
 * @brief 
 * Converts a complex frequency to an angular frequency
 * @param complex_frequency Complex frequency
 * @return Angular frequency, in radians per sample 
 */
static inline ${number-type} complex_frequency_to_angular${function-tag}(${number-type} complex complex_frequency) {
    return carg${math-function-suffix}(complex_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to a normalized ordinary frequency
 * @param complex_frequency Complex frequency
 * @return Normalized ordinary frequency 
 */
static inline ${number-type} complex_frequency_to_ordinary${function-tag}(${number-type} complex complex_frequency) {
    return angular_frequency_to_ordinary${function-tag}(
        complex_frequency_to_angular${function-tag}(complex_frequency)
    );
}

")

"
#endif
")