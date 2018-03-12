using namespace std;
#include "Dataset.h"
#include "GTMsd.h"
#include "I_O.h"
#include "interface.h"
#include "iostream"
#include "em.h"
#include "errors.h"
#include <time.h>

extern int VERBOSE_LEVEL;


int train_gtm(int c, int p, double lambda, bool stationary, int eras,
		int verbosity, P_GTMsd *gtm, Dataset *data, Interface *gui,
		const char *dataset_filename) {
	GTMsd training_gtm;
	int retval, D, L, era;
	ofstream file;
	clock_t start, stop;
	VERBOSE_LEVEL = verbosity;
	if (c <= 0 || p <= 0 || lambda <= 0 || eras <= 0 || not is_square(c)
			|| not is_square(p))
		//train parameters are incorrect
		return IP;

	start = clock();
	double cloc;
	double *loglik = new double[eras];
	retval = load_dataset(dataset_filename, data, &L, &D);
	if (retval)
		return retval;

	training_gtm = new GTMsd_c(*data, stationary, c, p, lambda, D, L);
	if (VERBOSE_LEVEL)
		cout << "\nTRAINING STARTED\n\n";
	for (era = 0; era < eras; era++) {
		retval = em_step(*data, training_gtm, era, loglik);
		if (gui) {
#ifndef GENTREE_STABLE
			char *s = new char[LINE];
			(*gui)->curr_likelyhood = loglik[era];
			sprintf(s, "%i%%\n", 100 * era / eras);
			(*gui)->status_line1->set_name(s);
			sprintf(s, "logL: %.4f", loglik[era]);
			(*gui)->status_line2->set_name(s);
			sprintf(s, "variance: %.4f", training_gtm->var);
			(*gui)->status_line3->set_name(s);
#endif
		}
		if (VERBOSE_LEVEL)
			cout << "era " << era << " time: " << ((double) (clock() - start)
					/ CLOCKS_PER_SEC) << " LogL: " << loglik[era]
					<< " variance: " << training_gtm->var << "\n\n";
		if (retval)
			break;
	}
	stop = clock();
	(training_gtm)->learning_period = ((double) (stop - start))
			/ CLOCKS_PER_SEC;
	if (verbosity) {
		cout << "train period: " << (training_gtm)->learning_period << "sec\n";
		cout << "\t\t\t\t\tEND\n";
	}
	cloc = (double) stop;
	*gtm = training_gtm->convert();
	return retval;
}


int train_gtm(P_GTMsd *gtm, Dataset *data, const char *dataset_filename,
		char *parameters_filename) {
	int C, P, eras, stationary, retval, verbose_level;
	double lambda;
	retval = read_train_parameters(parameters_filename, &C, &P, &lambda, &eras,
			&stationary, &verbose_level);
	if (retval) {
		return retval;
	}
	return train_gtm(C, P, lambda, stationary, eras, verbose_level, gtm, data,
			NULL, dataset_filename);
}


int train_gtm(int c, int p, double lambda, bool stationary, int epocs,
		int verbosity, const char *dataset_filename) {
	P_GTMsd gtm;
	Dataset data;
	int retval;
	retval = train_gtm(c, p, lambda, stationary, epocs, verbosity, &gtm, &data,
			NULL, dataset_filename);
	if (!retval) {
		data->~Dataset_c();
		gtm->~P_GTMsd_c();
	} else if (VERBOSE_LEVEL)
		my_cerr(retval);
	return retval;
}


int train_gtm(const char*dataset_filename, char*parameters_filename) {
	P_GTMsd gtm;
	Dataset data;
	int retval;
	retval = train_gtm(&gtm, &data, dataset_filename, parameters_filename);
	if (!retval) {
		data->~Dataset_c();
		((GTMsd) gtm)->~GTMsd_c();
	} else
		my_cerr(retval);
	return retval;
}


int train_gtm(int c, int p, double lambda, bool stationary, int epocs,
		int verbosity, const char *dataset_filename, char*gtm_save_filename) {
	P_GTMsd gtm;
	Dataset data;
	int retval;
	retval = train_gtm(c, p, lambda, stationary, epocs, verbosity, &gtm, &data,
			NULL, dataset_filename);
	if (!retval) {
		retval = gtm->save(gtm_save_filename);
		data->~Dataset_c();
		gtm->~P_GTMsd_c();
	} else if (VERBOSE_LEVEL)
		my_cerr(retval);
	return retval;

}


int train_gtm(const char*dataset_filename, char*gtm_save_filename,
		char*parameters_filename) {
	P_GTMsd gtm;
	Dataset data;
	int retval;
	retval = train_gtm(&gtm, &data, dataset_filename, parameters_filename);
	if (!retval) {
		retval = gtm->save(gtm_save_filename);
		data->~Dataset_c();
		((GTMsd) gtm)->~GTMsd_c();
	} else
		my_cerr(retval);
	return retval;
}
