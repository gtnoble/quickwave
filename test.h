#include "ext/munit/munit.h"

#define assert_complex_equal(a, b, precision) munit_assert_double_equal(creal(a), creal(b), precision); munit_assert_double_equal(cimag(a), cimag(b), precision);