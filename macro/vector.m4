include(`types.m4')dnl
divert(-1)

define(`M4_MAKE_VECTOR_SHIFT_PROTOTYPE',
`$1 M4_TAGGED_FUNCTION_NAME(vector_shift, $1)($1 element, M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_ELEMENT_PROTOTYPE',
`$1 *M4_TAGGED_FUNCTION_NAME(vector_element, $1)(int index, M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_INTERPOLATED_ELEMENT_PROTOTYPE',
`$1 M4_TAGGED_FUNCTION_NAME(vector_interpolated_element, $1)(double index, const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_DOT_PROTOTYPE',
`$1 M4_TAGGED_FUNCTION_NAME(vector_dot, $1)(const M4_VECTOR_TYPE($1) *a, const M4_VECTOR_TYPE($1) *b)')

define(`M4_MAKE_VECTOR_SCALE_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_scale, $1)($1 scalar, M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_APPLY_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_apply, $1)($1 (*operation)($1), M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_FROM_ARRAY_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_FUNCTION_NAME(vector_from_array, $1)(size_t size, const $1 elements[])')

define(`M4_MAKE_VECTOR_NEW_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_FUNCTION_NAME(vector_new, $1)(size_t size)')

define(`M4_MAKE_VECTOR_LENGTH_PROTOTYPE',
`size_t M4_TAGGED_FUNCTION_NAME(vector_length, $1)(const M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_REVERSE_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_reverse, $1)(M4_VECTOR_TYPE($1) *vector)')

define(`M4_MAKE_VECTOR_RESET_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_reset, $1)(M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_FREE_PROTOTYPE',
`void M4_TAGGED_FUNCTION_NAME(vector_free, $1)(M4_VECTOR_TYPE($1) *buf)')

define(`M4_MAKE_VECTOR_DUPLICATE_PROTOTYPE',
`M4_VECTOR_TYPE($1) *M4_TAGGED_FUNCTION_NAME(vector_duplicate, $1)(const M4_VECTOR_TYPE($1) *vector)')

divert(0)