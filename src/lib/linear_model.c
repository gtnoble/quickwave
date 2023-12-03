
#include "linear_model.h"
#include "filter.h"
#include "moving_average.h"
#include "assertions.h"
#include "memory.h"


LinearEstimatorDouble *linear_estimator_new_double(
    size_t window_length, 
    MemoryManager *manager
) {
    LinearEstimatorDouble *estimator = manager->allocate(sizeof(LinearEstimatorDouble));
    if (estimator == NULL) {
        return NULL;
    }

    estimator->free = manager->deallocate;

    MovingAverageRealDouble *intercept_estimator = moving_average_make_real_double(window_length, manager);
    if (intercept_estimator == NULL) {
        goto intercept_estimator_alloc_failure;
    }

    DigitalFilterRealDouble *slope_estimator = filter_make_savgol_real_double(window_length, 1, 1, manager);
    if (slope_estimator == NULL) {
        goto slope_estimator_alloc_failure;
    }

    estimator->intercept_estimator = intercept_estimator;
    estimator->slope_estimator = slope_estimator;
    estimator->intercept_x_offset = window_length / 2;

    return estimator;

    slope_estimator_alloc_failure:
        moving_average_free_real_double(intercept_estimator);
    intercept_estimator_alloc_failure:
        estimator->free(estimator);
        return NULL;
}

void linear_estimator_free_double(LinearEstimatorDouble *model) {
    moving_average_free_real_double(model->intercept_estimator);
    filter_free_digital_filter_real_double(model->slope_estimator);
    model->free(model);
}

void linear_estimator_estimate_double(
    double input, 
    LinearModelDouble *output_estimate,
    LinearEstimatorDouble *estimator
) {
    assert_not_null(estimator);

    double slope = filter_evaluate_digital_filter_real_double(input, estimator->slope_estimator);
    double intercept = 
        moving_average_evaluate_real_double(input, estimator->intercept_estimator) + 
        slope * 
        estimator->intercept_x_offset;

    output_estimate->intercept = intercept;
    output_estimate->slope = slope;
}

double linear_model_predict_double(double x, const LinearModelDouble *model) {
    return model->intercept + model->slope * x;
}



LinearEstimatorFloat *linear_estimator_new_float(
    size_t window_length, 
    MemoryManager *manager
) {
    LinearEstimatorFloat *estimator = manager->allocate(sizeof(LinearEstimatorFloat));
    if (estimator == NULL) {
        return NULL;
    }

    estimator->free = manager->deallocate;

    MovingAverageRealFloat *intercept_estimator = moving_average_make_real_float(window_length, manager);
    if (intercept_estimator == NULL) {
        goto intercept_estimator_alloc_failure;
    }

    DigitalFilterRealFloat *slope_estimator = filter_make_savgol_real_float(window_length, 1, 1, manager);
    if (slope_estimator == NULL) {
        goto slope_estimator_alloc_failure;
    }

    estimator->intercept_estimator = intercept_estimator;
    estimator->slope_estimator = slope_estimator;
    estimator->intercept_x_offset = window_length / 2;

    return estimator;

    slope_estimator_alloc_failure:
        moving_average_free_real_float(intercept_estimator);
    intercept_estimator_alloc_failure:
        estimator->free(estimator);
        return NULL;
}

void linear_estimator_free_float(LinearEstimatorFloat *model) {
    moving_average_free_real_float(model->intercept_estimator);
    filter_free_digital_filter_real_float(model->slope_estimator);
    model->free(model);
}

void linear_estimator_estimate_float(
    float input, 
    LinearModelFloat *output_estimate,
    LinearEstimatorFloat *estimator
) {
    assert_not_null(estimator);

    float slope = filter_evaluate_digital_filter_real_float(input, estimator->slope_estimator);
    float intercept = 
        moving_average_evaluate_real_float(input, estimator->intercept_estimator) + 
        slope * 
        estimator->intercept_x_offset;

    output_estimate->intercept = intercept;
    output_estimate->slope = slope;
}

float linear_model_predict_float(float x, const LinearModelFloat *model) {
    return model->intercept + model->slope * x;
}
