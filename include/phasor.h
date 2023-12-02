
#ifndef QUICKWAVE_PHASOR
#define QUICKWAVE_PHASOR

#include <complex.h>
#include "constants.h"


/**
 * @brief 
 * Phasor corresponding to zero ordinary or angular frequency
 */
static const double complex zero_complex_frequency_double = 1.0;

/**
 * @brief 
 * Phasor corresponding to zero angular phase
 */
static const double complex zero_phase_phasor_double = 1.0;

/**
 * @brief 
 * Rotates a phasor
 * @param phasor Phasor to be rotated
 * @param angle_radians Angle to rotate phasor by, in radians
 * @return Rotated phasor
 */
static inline double complex rotate_phasor_double(double complex phasor, double angle_radians) {
    return phasor * cexp(I * angle_radians);
}

/**
 * @brief 
 * Converts an angular frequency to a complex frequency
 * @param angular_frequency Angular frequency, in radians per sample
 * @return Complex frequency
 */
static inline double complex angular_to_complex_frequency_double(double angular_frequency) {
    return zero_complex_frequency_double * cexp(I * angular_frequency);
}

static inline double complex ordinary_to_complex_frequency_double(double ordinary_frequency){
    return angular_to_complex_frequency_double(
        ordinary_frequency_to_angular_double(ordinary_frequency)
    );
}

/**
 * @brief 
 * Converts a complex frequency to an angular frequency
 * @param complex_frequency Complex frequency
 * @return Angular frequency, in radians per sample 
 */
static inline double complex_frequency_to_angular_double(double complex complex_frequency) {
    return carg(complex_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to a normalized ordinary frequency
 * @param complex_frequency Complex frequency
 * @return Normalized ordinary frequency 
 */
static inline double complex_frequency_to_ordinary_double(double complex complex_frequency) {
    return angular_frequency_to_ordinary_double(
        complex_frequency_to_angular_double(complex_frequency)
    );
}




/**
 * @brief 
 * Phasor corresponding to zero ordinary or angular frequency
 */
static const float complex zero_complex_frequency_float = 1.0;

/**
 * @brief 
 * Phasor corresponding to zero angular phase
 */
static const float complex zero_phase_phasor_float = 1.0;

/**
 * @brief 
 * Rotates a phasor
 * @param phasor Phasor to be rotated
 * @param angle_radians Angle to rotate phasor by, in radians
 * @return Rotated phasor
 */
static inline float complex rotate_phasor_float(float complex phasor, float angle_radians) {
    return phasor * cexpf(I * angle_radians);
}

/**
 * @brief 
 * Converts an angular frequency to a complex frequency
 * @param angular_frequency Angular frequency, in radians per sample
 * @return Complex frequency
 */
static inline float complex angular_to_complex_frequency_float(float angular_frequency) {
    return zero_complex_frequency_float * cexpf(I * angular_frequency);
}

static inline float complex ordinary_to_complex_frequency_float(float ordinary_frequency){
    return angular_to_complex_frequency_float(
        ordinary_frequency_to_angular_float(ordinary_frequency)
    );
}

/**
 * @brief 
 * Converts a complex frequency to an angular frequency
 * @param complex_frequency Complex frequency
 * @return Angular frequency, in radians per sample 
 */
static inline float complex_frequency_to_angular_float(float complex complex_frequency) {
    return cargf(complex_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to a normalized ordinary frequency
 * @param complex_frequency Complex frequency
 * @return Normalized ordinary frequency 
 */
static inline float complex_frequency_to_ordinary_float(float complex complex_frequency) {
    return angular_frequency_to_ordinary_float(
        complex_frequency_to_angular_float(complex_frequency)
    );
}


#endif
