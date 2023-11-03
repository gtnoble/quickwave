#ifndef QUICKWAVE_PID
#define QUICKWAVE_PID

/**
 * @brief 
 * Proportional-integral-derivative filter.
 * Useful for control applications.
 */
typedef struct {
    double previous_input;
    double accumulated_input;
    double proportional_gain;
    double integral_gain;
    double derivative_gain;
} Pid;

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
);

/**
 * @brief 
 * Resets a PID filter to its initial values
 * @param pid PID filter to reset
 */
void pid_reset(Pid *pid);

/**
 * @brief 
 * Evaluates a PID filter
 * @param input Next input signal value 
 * @param pid PID filter
 * @return Filtered value
 */
double pid_evaluate(double input, Pid *pid);

#endif