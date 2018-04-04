#ifndef _PREDITIVE_TABLE_
#define _PREDITIVE_TABLE_

#include <string.h>
#include <stack>
#include <vector>

typedef std::vector<std::vector<std::vector<int> > > matrix; 

NONTERMINALS nonterminals;

void runTable(){
	std::stack<int> stack;
	matrix mtx;
	
	Token *t = getToken();
	while(t->key != FINAL) {
		printf("( %s, %d, %d ) \n", t->value, t->line, t->column );  
		//if(X >= )
		t = getToken();
	}
}

#endif