#ifndef SQLDSP_SINUSOID
#define SQLDSP_SINUSOID

#include <complex.h>
#include "buffer.h"

/**
 * @brief 
 * A sinusoid with a specified phase and frequency
 */
typedef struct {
    double _Complex complex_frequency;
    double _Complex phasor;
} Sinusoid;

/**
 * @brief 
 * Constructs a sinusoid
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed sinusoid 
 */
Sinusoid sinusoid_make(double phase_degrees, double normalized_frequency);

/**
 * @brief 
 * Adds two sinusoids together. Must have equal frequencies.
 * @param a Summand
 * @param b Summand
 * @return Sum
 */
Sinusoid sinusoid_add(Sinusoid a, Sinusoid b);

/**
 * @brief 
 * Multiplies two sinusoids together. Must have equal frequencies
 * @param a Multiplicand
 * @param b Multiplicand
 * @return Product
 */
Sinusoid sinusoid_mult(Sinusoid a, Sinusoid b);

/**
 * @brief 
 * Divides two sinusoids. Must have equal frequencies
 * @param a Dividend
 * @param b Divisor
 * @return Quotient
 */
Sinusoid sinusoid_div(Sinusoid a, Sinusoid b);

/**
 * @brief 
 * Normalizes a sinusoid. (Scales to amplitude of 1)
 * @param x Sinusoid to be normalized
 * @return Normalized sinusoid
 */
Sinusoid sinusoid_normalize(Sinusoid x);

/**
 * @brief 
 * Negates the phase of a sinusoid. (45 degrees becomes -45 degrees)
 * @param x Sinusoid
 * @return Sinusoid with negated phase
 */
Sinusoid sinusoid_negate_phase(Sinusoid x);

/**
 * @brief 
 * Shifts the phase of a sinusoid
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Sinusoid to be phase-shifted
 * @return Phase-shifted sinusoid 
 */
Sinusoid sinusoid_shift_phase(double angle, Sinusoid x);

/**
 * @brief 
 * In-phase (real) component of a sinusoid
 * @param x Sinusoid
 * @return In-phase component value
 */
double sinusoid_inphase(Sinusoid x);

/**
 * @brief 
 * Quadrature (imaginary) component of a sinusoid
 * @param x Sinusoid
 * @return Quadrature component value 
 */
double sinusoid_quadrature(Sinusoid x);

/**
 * @brief 
 * Amplitude of a sinusoid
 * @param x Sinusoid
 * @return Amplitude 
 */
double sinusoid_amplitude(Sinusoid x);

/**
 * @brief 
 * Phase of a sinusoid, in radians
 * @param x Sinusoid
 * @return Phase 
 */
double sinusoid_phase(Sinusoid x);

/**
 * @brief 
 * Angular frequency of a sinusoid, in radians per sample
 * @param x Sinusoid
 * @return Angular frequency 
 */
double sinusoid_angular_freq(Sinusoid x);

#endif


