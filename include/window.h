
#ifndef QUICKWAVE_WINDOW
#define QUICKWAVE_WINDOW

#include <stddef.h>


typedef double (WindowFunctionDouble)(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a rectangular window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
double window_rectangular_double(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a Hamming window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
double window_hamming_double(size_t element_index, size_t window_size);




typedef float (WindowFunctionFloat)(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a rectangular window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
float window_rectangular_float(size_t element_index, size_t window_size);

/**
 * @brief 
 * Evaluates a Hamming window function
 * @param element_index Window element last_element_index
 * @param window_size Size of window
 * @return Window value 
 */
float window_hamming_float(size_t element_index, size_t window_size);


#endif
