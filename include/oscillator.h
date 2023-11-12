#ifndef QUICKWAVE_OSCILLATOR
#define QUICKWAVE_OSCILLATOR

#include <complex.h>

/**
 * @brief 
 * A numerically-controlled oscillator with a specified phase and frequency
 */
typedef struct {
    double complex complex_frequency; /** Frequency (phase velocity) of numerically-controlled oscillator */
    double complex phasor; /** Amplitude and phase of numerically-controlled oscillator */
} Oscillator;

/**
 * @brief 
 * Constructs a numerically-controlled oscillator
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed numerically-controlled oscillator 
 */
Oscillator oscillator_make(double phase_degrees, double normalized_frequency);

/**
 * @brief 
 * Shifts the phase of a numerically-controlled oscillator
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Oscillator to be phase-shifted
 * @return Phase-shifted numerically-controlled oscillator 
 */
Oscillator nco_shift_phase(double angle, Oscillator x);

/**
 * @brief 
 * In-phase (real) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return In-phase component value
 */
double oscillator_inphase(Oscillator x);

/**
 * @brief 
 * Quadrature (imaginary) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return Quadrature component value 
 */
double oscillator_quadrature(Oscillator x);

double complex oscillator_phase(Oscillator osc);

/**
 * @brief 
 * Phase of a numerically-controlled oscillator, in radians
 * @param x Oscillator
 * @return Phase 
 */
double oscillator_angular_phase(Oscillator x);

double complex oscillator_frequency(Oscillator osc);

/**
 * @brief 
 * Angular frequency of a numerically-controlled oscillator, in radians per sample
 * @param x Oscillator
 * @return Angular frequency 
 */
double oscillator_angular_freq(Oscillator x);

/**
 * @brief 
 * Estimates the updated numerically-controlled oscillator after a change in time
 * @param delta_time Change in time in samples
 * @param x Initial numerically-controlled oscillator
 * @return Updated numerically-controlled oscillator 
 */
Oscillator oscillator_predict(double delta_time, Oscillator x);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco NCO to update
 * @return Updated NCO state
 */
Oscillator oscillator_update(double complex complex_freq, Oscillator *nco);

#endif


