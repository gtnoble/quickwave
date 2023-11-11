#ifndef QUICKWAVE_SINUSOID_FIT
#define QUICKWAVE_SINUSOID_FIT

#include "sinusoid.h"
#include "moving_average.h"

/**
 * @brief 
 * Stores the state for a running sinusoid fit.
 * A running sinusoid fit fits a sinusoid over a sliding window
 * This fit is calculated by calculating a single discrete Fourier transform term
 * over the window interval.
 */
typedef struct {
    Sinusoid reference; /** Reference sinusoid. The fit sinusoid is relative to this.*/
    MovingAverageComplex *fit_window; /** Window over which the sinusoid is fit */
} SinusoidFit;

/**
 * @brief 
 * Makes a constant-frequency running sinusoid fit
 * @param window_length window length for fitting the sinusoid, in samples
 * @param frequency normalized frequency of the fit sinusoids
 * @return sinusoid fitter
 */
SinusoidFit *sinusoid_fit_make(size_t window_length, double frequency);

/**
 * @brief 
 * Frees the memory associated with a sinusoid fit
 * @param model Sinusoid fit
 */
void sinusoid_fit_free(SinusoidFit *model);

/**
 * @brief 
 * Evaluates and updates a sinusoid fit.
 * The incoming sample is added to this window and the sinusoid fit is calculated.
 * @param input incoming sample
 * @param model sinusoid fit
 * @return resulting fit sinusoid 
 */
Sinusoid sinusoid_fit_evaluate(double input, SinusoidFit *model);

#endif