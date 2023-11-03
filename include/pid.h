
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

Pid pid_make(
    double proportional_gain, 
    double integral_gain, 
    double derivative_gain
);
void pid_reset(Pid *pid);
double pid_evaluate(double input, Pid *pid);
