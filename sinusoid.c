#include <assert.h>
#include <complex.h>

#include "sinusoid.h"
#include "buffer.h"
#include "constants.h"

const double complex zero_complex_frequency = 1.0;
const double complex zero_phase_phasor = 1.0;

Sinusoid sinusoid_make(double phase_degrees, double normalized_frequency) {
    Sinusoid result = {
        .complex_frequency = real_to_complex_frequency(
            ordinary_frequency_to_angular(normalized_frequency)
        ),
        .phasor = rotate_phasor(zero_phase_phasor, degrees_to_radians(phase_degrees))
    };
    return result;
}

Sinusoid sinusoid_add(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor + b.phasor
    };
    return result;
}

Sinusoid sinusoid_mult(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor * b.phasor
    };
    return result;
}

double sinusoid_inphase(Sinusoid x) {
    return creal(x.phasor);
}

double sinusoid_quadrature(Sinusoid x) {
    return cimag(x.phasor);
}

double sinusoid_amplitude(Sinusoid x) {
    return cabs(x.phasor);
}

double sinusoid_phase(Sinusoid x) {
    return carg(x.phasor);
}

double sinusoid_real_freq(Sinusoid x) {
    return carg(x.complex_frequency);
}

Sinusoid sinusoid_normalize(Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = x.phasor / cabs(x.phasor)
    };
    return result;
}

Sinusoid sinusoid_negate_phase(Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = conj(x.phasor)
    };
    return result;
}

Sinusoid sinusoid_shift_phase(double angle_radians, Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor(x.phasor, angle_radians)
    };
    return result;
}

double complex rotate_phasor(double complex phasor, double angle_radians) {
    return phasor * cexp(I * angle_radians);
}

double _Complex real_to_complex_frequency(double angular_frequency) {
    return zero_complex_frequency * cexp(I * angular_frequency);
}

Sinusoid update_vco(double _Complex complex_freq, Sinusoid vco) {
    Sinusoid updated_vco = {
        .complex_frequency = complex_freq,
        .phasor = vco.phasor * complex_freq
    };
    return updated_vco;
}

Sinusoid quadrature_demodulate(Sinusoid reference, CircularBuffer *lagged_input) {
    double in_phase_element_index;
    double reference_normal_frequency = angular_frequency_to_ordinary(
        sinusoid_real_freq(reference)
    );
    if (reference_normal_frequency < 0)
        in_phase_element_index = 0;
    else {
        int last_element_index = -(lagged_input->n_elements) + 1;
        double quadrature_lag = -(1 / reference_normal_frequency);
        in_phase_element_index = 
            quadrature_lag > last_element_index ? quadrature_lag : last_element_index;
    }

    int quadrature_element_index = 0;
    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            circbuf_interpolated_element(in_phase_element_index, lagged_input) +
            *circbuf_element(quadrature_element_index, lagged_input) * I
    };
    return sinusoid_mult(reference, input_sinusoid);
}