(load-from-path "substitutions.scm")

(output-text

"
#ifndef QUICKWAVE_PID
#define QUICKWAVE_PID

"
    (generate-text
        pid-schema
"
/**
 * @brief 
 * Proportional-integral-derivative controller.
 */
typedef struct {
    ${number-type} previous_input; /** The previous input value to the PID controller. Used to calculate the derivative */
    ${number-type} accumulated_input; /** The sum of all input values. Equal to the integral of the inputs. */
    ${number-type} proportional_gain; /** Gain for the proportional term of PID */
    ${number-type} integral_gain; /** Gain for the integral term of the PID */
    ${number-type} derivative_gain; /** Gain for the derivative term of the PID */
} ${pid-type};

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed controller
 */
${pid-type} pid_make${function-tag}(
    ${number-type} proportional_gain, 
    ${number-type} integral_gain, 
    ${number-type} derivative_gain
);

/**
 * @brief 
 * Resets a PID controller to its initial values
 * @param pid PID controller to reset
 */
void pid_reset${function-tag}(${pid-type} *pid);

/**
 * @brief 
 * Evaluates and updates a PID controller
 * @param input Next input signal value 
 * @param pid PID controller
 * @return control value
 */
${number-type} pid_evaluate${function-tag}(${number-type} input, ${pid-type} *pid);
")

"

#endif
"

)