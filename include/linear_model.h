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

#endif