#ifndef QUICKWAVE_CONSTANTS
#define QUICKWAVE_CONSTANTS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline double ordinary_frequency_to_angular(double ordinary_frequency) {
    return 2 * M_PI * ordinary_frequency;
}

static inline double angular_frequency_to_ordinary(double angular_frequency) {
    return angular_frequency / (2 * M_PI);
}

static inline double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180;
}

static inline double radians_to_degrees(double radians) {
    return radians * 180 / M_PI;
}

#endif