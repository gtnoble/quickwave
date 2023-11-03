#ifndef QUICKWAVE_WINDOW
#define QUICKWAVE_WINDOW

#include <stddef.h>

typedef double (WindowFunction)(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a rectangular window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @return Window value 
 */
double window_rectangular(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a Hamming window function
 * @param element_index Window element index
 * @param window_size Size of window
 * @return Window value 
 */
double window_hamming(size_t element_index, size_t window_size);

#endif