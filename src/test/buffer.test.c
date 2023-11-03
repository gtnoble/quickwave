#include "test.h"
#include "buffer.h"

void test_indexing();

int main(int , char **) {
    test_indexing();
    return 0;
}

void test_indexing() {
    CircularBufferComplex *test_buf = circbuf_complex_new(5);
    for (int i = 4; i >= 0; i--) {
       circbuf_complex_shift((double complex) i, test_buf);
    }

    assert_complex_equal(*circbuf_complex_element(0, test_buf), 0.0, 5);
    assert_complex_equal(*circbuf_complex_element(-1, test_buf), 1.0, 5);
    assert_complex_equal(circbuf_complex_interpolated_element(-0.75, test_buf), 0.75, 5);

    assert_complex_equal(*circbuf_complex_element(1, test_buf), 4.0, 5);
    assert_complex_equal(circbuf_complex_interpolated_element(0.75, test_buf), 3.0, 5);
}