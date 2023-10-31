#include "window.h"
#include <math.h>
#include <assert.h>
#include "constants.h"

/**
 * @brief 
 * Evaluates a rectangular window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @return Window value 
 */
double window_rectangular(size_t element_index, size_t window_size) {
    assert(element_index < window_size);
    return 1;
}

/**
 * @brief 
 * Evaluates a generalized Hann window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @param a0 Scaling constant
 * @return Window value 
 */
double generalized_hann_function(size_t element_index, size_t window_size, double a0) {
    assert(element_index < window_size);
    return a0 - (1 - a0) * cos(2 * M_PI * element_index / window_size);
}

/**
 * @brief 
 * Evaluates a Hamming window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @return Window value 
 */
double window_hamming(size_t element_index, size_t window_size) {
    return generalized_hann_function(element_index, window_size, 25.0/46.0);
}