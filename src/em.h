/*
 * em.h
 *
 *  Created on: 12/nov/2010
 *      Author: simonemsr
 */

#ifndef EM_H_
#define EM_H_


#include "WorkingTree.h"
#include "Dataset.h"

/*
 * Compute the n-th step for GTMsd EM learning algorithm. Return a nonzero value if some training error occurred, 0 otherwise
 */
int em_step(Dataset, GTMsd,int,double*);

/*
 * Update some GTM values in a EM algorithm step
 */
void update(GTMsd);

/*
 * Compute the upwards phase for EM algorithm
 */
void upwards(P_GTMsd, WorkingTree);

/*
 * Compute the downwards phase for EM algorithm
 */
void downwards(GTMsd, WorkingTree);



#endif /* EM_H_ */
