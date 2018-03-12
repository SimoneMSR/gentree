/*
 * WorkingTree.cpp
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */

using namespace std;

#include "WorkingTree.h"
#include <iostream>
#include "errors.h"

extern int VERBOSE_LEVEL;


WorkingTree_c::WorkingTree_c(P_GTMsd gtm, Tree t) :
	Tree_c(t) {
	int n, i;
	Nu = new double[nodes];
	E1 = new_matrix(nodes, gtm->C, 0);
	b = new_matrix(nodes, gtm->C);
	P_N = new_matrix(nodes, gtm->C, 0);
	Beta1 = new_matrix(nodes, gtm->C, 0);
	Beta2 = new_matrix(nodes, gtm->C, 0);
	E2nor = new_matrix(nodes, gtm->C, 0);
	for (n = 0; n < nodes; n++)
		//initialize b
		for (i = 0; i < gtm->C; i++)
			b[n][i] = gtm->get_emission(labels[n], i);
}


WorkingTree_c::~WorkingTree_c() {
	delete (Nu);
	free_matrix(E1, nodes);
	free_matrix(b, nodes);
	free_matrix(P_N, nodes);
	free_matrix(Beta1, nodes);
	free_matrix(Beta2, nodes);
	free_matrix(E2nor, nodes);
}


void WorkingTree_c::calculate_Beta1(P_GTMsd gtm, int node) {
	int i, j, l, child;
	double Nn = 0, temp = 0;
	if (is_leaf(node)) {
		//calculate Nu and then Beta1 for leaves
		if (gtm->stationary) {
			if (Nu[node] < FLT_MIN) {
				Nu[node] = 1;
				for (i = 0; i < gtm->C; i++)
					Beta1[node][i] = gtm->get_P_L(i);
			} else
				for (i = 0; i < gtm->C; i++)
					Beta1[node][i] = b[node][i] * gtm->get_P_L(i) / Nu[node];

		} else {
			l = parents[node][1];
			if (Nu[node] < FLT_MIN) {
				Nu[node] = 1;
				for (i = 0; i < gtm->C; i++)
					Beta1[node][i] = gtm->get_P_L(l, i);
			} else
				for (i = 0; i < gtm->C; i++)
					Beta1[node][i] = b[node][i] * gtm->get_P_L(l, i) / Nu[node];
		}
	} else {
		Nn = 0;
		//calculate Nu for internal nodes
		for (j = 0; j < gtm->C; j++) {
			temp = 0;
			for (l = 0; l < gtm->L; l++) {
				child = get_child(node, l);
				if (child > 0)
					temp += gtm->get_phi(l) * Beta2[child][j];
				else {
					if (gtm->stationary)
						temp += gtm->get_phi(l) * gtm->get_Astar(j);
					else
						temp += gtm->get_phi(l) * gtm->get_Astar(l, j);
				}
			}
			Nn += b[node][j] * temp;
		}
		Nu[node] = Nn;
		//calculate the Beta1 values for internal nodes
		for (i = 0; i < gtm->C; i++) {
			temp = 0;
			for (l = 0; l < gtm->L; l++) {
				child = children[node][l];
				if (child > 0)
					temp += gtm->get_phi(l) * Beta2[child][i];
				else {
					if (gtm->stationary)
						temp += gtm->get_phi(l) * gtm->get_Astar(i);
					else
						temp += gtm->get_phi(l) * gtm->get_Astar(l, i);
				}
			}
			if (Nu[node] < FLT_MIN)
				Beta1[node][i] = 1.0 / gtm->C;
			else
				Beta1[node][i] = b[node][i] * temp / Nu[node];
		}

	}
}


void WorkingTree_c::calculate_P_N(P_GTMsd gtm, int node) {
	int i, j, l, child;
	if (is_leaf(node)) {
		//calculate P_N for leaves
		if (gtm->stationary) {
			for (i = 0; i < gtm->C; i++) {
				P_N[node][i] = gtm->get_P_L(i);
			}
		} else {
			l=parents[node][1];
			for (i = 0; i < gtm->C; i++) {
				P_N[node][i] = gtm->get_P_L(l, i);
			}
		}
	} else {
		//calculate P_N for internal nodes
		for (i = 0; i < gtm->C; i++) {
			for (l = 0; l < gtm->L; l++) {
				child = children[node][l];
				if (child > 0) {
					//real child contribute
					for (j = 0; j < gtm->C; j++) {
						if (gtm->stationary) {
							P_N[node][i] += gtm->get_phi(l) * gtm->get_A(i, j)
									* P_N[child][j];
						} else
							P_N[node][i] += gtm->get_phi(l) * gtm->get_A(l, i,
									j) * P_N[child][j];

					}
				} else {
					//ghost child contribute
					if (gtm->stationary)
						P_N[node][i] += gtm->get_phi(l) * gtm->get_Astar(i);
					else
						P_N[node][i] += gtm->get_phi(l) * gtm->get_Astar(l, i);

				}
			}

		}
	}
}


NPosWorkingTree_c::NPosWorkingTree_c(P_GTMsd gtm, Tree tree) :
	WorkingTree_c(gtm, tree) {
	int i, n;
	for (n = 0; n < nodes; n++) {
		//calculate Nu for leaves
		Nu[n] = 0;
		if (is_leaf(n))
			for (i = 0; i < gtm->C; i++)
				Nu[n] += b[n][i] * gtm->get_P_L(i);

	}
}


void NPosWorkingTree_c::calculate_Beta2(P_GTMsd gtm, int parent) {
	int i, j, l, child;
	for (l = 0; l < arity; l++) {
		child = children[parent][l];
		if (child > 0) {
			for (i = 0; i < gtm->C; i++) {
				for (j = 0; j < gtm->C; j++) {
					Beta2[child][i] += gtm->get_A(i, j) * Beta1[child][j];
				}

				//	Beta2[child][i] = Beta2[child][i] / P_N[parent][i];
			}
		}
	}
}


void NPosWorkingTree_c::calculate_E1_E2_update_S_E(GTMsd gtm) {
	int i, j, position, l, l2, parent, child, child2, u;
	double temp = 0, normalize;
	double **E2_u;
	//calculate E1  and E2nor for the root
	for (i = 0; i < gtm->C; i++) {
		E1[0][i] = Beta1[0][i];
		for (l = 0; l < gtm->L; l++) {
			child = children[0][l];
			if (child > 0)
				E2nor[0][i] += gtm->get_phi(l) * Beta2[child][i];
			else
				E2nor[0][i] += gtm->get_phi(l) * gtm->get_Astar(i);
		}
		if (E2nor[0][i] < FLT_MIN) {
			E2nor[0][i] = 1.0 / gtm->C;
			if (VERBOSE_LEVEL == 2)
				my_cerr(OR_E2N);
		}
	}
	//initialize the temporary matrix useful in S_E2 calculus
	E2_u = new_matrix(gtm->C, gtm->C, 0);
	for (u = 1; u < nodes; u++) {
		//calculate E2nor for a node
		for (i = 0; i < gtm->C; i++) {
			for (l = 0; l < gtm->L; l++) {
				child = children[u][l];
				if (child > 0)
					E2nor[u][i] += gtm->get_phi(l) * Beta2[child][i];
				else
					E2nor[u][i] += gtm->get_phi(l) * gtm->get_Astar(i);
			}
			if (E2nor[u][i] < FLT_MIN) {
				E2nor[u][i] = 1.0 / gtm->C;
				if (VERBOSE_LEVEL == 2)
					my_cerr(OR_E2N);
			}
		}

		parent = parents[u][0];
		position = parents[u][1];
		for (i = 0; i < gtm->C; i++) {
			for (j = 0; j < gtm->C; j++) {
				E2_u[i][j] = E1[parent][i] * Beta1[u][j] * gtm->get_phi(
						position) * gtm->get_A(i, j) / E2nor[parent][i];
			}
		}



		//normalize E2
		normalize = 0;
		for (i = 0; i < gtm->C; i++)
			for (j = 0; j < gtm->C; j++)
				normalize += E2_u[i][j];
		for (i = 0; i < gtm->C; i++) {
			for (j = 0; j < gtm->C; j++) {
				E2_u[i][j] = E2_u[i][j] / normalize;
			}
		}

		//update temp_phi
			for (i = 0; i < gtm->C; i++)
				for (j = 0; j < gtm->C; j++)
					gtm->add_temp_phi(position, E2_u[i][j]);

		//calculate and normalize E1
		for (j = 0; j < gtm->C; j++) {
			for (i = 0; i < gtm->C; i++)
				E1[u][j] += E2_u[i][j];
		}
		normalize = 0;
		for (i = 0; i < gtm->C; i++)
			normalize += E1[u][i];

		for (i = 0; i < gtm->C; i++)
			E1[u][i] = E1[u][i] / normalize;

		//update accumulator S_E1 and S_E2, using real child of node
		for (i = 0; i < gtm->C; i++) {
			if (is_leaf(u))
				gtm->train_param-> S_E0[i] += E1[u][i];
			gtm->train_param-> S_E1[i] += E1[u][i];
			for (j = 0; j < gtm->C; j++) {
				gtm->train_param-> S_E2[i][j] += E2_u[i][j];
			}
		}
		//updates accumulator S_Estar, using ghost child of node
		normalize = 0;
		if (!is_leaf(u)) {
			for (l = 0; l < gtm->L; l++) {
				child =children[u][l];
				if (child < 0) {
					for (i = 0; i < gtm->C; i++) {
						temp = 0;
						for (l2 = 0; l2 < gtm->L; l2++) {
							child2 = children[u][l2];
							if (child2 > 0)
								temp += gtm->get_phi(l2) * Beta2[child2][i];
							else
								temp += gtm->get_phi(l2) * gtm->get_Astar(i);
						}
						gtm->train_param-> S_Estar[i] += E1[u][i]
								* gtm->get_phi(l) * gtm->get_Astar(i) / temp;
						if ((E1[u][i] * gtm->get_phi(l) * gtm->get_Astar(i))
								< FLT_MIN) {
							gtm->train_param->S_Estar[i] = 1.0 / gtm->C;
							if (VERBOSE_LEVEL == 2)
								my_cerr(OR_Estar);
						}
					}
				}

			}
		}

	}
	free_matrix(E2_u, gtm->C);
}


PosWorkingTree_c::PosWorkingTree_c(P_GTMsd gtm, Tree tree) :
	WorkingTree_c(gtm, tree) {
	int i, n, l;
	for (n = 0; n < nodes; n++) {
		//calculate N for leaves
		Nu[n] = 0;
		if (is_leaf(n)) {
			l = parents[n][1];
			for (i = 0; i < gtm->C; i++)
				Nu[n] += b[n][i] * gtm->get_P_L(l, i);
		}
	}
}


void PosWorkingTree_c::calculate_Beta2(P_GTMsd gtm, int parent) {
	int i, j, l, child;
	for (l = 0; l < arity; l++) {
		child = children[parent][l];
		if (child > 0) {
			for (i = 0; i < gtm->C; i++) {
				for (j = 0; j < gtm->C; j++) {
					Beta2[child][i] += gtm->get_A(l, i, j) * Beta1[child][j];
				}

			}
		}
	}
}


void PosWorkingTree_c::calculate_E1_E2_update_S_E(GTMsd gtm) {
	int i, j, position, l, l2, parent, child2,child, u;
	double **E2_u, normalize, temp = 0;
	//calculate E1  and E2nor for the root
	for (i = 0; i < gtm->C; i++) {
		E1[0][i] = Beta1[0][i];
		for (l = 0; l < gtm->L; l++) {
			child = children[0][l];
			if (child > 0)
				E2nor[0][i] += gtm->get_phi(l) * Beta2[child][i];
			else
				E2nor[0][i] += gtm->get_phi(l) * gtm->get_Astar(l, i);
		}
		if (E2nor[0][i] < FLT_MIN) {
			E2nor[0][i] = 1.0 / gtm->C;
			if (VERBOSE_LEVEL == 2)
				my_cerr(OR_E2N);
		}
	}
	//initialize the temporary matrix useful in S_E2 calculus containing the values of E2(parent(u),u,i,j) for all i j
	E2_u = new_matrix(gtm->C, gtm->C);
	//start to calculate
	for (u = 1; u < nodes; u++) {
		//calculate E2nor for the node
		for (i = 0; i < gtm->C; i++) {
			for (l = 0; l < gtm->L; l++) {
				child = children[u][l];
				if (child > 0)
					E2nor[u][i] += gtm->get_phi(l) * Beta2[child][i];
				else
					E2nor[u][i] += gtm->get_phi(l) * gtm->get_Astar(l, i);
			}
			if (E2nor[u][i] < FLT_MIN) {
				E2nor[u][i] = 1.0 / gtm->C;
				if (VERBOSE_LEVEL == 2)
					my_cerr(OR_E2N);
			}
		}

		parent = parents[u][0];
		position = parents[u][1];
		for (i = 0; i < gtm->C; i++) {
			for (j = 0; j < gtm->C; j++) {
				E2_u[i][j] = E1[parent][i] * Beta1[u][j] * gtm->get_phi(
						position) * gtm->get_A(position, i, j)
						/ E2nor[parent][i];
			}
		}

		//normalize E2
		normalize = 0;
		for (i = 0; i < gtm->C; i++)
			for (j = 0; j < gtm->C; j++)
				normalize += E2_u[i][j];
		for (i = 0; i < gtm->C; i++) {
			for (j = 0; j < gtm->C; j++)
				E2_u[i][j] = E2_u[i][j] / normalize;
		}

		//calculate and normalize E1
		for (j = 0; j < gtm->C; j++)
			for (i = 0; i < gtm->C; i++)
				E1[u][j] += E2_u[i][j];
		normalize = 0;
		for (i = 0; i < gtm->C; i++)
			normalize += E1[u][i];
		for (i = 0; i < gtm->C; i++)
			E1[u][i] = E1[u][i] / normalize;

		//update accumulator S_E1 and S_E2, using real child of node
		for (i = 0; i < gtm->C; i++) {
			gtm->train_param->ns_S_E1[position][i] += E1[u][i];
			if (is_leaf(u))
				gtm->train_param->ns_S_E0[position][i] += E1[u][i];
			for (j = 0; j < gtm->C; j++)
				gtm->train_param->ns_S_E2[position][i][j] += E2_u[i][j];
		}

		//update accumulator S_Estar, using ghost child of node
		for (l = 0; l < gtm->L; l++) {
			if (children[u][l] < 0) {
				for (i = 0; i < gtm->C; i++) {
					temp = 0;
					for (l2 = 0; l2 < gtm->L; l2++) {
						child2 = children[u][l2];
						if (child2 > 0)
							temp += gtm->get_phi(l2) * Beta2[child2][i];
						else
							temp += gtm->get_phi(l2) * gtm->get_Astar(l2, i);
					}
					gtm->train_param->ns_S_Estar[l][i] += E1[u][i]
							* gtm->get_phi(l) * gtm->get_Astar(l, i) / temp;
					if ((E1[u][i] * gtm->get_phi(l) * gtm->get_Astar(l, i))
							< FLT_MIN) {
						gtm->train_param->ns_S_Estar[l][i] = 1.0 / gtm->C;
						if (VERBOSE_LEVEL == 2)
							my_cerr(OR_Estar);
					}
				}

			}
		}
	}
	free_matrix(E2_u, gtm->C);

}
