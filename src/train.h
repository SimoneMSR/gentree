/*
 * train.h
 *
 *  Created on: 18/mar/2011
 *      Author: simonemsr
 */

#ifndef TRAIN_H_
#define TRAIN_H_

/*
 * Train procedure used in all user interaction modality.
 * Assign to GTMsd and Dataset parameters the reference on new GTMsd and Dataset objects. Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(int c, int p, double lambda, bool stationary, int epocs,
		int verbosity, P_GTMsd *gtm, Dataset *data, Interface *gui,
		const char *dataset_filename);

/*
 * Train procedure using file for parameters.
 *Assign to GTMsd and Dataset parameters the reference on new GTMsd and Dataset objects.Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(P_GTMsd *gtm, Dataset *data, const char *dataset_filename,
		char *parameters_filename);

/*
 * Train procedure using direct parameters. Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(int c, int p, double lambda, bool stationary, int epocs,
		int verbosity, const char *dataset_filename);

/*
 * Train procedure using file for parameter. Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(const char*dataset_filename, char*paramenters_filename);

/*
 * Train procedure using direct parameters. Save the trained GTM in the specified file. Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(int c, int p, double lambda, bool stationary, int epocs,
		int verbosity, const char *dataset_filename, char*gtm_save_filename);

/*
 * Train procedure using file for parameters. Save the trained GTM in the specified file. Return a nonzero error code if some i/o, format or train parameter error occured, 0 otherwise.
 */
int train_gtm(const char*dataset_filename, char*gtm_save_filename,
		char*paramenters_filename);

#endif /* TRAIN_H_ */
