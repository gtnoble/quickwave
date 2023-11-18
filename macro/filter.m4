include(types.m4)dnl
include(`vector.m4')dnl
divert(-1)

define(`M4_DIGITAL_FILTER_TYPE', 
    `DigitalFilter`'M4_STRUCT_TYPE_TAG($1)')


define(`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER_PROTOTYPE',
`$1 M4_TAGGED_FUNCTION_NAME(filter_evaluate_digital_filter, $1)(
    $1 input, 
    M4_DIGITAL_FILTER_TYPE($1) *filter)')

define(`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER',
`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER_PROTOTYPE($1) {
    assert_not_null(filter);
    assert_not_null(filter->feedforward);

    double accumulate = 0.0;
    M4_TAGGED_VECTOR_SHIFT($1)(input, filter->previous_input);
    accumulate +=
        M4_TAGGED_VECTOR_DOT($1)(
            filter->feedforward,
            filter->previous_input
        );
    if (filter->feedback != NULL) {
        accumulate += M4_TAGGED_VECTOR_DOT($1)(filter->feedback, filter->previous_output);
        M4_TAGGED_VECTOR_SHIFT($1)(accumulate, filter->previous_output);
    }
    return accumulate;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_FILTER_MAKE_DIGITAL_FILTER', `filter_make_digital_filter')

define(`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER_PROTOTYPE',

`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FILTER_MAKE_DIGITAL_FILTER($1)(
    const M4_VECTOR_TYPE($1) *feedforward,
    const M4_VECTOR_TYPE($1) *feedback
)')

define(`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER',
`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER_PROTOTYPE($1) {
    M4_DIGITAL_FILTER_TYPE($1) *filter = malloc(sizeof(M4_DIGITAL_FILTER_TYPE($1)));
    if (filter == NULL)
        return NULL;
    
    assert_valid_vector(feedforward);
    filter->feedforward = M4_TAGGED_VECTOR_DUPLICATE($1)(feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }
    
    filter->previous_input = M4_TAGGED_VECTOR_NEW($1)(M4_TAGGED_VECTOR_LENGTH($1)(feedforward));
    if (filter->previous_input == NULL) {
        goto fail_allocate_previous_input;
    }
    
    if (feedback != NULL) {
        assert_valid_vector(feedback);
        filter->feedback = M4_TAGGED_VECTOR_DUPLICATE($1)(feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
        
        filter->previous_output = 
            M4_TAGGED_VECTOR_NEW($1)(M4_TAGGED_VECTOR_LENGTH($1)(feedback));
        if (filter->previous_output == NULL) {
            goto fail_allocate_previous_output;
        }
    }
    else {
        filter->feedback = NULL;
    }
    
    return filter;
    
    fail_allocate_previous_output:
        M4_TAGGED_VECTOR_FREE($1)(filter->feedback);
    fail_allocate_feedback:
        M4_TAGGED_VECTOR_FREE($1)(filter->previous_input);
    fail_allocate_previous_input:
        M4_TAGGED_VECTOR_FREE($1)(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;

}')


define(`M4_MAKE_FILTER_MAKE_SAVGOL_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_savgol, $1)(size_t filter_length, int derivative, int polynomial_order)')

define(`M4_MAKE_FILTER_MAKE_SAVGOL',
`M4_MAKE_FILTER_MAKE_SAVGOL_PROTOTYPE($1) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    M4_VECTOR_TYPE($1) *feedforward = M4_TAGGED_VECTOR_NEW($1)(filter_length);
    if (feedforward == NULL) {
        return NULL;
    }

    for (size_t i = 1; i <= filter_length; i++) {
        *M4_TAGGED_VECTOR_ELEMENT($1)(- i, feedforward) = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    M4_DIGITAL_FILTER_TYPE($1) *filter = M4_TAGGED_FILTER_MAKE_DIGITAL_FILTER($1)(feedforward, NULL);
    M4_TAGGED_VECTOR_FREE($1)(feedforward);
    return filter;
}')


define(`M4_MAKE_FILTER_RESET_DIGITAL_FILTER_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(filter_reset_digital_filter, $1)(M4_DIGITAL_FILTER_TYPE($1) *filter)')

define(`M4_MAKE_FILTER_RESET_DIGITAL_FILTER',
`M4_MAKE_FILTER_RESET_DIGITAL_FILTER_PROTOTYPE($1) {
    assert_not_null(filter);
    
    M4_TAGGED_VECTOR_RESET($1)(filter->previous_input);
    if (filter->previous_output)
        M4_TAGGED_VECTOR_RESET($1)(filter->previous_output);
}')



define(`M4_MAKE_FILTER_FREE_DIGITAL_FILTER_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(filter_free_digital_filter, $1)(M4_DIGITAL_FILTER_TYPE($1) *filter)')

define(`M4_MAKE_FILTER_FREE_DIGITAL_FILTER',
`M4_MAKE_FILTER_FREE_DIGITAL_FILTER_PROTOTYPE($1) {
    assert_not_null(filter);
    
    assert_not_null(filter->feedforward);
    M4_TAGGED_VECTOR_FREE($1)(filter->feedforward);

    assert_not_null(filter->previous_input);
    M4_TAGGED_VECTOR_FREE($1)(filter->previous_input);

    if (filter->feedback != NULL) {
        M4_TAGGED_VECTOR_FREE($1)(filter->feedback);
        assert_not_null(filter->previous_output);
        M4_TAGGED_VECTOR_FREE($1)(filter->previous_output);
    }
    free(filter);
}')

define(`M4_MAKE_FILTER_MAKE_EWMA_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_ewma, $1)(M4_REAL_TYPE($1) alpha)')

define(`M4_MAKE_FILTER_MAKE_EWMA',
`M4_MAKE_FILTER_MAKE_EWMA_PROTOTYPE($1) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    M4_VECTOR_TYPE($1) *feedforward = M4_TAGGED_VECTOR_NEW($1)(1);
    if (feedforward == NULL)
        goto feedforward_allocation_failed;
    *M4_TAGGED_VECTOR_ELEMENT($1)(0, feedforward) = alpha;

    M4_VECTOR_TYPE($1) *feedback = M4_TAGGED_VECTOR_NEW($1)(1);
    if (feedback == NULL)
        goto feedback_allocation_failed;
    *M4_TAGGED_VECTOR_ELEMENT($1)(0, feedback) = 1 - alpha;

    M4_DIGITAL_FILTER_TYPE($1) *filter = 
        M4_TAGGED_FILTER_MAKE_DIGITAL_FILTER($1)(feedforward, feedback);

    M4_TAGGED_VECTOR_FREE($1)(feedforward);
    M4_TAGGED_VECTOR_FREE($1)(feedback);
    
    return filter;

    feedback_allocation_failed:
        M4_TAGGED_VECTOR_FREE($1)(feedforward);
    feedforward_allocation_failed:
        return NULL;
}')

define(`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(`filter_make_first_order_iir', $1)(M4_REAL_TYPE($1) cutoff_frequency)')

define(`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR',
`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR_PROTOTYPE($1) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    M4_REAL_TYPE($1) angular_frequency = ordinary_frequency_to_angular(cutoff_frequency);
    return M4_TAGGED_FUNCTION_NAME(filter_make_ewma, $1)(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}')

define(`M4_MAKE_FILTER_MAKE_SINC_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_sinc, $1)(
    M4_REAL_TYPE($1) cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
)')

define(`M4_MAKE_FILTER_MAKE_SINC',
`M4_MAKE_FILTER_MAKE_SINC_PROTOTYPE($1) {
    assert(cutoff_frequency >= 0);
    assert(filter_type == LOW_PASS || filter_type == HIGH_PASS);
    assert((length & 0x1) == 1);

    if (window == NULL)
        window = window_rectangular;

    M4_VECTOR_TYPE($1) *filter_coefficients = M4_TAGGED_VECTOR_NEW($1)(length);
    if (filter_coefficients == NULL) {
        return NULL;
    }

    M4_REAL_TYPE($1) kernel_shift = ((int) length - 1) / 2.0;
    M4_REAL_TYPE($1) uncorrected_dc_gain = 0;

    for (size_t i = 0; i < length; i++) {
        *M4_TAGGED_VECTOR_ELEMENT($1)(i, filter_coefficients) = 
            sinc(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
        
        uncorrected_dc_gain += *M4_TAGGED_VECTOR_ELEMENT($1)(i, filter_coefficients);
    }

    for (size_t i = 0; i < length; i++) {
        double dc_corrected_coefficient = 
            *M4_TAGGED_VECTOR_ELEMENT($1)(i, filter_coefficients) / uncorrected_dc_gain;
        *M4_TAGGED_VECTOR_ELEMENT($1)(i, filter_coefficients) = 
            filter_type == LOW_PASS ? 
            dc_corrected_coefficient : 
            dirac_delta(i - length / 2) - dc_corrected_coefficient;
    }
    M4_DIGITAL_FILTER_TYPE($1) *filter = 
        M4_TAGGED_FILTER_MAKE_DIGITAL_FILTER($1)(filter_coefficients, NULL);
    
    M4_TAGGED_VECTOR_FREE($1)(filter_coefficients);

    return filter;
}')

divert(0)