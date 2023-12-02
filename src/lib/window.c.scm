(load-from-path "substitutions.scm")

(output-text
"
#include \"window.h\"
#include <math.h>
#include <assert.h>
#include \"constants.h\"

"

    (generate-text
        window-schema
"
/**
 * @brief 
 * Evaluates a generalized Hann window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @param a0 Scaling constant
 * @return Window value 
 */
${number-type} generalized_hann_function${function-tag}(size_t element_index, size_t window_size, ${number-type} a0);

${number-type} window_rectangular${function-tag}(size_t element_index, size_t window_size) {
    assert(element_index < window_size);
    return 1;
}

${number-type} generalized_hann_function${function-tag}(size_t element_index, size_t window_size, ${number-type} a0) {
    assert(element_index < window_size);
    return a0 - (1 - a0) * cos${math-function-suffix}(2 * M_PI * element_index / window_size);
}

${number-type} window_hamming${function-tag}(size_t element_index, size_t window_size) {
    return generalized_hann_function${function-tag}(element_index, window_size, 25.0/46.0);
}
"))