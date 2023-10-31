#include <assert.h>
#include <complex.h>

#include "sinusoid.h"
#include "buffer.h"
#include "constants.h"

double complex rotate_phasor(double complex phasor, double angle_radians);

const double complex zero_complex_frequency = 1.0;
const double complex zero_phase_phasor = 1.0;

/**
 * @brief 
 * Constructs a sinusoid
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed sinusoid 
 */
Sinusoid sinusoid_make(double phase_degrees, double normalized_frequency) {
    Sinusoid result = {
        .complex_frequency = angular_to_complex_frequency(
            ordinary_frequency_to_angular(normalized_frequency)
        ),
        .phasor = rotate_phasor(zero_phase_phasor, degrees_to_radians(phase_degrees))
    };
    return result;
}

/**
 * @brief 
 * Adds two sinusoids together. Must have equal frequencies.
 * @param a Summand
 * @param b Summand
 * @return Sum
 */
Sinusoid sinusoid_add(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor + b.phasor
    };
    return result;
}

/**
 * @brief 
 * Multiplies two sinusoids together. Must have equal frequencies
 * @param a Multiplicand
 * @param b Multiplicand
 * @return Product
 */
Sinusoid sinusoid_mult(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor * b.phasor
    };
    return result;
}

/**
 * @brief 
 * Divides two sinusoids. Must have equal frequencies
 * @param a Dividend
 * @param b Divisor
 * @return Quotient
 */
Sinusoid sinusoid_div(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor / b.phasor
    };
    return result;
}

double sinusoid_evaluate(Sinusoid x) {
    return sinusoid_inphase(x) + sinusoid_quadrature(x);
}

/**
 * @brief 
 * In-phase (real) component of a sinusoid
 * @param x Sinusoid
 * @return In-phase component value
 */
double sinusoid_inphase(Sinusoid x) {
    return creal(x.phasor);
}

/**
 * @brief 
 * Quadrature (imaginary) component of a sinusoid
 * @param x Sinusoid
 * @return Quadrature component value 
 */
double sinusoid_quadrature(Sinusoid x) {
    return cimag(x.phasor);
}

/**
 * @brief 
 * Amplitude of a sinusoid
 * @param x Sinusoid
 * @return Amplitude 
 */
double sinusoid_amplitude(Sinusoid x) {
    return cabs(x.phasor);
}

/**
 * @brief 
 * Phase of a sinusoid, in radians
 * @param x Sinusoid
 * @return Phase 
 */
double sinusoid_phase(Sinusoid x) {
    return carg(x.phasor);
}

/**
 * @brief 
 * Angular frequency of a sinusoid, in radians per sample
 * @param x Sinusoid
 * @return Angular frequency 
 */
double sinusoid_angular_freq(Sinusoid x) {
    return carg(x.complex_frequency);
}

/**
 * @brief 
 * Normalizes a sinusoid. (Scales to amplitude of 1)
 * @param x Sinusoid to be normalized
 * @return Normalized sinusoid
 */
Sinusoid sinusoid_normalize(Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = x.phasor / cabs(x.phasor)
    };
    return result;
}

/**
 * @brief 
 * Negates the phase of a sinusoid. (45 degrees becomes -45 degrees)
 * @param x Sinusoid
 * @return Sinusoid with negated phase
 */
Sinusoid sinusoid_negate_phase(Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = conj(x.phasor)
    };
    return result;
}

/**
 * @brief 
 * Shifts the phase of a sinusoid
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Sinusoid to be phase-shifted
 * @return Phase-shifted sinusoid 
 */
Sinusoid sinusoid_shift_phase(double angle_radians, Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor(x.phasor, angle_radians)
    };
    return result;
}

/**
 * @brief 
 * Rotates a phasor
 * @param phasor Phasor to be rotated
 * @param angle_radians Angle to rotate phasor by, in radians
 * @return Rotated phasor
 */
double complex rotate_phasor(double complex phasor, double angle_radians) {
    return phasor * cexp(I * angle_radians);
}

/**
 * @brief 
 * Converts an angular frequency to a complex frequency
 * @param angular_frequency Angular frequency, in radians per sample
 * @return Complex frequency
 */
double _Complex angular_to_complex_frequency(double angular_frequency) {
    return zero_complex_frequency * cexp(I * angular_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to an angular frequency
 * @param complex_frequency Complex frequency
 * @return Angular frequency, in radians per sample 
 */
double complex_frequency_to_angular(double complex complex_frequency) {
    return carg(complex_frequency);
}

/**
 * @brief 
 * Converts a complex frequency to a normalized ordinary frequency
 * @param complex_frequency Complex frequency
 * @return Normalized ordinary frequency 
 */
double complex_frequency_to_ordinary(double complex complex_frequency) {
    return angular_frequency_to_ordinary(
        complex_frequency_to_angular(complex_frequency)
    );
}