include(`types.m4')dnl
include(`vector.m4')dnl
divert(-1)

define(`M4_MOVING_AVERAGE_TYPE',
`MovingAverage`'M4_STRUCT_TYPE_TAG($1)')


define(`M4_MAKE_MOVING_AVERAGE_MAKE_PROTOTYPE',
`M4_MOVING_AVERAGE_TYPE($1) *M4_TAGGED_FUNCTION_NAME(moving_average_make, $1)(size_t length)')

define(`M4_MAKE_MOVING_AVERAGE_MAKE',
`M4_MAKE_MOVING_AVERAGE_MAKE_PROTOTYPE($1) {
    M4_MOVING_AVERAGE_TYPE($1) *filter = malloc(sizeof(M4_MOVING_AVERAGE_TYPE($1)));
    
    if (filter == NULL) {
        return NULL;
    }
    
    filter->previous_input = M4_TAGGED_VECTOR_NEW($1)(length);
    if (filter->previous_input == NULL) {
        free(filter);
        return NULL;
    }
    
    filter->moving_sum = 0;
    
    return filter;
}')


define(`M4_MAKE_MOVING_AVERAGE_LENGTH_PROTOTYPE',
`size_t M4_TAGGED_FUNCTION_NAME(moving_average_length, $1)(M4_MOVING_AVERAGE_TYPE($1) *filter)')

define(`M4_MAKE_MOVING_AVERAGE_LENGTH',
`M4_MAKE_MOVING_AVERAGE_LENGTH_PROTOTYPE($1) {
    return M4_TAGGED_VECTOR_LENGTH($1)(filter->previous_input);
}')


define(`M4_MAKE_MOVING_AVERAGE_EVALUATE_PROTOTYPE',
`$1 M4_TAGGED_FUNCTION_NAME(moving_average_evaluate, $1)(
    $1 input, 
    M4_MOVING_AVERAGE_TYPE($1) *filter
)')

define(`M4_MAKE_MOVING_AVERAGE_EVALUATE',
`M4_MAKE_MOVING_AVERAGE_EVALUATE_PROTOTYPE($1) {
    assert_not_null(filter);
    
    filter->moving_sum += input;
    filter->moving_sum -= M4_TAGGED_VECTOR_SHIFT($1)(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements; 
}')


define(`M4_MAKE_MOVING_AVERAGE_RESET_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(moving_average_reset, $1)(M4_MOVING_AVERAGE_TYPE($1) *filter)')

define(`M4_MAKE_MOVING_AVERAGE_RESET',
`M4_MAKE_MOVING_AVERAGE_RESET_PROTOTYPE($1) {
    assert_not_null(filter);
    
    M4_TAGGED_VECTOR_RESET($1)(filter->previous_input);
    filter->moving_sum = 0; 
}')


define(`M4_MAKE_MOVING_AVERAGE_FREE_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(moving_average_free, $1)(M4_MOVING_AVERAGE_TYPE($1) *filter)')

define(`M4_MAKE_MOVING_AVERAGE_FREE',
` M4_MAKE_MOVING_AVERAGE_FREE_PROTOTYPE($1) {
    assert_not_null(filter);
    
    M4_TAGGED_VECTOR_FREE($1)(filter->previous_input);
    free(filter); 
}')

divert(0)