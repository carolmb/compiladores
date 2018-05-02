%{
#include "../../token.c"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>

void yyerror (char *s); 

int yylex();

void success();
%} 

%union {Token* token;}         /* Yacc definitions */
%start program

/*Terminals*/
%token HEXA_VALUE INT_VALUE REAL_VALUE FINAL BOOL_VALUE CONTINUE 
ID INT REAL BOOL STRING VECTOR PROG INIT END OF VAR LABEL STRUCT 
TYPE CONST RETURN JUMP FOR WHILE DO REPEAT UNTIL IF THEN ELSE CASE 
BE FUNC PROC BREAK 
REF AND OR LESSEQ GREATEQ EQUAL NOTEQ DOUBLEDOT 
STRING_VALUE CASSIGN WRITE READ INT REAL BOOL STRING
%%

program  			: PROG ID ';' prevdec block 							{printf("%s program -> 'prog' ID ';' prevdec block\n", $1); success();}
					;

block  				: INIT prevcommand END 									{printf("block -> 'inicio' prevcommand 'fim'\n");}
					;

prevdec  			: declaration ';' prevdec 								{printf("prevdec -> declaration ';' prevdec\n");}
		  			|  														{printf("prevdec -> LAMBDA\n");}
					;

declaration  		: vardec 												{printf("declaration -> vardec\n");}
			  		| usertype 												{printf("declaration -> usertype\n");}
			  		| labeldec 												{printf("declaration -> labeldec\n");}
			  		| constdec 												{printf("declaration -> constdec\n");}
			  		| abstractiondec 										{printf("declaration -> abstractiondec\n");}
					;

arraydec  			: VECTOR '[' rangelist ']' OF type arraydecaux 			{printf("arraydec -> 'vetor' '[' rangelist ']' 'de' type arraydecaux\n");}
					;

arraydecaux  		: '=' '(' expressionlist ')' 							{printf("arraydecaux -> '=' '(' expressionlist ')'\n");}
			  		|  														{printf("arraydecaux -> LAMBDA\n");}
					;

rangelist  			: range rangelistaux 									{printf("rangelist -> range rangelistaux\n");}
					;

rangelistaux  		: ',' range rangelistaux 								{printf("rangelistaux -> ',' range rangelistaux\n");}
			  		|   													{printf("rangelistaux -> LAMBDA\n");}
					;

range  				: atomic DOUBLEDOT atomic 								{printf("range -> atomic '..' atomic\n");}
					;

vardec  			: VAR idlist ':' varconstruction 						{printf("vardec -> 'var' idlist ':' varconstruction\n");}
					;

varconstruction  	: type decwithassign 									{printf("varconstruction -> type decwithassign\n");}
				  	| arraydec 												{printf("varconstruction -> arraydec\n");}
					;

decwithassign  		: '=' expr 												{printf("decwithassign -> '=' expr\n");}
			  		|   													{printf("decwithassign -> LAMBDA\n");}
					;

usertype  			: TYPE ID CASSIGN typedec 								{printf("usertype -> 'tipo' ID ':=' typedec\n");}
					;

typedec  			: arraydec 												{printf("typedec -> arraydec\n");}
		  			| typedecaux 											{printf("typedec -> typedecaux\n");}
					;

typedecaux  		: STRUCT vardeclist END 								{printf("typedecaux -> 'registro' vardeclist 'fim'\n");}
			  		| atomic typedecauxrange								{printf("typedecaux -> atomic typedecauxrange\n");}
			  		| '(' idlist ')' 										{printf("typedecaux -> '(' idlist ')'\n");}
					;

typedecauxrange		: DOUBLEDOT atomic 										{printf("typedecauxrange -> '..' atomic \n");}
					| 														{printf("typedecauxrange -> LAMBDA\n");}
					;

vardeclist  		: vardec vardeclistaux 									{printf("vardeclist -> vardec vardeclistaux\n");}
					;

vardeclistaux  		: ';' vardec vardeclistaux 								{printf("vardeclistaux -> ';' vardec vardeclistaux\n");}
			  		|   													{printf("vardeclistaux -> LAMBDA\n");}
					;

labeldec  			: LABEL idlist 											{printf("labeldec -> 'rotulo' idlist\n");}
					;

constdec  			: CONST idlist ':' type '=' expr 						{printf("constdec -> 'const' idlist ':' type '=' expr\n");}
					;

abstractiondec  	: procdec 												{printf("abstractiondec -> procdec\n");}
				  	| funcdec 												{printf("abstractiondec -> funcdec\n");}
					;

procdec  			: PROC ID '(' parameters ')' prevdec block 				{printf("procdec -> 'proc' ID '(' parameters ')' prevdec block\n");}
					;

funcdec  			: FUNC ID '(' parameters ')' ':' type prevdec block 	{printf("funcdec -> 'func' ID '(' parameters ')' ':' type prevdec block\n");}
					;

parameters  		: paramsaux 											{printf("parameters -> paramsaux\n");}
			  		|   													{printf("parameters -> LAMBDA\n");}
					;

paramsaux  			: ID ':' type paramslist 								{printf("paramsaux -> ID ':' type paramslist\n");}
		  			| REF ID ':' type paramslist 							{printf("paramsaux -> 'ref' ID ':' type paramslist\n");}
					;

paramslist  		: ',' paramsaux 										{printf("paramslist -> paramsaux\n");}
			  		|   													{printf("paramslist -> LAMBDA\n");}
					;


prevcommand  		: commands 												{printf("prevcommand -> commands\n");}
			  		|   													{printf("prevcommand -> LAMBDA\n");}
					;

callcommand  		: BREAK 												{printf("callcommand -> 'pare'\n");}
			  		| write 												{printf("callcommand -> write\n");}
			  		| read 													{printf("callcommand -> read\n");}
			  		| loop 													{printf("callcommand -> loop\n");}
			  		| block 												{printf("callcommand -> block\n");}
			  		| return 												{printf("callcommand -> return\n");}
			  		| CONTINUE 												{printf("callcommand -> 'continue'\n");}
			  		| id callidbegin 										{printf("callcommand -> id callidbegin\n");}
			  		| calllabel 											{printf("callcommand -> calllabel\n");}
			  		| conditional 											{printf("callcommand -> conditional\n");}
					;

commands  			: callcommand commandsaux 								{printf("commands -> callcommand commandsaux\n");}
					;

commandsaux  		: ';' commands 											{printf("commandsaux -> ';' commands\n");}
			  		|   													{printf("commandsaux -> LAMBDA\n");}
					;

callidbegin  		: ':' callcommand 										{printf("callidbegin -> ':' callcommand \n");}
			  		| '=' expr 												{printf("callidbegin -> '=' expr\n");}
			  		|   													{printf("callidbegin -> LAMBDA\n");}
					;

calllabel  			: JUMP ID 												{printf("calllabel -> JUMP ID\n");}
					;

write  				: WRITE '(' expressionlist ')' 							{printf("write -> 'escreva' '(' expressionlist ')'\n");}
					;

read  				: READ '(' expressionlist ')' 							{printf("read -> 'leia' '(' expressionlist ')'\n");}
					;

return  			: RETURN expr 											{printf("return -> 'retorne' expr \n");}
					;

loop  				: forloop 												{printf("loop -> forloop\n");}
	  				| whileloop 											{printf("loop -> whileloop\n");}
	  				| repeatloop 											{printf("loop -> repeatloop\n");}
					;

forloop  			: FOR '(' forstruct ')' DO callcommand 					{printf("forloop -> 'para' '(' forstruct ')' 'faca' callcommand\n");}
					;

forstruct  			: prevfor ';' expr ';' posfor 							{printf("forstruct -> prevfor ';' expr ';' posfor \n");}
					;

prevfor 			: varassignlist 										{printf("prevfor -> varassignlist\n");}
		  			|   													{printf("prevfor -> LAMBDA\n");}
					;

varassignlist  		: ID '=' expr varassignlistaux 							{printf("varassignlist -> ID '=' expr varassignlistaux\n");}
					;

varassignlistaux  	: ',' ID '=' expr varassignlistaux 						{printf("varassignlistaux -> ',' ID '=' expr varassignlistaux\n");}
				  	|   													{printf("varassignlistaux -> LAMBDA\n");}
					;

posfor  			: posforaux 											{printf("posfor -> posforaux\n");}
		  			|   													{printf("posfor -> LAMBDA\n");}
					;

posforaux  			: commands posforaux2 									{printf("posforaux -> commands posforaux2\n");}
					;

posforaux2  		: ',' commands posforaux2 								{printf("posforaux2 -> ',' commands posforaux2\n");}
			  		|   													{printf("posforaux2 -> LAMBDA\n");}
					;

whileloop  			: WHILE '(' expr ')' DO callcommand 					{printf("whileloop -> 'enquanto' '(' expr ')' 'faca' callcommand\n");}
					;

repeatloop  		: REPEAT commands UNTIL expr 							{printf("repeatloop -> 'repita' commands 'ate' expr \n");}
					;

conditional  		: ifcond 												{printf("conditional -> ifcond\n");}
			 		| casecond 												{printf("conditional -> casecond\n");}
					;

ifcond  			: IF '(' expr ')' THEN callcommand ifcondaux 			{printf("ifcond -> 'se' '(' expr ')' 'entao' callcommand ifcondaux\n");}
					;

ifcondaux  			: ELSE callcommand										{printf("ifcondaux -> 'senao' callcommand \n");}
					|														{printf("ifcondaux -> LAMBDA \n");}
					;
					
					
casecond  			: CASE '(' expr ')' BE caselist casecondaux 			{printf("casecond -> 'caso' '(' expr ')' 'seja' caselist casecondaux\n");}
					;

casecondaux  		: END 													{printf("casecondaux -> 'fim'\n");}
			  		| ELSE commands END 									{printf("casecondaux -> 'senao' commands 'fim'\n");}
					;

caselist  			: caseclause caselistaux 								{printf("caselist -> caseclause caselistaux\n");}
					;

caselistaux  		: ';' caselistaux2 										{printf("caselistaux -> ';' caselistaux2\n");}
			  		|   													{printf("caselistaux -> LAMBDA\n");}
					;

caselistaux2  		: caseclause caselistaux 								{printf("caselistaux2 -> caseclause caselistaux\n");}
			  		|   													{printf("caselistaux2 -> LAMBDA\n");}
					;

caseclause  		: atomiclist ':' callcommand 							{printf("caseclause -> atomiclist ':' callcommand\n");}
					;

expressionlist  	: expr expressionlistaux 								{printf("expressionlist -> expr expressionlistaux\n");}
					;

expressionlistaux  	: ',' expr expressionlistaux 							{printf("expressionlistaux -> ',' expr expressionlistaux\n");}
				  	|   													{printf("expressionlistaux -> LAMBDA\n");}
					;

expr				: andfact orfact										{printf("expr -> andfact orfact\n");}
					;

orfact				: OR andfact orfact										{printf("orfact -> OR andfact orfact\n");}
					|														{printf("orfact -> LAMBDA\n");}
					;

andfact				: notfact andfactaux									{printf("andfact -> notfact andfactaux\n");}
					;
		
andfactaux			: AND notfact andfactaux								{printf("andfactaux -> AND notfact andfactaux\n");}
					|														{printf("andfactaux -> LAMBDA\n");}
					;

notfact				: '!' expreq											{printf("notfact -> '!' expreq\n");}
					| expreq												{printf("notfact -> expreq\n");}
					;

expreq				: numericexpr expreqaux									{printf("expreq -> numericexpr expreqaux\n");}
					;

expreqaux			: EQUAL numericexpr expreqaux							{printf("expreqaux -> EQUAL numericexpr expreqaux\n");}
					| NOTEQ numericexpr expreqaux							{printf("expreqaux -> NOTEQ numericexpr expreqaux\n");}
					| '>' numericexpr expreqaux								{printf("expreqaux -> '>' numericexpr expreqaux\n");}
					| '<' numericexpr expreqaux								{printf("expreqaux -> '<' numericexpr expreqaux\n");}
					| LESSEQ numericexpr expreqaux							{printf("expreqaux -> LESSEQ numericexpr expreqaux\n");}
					| GREATEQ numericexpr expreqaux							{printf("expreqaux -> GREATEQ numericexpr expreqaux\n");}
					|														{printf("expreqaux -> LAMBDA\n");}
					;
			
numericexpr			: exprmul exprsum										{printf("numericexpr -> exprmul exprsum\n");}
					;

exprsum				: '+' exprmul exprsum									{printf("exprsum -> '+' exprmul exprsum\n");}
					| '-' exprmul exprsum									{printf("exprsum -> '-' exprmul exprsum\n");}
					|														{printf("exprsum -> LAMBDA\n");}
					;

exprmul				: simpleexpr exprmulaux									{printf("exprmul -> simpleexpr exprmulaux\n");}
					;
			
exprmulaux			: '*' simpleexpr exprmulaux								{printf("exprmulaux -> '*' simpleexpr exprmulaux\n");}
					| '/' simpleexpr exprmulaux								{printf("exprmulaux -> '/' simpleexpr exprmulaux\n");}
					|														{printf("exprmulaux -> LAMBDA\n");}
					;
			
simpleexpr			: atomic optrange										{printf("simpleexpr -> atomic optrange\n");}
					| optunary optbracket									{printf("simpleexpr -> optunary optbracket\n");}
					| '(' expr ')'											{printf("simpleexpr -> '(' expr ')'\n");}
					;
       	
optrange			: DOUBLEDOT atomic										{printf("optrange -> DOUBLEDOT atomic\n");}
					|														{printf("optrange -> LAMBDA\n");}
					;
        		
optunary			: '-'													{printf("optunary -> '-'\n");}
					| '+'													{printf("optunary -> '+'\n");}
					;
			
optbracket			: '(' expr ')'											{printf("optbracket -> '(' expr ')'\n");}
					| atomic												{printf("optbracket -> atomic\n");}
					;

idlist 				: ID idlistaux 											{printf("idlist -> ID idlistaux\n");}
					;

idlistaux 			: ',' ID idlistaux 										{printf("idlistaux -> ',' ID idlistaux\n");}
		 			| 														{printf("idlistaux -> LAMBDA\n");}
					;

type 				: INT 													{printf("type -> INT\n");}
 					| REAL 													{printf("type -> REAL\n");}
					| BOOL													{printf("type -> BOOL\n");}
 					| STRING 												{printf("type -> STRING\n");}
 					| ID 													{printf("type -> ID\n");}
 					;

literal				: INT_VALUE												{printf("literal -> INT_VALUE\n");}
					| REAL_VALUE											{printf("literal -> REAL_VALUE\n");}
					| HEXA_VALUE											{printf("literal -> HEXA_VALUE\n");}
					| BOOL_VALUE											{printf("literal -> BOOL_VALUE\n");}
					| STRING_VALUE											{printf("literal -> STRING_VALUE\n");}
					;

atomic				: literal												{printf("atomic -> literal\n");}
					| id													{printf("atomic -> id\n");}
					;

id					: ID idaux												{printf("id -> ID idaux\n");}
					;

idaux				: '[' expressionlist ']'								{printf("idaux -> '[' expressionlist ']'\n");}
					| '.' id												{printf("idaux -> '.' id\n");}
					| '(' idauxexpraux										{printf("idaux -> '(' idauxexpraux\n");}
					|														{printf("idaux -> LAMBDA\n");}
					;
					
idauxexpraux		: expressionlist ')'									{printf("idauxexpraux -> expressionlist ')'\n");}
					| ')'													{printf("idauxexpraux -> ')'\n");}
					;	
			
atomiclist  		: atomic atomiclistaux									{printf("atomiclist -> atomic atomiclistaux\n");}
					;
				
atomiclistaux		: ',' atomic atomiclistaux								{printf("atomiclistaux -> ',' atomic atomiclistaux\n");}
					|   													{printf("atomiclistaux -> LAMBDA\n");}
					;
		
%%                     /* C code */

void success() {
	printf("\033[0;32mparse success\033[0m\n"); 
	exit(1);
}

int main() {
	return yyparse();
}

void yyerror (char *s) {
	fprintf(stderr,"%s:", s);
	//fprintf(stderr,"Line: %d | Column: %d\n ", yylval.token->line, yylval.token->column);
	fprintf(stderr,"%d:%d: No expected '%s'\n", yylval.token->line, yylval.token->column, yylval.token->value);
}
