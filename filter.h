#include "buffer.h"

typedef struct {
    double* feedforward;
    size_t n_feedforward;
    CircularBuffer *previous_inputs;

    double* feedback;
    size_t n_feedback;
    CircularBuffer *previous_outputs;

    int window_size;
} DigitalFilter;

double filter_evaluate(double input, DigitalFilter *filter);
DigitalFilter *filter_make(size_t n_feedforward, size_t n_feedback);
void filter_free(DigitalFilter *filter);
double filter_current_value(DigitalFilter *filter);
DigitalFilter *filter_make_savgol(size_t window_length, int deriv, int polyorder);