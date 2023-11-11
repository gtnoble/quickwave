#include <assert.h>
#include <complex.h>

#include "sinusoid.h"
#include "vector.h"
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
    Sinusoid result = {
        .complex_frequency = a.complex_frequency + b.complex_frequency,
        .phasor = a.phasor + b.phasor
    };
    return result;
}

Sinusoid sinusoid_mult(Sinusoid a, Sinusoid b) {
    Sinusoid result = {
        .complex_frequency = a.complex_frequency * b.complex_frequency,
        .phasor = a.phasor * b.phasor
    };
    return result;
}

Sinusoid sinusoid_div(Sinusoid a, Sinusoid b) {
    Sinusoid result = {
        .complex_frequency = a.complex_frequency / b.complex_frequency,
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
        .complex_frequency = conj(x.complex_frequency),
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

Sinusoid nco_update(double complex update_frequency, Sinusoid nco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        nco.complex_frequency : 
        update_frequency / cabs(update_frequency);

    Sinusoid updated_nco = {
        .complex_frequency = next_frequency,
        .phasor = nco.phasor * next_frequency
    };
    return updated_nco;
}

Sinusoid quadrature_mix(Sinusoid reference, double complex input) {

    Sinusoid input_sinusoid = {
        .complex_frequency = reference.complex_frequency,
        .phasor = 
            input * reference.phasor
    };
    return input_sinusoid;
}
