#include "sinusoid.h"
#include "moving_average.h"

typedef struct {
    Sinusoid reference;
    MovingAverageComplex *fit_window;
} SinusoidFit;

SinusoidFit *sinusoid_fit_make(size_t window_length, double frequency);
void sinusoid_fit_free(SinusoidFit *model);
Sinusoid sinusoid_fit_evaluate(double input, SinusoidFit *model);