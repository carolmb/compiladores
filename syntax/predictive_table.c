#ifndef _PREDITIVE_TABLE_
#define _PREDITIVE_TABLE_

// #include <string.h>
#include <string>
#include <stack>
#include <vector>
#include <iterator>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

string terminals[] = {
"~", "literalhexa", "literalint", "literalreal", "FINAL", "literallogico", "continue", 
"(", ")", "*", "+", ",", "-", ".", "/", "id", "inteiro", "real", "logico", "texto", "vetor", 
"prog", "inicio", "fim", "de", ":", ";", "<", "=", ">", "var", "rotulo", "registro", "tipo", 
"const", "retorne", "pule", "para", "enquanto", "faca", "repita", "ate", "se", "entao", 
"senao", "caso", "seja", "func", "proc", "pare", "ref", "and", "or", "<=", ">=", "==", "!=", 
"[", "literaltexto", "]", "..", ":=", "escreva", "leia"};

string non_terminals[] =  {"program", "block", "prevdec", "declaration", "arraydec", "arraydecaux", 
"rangelist", "rangelistaux", "range", "vardec", "varconstruction", "decwithassign", "usertype", 
"typedec", "typedecauxrange", "typedecaux", "vardeclist", "vardeclistaux", "labeldec", "constdec", "abstractiondec", 
"procdec", "funcdec", "parameters", "paramsaux", "paramslist", "prevcommand", "callcommand", 
"commands", "commandsaux", "callidbegin", "calllabel", "write", "read", "return", "loop", "forloop", 
"forstruct", "prevfor", "varassignlist", "varassignlistaux", "posfor", "posforaux", "posforaux2", 
"whileloop", "repeatloop", "conditional", "ifcond", "ifcondaux", "casecond", "casecondaux", 
"caselist", "caselistaux", "caselistaux2", "caseclause", "expressionlist", "expressionlistaux", 
"expr", "orfact", "andfact", "andfactaux", "notfact", "expreq", "expreqaux", "numericexpr", "exprsum", 
"exprmul", "exprmulaux", "simpleexpr", "optrange", "optunary", "optbracket", "idlist", "idlistaux", 
"type", "literal", "atomic", "id", "idaux", "atomiclist", "atomiclistaux"};

typedef vector<int> list;

NONTERMINALS nonterminals;

bool is_valid_access(int non_terminal, int terminal, map<int, map<int, vector<int> > > mtx) {
	//nonterminal, terminal
	if(is_terminal(terminal)) {
		if(mtx[non_terminal][terminal].size() == 0) {
			return false;
		} else {
			return true;
		}
	}
	return false;
}

string to_print(int a) {
	if(a == 0) {
		return "lambda";
	}
	if(a < NOT) {
		return "0";
	}
	if(a > ATOMICLISTAUX_) {
		return "-1";
	}
	if(is_terminal(a)) {
		return terminals[a-NOT] + '(' + 't' + ')';
	} else {
		return non_terminals[a-ERROR-1] + '(' +'n' + ')';
	}
}

/*Recebe uma palavra chave e retorna o valor do ENUM correpondente*/
int translate(string key){

	int size_terminals = ERROR - NOT + 1;

	int size_non_terminals = ATOMICLISTAUX_ - PROGRAM_ + 1;

	/*Verifica terminais*/
	for (int i = 0; i < size_terminals; i++) {
		if (key.compare("'"+terminals[i]+"'") == 0) {
			return i + NOT;
		} else if (key.compare("''") == 0){
			return 0;
		}
	}

	/*Verifica não terminais*/
	for (int i = 0; i < size_non_terminals; i++) {
		if (key.compare(non_terminals[i]) == 0) {
			return i + PROGRAM_;
		}
	}

	/*Caso não encontrado*/
	return -1;
}

void get_predict(string text, unsigned &init_id, list &rules) {
	cout << endl << text << endl;
	string field = "";
	unsigned i = init_id;
	int element;
	cout << " predict: ";
	while (i < text.size()) {
		char c_read = text[i];
		i++;
	    if (c_read == ' ') {
    		element = translate(field);
			rules.push_back(element);
	    	cout << to_print(element) << " ";
	    	field = "";
	    } else {
			/*Constrói o campo, byte a byte*/
	    	field = field + c_read;
	    }
	}
	cout << endl;
}

/* 	Pega uma string correspondente a um elemento da tabela, e retorna uma lista de inteiros, 
	correspondendo ao índice da tabela
*/
void get_one_predict_set(map<int, map<list, list> > &rules, string text) {
	
	/*Se é um espaço vazio da tabela*/
	if (text.size() == 0) {
		return;
	} else {
		/*Irá iterá em cada elemento da regra a fim de separar cada regra como um inteiro*/
		unsigned int st = 0;
		string left_string = "";
		/*Retira a parte esquerda da regra*/
		while (st+1 < text.size() && text[st] != ' ') {
			left_string += text[st];
			st++;
		}
		st+=5; /*Retira o '-> '*/
		
		string t; /*Irá armazenar o conteúdo char a char, que depois será convertido em inteiro*/
		int key;
		int left_side = translate(left_string);
		//cout << to_print(left_side) << " -> ";
		list right_side;
		list predict = list();
		/*Percorre o lado direito da regra*/
		for (unsigned int i = st; i < text.size(); i++) {
			if(text[i] == ' ') {
				key = translate(t);
				right_side.push_back(key);
				//cout << to_print(key) << " ";
				t = ""; /*Reinicia t para construir os novos valores*/
			} else if(text[i] == '\t') {
				i++;
				get_predict(text, i, predict);
				break;
			} else {
				t+=text[i];
			}
		}
		//cout << endl;

		rules[left_side][right_side] = predict;
	}
}

bool has_lambda(list a_first_set) {
	for(list::iterator it = a_first_set.begin(); it != a_first_set.end(); it++) {
		if(*it == 0) {
			return true;
		}
	}
	return false;
}

void print_table(map<int, map<int, list > > &mtx) {
	for(map<int, map<int, list > >::iterator it = mtx.begin(); it != mtx.end(); it++) {
		int left = it->first;
		map<int, list > rights = it->second;
		for(map<int, list >::iterator right = rights.begin(); right != rights.end(); right++) {
			int terminal = right->first;
			cout << "[" << to_print(left) << ", " << to_print(terminal) << "]: ";
			for(list::iterator el = right->second.begin(); el != right->second.end(); el++) {
				cout << to_print(*el) << " ";
			}
			cout << endl;
		}
	}
}

void get_predict_sets(const char* file_name, map<int, map<list, list> > &predict_set) {
	//Buffer de leitura do arquivo
	ifstream fstream;
	fstream.open(file_name, ifstream::in);

	//variável auxiliar para armazenar os campos da tabela
	string field;
	
	while (fstream.good()) {

		/*Ler um caractere do arquivo*/
		char c_read;
		c_read = fstream.get();

		if( c_read == '\n') {
	    	get_one_predict_set(predict_set, field);
	    	field = "";
	    } else {
	    	/*Constrói o campo, byte a byte*/
	    	field = field + c_read;
	    }
	}
	fstream.close();
}

void init_table(map<int, map<int, list > > &mtx) {
	map<int, map<list, list> > predict_set;
	get_predict_sets("syntax/predict_set.csv", predict_set);
	
	for(map<int, map<list, list> >::iterator it = predict_set.begin(); it != predict_set.end(); it++) {
		int left_side = it->first;
		map<list, list> right_side = it->second;
		for(map<list,list>::iterator it2 = right_side.begin(); it2 != right_side.end(); it2++) {
			list right = it2->first;
			list predict = it2->second;
			for(list::iterator p = predict.begin(); p != predict.end(); p++) {
				if(is_terminal(*p)) {
					mtx[left_side][*p] = right;
				}
			}
		}
	}
}

void runTable(){
	stack<int> stack;
	
	map<int, map<int, list > > mtx;
	init_table(mtx);
	//print_table(mtx);
	//return;

	Token *t = getToken(); // ip 
	stack.push(PROGRAM_);
	while(!stack.empty()) {
		int top = stack.top(); // o que deveria ser encontrado no arquivo
		int a = t->key; // o que está sendo lido do arquivo
		if(is_terminal(top) || top == FINAL) {
			cout << "Is all terminal" << endl;
			cout << "Current top: " << to_print(top) << "; current a: " << to_print(a) << endl;
		
			if(top == 0) {
				stack.pop();
			} else if(top == a) {
				stack.pop();
				t = getToken();
			} else {
				// error
				cout << "Invalid syntax: line " << t->line << " expected " << to_print(top) << " instead of " << to_print(a) << endl;
				return;
			} 
		} else {
			cout << "Is nonterminal " << to_print(top) << " " << to_print(a) <<  endl;
			if(is_valid_access(top, a, mtx)) {
				cout << "Is valid access: top is " << to_print(top) << " and a is " << to_print(a) << endl;
				stack.pop();
				
				list predict = mtx[top][a];
				for(list::iterator it = predict.end()-1; it >= predict.begin(); it--) {
					cout << to_print(*it) << " ";
					stack.push(*it);
				}
				cout << endl;
			} else {
				// error
				cout << "Invalid syntax: line " << t->line << " expected " << to_print(top) << " instead of " << to_print(a) << endl;
				return;
			}
		}

		// printf("( %s, %d, %d ) \n", t->value, t->line, t->column );  
		// t = getToken();
	}
}

#endif
