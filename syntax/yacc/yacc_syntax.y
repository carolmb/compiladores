%{
#include "../../token.c"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>

void yyerror (char *s); 

int yylex();

void success();

void p(char *s);
void placeTab();

void add_tab();
void rem_tab();
int scopo;



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

program  			: PROG {p(" "); } ID {p("");} ';' {p(" ;\n");} prevdec {} block FINAL {success();}
					;

block  				: INIT {p("\n"); add_tab();} prevcommand {printf("\n"); rem_tab(); placeTab();} END {p("");}
					;

prevdec  			: declaration ';' {p(";\n"); } prevdec {}
		  			| {}
					;

declaration  		: vardec {}
			  		| usertype {}
			  		| labeldec {}
			  		| constdec {}
			  		| abstractiondec {}
					;

arraydec  			: VECTOR {p(" ");} '[' {p("[ ");} rangelist ']' {p("] ");} OF {p(" ");} type arraydecaux {}
					;

arraydecaux  		: '=' {p("= ");} '(' {p("( ");} expressionlist ')' {p(") ");} {}
			  		| {}
					;

rangelist  			: range rangelistaux {}
					;

rangelistaux  		: ',' {p(", ");} range rangelistaux {}
			  		| {}
					;

range  				: atomic DOUBLEDOT {p(" ");} atomic {}
					;

vardec  			: {placeTab();} VAR {p(" ");} idlist ':' {p(": ");} varconstruction {}
					;

varconstruction  	: type decwithassign {}
				  	| arraydec {}
					;

decwithassign  		: '=' {p("= ");} expr {}
			  		| {}
					;

usertype  			: {placeTab();} TYPE {p(" ");} ID {p(" ");} CASSIGN {p(" ");} typedec {}
					;

typedec  			: arraydec {}
		  			| typedecaux {}
					;

typedecaux  		: STRUCT {p("\n"); add_tab();} vardeclist {printf("\n"); rem_tab(); } END {p("");} {}
			  		| atomic typedecauxrange {}
			  		| '(' {p("( ");} idlist ')' {p(") ");} {}
					;

typedecauxrange		: DOUBLEDOT {p(" ");} atomic {}
					| {}
					;

vardeclist  		: vardec vardeclistaux {}
					;

vardeclistaux  		: ';' {p(";\n");} vardec vardeclistaux {}
			  		| {}
					;

labeldec  			: {placeTab();} LABEL {p(" ");} idlist {}
					;

constdec  			: {placeTab();} CONST {p(" ");} idlist ':' {p(": ");} type '=' {p("= ");} expr {}
					;

abstractiondec  	: procdec {}
				  	| funcdec {}
					;

procdec  			: {placeTab();} PROC {p(" ");} ID {p("");} '(' {p("( ");} parameters ')' {p(")\n");add_tab();} prevdec {rem_tab();} block 
					;

funcdec  			: {placeTab();} FUNC {p(" ");} ID {p("");} '(' {p("( ");} parameters ')' {p(") ");} ':' {p(": ");} type  {p("\n");add_tab();} prevdec {rem_tab();}  block 
					;

parameters  		: paramsaux {}
			  		| {}
					;

paramsaux  			: ID {p("");} ':' {p(": ");} type paramslist {}
		  			| REF {p(" ");} ID {p("");} ':' {p(": ");} type paramslist {}
					;

paramslist  		: ',' {p(", ");} paramsaux {}
			  		| {}
					;


prevcommand  		: commands {}
			  		| {}
					;

callcommand  		: {placeTab();} BREAK {p("");}		
			  		| {placeTab();} write {}
			  		| {placeTab();} read {}
			  		| loop {}
			  		| block {}
			  		| {placeTab();} return {}
			  		| {placeTab();} CONTINUE {p("");}
			  		| {placeTab();} id callidbegin {}
			  		| {placeTab();} calllabel {}
			  		| conditional {}
					;

commands  			: callcommand commandsaux {}
					;

commandsaux  		: ';' {p(";\n");} commands {}
			  		| {}
					;

callidbegin  		: ':' {p(":\n");} callcommand {}
			  		| '=' {p("= ");} expr {}
			  		| {}
					;

calllabel  			: JUMP {p(" ");} ID {p("");}
					;

write  				: WRITE {p("");} '(' {p("( ");} expressionlist ')' {p(")");}
					;

read  				: READ {p("");} '(' {p("( ");} expressionlist ')' {p(")");}
					;

return  			: RETURN {p(" ");} expr {}
					;

loop  				: {placeTab();} forloop {}
	  				| {placeTab();} whileloop {}
	  				| {placeTab();} repeatloop {}
					;

forloop  			: FOR {p("");} '(' {p("( ");} forstruct ')' {p(") ");} DO {p(" ");} callcommand 
					;

forstruct  			: prevfor ';' {p(" ");} expr ';' {p("; ");} posfor {}
					;

prevfor 			: varassignlist {}
		  			| {}
					;

varassignlist  		: ID {p(" ");} '=' {p("= ");} expr varassignlistaux {}
					;

varassignlistaux  	: ',' {p(", ");} ID {p(" ");} '=' {p("= ");} expr varassignlistaux {}
				  	| {}
					;

posfor  			: posforaux {}
		  			| {}
					;

posforaux  			: commands posforaux2 {}
					;

posforaux2  		: ',' {p(", ");} commands posforaux2 {}
			  		| {}
					;

whileloop  			: WHILE {p("");} '(' {p("( ");} expr ')' {p(") ");} DO {p(" ");} callcommand 
					;

repeatloop  		: REPEAT {p("\n");add_tab();} commands {rem_tab();printf("\n");placeTab();} UNTIL {p(" ");} expr {}
					;

conditional  		: {placeTab();} ifcond {}
			 		| {placeTab();} casecond {}
					;

ifcond  			: IF {p(" ");} '(' {p("( ");} expr ')' {p(") ");} THEN {p(" ");} callcommand {} ifcondaux {}
					;

ifcondaux  			: {printf("\n");placeTab();} ELSE {p(" ");} callcommand 
					| {}
					;
					
					
casecond  			:  CASE {p("");} '(' {p("( ");} expr ')' {p(") ");} BE {p("\n");add_tab();} caselist casecondaux 
					;

casecondaux  		: {printf("\n"); rem_tab(); placeTab();} END {p("");} {}
			  		| {placeTab();} ELSE {p("\n");}  commands {printf("\n");} END {p("");} {}
					;

caselist  			: caseclause caselistaux {}
					;

caselistaux  		: ';' {p(";\n");} caselistaux2 {}
			  		| {}
					;

caselistaux2  		: caseclause caselistaux {}
			  		| {}
					;

caseclause  		: {placeTab();} atomiclist ':' {p(":\n");add_tab();} callcommand {rem_tab();}
					;

expressionlist  	: expr expressionlistaux {}
					;

expressionlistaux  	: ',' {p(", ");} expr expressionlistaux {}
				  	| {}
					;

expr				: andfact orfact {}
					;

orfact				: OR {p(" ");} andfact orfact {}
					| {}
					;

andfact				: notfact andfactaux {}
					;
		
andfactaux			: AND {p(" ");} notfact andfactaux {}
					| {}
					;

notfact				: '!' {p("!");} expreq {}
					| expreq {}
					;

expreq				: numericexpr expreqaux {}
					;

expreqaux			: EQUAL {p(" ");} numericexpr expreqaux {}
					| NOTEQ {p(" ");} numericexpr expreqaux {}
					| '>' {p("> ");} numericexpr expreqaux {}
					| '<' {p("< ");} numericexpr expreqaux {}
					| LESSEQ {p(" ");} numericexpr expreqaux {}
					| GREATEQ {p(" ");} numericexpr expreqaux {}
					| {}
					;
			
numericexpr			: exprmul exprsum {}
					;

exprsum				: '+' {p("+ ");} exprmul exprsum {}
					| '-' {p("- ");} exprmul exprsum {}
					| {}
					;

exprmul				: simpleexpr exprmulaux {}
					;
			
exprmulaux			: '*' {p("* ");} simpleexpr exprmulaux {}
					| '/' {p("/ ");} simpleexpr exprmulaux {}
					| {}
					;
			
simpleexpr			: atomic optrange {}
					| optunary optbracket {}
					| '(' {p("( ");} expr ')' {p(") ");} {}
					;
       	
optrange			: DOUBLEDOT {p("");} atomic {}
					| {}
					;
        		
optunary			: '-' {p("- ");} {}
					| '+' {p("+ ");} {}
					;
			
optbracket			: '(' {p("( ");} expr ')' {p(") ");} {}
					| atomic {}
					;

idlist 				: ID {p(" ");} idlistaux {}
					;

idlistaux 			: ',' {p(", ");} ID {p(" ");} idlistaux {}
		 			| {}
					;

type 				: INT 			{p(" ");} {}
 					| REAL 			{p(" ");} {}
					| BOOL			{p(" ");} {}
 					| STRING 		{p(" ");} {}
 					| ID 			{p(" ");} {}
 					;

literal				: INT_VALUE		{p(" ");} {}
					| REAL_VALUE	{p(" ");} {}
					| HEXA_VALUE	{p(" ");} {}
					| BOOL_VALUE	{p(" ");} {}
					| STRING_VALUE	{p(" ");} {}
					;

atomic				: literal {}
					| id {}
					;

id					: ID {p("");} idaux {}
					;

idaux				: '[' {p("[ ");} expressionlist ']' {p("] ");} {}
					| '.' {p(".");} id {}
					| '(' {p("( ");} idauxexpraux {}
					| {printf(" ");}
					;
					
idauxexpraux		: expressionlist ')' {p(") ");} {}
					| ')' {p(") ");} {}
					;	
			
atomiclist  		: atomic atomiclistaux {}
					;
				
atomiclistaux		: ',' {p(", ");} atomic atomiclistaux {}
					| {}
					;
		
%%                     /* C code */

void success() {
	printf("\n\033[0;32mparse success\033[0m\n"); 
	exit(1);
}

void add_tab(){
	scopo++;
}
void rem_tab(){
	scopo--;
}

void placeTab(){
	int i;
	for (i = 0; i < scopo; i++){
		printf("\t");
	}
}

int main() {
	scopo = 0;
	return yyparse();
}

void p(char *s){
	printf("%s%s", yylval.token->value, s);
	yylval.token->value = "";
}

void yyerror (char *s) {
	fprintf(stderr,"%s:", s);
	fprintf(stderr,"%d:%d: No expected '%s'\n", yylval.token->line, yylval.token->column, yylval.token->value);
	exit(1); 
}
