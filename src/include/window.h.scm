(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_WINDOW
#define QUICKWAVE_WINDOW

#include <stddef.h>

"

    (generate-text
        window-schema
"
typedef ${number-type} (${window-function-type})(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a rectangular window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
${number-type} window_rectangular${function-tag}(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a Hamming window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
${number-type} window_hamming${function-tag}(size_t element_index, size_t window_size);

")

"
#endif
"
)