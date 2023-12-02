(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_LINEAR_MODEL
#define QUICKWAVE_LINEAR_MODEL

#include \"filter.h\"
#include \"moving_average.h\"

"
    (generate-text
        linear-estimator-schema
"
/**
 * @brief 
 * Stores the parameters of a linear function
 */
typedef struct {
    ${number-type} intercept;
    ${number-type} slope;
} ${linear-model-type};

/**
 * @brief 
 * Stores the state for a running linear fit.
 */
typedef struct {
    ${moving-average-type} *intercept_estimator; /** Estimates the y-intercept of the linear fit */
    ${filter-type} *slope_estimator; /** Estimates the slope of the linear fit. This is a first-order, first-derivative Savitzky-Golay filterA. */
    ${number-type} intercept_x_offset; /** Number of samples estimator values are delayed. Used to adjust the returned fit to be relative to the most recent sample. */
} ${linear-estimator-type};

/**
 * @brief 
 * Creates and allocates a new running linear fit
 * @param window_length number of samples over which the fit is calculated
 * @return Running linear fit
 */
${linear-estimator-type} *linear_estimator_new${function-tag}(size_t window_length);

/**
 * @brief 
 * Frees the memory associated with a running linear fit
 * @param model Running linear fit
 */
void linear_estimator_free${function-tag}(${linear-estimator-type} *model);

/**
 * @brief 
 * Estimates the linear function model for the current sample.
 * @param input Current sample
 * @param estimator Running linear fit
 * @return Linear function parameters
 */
${linear-model-type} linear_estimator_estimate${function-tag}(${number-type} input, ${linear-estimator-type} *estimator);

/**
 * @brief 
 * Performs linear prediction.
 * @param x Sample location at which to predict. This is relative to the most recent sample. So `1` would be one sample in the future.
 * @param model Linear function parameters
 * @return Predicted value 
 */
${number-type} linear_model_predict${function-tag}(${number-type} x, ${linear-model-type} model);

")

"
#endif
"

)