(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_SINUSOID_FIT
#define QUICKWAVE_SINUSOID_FIT

#include \"oscillator.h\"
#include \"moving_average.h\"

"
    (generate-text
        sinusoid-fit-schema
"
/**
 * @brief 
 * Stores the state for a running sinusoid fit.
 * A running sinusoid fit fits a sinusoid over a sliding window
 * This fit is calculated by calculating a single discrete Fourier transform term
 * over the window interval.
 */
typedef struct {
    ${oscillator-type} reference; /** Reference sinusoid. The fit sinusoid is relative to this.*/
    MovingAverageComplex${struct-type-suffix} *fit_window; /** Window over which the sinusoid is fit */
} ${sinusoid-fit-type};

/**
 * @brief 
 * Makes a constant-frequency running sinusoid fit
 * @param window_length window length for fitting the sinusoid, in samples
 * @param frequency normalized frequency of the fit sinusoids
 * @return sinusoid fitter
 */
${sinusoid-fit-type} *sinusoid_fit_make${function-tag}(size_t window_length, ${number-type} frequency);

/**
 * @brief 
 * Frees the memory associated with a sinusoid fit
 * @param model Oscillator fit
 */
void sinusoid_fit_free${function-tag}(${sinusoid-fit-type} *model);

/**
 * @brief 
 * Evaluates and updates a sinusoid fit.
 * The incoming sample is added to this window and the sinusoid fit is calculated.
 * @param input incoming sample
 * @param model sinusoid fit
 * @return resulting fit sinusoid 
 */
${oscillator-type} sinusoid_fit_evaluate${function-tag}(${number-type} input, ${sinusoid-fit-type} *model);

")

"
#endif
"
)