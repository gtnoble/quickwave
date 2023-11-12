#include <assert.h>
#include <complex.h>

#include "oscillator.h"
#include "vector.h"
#include "constants.h"
#include "phasor.h"


Oscillator oscillator_make(double phase_degrees, double normalized_frequency) {
    Oscillator result = {
        .complex_frequency = angular_to_complex_frequency(
            ordinary_frequency_to_angular(normalized_frequency)
        ),
        .phasor = rotate_phasor(zero_phase_phasor, degrees_to_radians(phase_degrees))
    };
    return result;
}

double oscillator_inphase(Oscillator x) {
    return creal(x.phasor);
}

double oscillator_quadrature(Oscillator x) {
    return cimag(x.phasor);
}

double complex oscillator_phase(Oscillator osc) {
    return osc.phasor;
}


double oscillator_angular_phase(Oscillator x) {
    return carg(x.phasor);
}

double complex oscillator_frequency(Oscillator osc) {
    return osc.complex_frequency;
}

double oscillator_angular_freq(Oscillator x) {
    return carg(x.complex_frequency);
}

Oscillator nco_shift_phase(double angle_radians, Oscillator x) {
    Oscillator result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor(x.phasor, angle_radians)
    };
    return result;
}

Oscillator oscillator_predict(double delta_samples, Oscillator x) {
    return nco_shift_phase(oscillator_angular_freq(x) * delta_samples, x);
}

Oscillator oscillator_update(double complex update_frequency, Oscillator *nco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabs(update_frequency);

    Oscillator current_nco = *nco;

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

    return current_nco;
}