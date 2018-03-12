/*
 * BLAS.cpp
 *
 *  Created on: 21/nov/2010
 *      Author: simonemsr
 */

using namespace std;

#include "BLAS.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "GTMsd.h"


void vectorial_sub(int dim, double *v, double *u, double *z) {
	int i;
	for (i = 0; i < dim; i++)
		z[i] = v[i] - u[i];
}


void matrix_vector_mul(int n, int m, double **X, double *v, double *u) {
	int i, j;
	for (i = 0; i < n; i++) {
		u[i] = 0;
		for (j = 0; j < m; j++)
			u[i] += X[i][j] * v[j];
	}
}


void matricial_product(int n, int m, int o, double ** X, double** Y,
		double ** Z) {
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < o; j++) {
			Z[i][j] = 0;
			for (k = 0; k < m; k++)
				Z[i][j] += X[i][k] * Y[k][j];
		}
}


double *read_vector(ifstream *f, int n) {
	double *v = new double[n];
	for (int i = 0; i < n; i++)
		*f >> v[i];
	return v;
}


double **read_matrix(ifstream*f, int n, int m) {
	double **M = new double*[n];
	for (int i = 0; i < n; i++)
		M[i] = read_vector(f, m);
	return M;
}


double ***read_tensor(ifstream *f, int n, int m, int o) {
	double ***T = new double**[n];
	for (int i = 0; i < n; i++)
		T[i] = read_matrix(f, m, o);
	return T;
}

ostream *print_vector(double* v, int n) {
	print_vector_file(&cout, v, n);
	return &cout;
}

ostream *print_matrix(double**M, int n, int m) {
	print_matrix_file(&cout, M, n, m);
	return &cout;
}

ostream* print_tensor(double***T, int n, int m, int o) {
	print_tensor_file(&cout, T, n, m, o);
	return &cout;
}

void print_vector_file(ostream * f, double* v, int n) {
	f->setf(ios::left);
	int i;
	for (i = 0; i < n; i++) {
		f->precision(4);
		f->width(14);
		*f << v[i];
	}
	*f << "\n";
}

void print_matrix_file(ostream *f, double**M, int n, int m) {
	f->setf(ios::left);
	f->width(14);
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			f->precision(7);
			f->width(14);
			*f << M[i][j];
		}
		*f << "\n";
	}
}

void print_tensor_file(ostream *f, double***T, int n, int m, int o) {
	int l = 0;
	for (l = 0; l < n; l++)
		print_matrix_file(f, T[l], m, o);
}


double* new_vector(int n, double val) {
	int i;
	double *vect = new double[n];
	for (i = 0; i < n; i++)
		vect[i] = val;
	return vect;
}


double** new_matrix(int n, int m) {
	int i;
	double **M = new double*[n];
	for (i = 0; i < n; i++)
		M[i] = new double[m];
	return M;
}


double** new_matrix_random(int n, int m) {
	int i, j;
	double **M = new_matrix(n, m);
	srand(time(NULL));
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			M[i][j] = rand() / (double) (RAND_MAX / 2) - 1;
	}
	return M;
}


double** new_matrix(int n, int m, double val) {
	int i, j;
	double **M = new_matrix(n, m);
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			M[i][j] = val;
	return M;
}


double*** new_tensor(int n, int m, int o) {
	int l;
	double ***T = new double**[n];
	for (l = 0; l < n; l++)
		T[l] = new_matrix(m, o);
	return T;
}


double*** new_tensor(int n, int m, int o, double val) {
	int l;
	double ***T = new double**[n];
	for (l = 0; l < n; l++)
		T[l] = new_matrix(m, o, val);
	return T;
}

void free_matrix(double **M, int n) {
	if (not M)
		return;
	int i = 0;
	for (i = 0; i < n; i++)
		delete[](M[i]);
	delete[](M);
}

void free_matrix(int **M, int n) {
	if (not M)
		return;
	int i = 0;
	for (i = 0; i < n; i++)
		delete[](M[i]);
	delete[](M);
}

void free_tensor(double ***T, int n, int m) {
	if (not T)
		return;
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			delete[](T[i][j]);
		delete[](T[i]);
	}
	delete[](T);
}

void realloc_vector(double **v, int old_dim, int new_dim) {
	double *new_v = new double[new_dim];
	for (int i = 0; i < old_dim; i++)
		new_v[i] = (*v)[i];
	delete (*v);
	*v = new_v;
}

void realloc_vector(int **v, int old_dim, int new_dim) {
	int *new_v = new int[new_dim];
	for (int i = 0; i < old_dim; i++)
		new_v[i] = (*v)[i];
	delete (*v);
	*v = new_v;
}

void free_vector(double*ptr) {
	if (ptr)
		delete[](ptr);
}

void free_vector(int *ptr) {
	if (ptr)
		delete[](ptr);
}

void free_vector(bool *ptr) {
	if (ptr)
		delete[](ptr);
}

void matricial_product_transposed(int tn, int c, int d, double ** X,
		double** Y, double ** Z) {
	int i, j, k;
	for (i = 0; i < c; i++)
		for (j = 0; j < d; j++) {
			Z[i][j] = 0;
			for (k = 0; k < tn; k++)
				Z[i][j] += X[k][i] * Y[k][j];
		}
}

double vectorial_norm_pow2(int dim, double *vect) {
	int i;
	double val = 0;
	for (i = 0; i < dim; i++) {
		val += vect[i] * vect[i];
	}
	return val;
}

void matricial_sum_I(int n, int m, double val, double ** X) {
	int i;
	for (i = 0; i < n; i++)
		X[i][i] += val;

}

void matrix_to_vector(int n, int m, double ** X, double *x) {
	int i, j, k = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++, k++)
			x[k] = X[i][j];
}

void vector_to_matrix(int n, int m, double *x, double **X) {
	int i, j, k = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++, k++)
			X[i][j] = x[k];
}

void matricial_product_to_trans(int n, int m, int o, double ** X, double** Y,
		double ** Z) {
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < o; j++) {
			Z[j][i] = 0;
			for (k = 0; k < m; k++)
				Z[j][i] += X[i][k] * Y[k][j];
		}
}

double calculate_sum_over_vector(double *v, int m) {
	int j;
	double retval = 0;
	for (j = 0; j < m; j++)
		retval += v[j];
	return retval;
}

double calculate_sum_over_rows(double **M, int m, int dim) {
	return calculate_sum_over_vector(M[m], dim);
}

double calculate_sum_over_columns(double **M, int c, int D) {
	int i;
	double retval = 0;
	for (i = 0; i < D; i++)
		retval += M[i][c];
	return retval;
}

void set_centres(double ** X, int C) {
	double x, y;
	x = -1;
	y = -1;
	for (int i = 0; i < C; i++) {
		X[i][0] = x;
		X[i][1] = y;
		x += 2.0 / (sqrt(C) - 1);
		if (x > 1) {
			x = -1;
			y += 2.0 / (sqrt(C) - 1);
		}
	}
}

bool is_square(double x) {
	double s = sqrt(x);
	int i_s = s;
	return s == i_s;
}
