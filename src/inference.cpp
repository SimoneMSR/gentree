/*
 * inference.cpp
 *
 *  Created on: 27/feb/2011
 *      Author: simonemsr
 */

#include "inference.h"
#include "BLAS.h"

int project_roots(bool refresh, P_GTMsd gtm, Dataset data, int trees,
		int*selection, int **ind, int**ids, int**nodes_ids, int*num_points,
		double **a, double**b) {
	WorkingTree w;
	double val = 0;
	int i, j, count;
	int ii, ii_;
	if (refresh) {
		*a = new double[trees * 2];
		*b = new double[trees * 2];
		*ind = new int[trees];
		*ids = new int[trees];
		*nodes_ids = new int[trees];
		count=0;
		*num_points = trees;
	} else {
		realloc_vector(a, *num_points * 2, (trees + *num_points) * 2);
		realloc_vector(b, *num_points * 2, (trees + *num_points) * 2);
		realloc_vector(ind, *num_points, trees + *num_points);
		realloc_vector(ids, *num_points, trees + *num_points);
		realloc_vector(nodes_ids, *num_points, trees + *num_points);
		count=*num_points;
		*num_points += trees;
	}
	for (i = 0; i < trees; i++) {

		if (gtm->stationary)
			w = new NPosWorkingTree_c(gtm, data->trees[selection[i]]);
		else
			w = new PosWorkingTree_c(gtm, data->trees[selection[i]]);
		val = 0;
		ii_ = 0;
		upwards(gtm, w);
		for (ii = 0; ii < gtm->C; ii++) {
			if (w->Beta1[0][ii] > val) {
				val = w->Beta1[0][ii];
				ii_ = ii;
			}
		}
		(*ind)[count] = ii_;
		(*ids)[count] = selection[i];
		(*nodes_ids)[count] = 0;
		(*b)[2 * count] = gtm->get_X(ii_,0);
		(*b)[2 * count + 1] = gtm->get_X(ii_,1);

		(*a)[2 * count] = 0;
		(*a)[2 * count + 1] = 0;
		for (j = 0; j < gtm->C; j++) {
			(*a)[2 * count] += w->Beta1[0][j] * gtm->get_X(j,0);
			(*a)[2 * count + 1] += w->Beta1[0][j] * gtm->get_X(j,1);
		}
		count++;
		delete (w);

	}

	return 0;

}

int project_all(bool refresh, P_GTMsd gtm, Dataset data, int trees,
		int*selection, int **indexes, int**ids, int**nodes_ids,
		int *num_points, double **a, double **b) {
	WorkingTree w;
	double val = 0;
	int i, j, k, count, num = 0;
	int ii, ii_;
	for (i = 0; i < trees; i++)
		num += data->trees[selection[i]]->nodes;
	if (refresh) {
		*a = new double[num * 2];
		*b = new double[num * 2];
		*indexes = new int[num];
		*ids = new int[num];
		*nodes_ids = new int[num];
		count = 0;
		*num_points = num;
	} else {
		realloc_vector(a, *num_points * 2, (num + *num_points) * 2);
		realloc_vector(b, *num_points * 2, (num + *num_points) * 2);
		realloc_vector(indexes, *num_points, num + *num_points);
		realloc_vector(ids, *num_points, num + *num_points);
		realloc_vector(nodes_ids, *num_points, num + *num_points);
		count = *num_points;
		*num_points += num;
	}

	for (i = 0; i < trees; i++) {
		if (gtm->stationary)
			w = new NPosWorkingTree_c(gtm, data->trees[selection[i]]);
		else
			w = new PosWorkingTree_c(gtm, data->trees[selection[i]]);
		upwards(gtm, w);
		for (j = 0; j < data->trees[selection[i]]->nodes; j++) {
			val = 0;
			ii_ = 0;
			for (ii = 0; ii < gtm->C; ii++) {
				if (w->Beta1[j][ii] > val) {
					val = w->Beta1[j][ii];
					ii_ = ii;
				}
			}
			(*indexes)[count] = ii_;
			(*ids)[count] = selection[i];
			(*nodes_ids)[count] = j;
			(*b)[2 * count] = gtm->get_X(ii_,0);
			(*b)[2 * count + 1] = gtm->get_X(ii_,1);

			(*a)[2 * count] = 0;
			(*a)[2 * count + 1] = 0;
			for (k = 0; k < gtm->C; k++) {
				(*a)[2 * count] += w->Beta1[j][k] * gtm->get_X(k,0);
				(*a)[2 * count + 1] += w->Beta1[j][k] * gtm->get_X(k,1);
			}

			count++;
		}

		delete (w);
	}
	return 0;

}

int project_level(bool refresh, P_GTMsd gtm, Dataset data, int livello,
		int trees, int *selection, int **indexes, int**ids, int**nodes_ids,
		int *num_points, double **a, double**b) {
	WorkingTree w;
	int n, i, ii, ii_, k, num = 0, count = 0;
	double val;
	for (n = 0; n < trees; n++) {
		for (i = 0; i < data->trees[selection[n]]->nodes; i++) {
			if (data->trees[selection[n]]->levels[i] == livello)
				num++;
		}
	}
	if (refresh) {
		*a = new double[num * 2];
		*b = new double[num * 2];
		*indexes = new int[num];
		*ids = new int[num];
		*nodes_ids = new int[num];
		count = 0;
		*num_points = num;
	} else {
		realloc_vector(a, *num_points * 2, (num + *num_points) * 2);
		realloc_vector(b, *num_points * 2, (num + *num_points) * 2);
		realloc_vector(indexes, *num_points, num + *num_points);
		realloc_vector(ids, *num_points, num + *num_points);
		realloc_vector(nodes_ids, *num_points, num + *num_points);
		count = *num_points;
		*num_points += num;
	}

	for (n = 0; n < trees; n++) {
		if (gtm->stationary)
			w = new NPosWorkingTree_c(gtm, data->trees[selection[n]]);
		else
			w = new PosWorkingTree_c(gtm, data->trees[selection[n]]);
		upwards(gtm, w);
		for (i = 0; i < data->trees[selection[n]]->nodes; i++) {
			if (data->trees[selection[n]]->levels[i] == livello) {
				val = 0;
				ii_ = 0;
				for (ii = 0; ii < gtm->C; ii++) {
					if (w->Beta1[i][ii] > val) {
						val = w->Beta1[i][ii];
						ii_ = ii;
					}
				}
				(*indexes)[count] = ii_;
				(*ids)[count] = selection[n];
				(*nodes_ids)[count] = i;
				(*b)[2 * count] = gtm->get_X(ii_,0);
				(*b)[2 * count + 1] = gtm->get_X(ii_,1);

				(*a)[2 * count] = 0;
				(*a)[2 * count + 1] = 0;
				for (k = 0; k < gtm->C; k++) {
					(*a)[2 * count] += w->Beta1[i][k] * gtm->get_X(k,0);
					(*a)[2 * count + 1] += w->Beta1[i][k] * gtm->get_X(k,1);
				}
				count++;
			}
		}
	}
	return 0;
}
