
#include <assert.h>
#include <complex.h>

#include "oscillator.h"
#include "vector.h"
#include "constants.h"
#include "phasor.h"


OscillatorDouble oscillator_make_double(double phase_degrees, double normalized_frequency) {
    OscillatorDouble result = {
        .complex_frequency = angular_to_complex_frequency_double(
            ordinary_frequency_to_angular_double(normalized_frequency)
        ),
        .phasor = rotate_phasor_double(zero_phase_phasor, degrees_to_radians_double(phase_degrees))
    };
    return result;
}

double oscillator_inphase_double(OscillatorDouble x) {
    return creal(x.phasor);
}

double oscillator_quadrature_double(OscillatorDouble x) {
    return cimag(x.phasor);
}

double complex oscillator_phase_double(OscillatorDouble osc) {
    return osc.phasor;
}


double oscillator_angular_phase_double(OscillatorDouble x) {
    return carg(x.phasor);
}

double complex oscillator_frequency_double(OscillatorDouble osc) {
    return osc.complex_frequency;
}

double oscillator_angular_freq_double(OscillatorDouble x) {
    return carg(x.complex_frequency);
}

OscillatorDouble nco_shift_phase_double(double angle_radians, OscillatorDouble x) {
    OscillatorDouble result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor_double(x.phasor, angle_radians)
    };
    return result;
}

OscillatorDouble oscillator_predict_double(double delta_samples, OscillatorDouble x) {
    return nco_shift_phase_double(oscillator_angular_freq_double(x) * delta_samples, x);
}

OscillatorDouble oscillator_update_double(double complex update_frequency, OscillatorDouble *nco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabs(update_frequency);

    OscillatorDouble current_nco = *nco;

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

    return current_nco;
}



OscillatorFloat oscillator_make_float(float phase_degrees, float normalized_frequency) {
    OscillatorFloat result = {
        .complex_frequency = angular_to_complex_frequency_float(
            ordinary_frequency_to_angular_float(normalized_frequency)
        ),
        .phasor = rotate_phasor_float(zero_phase_phasor, degrees_to_radians_float(phase_degrees))
    };
    return result;
}

float oscillator_inphase_float(OscillatorFloat x) {
    return crealf(x.phasor);
}

float oscillator_quadrature_float(OscillatorFloat x) {
    return cimagf(x.phasor);
}

float complex oscillator_phase_float(OscillatorFloat osc) {
    return osc.phasor;
}


float oscillator_angular_phase_float(OscillatorFloat x) {
    return cargf(x.phasor);
}

float complex oscillator_frequency_float(OscillatorFloat osc) {
    return osc.complex_frequency;
}

float oscillator_angular_freq_float(OscillatorFloat x) {
    return cargf(x.complex_frequency);
}

OscillatorFloat nco_shift_phase_float(float angle_radians, OscillatorFloat x) {
    OscillatorFloat result = {
        .complex_frequency = x.complex_frequency,
        .phasor = rotate_phasor_float(x.phasor, angle_radians)
    };
    return result;
}

OscillatorFloat oscillator_predict_float(float delta_samples, OscillatorFloat x) {
    return nco_shift_phase_float(oscillator_angular_freq_float(x) * delta_samples, x);
}

OscillatorFloat oscillator_update_float(float complex update_frequency, OscillatorFloat *nco) {
    float complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabsf(update_frequency);

    OscillatorFloat current_nco = *nco;

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

    return current_nco;
}
