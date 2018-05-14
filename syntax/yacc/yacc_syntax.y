%{
#include "../../token.c"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

void yyerror (char *s); 

int yylex();

void success();

void addTable(std::string type);

int currentScope;

struct Symbol {
	std::string name;
	std::string type;
	Symbol(std::string n, std::string t) : name(n), type(t) {}
};

struct Scope {
	int parentScope;
	std::vector<Symbol> symbolsTable;
	Scope(int p, std::vector<Symbol> st) : parentScope(p), symbolsTable(st) {}
	Scope(){}
};

std::map<int, Scope> scopesTable; 

std::vector<std::string> prevSymbols;
std::string prevType;

%}

%union { Token* token; std::string *symbolType; }         /* Yacc definitions */
%start program

%type <symbolType> type

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

declaration  		: vardec {}
			  		| usertype {}
			  		| labeldec {}
			  		| constdec {}
			  		| abstractiondec {}
					;

arraydec  			: VECTOR '[' rangelist ']' OF type arraydecaux {}
					;

arraydecaux  		: '=' '(' expressionlist ')' {}
			  		| {}
					;

rangelist  			: range rangelistaux {}
					;

rangelistaux  		: ',' range rangelistaux {}
			  		| {}
					;

range  				: atomic DOUBLEDOT atomic {}
					;

vardec  			: VAR idlist ':' varconstruction {}
					;

varconstruction  	: type decwithassign { addTable(*$1); }
				  	| arraydec {}
					;

decwithassign  		: '=' expr {}
			  		| {}
					;

usertype  			: TYPE ID CASSIGN typedec {}
					;

typedec  			: arraydec {}
		  			| typedecaux {}
					;

typedecaux  		: STRUCT vardeclist END {}
			  		| atomic typedecauxrange {}
			  		| '(' idlist ')' {}
					;

typedecauxrange		: DOUBLEDOT atomic {}
					| {}
					;

vardeclist  		: vardec vardeclistaux {}
					;

vardeclistaux  		: ';' vardec vardeclistaux {}
			  		| {}
					;

labeldec  			: LABEL idlist {}
					;

constdec  			: CONST idlist ':' type '=' expr {}
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: PROC ID '(' parameters ')' prevdec block 
					;

funcdec  			: FUNC ID '(' parameters ')' ':' type prevdec block 
					;

parameters  		: paramsaux {}
			  		| {}
					;

paramsaux  			: ID ':' type paramslist {}
		  			| REF ID ':' type paramslist {}
					;

paramslist  		: ',' paramsaux {}
			  		| {}
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

idlist 				: ID { prevSymbols.push_back(yylval.token->value); } idlistaux
					;

idlistaux 			: ',' ID { prevSymbols.push_back(yylval.token->value); } idlistaux
		 			| {}
					;

type 				: INT { $$ = new std::string("int"); }
 					| REAL { $$ = new std::string("real"); }
					| BOOL { $$ = new std::string("bool"); }
 					| STRING { $$ = new std::string("str"); }
 					| ID { $$ = new std::string(yylval.token->value); }
 					;

literal				: INT_VALUE	{}
					| REAL_VALUE {}
					| HEXA_VALUE {}
					| BOOL_VALUE {}
					| STRING_VALUE {}
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

int main() {
	currentScope = -1;
	scopesTable[++currentScope] = Scope(currentScope, std::vector<Symbol>());
	return yyparse();
}

void addTable(std::string type) {
	std::cout << "Scope: " << currentScope << std::endl;
	
	for(auto it = prevSymbols.begin(); it != prevSymbols.end(); it++) {
		Symbol sym(*it, type);
		scopesTable[currentScope].symbolsTable.push_back(sym);
		std::cout << sym.name << ": " << sym.type << std::endl;
	}
	std::cout << std::endl;
	prevSymbols.clear();
}