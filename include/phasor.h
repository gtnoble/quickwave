#ifndef QUICKWAVE_PHASOR
#define QUICKWAVE_PHASOR

#include <complex.h>
#include "constants.h"

/**
 * @brief 
 * Phasor corresponding to zero ordinary or angular frequency
 */
static const double complex zero_complex_frequency = 1.0;

/**
 * @brief 
 * Phasor corresponding to zero angular phase
 */
static const double complex zero_phase_phasor = 1.0;

/**
 * @brief 
 * Rotates a phasor
 * @param phasor Phasor to be rotated
 * @param angle_radians Angle to rotate phasor by, in radians
 * @return Rotated phasor
 */
static inline double complex rotate_phasor(double complex phasor, double angle_radians) {
    return phasor * cexp(I * angle_radians);
}

/**
 * @brief 
 * Converts an angular frequency to a complex frequency
 * @param angular_frequency Angular frequency, in radians per sample
 * @return Complex frequency
 */
static inline double complex angular_to_complex_frequency(double angular_frequency) {
    return zero_complex_frequency * cexp(I * angular_frequency);
}

static inline double complex ordinary_to_complex_frequency(double ordinary_frequency){
    return angular_to_complex_frequency(
        ordinary_frequency_to_angular(ordinary_frequency)
    );
}

/**
 * @brief 
 * Converts a complex frequency to an angular frequency
 * @param complex_frequency Complex frequency
 * @return Angular frequency, in radians per sample 
 */
static inline double complex_frequency_to_angular(double complex complex_frequency) {
    return carg(complex_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to a normalized ordinary frequency
 * @param complex_frequency Complex frequency
 * @return Normalized ordinary frequency 
 */
static inline double complex_frequency_to_ordinary(double complex complex_frequency) {
    return angular_frequency_to_ordinary(
        complex_frequency_to_angular(complex_frequency)
    );
}

#endif