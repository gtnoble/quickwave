(load-from-path "substitutions.scm")

(output-text
"
#ifndef QUICKWAVE_CONSTANTS
#define QUICKWAVE_CONSTANTS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

"

    (generate-text
        number-schema
"
static inline ${number-base-type} ordinary_frequency_to_angular${function-tag}(${number-base-type} ordinary_frequency) {
    return 2 * M_PI * ordinary_frequency;
}

static inline ${number-base-type} angular_frequency_to_ordinary${function-tag}(${number-base-type} angular_frequency) {
    return angular_frequency / (2 * M_PI);
}

static inline ${number-base-type} degrees_to_radians${function-tag}(${number-base-type} degrees) {
    return degrees * M_PI / 180;
}

static inline ${number-base-type} radians_to_degrees${function-tag}(${number-base-type} radians) {
    return radians * 180 / M_PI;
}

")

"
#endif
")