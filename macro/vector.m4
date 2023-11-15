divert(-1)

define(`macro_function_type_tag', 
    `ifelse(
        $1, `double complex', `complex_double',
        $1, `double', `real_double',
        $1, `float complex', `complex_float',
        $1, `float', `real_float')')

define(`macro_vector_type', 
    `ifelse(
        $1, `double complex', `VectorComplexDouble',
        $1, `double', `VectorRealDouble',
        $1, `float complex', `VectorComplexFloat',
        $1, `float', `VectorRealFloat')')

define(`macro_make_for_numeric_types', 
`$1(`double complex')

$1(`float complex')

$1(`double')

$1(`float')')

define(`macro_tagged_function_name', `$1`'_`'macro_function_type_tag($2)')

define(`macro_make_vector_shift_prototype',
`$1 macro_tagged_function_name(vector_shift, $1)($1 element, macro_vector_type($1) *buf)')

define(`macro_make_vector_element_prototype',
`$1 *macro_tagged_function_name(vector_element, $1)(int index, macro_vector_type($1) *buf)')

define(`macro_make_vector_interpolated_element_prototype',
`$1 macro_tagged_function_name(vector_interpolated_element, $1)(double index, const macro_vector_type($1) *buf)')

define(`macro_make_vector_dot_prototype',
`$1 macro_tagged_function_name(vector_dot, $1)(const macro_vector_type($1) *a, const macro_vector_type($1) *b)')

define(`macro_make_vector_scale_prototype',
`void macro_tagged_function_name(vector_scale, $1)($1 scalar, macro_vector_type($1) *vector)')

define(`macro_make_vector_apply_prototype',
`void macro_tagged_function_name(vector_apply, $1)($1 (*operation)($1), macro_vector_type($1) *vector)')

define(`macro_make_vector_from_array_prototype',
`macro_vector_type($1) *macro_tagged_function_name(vector_from_array, $1)(size_t size, const $1 elements[])')

define(`macro_make_vector_new_prototype',
`macro_vector_type($1) *macro_tagged_function_name(vector_new, $1)(size_t size)')

define(`macro_make_vector_length_prototype',
`size_t macro_tagged_function_name(vector_length, $1)(const macro_vector_type($1) *buf)')

define(`macro_make_vector_reverse_prototype',
`void macro_tagged_function_name(vector_reverse, $1)(macro_vector_type($1) *vector)')

define(`macro_make_vector_reset_prototype',
`void macro_tagged_function_name(vector_reset, $1)(macro_vector_type($1) *buf)')

define(`macro_make_vector_free_prototype',
`void macro_tagged_function_name(vector_free, $1)(macro_vector_type($1) *buf')

define(`macro_make_vector_duplicate_prototype',
`macro_vector_type($1) *macro_tagged_function_name(vector_duplicate, $1)(const macro_vector_type($1) *vector)')

divert dnl