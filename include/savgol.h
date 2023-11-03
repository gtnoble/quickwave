/**
 * @brief 
 * Calucluates the coefficients of a Savitzky-Golay (savgol) filter
 * @param i Index of coefficient. Must be greater than or equal to zero
 * @param center Where to center the estimated value. Zero means that it is centered at the middle of the filter
 * @param window Number of filter coefficients. Must be odd
 * @param polyorder Order of polynomial used to perform the smoothing
 * @param derivative Order of the derivative of the estimated value
 * @return Filter coefficient
 */
double savgol_weight(int i, int center, int window, int polyorder, int derivative);