/*
 * BLAS.h
 *
 *  Created on: 21/nov/2010
 *      Author: simonemsr
 */
#ifndef BLAS_H_
#define BLAS_H_

using namespace std;
#include <iostream>
#include <fstream>


/*
 * Calculate the vector sub z=v-u.
 *
 */
void vectorial_sub(int dim, double *v, double *u, double *z) ;

/*
 * Calculate matrix-vector product u=X*z
 */
void matrix_vector_mul(int n, int m, double **X, double *v, double *u) ;

/*
 * Calculate matrix-matrix product Z=X*Y
 */
void matricial_product(int n, int m, int o, double ** X, double** Y,
		double ** Z);

/*
 * Allocate a new vector v, reading its elements from file.
 * Return v.
 */
double *read_vector(ifstream *f, int n) ;
/*
 * Allocate a new matrix M, reading its elements from file.
 * Returns M.
 */
double **read_matrix(ifstream*f, int n, int m) ;

/*
 * Allocate a new tensor T, reading its elements from file.
 * Return T.
 */
double ***read_tensor(ifstream *f, int n, int m, int o);

/*
 * Put in the outstream values of vector v
 */
ostream *print_vector(double* v, int n) ;

/*
 * Put in the outstream values of matrix M
 */
ostream *print_matrix(double**M, int n, int m);

/*
 * Put in the outstream values of tensor T
 */
ostream* print_tensor(double***T, int n, int m, int o) ;

/*
 * Write in f values of vector v
 */
void print_vector_file(ostream * f, double* v, int n);

/*
 * Write in f values of matrix M
 */
void print_matrix_file(ostream *f, double**M, int n, int m);

/*
 * Write in f values of tensor T
 */
void print_tensor_file(ostream *f, double***T, int n, int m, int o) ;

/*
 * Allocate a new vector v. Set its elements to val.
 * Return v.
 */
double* new_vector(int n, double val) ;

/*
 * Allocate a new matrix M.
 * Return M.
 */
double** new_matrix(int n, int m) ;

/*
 * Allocate a new matrix M. Set its elements to random values.
 * Return M.
 */
double** new_matrix_random(int n, int m) ;
/*
 * Allocate a new matrix M. Set its elements to val.
 * Return M.
 */
double** new_matrix(int n, int m, double val) ;
/*
 * Allocate a new tensor T.
 * Return T.
 */
double*** new_tensor(int n, int m, int o) ;

/*
 * Allocate a new tensor T. Set its elements to val.
 * Return T.
 */
double*** new_tensor(int n, int m, int o, double val);

/*
 * Free allocate memory for vector v
 *
 */
void free_vector(double*v) ;

/*
 * Free allocate memory for vector v
 *
 */
void free_vector(int *v) ;

/*
 * Free allocate memory for vector v
 *
 */
void free_vector(bool *v) ;

/*
 * Free allocate memory for matrix M
 *
 */
void free_matrix(double **M, int n) ;

/*
 * Free allocate memory for matrix M
 *
 */
void free_matrix(int **M, int n);

/*
 * Free allocate memory for tensor T
 *
 */
void free_tensor(double ***T, int n, int m) ;

/*
 * Reallocate memory for vector v to the new dimension new_dim
 *
 */
void realloc_vector(double **v, int old_dim, int new_dim);

/*
 * Reallocate memory for vector v to the new dimension new_dim
 *
 */
void realloc_vector(int **v, int old_dim, int new_dim) ;


/*
 * Calculate matrix-matrix product Z=X'*Y where X' is the transposed X matrix
 */
void matricial_product_transposed(int tn, int c, int d, double ** X,
		double** Y, double ** Z);

/*
 * Calculate double power of euclidean norm of the vector vect
 */
double vectorial_norm_pow2(int dim, double *vect);

/*
 * Add val to the diagonal elements of X
 */
void matricial_sum_I(int n, int m, double val, double ** X);

/*
 * Store (by lines) values of X in x
 */
void matrix_to_vector(int n, int m, double ** X, double *x);

/*
 * Store (by lines) values of x in X
 */
void vector_to_matrix(int n, int m, double *x, double **X) ;

/*
 * Calculate matrix-matrix product Z'=X*Y where Z' is the transposed Z result matrix
 */
void matricial_product_to_trans(int n, int m, int o, double ** X, double** Y,
		double ** Z) ;

/*
 * Calculate the sum of v values
 */
double calculate_sum_over_vector(double *v, int m) ;

/*
 * Calculate the sum of m-th row values of matrix M
 */
double calculate_sum_over_rows(double **M, int m, int dim) ;

/*
 * Calculate the sum of c-th column values of matrix M
 */
double calculate_sum_over_columns(double **M, int c, int D);

/*
 * Fill X with C equidistant values in [-1;1]
 */
void set_centres(double ** X, int C) ;

/*
 * Return true if x is a square, false otherwise.
 */
bool is_square(double x);

#endif /* BLAS_H_ */
