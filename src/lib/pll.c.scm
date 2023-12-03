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
${pll-type} *pll_make${function-tag}(
    const ${oscillator-type} *nco_initial,
    const ${pid-type} *loop_filter,
    const MemoryManager *manager
) {
    ${pll-type} *pll = manager->allocate(sizeof(${pll-type}));
    pll->loop_filter = *loop_filter;
    pll->nco = *nco_initial;
    pll->free = manager->deallocate;

    return pll;
}

void pll_reset${function-tag}(${oscillator-type} nco_initial, ${pll-type} *pll) {
    pll->nco = nco_initial;
}

void pll_free${function-tag}(${pll-type} *pll) {
    pll->free(pll);
}

void pll_evaluate${function-tag}(
    ${number-type} input, 
    ${oscillator-type} *output,
    ${pll-type} *pll
) {
    ${number-type} complex phase_error = conj${math-function-suffix}(oscillator_phase${function-tag}(&pll->nco)) * input;

    ${number-type} complex next_frequency = 
        angular_to_complex_frequency${function-tag}(
            pid_evaluate${function-tag}(carg${math-function-suffix}(phase_error), &pll->loop_filter)
        );

    *output = pll->nco;
    oscillator_update${function-tag}(next_frequency, &pll->nco);
}

${pid-type} *pll_loop_filter_make${function-tag}(
    ${number-type} noise_bandwidth, 
    ${number-type} damping_coefficient, 
    const MemoryManager *manager
) {
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
        0.0,
        manager
    );
}
"
    )

)