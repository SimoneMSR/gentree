/*
 * error.cpp
 *
 *  Created on: 19/mar/2011
 *      Author: simonemsr
 */

#include "errors.h"
#include <iostream>

using namespace std;

void my_cerr(int error) {
	switch (error) {
	case DS_FN:
		cerr << DS_FN_MSG;
		break;
	case DS_FRMT:
		cerr << DS_FRMT_MSG;
		break;
	case TP_FN:
		cerr << TP_FN_MSG;
		break;
	case TP_FRMT:
		cerr << TP_FRMT_MSG;
		break;
	case CD:
		cerr << CD_MSG;
		break;
	case IP:
		cerr << IP_MSG;
		break;
	case VARIANCE:
		cerr << VARIANCE_MSG;
		break;
	case LOGLIKELYHOOD:
		cerr << LOGLIKELYHOOD_MSG;
		break;

	case OR_E2N:
		cerr << OR_E2N_MSG;
		break;
	case OR_Estar:
		cerr << OR_Estar_MSG;
		break;
	case OR_EMISSION:
		cerr << OR_EMISSION_MSG;
		break;
	case OR_PHI:
		cerr << OR_PHI_MSG;
		break;
	case OR_A:
		cerr << OR_A_MSG;
		break;
	case OR_PL:
		cerr << OR_PL_MSG;
		break;
	case OR_Astar:
		cerr << OR_Astar_MSG;
		break;
	}
	cerr << "\n";
}
