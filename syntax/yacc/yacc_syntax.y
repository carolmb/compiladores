%{
#include "../../token.c"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include "../../utilities.h"
#include <iostream>
#include <map>

void yyerror (char *s); 
int yylex();
void success();
std::string getType(std::string label);


void yyerrorDuplicateIdentifier (std::string sym_name);
void yyerrorSize (int expSize, int currentSize);
void yyerrorType (std::string expType, std::string currenType);

// SCOPES --------------

struct Scope {
	//Scope* parent
	std::map<std::string, Symbol*> symbolsTable;
	Scope(std::map<std::string, Symbol*> st) : symbolsTable(st) {}
	Scope(){}
};

std::vector<Scope> scopesTable; 

void addScope();
void removeScope();
void initTable();
Symbol* searchElementInTableByLabel(std::string label);
std::string searchElementInTableBySymbol(Symbol *symbol);

// VARS --------------

int vIndex = 0;

void addSymbol(std::string label, Symbol *sym);
void addVarDecList(std::pair<std::vector<std::string>, std::string> vardeclist, bool is_const);
std::string addVec(int size, std::string expectedType, std::vector<std::string> *initTypes);
int rangeSize(TypeValue *first, TypeValue *second);
void addUserType(std::string label, std::string name);
std::string verifyUsertype(std::string label);

%}

%union { 
	std::string *typeName;
	int size;
	Token *token; 
	std::vector<Field> *structFields;
	std::vector<std::string> *list; /*it can be idlist, expressionlist and so on... */
	std::pair<std::vector<std::string>, std::string> *varDec;
	std::vector<Field> *fields;
	TypeValue *var;
}

%start program

%type <varDec> vardec constdec
%type <typeName> type varconstruction arraydec typedecauxrange expr
%type <structFields> vardeclist vardeclistaux
%type <list> idlist idlistaux expressionlist expressionlistaux arraydecaux
%type <fields> paramslist paramsaux parameters
%type <size> rangelistaux rangelist range
%type <var> atomic literal

/*Terminals*/
%token HEXA_VALUE INT_VALUE REAL_VALUE FINAL BOOL_VALUE CONTINUE 
ID INT REAL BOOL STRING VECTOR PROG INIT END OF VAR LABEL STRUCT 
TYPE CONST RETURN JUMP FOR WHILE DO REPEAT UNTIL IF THEN ELSE CASE 
BE FUNC PROC BREAK 
REF AND OR LESSEQ GREATEQ EQUAL NOTEQ DOUBLEDOT 
STRING_VALUE CASSIGN WRITE READ INT REAL BOOL STRING
%%

program  			: PROG ID ';' prevdec block FINAL { success(); }
					;

block  				: INIT prevcommand END {}
					;

prevdec  			: declaration ';' prevdec {}
		  			| {}
					;

declaration  		: { std::cout << "VARDEC" << std::endl; } vardec { addVarDecList(*$2, false); }
			  		| { std::cout << "USERTYPE" << std::endl; } usertype 
			  		| labeldec {}
			  		| { std::cout << "CONST" << std::endl; } constdec { addVarDecList(*$2, true); }
			  		| { std::cout << "ABSTRACTIONDEC" << std::endl; } abstractiondec
					;

arraydec  			: VECTOR '[' rangelist ']' OF type arraydecaux { std::string v = addVec($3, *$6, $7); $$ = new std::string(v); }
					;

arraydecaux  		: '=' '(' expressionlist ')' { $$ = $3; }
			  		| { $$ = nullptr; }
					;

rangelist  			: range rangelistaux { $$ = ($1 + 1)*$2; }
					;

rangelistaux  		: ',' range rangelistaux { $$ = ($2 + 1)*$3; }
			  		| {$$ = 1;}
					;

range  				: atomic DOUBLEDOT atomic { $$ = rangeSize($1, $3); }
					;

vardec  			: VAR idlist ':' varconstruction { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

varconstruction  	: type decwithassign { $$ = $1; }
				  	| arraydec { $$ = $1; }
					;

decwithassign  		: '=' expr {}
			  		| {}
					;

usertype  			: TYPE ID CASSIGN arraydec { addUserType($2.token->value, *$4); }
					| TYPE ID CASSIGN STRUCT vardeclist END {}
					| TYPE ID CASSIGN literal DOUBLEDOT literal { }
					| TYPE ID CASSIGN id typedecauxrange {}
					;

typedecauxrange		: DOUBLEDOT id { $$ = new std::string(""); }
					| { $$ = new std::string(""); }
					;

vardeclist  		: vardec vardeclistaux {}
					;

vardeclistaux  		: ';' vardec vardeclistaux {}
			  		| {}
					;

labeldec  			: LABEL idlist {}
					;

constdec  			: CONST idlist ':' type '=' expr { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: PROC { addScope(); } ID '(' parameters ')' { /* Symbol *ab = new AbstractionSymbol("", *$4); addSymbol($2.token->value, ab);*/ } prevdec block { removeScope(); } 
					;

funcdec  			: FUNC { addScope(); } ID '(' parameters ')' ':' type { /* Symbol *ab = new AbstractionSymbol("", *$4); addSymbol($2.token->value, ab);*/ } prevdec block { removeScope(); }
					;

parameters  		: paramsaux { $$ = $1; }
			  		| { $$ = new std::vector<Field>(); }
					;

paramsaux  			: ID ':' type paramslist { $4->insert($4->begin(), Field($1.token->value, *$3)); $$ = $4; }
		  			| REF ID ':' type paramslist { $5->insert($5->begin(), Field($2.token->value, *$4)); $$ = $5; }
					;

paramslist  		: ',' paramsaux { $$ = $2; }
			  		| { $$ = new std::vector<Field>(); }
					;


prevcommand  		: commands {}
			  		| {}
					;

callcommand  		: BREAK {}		
			  		| write {}
			  		| read {}
			  		| loop {}
			  		| block {}
			  		| return {}
			  		| CONTINUE {}
			  		| id callidbegin {}
			  		| calllabel {}
			  		| conditional {}
					;

commands  			: callcommand commandsaux {}
					;

commandsaux  		: ';' commands {}
			  		| {}
					;

callidbegin  		: ':' callcommand {}
			  		| '=' expr {}
			  		| {}
					;

calllabel  			: JUMP ID
					;

write  				: WRITE '(' expressionlist ')'
					;

read  				: READ '(' expressionlist ')'
					;

return  			: RETURN expr {}
					;

loop  				: forloop {}
	  				| whileloop {}
	  				| repeatloop {}
					;

forloop  			: FOR '(' forstruct ')' DO callcommand 
					;

forstruct  			: prevfor ';' expr ';' posfor {}
					;

prevfor 			: varassignlist {}
		  			| {}
					;

varassignlist  		: ID '=' expr varassignlistaux {}
					;

varassignlistaux  	: ',' ID '=' expr varassignlistaux {}
				  	| {}
					;

posfor  			: posforaux {}
		  			| {}
					;

posforaux  			: commands posforaux2 {}
					;

posforaux2  		: ',' commands posforaux2 {}
			  		| {}
					;

whileloop  			: WHILE '(' expr ')' DO callcommand 
					;

repeatloop  		: REPEAT commands UNTIL expr {}
					;

conditional  		: ifcond {}
			 		| casecond {}
					;

ifcond  			: IF '(' expr ')' THEN callcommand ifcondaux {}
					;

ifcondaux  			: ELSE callcommand 
					| {}
					;
					
					
casecond  			:  CASE '(' expr ')' BE caselist casecondaux 
					;

casecondaux  		: END {}
			  		| ELSE commands END {}
					;

caselist  			: caseclause caselistaux {}
					;

caselistaux  		: ';' caselistaux2 {}
			  		| {}
					;

caselistaux2  		: caseclause caselistaux {}
			  		| {}
					;

caseclause  		: atomiclist ':' callcommand {}
					;

expressionlist  	: expr expressionlistaux { $$ = $2; $$->insert($$->begin(), *$1); }
					;

expressionlistaux  	: ',' expr expressionlistaux { $$ = $3; $$->insert($$->begin(), *$2); }
				  	| { $$ = new std::vector<std::string>(); }
					;
	
expr				: andfact orfact {}
					;

orfact				: OR andfact orfact {}
					| {}
					;

andfact				: notfact andfactaux {}
					;
		
andfactaux			: AND notfact andfactaux {}
					| {}
					;

notfact				: '!' expreq {}
					| expreq {}
					;

expreq				: numericexpr expreqaux {}
					;

expreqaux			: EQUAL numericexpr expreqaux {}
					| NOTEQ numericexpr expreqaux {}
					| '>' numericexpr expreqaux {}
					| '<' numericexpr expreqaux {}
					| LESSEQ numericexpr expreqaux {}
					| GREATEQ numericexpr expreqaux {}
					| {}
					;
			
numericexpr			: exprmul exprsum {}
					;

exprsum				: '+' exprmul exprsum {}
					| '-' exprmul exprsum {}
					| {}
					;

exprmul				: simpleexpr exprmulaux {}
					;
			
exprmulaux			: '*' simpleexpr exprmulaux {}
					| '/' simpleexpr exprmulaux {}
					| {}
					;
			
simpleexpr			: atomic optrange {}
					| optunary optbracket {}
					| '(' expr ')' {}
					;
       	
optrange			: DOUBLEDOT atomic {}
					| {}
					;
        		
optunary			: '-' {}
					| '+' {}
					;
			
optbracket			: '(' expr ')' {}
					| atomic {}
					;

idlist 				: ID idlistaux { $$ = $2; $$->insert($$->begin(), $1.token->value); } 
					;

idlistaux 			: ',' ID idlistaux { $$ = $3; $$->insert($$->begin(), $2.token->value); } 
		 			| { $$ = new std::vector<std::string>(); }
					;

type 				: INT { $$ = new std::string("int"); }
 					| REAL { $$ = new std::string("real"); }
					| BOOL { $$ = new std::string("bool"); }
 					| STRING { $$ = new std::string("str"); }
 					| ID { $$ = new std::string(verifyUsertype(yylval.token->value)); }
 					;

literal				: INT_VALUE	{ $$ = new TypeValue("int", std::stoi($1.token->value)); }
					| REAL_VALUE { $$ = new TypeValue("real"); }
					| HEXA_VALUE { $$ = new TypeValue("int", std::stoi($1.token->value)); }
					| BOOL_VALUE { $$ = new TypeValue("bool"); }
					| STRING_VALUE { $$ = new TypeValue("str"); }
					;

atomic				: literal { $$ = $1; }
					| id {}
					;

id					: ID idaux {}
					;

idaux				: '[' expressionlist ']' {}
					| '.' id {}
					| '(' idauxexpraux {}
					|
					;
					
idauxexpraux		: expressionlist ')' {}
					| ')' {}
					;	
			
atomiclist  		: atomic atomiclistaux {}
					;
				
atomiclistaux		: ',' atomic atomiclistaux {}
					| {}
					;
		
%%                     /* C code */

void success() {
	printf("\n\033[0;32mparse success\033[0m\n"); 
	exit(1);
}

void yyerror(char *s) {
	printf("\n%s:", s);
	printf("%d:%d: No expected '%s'\n", yylval.token->line, yylval.token->column, yylval.token->value);
	exit(1); 
}

void yyerrorDuplicateIdentifier(std::string sym_name) {
	printf("(%d:%d) Error: Duplicate identifier \"", yylval.token->line, yylval.token->column);
	std::cout << sym_name << '"' << std::endl;
	exit(1); 
}

void yyerrorSize(int expSize, int currentSize) {
	printf("(%d:%d) Error: Incompatible sizes of array \t", yylval.token->line, yylval.token->column);
	std::cout << "Expected size " << std::to_string(expSize) <<  " and current size " << std::to_string(currentSize) << std::endl;
	exit(1); 
}

void yyerrorType(std::string expType, std::string currentType) {
	printf("(%d:%d) Error: Incompatible types \t", yylval.token->line, yylval.token->column);
	std::cout << "Expected type " << expType <<  " and current type " << currentType << std::endl;
	exit(1); 
}

void yyerrorUnknownType(std::string label) {
	printf("(%d:%d) Error: Unknown type \t", yylval.token->line, yylval.token->column);
	std::cout << "There is no type called " << label << std::endl;
	exit(1); 
}


/*
	SCOPES
*/
void addScope() {
	Scope new_scope = Scope(std::map<std::string, Symbol*>()); 
	scopesTable.push_back(new_scope);
}

void removeScope() {
	scopesTable.pop_back();
}

void initTable(){
	/* TODO */
	scopesTable.push_back(Scope(std::map<std::string, Symbol*>()));
}

int main() {
	initTable();
	return yyparse();
}

void addVarDecList(std::pair<std::vector<std::string>, std::string> vardeclist, bool is_const) { 
	std::vector<std::string> idlist = vardeclist.first;
	std::string type_name = vardeclist.second;
	Symbol *var = new VariableSymbol(type_name, is_const);
	for(auto id = idlist.begin(); id != idlist.end(); id++) {
		addSymbol(*id, var);
	}
}

std::string getType(std::string label) {
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		Symbol *sym = itSym->second;
		if(itSym->first == label && sym->getMeaning() == "variable") {
			VariableSymbol *var = dynamic_cast<VariableSymbol*>(sym);
			return var->getVarType();
		}
	}
}

void printTable() {
	std::cout << "printTable begin" << std::endl;
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		std::cout << itSym->first << " " << itSym->second << std::endl;
	}
	std::cout << "printTable end" << std::endl;
}

Symbol* searchElementInTableByLabel(std::string label) {
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		if(itSym->first == label){
			return itSym->second;
		}
	}
	return nullptr;
}

std::string searchElementInTableBySymbol(Symbol *symbol) {
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		if(itSym->second->compare(symbol)){
			return itSym->first;
		}
	}
	return "";
}

void addSymbol(std::string label, Symbol *sym) {
	Symbol* element = searchElementInTableByLabel(label);

	if(element != nullptr) {
		yyerrorDuplicateIdentifier(label);
	}

	int currentScope = scopesTable.size()-1;
	std::cout << "Scope: " << currentScope << std::endl;

	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	symbolsTable[label] = sym;
	scopesTable[currentScope].symbolsTable = symbolsTable;
	
	std::cout << "Symbol added called '" << label << "' (" << *sym << ")" << std::endl;
}

std::string addVec(int size, std::string expectedType, std::vector<std::string> *initTypes) {
	if (initTypes != nullptr) {
		if (size != initTypes->size()) {
			yyerrorSize(size, initTypes->size());
		}
		for(auto it = initTypes->begin(); it != initTypes->end(); it++) {
			if(*it != expectedType) {
				yyerrorType (expectedType, *it);
			}
		}
	}

	VectorType *vec = new VectorType(expectedType, size); 
	std::string label = searchElementInTableBySymbol(vec);
	if (label != "") {
		return label;
	} else {
		std::string vecName = "-v" + std::to_string(vIndex);
		vIndex++;
		addSymbol(vecName, vec);
		return vecName;
	}
}

int rangeSize(TypeValue *first, TypeValue *second) {
	int value = 0;
	if(first->nameType == "int" && second->nameType == "int") {
		if (second->value > first->value) {
			return second->value - first->value;
		}
	} else {
		/* incompatible type */
		std::string currentType = first->nameType;
		if(currentType == "int")
			currentType = second->nameType;
		yyerrorType ("int", currentType);
	}
	return 0;
}

void addUserType(std::string label, std::string name) {
	Field field(name, name);
	std::vector<Field> fields;
	fields.push_back(field);
	UserType *userType = new UserType(label, fields);
	addSymbol(label, userType);
}

std::string verifyUsertype(std::string label) {
	Symbol* sym = searchElementInTableByLabel(label);

	if (sym == nullptr) 
		yyerrorUnknownType(label);

	UserType *userType = dynamic_cast<UserType*>(sym);

	if (userType == nullptr) 
		yyerrorUnknownType(label);

	return label;
}