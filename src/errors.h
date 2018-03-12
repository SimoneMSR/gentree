/*
 * errors.h
 *
 *  Created on: 19/mar/2011
 *      Author: simonemsr
 */

/*
 *
 * DEFINITION OF ERROR VALUES RETURNED BY PROJECT ROUTINES AND A ERROR REPORTING FUNCTION
 *
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#define DS_FN 1
#define DS_FN_MSG "dataset loading error:error reading file"
#define DS_FRMT 2
#define DS_FRMT_MSG "dataset loading error:incorrect format"
#define TP_FN 3
#define TP_FN_MSG "pamater loading error:errore reading file"
#define TP_FRMT 4
#define TP_FRMT_MSG "pamater loading error:incorrect format"
#define CD 5
#define CD_MSG "wight matrix W not positive definited:using SVD decomposition"
#define IP 6
#define IP_MSG "Invalid train parameters. Exiting."
#define VARIANCE 7
#define VARIANCE_MSG "Variance is less than its limit "
#define LOGLIKELYHOOD 8
#define LOGLIKELYHOOD_MSG "Loglikelyhood increase rate is too small. Forcing training stop."


#define OR_E2N 101
#define OR_E2N_MSG "E2nor parameter is out of range: normalization"
#define OR_Estar 102
#define OR_Estar_MSG "Estar parameter is out of range: normalization"
#define OR_EMISSION 103
#define OR_EMISSION_MSG "Emission value out of range: normalization"
#define OR_PHI 104
#define OR_PHI_MSG "Switching parent value out of range: normalization"
#define OR_A 105
#define OR_A_MSG "Transition element value out of range: normalization"
#define OR_PL 106
#define OR_PL_MSG "Prior for leaves out of range: normalization"
#define OR_Astar 107
#define OR_Astar_MSG "Ghost transition element value out of range: normalization"
void my_cerr(int);

#endif /* ERRORS_H_ */
