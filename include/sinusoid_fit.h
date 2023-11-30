
#ifndef QUICKWAVE_SINUSOID_FIT
#define QUICKWAVE_SINUSOID_FIT

#include "oscillator.h"
#include "moving_average.h"


/**
 * @brief 
 * Stores the state for a running sinusoid fit.
 * A running sinusoid fit fits a sinusoid over a sliding window
 * This fit is calculated by calculating a single discrete Fourier transform term
 * over the window interval.
 */
typedef struct {
    OscillatorDouble reference; /** Reference sinusoid. The fit sinusoid is relative to this.*/
    MovingAverageComplexDouble *fit_window; /** Window over which the sinusoid is fit */
} SinusoidFitDouble;

/**
 * @brief 
 * Makes a constant-frequency running sinusoid fit
 * @param window_length window length for fitting the sinusoid, in samples
 * @param frequency normalized frequency of the fit sinusoids
 * @return sinusoid fitter
 */
SinusoidFitDouble *sinusoid_fit_make_double(size_t window_length, double frequency);

/**
 * @brief 
 * Frees the memory associated with a sinusoid fit
 * @param model Oscillator fit
 */
void sinusoid_fit_free_double(SinusoidFitDouble *model);

/**
 * @brief 
 * Evaluates and updates a sinusoid fit.
 * The incoming sample is added to this window and the sinusoid fit is calculated.
 * @param input incoming sample
 * @param model sinusoid fit
 * @return resulting fit sinusoid 
 */
OscillatorDouble sinusoid_fit_evaluate_double(double input, SinusoidFitDouble *model);




/**
 * @brief 
 * Stores the state for a running sinusoid fit.
 * A running sinusoid fit fits a sinusoid over a sliding window
 * This fit is calculated by calculating a single discrete Fourier transform term
 * over the window interval.
 */
typedef struct {
    OscillatorFloat reference; /** Reference sinusoid. The fit sinusoid is relative to this.*/
    MovingAverageComplexFloat *fit_window; /** Window over which the sinusoid is fit */
} SinusoidFitFloat;

/**
 * @brief 
 * Makes a constant-frequency running sinusoid fit
 * @param window_length window length for fitting the sinusoid, in samples
 * @param frequency normalized frequency of the fit sinusoids
 * @return sinusoid fitter
 */
SinusoidFitFloat *sinusoid_fit_make_float(size_t window_length, float frequency);

/**
 * @brief 
 * Frees the memory associated with a sinusoid fit
 * @param model Oscillator fit
 */
void sinusoid_fit_free_float(SinusoidFitFloat *model);

/**
 * @brief 
 * Evaluates and updates a sinusoid fit.
 * The incoming sample is added to this window and the sinusoid fit is calculated.
 * @param input incoming sample
 * @param model sinusoid fit
 * @return resulting fit sinusoid 
 */
OscillatorFloat sinusoid_fit_evaluate_float(float input, SinusoidFitFloat *model);


#endif
