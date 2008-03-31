#include <util/quartic.h>

#include <cmath>
#include <cassert>
#include <algorithm>
using namespace std;

int depressed_cubic_real_roots(double P, double Q, double r[])
{
    static const double third = 1.0/3.0;
    double third_P = third * P;
    double disc = 4 * (third_P*third_P*third_P) + Q*Q;
    if (disc >= 0) {
	double root_disc = sqrt(disc);
	double cube = Q < 0 ? -0.5 * (Q - root_disc) : -0.5 * (Q + root_disc);
	double u = cbrt(cube);
	double x = u - third_P / u;
	r[0] = x;
	return 1;
    } else {
	double y3_re = -0.5  * Q;
	double y3_im = 0.5 * sqrt(-disc);
	// y = cube root (y3)
	double theta = atan2(y3_im, y3_re) * third;
	double radius = sqrt(-third_P);
	double y_re = radius * cos(theta);
	double x = 2*y_re;
	double root_disc = sqrt(-3*x*x - 4*P);
	if (x > 0) {
	    r[0] = -0.5 * (x + root_disc);
	    r[1] = -0.5 * (x - root_disc);
	    r[2] = x;
	} else {
	    r[0] = x;
	    r[1] = -0.5 * (x + root_disc);
	    r[2] = -0.5 * (x - root_disc);
	}
	return 3;
    }    
}

int find_quartic_real_roots(double B, double C, double D, double E, double r[])
{
    static const double third = 1.0/3.0;
    double alpha = C - 0.375 * B * B;
    double half_B = 0.5 * B;
    double beta = half_B *(half_B*half_B - C) + D;
    double q_B = 0.25 * B;
    double gamma = q_B * (q_B *(C - 3 * q_B * q_B) - D) + E;
    double third_alpha = alpha * third;
    double P = -alpha * third_alpha * 0.25 - gamma;
    double Q = third_alpha * (-third_alpha * third_alpha * 0.25 + gamma) - beta*beta*0.125;

    double dcr[3];
    int ndcr = depressed_cubic_real_roots(P,Q, dcr);
    double y = dcr[ndcr-1] - third * 2.5 * alpha;

    double disc2 = alpha + 2*y;
    
    double W = sqrt(disc2);

    double disc_base = 2*alpha + disc2; //3*alpha + 2*y;
    double disc_add = 2*beta / W;
    double x_base = -0.25 * B;

    int count = 0;
    if (disc_base + disc_add <= 0) {
	double sr = sqrt(-disc_base-disc_add);
	r[count++] = x_base + 0.5 * (W - sr);
	r[count++] = x_base + 0.5 * (W + sr);
    }
    if (disc_base - disc_add <= 0) {
	double sr = sqrt(-disc_base + disc_add);
	r[count++] = x_base - 0.5 * (W + sr);	
	r[count++] = x_base - 0.5 * (W - sr);
    }
    return count;
}