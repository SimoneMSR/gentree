/*
 * Tree.cpp
 *
 *  Created on: 05/nov/2010
 *      Author: simonemsr
 */

using namespace std;
#include "Tree.h"


Tree_c::Tree_c(int nodes, int arity, int dimlabel) {
	int i;
	this->nodes = nodes;
	parents = new int*[nodes];
	children = new int*[nodes];
	labels = new double*[nodes];
	levels = new int[nodes];
	for (i = 0; i < nodes; i++) {
		parents[i] = new int[2];
		children[i] = new int[arity];
		labels[i] = new double[dimlabel];
	}
	this->arity = arity;
}

Tree_c::Tree_c(Tree t){
	children=t->children;
	arity=t->arity;
	parents=t->parents;
	levels=t->levels;
	labels=t->labels;
	label=t->label;
	name=t->name;
	nodes=t->nodes;
}


void Tree_c::free() {
	free_matrix(parents, nodes);
	free_matrix(children, nodes);
	free_matrix(labels, nodes);
	delete[](levels);

}


int Tree_c::get_child(int u, int l) {
	if(l>=arity)
		return -1;
	return children[u][l];
}



int Tree_c::get_nodes() {
	return nodes;
}


bool Tree_c::is_leaf(int u) {
	int l;
	for (l = 0; l < arity; l++) {
		if (children[u][l] != -1)
			return false;
	}
	return true;
}



void Tree_c::set_child(int parent, int position, int child) {
	children[parent][position] = child;
	if (child >= 0) {
		parents[child][0] = parent;
		parents[child][1] = position;
	}
}


double Tree_c::get_label_field(int node, int d) {
	return labels[node][d];
}


void Tree_c::set_label_field(int node, int d, double value) {
	labels[node][d] = value;
}

void Tree_c::topologic_sort(int L, int D) {
	int *new_nodes = new int[nodes];
	int new_node, old_node, next_child, i, j, **matrix;
	double **labels;
	list<int> *old_nodes;
	for (i = 0; i < nodes; i++) {
		new_nodes[i]=0;
		for (j = 0; j < L; j++) {
			if (get_child(i, j) >= 0)
				new_nodes[get_child(i, j)] = 1;
		}
	}

	i = 0;
	while (new_nodes[i] == 1)
		i++;
	old_nodes = new list<int> ();
	//insert the root
	old_nodes->push_front(i);
	next_child = 1;
	new_nodes[i] = 0;
	//allocate memory for new children matrix
	matrix = new int*[get_nodes()];
	labels = new double*[get_nodes()];
	while (!(old_nodes->empty())) {
		old_node = old_nodes->front();
		old_nodes->pop_front();
		//get the new id of the next node
		new_node = new_nodes[old_node];
		matrix[new_node] = new int[L];
		labels[new_node] = new double[D];
		for (i = 0; i < D; i++)
			labels[new_node][i] = get_label_field(old_node, i);
		for (i = 0; i < L; i++) {
			j = get_child(old_node, i);
			if (j != -1) {
				old_nodes->push_back(j);
				new_nodes[j] = next_child;
				matrix[new_node][i] = next_child;
				parents[next_child][0] = new_node;
				parents[next_child][1] = i;
				next_child++;
			} else
				matrix[new_node][i] = -1;
		}
	}
	free_matrix(children, nodes);
	children = matrix;
	free_matrix(this->labels, nodes);
	this->labels = labels;
	set_levels(0);
	old_nodes->~list<int>();
}


int Tree_c::set_levels(int node) {
	if (is_leaf(node)) {
		levels[node] = 0;
		return 0;
	} else {
		int lev;
		int i = 0;
		levels[node]=0;
		for (i = 0; i < arity; i++) {
			if (children[node][i] != -1)
				lev = set_levels(children[node][i]);
			if (levels[node] < lev+1)
				levels[node] = lev+1;
		}
		return levels[node];
	}
}

const char *Tree_c::get_name(){
	return name;
}
