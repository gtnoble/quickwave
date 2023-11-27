(load-from-path "substitutions.scm")

(output-text

"
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include \"savgol.h\"
#include \"filter.h\"
#include \"constants.h\"
#include \"assertions.h\"

"

(generate-text
    real-number-schema
"
/**
 * @brief 
 * Normalized sinc function
 * @param x input
 * @return result
 */
${number-type} sinc${function-tag}(${number-type} x);

/**
 * @brief 
 * Dirac delta function
 * @param x Input
 * @return Result 
 */
${number-type} dirac_delta${function-tag}(${number-type} x);

${number-type} sinc${function-tag}(${number-type} x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

${number-type} dirac_delta${function-tag}(${number-type} x) {
    return x == 0.0 ? 1.0 : 0.0;
}
")

(generate-text
    filter-schema
"

${number-type} filter_evaluate_digital_filter${function-tag}(${number-type} input, ${filter-type} *filter) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    ${number-type} accumulate = 0.0;
    vector_shift${function-tag}(input, filter->previous_input);
    accumulate +=
        vector_dot${function-tag}(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += vector_dot${function-tag}(filter->feedback, filter->previous_output);
        vector_shift${function-tag}(accumulate, filter->previous_output);
    }
    return accumulate;
}

${filter-type} *filter_make_digital_filter${function-tag}(
    const ${vector-type} *feedforward,
    const ${vector-type} *feedback
) {
    ${filter-type} *filter = malloc(sizeof(${filter-type}));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = vector_duplicate${function-tag}(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = vector_new${function-tag}(vector_length${function-tag}(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = vector_duplicate${function-tag}(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            vector_complex_new(vector_length${function-tag}(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        vector_free${function-tag}(filter->feedback);
    fail_allocate_feedback:
        vector_free${function-tag}(filter->previous_input);
    fail_allocate_previous_input:
        vector_free${function-tag}(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}


void filter_reset_digital_filter${function-tag}(${filter-type} *filter) {
    assert_not_null(filter);
    
    vector_reset${function-tag}(filter->previous_input);
    if (filter->previous_output)
        vector_reset${function-tag}(filter->previous_output);
}


void filter_free_digital_filter${function-tag}(${filter-type} *filter) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    vector_free${function-tag}(filter->feedforward);

    assert_not_null(filter->previous_input);
    vector_free${function-tag}(filter->previous_input);

    if (filter->feedback != NULL) {
        vector_free${function-tag}(filter->feedback);
        assert_not_null(filter->previous_output);
        vector_free${function-tag}(filter->previous_output);
    }
    free(filter);
}

${filter-type} *filter_make_savgol${function-tag}(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    ${vector-type} *feedforward = vector_new${function-tag}(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *vector_element${function-tag}(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    ${filter-type} *filter = filter_make_digital_filter${function-tag}(feedforward, NULL);
    vector_free${function-tag}(feedforward);
    return filter;
}

${filter-type} *filter_make_ewma${function-tag}(${number-base-type} alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    ${vector-type} *feedforward = vector_new${function-tag}(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *vector_element${function-tag}(0, feedforward) = alpha;

    ${vector-type} *feedback = vector_complex_new(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *vector_element${function-tag}(0, feedback) = 1 - alpha;

    ${filter-type} *filter = 
        filter_make_digital_filter${function-tag}(feedforward, feedback);

    vector_free${function-tag}(feedforward);
    vector_free${function-tag}(feedback);
    
    return filter;

    feedback_allocation_failed:
        vector_free${function-tag}(feedforward);
    feedforward_allocation_failed:
        return NULL;
}

${filter-type} *filter_make_first_order_iir${function-tag}(${number-base-type} cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    ${number-base-type} angular_frequency = ordinary_frequency_to_angular(cutoff_frequency);
    return filter_make_ewma(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

${filter-type} *filter_make_sinc${function-tag}(
    ${number-base-type} cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular;

    ${vector-type} *filter_coefficients = vector_new${function-tag}(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    ${number-base-type} kernel_shift = ((int) length - 1) / 2.0;
    ${number-type} uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *vector_element${function-tag}(i, filter_coefficients) = 
            sinc_real_${number-base-type}(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *vector_element${function-tag}(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        ${number-type} dc_corrected_coefficient = 
            *vector_element${function-tag}(i, filter_coefficients) / uncorrected_dc_gain;
        *vector_element${function-tag}(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta_${number-base-type}(i - length / 2) - dc_corrected_coefficient;
    }
    ${filter-type} *filter = 
        filter_make_digital_filter${function-tag}(filter_coefficients, NULL);
    
    vector_free${function-tag}(filter_coefficients);

    return filter;
}

")

)