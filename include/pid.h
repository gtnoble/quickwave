#ifndef QUICKWAVE_PID
#define QUICKWAVE_PID

/**
 * @brief 
 * Proportional-integral-derivative controller.
 */
typedef struct {
    double previous_input; /** The previous input value to the PID controller. Used to calculate the derivative */
    double accumulated_input; /** The sum of all input values. Equal to the integral of the inputs. */
    double proportional_gain; /** Gain for the proportional term of PID */
    double integral_gain; /** Gain for the integral term of the PID */
    double derivative_gain; /** Gain for the derivative term of the PID */
} Pid;

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed controller
 */
Pid pid_make(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain
);

/**
 * @brief 
 * Resets a PID controller to its initial values
 * @param pid PID controller to reset
 */
void pid_reset(Pid *pid);

/**
 * @brief 
 * Evaluates and updates a PID controller
 * @param input Next input signal value 
 * @param pid PID controller
 * @return control value
 */
double pid_evaluate(double input, Pid *pid);

#endif