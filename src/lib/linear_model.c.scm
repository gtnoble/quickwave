(load-from-path "substitutions.scm")

(output-text
"
#include \"linear_model.h\"
#include \"filter.h\"
#include \"moving_average.h\"
#include \"assertions.h\"
#include \"memory.h\"

"

    (generate-text
        linear-estimator-schema
"
${linear-estimator-type} *linear_estimator_new${function-tag}(
    size_t window_length, 
    MemoryManager *manager
) {
    ${linear-estimator-type} *estimator = manager->allocate(sizeof(${linear-estimator-type}));
    if (estimator == NULL) {
        return NULL;
    }

    estimator->free = manager->deallocate;

    ${moving-average-type} *intercept_estimator = moving_average_make_real${function-tag}(window_length, manager);
    if (intercept_estimator == NULL) {
        goto intercept_estimator_alloc_failure;
    }

    ${filter-type} *slope_estimator = filter_make_savgol_real${function-tag}(window_length, 1, 1, manager);
    if (slope_estimator == NULL) {
        goto slope_estimator_alloc_failure;
    }

    estimator->intercept_estimator = intercept_estimator;
    estimator->slope_estimator = slope_estimator;
    estimator->intercept_x_offset = window_length / 2;

    return estimator;

    slope_estimator_alloc_failure:
        moving_average_free_real${function-tag}(intercept_estimator);
    intercept_estimator_alloc_failure:
        estimator->free(estimator);
        return NULL;
}

void linear_estimator_free${function-tag}(${linear-estimator-type} *model) {
    moving_average_free_real${function-tag}(model->intercept_estimator);
    filter_free_digital_filter_real${function-tag}(model->slope_estimator);
    model->free(model);
}

void linear_estimator_estimate${function-tag}(
    ${number-type} input, 
    ${linear-model-type} *output_estimate,
    ${linear-estimator-type} *estimator
) {
    assert_not_null(estimator);

    ${number-type} slope = filter_evaluate_digital_filter_real${function-tag}(input, estimator->slope_estimator);
    ${number-type} intercept = 
        moving_average_evaluate_real${function-tag}(input, estimator->intercept_estimator) + 
        slope * 
        estimator->intercept_x_offset;

    output_estimate->intercept = intercept;
    output_estimate->slope = slope;
}

${number-type} linear_model_predict${function-tag}(${number-type} x, const ${linear-model-type} *model) {
    return model->intercept + model->slope * x;
}
")
)