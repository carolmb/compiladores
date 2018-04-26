%{
#include "../../token.c"
%}

%{
void yyerror (char *s);
int yylex();
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>

int symbols[52];
int symbolVal(char symbol);
void updateSymbolVal(char symbol, int val);
%} 

%union {Token* token;}         /* Yacc definitions */
%start expr
%token exit_command

/*Terminals*/
%token HEXA_VALUE INT_VALUE REAL_VALUE FINAL BOOL_VALUE CONTINUE  ID INT REAL BOOL STRING VECTOR PROG INIT END OF VAR LABEL STRUCT TYPE CONST RETURN JUMP FOR WHILE DO REPEAT UNTIL IF THEN ELSE CASE BE FUNC PROC BREAK REF AND OR LESSEQ GREATEQ EQUAL NOTEQ DOUBLEDOT STRING_VALUE CASSIGN WRITE READ  

%%


expr		: andfact orfact					{printf("expr -> andfact orfact\n");}
			;

orfact		: OR andfact orfact					{printf("orfact -> OR andfact orfact\n");}
			|									{printf("orfact -> LAMBDA\n");}
			;

andfact		: notfact andfactaux				{printf("andfact -> notfact andfactaux\n");}
			;
		
andfactaux	: AND notfact andfactaux			{printf("andfactaux -> AND notfact andfactaux\n");}
			|									{printf("andfactaux -> LAMBDA\n");}
			;

notfact		: '!' expreq						{printf("notfact -> '!' expreq\n");}
			| expreq							{printf("notfact -> expreq\n");}
			;

expreq		: numericexpr expreqaux				{printf("expreq -> numericexpr expreqaux\n");}
			;

expreqaux	: EQUAL numericexpr expreqaux		{printf("expreqaux -> EQUAL numericexpr expreqaux\n");}
			| NOTEQ numericexpr expreqaux		{printf("expreqaux -> NOTEQ numericexpr expreqaux\n");}
			| '>' numericexpr expreqaux			{printf("expreqaux -> '>' numericexpr expreqaux\n");}
			| '<' numericexpr expreqaux			{printf("expreqaux -> '<' numericexpr expreqaux\n");}
			| LESSEQ numericexpr expreqaux		{printf("expreqaux -> LESSEQ numericexpr expreqaux\n");}
			| GREATEQ numericexpr expreqaux		{printf("expreqaux -> GREATEQ numericexpr expreqaux\n");}
			|									{printf("expreqaux -> LAMBDA\n");}
			;
			
numericexpr	: exprmul exprsum					{printf("numericexpr -> exprmul exprsum\n");}
			;

exprsum		: '+' exprmul exprsum				{printf("exprsum -> '+' exprmul exprsum\n");}
			| '-' exprmul exprsum				{printf("exprsum -> '-' exprmul exprsum\n");}
			|									{printf("exprsum -> LAMBDA\n");}
			;

exprmul		: simpleexpr exprmulaux				{printf("exprmul -> simpleexpr exprmulaux\n");}
			;
			
exprmulaux	: '*' simpleexpr exprmulaux			{printf("exprmulaux -> '*' simpleexpr exprmulaux\n");}
			| '/' simpleexpr exprmulaux			{printf("exprmulaux -> '/' simpleexpr exprmulaux\n");}
			|									{printf("exprmulaux -> LAMBDA\n");}
			;
			
simpleexpr	: atomic optrange					{printf("simpleexpr -> atomic optrange\n");}
			| optunary optbracket				{printf("simpleexpr -> optunary optbracket\n");}
			| '(' expr ')'						{printf("simpleexpr -> '(' expr ')'\n");}
			;
       	
optrange	: DOUBLEDOT atomic					{printf("optrange -> DOUBLEDOT atomic\n");}
			|									{printf("optrange -> LAMBDA\n");}
			;
        		
optunary	: '-'								{printf("optunary -> '-'\n");}
			| '+'								{printf("optunary -> '+'\n");}
			;
			
optbracket	: '(' expr ')'						{printf("optbracket -> '(' expr ')'\n");}
			| atomic							{printf("optbracket -> atomic\n");}
			;

atomic		: literal							{printf("atomic -> literal\n");}
			| id								{printf("atomic -> id\n");}
			;

literal		: INT_VALUE							{printf("literal -> INT_VALUE\n");}
			| REAL_VALUE						{printf("literal -> REAL_VALUE\n");}
			| HEXA_VALUE						{printf("literal -> HEXA_VALUE\n");}
			| BOOL_VALUE						{printf("literal -> BOOL_VALUE\n");}
			| STRING_VALUE						{printf("literal -> STRING_VALUE\n");}
			;

id			: ID idaux							{printf("id -> ID idaux\n");}
			;

idaux		: '['  ']'			{printf("idaux -> '[' expressionlist ']'\n");}
			| '.' id							{printf("idaux -> '.' id\n");}
			| '('  ')'			{printf("idaux -> '(' expressionlist ')'\n");}
			|									{printf("idaux -> LAMBDA\n");}
			;
			
			

%%                     /* C code */

int computeSymbolIndex(char token) {
	int idx = -1;
	if(islower(token)) {
		idx = token - 'a' + 26;
	} else if(isupper(token)) {
		idx = token - 'A';
	}
	return idx;
} 

/* returns the value of a given symbol */
int symbolVal(char symbol) {
	int bucket = computeSymbolIndex(symbol);
	return symbols[bucket];
}

/* updates the value of a given symbol */
void updateSymbolVal(char symbol, int val) {
	int bucket = computeSymbolIndex(symbol);
	symbols[bucket] = val;
}

int main (void) {

	/* init symbol table */
	
	int i;
	for(i=0; i<52; i++) {
		symbols[i] = 0;
	}

	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

