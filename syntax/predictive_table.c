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
	"typedec", "typedecaux", "vardeclist", "vardeclistaux", "labeldec", "constdec", "abstractiondec", 
	"procdec", "funcdec", "parameters", "paramsaux", "paramslist", "prevcommand", "callcommand", 
	"commands", "commandsaux", "callidbegin", "calllabel", "write", "read", "return", "loop", "forloop", 
	"forstruct", "prevfor", "varassignlist", "varassignlistaux", "posfor", "posforaux", "posforaux2", 
	"whileloop", "repeatloop", "conditional", "ifcond", "ifcondaux", "casecond", "casecondaux", 
	"caselist", "caselistaux", "caselistaux2", "caseclause", "expressionlist", "expressionlistaux", 
	"expr", "orfact", "andfact", "andfactaux", "notfact", "expreq", "expreqaux", "numericexpr", "exprsum", 
	"exprmul", "exprmulaux", "simpleexpr", "optrange", "optunary", "optbracket", "idlist", "idlistaux", 
	"type", "literal", "atomic", "id", "idaux", "atomiclist", "atomiclistaux", };


	for(int i = 0; i < size_terminals; i++){
		if(key.compare("'"+terminals[i]+"'") == 0){
			return i + NOT;
		}else if(key.compare("''") == 0){
			return 0;
		}
	}
	for(int i = 0; i < size_non_terminals; i++){
		if(key.compare(non_terminals[i]) == 0){
			return i + PROGRAM_;
		}
	}

	return -1;
}



/* 	Pega uma string correspondente a um elemento da tabela, e retorna uma lista
	O ideal é que a lista seja de inteiros, correspondendo ao índice da tabela
	*/
vector<int> splite(string text){
	vector<int> parts;
	// cout << text << endl;
	if (text.size() == 0){

	}else if(text[0]=='\''){
		parts.push_back(translate(text));
	}else{

		unsigned int st = 0;
		while(st+1 < text.size() && text[st] != '-' && text[st+1] != '>') st++;
		
		string t;
		for(unsigned int i = st+3; i < text.size(); i++){
			switch(text[i]){
				case ' ':
					// cout << t << endl;
					parts.push_back(translate(t));
					t = "";
					break;
				default:
					t+=text[i];

			}
		}
		if(t.size() > 0)
			// cout << t << endl;
			parts.push_back(translate(t));
	}
	

	
		


	// }
	
	// vector<string> parts;
	// string part = "";
	// for(unsigned int i = 0; i < text.size(); i++){

		

	// 	switch(text[i]){
	// 		case ' ':
	// 			parts.push_back(part);
	// 			part = "";
	// 			break;
	// 		case ',':
	// 			return parts;
	// 			break;
	// 		default:
	// 			part = part + text[i];
	// 			break;
	// 	}

	// }
	
	// //Caso em que só o elemento aparece
	// if(part.size() > 0)
	// 	parts.push_back(part);
	return parts;
}

map<string,int> mapToken;



/* 	Salvei em XLS
	Exportei para csv modificando as tabulações para '\t'
	Com isso não há coflito com o ';' ou com ','
	*/
void readMatrix(const char* file_name){

	//Buffer de leitura do arquivo
	ifstream fstream;
	fstream.open(file_name, ifstream::in);

	//variável auxiliar para armazenar os campos da tabela
	string field;

	//Tabela lida do CSV
	vector<vector<vector<int> > > elements;

	bool firstLine = true;


	while(fstream.good()){

		//Cria a primeira linha
		if(elements.size() == 0) elements.push_back(vector<vector<int> >());

		// Ler um caractere do arquivo
		char c_read;
		c_read = fstream.get();

		// Se for uma tabulação (delimitador de campo)
	    if( c_read == '\t' ) {

	    	if(firstLine){
	    		translate(field);
	    	}
	    	
	    	// Insere o novo elemento
	    	elements[elements.size()-1].push_back(splite(field));
	    	
	    	// Limpa para ler o próximo campo
	    	field = "";

	    // Se for final de linha
	    } else if( c_read == '\n') {

	    	firstLine = false;
	    	
	    	//Cria uma nova linha da matriz
	    	elements.push_back(vector<vector<int> >());
	    	
	    	// Garante a Limpeza do campo para ler o próximo campo na nova linha (Pode ser que não seja necessário)
	    	field = "";

	    } else {

	    	// Constrói o campo, byte a byte
	    	field = field + c_read;

	    }
		
	} 

	// Imprime o resultado
	for(unsigned int i = 0; i <  elements.size(); i++){
		for(unsigned int j = 0; j <  elements[i].size(); j++){
			if(elements[i][j].size() == 0) {
				cout << '\t';
				continue;
			}
			cout << "[";
			for(unsigned int k = 0; k <  elements[i][j].size(); k++){
				cout << elements[i][j][k];
				if(k+1 < elements[i][j].size()) 
					cout << ", ";
			}
			cout << "]";
			cout << '\t';
		}
		cout << endl;
	}
	
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
