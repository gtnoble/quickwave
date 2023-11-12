#include "fft.h"
#include "test.h"
#include "vector.h"

const double complex test_points[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
size_t num_test_points = 8;


int main() {

    VectorComplex *test_vector = 
        vector_complex_from_array(num_test_points, test_points);
    
    FftComplex *fft = fft_make_fft_complex(num_test_points);

    fft_fft(test_vector, fft);
    fft_ifft(test_vector, fft);

    for (size_t i = 0; i < num_test_points; i++) {
        assert_complex_equal(*vector_element_generic(i, test_vector), test_points[i], 3);
    }
    
}