
#include <assert.h>
#include <complex.h>

#include "oscillator.h"
#include "vector.h"
#include "constants.h"
#include "phasor.h"


OscillatorDouble *oscillator_make_double(
    double phase_degrees, 
    double normalized_frequency,
    const MemoryManager *manager
) {
    OscillatorDouble *oscillator = manager->allocate(sizeof(OscillatorDouble));
    if (oscillator == NULL) {
        return NULL;
    }

    oscillator->complex_frequency = angular_to_complex_frequency_double(
        ordinary_frequency_to_angular_double(normalized_frequency)
    );
    oscillator->phasor = rotate_phasor_double(
        zero_phase_phasor_double, 
        degrees_to_radians_double(phase_degrees)
    );

    return oscillator;
}

void oscillator_free_double(OscillatorDouble *oscillator) {
    oscillator->free(oscillator);
}

double oscillator_inphase_double(const OscillatorDouble *x) {
    return creal(x->phasor);
}

double oscillator_quadrature_double(const OscillatorDouble *x) {
    return cimag(x->phasor);
}

double complex oscillator_phase_double(const OscillatorDouble *osc) {
    return osc->phasor;
}


double oscillator_angular_phase_double(const OscillatorDouble *x) {
    return carg(x->phasor);
}

double complex oscillator_frequency_double(const OscillatorDouble *osc) {
    return osc->complex_frequency;
}

double oscillator_angular_freq_double(const OscillatorDouble *x) {
    return carg(x->complex_frequency);
}

void oscillator_shift_phase_double(
    double angle_radians, 
    OscillatorDouble *shifted_output, 
    const OscillatorDouble *x
) {
    shifted_output->complex_frequency = x->complex_frequency;
    shifted_output->phasor = rotate_phasor_double(x->phasor, angle_radians);
}

void oscillator_predict_double(
    double delta_samples, 
    OscillatorDouble *prediction, 
    const OscillatorDouble *x
) {
    oscillator_shift_phase_double(
        oscillator_angular_freq_double(x) * delta_samples, 
        prediction,
        x
    );
}

void oscillator_update_double(double complex update_frequency, OscillatorDouble *nco) {
    double complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabs(update_frequency);

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

}



OscillatorFloat *oscillator_make_float(
    float phase_degrees, 
    float normalized_frequency,
    const MemoryManager *manager
) {
    OscillatorFloat *oscillator = manager->allocate(sizeof(OscillatorFloat));
    if (oscillator == NULL) {
        return NULL;
    }

    oscillator->complex_frequency = angular_to_complex_frequency_float(
        ordinary_frequency_to_angular_float(normalized_frequency)
    );
    oscillator->phasor = rotate_phasor_float(
        zero_phase_phasor_float, 
        degrees_to_radians_float(phase_degrees)
    );

    return oscillator;
}

void oscillator_free_float(OscillatorFloat *oscillator) {
    oscillator->free(oscillator);
}

float oscillator_inphase_float(const OscillatorFloat *x) {
    return crealf(x->phasor);
}

float oscillator_quadrature_float(const OscillatorFloat *x) {
    return cimagf(x->phasor);
}

float complex oscillator_phase_float(const OscillatorFloat *osc) {
    return osc->phasor;
}


float oscillator_angular_phase_float(const OscillatorFloat *x) {
    return cargf(x->phasor);
}

float complex oscillator_frequency_float(const OscillatorFloat *osc) {
    return osc->complex_frequency;
}

float oscillator_angular_freq_float(const OscillatorFloat *x) {
    return cargf(x->complex_frequency);
}

void oscillator_shift_phase_float(
    float angle_radians, 
    OscillatorFloat *shifted_output, 
    const OscillatorFloat *x
) {
    shifted_output->complex_frequency = x->complex_frequency;
    shifted_output->phasor = rotate_phasor_float(x->phasor, angle_radians);
}

void oscillator_predict_float(
    float delta_samples, 
    OscillatorFloat *prediction, 
    const OscillatorFloat *x
) {
    oscillator_shift_phase_float(
        oscillator_angular_freq_float(x) * delta_samples, 
        prediction,
        x
    );
}

void oscillator_update_float(float complex update_frequency, OscillatorFloat *nco) {
    float complex next_frequency =
        update_frequency == 0.0 ? 
        nco->complex_frequency : 
        update_frequency / cabsf(update_frequency);

    nco->complex_frequency = next_frequency;
    nco->phasor = nco->phasor * next_frequency;

}
