%{
#include "../../token.c"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include "../../utilities.h"
#include "../../errors.h"
#include "../../table_manage.h"
#include <iostream>
#include <map>


int yylex();
void success();

// SCOPES --------------

void addScope();
void removeScope();
void initTable();
Symbol* searchElementInTableByLabel(std::string label);
std::string searchElementInTableBySymbol(Symbol *symbol);

// VARS --------------

void addParams(std::vector<Field> params);
%}

%union { 
	std::string *typeName;
	int size;
	Token *token; 
	std::vector<std::string> *list; /*it can be idlist, expressionlist and so on... */
	std::pair<std::vector<std::string>, std::string> *varDec;
	std::vector<Field> *fields;
	TypeValue *var;
}

%start program

%type <varDec> vardec constdec
%type <typeName> type varconstruction arraydec typedecauxrange expr
%type <list> idlist idlistaux expressionlist expressionlistaux arraydecaux idaux id idauxexpraux
%type <fields> paramslist paramsaux parameters vardeclist vardeclistaux
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

declaration  		: vardec { addVarDecList(*$1, false); }
			  		| usertype {}
			  		| labeldec {}
			  		| constdec { addVarDecList(*$1, true); }
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
					| TYPE ID CASSIGN STRUCT vardeclist END { addUserType($2.token->value, *$5); }
					| TYPE ID CASSIGN literal DOUBLEDOT literal { addUserType($2.token->value, $4->nameType, $6->nameType); }
					| TYPE ID CASSIGN id typedecauxrange { addUserType($2.token->value, getTypeByPath(*$4), *$5); }
					| TYPE ID CASSIGN '(' idlist ')' { addUserType($2.token->value, *$5); }
					;

typedecauxrange		: DOUBLEDOT id { $$ = new std::string(getTypeByPath(*$2)); }
					| { $$ = new std::string(""); }
					;

vardeclist  		: vardec vardeclistaux { $$ = $2; insertVarDec($$, $1); }
					;

vardeclistaux  		: ';' vardec vardeclistaux { $$ = $3; insertVarDec($$, $2); }
			  		| { $$ = new std::vector<Field>(); }
					;

labeldec  			: LABEL idlist {}
					;

constdec  			: CONST idlist ':' type '=' expr { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: PROC ID '(' parameters ')' { Symbol *ab = new AbstractionSymbol("", *$4); addSymbol($2.token->value, ab); addScope(); addParams(*$4);/* add parameters in current scope */ } prevdec block { removeScope(); } 
					;

funcdec  			: FUNC ID '(' parameters ')' ':' type { Symbol *ab = new AbstractionSymbol(*$7, *$4); addSymbol($2.token->value, ab); addScope(); addParams(*$4); } prevdec block { removeScope(); }
					;

parameters  		: paramsaux { $$ = $1;}
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
					| id { $$ = new TypeValue(getTypeByPath(*$1)); }
					;

id					: ID idaux { $$ = $2; $$->insert($$->begin(), $1.token->value); }
					;

idaux				: '[' expressionlist ']' { $$ = $2;}
					| '.' id { $$ = $2; }
					| '(' idauxexpraux { $$ = $2; }
					| { $$ = new std::vector<std::string>(); }
					;
					
idauxexpraux		: expressionlist ')' { int size = $1->size(); $$ = $1; $$->insert($$->begin(), std::to_string(size)); /*verificar ordem dos argumentos*/ }
					| ')' { $$ = new std::vector<std::string>(); $$->push_back("0"); }
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

void yyerrorDuplicateIdentifier(std::string symName) {
	printf("(%d:%d) Error: Duplicate identifier \"", yylval.token->line, yylval.token->column);
	std::cout << symName << '"' << std::endl;
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

void yyerrorInvalidType(std::string label) {
	printf("(%d:%d) Error: Invalid type \t", yylval.token->line, yylval.token->column);
	std::cout << label << " is not a valid type in current context" << std::endl;
	exit(1); 
}

void yyerrorUnknownVar(std::string label) {
	printf("(%d:%d) Error: Unknown variable \t", yylval.token->line, yylval.token->column);
	std::cout << label << " is not a valid variable in current context" << std::endl;
	exit(1); 
}

void yyerrorInvalidArgs(std::string label) {
	printf("(%d:%d) Error: Invalid list of arguments \t", yylval.token->line, yylval.token->column);
	std::cout << label << " parameters and arguments not compatible" << std::endl;
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
	std::map<std::string, Symbol*> table = std::map<std::string, Symbol*>();
	table["int"] = new PrimitiveType("int");
	Scope scope = Scope(table);
	scopesTable.push_back(scope);
}

int main() {
	initTable();
	return yyparse();
}

void printTable() {
	std::cout << "-----printTable begin-----" << std::endl;
	//int currentScope = scopesTable.size()-1;
	//std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	int scop = scopesTable.size()-1;
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			std::cout  << "Scope " << scop << ", " << itSym->first << " " << itSym->second->getMeaning() << std::endl;
		}
		scop--;
	}
	std::cout << "-----printTable end-----" << std::endl;
}