#ifndef QUICKWAVE_LINEAR_MODEL
#define QUICKWAVE_LINEAR_MODEL

#include "filter.h"
#include "moving_average.h"

/**
 * @brief 
 * Stores the parameters of a linear function
 */
typedef struct {
    double intercept;
    double slope;
} LinearModel;

/**
 * @brief 
 * Stores the state for a running linear fit.
 */
typedef struct {
    MovingAverageReal *intercept_estimator; /** Estimates the y-intercept of the linear fit */
    DigitalFilterReal *slope_estimator; /** Estimates the slope of the linear fit. This is a first-order, first-derivative Savitzky-Golay filterA. */
    double intercept_x_offset; /** Number of samples estimator values are delayed. Used to adjust the returned fit to be relative to the most recent sample. */
} LinearEstimator;

/**
 * @brief 
 * Creates and allocates a new running linear fit
 * @param window_length number of samples over which the fit is calculated
 * @return Running linear fit
 */
LinearEstimator *linear_estimator_new(size_t window_length);

/**
 * @brief 
 * Frees the memory associated with a running linear fit
 * @param model Running linear fit
 */
void linear_estimator_free(LinearEstimator *model);

/**
 * @brief 
 * Estimates the linear function model for the current sample.
 * @param input Current sample
 * @param estimator Running linear fit
 * @return Linear function parameters
 */
LinearModel linear_estimator_estimate(double input, LinearEstimator *estimator);

/**
 * @brief 
 * Performs linear prediction.
 * @param x Sample location at which to predict. This is relative to the most recent sample. So `1` would be one sample in the future.
 * @param model Linear function parameters
 * @return Predicted value 
 */
double linear_model_predict(double x, LinearModel model);

#endif