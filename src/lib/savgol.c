#include <assert.h>

static double gp(int i, int m, int k, int s) {

    if (k>0) {
	return (4.0*k-2.0)/(k*(2.0*m-k+1.0))*(i*gp(i,m,k-1,s) +
	  s*gp(i,m,k-1,s-1))-((k-1.0)*(2.0*m+k))/(k*(2.0*m-k+1.0))*gp(i,m,k-2,s);    
    } else {
	if (k==0 && s==0) {
	    return 1.0;
	} else {
	    return 0.0;
	}
    }
}

// generalized factorial
// (a)(a-1)...(a-b+1)

static double genfact(int a, int b) {
    int j;
    double gf;
    gf=1.0;
    for (j=(a-b)+1; j<=a; j++) {
	gf*=j;
    }
    return(gf);
}

// calculate the weight of the ith dat point for the tth
// lease-square point of the sth derivative, over 2m+1 points
// order n

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
double savgol_weight(int i, int center, int window, int polyorder, int derivative) {
    assert(i >= 0);
    assert(window > 0);

    /**
     * @brief 
     * Window length must be odd
     */
    assert((window & 0x1) == 1);

    assert(polyorder >= 0);
    assert(derivative >= 0);

    int n = window / 2;
    int k;
    double w = 0;
    for (k=0; k<=polyorder; k++) {
	w += (2*k+1)*(genfact(2*n,k)/genfact(2*n+k+1,k+1))*gp(i-n,n,k,0)*gp(center,n,k,derivative);
    }
    return w;
}