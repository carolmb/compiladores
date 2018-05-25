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
std::string get_type(std::string label);
void addVarDecList(std::pair<std::vector<std::string>, std::string> vardeclist, bool is_const);

void yyerrorDuplicateIdentifier (std::string sym_name);

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

// VARS --------------

void addTable(std::string label, Symbol *sym);

%}

%union { 
	std::string *typeName;
	Token *token; 
	Type *symbolType; 
	std::vector<Field> *structFields;
	std::vector<std::string> *idList;
	std::pair<std::vector<std::string>, std::string> *varDec;
	std::vector<Field> *fields;
}

%start program

%type <varDec> vardec constdec
%type <typeName> type varconstruction arraydec typedecauxrange literal
%type <symbolType> typedecaux typedec
%type <structFields> vardeclist vardeclistaux
%type <idList> idlist idlistaux
%type <fields> paramslist paramsaux parameters

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

declaration  		: vardec { std::cout << "VARDEC" << std::endl; addVarDecList(*$1, false); }
			  		| usertype { std::cout << "USERTYPE" << std::endl; }
			  		| labeldec {}
			  		| constdec { std::cout << "USERTYPE" << std::endl; addVarDecList(*$1, true); }
			  		| abstractiondec { std::cout << "ABSTRACTIONDEC" << std::endl; }
					;

arraydec  			: VECTOR '[' rangelist ']' OF type arraydecaux { $$ = new std::string("vetor"); /*adiciona o vetor c nome vxx na tabela e só retorna seu nome (verifica tipos) */ }
					;

arraydecaux  		: '=' '(' expressionlist ')' { /* retorna tamanho e tipo */ }
			  		| {}
					;

rangelist  			: range rangelistaux {}
					;

rangelistaux  		: ',' range rangelistaux {}
			  		| {}
					;

range  				: atomic DOUBLEDOT atomic {}
					;

vardec  			: VAR idlist ':' varconstruction { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

varconstruction  	: type decwithassign { $$ = $1; }
				  	| arraydec { $$ = new std::string("vector"); }
					;

decwithassign  		: '=' expr {}
			  		| {}
					;

usertype  			: TYPE ID CASSIGN typedec {}
					;

typedec  			: arraydec { /* cria o Type*/ }
		  			| typedecaux { /* já é Type $$ = $1; */ }
					;

typedecaux  		: STRUCT vardeclist END { $$ = new UserType();  }
			  		| literal DOUBLEDOT literal { $$ = new RangeType(); }
			  		| id typedecauxrange { $$ = new RangeType(); }
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

constdec  			: CONST idlist ':' type '=' expr { *$$ = std::make_pair(*$2, *$4); }
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: PROC { addScope(); } ID '(' parameters ')' { /* Symbol *ab = new AbstractionSymbol("", *$4); addTable($2.token->value, ab);*/ } prevdec block { removeScope(); } 
					;

funcdec  			: FUNC { addScope(); } ID '(' parameters ')' ':' type { /* Symbol *ab = new AbstractionSymbol("", *$4); addTable($2.token->value, ab);*/ } prevdec block { removeScope(); }
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

expressionlist  	: expr expressionlistaux {}
					;

expressionlistaux  	: ',' expr expressionlistaux {}
				  	| {}
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
 					| ID { $$ = new std::string(yylval.token->value); /* TODO verify if label is a valid user type */ }
 					;

literal				: INT_VALUE	{ $$ = new std::string("int"); }
					| REAL_VALUE { $$ = new std::string("real"); }
					| HEXA_VALUE { $$ = new std::string("int"); }
					| BOOL_VALUE { $$ = new std::string("bool"); }
					| STRING_VALUE { $$ = new std::string("str"); }
					;

atomic				: literal {}
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

void yyerror (char *s) {
	printf("\n%s:", s);
	printf("%d:%d: No expected '%s'\n", yylval.token->line, yylval.token->column, yylval.token->value);
	exit(1); 
}

void yyerrorDuplicateIdentifier (std::string sym_name) {
	printf("(%d:%d) Error: Duplicate identifier \"", yylval.token->line, yylval.token->column);
	std::cout << sym_name << '"' << std::endl;
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
	std::cout << "aqui" << std::endl;
	std::vector<std::string> idlist = vardeclist.first;
	std::string type_name = vardeclist.second;
	Symbol *var = new VariableSymbol(type_name, is_const);
	for(auto id = idlist.begin(); id != idlist.end(); id++) {
		addTable(*id, var);
	}
}

std::string get_type(std::string label) {
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		if(itSym->first == label && itSym->second->get_meaning() == "variable") {
			Symbol *bla = itSym->second;
			VariableSymbol *var = dynamic_cast<VariableSymbol*>(bla);
			return var->get_varType();
		}
	}
}

void print_table() {
	std::cout << "print_table begin" << std::endl;
	int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		std::cout << itSym->first << " " << itSym->second << std::endl;
	}
	std::cout << "print_table end" << std::endl;
}

void addTable(std::string label, Symbol *sym) {
	int currentScope = scopesTable.size()-1;
	std::cout << "Scope: " << currentScope << std::endl;
	
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		if(itSym->first == label){
			yyerrorDuplicateIdentifier(label);
		}
	}
	
	symbolsTable[label] = sym;
	scopesTable[currentScope].symbolsTable = symbolsTable;
	
	std::cout << "Symbol added called '" << label << "' (" << sym->get_meaning() << ")" << std::endl;
}