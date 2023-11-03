
#include "pid.h"

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed filter
 */
Pid pid_make(
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
double pid_evaluate(double input, Pid *pid) {
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
void pid_reset(Pid *pid) {
    pid->accumulated_input = 0.0;
    pid->previous_input = 0.0;
}
