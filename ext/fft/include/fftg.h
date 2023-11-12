typedef enum {
    FORWARD_TRANSFORM = 1,
    REVERSE_TRANSFORM = -1
} TransformDirection;

/**
 * @brief 
 * Complex Discrete Fourier Transform
 * @param n data length, n >= 1, n = power of 2
 * @param isgn transform direction, 1 for forward, -1 for unscaled reverse
 * @param a input/output data
 * @param ip work area for bit reversal 
 * @param w cos/sin table
 * 
 * @details
 * [definition]
 *     <case1>
 *         X[k] = sum_j=0^n-1 x[j]*exp(2*pi*i*j*k/n), 0<=k<n
 *     <case2>
 *         X[k] = sum_j=0^n-1 x[j]*exp(-2*pi*i*j*k/n), 0<=k<n
 *     (notes: sum_j=0^n-1 is a summation from j=0 to n-1)
 * [usage]
 *     <case1>
 *         ip[0] = 0; // first time only
 *         cdft(2*n, 1, a, ip, w);
 *     <case2>
 *         ip[0] = 0; // first time only
 *         cdft(2*n, -1, a, ip, w);
 * [parameters]
 *     2*n            :data length (int)
 *                     n >= 1, n = power of 2
 *     a[0...2*n-1]   :input/output data (double *)
 *                     input data
 *                         a[2*j] = Re(x[j]), 
 *                         a[2*j+1] = Im(x[j]), 0<=j<n
 *                     output data
 *                         a[2*k] = Re(X[k]), 
 *                         a[2*k+1] = Im(X[k]), 0<=k<n
 *     ip[0...*]      :work area for bit reversal (int *)
 *                     length of ip >= 2+sqrt(n)
 *                     strictly, 
 *                     length of ip >= 
 *                         2+(1<<(int)(log(n+0.5)/log(2))/2).
 *                     ip[0],ip[1] are pointers of the cos/sin table.
 *     w[0...n/2-1]   :cos/sin table (double *)
 *                     w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 *     Inverse of 
 *         cdft(2*n, -1, a, ip, w);
 *     is 
 *         cdft(2*n, 1, a, ip, w);
 *         for (j = 0; j <= 2 * n - 1; j++) {
 *             a[j] *= 1.0 / n;
 *         }
 * 
 */
void cdft(int n, TransformDirection isgn, double *a, int *ip, double *w);

/**
 * @brief 
 * Real Discrete Fourier Transform
 * @param n data length, n >= 1, n = power of 2
 * @param isgn transform direction, 1 for forward, -1 for unscaled reverse
 * @param a input/output data
 * @param ip work area for bit reversal 
 * @param w cos/sin table
 * 
 * @details
 * [definition]
 *     <case1> RDFT
 *         R[k] = sum_j=0^n-1 a[j]*cos(2*pi*j*k/n), 0<=k<=n/2
 *         I[k] = sum_j=0^n-1 a[j]*sin(2*pi*j*k/n), 0<k<n/2
 *     <case2> IRDFT (excluding scale)
 *         a[k] = (R[0] + R[n/2]*cos(pi*k))/2 + 
 *                 sum_j=1^n/2-1 R[j]*cos(2*pi*j*k/n) + 
 *                 sum_j=1^n/2-1 I[j]*sin(2*pi*j*k/n), 0<=k<n
 * [usage]
 *     <case1>
 *         ip[0] = 0; // first time only
 *         rdft(n, 1, a, ip, w);
 *     <case2>
 *         ip[0] = 0; // first time only
 *         rdft(n, -1, a, ip, w);
 * [parameters]
 *     n              :data length (int)
 *                     n >= 2, n = power of 2
 *     a[0...n-1]     :input/output data (double *)
 *                     <case1>
 *                         output data
 *                             a[2*k] = R[k], 0<=k<n/2
 *                             a[2*k+1] = I[k], 0<k<n/2
 *                             a[1] = R[n/2]
 *                     <case2>
 *                         input data
 *                             a[2*j] = R[j], 0<=j<n/2
 *                             a[2*j+1] = I[j], 0<j<n/2
 *                             a[1] = R[n/2]
 *     ip[0...*]      :work area for bit reversal (int *)
 *                     length of ip >= 2+sqrt(n/2)
 *                     strictly, 
 *                     length of ip >= 
 *                         2+(1<<(int)(log(n/2+0.5)/log(2))/2).
 *                     ip[0],ip[1] are pointers of the cos/sin table.
 *     w[0...n/2-1]   :cos/sin table (double *)
 *                     w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 *     Inverse of 
 *         rdft(n, 1, a, ip, w);
 *     is 
 *         rdft(n, -1, a, ip, w);
 *         for (j = 0; j <= n - 1; j++) {
 *             a[j] *= 2.0 / n;
 *         }
 *     .
 */
void rdft(int n, TransformDirection isgn, double *a, int *ip, double *w);

void ddct(int n, int isgn, double *a, int *ip, double *w);

void ddst(int n, int isgn, double *a, int *ip, double *w);

void dfct(int n, double *a, double *t, int *ip, double *w);

void dfst(int n, double *a, double *t, int *ip, double *w);

void makewt(int nw, int *ip, double *w);

void makect(int nc, int *ip, double *c);