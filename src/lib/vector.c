

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

double complex vector_shift_complex_double(double complex element, VectorComplexDouble *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

float complex vector_shift_complex_float(float complex element, VectorComplexFloat *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    float complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

double vector_shift_real_double(double element, VectorRealDouble *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

float vector_shift_real_float(float element, VectorRealFloat *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    float last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

double complex *vector_element_complex_double(int index, VectorComplexDouble *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

float complex *vector_element_complex_float(int index, VectorComplexFloat *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

double *vector_element_real_double(int index, VectorRealDouble *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

float *vector_element_real_float(int index, VectorRealFloat *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

double complex vector_element_value_complex_double(int index, const VectorComplexDouble *buf) {
    return *vector_element_complex_double(index, (VectorComplexDouble*) buf);
}

float complex vector_element_value_complex_float(int index, const VectorComplexFloat *buf) {
    return *vector_element_complex_float(index, (VectorComplexFloat*) buf);
}

double vector_element_value_real_double(int index, const VectorRealDouble *buf) {
    return *vector_element_real_double(index, (VectorRealDouble*) buf);
}

float vector_element_value_real_float(int index, const VectorRealFloat *buf) {
    return *vector_element_real_float(index, (VectorRealFloat*) buf);
}

double complex vector_interpolated_element_complex_double(double index, const VectorComplexDouble *buf) {
    double complex fraction_between_elements = index - floor(index);
    return 
        vector_element_value_complex_double((int) floor(index), buf) * 
            (1 - fraction_between_elements) + 
        vector_element_value_complex_double((int) ceil(index), buf) * 
            fraction_between_elements;
}

float complex vector_interpolated_element_complex_float(double index, const VectorComplexFloat *buf) {
    float complex fraction_between_elements = index - floor(index);
    return 
        vector_element_value_complex_float((int) floor(index), buf) * 
            (1 - fraction_between_elements) + 
        vector_element_value_complex_float((int) ceil(index), buf) * 
            fraction_between_elements;
}

double vector_interpolated_element_real_double(double index, const VectorRealDouble *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        vector_element_value_real_double((int) floor(index), buf) * 
            (1 - fraction_between_elements) + 
        vector_element_value_real_double((int) ceil(index), buf) * 
            fraction_between_elements;
}

float vector_interpolated_element_real_float(double index, const VectorRealFloat *buf) {
    float fraction_between_elements = index - floor(index);
    return 
        vector_element_value_real_float((int) floor(index), buf) * 
            (1 - fraction_between_elements) + 
        vector_element_value_real_float((int) ceil(index), buf) * 
            fraction_between_elements;
}

double complex vector_dot_complex_double(const VectorComplexDouble *a, const VectorComplexDouble *b) {
    assert(vector_length_complex_double(a) == vector_length_complex_double(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_complex_double(a); i++) {
        sum += 
            vector_element_value_complex_double(i, a) * 
            vector_element_value_complex_double(i, b);
    }
    return sum;
}

float complex vector_dot_complex_float(const VectorComplexFloat *a, const VectorComplexFloat *b) {
    assert(vector_length_complex_float(a) == vector_length_complex_float(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_complex_float(a); i++) {
        sum += 
            vector_element_value_complex_float(i, a) * 
            vector_element_value_complex_float(i, b);
    }
    return sum;
}

double vector_dot_real_double(const VectorRealDouble *a, const VectorRealDouble *b) {
    assert(vector_length_real_double(a) == vector_length_real_double(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_real_double(a); i++) {
        sum += 
            vector_element_value_real_double(i, a) * 
            vector_element_value_real_double(i, b);
    }
    return sum;
}

float vector_dot_real_float(const VectorRealFloat *a, const VectorRealFloat *b) {
    assert(vector_length_real_float(a) == vector_length_real_float(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_real_float(a); i++) {
        sum += 
            vector_element_value_real_float(i, a) * 
            vector_element_value_real_float(i, b);
    }
    return sum;
}

void vector_scale_complex_double(double complex scalar, VectorComplexDouble *vector) {
    for (size_t i = 0; i < vector_length_complex_double(vector); i++) {
        *vector_element_complex_double(i, vector) *= scalar;
    }
}

void vector_scale_complex_float(float complex scalar, VectorComplexFloat *vector) {
    for (size_t i = 0; i < vector_length_complex_float(vector); i++) {
        *vector_element_complex_float(i, vector) *= scalar;
    }
}

void vector_scale_real_double(double scalar, VectorRealDouble *vector) {
    for (size_t i = 0; i < vector_length_real_double(vector); i++) {
        *vector_element_real_double(i, vector) *= scalar;
    }
}

void vector_scale_real_float(float scalar, VectorRealFloat *vector) {
    for (size_t i = 0; i < vector_length_real_float(vector); i++) {
        *vector_element_real_float(i, vector) *= scalar;
    }
}

void vector_apply_complex_double(double complex (*operation)(double complex), const VectorComplexDouble *input, VectorComplexDouble *output) {
    assert(vector_length_complex_double(input) == vector_length_complex_double(output));
    for (size_t i = 0; i < vector_length_complex_double(input); i++) {
        *vector_element_complex_double(i, output) = 
            operation(vector_element_value_complex_double(i, input));
    }
}

void vector_apply_complex_float(float complex (*operation)(float complex), const VectorComplexFloat *input, VectorComplexFloat *output) {
    assert(vector_length_complex_float(input) == vector_length_complex_float(output));
    for (size_t i = 0; i < vector_length_complex_float(input); i++) {
        *vector_element_complex_float(i, output) = 
            operation(vector_element_value_complex_float(i, input));
    }
}

void vector_apply_real_double(double (*operation)(double), const VectorRealDouble *input, VectorRealDouble *output) {
    assert(vector_length_real_double(input) == vector_length_real_double(output));
    for (size_t i = 0; i < vector_length_real_double(input); i++) {
        *vector_element_real_double(i, output) = 
            operation(vector_element_value_real_double(i, input));
    }
}

void vector_apply_real_float(float (*operation)(float), const VectorRealFloat *input, VectorRealFloat *output) {
    assert(vector_length_real_float(input) == vector_length_real_float(output));
    for (size_t i = 0; i < vector_length_real_float(input); i++) {
        *vector_element_real_float(i, output) = 
            operation(vector_element_value_real_float(i, input));
    }
}

VectorComplexDouble *vector_new_complex_double(size_t size) {
    VectorComplexDouble *circbuf = malloc(
        sizeof(VectorComplexDouble) + sizeof(double complex) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_complex_double(circbuf);
    return circbuf;
}

VectorComplexFloat *vector_new_complex_float(size_t size) {
    VectorComplexFloat *circbuf = malloc(
        sizeof(VectorComplexFloat) + sizeof(float complex) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_complex_float(circbuf);
    return circbuf;
}

VectorRealDouble *vector_new_real_double(size_t size) {
    VectorRealDouble *circbuf = malloc(
        sizeof(VectorRealDouble) + sizeof(double) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_real_double(circbuf);
    return circbuf;
}

VectorRealFloat *vector_new_real_float(size_t size) {
    VectorRealFloat *circbuf = malloc(
        sizeof(VectorRealFloat) + sizeof(float) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_real_float(circbuf);
    return circbuf;
}

VectorComplexDouble *vector_from_array_complex_double(size_t size, const double complex elements[]) {
    VectorComplexDouble *vector = vector_new_complex_double(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_complex_double(i, vector) = elements[i];
    }

    return vector;
}

VectorComplexFloat *vector_from_array_complex_float(size_t size, const float complex elements[]) {
    VectorComplexFloat *vector = vector_new_complex_float(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_complex_float(i, vector) = elements[i];
    }

    return vector;
}

VectorRealDouble *vector_from_array_real_double(size_t size, const double elements[]) {
    VectorRealDouble *vector = vector_new_real_double(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_real_double(i, vector) = elements[i];
    }

    return vector;
}

VectorRealFloat *vector_from_array_real_float(size_t size, const float elements[]) {
    VectorRealFloat *vector = vector_new_real_float(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_real_float(i, vector) = elements[i];
    }

    return vector;
}

VectorComplexDouble *vector_duplicate_complex_double(const VectorComplexDouble *vector) {
    size_t vector_length = vector_length_complex_double(vector);
    VectorComplexDouble *new_vector = vector_new_complex_double(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(double complex) * vector_length
    );
    return new_vector;
}

VectorComplexFloat *vector_duplicate_complex_float(const VectorComplexFloat *vector) {
    size_t vector_length = vector_length_complex_float(vector);
    VectorComplexFloat *new_vector = vector_new_complex_float(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(float complex) * vector_length
    );
    return new_vector;
}

VectorRealDouble *vector_duplicate_real_double(const VectorRealDouble *vector) {
    size_t vector_length = vector_length_real_double(vector);
    VectorRealDouble *new_vector = vector_new_real_double(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(double) * vector_length
    );
    return new_vector;
}

VectorRealFloat *vector_duplicate_real_float(const VectorRealFloat *vector) {
    size_t vector_length = vector_length_real_float(vector);
    VectorRealFloat *new_vector = vector_new_real_float(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(float) * vector_length
    );
    return new_vector;
}

size_t vector_length_complex_double(const VectorComplexDouble *buf) {
    return buf->n_elements;
}

size_t vector_length_complex_float(const VectorComplexFloat *buf) {
    return buf->n_elements;
}

size_t vector_length_real_double(const VectorRealDouble *buf) {
    return buf->n_elements;
}

size_t vector_length_real_float(const VectorRealFloat *buf) {
    return buf->n_elements;
}

void vector_reverse_complex_double(VectorComplexDouble *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_reverse_complex_float(VectorComplexFloat *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_reverse_real_double(VectorRealDouble *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_reverse_real_float(VectorRealFloat *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_reset_complex_double(VectorComplexDouble *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_reset_complex_float(VectorComplexFloat *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_reset_real_double(VectorRealDouble *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_reset_real_float(VectorRealFloat *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_free_complex_double(VectorComplexDouble *buf) {
    free(buf);
}

void vector_free_complex_float(VectorComplexFloat *buf) {
    free(buf);
}

void vector_free_real_double(VectorRealDouble *buf) {
    free(buf);
}

void vector_free_real_float(VectorRealFloat *buf) {
    free(buf);
}

int modular_add(int a, int b, int max) {
    int sum = a + b;
    return modulo_euclidean(sum, max);
}

int modulo_euclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}