divert(-1)

define(`M4_REAL_TYPE',
`ifelse(
    $1, `double complex', `double',
    $1, `float complex', `float',
    $1, `float', `float',
    $1, `double', `double')`)

define(`M4_STRUCT_TYPE_TAG',
    `ifelse(
        $1, `double complex', `ComplexDouble',
        $1, `double', `RealDouble',
        $1, `float complex', `ComplexFloat',
        $1, `float', `RealFloat')')

define(`M4_VECTOR_TYPE', 
    `Vector`'M4_STRUCT_TYPE_TAG($1)')

define(`M4_DIGITAL_FILTER_TYPE', 
    `Filter`'M4_STRUCT_TYPE_TAG($1)')

define(`M4_FUNCTION_TYPE_TAG', 
    `ifelse(
        $1, `double complex', `complex_double',
        $1, `double', `real_double',
        $1, `float complex', `complex_float',
        $1, `float', `real_float')')

define(`M4_TAGGED_FUNCTION_NAME', `$1`'_`'M4_FUNCTION_TYPE_TAG($2)')

define(`M4_MAKE_TAGGED_FUNCTION',
`define(`$1',`M4_TAGGED_FUNCTION_NAME($2, $'`1)')')

divert(0)