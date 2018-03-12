/*
 * Context.h
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */
#ifndef GTMSD_H_
#define GTMSD_H_

using namespace std;
#include "Dataset.h"
#include <time.h>
#include <float.h>
#include "BLAS.h"
#include "Tree.h"
#include "em_parameter.h"
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#define INIT 0.5
#define GTM_HEADER "__GTM_HEADER__"
#define VARIANCE_LIMIT 0.001
#define LOGLIKELYHOOD_LIMIT 0.00001

/*
 * The projection GTMSD class. A P_GTMsd object is a GTM for structured data useful for projection, not for learning.
 *
 */
class P_GTMsd_c {
public:
	bool stationary;//stationarity of model
	int L; //max number of children
	int D; //dimension of label vector
	int C; //number of hidden states
	int P; //RBF dimension
	double var;//variance^2


protected:
	//parameter for the standard GTM activation [see (5)]
	double **W; //weight matrix

	//switching parent
	double *phi;

	//RBF matrix
	double **Phi;

	//prior probability for leaves for stationary/non stationary model
	double *P_L;
	double **ns_P_L;

	double **X; //latent points

	//transitions for stationary/non stationary model (existing children)
	double **A;
	double ***ns_A;

	//transitions for stationary/non stationary model (ghost children)
	double *Astar;
	double **ns_Astar;

public:

	//Empty constructor
	P_GTMsd_c() {
	}
	;

	/*
	 * Free memory used by a P_GTMsd object
	 */
	~P_GTMsd_c();

	/*
	 *Load a GTMsd model from file.
	 *The loaded GTMsd is useful for projection, not for new training.
	 *If an error occurred during I/O operations constructor returns,
	 *signaling the error by setting C variable to -1
	 */
	P_GTMsd_c(const char *filename);

	/*
	 * Return the specified element of switching parent vector
	 */
	double get_phi(int);

	/*
	 * Store in center the coordinates of i-th RBF matrix center
	 */
	void get_Phi(int i, double *center);

	/*
	 * Return the specified element of transition probability (for real child)
	 */
	double get_A(int, int, int);
	double get_A(int, int);

	/*
	 * Return the specified element of transition probability (for ghost child)
	 */
	double get_Astar(int, int);
	double get_Astar(int);

	/*
	 * Return the specified element of prior probability
	 */
	double get_P_L(int, int);
	double get_P_L(int);

	/*
	 * Return the emission for specified label
	 */
	double get_emission(double*, int);

	/*
	 * Return the specified element of hidden-state center matrix
	 */
	double get_X(int, int);

	/*
	 * Save a P_GTMsd model in a file.
	 */
	int save(const char *filename);
};

typedef P_GTMsd_c* P_GTMsd;

/*
 * The GTMSD class. A GTMsd object is a GTM for structured data, a collection of structure containing
 * learning parameters and a reference of EM algorithm parameters. Inherit from P_GTMsd
 *
 */
class GTMsd_c: public P_GTMsd_c {

private:

	double lam;

	//RBF points and dimension
	double **cp;

	double **R;//state-label matrix
	double **G;//diagonal matrix


	//switching parent updator
	double *temp_phi;

	double likelyhood;
	char *dataset;//dataset filename
	double *Tn;//number of nodes in dataset Trees

public:

	double learning_period;
	EM_parameter train_param;

	/*
	 * Initialize a new GTMsd object and allocate its EM algorithm structures.
	 * No controls on parameters are done.
	 */
	GTMsd_c(Dataset, bool, int, int, double, int, int);

	/*
	 * Free memory used by a GTMsd object
	 */
	~GTMsd_c();

	/*
	 * Update the specified value of switching parent vector (not stationary model)
	 */
	void add_temp_phi(int l, double val);

	/*
	 * Set the specified element of switching parent value (stationary model)
	 */
	void set_phi(int, double);

	/*
	 * Set to n the specified element of Tn vector
	 */
	void set_tn(int i, int n);

	/*
	 * Set the specified value of transition probability
	 */
	void set_A(int, int, int, double); //not stationary
	void set_A(int, int, double); //stationary

	/*
	 * Set the specified value of prior probability
	 */
	void set_P_L(int, double); //stationary
	void set_P_L(int, int, double); //not stationary

	/*
	 * Update some EM parameters with current observer tree learning data
	 */
	void update(int tree_num, double **E1, double** labels, double * Nu, int);

	/*
	 *Upgrade learning structure at the end of an era. Reset EM accumulator
	 */
	int upgrade(Dataset, int, double*);

	/*
	 *Make the GTMsd object useful for projection only.
	 */
	P_GTMsd convert();

};

typedef class GTMsd_c *GTMsd;

#endif /* GTMSD_H_ */
