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

/* 	Pega uma string correspondente a um elemento da tabela, e retorna uma lista de inteiros, 
	correspondendo ao índice da tabela
*/
void get_rule(map<int, vector<list > > &rules, string text) {
	list parts;

	/*Se é um espaço vazio da tabela*/
	if (text.size() == 0) {
		/*Não faz nada, a lista será vazia*/
		return;
	} else if(text[0]=='\'') {
		/*Se começar com aspas simples, tratará de um terminal*/
		cout << "DEVERIA ENTRAR AQUI?" << endl;
		parts.push_back(translate(text));
	} else {
		cout << text << endl;
		/*Irá iterá em cada elemento da regra a fim de separar cada regra como um inteiro*/
		unsigned int st = 0;

		string no_terminal = "";
		/*Retira a parte esquerda da regra*/
		while (st+1 < text.size() && text[st] != ' ') {
			no_terminal += text[st];
			st++;
		}

		st+=4; /*Retira o '-> '*/
		
		string t; /*Irá armazenar o conteúdo char a char, que depois será convertido em inteiro*/
		int key;
		/*Percorre o lado direito da regra*/
		for (unsigned int i = st; i < text.size(); i++) {

			switch(text[i]){

				/*Se o char for espaço*/
				case ' ':
					
					/*Adiciona o t contrído a lista convertendo-o para o ENUM correspondente*/
					key = translate(t);
					parts.push_back(key);
					//cout << key << " " << t << endl;
					t = ""; /*Reinicia t para construir os novos valores*/
					break;

				default:
					t+=text[i];

			}
		}

		/*Adiciona o último t a lista*/
		if(t.size() > 0) parts.push_back(translate(t));
		vector<list> right = rules[translate(no_terminal)];
		// for(list::iterator it = parts.begin(); it != parts.end(); it++)
		// 	cout << *it << " ";
		// cout << endl;
		right.push_back(parts);
		rules[translate(no_terminal)] = right;
	}
}

/* 	Salvei em XLS
	Exportei para csv modificando as tabulações para '\t'
	Com isso não há coflito com o ';' ou com ','
	*/
void init_rules(const char* file_name, map<int, vector<list > > &rules) {

	//Buffer de leitura do arquivo
	ifstream fstream;
	fstream.open(file_name, ifstream::in);

	//variável auxiliar para armazenar os campos da tabela
	string field;
	
	//vector<int> term_idx;

	// bool firstLine = true;
	int i = 0;
	
	while (fstream.good()) {

		/*Ler um caractere do arquivo*/
		char c_read;
		c_read = fstream.get();

		/*Se for uma tabulação (delimitador de campo)*/
	    if ( c_read == '\t' ) {


	    	if (i == 0) {
	    		//term_idx.push_back(translate(field));
	    	} else {
	    		get_rule(rules, field);
	    	}

	    	/*Limpa para ler o próximo campo*/
	    	field = "";

	    /*Se for final de linha*/
	    } else if( c_read == '\n') {
	    	i++;
	    	/*Garante a Limpeza do campo para ler o próximo campo na nova linha*/
	    	field = "";

	    } else {

	    	/*Constrói o campo, byte a byte*/
	    	field = field + c_read;
	    }
	}

	fstream.close();
}

void init_sets(const char* file_name, map<int, list> &sets) {
	cout << "init_sets " << file_name << endl;
	ifstream fstream;
	fstream.open(file_name, ifstream::in);

	string field = "";
	int i = 0;
	int key, element;

	while (fstream.good()) {
		char c_read;
		c_read = fstream.get();

	    if ( c_read == ' ' ) {

	    	if (i == 0) {
	    		// lê terminal
	    		key = translate(field);
	    		sets[key] = list();
	    	} else {
	    		// lê elemento do conjunto
	    		element = translate(field);
	    		sets[key].push_back(element);
	    	}
			field = "";
	    	i++;
	    } else if( c_read == '\n') {
	    	i = 0;
			field = "";
	    } else {
			/*Constrói o campo, byte a byte*/
	    	field = field + c_read;
	    }
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

void print_sets(map<int, list> sets) {
	for(map<int, list>::iterator it = sets.begin(); it != sets.end(); it++) {
		cout << "set of " << to_print(it->first) << ": ";
		for(list::iterator el = it->second.begin(); el != it->second.end(); el++) {
			cout << to_print(*el) << " ";
		}
		cout << endl;
	}
}

void init_table(map<int, map<int, list > > &mtx) {
	map<int, list> first_set, follow_set;
	init_sets("syntax/first_set.csv", first_set);
	init_sets("syntax/follow_set.csv", follow_set);
	
	map<int, vector<list> > rules;
	init_rules("syntax/spt.csv", rules);
	
	for (map<int,vector<list> >::iterator it = rules.begin(); it != rules.end(); it++) {
		int left = it->first; // lado esquerdo da regra (não terminal)
		vector<list> right_sides = it->second; // todos os lados direitos possíveis
		
		for(vector<list>::iterator right = right_sides.begin(); right != right_sides.end(); right++) {
			// um lado direito possível (é uma lista de inteiros)
			for(list::iterator a = right->begin(); a != right->end(); a++) {
				list a_first_set = first_set[*a];
				for(list::iterator t = a_first_set.begin(); t != a_first_set.end(); t++) {
					if(is_terminal(*t))
						mtx[left][*t] = *right;	
				}
				if(has_lambda(a_first_set)) {
					list a_follow_set = follow_set[*a];
					for(list::iterator t = a_follow_set.begin(); t != a_follow_set.end(); t++) {
						if(is_terminal(*t)) {
							mtx[left][*t] = *right;
						}
					}
				}
			}
		}
	}
}

void runTable(){
	stack<int> stack;
	
	map<int, map<int, list > > mtx;
	init_table(mtx);
	print_table(mtx);
	return;
	

	Token *t = getToken(); // ip 
	stack.push(PROGRAM_);
	//cout << "PROGRAM_ " << PROGRAM_ << " " << translate("program") << endl;
	while(!stack.empty()) {
		int top = stack.top(); // o que deveria ser encontrado no arquivo
		int a = t->key; // o que está sendo lido do arquivo
		if(is_terminal(top) || top == FINAL) {
			cout << "Is all terminal" << endl;
			cout << "Current top: " << to_print(top) << "; current a: " << to_print(a) << endl;
		
			if(top == a) {
				stack.pop();
				t = getToken();
			} else {
				// error
				cout << "Invalid syntax: line " << t->line << " expected " << to_print(top) << " instead of " << to_print(a) << endl;
				return;
			} 
		} else {
			cout << "Is nonterminal" << endl;
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
