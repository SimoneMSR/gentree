/*
 * em_parameter.h
 *
 *  Created on: 12/nov/2010
 *      Author: simonemsr
 */

/*
 * Set of parameters used in EM algorithm
 */
class EM_parameter_c {
public:
	double *S_E1; //sum of E1 for stationary model
	double **S_E2; //sum of E2 stationary model
	double *S_E0; //sum of E3 stationary model
	double *S_Estar;
	double **ns_S_Estar;
	double **ns_S_E1; //sum of E1 for non stationary model
	double ***ns_S_E2; //sum of E2 for non stationary model
	double **ns_S_E0; //sum of E3 for non stationary model
	double ***ALL_E; //matrix of all E_1(i)

	EM_parameter_c(){};
};

typedef class EM_parameter_c *EM_parameter;
