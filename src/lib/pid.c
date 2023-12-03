
#include "pid.h"


PidDouble *pid_make_double(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain,
    const MemoryManager *manager
) {
    PidDouble *pid = manager->allocate(sizeof(PidDouble));
    if (pid == NULL) {
        return NULL;
    }
    pid->previous_input = 0.0;
    pid->accumulated_input = 0.0;
    pid->proportional_gain = proportional_gain;
    pid->integral_gain = integral_gain;
    pid->derivative_gain = derivative_gain;
    pid->free = manager->deallocate;
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

void pid_free_double(PidDouble *pid) {
    pid->free(pid);
}



PidFloat *pid_make_float(
    float proportional_gain, 
    float integral_gain, 
    float derivative_gain,
    const MemoryManager *manager
) {
    PidFloat *pid = manager->allocate(sizeof(PidFloat));
    if (pid == NULL) {
        return NULL;
    }
    pid->previous_input = 0.0;
    pid->accumulated_input = 0.0;
    pid->proportional_gain = proportional_gain;
    pid->integral_gain = integral_gain;
    pid->derivative_gain = derivative_gain;
    pid->free = manager->deallocate;
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

void pid_free_float(PidFloat *pid) {
    pid->free(pid);
}
