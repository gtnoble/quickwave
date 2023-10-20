#ifndef SQLDSP_SINUSOID
#define SQLDSP_SINUSOID

#include <complex.h>
#include "buffer.h"

typedef struct {
    double _Complex complex_frequency;
    double _Complex phasor;
} Sinusoid;

Sinusoid sinusoid_add(Sinusoid a, Sinusoid b);
Sinusoid sinusoid_mult(Sinusoid a, Sinusoid b);
double sinusoid_inphase(Sinusoid x);
double sinusoid_quadrature(Sinusoid x);
double sinusoid_amplitude(Sinusoid x);
double sinusoid_phase(Sinusoid x);
double _Complex real_to_complex_frequency(double angular_frequency);
Sinusoid sinusoid_normalize(Sinusoid x);
Sinusoid sinusoid_negate_phase(Sinusoid x);
Sinusoid sinusoid_shift_phase(double angle, Sinusoid x);
Sinusoid update_vco(double _Complex complex_freq, Sinusoid vco);
Sinusoid quadrature_demodulate(Sinusoid reference, CircularBuffer *lagged_input);

#endif


