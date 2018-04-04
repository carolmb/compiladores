#ifndef _PREDITIVE_TABLE_
#define _PREDITIVE_TABLE_

#include <string.h>
#include <stack>
#include <vector>
#include <iterator>

typedef std::vector<int> list;
typedef std::vector<std::vector<list> > matrix; 

NONTERMINALS nonterminals;

bool is_valid_access(int non_terminal, int terminal) {
	if(true)
		return true;
}

void init_table(matrix &mtx) {
	int n_non_terminals = EXPRESSIONLIST_ - PROGRAM_ + 1;
	int n_terminals = ERROR - NOT;
	mtx = matrix(n_non_terminals, std::vector<list>(n_terminals, std::vector<int>()));	
}

void runTable(){
	std::stack<int> stack;
	matrix mtx;
	init_table(mtx);

	Token *t = getToken(); // ip 
	stack.push(PROGRAM_);
	while(!stack.empty()) {
		int top = stack.top();
		int a = t->key;
		if(is_terminal(top) || top == FINAL) {
			if(top == a) {
				stack.pop();
				t = getToken();
			} else {
				// error
			} 
		} else {
			if(is_valid_access(top, a)) {
				list predict = mtx[top][a];
				for(list::iterator it = predict.end(); it != predict.begin(); it--) {
					stack.push(*it);
				}
			}
		}

		printf("( %s, %d, %d ) \n", t->value, t->line, t->column );  
		//if(X >= )
		t = getToken();
	}
}

#endif