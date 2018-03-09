%{
	//VARIABLES

    int lines = 1;
    int column = 1;
%}

%{
	//FUNCTIONS AND PROCEDURES
	
	void baseBlock ( const char* token_id ) {
	    printf( "( %s, %s, %d, %d )\n",token_id, yytext, lines, column);
        column += yyleng;
	}
	
	void simpleBaseBlock () {
	    printf( "( %s, %d, %d )\n", yytext, lines, column);
        column += yyleng;
	}
	
	void error () {
	    printf( "( (ERROR) Unrecognized character: %s, %d, %d )\n", yytext, lines, column);
        column += yyleng;
	}
%}


LINE            \n

HEXA_VALUE		0x[0-9]+
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
SCOMMENT        \*\*.* 

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

{INT}       { baseBlock ( "INT" );}

{REAL}      { baseBlock ( "REAL" );}
            
{BOOL}      { baseBlock ( "BOOL" );}

{STRING}    { baseBlock ( "STRING" );}	

{VECTOR}	{ baseBlock ( "VECTOR" );}         

%{
    //======================================
    
    //VALUE
    
    //======================================
%}

{HEXA_VALUE}	{ baseBlock ( "HEXA_VALUE" );} 

{INT_VALUE}		{ baseBlock ( "INT_VALUE" );}    

{REAL_VALUE}	{ baseBlock ( "REAL_VALUE" );}    

{BOOL_VALUE}	{ baseBlock ( "BOOL_VALUE" );}    

{STRING_VALUE}	{ baseBlock ( "STRING_VALUE" );}    
            
%{
    //======================================
    
    //KEYWORDS
    
    //======================================
%}

{PROG}      { baseBlock ( "PROG" );}    

{BEGIN}		{ baseBlock ( "BEGIN" );}    
			
{END}	    { baseBlock ( "END" );}    

{OF}		{ baseBlock ( "OF" );}    
			
{VAR}		{ baseBlock ( "VAR" );}    
			
{LABEL}		{ baseBlock ( "LABEL" );}    
			
{STRUCT}	{ baseBlock ( "STRUCT" );}    
			
{TYPE}		{ baseBlock ( "TYPE" );}    
			
{CONST}		{ baseBlock ( "CONST" );}    

{REF}	    { baseBlock ( "REF" );}    
			
%{
    //======================================
    
    //REPEAT STRUCTURE
    
    //======================================
%}
			
{JUMP}		{ baseBlock ( "JUMP" );}    
			
{FOR}		{ baseBlock ( "FOR" );}    
			
{WHILE}		{ baseBlock ( "WHILE" );}    
			
{DO}		{ baseBlock ( "DO" );}    
			
{REPEAT}	{ baseBlock ( "REPEAT" );}    
			
{UNTIL}		{ baseBlock ( "UNTIL" );}    
			
{BREAK}		{ baseBlock ( "BREAK" );}    
			
{CONTINUE}	{ baseBlock ( "CONTINUE" );}    

%{
    //======================================
    
    //CONDITIONAL STRUCTURE
    
    //======================================
%}
	
			
{IF}		{ baseBlock ( "IF" );} 
			
{THEN}		{ baseBlock ( "THEN" );} 
			
{ELSE}		{ baseBlock ( "ELSE" );} 
			
{CASE}		{ baseBlock ( "CASE" );} 
			
{BE}		{ baseBlock ( "BE" );} 

%{
    //======================================
    
    //ABSTRACTION
    
    //======================================
%}
			
{FUNC}		{ baseBlock ( "FUNC" );} 
			
{PROC}		{ baseBlock ( "PROC" );} 

{RETURN}	{ baseBlock ( "RETURN" );} 

%{
    //======================================
    
    //ARITHMETIC OPERATOR 
    
    //======================================
%}

{SUM}		{ simpleBaseBlock ();}
 
{MINOR}		{ simpleBaseBlock ();}
 
{MULT}		{ simpleBaseBlock ();}
 
{DIVSION}	{ simpleBaseBlock ();}
 
{MOD}		{ simpleBaseBlock ();}

%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{ baseBlock ( "AND" );} 
 
{OR}	    { baseBlock ( "OR" );} 
 
{LESS}		{ baseBlock ( "LESS" );} 
 
{GREATER}	{ baseBlock ( "GREATER" );} 
 
{LESSEQ}	{ baseBlock ( "LESSEQ" );} 
 
{GREATEQ}	{ baseBlock ( "GREATEQ" );} 
 
{EQUAL}		{ baseBlock ( "EQUAL" );} 
 
{NOTEQ}		{ baseBlock ( "NOTEQ" );} 
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{ simpleBaseBlock ();}
 
{COMMA}		{ simpleBaseBlock ();}
 
{COLON}		{ simpleBaseBlock ();}
 
{LPARENT}	{ simpleBaseBlock ();}
 
{RPARENT}	{ simpleBaseBlock ();}

{LBRACKET}	{ simpleBaseBlock ();}
 
{RBRACKET}	{ simpleBaseBlock ();}
 
{DOUBLEDOT}	{ baseBlock ( "DOUBLEDOT");}
 
{QUOTE}		{ simpleBaseBlock ();}
 
{ASSIGN}	{ simpleBaseBlock ();}
 
{CASSIGN}	{ baseBlock ( "CASSIGN" );}

{ID}        { baseBlock ( "ID" );}

[ \t]+      {
                /* eat up whitespace */
                column += yyleng;
            }

.           {error();}

%%

int main( int argc, char **argv ) {
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	        yyin = fopen( argv[0], "r" );
	else
	        yyin = stdin;

	yylex();
}

