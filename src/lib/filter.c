#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include "savgol.h"
#include "filter.h"
#include "constants.h"
#include "assertions.h"

double sinc(double x);

ConvolutionComplex *filter_make_convolution_complex(
    size_t n_stationary_elements, 
    const double complex stationary_elements[]
) {
    CircularBufferComplex *sliding_elements = circbuf_complex_new(n_stationary_elements);
    if (sliding_elements == NULL) {
        return NULL;
    }

    size_t stationary_elements_size = sizeof(double complex) * n_stationary_elements;
    ConvolutionComplex *convolution = malloc(
        sizeof(ConvolutionComplex) + stationary_elements_size
    );

    if (convolution == NULL) {
        circbuf_complex_free(sliding_elements);
        return NULL;
    }

    convolution->n_stationary_elements = n_stationary_elements;
    convolution->sliding_elements = sliding_elements;
    memcpy(
        convolution->stationary_elements, 
        stationary_elements, 
        stationary_elements_size
    );

    return convolution;
}

ConvolutionReal *filter_make_convolution_real(
    size_t n_stationary_elements, 
    const double stationary_elements[]
) {
    CircularBufferReal *sliding_elements = circbuf_real_new(n_stationary_elements);
    if (sliding_elements == NULL) {
        return NULL;
    }

    size_t stationary_elements_size = sizeof(double) * n_stationary_elements;
    ConvolutionReal *convolution = malloc(
        sizeof(ConvolutionReal) + stationary_elements_size
    );

    if (convolution == NULL) {
        circbuf_real_free(sliding_elements);
        return NULL;
    }

    convolution->n_stationary_elements = n_stationary_elements;
    convolution->sliding_elements = sliding_elements;
    memcpy(
        convolution->stationary_elements, 
        stationary_elements, 
        stationary_elements_size
    );

    return convolution;
}

void filter_reset_convolution_complex(ConvolutionComplex *convolution) {
    assert_not_null(convolution);
    circbuf_complex_reset(convolution->sliding_elements);
}

void filter_reset_convolution_real(ConvolutionReal *convolution) {
    assert_not_null(convolution);
    circbuf_real_reset(convolution->sliding_elements);
}

void filter_free_convolution_complex(ConvolutionComplex *convolution) {
    assert_not_null(convolution);
    circbuf_complex_free(convolution->sliding_elements);
    free(convolution);
}

void filter_free_convolution_real(ConvolutionReal *convolution) {
    assert_not_null(convolution);
    circbuf_real_free(convolution->sliding_elements);
    free(convolution);
}

double complex filter_convolve_complex(
    double complex input, 
    ConvolutionComplex *convolution
) {
    assert_not_null(convolution);

    double complex accumulate = 0;
    assert(convolution->sliding_elements != NULL);
    circbuf_complex_shift(input, convolution->sliding_elements);
    for (size_t i = 0; i < convolution->n_stationary_elements; i++) {
        accumulate +=  *circbuf_complex_element(-i, convolution->sliding_elements) *
            convolution->stationary_elements[i];
    }

    return accumulate;
}

double filter_convolve_real(
    double input, 
    ConvolutionReal *convolution
) {
    assert_not_null(convolution);

    double accumulate = 0;
    assert(convolution->sliding_elements != NULL);
    circbuf_real_shift(input, convolution->sliding_elements);
    for (size_t i = 0; i < convolution->n_stationary_elements; i++) {
        accumulate +=  *circbuf_real_element(-i, convolution->sliding_elements) *
            convolution->stationary_elements[i];
    }

    return accumulate;
}

/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double complex filter_evaluate_digital_filter_complex(double complex input, DigitalFilterComplex *filter) {
    assert_not_null(filter);

    double complex accumulate = 0;
    accumulate += filter_convolve_complex(input, filter->feedforward);
    if (filter->feedback != NULL) {
        accumulate += filter_convolve_complex(filter->previous_output, filter->feedback);
        filter->previous_output = accumulate;
    }

    return accumulate;
}

/**
 * @brief 
 * Evaluates a linear digital filter
 * @param input Next input signal value
 * @param filter Filter to apply
 * @return Filtered value
 */
double filter_evaluate_digital_filter_real(double input, DigitalFilterReal *filter) {
    assert_not_null(filter);

    double accumulate = 0;
    accumulate += filter_convolve_real(input, filter->feedforward);
    if (filter->feedback != NULL) {
        accumulate += filter_convolve_real(filter->previous_output, filter->feedback);
        filter->previous_output = accumulate;
    }

    return accumulate;
}

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param n_feedforward Number of filter feedforward coefficients
 * @param feedforward Feedforward coefficient values
 * @param n_feedback Number of filter feedback coefficients
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterComplex *filter_make_digital_filter_complex(
    size_t n_feedforward, 
    const double complex feedforward[],
    size_t n_feedback,
    const double complex feedback[]
    ) {
    DigitalFilterComplex *filter = malloc(sizeof(DigitalFilterComplex));
    if (filter == NULL)
        return NULL;

    assert(n_feedforward > 0);
    assert(feedforward != NULL);
    filter->feedforward = filter_make_convolution_complex(n_feedforward, feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }

    if (n_feedback > 0) {
        assert(feedback != NULL);
        filter->feedback = filter_make_convolution_complex(n_feedback, feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
    }
    else {
        filter->feedback = NULL;
    }

    filter->previous_output = 0;

    return filter;

    fail_allocate_feedback: 
        filter_free_convolution_complex(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;
}

/**
 * @brief 
 * Makes and allocates a linear digital filter.
 * @param n_feedforward Number of filter feedforward coefficients
 * @param feedforward Feedforward coefficient values
 * @param n_feedback Number of filter feedback coefficients
 * @param feedback Feedback coefficient values
 * @return Constucted filter
 */
DigitalFilterReal *filter_make_digital_filter_real(
    size_t n_feedforward, 
    const double feedforward[],
    size_t n_feedback,
    const double feedback[]
    ) {
    DigitalFilterReal *filter = malloc(sizeof(DigitalFilterReal));
    if (filter == NULL)
        return NULL;

    assert(n_feedforward > 0);
    assert(feedforward != NULL);
    filter->feedforward = filter_make_convolution_real(n_feedforward, feedforward);
    if (filter->feedforward == NULL) {
        goto fail_allocate_feedforward;
    }

    if (n_feedback > 0) {
        assert(feedback != NULL);
        filter->feedback = filter_make_convolution_real(n_feedback, feedback);
        if (filter->feedback == NULL) {
            goto fail_allocate_feedback;
        }
    }
    else {
        filter->feedback = NULL;
    }

    filter->previous_output = 0;

    return filter;

    fail_allocate_feedback: 
        filter_free_convolution_real(filter->feedforward);
    fail_allocate_feedforward:
        free(filter);
        return NULL;
}

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_complex(DigitalFilterComplex *filter) {
    assert_not_null(filter);

    filter_reset_convolution_complex(filter->feedforward);
    filter_reset_convolution_complex(filter->feedback);
    filter->previous_output = 0;
}

/**
 * @brief 
 * Resets a linear filter to its initial state
 * @param filter Filter to be reset
 */
void filter_reset_digital_filter_real(DigitalFilterReal *filter) {
    assert_not_null(filter); 

    filter_reset_convolution_real(filter->feedforward);
    filter_reset_convolution_real(filter->feedback);
    filter->previous_output = 0;
}

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_complex(DigitalFilterComplex *filter) {
    assert_not_null(filter);

    filter_free_convolution_complex(filter->feedforward);
    if (filter->feedback != NULL) {
        filter_free_convolution_complex(filter->feedback);
    }
    free(filter);
}

/**
 * @brief 
 * Frees memory associated with a linear filter
 * @param filter Filter to be freed
 */
void filter_free_digital_filter_real(DigitalFilterReal *filter) {
    assert_not_null(filter);

    filter_free_convolution_real(filter->feedforward);
    if (filter->feedback != NULL) {
        filter_free_convolution_real(filter->feedback);
    }
    free(filter);
}

/**
 * @brief 
 * Makes and allocates Savitzky-Golay (savgol) filter
 * @param filter_length Number of filter coefficients
 * @param derivative Order of the derivative for the returned value. 0 means no derivative. 
 * @param polynomial_order Order of the polynomial used for smoothing. 1 is linear, 2 parabolic, etc.
 * @return Constructed filter
 */
DigitalFilterReal *filter_make_savgol(
    size_t filter_length, 
    int derivative, 
    int polynomial_order
) {
    // Window length must be odd
    assert((filter_length & 0x1) == 1);
    assert(filter_length > 0);
    assert(derivative >= 0);
    assert(derivative <= polynomial_order);

    int center = 0;
    double feedforward[filter_length];

    for (size_t i = 0; i < filter_length; i++) {
        feedforward[i] = savgol_weight(
            i, 
            center, 
            filter_length, 
            polynomial_order, 
            derivative
        );
    }

    return filter_make_digital_filter_real(filter_length, feedforward, 0, NULL);
}

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed filter
 */
Pid filter_make_pid(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain
) {
    Pid pid = {
        .previous_input = 0.0,
        .accumulated_input = 0.0,
        .proportional_gain = proportional_gain,
        .integral_gain = integral_gain,
        .derivative_gain = derivative_gain
    };
    return pid;
}

/**
 * @brief 
 * Evaluates a PID filter
 * @param input Next input signal value 
 * @param pid PID filter
 * @return Filtered value
 */
double filter_evaluate_pid(double input, Pid *pid) {
    double proportional = input * pid->proportional_gain;
    double integral = pid->integral_gain * (pid->accumulated_input += input);
    double derivative = pid->derivative_gain * (input - pid->previous_input);
    pid->previous_input = input;
    return proportional + integral + derivative;
}

/**
 * @brief 
 * Resets a PID filter to its initial values
 * @param pid PID filter to reset
 */
void filter_reset_pid(Pid *pid) {
    pid->accumulated_input = 0.0;
    pid->previous_input = 0.0;
}

/**
 * @brief 
 * Makes an exponentially weighted moving average (EWMA) filter
 * @param alpha Smoothing factor 0 < alpha < 1. Smaller alpha means more smoothing.
 * @return EWMA filter 
 */
DigitalFilterComplex *filter_make_ewma(double alpha) {
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    complex double feedforward[] = {alpha};
    complex double feedback[] = {1 - alpha};
    return filter_make_digital_filter_complex(1, feedforward, 1, feedback);
}

/**
 * @brief 
 * Makes a first order IIR low-pass filter. This is a variant of the EWMA filter.
 * @param cutoff_frequency Normalized cutoff frequency
 * @return Constructed filter
 */
DigitalFilterComplex *filter_make_first_order_iir(double cutoff_frequency) {
    assert(cutoff_frequency < 0.5);
    assert(cutoff_frequency >= 0);
    double angular_frequency = ordinary_frequency_to_angular(cutoff_frequency);
    return filter_make_ewma(
        cos(angular_frequency) -
        1 +
        sqrt(
            pow(cos(angular_frequency), 2) - 4 * cos(angular_frequency) + 3
        )
    );
}

/**
 * @brief 
 * Makes and allocates a windowed-sinc low-pass filter
 * @param cutoff_frequency Normalized cutoff frequency
 * @param length Number of filter coefficients
 * @param window Windowing function
 * @return Constructed filter
 */
DigitalFilterReal *filter_make_sinc(
    double cutoff_frequency, 
    size_t length, 
    WindowFunction window
) {
    assert(cutoff_frequency >= 0);

    if (window == NULL)
        window = window_rectangular;

    double feedforward[length];
    double kernel_shift = ((int) length - 1) / 2.0;

    for (size_t i = 0; i < length; i++) {
        feedforward[i] = 
            2 * 
            cutoff_frequency * 
            sinc(2 * cutoff_frequency * ((double) i - kernel_shift)) * 
            window(i, length);
    }
    return filter_make_digital_filter_real(length, feedforward, 0, NULL);
}

/**
 * @brief 
 * Makes and allocates a simple moving average filter
 * @param length Number of sequential elements to average
 * @return Constructed filter
 */
MovingAverage *filter_make_moving_average(size_t length) {
    MovingAverage *filter = malloc(sizeof(MovingAverage));
    
    if (filter == NULL) {
        return NULL;
    }

    filter->previous_input = circbuf_complex_new(length);
    if (filter->previous_input == NULL) {
        free(filter);
        return NULL;
    }

    filter->moving_sum = 0;

    return filter;
}

/**
 * @brief 
 * Evaluates a moving average filter
 * @param input Next input signal value
 * @param filter Moving average filter
 * @return Filtered value
 */
double complex filter_evaluate_moving_average(
    double complex input, 
    MovingAverage *filter
) {
    assert_not_null(filter);

    filter->moving_sum += input;
    filter->moving_sum -= circbuf_complex_shift(input, filter->previous_input);
    return filter->moving_sum / filter->previous_input->n_elements;
}

/**
 * @brief 
 * Resets moving average filter to initial state
 * @param filter Filter to reset
 */
void filter_reset_moving_average(MovingAverage *filter) {
    assert_not_null(filter);

    circbuf_complex_reset(filter->previous_input);
    filter->moving_sum = 0;
}

/**
 * @brief 
 * Frees the memory assicated with a moving average filter
 * @param filter Filter to be freed
 */
void filter_free_moving_average(MovingAverage *filter) {
    assert_not_null(filter);

    free(filter->previous_input);
    free(filter);
}

/**
 * @brief 
 * Normalized sinc function
 * @param x Operand
 * @return Output
 */
double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}