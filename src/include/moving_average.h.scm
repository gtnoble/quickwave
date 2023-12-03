(load-from-path "substitutions.scm")

(output-text

"
#ifndef QUICKWAVE_MOVING_AVERAGE
#define QUICKWAVE_MOVING_AVERAGE

#include <complex.h>
#include \"vector.h\"
#include \"memory.h\"
"

(generate-text 
    moving-average-schema
"
/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    ${number-type} moving_sum;
    ${vector-type} *previous_input;
    Deallocator *free;
} ${moving-average-type};

/**
 * @brief 
 * Makes and allocates a simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
${moving-average-type} *moving_average_make${function-tag}(
    size_t length, 
    const MemoryManager *manager
);

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
${number-type} moving_average_evaluate${function-tag}(
    ${number-type} input, 
    ${moving-average-type} *filter
);

/**
 * @brief 
 * Resets moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset${function-tag}(${moving-average-type} *filter);

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free${function-tag}(${moving-average-type} *filter);

")

"
#endif
")