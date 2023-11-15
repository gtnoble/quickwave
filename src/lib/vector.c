 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "vector.h"

double vector_real_element_value(int index, const VectorRealDouble *buf);
double complex vector_complex_element_value(int index, const VectorComplexDouble *buf);

#define vector_element_value_generic(index, buf) \
    _Generic((buf), \
        const VectorReal*: vector_real_element_value, \
        const VectorComplex*: vector_complex_element_value \
    )(index, buf)




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
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

double vector_shift_real_double(double element, VectorRealDouble *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

float vector_shift_real_float(float element, VectorRealFloat *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    double complex last_element = buf->elements[buf->last_element_index];
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
        vector_element_value_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        vector_element_value_generic((int) ceil(index), buf) * fraction_between_elements;
}

float complex vector_interpolated_element_complex_float(double index, const VectorComplexFloat *buf) {
    float complex fraction_between_elements = index - floor(index);
    return 
        vector_element_value_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        vector_element_value_generic((int) ceil(index), buf) * fraction_between_elements;
}

double vector_interpolated_element_real_double(double index, const VectorRealDouble *buf) {
    double fraction_between_elements = index - floor(index);
    return 
        vector_element_value_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        vector_element_value_generic((int) ceil(index), buf) * fraction_between_elements;
}

float vector_interpolated_element_real_float(double index, const VectorRealFloat *buf) {
    float fraction_between_elements = index - floor(index);
    return 
        vector_element_value_generic((int) floor(index), buf) * (1 - fraction_between_elements) + 
        vector_element_value_generic((int) ceil(index), buf) * fraction_between_elements;
}

double vector_real_dot(const VectorRealDouble *a, const VectorRealDouble *b) {
    assert(vector_length_generic(a) == vector_length_generic(b));
    double sum = 0;
    for (size_t i = 0; i < vector_length_generic(a); i++) {
        sum += vector_element_value_generic(i, a) * vector_element_value_generic(i, b);
    }
    return sum;
}

double complex vector_complex_dot(const VectorComplexDouble *a, const VectorComplexDouble *b) {
    assert(vector_length_generic(a) == vector_length_generic(b));
    double complex sum = 0;
    for (size_t i = 0; i < vector_length_generic(a); i++) {
        sum += vector_element_value_generic(i, a) * vector_element_value_generic(i, b);
    }
    return sum;
}

void vector_real_scale(double scalar, VectorRealDouble *vector) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *vector_element_generic(i, vector) *= scalar;
    }
}

void vector_complex_scale(double complex scalar, VectorComplexDouble *vector) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *vector_element_generic(i, vector) *= scalar;
    }
}

void vector_real_apply(double (*operation)(double), VectorRealDouble *vector) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *vector_element_generic(i, vector) = 
            operation(*vector_element_generic(i, vector));
    }
}

void vector_complex_apply(
    double complex (*operation)(double complex), 
    VectorComplexDouble *vector
) {
    for (size_t i = 0; i < vector_length_generic(vector); i++) {
        *vector_element_generic(i, vector) = 
            operation(*vector_element_generic(i, vector));
    }
}


VectorComplexDouble *vector_complex_new(size_t size) {
    VectorComplexDouble *circbuf = malloc(
        sizeof(VectorComplex) + sizeof(double complex) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_generic(circbuf);
    return circbuf;
}

VectorRealDouble *vector_real_new(size_t size) {
    VectorRealDouble *circbuf = malloc(
        sizeof(VectorReal) + sizeof(double) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset_generic(circbuf);
    return circbuf;
}

VectorComplexDouble *vector_complex_from_array(size_t size, const double complex elements[]) {
    VectorComplexDouble *vector = vector_complex_new(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_generic(i, vector) = elements[i];
    }

    return vector;
}

VectorRealDouble *vector_real_from_array(size_t size, const double elements[]) {
    VectorRealDouble *vector = vector_real_new(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element_generic(i, vector) = elements[i];
    }

    return vector;
}

VectorComplexDouble *vector_complex_duplicate(const VectorComplexDouble *vector) {
    size_t vector_length = vector_length_generic(vector);
    VectorComplexDouble *new_vector = vector_complex_new(vector_length);
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

VectorRealDouble *vector_real_duplicate(const VectorRealDouble *vector) {
    size_t vector_length = vector_length_generic(vector);
    VectorRealDouble *new_vector = vector_real_new(vector_length);
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


size_t vector_complex_length(const VectorComplexDouble *buf) {
    return buf->n_elements;
}

size_t vector_real_length(const VectorRealDouble *buf) {
    return buf->n_elements;
}

void vector_complex_reverse(VectorComplexDouble *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_real_reverse(VectorRealDouble *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_complex_reset(VectorComplexDouble *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_real_reset(VectorRealDouble *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_complex_free(VectorComplexDouble *buf) {
    free(buf);
}

void vector_real_free(VectorRealDouble *buf) {
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