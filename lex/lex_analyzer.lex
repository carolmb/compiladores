%{
/* need this for the call to atof() below */
#include <math.h>
%}

INT             [0-9]+
REAL            [0-9]+"."[0-9]+
BOOL            verdadeiro|falso  
STRING          "'"[^']*"'"

ID              ([_a-zA-Z]+[_a-zA-Z0-9]*)+

/*KEYWORDS*/
PROG            prog                
BEGIN           inicio              
END             fim
VECTOR          vetor
OF              de 
VAR             var
LABEL           rotulo
STRUCT          registro
TYPE            tipo
CONST           const
RETURN          retorne
JUMP            pular
FOR             para
WHILE           enquanto
DO              fazer
REPEAT          repetir
UNTIL           ate 
IF              se 
THEN            entao 
ELSE            senao 
CASE            caso
BE              seja 
FUNC            func
PROC            proc

/*ARITHMETIC OPERATOR*/
SUM             "+"
MINOR           "-"
MULT            "*"
DIVSION         "/"
MOD             "%"

/*LOGIC OPERATOR*/
AND             "&&"
OR              "||"
LESS            "<"
GREATER         ">"
LESSEQ          "<="
GREATEQ         ">="
EQUAL           "=="
NOTEQ           "!="


/*OTHER OPERATOR*/
SEMICOMMA       ";"
COMMA           ","
COLON           ":"
LBRACKET         "("
RBRACKET         ")"
DOUBLEDOT       ".."
QUOTE           "\'"

ASSIGN          "="
CASSIGN         ":="


COMMENT         \(\*[^\(]*\*\)

%%

%{
    //======================================
    
    //COMMENT
    
    //======================================
%}

{COMMENT}   /* eat up one-line comments */


%{
    //======================================
    
    //TYPE
    
    //======================================
%}

{INT}       {
                printf( "( INT, %s )\n", yytext);
            }

{REAL}      {
                printf( "( REAL, %s )\n", yytext);
            }

{BOOL}      {
                printf( "( BOOL, %s )\n", yytext);
            }

{STRING}    {
                printf( "( STRING, %s )\n", yytext );
            }

%{
    //======================================
    
    //KEYWORDS
    
    //======================================
%}

{PROG}      {
                printf( "( PROG, %s )\n", yytext );
            }

{BEGIN}		{
				printf("( BEGIN, %s )\n",yytext);
			}
			
{END}		{
				printf("( END, %s )\n",yytext);
			}
			
{VECTOR}	{
				printf("( VECTOR, %s )\n",yytext);
			}
			
{OF}		{
				printf("( OF, %s )\n",yytext);
			}
			
{VAR}		{
				printf("( VAR, %s )\n",yytext);
			}
			
{LABEL}		{
				printf("( LABEL, %s )\n",yytext);
			}
			
{STRUCT}	{
				printf("( STRUCT, %s )\n",yytext);
			}
			
{TYPE}		{
				printf("( TYPE, %s )\n",yytext);
			}
			
{CONST}		{
				printf("( CONST, %s )\n",yytext);
			}
			
{RETURN}	{
				printf("( RETURN, %s )\n",yytext);
			}
			
{JUMP}		{
				printf("( JUMP, %s )\n",yytext);
			}
			
{FOR}		{
				printf("( FOR, %s )\n",yytext);
			}
			
{WHILE}		{
				printf("( WHILE, %s )\n",yytext);
			}
			
{DO}		{
				printf("( DO, %s )\n",yytext);
			}
			
{REPEAT}	{
				printf("( REPEAT, %s )\n",yytext);
			}
			
{UNTIL}		{
				printf("( UNTIL, %s )\n",yytext);
			}
			
{IF}		{
				printf("( IF, %s )\n",yytext);
			}
			
{THEN}		{
				printf("( THEN, %s )\n",yytext);
			}
			
{ELSE}		{
				printf("( ELSE, %s )\n",yytext);
			}
			
{CASE}		{
				printf("( CASE, %s )\n",yytext);
			}
			
{BE}		{
				printf("( BE, %s )\n",yytext);
			}

%{
    //======================================
    
    //ABSTRACTION
    
    //======================================
%}
			
{FUNC}		{
				printf("( FUNC, %s )\n",yytext);
			}
			
{PROC}		{
				printf("( PROC, %s )\n",yytext);
			}

%{
    //======================================
    
    //OPERATOR
    
    //======================================
%}

{SUM}		{
				printf("( %s )\n",yytext);
			}
 
{MINOR}		{
				printf("( %s )\n",yytext);
			}
 
{MULT}		{
				printf("( %s )\n",yytext);
			}
 
{DIVSION}	{
				printf("( %s )\n",yytext);
			}
 
{MOD}		{
				printf("( %s )\n",yytext);
			}

%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{
				printf("(  AND, %s )\n",yytext);
			}
 
{OR}		{
				printf("(  OR, %s )\n",yytext);
			}
 
{LESS}		{
				printf("( %s )\n",yytext);
			}
 
{GREATER}	{
				printf("( %s )\n",yytext);
			}
 
{LESSEQ}	{
				printf("(  LESSEQ, %s )\n",yytext);
			}
 
{GREATEQ}	{
				printf("(  GREATEQ, %s )\n",yytext);
			}
 
{EQUAL}		{
				printf("(  EQUAL, %s )\n",yytext);
			}
 
{NOTEQ}		{
				printf("(  NOTEQ, %s )\n",yytext);
			}
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{
				printf("( %s )\n",yytext);
			}
 
{COMMA}		{
				printf("( %s )\n",yytext);
			}
 
{COLON}		{
				printf("( %s )\n",yytext);
			}
 
{LBRACKET}	{
				printf("( %s )\n",yytext);
			}
 
{RBRACKET}	{
				printf("( %s )\n",yytext);
			}
 
{DOUBLEDOT}	{
				printf("(  DOUBLEDOT, %s )\n",yytext);
			}
 
{QUOTE}		{
				printf("( %s )\n",yytext);
			}
 
{ASSIGN}	{
				printf("( %s )\n",yytext);
			}
 
{CASSIGN}	{
				printf("( CASSIGN, %s )\n",yytext);
			}

{ID}        { 
                printf( "( ID, %s )\n", yytext );
            }


[ \t\n]+    /* eat up whitespace */

.           printf( "Unrecognized character: %s\n", yytext );

%%

int main( int argc, char **argv ) {
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	        yyin = fopen( argv[0], "r" );
	else
	        yyin = stdin;

	yylex();
}

