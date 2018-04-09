#ifndef _PREDITIVE_TABLE_
#define _PREDITIVE_TABLE_

#include <string.h>
#include <stack>
#include <vector>
#include <iterator>
#include <map>

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


/* 	Pega uma string correspondente a um elemento da tabela, e retorna uma lista
	O ideal é que a lista seja de inteiros, correspondendo ao índice da tabela
	*/
vector<string> splite(string text){
	vector<string> parts;
	parts.push_back(text);
	
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

void translate(string key){
	vector<string> terminals = {"~","literalhexa","literalint","literalreal","FINAL","literallogico","continue",
		"(",")","*","+",",","-",".","/","id","inteiro","real","logico","texto","vetor","prog","inicio","fim",
		"de","enquanto",":",";","<","=",">","var","rotulo","registro","tipo","const","retorne","pule","faca",

		"[","]","..",":=","value","proc","func","ref","pare","escreva","leia",
		"para","repita","ate","se","entao","senao","caso","seja","or","and","==","!=",">=",
		"<=",,"literaltexto"};
}
/*
NOT = '!',  
	HEXA_VALUE,		
    INT_VALUE,       
    REAL_VALUE, 
	FINAL,
	BOOL_VALUE,
	CONTINUE,
	LPARENT = '(',         
    RPARENT = ')',
    MULT = '*', 
    SUM = '+', 
    COMMA = ',',             
    MINOR = '-',           
    DOT = '.', //ADD NO DOC
    DIVISION = '/',         
    ID,
    INT,
    REAL,
    BOOL,
    STRING,
    VECTOR,      
    PROG,                   
    INIT,              
    END,
    OF,   
    COLON = ':',
    SEMICOMMA = ';',        
    LESS = '<',
    ASSIGN = '=',            
    GREATER = '>',         
    VAR,
    LABEL,           
    STRUCT,         
    TYPE,            
    CONST,           
    RETURN,          
    JUMP,            
    FOR,
    WHILE,              
    DO,              
    REPEAT,          
    UNTIL,            
    IF,              
    THEN,            
    ELSE,            
    CASE,            
    BE,             
    FUNC,            
    PROC,            
    BREAK,                  
    REF,    
    AND,             
    OR,                 
    LESSEQ,          
    GREATEQ,         
    EQUAL,           
    NOTEQ,    
    LBRACKET = '[', 
    STRING_VALUE,
    RBRACKET = ']',                   
    DOUBLEDOT,             
    CASSIGN,
    ERROR
*/


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
	vector<vector<vector<string> > > elements;

	bool firstLine = true;


	while(fstream.good()){

		//Cria a primeira linha
		if(elements.size() == 0) elements.push_back(vector<vector<string> >());

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
	    	elements.push_back(vector<vector<string> >());
	    	
	    	// Garante a Limpeza do campo para ler o próximo campo na nova linha (Pode ser que não seja necessário)
	    	field = "";

	    } else {

	    	// Constrói o campo, byte a byte
	    	field = field + c_read;

	    }
		
	} 

	//Imprime o resultado
	// for(unsigned int i = 0; i <  elements.size(); i++){
	// 	for(unsigned int j = 0; j <  elements[i].size(); j++){
	// 		for(unsigned int k = 0; k <  elements[i][j].size(); k++){
	// 			cout << "[" << elements[i][j][k] << "]";
	// 		}
	// 		cout << '\t';
	// 	}
	// 	cout << endl;
	// }
	
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
