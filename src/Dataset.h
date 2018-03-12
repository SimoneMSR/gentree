/*
 * Dataset.h
 *
 *  Created on: 10/nov/2010
 *      Author: simonemsr
 */
#ifndef DATASET_H_
#define DATASET_H_

#include <string.h>
#include "Tree.h"

/*
 * A collection of Trees with a method for tree searching using name
 */
class Dataset_c{

public:
	int n;
	Tree *trees;
	Dataset_c(int i);
	~Dataset_c();
	int search_tree(char *name);
};

typedef Dataset_c *Dataset;



#endif /* DATASET_H_ */
