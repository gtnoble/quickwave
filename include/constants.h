
#ifndef QUICKWAVE_CONSTANTS
#define QUICKWAVE_CONSTANTS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


static inline double ordinary_frequency_to_angular_double(double ordinary_frequency) {
    return 2 * M_PI * ordinary_frequency;
}

static inline double angular_frequency_to_ordinary_double(double angular_frequency) {
    return angular_frequency / (2 * M_PI);
}

static inline double degrees_to_radians_double(double degrees) {
    return degrees * M_PI / 180;
}

static inline double radians_to_degrees_double(double radians) {
    return radians * 180 / M_PI;
}




static inline float ordinary_frequency_to_angular_float(float ordinary_frequency) {
    return 2 * M_PI * ordinary_frequency;
}

static inline float angular_frequency_to_ordinary_float(float angular_frequency) {
    return angular_frequency / (2 * M_PI);
}

static inline float degrees_to_radians_float(float degrees) {
    return degrees * M_PI / 180;
}

static inline float radians_to_degrees_float(float radians) {
    return radians * 180 / M_PI;
}


#endif
