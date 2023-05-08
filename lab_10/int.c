#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp) (double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
	return 2 * pow(x, 5) - 4 * pow(x, 4) + 3.5 * pow(x, 2) + 1.35 * x - 6.25;
}

double f_rat(double x) {
	return 1 / (pow((x - 0.5), 2) + 0.01);
}

double f_exp(double x) {
	return 2 * x * exp(-1.5 * x) - 1;
}

double f_trig(double x) {
	return x * tan(x);
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
	double delta = (b - a) / n;
	double x_to_value = a;
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += delta * f1(x_to_value);
		x_to_value += delta;
	}
	return result;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
	double delta = (b - a) / n;
	double x_to_value = a + delta;
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += delta * f1(x_to_value);
		x_to_value += delta;
	}
	return result;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
	double delta = (b - a) / n;
	double x_to_value = a + delta / 2;
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += delta * f1(x_to_value);
		x_to_value += delta;
	}
	return result;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
	double delta = (b - a) / n;
	double x_to_value_1 = a;
	double x_to_value_2 = a + delta;
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += ((func(x_to_value_1) + func(x_to_value_2)) * delta) / 2;
		x_to_value_1 = x_to_value_2;
		x_to_value_2 += delta;
	}
	return result;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
	double delta = (b - a) / n;
	double x_to_value_1 = a;
	double h = delta / 2;
	double x_to_value_2 = a + h;
	double x_to_value_3 = a + delta;
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += (h / 3) * (f(x_to_value_1) + 4 * f(x_to_value_2) + f(x_to_value_3));
		x_to_value_1 = x_to_value_3;
		x_to_value_2 += delta;
		x_to_value_3 += delta;
	}
	return result;
}

// pointer to quadrature function
typedef double (*QuadratureFp) (Func1vFp, double, double, int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
	return quad_tab[quad_no](func_tab[fun_no], a, b, n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
	if (level == RECURS_LEVEL_MAX) {
		return NAN;
	}
	double c = (a + b) / 2;
	double result_1 = quad(f, a, c, 1);
	double result_2 = quad(f, c, b, 1);
	if (fabs(result_1 + result_2 - S) < delta) {
		return result_1 + result_2;
	}
	else {
		return recurs(f, a, c, result_1, delta / 2, quad, level + 1) +
			recurs(f, c, b, result_2, delta / 2, quad, level + 1);
	}
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
	double result_0 = quad(f, a, b, 1);
	return recurs(f, a, b, result_0, delta, quad, 1);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double, double);

double func2v_2(double x, double y) {
	return 2 - x * x - y * y * y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7 * exp(-2 * x * x);
}
double upper_bound2(double x) {
	return sin(10 * x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
	double delta_x = (x2 - x1) / nx;
	double delta_y = (y2 - y1) / ny;
	double x_to_value = x1;
	double y_to_value = y1;
	double result = 0;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			result += (f(x_to_value, y_to_value));
			y_to_value += delta_y;
		}
		x_to_value += delta_x;
		y_to_value = y1;
	}
	return result * delta_x * delta_y;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {
	double delta_x = (x2 - x1) / nx;
	int ny = (int)(round(fmax(fh(x1), fh(x2)) - fmin(fg(x1), fg(x2))) / hy) + 1;
	double result = 0;
	for (double x_to_value = x1 + delta_x / 2.0; x_to_value < x2; x_to_value += delta_x) {
		double gy = fg(x_to_value);
		double hy = fh(x_to_value);
		double delta_y = (hy - gy) / ny;
		for (double yj = gy + delta_y / 2.0; yj < hy; yj += delta_y) {
			result += f(x_to_value, yj) * delta_y * delta_x;
		}
	}
	return result;

}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh) {
	double hx = (x2 - x1) / nx;
	double hy = (y2 - y1) / ny;
	double x, y;
	double xi = x1, yi = y1, qi;
	double result = 0;

	for (int i = 0; i < nx; i++) {
		qi = 0;
		for (int j = 0; j < ny; j++) {
			if (fg(xi) < yi && yi < fh(xi)) {
				qi += f(xi, yi);
			}
			yi += hy;
		}
		result += qi * hx * hy;
		xi += hx;
		yi = y1;
	}

	return result;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2 * v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1] * v[1] + (v[2] - 1) * (v[2] - 1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i * v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i] - 1) * (v[i] - 1);
	return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
	double result = 0;
	double x, y, z;
	double x0, y0, z0;
	int n = 3;
	int m = tn[0];
	int p = tn[1];
	int q = tn[2];

	double dx = (variable_lim[0][1] - variable_lim[0][0]) / m;
	double dy = (variable_lim[1][1] - variable_lim[1][0]) / p;
	double dz = (variable_lim[2][1] - variable_lim[2][0]) / q;

	x0 = variable_lim[0][0] + (0.5) * dx;
	y0 = variable_lim[1][0] + (0.5) * dy;
	z0 = variable_lim[2][0] + (0.5) * dz;

	x = x0;
	for (int i = 0; i < m; i++) {
		y = y0;
		for (int j = 0; j < p; j++) {
			z = z0;
			for (int k = 0; k < q; k++) {
				double v[] = { x, y, z };

				if (boundary == NULL || boundary(v, n) == 1) {
					result += f(v, n);
				}
				z += dz;
			}
			y += dy;
		}
		x += dx;
	}

	return result * dx * dy * dz;
}

double h_g[20];
// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double* p_sum, FuncNvFp f, int variable_no, double t_variable[], double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) {
	double h = (variable_lim[level][1] - variable_lim[level][0]) / tn[level];
	t_variable[level] = variable_lim[level][0] + (h / 2.0);

	h_g[level] = h;

	for (int i = 0; i < tn[level]; i++)
	{
		if (level < variable_no - 1) recur_quad_rect_mid(p_sum, f, variable_no, t_variable, variable_lim, tn, level + 1, boundary);

		else if (level == variable_no - 1 && (boundary == NULL || boundary(t_variable, variable_no))) {
			double to_add = f(t_variable, variable_no);
			for (int j = 0; j <= level; j++)
				to_add *= h_g[j];
			(*p_sum) += to_add;
		}

		t_variable[level] += h;
	}
}

int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
	case 1: // loop over quadratures and integrands
		scanf("%lf %lf %d", &a, &b, &n);
		for (int q = 0; q < no_quads; ++q) {
			for (int f = 0; f < no_funcs; ++f) {
				printf("%.5f ", quad_select(f, q, a, b, n));
			}
			printf("\n");
		}
		break;
	case 2: // adaptive algorithm
		scanf("%d %d", &integrand_fun_no, &method_no);
		scanf("%lf %lf %lf", &a, &b, &delta);
		printf("%.5f\n", init_recurs(func_tab[integrand_fun_no], a, b, delta, quad_tab[method_no]));
		break;
	case 3: // double integral over a rectangle
		scanf("%lf %lf %d", &x1, &x2, &nx);
		scanf("%lf %lf %d", &y1, &y2, &ny);
		printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
		break;
	case 4: // double integral over normal domain
		scanf("%lf %lf %d", &x1, &x2, &nx);
		scanf("%lf", &hy);
		printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
		break;
	case 5: // double integral over multiple normal domains
		scanf("%lf %lf %d", &x1, &x2, &nx);
		scanf("%lf %lf %d", &y1, &y2, &ny);
		printf("%.5f\n", dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
		break;
	case 6: // triple integral over a cuboid
		scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
		scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn + 1);
		scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn + 2);
		scanf("%d", &flag);
		printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
		break;
	case 7: // multiple integral over hyper-cuboid
		scanf("%d", &n);
		if (n > N_MAX) break;
		for (int i = 0; i < n; ++i) {
			scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn + i);
		}
		scanf("%d", &flag);
		sum = 0.;
		recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
		printf("%.5f\n", sum);
		break;
	default:
		printf("Nothing to do for %d\n", to_do);
		break;
	}
	return 0;
}
