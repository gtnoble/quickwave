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

divert(0)