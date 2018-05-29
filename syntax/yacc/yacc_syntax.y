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
#include "../../type_verification.h"
#include <iostream>
#include <map>


int yylex();
void success();

// SCOPES --------------

void addScope(std::string returnType);
void removeScope();
void initTable();
Symbol* searchElementInTableByLabel(std::string label);
std::string searchElementInTableBySymbol(Symbol *symbol);

// VARS --------------

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
%type <typeName> type varconstruction arraydec typedecauxrange expr callidbegin atomic orfact andfact andfactaux notfact expreq expreqaux numericexpr exprsum exprmul exprmulaux simpleexpr optbracket decwithassign atomiclistaux atomiclist caseclause
%type <list> idlist idlistaux expressionlist expressionlistaux arraydecaux idaux id idauxexpraux caselist caselistaux caselistaux2
%type <fields> paramslist paramsaux parameters vardeclist vardeclistaux
%type <size> rangelistaux rangelist range
%type <var> literal

/*Terminals*/
%token REAL_VALUE FINAL BOOL_VALUE CONTINUE 
INT REAL BOOL STRING VECTOR PROG INIT END OF VAR STRUCT 
TYPE CONST RETURN JUMP FOR WHILE DO REPEAT UNTIL IF THEN ELSE CASE 
BE FUNC PROC BREAK REF AND OR LESSEQ GREATEQ EQUAL NOTEQ DOUBLEDOT 
STRING_VALUE CASSIGN WRITE READ
%token <token> ID INT_VALUE HEXA_VALUE LABEL
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
			  		| abstractiondec
					;

arraydec  			: VECTOR '[' rangelist ']' OF type arraydecaux { std::string v = addVec($3, *$6, $7); $$ = new std::string(v); }
					;

arraydecaux  		: '=' '(' expressionlist ')' { $$ = $3; }
			  		| { $$ = nullptr; }
					;

rangelist  			: range rangelistaux { $$ = $1 + $2; }
					;

rangelistaux  		: ',' range rangelistaux { $$ = $3 + $2; }
			  		| {$$ = 0;}
					;

range  				: literal DOUBLEDOT literal { $$ = rangeSize($1, $3); }
					;

vardec  			: VAR idlist ':' varconstruction { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

varconstruction  	: type decwithassign { if(*$2 == "") { $$ = $1; } else if(*$1 == *$2) { $$ = $1; } else { yyerrorInvalidType(*$2); } }
				  	| arraydec { $$ = $1; }
					;

decwithassign  		: '=' expr { $$ = $2; }
			  		| { $$ = new std::string(""); }
					;

usertype  			: TYPE ID CASSIGN arraydec { addUserType($2->value, *$4); }
					| TYPE ID CASSIGN STRUCT vardeclist END { addUserType($2->value, *$5); }
					| TYPE ID CASSIGN literal DOUBLEDOT literal { addUserType($2->value, $4->nameType, $6->nameType); }
					| TYPE ID CASSIGN id typedecauxrange { addUserType($2->value, getTypeByPath(*$4), *$5); }
					| TYPE ID CASSIGN '(' idlist ')' { addUserType($2->value, *$5); }
					;

typedecauxrange		: DOUBLEDOT id { $$ = new std::string(getTypeByPath(*$2)); }
					| { $$ = new std::string(""); }
					;

vardeclist  		: vardec vardeclistaux { $$ = $2; insertVarDec($$, $1); }
					;

vardeclistaux  		: ';' vardec vardeclistaux { $$ = $3; insertVarDec($$, $2); }
			  		| { $$ = new std::vector<Field>(); }
					;

labeldec  			: LABEL idlist { addVarDecList(std::pair<std::vector<std::string>, std::string>(*$2, $1->value), false); }
					;

constdec  			: CONST idlist ':' type '=' expr { $$ = new std::pair<std::vector<std::string>, std::string>(*$2, *$4); }
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: PROC ID '(' parameters ')' { Symbol *ab = new AbstractionSymbol("", *$4); addSymbol($2->value, ab); addScope(""); addParams($2->value, *$4); } prevdec block { removeScope(); } 
					;

funcdec  			: FUNC ID '(' parameters ')' ':' type { Symbol *ab = new AbstractionSymbol(*$7, *$4); addSymbol($2->value, ab); addScope(*$7); addParams($2->value, *$4); } prevdec block { removeScope(); }
					;

parameters  		: paramsaux { $$ = $1;}
			  		| { $$ = new std::vector<Field>(); }
					;

paramsaux  			: ID ':' type paramslist { $4->insert($4->begin(), Field($1->value, *$3)); $$ = $4; }
		  			| REF ID ':' type paramslist { $5->insert($5->begin(), Field($2->value, *$4)); $$ = $5; }
					;

paramslist  		: ',' paramsaux { $$ = $2; }
			  		| { $$ = new std::vector<Field>(); }
					;


prevcommand  		: commands {}
			  		| {}
					;

callcommand  		: BREAK {}	
			  		| loop {}
			  		| block {}
			  		| return {}
			  		| CONTINUE {}
			  		| id callidbegin { std::string t = getTypeByPath(*$1); if(*$2 != "") { verifyType(t, *$2); } }
			  		| calllabel {}
			  		| conditional {}
					;

commands  			: callcommand commandsaux {}
					;

commandsaux  		: ';' commands {}
			  		| {}
					;

callidbegin  		: ':' callcommand { $$ = new std::string("rotulo"); }
			  		| '=' expr { $$ = $2; }
			  		| { $$ = new std::string(""); }
					;

calllabel  			: JUMP ID { verifyLabel($2->value); }
					;
					
return  			: RETURN expr { int currentScope = scopesTable.size()-1; std::string r = scopesTable[currentScope].returnType; verifyType(r, *$2); }
					;

loop  				: forloop {}
	  				| whileloop {}
	  				| repeatloop {}
					;

forloop  			: FOR '(' forstruct ')' DO callcommand 
					;

forstruct  			: prevfor ';' expr ';' posfor { if(*$3 != "bool") { yyerrorType("bool", *$3); } }
					;

prevfor 			: varassignlist {}
		  			| {}
					;

varassignlist  		: ID '=' expr varassignlistaux { verifyTypeAssign($1->value, *$3); }
					;

varassignlistaux  	: ',' ID '=' expr varassignlistaux { verifyTypeAssign($2->value, *$4); }
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

whileloop  			: WHILE '(' expr ')' { if(*$3 != "bool") { yyerrorType("bool", *$3); } } DO callcommand 
					;

repeatloop  		: REPEAT commands UNTIL expr { if(*$4 != "bool") { yyerrorType("bool", *$4); } }
					;

conditional  		: ifcond {}
			 		| casecond {}
					;

ifcond  			: IF '(' expr ')' { if(*$3 != "bool") { yyerrorType("bool", *$3); } }  THEN callcommand ifcondaux {}
					;

ifcondaux  			: ELSE callcommand 
					| {}
					;
					
					
casecond  			:  CASE '(' expr ')' BE caselist casecondaux { verifyCaseList(*$3, *$6); }
					;

casecondaux  		: END {}
			  		| ELSE commands END {}
					;

caselist  			: caseclause caselistaux { $2->insert($2->begin(), *$1); $$ = $2; }
					;

caselistaux  		: ';' caselistaux2 { $$ = $2; }
			  		| { $$ = new std::vector<std::string>(); }
					;

caselistaux2  		: caseclause caselistaux { $$ = $2; $$->insert($$->begin(), *$1); }
			  		| { $$ = new std::vector<std::string>(); }
					;

caseclause  		: atomiclist ':' callcommand { $$ = $1; }
					;

expressionlist  	: expr expressionlistaux { $$ = $2; $$->insert($$->begin(), *$1); }
					;

expressionlistaux  	: ',' expr expressionlistaux { $$ = $3; $$->insert($$->begin(), *$2); }
				  	| { $$ = new std::vector<std::string>(); }
					;
	
expr				: andfact orfact {  $$ = verifyLogicExprType(*$1, *$2); }
					;

orfact				: OR andfact orfact { $$ = verifyLogicExprType(*$2, *$3); }
					| { $$ = new std::string(""); }
					;

andfact				: notfact andfactaux { $$ = verifyLogicExprType(*$1, *$2); }
					;
		
andfactaux			: AND notfact andfactaux { $$ = verifyLogicExprType(*$2, *$3); }
					| { $$ = new std::string(""); }
					;

notfact				: '!' expreq { if(*$2 == "bool") { $$ = $2; } else { yyerrorInvalidType(*$2); } }
					| expreq { $$ = $1; }
					;

expreq				: numericexpr expreqaux { $$ = verifyRelationalExprType(*$1, *$2); }
					;

expreqaux			: EQUAL numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| NOTEQ numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| '>' numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| '<' numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| LESSEQ numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| GREATEQ numericexpr expreqaux { $$ = verifyRelationalExprType(*$2, *$3); }
					| { $$ = new std::string(""); }
					;
			
numericexpr			: exprmul exprsum { $$ = verifyArithmeticExprType(*$1, *$2); }
					;

exprsum				: '+' exprmul exprsum { $$ = verifyArithmeticExprType(*$2, *$3); }
					| '-' exprmul exprsum { $$ = verifyArithmeticExprType(*$2, *$3); }
					| { $$ = new std::string(""); }
					;

exprmul				: simpleexpr exprmulaux { $$ = verifyArithmeticExprType(*$1, *$2); }
					;
			
exprmulaux			: '*' simpleexpr exprmulaux { $$ = verifyArithmeticExprType(*$2, *$3); }
					| '/' simpleexpr exprmulaux { $$ = verifyArithmeticExprType(*$2, *$3); } 
					| { $$ = new std::string(""); }
					;
			
simpleexpr			: atomic { $$ = $1; }
					| optunary optbracket { $$ = $2; }
					| '(' expr ')' { $$ = $2; }
					;

optunary			: '-' {}
					| '+' {}
					;
			
optbracket			: '(' expr ')' { $$ = $2; }
					| atomic { $$ = $1;}
					;

idlist 				: ID idlistaux { $$ = $2; $$->insert($$->begin(), $1->value); } 
					;

idlistaux 			: ',' ID idlistaux { $$ = $3; $$->insert($$->begin(), $2->value); } 
		 			| { $$ = new std::vector<std::string>(); }
					;

type 				: INT { $$ = new std::string("int"); }
 					| REAL { $$ = new std::string("real"); }
					| BOOL { $$ = new std::string("bool"); }
 					| STRING { $$ = new std::string("texto"); }
 					| ID { $$ = new std::string(verifyUsertype($1->value)); }
 					;

literal				: INT_VALUE	{ $$ = new TypeValue("int", std::stoi($1->value)); }
					| REAL_VALUE { $$ = new TypeValue("real"); }
					| HEXA_VALUE { $$ = new TypeValue("int", std::stoi($1->value)); }
					| BOOL_VALUE { $$ = new TypeValue("bool"); }
					| STRING_VALUE { $$ = new TypeValue("texto"); }
					;

atomic				: literal { $$ = new std::string($1->nameType); }
					| id { $$ = new std::string(getTypeByPath(*$1)); }
					;

id					: ID idaux { $$ = $2; $$->insert($$->begin(), $1->value); }
					;

idaux				: '[' expressionlist ']' { int size = $2->size(); $$ = $2; $$->insert($$->begin(), std::to_string(size)); }
					| '.' id { $$ = $2; }
					| '(' idauxexpraux { $$ = $2; }
					| { $$ = new std::vector<std::string>(); }
					;
					
idauxexpraux		: expressionlist ')' { int size = $1->size(); $$ = $1; $$->insert($$->begin(), std::to_string(size)); }
					| ')' { $$ = new std::vector<std::string>(); $$->push_back("0"); }
					;	

atomiclist  		: atomic atomiclistaux { if (*$2 == "" || *$1 == *$2) { $$ = $1; } else { yyerrorType(*$1, *$2); } }
					;
				
atomiclistaux		: ',' atomic atomiclistaux { if (*$3 == "" || *$2 == *$3) { $$ = $2; } else { yyerrorType(*$2, *$3); } }
					| { $$ = new std::string(""); }
					;
		
%%                     /* C code */

/*
	SCOPES
*/
void addScope(std::string returnType) {
	Scope new_scope = Scope(std::map<std::string, Symbol*>()); 
	new_scope.returnType = returnType;
	scopesTable.push_back(new_scope);
}

void removeScope() {
	scopesTable.pop_back();
}

void printTable() {
	std::cout << "-----printTable begin-----" << std::endl;
	
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	int scop = scopesTable.size()-1;
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		std::cout << "Current scope: " << scop << std::endl;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			std::cout << "\t- " << itSym->first;
			std::cout << " (" << *itSym->second << ")" << std::endl;
		}
		scop--;
	}
	std::cout << "-----printTable end-----" << std::endl;
}

void initTable(){
	/* TODO */
	std::map<std::string, Symbol*> table = std::map<std::string, Symbol*>();
	table["int"] = new PrimitiveType(new std::string("int"));
	table["real"] = new PrimitiveType(new std::string("real"));
	table["texto"] = new PrimitiveType(new std::string("texto"));
	table["bool"] = new PrimitiveType(new std::string("bool"));
	table["rotulo"] = new PrimitiveType(new std::string("label"));
	table[""] = new PrimitiveType(new std::string(""));
	
	std::vector<Field> paramsEscreva;
	paramsEscreva.push_back(Field("arg", "texto"));
	table["escreva"] = new AbstractionSymbol("", paramsEscreva); 
	
	std::vector<Field> paramsLeiaInt;
	paramsLeiaInt.push_back(Field("arg", "int"));
	table["leiaInteiro"] = new AbstractionSymbol("", paramsLeiaInt); 
	
	std::vector<Field> paramsLeiaReal;
	paramsLeiaReal.push_back(Field("arg", "real"));
	table["leiaReal"] = new AbstractionSymbol("", paramsLeiaReal); 
	
	std::vector<Field> paramsLeiaTexto;
	paramsLeiaTexto.push_back(Field("arg", "texto"));
	table["leiaTexto"] = new AbstractionSymbol("", paramsLeiaTexto); 
	
	std::vector<Field> paramsLeiaBool;
	paramsLeiaBool.push_back(Field("arg", "bool"));
	table["leiaLogico"] = new AbstractionSymbol("", paramsLeiaBool); 
	
	Scope scope = Scope(table);
	scopesTable.push_back(scope);
}

int main() {
	vIndex = 0;
	initTable();
	return yyparse();
}

void success() {
	printf("\n\033[0;32mparse success\033[0m\n"); 
	printTable();
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

void yyerrorUnknownLabel(std::string label) {
	printf("(%d:%d) Error: Unknown label \t", yylval.token->line, yylval.token->column);
	std::cout << label << " is not a valid variable in current context" << std::endl;
	exit(1); 
}

void yyerrorInvalidArgs(std::string label) {
	printf("(%d:%d) Error: Invalid list of arguments \t", yylval.token->line, yylval.token->column);
	std::cout << label << " parameters and arguments not compatible" << std::endl;
	exit(1); 
}
