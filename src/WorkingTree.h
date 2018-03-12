/*
 * Working_tree.h
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */
#ifndef WORKINGTREE_H_
#define WORKINGTREE_H_

using namespace std;

#include "GTMsd.h"


/*
 *
 * The WorkingTree class. A WorkingTree is a Tree witch has learning information used during EM algorithm steps. Inherit from Tree class.
 *
 */
class WorkingTree_c :public  Tree_c{

protected:

	double **P_N; //prior propagation [see (12)]
public:

	double **Beta1; //upwards probability [see (9) and (10)]
	double **Beta2; //upwards ausilary probability [see (13)]
	double **b; //emission probability [see (5)]
	double *Nu; //Nu values for all nodes
	double **E2nor;
	double **E1; //smoothed probability [see (8)]
	WorkingTree_c() {
	}
	;


	/*
	 * Create a new WorkingTree, inherited from the Tree t.
	 */
	WorkingTree_c(P_GTMsd, Tree);

	/*
	 * Free memory of WorkingTree.
	 */
	~WorkingTree_c();

	/*
	 * Calculate the value of P_N(node,i), for all i [see (12)]
	 */
	void calculate_P_N(P_GTMsd, int);

	/*
	 * Calculate the value of Beta1(node,i) for all i [see (10)]
	 */
	void calculate_Beta1(P_GTMsd, int);

	virtual void calculate_Beta2(P_GTMsd, int){};
	virtual void calculate_E1_E2_update_S_E(GTMsd){};
};

typedef class WorkingTree_c *WorkingTree;

/*
 * The NPosWorkingTree class. A NPosWorkingTree is a WorkingTree for stationary model learning. Inherit from WorkingTree class.
 */
class NPosWorkingTree_c: public WorkingTree_c {

public:
	/*
	 * Create a new WorkinTree  for non positional GTM. Inherit from Tree t
	 */
	NPosWorkingTree_c(P_GTMsd, Tree);

	/*
	 * Calculate the value of Beta2(u=parent(child),child,i) for all i [see (13)]
	 */
	void calculate_Beta2(P_GTMsd, int);

	/*
	 * Calculate the values of E1(node,i) and E2(node,i) for all node and i [see (8) and (16)]
	 */
	void calculate_E1_E2_update_S_E(GTMsd);
};

typedef class NPosWorkingTree_c *NPosWorkingTree;

/*
 * The PosWorkingTree class. A PosWorkingTree is a WorkingTree for not stationary model learning. Inherit from WorkingTree class.
 */
class PosWorkingTree_c: public WorkingTree_c {
public:

	/*
	 * Create a new WorkinTree  for positional GTM. Inherit from Tree t
	 */
	PosWorkingTree_c(P_GTMsd, Tree);

	/*
	 * Calculate the value of Beta2(parent,child(parent,l),i) for all l and i [see (13)]
	 */
	void calculate_Beta2(P_GTMsd, int);

	/*
	 * Calculate the values of E1(node,i) and E2(node,i) for all node and i [see (8) and (16)]
	 */
	void calculate_E1_E2_update_S_E(GTMsd);
};

typedef class PosWorkingTree_c *PosWorkingTree;

#endif /* WORKINGTREE_H_ */
