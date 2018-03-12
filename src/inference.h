/*
 * inference.h
 *
 *  Created on: 27/feb/2011
 *      Author: simonemsr
 */

#ifndef INFERENCE_H_
#define INFERENCE_H_
#include "em.h"

/*
 * Put in the vector parameters the modal projection point, the average projection point, the modal projection latent point, id of the tree, id of roots in the selected datasubset
 */
int project_roots(bool,P_GTMsd gtm, Dataset data,int, int*, int**,int**,int**nodes_ids,int*num_points,double**,
		double**);

/*
 * Put in the vector parameters the modal projection point, the average projection point, the modal projection latent point, id of the tree, id of all nodes in the selected datasubset
 */
int project_all(bool,P_GTMsd gtm, Dataset data,int, int*selection, int **indexes,int **ids,int**nodes_ids,
		int *num_point, double**, double**);

/*
 * Put in the vector parameters the modal projection point, the average projection point, the modal projection latent point, id of the tree, id of noden in the selected datasubset witch are in the specified level
 */
int project_level(bool,P_GTMsd gtm, Dataset data, int livello, int trees,int *selection,
		int**indexes,int** ids, int**nodes_ids,int*n, double**, double**);

#endif /* INFERENCE_H_ */
