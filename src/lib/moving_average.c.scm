
(load-from-path "substitutions.scm")

(output-text
"
#include \"moving_average.h\"
#include \"assertions.h\"
#include \"memory.h\"
"

    (generate-text 
    moving-average-schema
"
${moving-average-type} *moving_average_make${function-tag}(
    size_t length, 
    const MemoryManager *manager
) {
    ${moving-average-type} *filter = manager->allocate(sizeof(${moving-average-type})); 
    if (filter == NULL) { 
        return NULL; 
    } 
    filter->free = manager->deallocate;

    filter->previous_input = vector_new${function-tag}(length, manager); 
    if (filter->previous_input == NULL) { 
        filter->free(filter); 
        return NULL; 
    } 
    
    filter->moving_sum = 0; 
    
    return filter;
}

${number-type} moving_average_evaluate${function-tag}(
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

void moving_average_free${function-tag}(${moving-average-type} *filter) {
    assert_not_null(filter); 
    
    vector_free${function-tag}(filter->previous_input); 
    filter->free(filter); 
}
")
)