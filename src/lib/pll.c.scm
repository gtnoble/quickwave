(load-from-path "substitutions.scm")

(output-text
"
#include \"pll.h\"
#include \"oscillator.h\"
#include \"constants.h\"
#include \"phasor.h\"
#include \"pid.h\"
#include <math.h>
#include <assert.h>
#include <complex.h>

"

    (generate-text
        pll-schema
"
${pll-type} pll_make${function-tag}(
    ${oscillator-type} nco_initial,
    ${pid-type} loop_filter
) {
    ${pll-type} pll;
    pll.loop_filter = loop_filter;
    pll.nco = nco_initial;

    return pll;
}

void pll_reset${function-tag}(${oscillator-type} nco_initial, ${pll-type} *pll) {
    pll->nco = nco_initial;
}

${oscillator-type} pll_evaluate${function-tag}(${number-type} input, ${pll-type} *pll) {
    ${number-type} complex phase_error = conj${math-function-suffix}(oscillator_phase${function-tag}(pll->nco)) * input;

    ${number-type} complex next_frequency = 
        angular_to_complex_frequency${function-tag}(
            pid_evaluate${function-tag}(carg${math-function-suffix}(phase_error), &pll->loop_filter)
        );

    return oscillator_update${function-tag}(next_frequency, &pll->nco);
}

${pid-type} pll_loop_filter_make${function-tag}(${number-type} noise_bandwidth, ${number-type} damping_coefficient) {
    return pid_make${function-tag}(
        4.0 * damping_coefficient * noise_bandwidth / 
        (
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient)
        ),
        4.0 * pow${math-function-suffix}(noise_bandwidth, 2.0) /
        pow${math-function-suffix}(
            damping_coefficient + 
            1.0 / (4.0 * damping_coefficient),
            2.0
        ),
        0.0
    );
}
"
    )

)