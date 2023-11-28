
(load-from-path "substitutions.scm")

(output-text
"
#include \"moving_average.h\"
#include \"assertions.h\"
"

    (generate-text 
    moving-average-schema
"
${moving-average-type} *moving_average_complex_make${function-tag}(size_t length) {
    ${moving-average-type} *filter = malloc(sizeof(${moving-average-type})); 
    
    if (filter == NULL) { 
        return NULL; 
    } 
    
    filter->previous_input = vector_new${function-tag}(length); 
    if (filter->previous_input == NULL) { 
        free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

double complex moving_average_complex_evaluate${function-tag}(
    ${number-type} input, 
    ${moving-average-type} *filter
) {
    assert_not_null(filter); 
    
    filter->moving_sum += input; 
    filter->moving_sum -= vector_shift${function-tag}(input, filter->previous_input); 
    return filter->moving_sum / vector_length${function-tag}(filter->previous_input); 
}

void moving_average_complex_reset${function-tag}(${moving-average-type} *filter) {
    assert_not_null(filter); 
    
    vector_reset${function-tag}(filter->previous_input); 
    filter->moving_sum = 0; 
}

void moving_average_complex_free${function-tag}(${moving-average-type} *filter) {
    assert_not_null(filter); 
    
    vector_free${function-tag}(filter->previous_input); 
    free(filter); 
}
")
)