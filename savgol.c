
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
double savgol_weight(int i, int center, int window, int polyorder, int derivative) {
    int k;
    double w = 0;
    for (k=0; k<=polyorder; k++) {
	w += (2*k+1)*(genfact(2*window,k)/genfact(2*window+k+1,k+1))*gp(i,window,k,0)*gp(center,window,k,derivative);
    }
    return w;
}