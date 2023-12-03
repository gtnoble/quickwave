(load-from-path "substitutions.scm")

(output-text

"
#ifndef QUICKWAVE_OSCILLATOR
#define QUICKWAVE_OSCILLATOR

#include <complex.h>
#include \"memory.h\"

"
    (generate-text
        oscillator-schema
"
/**
 * @brief 
 * A numerically-controlled oscillator with a specified phase and frequency
 */
typedef struct {
    ${number-base-type} complex complex_frequency; /** Frequency (phase velocity) of numerically-controlled oscillator */
    ${number-base-type} complex phasor; /** Amplitude and phase of numerically-controlled oscillator */
    Deallocator *free;
} ${oscillator-type};

/**
 * @brief 
 * Constructs a numerically-controlled oscillator
 * @param phase_degrees Phase in degrees
 * @param normalized_frequency Normalized frequency
 * @return Constructed numerically-controlled oscillator 
 */
${oscillator-type} *oscillator_make${function-tag}(
    ${number-base-type} phase_degrees, 
    ${number-base-type} normalized_frequency,
    const MemoryManager *manager
);

void oscillator_free${function-tag}(${oscillator-type} *oscillator);

/**
 * @brief 
 * Shifts the phase of a numerically-controlled oscillator
 * @param angle_radians Angle to shift phase by, in radians
 * @param x Oscillator to be phase-shifted
 * @param shifted_output Phase-shifted numerically-controlled oscillator 
 */
void oscillator_shift_phase${function-tag}(${number-base-type} angle, ${oscillator-type} *shifted_output, const ${oscillator-type} *x);

/**
 * @brief 
 * In-phase (real) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return In-phase component value
 */
${number-base-type} oscillator_inphase${function-tag}(const ${oscillator-type} *x);

/**
 * @brief 
 * Quadrature (imaginary) component of a numerically-controlled oscillator
 * @param x Oscillator
 * @return Quadrature component value 
 */
${number-base-type} oscillator_quadrature${function-tag}(const ${oscillator-type} *x);

${number-base-type} complex oscillator_phase${function-tag}(const ${oscillator-type} *osc);

/**
 * @brief 
 * Phase of a numerically-controlled oscillator, in radians
 * @param x Oscillator
 * @return Phase 
 */
${number-base-type} oscillator_angular_phase${function-tag}(const ${oscillator-type} *x);

${number-base-type} complex oscillator_frequency${function-tag}(const ${oscillator-type} *osc);

/**
 * @brief 
 * Angular frequency of a numerically-controlled oscillator, in radians per sample
 * @param x Oscillator
 * @return Angular frequency 
 */
${number-base-type} oscillator_angular_freq${function-tag}(const ${oscillator-type} *x);

/**
 * @brief 
 * Estimates the updated numerically-controlled oscillator after a change in time
 * @param delta_time Change in time in samples
 * @param x Initial numerically-controlled oscillator
 * @param prediction Updated numerically-controlled oscillator 
 */
void oscillator_predict${function-tag}(
    ${number-base-type} delta_time, 
    ${oscillator-type} *prediction, 
    const ${oscillator-type} *x
);

/**
 * @brief 
 * Updates frequency and phase of a Numerically-controlled osciallator (NCO) for the next time step.
 * @param update_frequency Next frequency
 * @param nco Oscillator to update
 */
void oscillator_update${function-tag}(${number-base-type} complex complex_freq, ${oscillator-type} *nco);

")

"
#endif
")