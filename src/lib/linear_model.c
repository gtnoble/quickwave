#include "linear_model.h"
#include "filter.h"
#include "moving_average.h"
#include "assertions.h"

LinearEstimator *linear_estimator_new(size_t window_length) {
    LinearEstimator *estimator = malloc(sizeof(LinearEstimator));
    if (estimator == NULL) {
        return NULL;
    }

    MovingAverage *intercept_estimator = moving_average_make(window_length);
    if (intercept_estimator == NULL) {
        goto intercept_estimator_alloc_failure;
    }

    DigitalFilterReal *slope_estimator = filter_make_savgol(window_length, 1, 1);
    if (slope_estimator == NULL) {
        goto slope_estimator_alloc_failure;
    }

    estimator->intercept_estimator = intercept_estimator;
    estimator->slope_estimator = slope_estimator;
    estimator->intercept_x_offset = window_length / 2;

    return estimator;

    slope_estimator_alloc_failure:
        moving_average_free(intercept_estimator);
    intercept_estimator_alloc_failure:
        free(estimator);
        return NULL;
}

LinearModel linear_estimator_estimate(double input, LinearEstimator *estimator) {
    assert_not_null(estimator);

    double slope = filter_evaluate_digital_filter_real(input, estimator->slope_estimator);
    double intercept = 
        moving_average_evaluate(input, estimator->intercept_estimator) + 
        slope * 
        estimator->intercept_x_offset;
    LinearModel model = {
        .intercept = intercept,
        .slope = slope
    };
    return model;
}

double linear_model_predict(double x, LinearModel model) {
    return model.intercept + model.slope * x;
}