/*
 * Context.c
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */

using namespace std;

#include "GTMsd.h"
#include "iostream"
#include "errors.h"

extern int VERBOSE_LEVEL;


P_GTMsd_c::P_GTMsd_c(const char *filename) {
	ifstream file;
	char line[1024];
	file.open(filename, ios::in);
	if (!file.is_open()) {
		C = -1;
		return;
	}
	file >> line;
	if (strcmp(line, GTM_HEADER) != 0) {
		C = -1;
		return;
	}
	file >> C;
	file >> D;
	file >> P;
	file >> L;
	file >> var;
	file >> stationary;
	W = read_matrix(&file, D, P + 1);
	phi = read_vector(&file, L);
	if (stationary) {
		P_L = read_vector(&file, C);
		A = read_matrix(&file, C, C);
		Astar = read_vector(&file, C);
	} else {
		ns_P_L = read_matrix(&file, L, C);
		ns_A = read_tensor(&file, L, C, C);
		ns_Astar = read_matrix(&file, L, C);
	}
	Phi = read_matrix(&file, C, P + 1);
	X = read_matrix(&file, C, 2);

}


P_GTMsd_c::~P_GTMsd_c() {
	if (C >= 0) {
		free_vector(phi);
		free_matrix(W, D);
		free_matrix(Phi, C);
		free_matrix(X, C);
		if (stationary) {
			free_vector(P_L);
			free_matrix(A, C);
			delete[] (Astar);
		} else {
			free_tensor(ns_A, L, C);
			free_matrix(ns_P_L, L);
			free_matrix(ns_Astar, L);
		}
	}
}

double P_GTMsd_c::get_phi(int l) {
	return phi[l];
}

double P_GTMsd_c::get_A(int position, int i, int j) {
	return ns_A[position][i][j];
}

double P_GTMsd_c::get_A(int i, int j) {
	return A[i][j];
}

double P_GTMsd_c::get_Astar(int position, int i) {
	return ns_Astar[position][i];
}

double P_GTMsd_c::get_Astar(int i) {
	return Astar[i];
}

double P_GTMsd_c::get_P_L(int position, int i) {
	return ns_P_L[position][i];
}

double P_GTMsd_c::get_P_L(int i) {
	return P_L[i];
}

void P_GTMsd_c::get_Phi(int i, double *ris) {
	int j;
	for (j = 0; j < P + 1; j++)
		ris[j] = Phi[i][j];
}

double P_GTMsd_c::get_X(int i, int j) {
	return X[i][j];
}

double P_GTMsd_c::get_emission(double *label, int i) {
	double norm, retval;
	double *p = new double[P + 1];
	double *vect = new double[D];
	get_Phi(i, p);
	matrix_vector_mul(D, P + 1, W, p, vect);
	vectorial_sub(D, label, vect, vect);
	norm = vectorial_norm_pow2(D, vect);
	retval = exp(-1 * (norm / (2 * var))) / pow((2 * M_PI * var), D / 2);
	if (retval < FLT_MIN) {
		//underflow: replace with consistent value
		if (VERBOSE_LEVEL == 2)
			my_cerr(OR_EMISSION);
		return FLT_MIN;
	}
	delete[] (p);
	delete[] (vect);
	return retval;
}

GTMsd_c::GTMsd_c(Dataset data, bool stat, int c, int p, double lambda, int d,
		int arity) {
	gsl_vector_view point, centre, result;
	int i, j, l, tn, N = data->n;
	double norm = 0, *temp, normalize = 0;
	lam = lambda;
	var = 1;
	stationary = stat;
	likelyhood = 0;
	L = arity;
	C = c;
	P = p;
	D = d;
	Tn = new double[N];
	R = new_matrix(C, D, 0);
	G = new_matrix(C, C, 0);
	W = new_matrix_random(D, P + 1);
	Phi = new_matrix(C, P + 1);
	cp = new_matrix(P, 2);
	X = new_matrix(C, 2);
	phi = new double[L];
	temp = new double[2];
	train_param = new EM_parameter_c;
	for (i = 0; i < N; i++)
		Tn[i] = data->trees[i]->nodes;

	//set the RBF and latent centers
	set_centres(cp, P);
	set_centres(X, C);

	for (i = 0; i < C; i++) {
		//set RBF values
		point = gsl_vector_view_array(X[i], 2);
		Phi[i][P] = 1;
		for (j = 0; j < P; j++) {
			centre = gsl_vector_view_array(cp[j], 2);
			result = gsl_vector_view_array(temp, 2);
			gsl_blas_dcopy(&(centre.vector), &(result.vector));
			gsl_blas_daxpy(-1, &(point.vector), &(result.vector));
			norm = gsl_blas_dnrm2(&(result.vector));
			norm = pow(norm, 2);
			Phi[i][j] = exp(-1 * (norm / (2 * var))) / (2 * M_PI * var);
		}
	}

	//set switching parent values
	normalize = 0;
	for (i = 0; i < L; i++)
		normalize += (phi[i] = rand() / (double) RAND_MAX);
	for (i = 0; i < L; i++)
		phi[i] = phi[i] / normalize;
	if (stationary) {
		temp_phi = new_vector(L, 0);
		//set prior and transition matrixes (for stationary model)
		normalize = 0;
		P_L = new double[C];
		A = new_matrix(C, C);
		Astar = new_vector(C, 0);
		for (i = 0; i < C; i++)
			normalize += (P_L[i] = rand() / (double) RAND_MAX);
		for (i = 0; i < C; i++)
			P_L[i] = P_L[i] / normalize;
		for (j = 0; j < C; j++) {
			normalize = 0;
			for (i = 0; i < C; i++)
				normalize += (A[i][j] = rand() / (double) RAND_MAX);
			for (i = 0; i < C; i++)
				A[i][j] = A[i][j] / normalize;
		}
		normalize = 0;
		for (i = 0; i < C; i++)
			normalize += (Astar[i] = rand() / (double) RAND_MAX);
		for (i = 0; i < C; i++)
			Astar[i] = Astar[i] / normalize;
	} else {
		//set prior and transition matrixes (for non stationary model)
		ns_A = new_tensor(L, C, C);
		ns_Astar = new_matrix(L, C);
		ns_P_L = new_matrix(L, C);
		for (l = 0; l < L; l++) {
			for (i = 0; i < C; i++) {
				normalize = 0;
				for (j = 0; j < C; j++)
					normalize += (ns_A[l][i][j] = rand() / (double) RAND_MAX);
				for (j = 0; j < C; j++)
					ns_A[l][i][j] = ns_A[l][i][j] / normalize;
			}
		}
		for (l = 0; l < L; l++) {
			normalize = 0;
			for (i = 0; i < C; i++)
				normalize += (ns_Astar[l][i] = rand() / (double) RAND_MAX);
			for (i = 0; i < C; i++)
				ns_Astar[l][i] = ns_Astar[l][i] / normalize;
		}
		for (l = 0; l < L; l++) {
			normalize = 0;
			for (i = 0; i < C; i++)
				normalize += (ns_P_L[l][i] = rand() / (double) RAND_MAX);
			for (i = 0; i < C; i++)
				ns_P_L[l][i] = ns_P_L[l][i] / normalize;
		}

	}

	//initialize EM parameters
	train_param->ALL_E = new double**[N];
	for (i = 0; i < N; i++) {
		tn = Tn[i];
		train_param->ALL_E[i] = new double*[tn];
		for (j = 0; j < tn; j++)
			train_param-> ALL_E[i][j] = new double[C];
	}
	if (stationary) {
		train_param->S_E0 = new_vector(C, 0);
		train_param->S_E1 = new_vector(C, 0);
		train_param->S_E2 = new_matrix(C, C, 0);
		train_param->S_Estar = new_vector(C, 0);
	} else {
		train_param-> ns_S_E0 = new_matrix(L, C, 0);
		train_param->ns_S_E1 = new_matrix(L, C, 0);
		train_param->ns_S_E2 = new_tensor(L, C, C, 0);
		train_param->ns_S_Estar = new_matrix(L, C, 0);
	}
}


GTMsd_c::~GTMsd_c() {
	if (C >= 0) {
		free_vector(phi);
		free_matrix(R, C);
		free_matrix(G, C);
		free_matrix(X, C);
		if (stationary) {
			free_vector(train_param->S_E0);
			free_vector(train_param->S_E1);
			free_matrix(train_param->S_E2, C);
		} else {
			free_matrix(train_param->ns_S_E0, L);
			free_matrix(train_param->ns_S_E1, L);
			free_tensor(train_param->ns_S_E2, L, C);
		}
	}
}

P_GTMsd GTMsd_c::convert() {
	free_matrix(R, C);
	free_matrix(G, C);
	free_matrix(cp, P);
	delete[] (Tn);
	if (stationary) {
		free_vector(temp_phi);
		free_vector(train_param->S_E0);
		free_vector(train_param->S_E1);
		free_matrix(train_param->S_E2, C);
	} else {
		free_matrix(train_param->ns_S_E0, L);
		free_matrix(train_param->ns_S_E1, L);
		free_tensor(train_param->ns_S_E2, L, C);
	}
	return this;
}

void GTMsd_c::set_tn(int i, int n) {
	Tn[i] = n;
}

void GTMsd_c::set_phi(int l, double val) {
	phi[l] = val;
}

void GTMsd_c::add_temp_phi(int l, double val) {
	temp_phi[l] += val;
}

void GTMsd_c::set_A(int i, int j, double val) {
	A[i][j] = val;
}
void GTMsd_c::set_A(int l, int i, int j, double val) {
	ns_A[l][i][j] = val;

}
void GTMsd_c::set_P_L(int i, double val) {
	P_L[i] = val;
}
void GTMsd_c::set_P_L(int l, int i, double val) {
	ns_P_L[l][i] = val;
}

void GTMsd_c::update(int tree_num, double **E1, double** labels, double *Nu,
		int tn) {
	int i, j, u;
	double **RY = new double*[C];
	for (i = 0; i < C; i++)
		RY[i] = new double[tn];

	for (u = 0; u < Tn[tree_num]; u++) {
		for (i = 0; i < C; i++) {
			train_param->ALL_E[tree_num][u][i] = E1[u][i];
		}
	}

	matricial_product_transposed(tn, C, D, train_param->ALL_E[tree_num],
			labels, RY);
	for (i = 0; i < C; i++)
		for (j = 0; j < D; j++)
			R[i][j] += RY[i][j];
	for (i = 0; i < C; i++)
		for (j = 0; j < tn; j++)
			G[i][i] += train_param->ALL_E[tree_num][j][i];
	for (i = 0; i < tn; i++)
		likelyhood += log(Nu[i]);
	free_matrix(RY, C);
}


int GTMsd_c::upgrade(Dataset data, int era, double *loglik) {
	int i, j, n, u, Tnd = 0;
	gsl_matrix_view inverse;
	double norm;
	double **LT, **LT1, **PR, **WP;
	double *matrix, *p = new double[P + 1], *vect = new double[D];
	matrix = new double[(P + 1) * (P + 1)];
	LT1 = new double*[P + 1];
	LT = new double*[P + 1];
	PR = new double*[P + 1];
	WP = new_matrix(C, D);
	int i2, l = 0, l2;
	double num = 0, den = 0;
	if (stationary) {
		/*
		 * STATIONARY MODEL UPGRADING
		 */
		den = 0;
		//upgrade the switching parent
		for (i = 0; i < C; i++) {
			for (j = 0; j < C; j++) {
				den += train_param->S_E2[i][j];
			}
		}
		if (den < FLT_MIN) {
			if (VERBOSE_LEVEL == 2)
				my_cerr(OR_PHI);
			for (l = 0; l < L; l++)
				phi[l] = 1.0 / L;
		} else
			for (l = 0; l < L; l++)
				phi[l] = temp_phi[l] / den;
		//upgrade the transitions
		for (i = 0; i < C; i++) {
			for (j = 0; j < C; j++) {
				den = 0;
				for (i2 = 0; i2 < C; i2++)
					den += train_param->S_E2[i2][j];
				if (den < FLT_MIN) {
					A[i][j] = 1.0 / C;
					if (VERBOSE_LEVEL == 2)
						my_cerr(OR_A);
				} else
					A[i][j] = train_param-> S_E2[i][j] / den;
			}
		}
		den = 0;
		//upgrade the prior
		for (i = 0; i < C; i++)
			den += train_param->S_E0[i];
		for (i = 0; i < C; i++) {
			if (den < FLT_MIN) {
				set_P_L(i, FLT_MIN);
				if (VERBOSE_LEVEL == 2)
					my_cerr(OR_PL);
			} else
				set_P_L(i, train_param->S_E0[i] / den);
		}
		//upgrade the transitions for ghost children
		den = 0;
		for (i = 0; i < C; i++)
			den += train_param->S_Estar[i];
		for (i = 0; i < C; i++)
			if (den < FLT_MIN) {
				Astar[i] = 1.0 / C;
				if (VERBOSE_LEVEL == 2)
					my_cerr(OR_Astar);
			} else
				Astar[i] = train_param->S_Estar[i] / den;
	} else {
		/*
		 * NOT STATIONARY MODEL UPGRADING
		 */
		for (l = 0; l < L; l++) {
			num = 0;
			den = 0;
			//upgrade the switching parent
			for (i = 0; i < C; i++) {
				for (j = 0; j < C; j++) {
					num += train_param-> ns_S_E2[l][i][j];
					for (l2 = 0; l2 < L; l2++)
						den += train_param->ns_S_E2[l2][i][j];
				}
			}
			if (den < FLT_MIN) {
				if (VERBOSE_LEVEL == 2)
					my_cerr(OR_PHI);
				phi[l] = 1.0 / L;
			} else
				phi[l] = num / den;
			//upgrade the transitions
			for (i = 0; i < C; i++) {
				for (j = 0; j < C; j++) {
					den = 0;
					for (i2 = 0; i2 < C; i2++)
						den += train_param->ns_S_E2[l][i2][j];
					if (den < FLT_MIN) {
						ns_A[l][i][j] = 1.0 / C;
						if (VERBOSE_LEVEL == 2)
							my_cerr(OR_A);
					} else
						ns_A[l][i][j] = train_param->ns_S_E2[l][i][j] / den;
				}
			}
			den = 0;
			//upgrade the prior
			for (i = 0; i < C; i++)
				den += train_param->ns_S_E0[l][i];
			for (i = 0; i < C; i++) {
				if (den < FLT_MIN) {
					ns_P_L[l][i] = FLT_MIN;
					if (VERBOSE_LEVEL == 2)
						my_cerr(OR_PL);
				} else
					ns_P_L[l][i] = train_param-> ns_S_E0[l][i] / den;
			}

			//upgrade the transitions for ghost node
			den = 0;
			for (i = 0; i < C; i++)
				den += train_param-> ns_S_Estar[l][i];
			for (i = 0; i < C; i++) {
				if (den < FLT_MIN) {
					ns_Astar[l][i] = 1.0 / C;
					if (VERBOSE_LEVEL == 2)
						my_cerr(OR_Astar);
				} else
					ns_Astar[l][i] = train_param->ns_S_Estar[l][i] / den;
			}
		}
	}

	for (i = 0; i < P + 1; i++) {
		LT1[i] = new double[C];
		LT[i] = new double[P + 1];
		PR[i] = new double[D];
	}

	//upgrade  weight matrix
	gsl_set_error_handler_off();
	matricial_product_transposed(C, P + 1, C, Phi, G, LT1);
	matricial_product(P + 1, C, P + 1, LT1, Phi, LT);
	matricial_sum_I(P + 1, P + 1, lam * var, LT);
	matrix_to_vector(P + 1, P + 1, LT, matrix);
	inverse = gsl_matrix_view_array(matrix, P + 1, P + 1);
	if (gsl_linalg_cholesky_decomp(&(inverse.matrix)) != GSL_EDOM)
		gsl_linalg_cholesky_invert(&(inverse.matrix));
	else {
		//LT matrix is not positive definite: use SVD decomposition
		if (VERBOSE_LEVEL)
			my_cerr(CD);
		gsl_vector *s = gsl_vector_alloc(P + 1);
		gsl_matrix *V = gsl_matrix_alloc(P + 1, P + 1);
		gsl_matrix *s_pseudo = gsl_matrix_alloc(P + 1, P + 1);
		gsl_matrix *VS = gsl_matrix_alloc(P + 1, P + 1);
		gsl_matrix *VSU = gsl_matrix_alloc(P + 1, P + 1);
		gsl_linalg_SV_decomp_jacobi(&(inverse.matrix), V, s);
		gsl_matrix_set_zero(s_pseudo);
		for (int i = 0; i < P + 1; i++)
			if (gsl_vector_get(s, i) != 0)
				gsl_matrix_set(s_pseudo, i, i, 1 / gsl_vector_get(s, i));
		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, V, s_pseudo, 0, VS);
		gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1, VS, &(inverse.matrix), 0,
				VSU);
		gsl_matrix_memcpy(&(inverse.matrix), VSU);
		gsl_vector_free(s);
		gsl_matrix_free(V);
		gsl_matrix_free(s_pseudo);
		gsl_matrix_free(VS);
		gsl_matrix_free(VSU);
	}
	matricial_product_transposed(C, P + 1, D, Phi, R, PR);
	vector_to_matrix(P + 1, P + 1, matrix, LT);
	matricial_product_to_trans(P + 1, P + 1, D, LT, PR, W);
	//upgrade variance
	for (i = 0; i < C; i++) {
		get_Phi(i, p);
		matrix_vector_mul(D, P + 1, W, p, WP[i]);
	}
	var = 0;
	for (n = 0; n < data->n; n++) {
		for (u = 0; u < Tn[n]; u++)
			for (i = 0; i < C; i++) {

				vectorial_sub(D, data->trees[n]->labels[u], WP[i], vect);
				norm = vectorial_norm_pow2(D, vect);
				var += norm * train_param->ALL_E[n][u][i];
			}
		Tnd += Tn[n] * D;
	}
	var = var / Tnd;
	if (VERBOSE_LEVEL)
		if (var < VARIANCE_LIMIT)
			my_cerr(VARIANCE);

	loglik[era] = likelyhood;
	if (era > 0 && fabs(fabs(loglik[era]) - fabs(loglik[era - 1]))
			< LOGLIKELYHOOD_LIMIT) {
		if (VERBOSE_LEVEL)
			my_cerr(LOGLIKELYHOOD);
	} else {
		//reset EM parameters
		likelyhood = 0;
		if (stationary) {
			for (i = 0; i < C; i++) {
				train_param->S_E0[i] = 0;
				train_param->S_E1[i] = 0;
				train_param->S_Estar[i] = 0;
				for (j = 0; j < C; j++)
					train_param-> S_E2[i][j] = 0;
			}
			for (l = 0; l < L; l++)
				temp_phi[l] = 0;
		} else {
			for (l = 0; l < L; l++) {
				for (i = 0; i < C; i++) {
					train_param->ns_S_E0[l][i] = 0;
					train_param->ns_S_E1[l][i] = 0;
					train_param->ns_S_Estar[l][i] = 0;
					for (j = 0; j < C; j++)
						train_param->ns_S_E2[l][i][j] = 0;
				}
			}
		}
		for (i = 0; i < C; i++) {
			G[i][i] = 0;
			for (j = 0; j < D; j++)
				R[i][j] = 0;
		}

	}
	delete (matrix);
	delete (vect);
	delete (p);
	free_matrix(LT, P + 1);
	free_matrix(LT1, P + 1);
	free_matrix(PR, P + 1);
	free_matrix(WP, C);
	if (era > 0 && fabs(fabs(loglik[era]) - fabs(loglik[era - 1]))
			< LOGLIKELYHOOD_LIMIT)
		return LOGLIKELYHOOD;
	return 0;
}


int P_GTMsd_c::save(const char *filename) {
	ofstream file;
	file.open(filename, ios::out);
	if (not file.is_open())
		return 1;
	file << GTM_HEADER;
	file << "\n" << C << " " << D << " " << P << " " << L << " " << var << " ";
	if (stationary)
		file << 1 << "\n";
	else
		file << 0 << "\n";
	print_matrix_file(&file, W, D, P + 1);
	file << "\n";
	print_vector_file(&file, phi, L);
	file << "\n";
	if (stationary) {
		print_vector_file(&file, P_L, C);
		file << "\n";
		print_matrix_file(&file, A, C, C);
		file << "\n";
		print_vector_file(&file, Astar, C);
		file << "\n";
	} else {
		print_matrix_file(&file, ns_P_L, L, C);
		file << "\n";
		print_tensor_file(&file, ns_A, L, C, C);
		file << "\n";
		print_matrix_file(&file, ns_Astar, L, C);
		file << "\n";
	}
	print_matrix_file(&file, Phi, C, P + 1);
	file << "\n";
	print_matrix_file(&file, X, C, 2);
	file << "\n";
	file.close();
	return 0;
}
