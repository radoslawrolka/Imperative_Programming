#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for (int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void swap(double A[][SIZE], int start, int change, int len, double res[]) {
	for (int i = 0; i < len; i++) {
		double tmp = A[start][i];
		A[start][i] = A[change][i];
		A[change][i] = tmp;
	}
	double tp = res[start];
	res[start] = res[change];
	res[change] = tp;
}

void swap_tab(double A[][SIZE], int start, int change, int len) {
	for (int i = 0; i < len; i++) {
		double tmp = A[start][i];
		A[start][i] = A[change][i];
		A[change][i] = tmp;
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < p; j++) {
			for (int k = 0; k < n; k++) {
				AB[i][k] += A[i][j] * B[j][k];
			}
		}
	}
}

// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (A[j][i] != 0) {
				double divider = A[j][i] / A[i][i];
				for (int k = i; k < n; k++) {
					A[j][k] -= divider * A[i][k];
				}
			}
		}
	}
	double determinant = 1;
	for (int i = 0; i < n; i++) {
		determinant *= A[i][i];
	}
	if (determinant != 0) {
		return determinant;
	}	
	return NAN;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
	
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	double tab[SIZE];
	for (int i = 0; i < n; i++) {
		tab[i] = b[i];
	}
	for (int i = 0; i < n-1; i++) {
		double maks = 0;
		int maks_ind = 0;
		for (int j = i + 1; j < n; j++) {
			if (maks < fabs(A[j][i])) {
				maks = fabs(A[j][i]);
				maks_ind = j;
			}
		}
		if (maks < eps) {
			return 0;
		}
		swap(A, i, maks_ind, n, tab);
		for (int j = i + 1; j < n; j++) {
			if (A[j][i] != 0) {
				double divider = A[j][i] / A[i][i];
				for (int k = i; k < n; k++) {
					A[j][k] -= divider * A[i][k];
				}
				tab[j] -= divider * tab[i];
			}
		}
	}
	double determinant = 1;
	for (int i = 0; i < n; i++) {
		determinant *= A[i][i];
	}

	for (int i = n - 1; i > -1; i--) {
		for (int j = n-1; j > i; j--) {
			tab[i] -= A[i][j]*tab[j];
		}
		tab[i] = tab[i] / A[i][i];
		A[i][i] = 1;
	}

	for (int i = 0; i < n; i++) {
		x[i] = tab[i];
	}
	
	if (determinant != 0) {
		return determinant;
	}
	return NAN;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				B[i][i] = 1;
			}
			else {
				B[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < n - 1; i++) {
		double maks = 0;
		int maks_ind = 0;
		for (int j = i + 1; j < n; j++) {
			if (maks < fabs(A[j][i])) {
				maks = fabs(A[j][i]);
				maks_ind = j;
			}
		}
		if (maks < eps) {
			return 0;
		}
		swap_tab(A, i, maks_ind, n);
		swap_tab(B, i, maks_ind, n);
		for (int j = i + 1; j < n; j++) {
			if (A[j][i] != 0) {
				double divider = A[j][i] / A[i][i];
				for (int k = i; k < n; k++) {
					A[j][k] -= divider * A[i][k];
				}
				for (int k = 0; k<n; k++) {
					B[j][k] -= divider * B[i][k];
				}
			}
		}
	}

	double determinant = 1;
	for (int i = 0; i < n; i++) {
		determinant *= A[i][i];
	}


	for (int i = n - 1; i >= 0; i--) {
		for (int j = i-1; j > -1; j--) {
			double div = A[j][i] / A[i][i];
			A[j][i] = 0;
			for (int k = 0; k < n; k++) {
				B[j][k] -= div * B[i][k];
				B[i][k] = B[i][k] / A[i][i];
			}
			A[i][i] = 1;
		}
	}
	for (int i = 0; i < n; i++) {
		B[0][i] = B[0][i] / A[0][0];
	}

	if (determinant != 0) {
		return determinant;
	}
	return NAN;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf("%d", &to_do);

	switch (to_do) {
	case 1:
		scanf("%d %d %d", &m, &p, &n);
		read_mat(A, m, p);
		read_mat(B, p, n);
		mat_product(A, B, C, m, p, n);
		print_mat(C, m, n);
		break;
	case 2:
		scanf("%d", &n);
		read_mat(A, n, n);
		printf("%.4f\n", gauss_simplified(A, n));
		break;
	case 3:
		scanf("%d", &n);
		read_mat(A, n, n);
		read_vector(b, n);
		det = gauss(A, b, x, n, eps);
		printf("%.4f\n", det);
		if (det) print_vector(x, n);
		break;
	case 4:
		scanf("%d", &n);
		read_mat(A, n, n);
		det = matrix_inv(A, B, n, eps);
		printf("%.4f\n", det);
		if (det) print_mat(B, n, n);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}

