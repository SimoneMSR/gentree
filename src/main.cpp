/*
 * main.cpp
 *
 *  Created on: 18/mar/2011
 *      Author: simonemsr
 */
#include "gui.h"
#include <iostream>

#define USAGE "usage:\n\n./command dataset_filename dir_path_filename [-g gtm_output_filename] [-p parameter_filename] -C hidden_states -P RBF_dimension -L lambda -S stationary -E eras -V verbose_level\n\n"

int command_line_parser(int argc, char **argv) {
	if (argc < 5)
		return -1;
	if (strcmp(argv[3], "-g")==0) {
		if (argc>5&&(strcmp(argv[5], "-p")==0))
			return argc != 7;
		return argc != 17;
	} else {
		if (strcmp(argv[3], "-p")==0)
			return argc != 5;
		return argc != 15;
	}
}

int main(int argc, char **argv) {
	if (argc == 1)
		//interactive mode
		return gui(argc, argv);
	if (command_line_parser(argc, argv) != 0){
		cerr << USAGE;
		return 1;
	}
	switch (argc){
	case 7 :{
		return train_gtm(argv[1],argv[4],argv[6]);
	}break;
	case 15:{
		return train_gtm(atoi(argv[4]),atoi(argv[6]),atof(argv[8]),atoi(argv[10]),atoi(argv[12]),atoi(argv[14]),argv[1]);
	}break;
	case 17 : {
		return train_gtm(atoi(argv[6]),atoi(argv[8]),atof(argv[10]),atoi(argv[12]),atoi(argv[14]),atoi(argv[16]),argv[1],argv[4]);
	}break;
	}
}
