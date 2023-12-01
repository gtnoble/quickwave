
#include "window.h"
#include <math.h>
#include <assert.h>
#include "constants.h"


/**
 * @brief 
 * Evaluates a generalized Hann window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @param a0 Scaling constant
 * @return Window value 
 */
double generalized_hann_function_double(size_t element_index, size_t window_size, double a0);

double window_rectangular_double(size_t element_index, size_t window_size) {
    assert(element_index < window_size);
    return 1;
}

double generalized_hann_function_double(size_t element_index, size_t window_size, double a0) {
    assert(element_index < window_size);
    return a0 - (1 - a0) * cos(2 * M_PI * element_index / window_size);
}

double window_hamming_double(size_t element_index, size_t window_size) {
    return generalized_hann_function(element_index, window_size, 25.0/46.0);
}



/**
 * @brief 
 * Evaluates a generalized Hann window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @param a0 Scaling constant
 * @return Window value 
 */
float generalized_hann_function_float(size_t element_index, size_t window_size, float a0);

float window_rectangular_float(size_t element_index, size_t window_size) {
    assert(element_index < window_size);
    return 1;
}

float generalized_hann_function_float(size_t element_index, size_t window_size, float a0) {
    assert(element_index < window_size);
    return a0 - (1 - a0) * cosf(2 * M_PI * element_index / window_size);
}

float window_hamming_float(size_t element_index, size_t window_size) {
    return generalized_hann_function(element_index, window_size, 25.0/46.0);
}
