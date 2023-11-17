divert(-1)

define(`M4_MAKE_FOR_NUMERIC_TYPES', 
`$1(`double complex')

$1(`float complex')

$1(`double')

$1(`float')')

define(`_M4_JOIN_DECLARATION_COMPONENTS',
``$1'($`'1);')

define(`M4_MAKE_FUNCTION_DECLARATIONS',
`define(`_M4_MAKE_DECLARATION',
`$2'
_M4_JOIN_DECLARATION_COMPONENTS(`$1'))
M4_MAKE_FOR_NUMERIC_TYPES(`_M4_MAKE_DECLARATION')')

divert(0)