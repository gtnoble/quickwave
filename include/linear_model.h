#ifndef QUICKWAVE_LINEAR_MODEL
#define QUICKWAVE_LINEAR_MODEL

#include "filter.h"
#include "moving_average.h"

typedef struct {
    double intercept;
    double slope;
} LinearModel;

typedef struct {
    MovingAverage *intercept_estimator;
    DigitalFilterReal *slope_estimator;
    double intercept_x_offset;
} LinearEstimator;


LinearEstimator *linear_estimator_new(size_t window_length);
LinearModel linear_estimator_estimate(double input, LinearEstimator *estimator);
double linear_model_predict(double x, LinearModel model);

#endif