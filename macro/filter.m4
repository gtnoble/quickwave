include(types.m4)dnl
divert(-1)

define(`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER_PROTOTYPE',
`M4_TAGGED_FUNCTION_NAME(filter_evaluate_digital_filter, $1)(
    $1 input, 
    M4_DIGITAL_FILTER_TYPE($1) *filter)')

define(`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_digital_filter, $1)(
    const M4_VECTOR_TYPE($1) *feedforward,
    const M4_VECTOR_TYPE($1) *feedback
)')

define(`M4_MAKE_FILTER_MAKE_EWMA_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_ewma, $1)($1 alpha)')

define(`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_first_order_iir, $1)($1 cutoff_frequency)')

define(`M4_MAKE_FILTER_MAKE_SINC_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_sinc, $1)(
    double cutoff_frequency, 
    size_t length, 
    enum FilterType filter_type,
    WindowFunction window
)')

define(`M4_MAKE_FILTER_MAKE_SAVGOL_PROTOTYPE',
`M4_DIGITAL_FILTER_TYPE($1) *M4_TAGGED_FUNCTION_NAME(filter_make_savgol, $1)(size_t window_length, int deriv, int polyorder)')

define(`M4_MAKE_FILTER_RESET_DIGITAL_FILTER_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(filter_reset_digital_filter, $1)(M4_DIGITAL_FILTER_TYPE($1) *filter)')

define(`M4_MAKE_FILTER_FREE_DIGITAL_FILTER_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(filter_free_digital_filter, $1)(M4_DIGITAL_FILTER_TYPE($1) *filter)')

divert(0)