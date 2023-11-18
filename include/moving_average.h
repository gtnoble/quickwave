

#ifndef QUICKWAVE_MOVING_AVERAGE
#define QUICKWAVE_MOVING_AVERAGE

#include <complex.h>
#include "vector.h"



/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double complex moving_sum;
    VectorComplexDouble *previous_input;
} MovingAverageComplexDouble;

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    float complex moving_sum;
    VectorComplexFloat *previous_input;
} MovingAverageComplexFloat;

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    double moving_sum;
    VectorRealDouble *previous_input;
} MovingAverageRealDouble;

/**
 * @brief 
 * Simple moving average.
 * While the simple moving average is a linear filter,
 * this is a more optimized O(1) implementation than a naive linear filter.
 */
typedef struct {
    float moving_sum;
    VectorRealFloat *previous_input;
} MovingAverageRealFloat;


/**
 * @brief 
 * Makes and allocates a moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageComplexDouble *moving_average_make_complex_double(size_t length);

/**
 * @brief 
 * Makes and allocates a moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageComplexFloat *moving_average_make_complex_float(size_t length);

/**
 * @brief 
 * Makes and allocates a moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageRealDouble *moving_average_make_real_double(size_t length);

/**
 * @brief 
 * Makes and allocates a moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverageRealFloat *moving_average_make_real_float(size_t length);




size_t moving_average_length_complex_double(MovingAverageComplexDouble *filter);


size_t moving_average_length_complex_float(MovingAverageComplexFloat *filter);


size_t moving_average_length_real_double(MovingAverageRealDouble *filter);


size_t moving_average_length_real_float(MovingAverageRealFloat *filter);


/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double complex moving_average_evaluate_complex_double(
    double complex input, 
    MovingAverageComplexDouble *filter
);

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
float complex moving_average_evaluate_complex_float(
    float complex input, 
    MovingAverageComplexFloat *filter
);

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double moving_average_evaluate_real_double(
    double input, 
    MovingAverageRealDouble *filter
);

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
float moving_average_evaluate_real_float(
    float input, 
    MovingAverageRealFloat *filter
);



/**
 * @brief 
 * Resets a moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset_complex_double(MovingAverageComplexDouble *filter);

/**
 * @brief 
 * Resets a moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset_complex_float(MovingAverageComplexFloat *filter);

/**
 * @brief 
 * Resets a moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset_real_double(MovingAverageRealDouble *filter);

/**
 * @brief 
 * Resets a moving average filter to initial state
 * @param filter Filter to reset
 */
void moving_average_reset_real_float(MovingAverageRealFloat *filter);



/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free_complex_double(MovingAverageComplexDouble *filter);

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free_complex_float(MovingAverageComplexFloat *filter);

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free_real_double(MovingAverageRealDouble *filter);

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void moving_average_free_real_float(MovingAverageRealFloat *filter);

#endif
