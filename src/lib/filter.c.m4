include(`types.m4')
include(`code_generator.m4')
include(`filter.m4')

#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "savgol.h"
#include "filter.h"
#include "constants.h"
#include "assertions.h"

/**
 * @brief 
 * Normalized sinc function
 * @param x input
 * @return result
 */
double sinc(double x);

/**
 * @brief 
 * Dirac delta function
 * @param x Input
 * @return Result 
 */
double dirac_delta(double x);

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_EVALUATE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_RESET_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_FREE_DIGITAL_FILTER')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_SAVGOL')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_EWMA')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_FIRST_ORDER_IIR')

M4_MAKE_FOR_NUMERIC_TYPES(`M4_MAKE_FILTER_MAKE_SINC')

double sinc(double x) {
    return x == 0.0 ? 1.0 : sin(M_PI * x) / (M_PI * x);
}

double dirac_delta(double x) {
    return x == 0.0 ? 1.0 : 0.0;
}