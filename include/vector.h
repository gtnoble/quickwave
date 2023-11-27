#ifndef QUICKWAVE_BUFFER
#define QUICKWAVE_BUFFER

#include <stdlib.h>
#include <complex.h>
#include "assertions.h"
#include <stdbool.h>
/**
 * @brief 
 * ${documentation-number-type} element type circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    double complex elements[]; /** Buffer elements */
} VectorComplexDouble;

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double complex vector_shift_complex_double(double complex element, VectorComplexDouble *buf);


/**
 * @brief 
 * Returns element at `last_element_index`. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Vector to be accessed
 * @return Indexed element
 */
double complex *vector_element_complex_double(int index, VectorComplexDouble *buf);

/**
 * @brief 
 * Interpolates between values of a circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Vector to be accessed
 * @return Interpolated element
 */
double complex vector_interpolated_element_complex_double(double index, const VectorComplexDouble *buf);

double complex vector_dot_complex_double(const VectorComplexDouble *a, const VectorComplexDouble *b);

void vector_scale_complex_double(double complex scalar, VectorComplexDouble *vector);

void vector_apply_complex_double(
    double complex (*operation)(double complex), 
    VectorComplexDouble *vector
);

VectorComplexDouble *vector_from_array_complex_double(size_t size, const double complex elements[]);

/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the circular buffer
 * @return Vector
 */
VectorComplexDouble *vector_new_complex_double(size_t size);

VectorComplexDouble *vector_duplicate_complex_double(const VectorComplexDouble *vector);


size_t vector_length_complex_double(const VectorComplexDouble *buf);

void vector_reverse_complex_double(VectorComplexDouble *vector);

/**
 * @brief 
 * Resets all values of the vector to zero
 * @param buf Vector to reset
 */
void vector_reset_complex_double(VectorComplexDouble *buf);

/**
 * @brief 
 * Frees vector memory allocations
 * @param buf Vector to be freed
 */
void vector_free_complex_double(VectorComplexDouble *buf);


/**
 * @brief 
 * ${documentation-number-type} element type circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    float complex elements[]; /** Buffer elements */
} VectorComplexFloat;

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
float complex vector_shift_complex_float(float complex element, VectorComplexFloat *buf);


/**
 * @brief 
 * Returns element at `last_element_index`. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Vector to be accessed
 * @return Indexed element
 */
float complex *vector_element_complex_float(int index, VectorComplexFloat *buf);

/**
 * @brief 
 * Interpolates between values of a circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Vector to be accessed
 * @return Interpolated element
 */
float complex vector_interpolated_element_complex_float(float index, const VectorComplexFloat *buf);

float complex vector_dot_complex_float(const VectorComplexFloat *a, const VectorComplexFloat *b);

void vector_scale_complex_float(float complex scalar, VectorComplexFloat *vector);

void vector_apply_complex_float(
    float complex (*operation)(float complex), 
    VectorComplexFloat *vector
);

VectorComplexFloat *vector_from_array_complex_float(size_t size, const float complex elements[]);

/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the circular buffer
 * @return Vector
 */
VectorComplexFloat *vector_new_complex_float(size_t size);

VectorComplexFloat *vector_duplicate_complex_float(const VectorComplexFloat *vector);


size_t vector_length_complex_float(const VectorComplexFloat *buf);

void vector_reverse_complex_float(VectorComplexFloat *vector);

/**
 * @brief 
 * Resets all values of the vector to zero
 * @param buf Vector to reset
 */
void vector_reset_complex_float(VectorComplexFloat *buf);

/**
 * @brief 
 * Frees vector memory allocations
 * @param buf Vector to be freed
 */
void vector_free_complex_float(VectorComplexFloat *buf);


/**
 * @brief 
 * ${documentation-number-type} element type circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    double elements[]; /** Buffer elements */
} VectorRealDouble;

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
double vector_shift_real_double(double element, VectorRealDouble *buf);


/**
 * @brief 
 * Returns element at `last_element_index`. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Vector to be accessed
 * @return Indexed element
 */
double *vector_element_real_double(int index, VectorRealDouble *buf);

/**
 * @brief 
 * Interpolates between values of a circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Vector to be accessed
 * @return Interpolated element
 */
double vector_interpolated_element_real_double(double index, const VectorRealDouble *buf);

double vector_dot_real_double(const VectorRealDouble *a, const VectorRealDouble *b);

void vector_scale_real_double(double scalar, VectorRealDouble *vector);

void vector_apply_real_double(
    double (*operation)(double), 
    VectorRealDouble *vector
);

VectorRealDouble *vector_from_array_real_double(size_t size, const double elements[]);

/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the circular buffer
 * @return Vector
 */
VectorRealDouble *vector_new_real_double(size_t size);

VectorRealDouble *vector_duplicate_real_double(const VectorRealDouble *vector);


size_t vector_length_real_double(const VectorRealDouble *buf);

void vector_reverse_real_double(VectorRealDouble *vector);

/**
 * @brief 
 * Resets all values of the vector to zero
 * @param buf Vector to reset
 */
void vector_reset_real_double(VectorRealDouble *buf);

/**
 * @brief 
 * Frees vector memory allocations
 * @param buf Vector to be freed
 */
void vector_free_real_double(VectorRealDouble *buf);


/**
 * @brief 
 * ${documentation-number-type} element type circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    float elements[]; /** Buffer elements */
} VectorRealFloat;

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
float vector_shift_real_float(float element, VectorRealFloat *buf);


/**
 * @brief 
 * Returns element at `last_element_index`. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Vector to be accessed
 * @return Indexed element
 */
float *vector_element_real_float(int index, VectorRealFloat *buf);

/**
 * @brief 
 * Interpolates between values of a circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Vector to be accessed
 * @return Interpolated element
 */
float vector_interpolated_element_real_float(float index, const VectorRealFloat *buf);

float vector_dot_real_float(const VectorRealFloat *a, const VectorRealFloat *b);

void vector_scale_real_float(float scalar, VectorRealFloat *vector);

void vector_apply_real_float(
    float (*operation)(float), 
    VectorRealFloat *vector
);

VectorRealFloat *vector_from_array_real_float(size_t size, const float elements[]);

/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the circular buffer
 * @return Vector
 */
VectorRealFloat *vector_new_real_float(size_t size);

VectorRealFloat *vector_duplicate_real_float(const VectorRealFloat *vector);


size_t vector_length_real_float(const VectorRealFloat *buf);

void vector_reverse_real_float(VectorRealFloat *vector);

/**
 * @brief 
 * Resets all values of the vector to zero
 * @param buf Vector to reset
 */
void vector_reset_real_float(VectorRealFloat *buf);

/**
 * @brief 
 * Frees vector memory allocations
 * @param buf Vector to be freed
 */
void vector_free_real_float(VectorRealFloat *buf);

/**
 * @brief 
 * Performs Euclidean modular addition
 * @param a Summand
 * @param b Summand
 * @param max Divisor
 * @return Result
 */
int modular_add(int a, int b, int max);

/**
 * @brief 
 * Performs Euclidean modulus
 * @param a Dividend
 * @param b Divisor
 * @return Remainder
 */
int modulo_euclidean(int a, int b);

#endif