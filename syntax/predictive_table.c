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

typedef vector<int> list;
typedef vector<vector<list> > matrix; 



NONTERMINALS nonterminals;

bool is_valid_access(int non_terminal, int terminal) {
	if(true)
		return true;
}

void init_table(matrix &mtx) {
	int n_non_terminals = EXPRESSIONLIST_ - PROGRAM_ + 1;
	int n_terminals = ERROR - NOT;
	mtx = matrix(n_non_terminals, vector<list>(n_terminals, vector<int>()));	
}


/*Recebe uma palavra chave e retorna o valor do ENUM correpondente*/
int translate(string key){

	int size_terminals = ERROR - NOT + 1;

	int size_non_terminals = ATOMICLISTAUX_ - PROGRAM_ + 1;

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
	"type", "literal", "atomic", "id", "idaux", "atomiclist", "atomiclistaux", };

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
vector<int> splite(string text) {

	vector<int> parts;

	/*Se é um espaço vazio da tabela*/
	if (text.size() == 0) {
		/*Não faz nada, a lista será vazia*/

	} else if(text[0]=='\'') {
		/*Se começar com aspas simples, tratará de um terminal*/
		parts.push_back(translate(text));
	} else {

		/*Irá iterá em cada elemento da regra a fim de separar cada regra como um inteiro*/
		unsigned int st = 0;

		/*Retira a parte esquerda da regra*/
		while (st+1 < text.size() && text[st] != '-' && text[st+1] != '>') st++;

		st+=3; /*Retira o '-> '*/
		
		string t; /*Irá armazenar o conteúdo char a char, que depois será convertido em inteiro*/

		/*Percorre o lado direito da regra*/
		for (unsigned int i = st; i < text.size(); i++) {

			switch(text[i]){

				/*Se o char for espaço*/
				case ' ':
					
					/*Adiciona o t contrído a lista convertendo-o para o ENUM correspondente*/
					parts.push_back(translate(t));
					t = ""; /*Reinicia t para construir os novos valores*/
					break;

				default:
					t+=text[i];

			}
		}

		/*Adiciona o último t a lista*/
		if(t.size() > 0) parts.push_back(translate(t));
	}

	return parts;
}

// void printTable(map<int, map<int, vector<int> > > map_elements){

// 	for (map<int, map<int, vector<int> > >::iterator it=map_elements.begin(); it!=map_elements.end(); ++it) {
//     	for (map<int, vector<int> >::iterator it2=it->second.begin(); it2!=it->second.end(); ++it2) {
//     		vector<int> v = it2->second;
//     		if(v.size() == 0) {
//     			cout << '\t';
// 				continue;
// 			}
// 			cout << "[";
// 			for(unsigned int i = 0; i <  v.size(); i++) {
// 				cout << v[i];
// 				if(i+1 < v.size()) 
// 					cout << ", ";
// 			}
// 				cout << "]";
// 			cout << '\t';
// 		}
// 		cout << endl;
// 	}
// }


// void printTable(vector<vector<vector<int> > > elements) {

// 	/*Imprime o resultado*/
// 	for(unsigned int i = 0; i <  elements.size(); i++) {
// 		for(unsigned int j = 0; j <  elements[i].size(); j++) {
// 			if(elements[i][j].size() == 0) {
// 				cout << '\t';
// 				continue;
// 			}
// 			cout << "[";
// 			for(unsigned int k = 0; k <  elements[i][j].size(); k++) {
// 				cout << elements[i][j][k];
// 				if(k+1 < elements[i][j].size()) 
// 					cout << ", ";
// 			}
// 			cout << "]";
// 			cout << '\t';
// 		}
// 		cout << endl;
// 	}
// }

/* 	Salvei em XLS
	Exportei para csv modificando as tabulações para '\t'
	Com isso não há coflito com o ';' ou com ','
	*/
void readMatrix(const char* file_name) {

	//Buffer de leitura do arquivo
	ifstream fstream;
	fstream.open(file_name, ifstream::in);

	//variável auxiliar para armazenar os campos da tabela
	string field;

	map<int, map<int, vector<int> > > map_elements;

	vector<int> term_idx;

	// bool firstLine = true;
	int i = 0;
	int j = 0;

	while (fstream.good()) {

		/*Ler um caractere do arquivo*/
		char c_read;
		c_read = fstream.get();

		/*Se for uma tabulação (delimitador de campo)*/
	    if ( c_read == '\t' ) {


	    	if (i==0) {
	    		term_idx.push_back(translate(field));
	    	} else {
	    		map_elements[i-1+PROGRAM_][term_idx[j]] = splite(field);
	    	}

	    	j++;

	    	/*Limpa para ler o próximo campo*/
	    	field = "";

	    /*Se for final de linha*/
	    } else if( c_read == '\n') {
	    	i++;
	    	j = 0;

	    	/*Garante a Limpeza do campo para ler o próximo campo na nova linha*/
	    	field = "";

	    } else {

	    	/*Constrói o campo, byte a byte*/
	    	field = field + c_read;
	    }
	}

	// printTable(map_elements);

	vector<int> v = map_elements[PROGRAM_][PROG];
	cout << "(" << PROGRAM_ << ", " << PROG << ") = " << v[0] << endl;
	
	fstream.close();
}

void runTable(){
	stack<int> stack;
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
