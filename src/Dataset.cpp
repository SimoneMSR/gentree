/*
 * Dataset.cpp
 *
 *  Created on: 10/nov/2010
 *      Author: simonemsr
 */

using namespace std;

#include "Dataset.h"

Dataset_c::Dataset_c(int i){
	n=i;
	trees= new Tree[i];
}

Dataset_c::~Dataset_c(){
	int i;
	for(i=0;i<n;i++)
		trees[i]->free();
	delete[](trees);
}

int Dataset_c::search_tree(char *name){
	int i=0;
	while(i<n&&(strcmp(trees[i]->name,name)!=0)) i++;
	if(i==n) return -1;
	return i;
}
