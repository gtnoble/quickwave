(load-from-path "template.scm")
(load-from-path "vector.scm")

(generate-code
vector-substitutions

"#ifndef QUICKWAVE_BUFFER
#define QUICKWAVE_BUFFER

#include <stdlib.h>
#include <complex.h>
#include \"assertions.h\"
#include <stdbool.h>"

"

/**
 * @brief 
 * ${documentation-number-type} element type circular buffer. Stores the `n_elements` most recently inserted elements.
 * Older elements are overwritten
 */
typedef struct {
    size_t n_elements; /** Number of elements in the buffer */
    size_t last_element_index; /** Index for the last element in the buffer. This is shifted as elements are added. */
    bool is_reversed;
    ${element-type} elements[]; /** Buffer elements */
} ${vector-type};

/**
 * @brief 
 * Circular buffer shift.
 * Inserts an element into the beginning of the buffer and returns the last element
 * @param element Element to be inserted into the beginning of the buffer
 * @param buf Buffer to be operated on
 * @return Last element in the buffer, before being overwritten
 */
${element-type} vector_shift${function-tag}(${element-type} element, ${vector-type} *buf);


/**
 * @brief 
 * Returns element at `last_element_index`. Negative indices represent previously inserted elements.
 * @param last_element_index Index of element to be returned
 * @param buf Vector to be accessed
 * @return Indexed element
 */
${element-type} *vector_element${function-tag}(int index, ${vector-type} *buf);

/**
 * @brief 
 * Interpolates between values of a circular buffer
 * @param last_element_index Generalized last_element_index to interpolate at. Can be between integer indices.
 * @param buf Vector to be accessed
 * @return Interpolated element
 */
${element-type} vector_interpolated_element${function-tag}(${real-number-type} index, const ${vector-type} *buf);

${element-type} vector_dot${function-tag}(const ${vector-type} *a, const ${vector-type} *b);

void vector_scale${function-tag}(${element-type} scalar, ${vector-type} *vector);

void vector_apply${function-tag}(
    ${element-type} (*operation)(${element-type}), 
    ${vector-type} *vector
);

${vector-type} *vector_from_array${function-tag}(size_t size, const ${element-type} elements[]);

/**
 * @brief 
 * Makes and allocates a new vector
 * @param size Number of elements in the circular buffer
 * @return Vector
 */
${vector-type} *vector_new${function-tag}(size_t size);

${vector-type} *vector_duplicate${function-tag}(const ${vector-type} *vector);


size_t vector_length${function-tag}(const ${vector-type} *buf);

void vector_reverse${function-tag}(${vector-type} *vector);

/**
 * @brief 
 * Resets all values of the vector to zero
 * @param buf Vector to reset
 */
void vector_reset${function-tag}(${vector-type} *buf);

/**
 * @brief 
 * Frees vector memory allocations
 * @param buf Vector to be freed
 */
void vector_free${function-tag}(${vector-type} *buf);"

"

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

#endif"
)

