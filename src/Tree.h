/*
 * Tree.h
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */
#ifndef TREE_H_
#define TREE_H_

using namespace std;
#include "BLAS.h"
#include <list>

/*
 *
 * The Tree class. A Tree T=(G,E) is a DAG with fixed arity. A Tree_c object
 * contains structure about: tree's name,number of nodes, arity, tree's label, node's children,
 * node's parent and relative position,node's level and node's labels.
 *
 */
class Tree_c {

protected:

	int **children; //parent-children matrix

public:
	int arity; //fixed arity (maximum number of children of a node)
	int **parents; //nodes parent and their relative positions
	int *levels; //each node's levels
	double **labels; //nodes' labels
	int label;//tree's label
	int nodes;//number of nodes
	char *name;//tree's name

	/*
	 * Create a new empty tree, without nodes and edges. Only number of nodes, maximum outdegree and dimension of node labels are fixed
	 */
	Tree_c(int nodes, int arity, int dimlabel);

	/*
	 * Create a new tree as a copy of the specified tree
	 */
	Tree_c(Tree_c *t);

	Tree_c() {
	}
	;

	/*
	 * Free memory allocated for the tree
	 */
	void free();

	/*
	 * Return the l-th child of u-th node
	 */
	int get_child(int parent, int l);

	/*
	 * Return the d-th filed of node label
	 */
	double get_label_field(int node, int d);

	/*
	 * Set the d-th filed of node label
	 */
	void set_label_field(int node, int d, double value);

	/*
	 * Returns number of nodes in the tree
	 */
	int get_nodes();

	/*
	 * Return true if u-th node is a leaf, false instead
	 */
	bool is_leaf(int u);

	/*
	 * Define a new edge between two nodes.
	 */
	void set_child(int parent, int l, int child);

	/*
	 * Apply the topologic sorting to the tree
	 */
	void topologic_sort(int L, int D);

	/*
	 *Set the level of each node in the tree
	 */
	int set_levels(int node);

	/*
	 * Return the name of tree
	 */
	const char *get_name();

};

typedef class Tree_c *Tree;

#endif /* TREE_H_ */
