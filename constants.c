#include "constants.h"

bool is_terminal(int value){
	if(value == 0) 
		return true;
	
	if(value >= NOT && value <= ERROR)
		return true;
	return false;
}

bool is_non_terminal(int value){
	if(value >= PROGRAM_ && value <= ATOMICLISTAUX_)
		return true;
	return false;
}

int idx(int value){
	return value - NOT;
}
