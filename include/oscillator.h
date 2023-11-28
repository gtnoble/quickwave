
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
} OscillatorDouble;

/**
 * @brief 
 * Constructs a numerically-controlled oscillator
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed numerically-controlled oscillator 
 */
OscillatorDouble oscillator_make_double(double phase_degrees, double normalized_frequency);

/**
 * @brief 
 * Shifts the phase of a numerically-controlled oscillator
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Oscillator to be phase-shifted
 * @return Phase-shifted numerically-controlled oscillator 
 */
OscillatorDouble nco_shift_phase_double(double angle, OscillatorDouble x);

/**
 * @brief 
 * In-phase (real) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return In-phase component value
 */
double oscillator_inphase_double(OscillatorDouble x);

/**
 * @brief 
 * Quadrature (imaginary) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return Quadrature component value 
 */
double oscillator_quadrature_double(OscillatorDouble x);

double complex oscillator_phase_double(OscillatorDouble osc);

/**
 * @brief 
 * Phase of a numerically-controlled oscillator, in radians
 * @param x Oscillator
 * @return Phase 
 */
double oscillator_angular_phase_double(OscillatorDouble x);

double complex oscillator_frequency_double(OscillatorDouble osc);

/**
 * @brief 
 * Angular frequency of a numerically-controlled oscillator, in radians per sample
 * @param x Oscillator
 * @return Angular frequency 
 */
double oscillator_angular_freq_double(OscillatorDouble x);

/**
 * @brief 
 * Estimates the updated numerically-controlled oscillator after a change in time
 * @param delta_time Change in time in samples
 * @param x Initial numerically-controlled oscillator
 * @return Updated numerically-controlled oscillator 
 */
OscillatorDouble oscillator_predict_double(double delta_time, OscillatorDouble x);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco Oscillator to update
 * @return Updated oscillator state
 */
OscillatorDouble oscillator_update_double(double complex complex_freq, OscillatorDouble *nco);




/**
 * @brief 
 * A numerically-controlled oscillator with a specified phase and frequency
 */
typedef struct {
    float complex complex_frequency; /** Frequency (phase velocity) of numerically-controlled oscillator */
    float complex phasor; /** Amplitude and phase of numerically-controlled oscillator */
} OscillatorFloat;

/**
 * @brief 
 * Constructs a numerically-controlled oscillator
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed numerically-controlled oscillator 
 */
OscillatorFloat oscillator_make_float(float phase_degrees, float normalized_frequency);

/**
 * @brief 
 * Shifts the phase of a numerically-controlled oscillator
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Oscillator to be phase-shifted
 * @return Phase-shifted numerically-controlled oscillator 
 */
OscillatorFloat nco_shift_phase_float(float angle, OscillatorFloat x);

/**
 * @brief 
 * In-phase (real) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return In-phase component value
 */
float oscillator_inphase_float(OscillatorFloat x);

/**
 * @brief 
 * Quadrature (imaginary) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return Quadrature component value 
 */
float oscillator_quadrature_float(OscillatorFloat x);

float complex oscillator_phase_float(OscillatorFloat osc);

/**
 * @brief 
 * Phase of a numerically-controlled oscillator, in radians
 * @param x Oscillator
 * @return Phase 
 */
float oscillator_angular_phase_float(OscillatorFloat x);

float complex oscillator_frequency_float(OscillatorFloat osc);

/**
 * @brief 
 * Angular frequency of a numerically-controlled oscillator, in radians per sample
 * @param x Oscillator
 * @return Angular frequency 
 */
float oscillator_angular_freq_float(OscillatorFloat x);

/**
 * @brief 
 * Estimates the updated numerically-controlled oscillator after a change in time
 * @param delta_time Change in time in samples
 * @param x Initial numerically-controlled oscillator
 * @return Updated numerically-controlled oscillator 
 */
OscillatorFloat oscillator_predict_float(float delta_time, OscillatorFloat x);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco Oscillator to update
 * @return Updated oscillator state
 */
OscillatorFloat oscillator_update_float(float complex complex_freq, OscillatorFloat *nco);


#endif
