
#ifndef QUICKWAVE_PID
#define QUICKWAVE_PID

#include "memory.h"


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
    Deallocator *free;
} PidDouble;

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed controller
 */
PidDouble *pid_make_double(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain,
    const MemoryManager *manager
);

/**
 * @brief 
 * Resets a PID controller to its initial values
 * @param pid PID controller to reset
 */
void pid_reset_double(PidDouble *pid);

/**
 * @brief 
 * Evaluates and updates a PID controller
 * @param input Next input signal value 
 * @param pid PID controller
 * @return control value
 */
double pid_evaluate_double(double input, PidDouble *pid);

void pid_free_double(PidDouble *pid);



/**
 * @brief 
 * Proportional-integral-derivative controller.
 */
typedef struct {
    float previous_input; /** The previous input value to the PID controller. Used to calculate the derivative */
    float accumulated_input; /** The sum of all input values. Equal to the integral of the inputs. */
    float proportional_gain; /** Gain for the proportional term of PID */
    float integral_gain; /** Gain for the integral term of the PID */
    float derivative_gain; /** Gain for the derivative term of the PID */
    Deallocator *free;
} PidFloat;

/**
 * @brief 
 * Makes a proportional-integral-derivative (PID) filter. Useful for control applications.
 * @param proportional_gain Proportional gain
 * @param integral_gain Integral gain
 * @param derivative_gain Derivative gain
 * @return Constructed controller
 */
PidFloat *pid_make_float(
    float proportional_gain, 
    float integral_gain, 
    float derivative_gain,
    const MemoryManager *manager
);

/**
 * @brief 
 * Resets a PID controller to its initial values
 * @param pid PID controller to reset
 */
void pid_reset_float(PidFloat *pid);

/**
 * @brief 
 * Evaluates and updates a PID controller
 * @param input Next input signal value 
 * @param pid PID controller
 * @return control value
 */
float pid_evaluate_float(float input, PidFloat *pid);

void pid_free_float(PidFloat *pid);


#endif
