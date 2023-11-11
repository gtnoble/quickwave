#ifndef QUICKWAVE_SINUSOID
#define QUICKWAVE_SINUSOID

#include <complex.h>

/**
 * @brief 
 * A sinusoid with a specified phase and frequency
 */
typedef struct {
    double complex complex_frequency; /** Frequency (phase velocity) of sinusoid */
    double complex phasor; /** Amplitude and phase of sinusoid */
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

/**
 * @brief 
 * Estimates the updated sinusoid after a change in time
 * @param delta_time Change in time in samples
 * @param x Initial sinusoid
 * @return Updated sinusoid 
 */
Sinusoid sinusoid_predict(double delta_time, Sinusoid x);

/**
 * @brief 
 * Scales the amplitude of a sinusoid
 * @param scaling_factor Factor by which to scale.
 * @param x Sinusoid to scale
 * @return Scaled sinusoid 
 */
Sinusoid sinusoid_scale(double scaling_factor, Sinusoid x);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco NCO to update
 * @return Updated NCO
 */
Sinusoid nco_update(double complex complex_freq, Sinusoid nco);

/**
 * @brief 
 * Performs quadrature mixing. Converts a baseband signal to and from an I/Q (inphase and quadrature) signal.
 * @param reference Oscillator used as the reference for the mixing.
 * @param input Next input signal value to mix
 * @return Mixed value
 */
Sinusoid quadrature_mix(Sinusoid reference, double complex input);

#endif


