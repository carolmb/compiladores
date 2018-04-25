%{
#include "../token.c"
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

expr		: andfact orfact					{;}
			;

orfact		: OR andfact orfact					{;}
			|									{;}
			;

andfact		: notfact andfactaux				{;}
			;
		
andfactaux	: AND notfact andfactaux			{;}
			|									{;}
			;

notfact		: '!' expreq						{;}
			| expreq							{;}
			;

expreq		: numericexpr expreqaux				{;}
			;

expreqaux	: EQUAL numericexpr expreqaux		{;}
			| NOTEQ numericexpr expreqaux		{;}
			| '>' numericexpr expreqaux		{;}
			| '<' numericexpr expreqaux		{;}
			| LESSEQ numericexpr expreqaux		{;}
			| GREATEQ numericexpr expreqaux		{;}
			|									{;}
			;
			
numericexpr	: exprmul exprsum					{;}
			;

exprsum		: '+' exprmul exprsum				{;}
			| '-' exprmul exprsum				{;}
			|									{;}
			;

exprmul		: simpleexpr exprmulaux				{;}
			;
			
exprmulaux	: '*' simpleexpr exprmulaux		{;}
			| '/' simpleexpr exprmulaux	{;}
			|									{;}
			;
			
simpleexpr	: atomic optrange					{;}
			| optunary optbracket				{;}
			| '(' expr ')'				{;}
			;
       	
optrange	: DOUBLEDOT atomic						{;}
			|									{;}
			;
        		
optunary	: '-'								{;}
			| '+'								{;}
			;
			
optbracket	: '(' expr ')'				{;}
			| atomic							{;}
			;

atomic		: literal							{;}
			| id								{;}
			;

literal		: INT_VALUE							{;}
			| REAL_VALUE						{;}
			| HEXA_VALUE						{;}
			| BOOL_VALUE						{;}
			| STRING_VALUE						{;}
			;

id			: ID idaux							{;}
			;

idaux		: '['  ']'	{;}
			| '.' id							{;}
			| '('  ')'	{;}
			|									{;}
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

