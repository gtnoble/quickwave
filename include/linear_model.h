
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
} LinearModelDouble;

/**
 * @brief 
 * Stores the state for a running linear fit.
 */
typedef struct {
    MovingAverageRealDouble *intercept_estimator; /** Estimates the y-intercept of the linear fit */
    DigitalFilterRealDouble *slope_estimator; /** Estimates the slope of the linear fit. This is a first-order, first-derivative Savitzky-Golay filterA. */
    double intercept_x_offset; /** Number of samples estimator values are delayed. Used to adjust the returned fit to be relative to the most recent sample. */
} LinearEstimatorDouble;

/**
 * @brief 
 * Creates and allocates a new running linear fit
 * @param window_length number of samples over which the fit is calculated
 * @return Running linear fit
 */
LinearEstimatorDouble *linear_estimator_new_double(size_t window_length);

/**
 * @brief 
 * Frees the memory associated with a running linear fit
 * @param model Running linear fit
 */
void linear_estimator_free_double(LinearEstimatorDouble *model);

/**
 * @brief 
 * Estimates the linear function model for the current sample.
 * @param input Current sample
 * @param estimator Running linear fit
 * @return Linear function parameters
 */
LinearModelDouble linear_estimator_estimate_double(double input, LinearEstimatorDouble *estimator);

/**
 * @brief 
 * Performs linear prediction.
 * @param x Sample location at which to predict. This is relative to the most recent sample. So `1` would be one sample in the future.
 * @param model Linear function parameters
 * @return Predicted value 
 */
double linear_model_predict_double(double x, LinearModelDouble model);




/**
 * @brief 
 * Stores the parameters of a linear function
 */
typedef struct {
    float intercept;
    float slope;
} LinearModelFloat;

/**
 * @brief 
 * Stores the state for a running linear fit.
 */
typedef struct {
    MovingAverageRealFloat *intercept_estimator; /** Estimates the y-intercept of the linear fit */
    DigitalFilterRealFloat *slope_estimator; /** Estimates the slope of the linear fit. This is a first-order, first-derivative Savitzky-Golay filterA. */
    float intercept_x_offset; /** Number of samples estimator values are delayed. Used to adjust the returned fit to be relative to the most recent sample. */
} LinearEstimatorFloat;

/**
 * @brief 
 * Creates and allocates a new running linear fit
 * @param window_length number of samples over which the fit is calculated
 * @return Running linear fit
 */
LinearEstimatorFloat *linear_estimator_new_float(size_t window_length);

/**
 * @brief 
 * Frees the memory associated with a running linear fit
 * @param model Running linear fit
 */
void linear_estimator_free_float(LinearEstimatorFloat *model);

/**
 * @brief 
 * Estimates the linear function model for the current sample.
 * @param input Current sample
 * @param estimator Running linear fit
 * @return Linear function parameters
 */
LinearModelFloat linear_estimator_estimate_float(float input, LinearEstimatorFloat *estimator);

/**
 * @brief 
 * Performs linear prediction.
 * @param x Sample location at which to predict. This is relative to the most recent sample. So `1` would be one sample in the future.
 * @param model Linear function parameters
 * @return Predicted value 
 */
float linear_model_predict_float(float x, LinearModelFloat model);


#endif
