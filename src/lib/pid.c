
#include "pid.h"


PidDouble pid_make_double(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain
) {
    PidDouble pid = {
        .previous_input = 0.0,
        .accumulated_input = 0.0,
        .proportional_gain = proportional_gain,
        .integral_gain = integral_gain,
        .derivative_gain = derivative_gain
    };
    return pid;
}

double pid_evaluate_double(double input, PidDouble *pid) {
    double proportional = input * pid->proportional_gain;
    double integral = pid->integral_gain * (pid->accumulated_input += input);
    double derivative = pid->derivative_gain * (input - pid->previous_input);
    pid->previous_input = input;
    return proportional + integral + derivative;
}

void pid_reset_double(PidDouble *pid) {
    pid->accumulated_input = 0.0;
    pid->previous_input = 0.0;
}



PidFloat pid_make_float(
    float proportional_gain, 
    float integral_gain, 
    float derivative_gain
) {
    PidFloat pid = {
        .previous_input = 0.0,
        .accumulated_input = 0.0,
        .proportional_gain = proportional_gain,
        .integral_gain = integral_gain,
        .derivative_gain = derivative_gain
    };
    return pid;
}

float pid_evaluate_float(float input, PidFloat *pid) {
    float proportional = input * pid->proportional_gain;
    float integral = pid->integral_gain * (pid->accumulated_input += input);
    float derivative = pid->derivative_gain * (input - pid->previous_input);
    pid->previous_input = input;
    return proportional + integral + derivative;
}

void pid_reset_float(PidFloat *pid) {
    pid->accumulated_input = 0.0;
    pid->previous_input = 0.0;
}
