#include "test.h"
#include "vector.h"

void test_indexing();

int main() {
    test_indexing();
    return 0;
}

void test_indexing() {
    VectorComplex *test_buf = vector_complex_new(5);
    for (int i = 0; i <= 4; i++) {
       vector_complex_shift((double complex) i, test_buf);
    }

    assert_complex_equal(*vector_complex_element(0, test_buf), 0.0, 5);
    assert_complex_equal(*vector_complex_element(-1, test_buf), 4.0, 5);
    assert_complex_equal(vector_complex_interpolated_element(-0.5, test_buf), 2, 5);

    assert_complex_equal(*vector_complex_element(1, test_buf), 1.0, 5);
    assert_complex_equal(vector_complex_interpolated_element(0.75, test_buf), 0.75, 5);

    vector_complex_reverse(test_buf);

    assert_complex_equal(*vector_complex_element(0.0, test_buf), 4.0, 5);
    assert_complex_equal(*vector_complex_element(1.0, test_buf), 3.0, 5);

    assert_complex_equal(vector_complex_dot(test_buf, test_buf), 30.0, 5);
}