include(`types.m4')dnl
divert(-1)

define(`M4_VECTOR_TYPE', 
    `Vector`'M4_STRUCT_TYPE_TAG($1)')

M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_ELEMENT_VALUE', `vector_element_value')

define(`M4_MAKE_VECTOR_ELEMENT_VALUE_PROTOTYPE',
`$1 M4_TAGGED_VECTOR_ELEMENT_VALUE($1)(int index, const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_ELEMENT_VALUE_DECLARATION',
`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE($1);')

define(`M4_MAKE_VECTOR_ELEMENT_VALUE',
`M4_MAKE_VECTOR_ELEMENT_VALUE_PROTOTYPE($1) {
    return *M4_TAGGED_VECTOR_ELEMENT($1)(index, (M4_VECTOR_TYPE($1)*) buf);
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_SHIFT', `vector_shift')

define(`M4_MAKE_VECTOR_SHIFT_PROTOTYPE',
`$1 M4_TAGGED_VECTOR_SHIFT($1)($1 element, M4_VECTOR_TYPE($1) *buf)')

define(`make_vector_shift',
`M4_MAKE_VECTOR_SHIFT_PROTOTYPE($1) {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    $1 last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_ELEMENT', `vector_element')

define(`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE',
`$1 *M4_TAGGED_VECTOR_ELEMENT($1)(int index, M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_ELEMENT', 
`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE($1) {
    return &buf->elements[
        modular_add( 
            index * (buf->is_reversed ? -1 : 1), 
            buf->last_element_index + ! buf->is_reversed,
            buf->n_elements)
    ];
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_INTERPOLATED_ELEMENT', `vector_interpolated_element')

define(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT_PROTOTYPE',
`$1 M4_TAGGED_VECTOR_INTERPOLATED_ELEMENT($1)(double index, const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT',
`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT_PROTOTYPE($1) {
    $1 fraction_between_elements = index - floor(index);
    return 
        M4_TAGGED_VECTOR_ELEMENT_VALUE($1)((int) floor(index), buf) * 
            (1 - fraction_between_elements) + 
        M4_TAGGED_VECTOR_ELEMENT_VALUE($1)((int) ceil(index), buf) * 
            fraction_between_elements;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_DOT', `vector_dot')

define(`M4_MAKE_VECTOR_DOT_PROTOTYPE',
`$1 M4_TAGGED_VECTOR_DOT($1)(const M4_VECTOR_TYPE($1) *a, const M4_VECTOR_TYPE($1) *b)')

define(`M4_MAKE_VECTOR_DOT',
`M4_MAKE_VECTOR_DOT_PROTOTYPE($1) {
    assert(M4_TAGGED_VECTOR_LENGTH($1)(a) == M4_TAGGED_VECTOR_LENGTH($1)(b));
    double sum = 0;
    for (size_t i = 0; i < M4_TAGGED_VECTOR_LENGTH($1)(a); i++) {
        sum += 
            M4_TAGGED_VECTOR_ELEMENT_VALUE($1)(i, a) * 
            M4_TAGGED_VECTOR_ELEMENT_VALUE($1)(i, b);
    }
    return sum;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_SCALE', `vector_scale')

define(`M4_MAKE_VECTOR_SCALE_PROTOTYPE',
`void M4_TAGGED_VECTOR_SCALE($1)($1 scalar, M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_SCALE',
`M4_MAKE_VECTOR_SCALE_PROTOTYPE($1) {
    for (size_t i = 0; i < M4_TAGGED_VECTOR_LENGTH($1)(vector); i++) {
        *M4_TAGGED_VECTOR_ELEMENT($1)(i, vector) *= scalar;
    }
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_APPLY', `vector_apply')

define(`M4_MAKE_VECTOR_APPLY_PROTOTYPE',
`void M4_TAGGED_VECTOR_APPLY($1)($1 (*operation)($1), M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_APPLY',
`M4_MAKE_VECTOR_APPLY_PROTOTYPE($1) {
    for (size_t i = 0; i < M4_TAGGED_VECTOR_LENGTH($1)(vector); i++) {
        *M4_TAGGED_VECTOR_ELEMENT($1)(i, vector) = 
            operation(*M4_TAGGED_VECTOR_ELEMENT($1)(i, vector));
    }
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_FROM_ARRAY', `vector_from_array')

define(`M4_MAKE_VECTOR_FROM_ARRAY_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_VECTOR_FROM_ARRAY($1)(size_t size, const $1 elements[])')

define(`M4_MAKE_VECTOR_FROM_ARRAY',
`M4_MAKE_VECTOR_FROM_ARRAY_PROTOTYPE($1) {
    M4_VECTOR_TYPE($1) *vector = M4_TAGGED_VECTOR_NEW($1)(size);
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        *M4_TAGGED_VECTOR_ELEMENT($1)(i, vector) = elements[i];
    }

    return vector;
}')



M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_NEW', `vector_new')

define(`M4_MAKE_VECTOR_NEW_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_VECTOR_NEW($1)(size_t size)')

define(`M4_MAKE_VECTOR_NEW',
`M4_MAKE_VECTOR_NEW_PROTOTYPE($1) {
    M4_VECTOR_TYPE($1) *circbuf = malloc(
        sizeof(M4_VECTOR_TYPE($1)) + sizeof($1) * size);
    if (circbuf == NULL)
        return NULL;

    circbuf->n_elements = size;

    M4_TAGGED_VECTOR_RESET($1)(circbuf);
    return circbuf;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_LENGTH', `vector_length')

define(`M4_MAKE_VECTOR_LENGTH_PROTOTYPE',
`size_t M4_TAGGED_VECTOR_LENGTH($1)(const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_LENGTH',
`M4_MAKE_VECTOR_LENGTH_PROTOTYPE($1) {
    return buf->n_elements;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_REVERSE', `vector_reverse')

define(`M4_MAKE_VECTOR_REVERSE_PROTOTYPE',
`void M4_TAGGED_VECTOR_REVERSE($1)(M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_REVERSE',
`M4_MAKE_VECTOR_REVERSE_PROTOTYPE($1) {
    vector->is_reversed = ! vector->is_reversed;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_RESET', `vector_reset')

define(`M4_MAKE_VECTOR_RESET_PROTOTYPE',
`void M4_TAGGED_VECTOR_RESET($1)(M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_RESET',
`M4_MAKE_VECTOR_RESET_PROTOTYPE($1) {
    if (buf == NULL)
        return;
    for (size_t ii = 0; ii < buf->n_elements; ii++) {
        buf->elements[ii] = 0.0;
    }
    buf->last_element_index = 0;
    buf->is_reversed = false;
}')


M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_FREE', `vector_free')

define(`M4_MAKE_VECTOR_FREE_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_free, $1)(M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_FREE',
`M4_MAKE_VECTOR_FREE_PROTOTYPE($1) {
    free(buf);
}')



M4_MAKE_TAGGED_FUNCTION(`M4_TAGGED_VECTOR_DUPLICATE', `vector_duplicate')

define(`M4_MAKE_VECTOR_DUPLICATE_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_VECTOR_DUPLICATE($1)(const M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_DUPLICATE',
`M4_MAKE_VECTOR_DUPLICATE_PROTOTYPE($1) {
    size_t vector_length = M4_TAGGED_VECTOR_LENGTH($1)(vector);
    M4_VECTOR_TYPE($1) *new_vector = M4_TAGGED_VECTOR_NEW($1)(vector_length);
    if (new_vector == NULL) {
        return NULL;
    }
    memcpy(
        new_vector->elements, 
        vector->elements, 
        sizeof($1) * vector_length
    );
    return new_vector;
}')


divert(0)