(load-from-path "substitutions.scm")

(output-text
"
typedef enum {
    FORWARD_TRANSFORM,
    UNSCALED_INVERSE_TRANSFORM
} TransformDirection;

"

    (generate-text
        fft-schema
"
/**
 * @brief 
 * Complex Discrete Fourier Transform
 * @param n Length of the input/output data buffer. 
 * Must be twice the input data length, must be a power of 2
 * @param isgn transform direction
 * @param a input/output data. The first half of the elements store the input and output data.
 * @param ip work area for bit reversal. 
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
 *         cdft(2*n, UNSCALED_INVERSE_TRANSFORM, a, ip, w);
 *     <case2>
 *         ip[0] = 0; // first time only
 *         cdft(2*n, FORWARD_TRANSFORM, a, ip, w);
 * [parameters]
 *     2*n            :data length (int)
 *                     n >= 1, n = power of 2
 *     a[0...2*n-1]   :input/output data (${number-type} *)
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
 *     w[0...n/2-1]   :cos/sin table (${number-type} *)
 *                     w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 *     Inverse of 
 *         cdft(2*n, FORWARD_TRANSFORM, a, ip, w);
 *     is 
 *         cdft(2*n, UNSCALED_INVERSE_TRANSFORM, a, ip, w);
 *         for (j = 0; j <= 2 * n - 1; j++) {
 *             a[j] *= 1.0 / n;
 *         }
 * 
 */
void cdft${function-tag}(int n, TransformDirection direction, ${number-type} *a, int *ip, ${number-type} *w);

/**
 * @brief 
 * Real Discrete Fourier Transform
 * @param n data length, n >= 1, n = power of 2
 * @param isgn transform direction
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
 *         rdft(n, FORWARD_TRANSFORM, a, ip, w);
 *     <case2>
 *         ip[0] = 0; // first time only
 *         rdft(n, UNSCALED_INVERSE_TRANSFORM, a, ip, w);
 * [parameters]
 *     n              :data length (int)
 *                     n >= 2, n = power of 2
 *     a[0...n-1]     :input/output data (${number-type} *)
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
 *     w[0...n/2-1]   :cos/sin table (${number-type} *)
 *                     w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 *     Inverse of 
 *         rdft(n, FORWARD_TRANSFORM, a, ip, w);
 *     is 
 *         rdft(n, UNSCALED_INVERSE_TRANSFORM, a, ip, w);
 *         for (j = 0; j <= n - 1; j++) {
 *             a[j] *= 2.0 / n;
 *         }
 *     .
 */
void rdft${function-tag}(int n, TransformDirection isgn, ${number-type} *a, int *ip, ${number-type} *w);

/**
 * @brief 
 * Discrete Cosine Transform
 * @param n data length, n >= 1, n = power of 2
 * @param isgn transform direction
 * @param a input/output data
 * @param ip work area for bit reversal 
 * @param w cos/sin table
 * 
 * @details
 * [definition]
 * <case1> IDCT (excluding scale)
 *     C[k] = sum_j=0^n-1 a[j]*cos(pi*j*(k+1/2)/n), 0<=k<n
 * <case2> DCT
 *     C[k] = sum_j=0^n-1 a[j]*cos(pi*(j+1/2)*k/n), 0<=k<n
 * [usage]
 * <case1>
 *     ip[0] = 0; // first time only
 *     ddct(n, 1, a, ip, w);
 * <case2>
 *     ip[0] = 0; // first time only
 *     ddct(n, -1, a, ip, w);
 * [parameters]
 * n              :data length (int)
 *                 n >= 2, n = power of 2
 * a[0...n-1]     :input/output data (${number-type} *)
 *                 output data
 *                     a[k] = C[k], 0<=k<n
 * ip[0...*]      :work area for bit reversal (int *)
 *                 length of ip >= 2+sqrt(n/2)
 *                 strictly, 
 *                 length of ip >= 
 *                     2+(1<<(int)(log(n/2+0.5)/log(2))/2).
 *                 ip[0],ip[1] are pointers of the cos/sin table.
 * w[0...n*5/4-1] :cos/sin table (${number-type} *)
 *                 w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 * Inverse of 
 *     ddct(n, -1, a, ip, w);
 * is 
 *     a[0] *= 0.5;
 *     ddct(n, 1, a, ip, w);
 *     for (j = 0; j <= n - 1; j++) {
 *         a[j] *= 2.0 / n;
 *     }
 * .
 */
void ddct${function-tag}(int n, int isgn, ${number-type} *a, int *ip, ${number-type} *w);

/**
 * @brief 
 * Discrete Sine Transform
 * @param n data length, n >= 1, n = power of 2
 * @param isgn transform direction
 * @param a input/output data
 * @param ip work area for bit reversal 
 * @param w cos/sin table
 * @details
 * [definition]
 * <case1> IDST (excluding scale)
 *     S[k] = sum_j=1^n A[j]*sin(pi*j*(k+1/2)/n), 0<=k<n
 * <case2> DST
 *     S[k] = sum_j=0^n-1 a[j]*sin(pi*(j+1/2)*k/n), 0<k<=n
 * [usage]
 * <case1>
 *     ip[0] = 0; // first time only
 *     ddst(n, 1, a, ip, w);
 * <case2>
 *     ip[0] = 0; // first time only
 *     ddst(n, -1, a, ip, w);
 * [parameters]
 * n              :data length (int)
 *                 n >= 2, n = power of 2
 * a[0...n-1]     :input/output data (${number-type} *)
 *                 <case1>
 *                     input data
 *                         a[j] = A[j], 0<j<n
 *                         a[0] = A[n]
 *                     output data
 *                         a[k] = S[k], 0<=k<n
 *                 <case2>
 *                     output data
 *                         a[k] = S[k], 0<k<n
 *                         a[0] = S[n]
 * ip[0...*]      :work area for bit reversal (int *)
 *                 length of ip >= 2+sqrt(n/2)
 *                 strictly, 
 *                 length of ip >= 
 *                     2+(1<<(int)(log(n/2+0.5)/log(2))/2).
 *                 ip[0],ip[1] are pointers of the cos/sin table.
 * w[0...n*5/4-1] :cos/sin table (${number-type} *)
 *                 w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 * Inverse of 
 *     ddst(n, -1, a, ip, w);
 * is 
 *     a[0] *= 0.5;
 *     ddst(n, 1, a, ip, w);
 *     for (j = 0; j <= n - 1; j++) {
 *         a[j] *= 2.0 / n;
 *     }
 * .
*/
void ddst${function-tag}(int n, int isgn, ${number-type} *a, int *ip, ${number-type} *w);

/**
 * @brief 
 * Cosine Transform of RDFT (Real Symmetric DFT)
 * @param n 
 * @param a 
 * @param t 
 * @param ip 
 * @param w 
 * 
 * @details
 * [definition]
 * C[k] = sum_j=0^n a[j]*cos(pi*j*k/n), 0<=k<=n
 * [usage]
 * ip[0] = 0; // first time only
 * dfct(n, a, t, ip, w);
 * [parameters]
 * n              :data length - 1 (int)
 *                 n >= 2, n = power of 2
 * a[0...n]       :input/output data (${number-type} *)
 *                 output data
 *                     a[k] = C[k], 0<=k<=n
 * t[0...n/2]     :work area (${number-type} *)
 * ip[0...*]      :work area for bit reversal (int *)
 *                 length of ip >= 2+sqrt(n/4)
 *                 strictly, 
 *                 length of ip >= 
 *                     2+(1<<(int)(log(n/4+0.5)/log(2))/2).
 *                 ip[0],ip[1] are pointers of the cos/sin table.
 * w[0...n*5/8-1] :cos/sin table (${number-type} *)
 *                 w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 * Inverse of 
 *     a[0] *= 0.5;
 *     a[n] *= 0.5;
 *     dfct(n, a, t, ip, w);
 * is 
 *     a[0] *= 0.5;
 *     a[n] *= 0.5;
 *     dfct(n, a, t, ip, w);
 *     for (j = 0; j <= n; j++) {
 *         a[j] *= 2.0 / n;
 *     }
 * .
 */
void dfct${function-tag}(int n, ${number-type} *a, ${number-type} *t, int *ip, ${number-type} *w);

/**
 * @brief 
 * Sine Transform of RDFT (Real Anti-symmetric DFT)
 * @param n 
 * @param a 
 * @param t 
 * @param ip 
 * @param w 
 * 
 * @details
 * [definition]
 *     S[k] = sum_j=1^n-1 a[j]*sin(pi*j*k/n), 0<k<n
 * [usage]
 *     ip[0] = 0; // first time only
 *     dfst(n, a, t, ip, w);
 * [parameters]
 *     n              :data length + 1 (int)
 *                     n >= 2, n = power of 2
 *     a[0...n-1]     :input/output data (${number-type} *)
 *                     output data
 *                         a[k] = S[k], 0<k<n
 *                     (a[0] is used for work area)
 *     t[0...n/2-1]   :work area (${number-type} *)
 *     ip[0...*]      :work area for bit reversal (int *)
 *                     length of ip >= 2+sqrt(n/4)
 *                     strictly, 
 *                     length of ip >= 
 *                         2+(1<<(int)(log(n/4+0.5)/log(2))/2).
 *                     ip[0],ip[1] are pointers of the cos/sin table.
 *     w[0...n*5/8-1] :cos/sin table (${number-type} *)
 *                     w[],ip[] are initialized if ip[0] == 0.
 * [remark]
 *     Inverse of 
 *         dfst(n, a, t, ip, w);
 *     is 
 *         dfst(n, a, t, ip, w);
 *         for (j = 1; j <= n - 1; j++) {
 *             a[j] *= 2.0 / n;
 *         }
 */
void dfst${function-tag}(int n, ${number-type} *a, ${number-type} *t, int *ip, ${number-type} *w);

void makewt${function-tag}(int nw, int *ip, ${number-type} *w);

void makect${function-tag}(int nc, int *ip, ${number-type} *c);
"
)
)