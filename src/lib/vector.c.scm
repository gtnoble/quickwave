(load-from-path "template.scm")
(load-from-path "substitutions.scm")

(generate-code
vector-schema
"#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include \"vector.h\""

"

${number-type} vector_element_value${function-tag}(int index, const ${vector-type} *buf);

${number-type} vector_shift${function-tag}(${number-type} element, ${vector-type} *buf) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    ${number-type} last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}

${number-type} *vector_element${function-tag}(int index, ${vector-type} *buf) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}

${number-type} vector_element_value${function-tag}(int index, const ${vector-type} *buf) {
    return *vector_element${function-tag}(index, (${vector-type}*) buf);
}

${number-type} vector_interpolated_element${function-tag}(${number-base-type} index, const ${vector-type} *buf) {
    ${number-base-type} fraction_between_elements = index - floor(index);
    return 
        vector_element_value${function-tag}((int) floor(index), buf) * (1 - fraction_between_elements) + 
        vector_element_value${function-tag}((int) ceil(index), buf) * fraction_between_elements;
}

${number-type} vector_dot${function-tag}(const ${vector-type} *a, const ${vector-type} *b) {
    assert(vector_length${function-tag}(a) == vector_length${function-tag}(b));
    ${number-type} sum = 0;
    for (size_t i = 0; i < vector_length${function-tag}(a); i++) {
        sum += vector_element_value${function-tag}(i, a) * vector_element_value${function-tag}(i, b);
    }
    return sum;
}

void vector_scale${function-tag}(${number-type} scalar, ${vector-type} *vector) {
    for (size_t i = 0; i < vector_length${function-tag}(vector); i++) {
        *vector_element${function-tag}(i, vector) *= scalar;
    }
}

void vector_apply${function-tag}(
    ${number-type} (*operation)(${number-type}), 
    ${vector-type} *vector
) {
    for (size_t i = 0; i < vector_length${function-tag}(vector); i++) {
        *vector_element${function-tag}(i, vector) = 
            operation(*vector_element${function-tag}(i, vector));
    }
}

${vector-type} *vector_new${function-tag}(size_t size) {
    ${vector-type} *circbuf = malloc(
        sizeof(${vector-type}) + sizeof(${number-type}) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    vector_reset${function-tag}(circbuf);
    return circbuf;
}

${vector-type} *vector_from_array${function-tag}(size_t size, const ${number-type} elements[]) {
    ${vector-type} *vector = vector_new${function-tag}(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *vector_element${function-tag}(i, vector) = elements[i];
    }

    return vector;
}

${vector-type} *vector_duplicate${function-tag}(const ${vector-type} *vector) {
    size_t vector_length = vector_length${function-tag}(vector);
    ${vector-type} *new_vector = vector_new${function-tag}(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof(${number-type}) * vector_length
    );
    return new_vector;
}

size_t vector_length${function-tag}(const ${vector-type} *buf) {
    return buf->n_elements;
}

void vector_reverse${function-tag}(${vector-type} *vector) {
    vector->is_reversed = ! vector->is_reversed;
}

void vector_reset${function-tag}(${vector-type} *buf) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}

void vector_free${function-tag}(${vector-type} *buf) {
    free(buf);
}"

"

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
}")
