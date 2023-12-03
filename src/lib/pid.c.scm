(load-from-path "substitutions.scm")

(output-text

"
#include \"pid.h\"

"

    (generate-text
        pid-schema
"
${pid-type} *pid_make${function-tag}(
    ${number-type} proportional_gain, 
    ${number-type} integral_gain, 
    ${number-type} derivative_gain,
    const MemoryManager *manager
) {
    ${pid-type} *pid = manager->allocate(sizeof(${pid-type}));
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

${number-type} pid_evaluate${function-tag}(${number-type} input, ${pid-type} *pid) {
    ${number-type} proportional = input * pid->proportional_gain;
    ${number-type} integral = pid->integral_gain * (pid->accumulated_input += input);
    ${number-type} derivative = pid->derivative_gain * (input - pid->previous_input);
    pid->previous_input = input;
    return proportional + integral + derivative;
}

void pid_reset${function-tag}(${pid-type} *pid) {
    pid->accumulated_input = 0.0;
    pid->previous_input = 0.0;
}

void pid_free${function-tag}(${pid-type} *pid) {
    pid->free(pid);
}
")
)