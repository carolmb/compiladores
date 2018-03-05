%{
    /* need this for the call to atof() below */
    #include <math.h>
    
    int lines = 1;
    int column = 1;
%}

LINE            \n

INT_VALUE       [0-9]+
REAL_VALUE      [0-9]+"."[0-9]+
BOOL_VALUE      verdadeiro|falso  
STRING_VALUE    "'"[^']*"'"

ID              ([_a-zA-Z]+[_a-zA-Z0-9]*)+

/*PRIMITIVE TYPES*/
INT             inteiro
REAL            real
BOOL            logico
STRING          texto
VECTOR          vetor

/*KEYWORDS*/
PROG            prog                
BEGIN           inicio              
END             fim
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
BREAK           parar
CONTINUE        continue
REF             ref

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
LPARENT         "("
RPARENT         ")"
LBRACKET        "["
RBRACKET        "]"
DOUBLEDOT       ".."
QUOTE           "\'"

ASSIGN          "="
CASSIGN         ":="


COMMENT         \(\*[^\(]*\*\)
SCOMMENT        \*\*.*\n 

%%

{LINE}      {
                ++lines;
                column = 1;
            }

%{
    //======================================
    
    //COMMENT
    
    //======================================
%}

{COMMENT}   /* eat up mult-line comments */

{SCOMMENT}  /* eat up one-line comments */


%{
    //======================================
    
    //TYPE
    
    //======================================
%}

{INT}       {
                printf( "( INT, %s, %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }

{REAL}      {
                printf( "( REAL, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }
            
{BOOL}      {
                printf( "( BOOL, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }

{STRING}    {
                printf( "( STRING, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }		

{VECTOR}	{
				printf("( VECTOR, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}            

%{
    //======================================
    
    //VALUE
    
    //======================================
%}

{INT_VALUE}		{
                	printf( "( INT_VALUE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            	}

{REAL_VALUE}	{
                	printf( "( REAL_VALUE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            	}

{BOOL_VALUE}	{
                	printf( "( BOOL_VALUE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            	}

{STRING_VALUE}	{
                	printf( "( STRING_VALUE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            	}
            
%{
    //======================================
    
    //KEYWORDS
    
    //======================================
%}

{PROG}      {
                printf( "( PROG, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }
{BEGIN}		{
				printf("( BEGIN, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{END}		{
				printf("( END, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

{OF}		{
				printf("( OF, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{VAR}		{
				printf("( VAR, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{LABEL}		{
				printf("( LABEL, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{STRUCT}	{
				printf("( STRUCT, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{TYPE}		{
				printf("( TYPE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{CONST}		{
				printf("( CONST, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

{REF}	    {
				printf("( REF, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
%{
    //======================================
    
    //REPEAT STRUCTURE
    
    //======================================
%}
			
{JUMP}		{
				printf("( JUMP, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{FOR}		{
				printf("( FOR, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{WHILE}		{
				printf("( WHILE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{DO}		{
				printf("( DO, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{REPEAT}	{
				printf("( REPEAT, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{UNTIL}		{
				printf("( UNTIL, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{BREAK}		{
				printf("( BREAK, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{CONTINUE}	{
				printf("( CONTINUE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

%{
    //======================================
    
    //CONDITIONAL STRUCTURE
    
    //======================================
%}
	
			
{IF}		{
				printf("( IF, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{THEN}		{
				printf("( THEN, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{ELSE}		{
				printf("( ELSE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{CASE}		{
				printf("( CASE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{BE}		{
				printf("( BE, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

%{
    //======================================
    
    //ABSTRACTION
    
    //======================================
%}
			
{FUNC}		{
				printf("( FUNC, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
{PROC}		{
				printf("( PROC, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

{RETURN}	{
				printf("( RETURN, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

%{
    //======================================
    
    //ARITHMETIC OPERATOR 
    
    //======================================
%}

{SUM}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{MINOR}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{MULT}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{DIVSION}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{MOD}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{
				printf("(  AND, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{OR}		{
				printf("(  OR, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{LESS}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{GREATER}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{LESSEQ}	{
				printf("(  LESSEQ, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{GREATEQ}	{
				printf("(  GREATEQ, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{EQUAL}		{
				printf("(  EQUAL, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{NOTEQ}		{
				printf("(  NOTEQ, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{COMMA}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{COLON}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{LPARENT}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{RPARENT}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

{LBRACKET}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{RBRACKET}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{DOUBLEDOT}	{
				printf("(  DOUBLEDOT, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{QUOTE}		{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{ASSIGN}	{
				printf("( %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}
 
{CASSIGN}	{
				printf("( CASSIGN, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
			}

{ID}        { 
                printf( "( ID, %s,  %d, %d )\n", yytext, lines, column);
                column += yyleng;
            }

[ \t]+      {
                /* eat up whitespace */
                column += yyleng;
            }

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

