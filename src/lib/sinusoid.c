#include <assert.h>
#include <complex.h>

#include "sinusoid.h"
#include "buffer.h"
#include "constants.h"
#include "phasor.h"


Sinusoid sinusoid_make(double phase_degrees, double normalized_frequency) {
    Sinusoid result = {
        .complex_frequency = angular_to_complex_frequency(
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

Sinusoid sinusoid_div(Sinusoid a, Sinusoid b) {
    assert(a.complex_frequency == b.complex_frequency);
    Sinusoid result = {
        .complex_frequency = a.complex_frequency,
        .phasor = a.phasor / b.phasor
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

double sinusoid_angular_freq(Sinusoid x) {
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

Sinusoid sinusoid_predict(double delta_samples, Sinusoid x) {
    return sinusoid_shift_phase(sinusoid_angular_freq(x) * delta_samples, x);
}

Sinusoid sinusoid_scale(double scaling_factor, Sinusoid x) {
    Sinusoid result = {
        .complex_frequency = x.complex_frequency,
        .phasor = x.phasor * scaling_factor
    };
    return result;
}