/*
 * em.cpp
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */

using namespace std;

#include "em.h"
#include "BLAS.h"

int VERBOSE_LEVEL;

int em_step(Dataset data, GTMsd gtm, int era, double *loglik) {
	int tree;
	WorkingTree w = NULL;
	//Expectation step
	for (tree = 0; tree < data->n; tree++) {
		if (gtm->stationary)
			w
					= new NPosWorkingTree_c(gtm, data->trees[tree]);
		else
			w = new PosWorkingTree_c(gtm, data->trees[tree]);
		upwards(gtm, w);
		downwards(gtm, w);
		gtm->update(tree, w->E1, w->labels, w->Nu, w->get_nodes());
		w->~WorkingTree_c();
	}
	//Maximization step
	return gtm->upgrade(data, era, loglik);

}

void upwards(P_GTMsd gtm, WorkingTree w) {
	int  u = w->get_nodes() - 1;
	for (; u >= 0; u--) {
		w->calculate_P_N(gtm, u);
		if (!(w->is_leaf(u)))
			w->calculate_Beta2(gtm, u);
		w->calculate_Beta1(gtm, u);
	}
}

void downwards(GTMsd gtm, WorkingTree w) {
	w->calculate_E1_E2_update_S_E(gtm);
}

