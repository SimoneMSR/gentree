/*
 * I_O.h
 *
 *  Created on: 21/nov/2010
 *      Author: simonemsr
 */

#ifndef I_O_H_
#define I_O_H_
#include "GTMsd.h"
#include "sstream"
#include <png.h>
#define ERR_OPEN -1
#define ERR_FORMAT -2
#define ERR_FILE -3
#define LINE 1024
#define DSIZE 16
#define FORMAT "format=nodenumber,nodelabel,links,label"
#define FORMAT_L 39
#define F_IN "indegree="
#define F_IN_L 9
#define F_OUT "outdegree="
#define F_OUT_L 10
#define F_DIM "dim_label="
#define F_DIM_L 10
#define F_NUM "tree_num="
#define F_NUM_L 9
#define GRAP "graph:"
#define SEPARATOR '2'

using namespace std;

/*
 * Load a dataset from a file. Store the dataset, dimension of labels and maximum arity. Return a negative value if an i/o or format error occured, 0 otherwise.
 */
int load_dataset(const char *filename, Dataset *d,int*,int*);

/*
 * Save a dataset in a file
 */
void save_dataset(int D,int L, Dataset data, char* filename);

/*
 * Reads from a file  a list of tree names, search them in the Dataset and fills num with their ids. Return 0 if no i/o or format errors occured,-1 otherwise.
 */
int select_trees_file(Dataset d, int *num, int**sel,const char *filename);

/*
 * Reads from a string a list of tree names, search them in the Dataset and fills num with their ids. Return 0 if no format errors occured,-1 otherwise.
 */
int select_trees_string(Dataset d, int*num,int**sel, const char *trees);

/*
 * Load a png image from file and return its raster values.
 */
double *read_png(std::string filename, int *width, int *height, int *depth);

/*
 * Load from file the user train parameter. Return an error code if no i/o or format errors occured, 1 otherwise.
 */
int read_train_parameters(char *filename, int *C, int*P,double*lambda, int*eras,
		int*stationary,int*verbose_level);

#endif /* I_O_H_ */
