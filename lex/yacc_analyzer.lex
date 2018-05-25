%{
	//LIBRARY
    #include "../../token.c"
    #include <string>
    #include <vector>
    #include <utility>
    #include <string.h>
    
    #include "../../utilities.h"
    #include "y.tab.h"

    #include <stdlib.h>
    
	//VARIABLES
    int lines = 1;
    int column = 1;

    void yyerror (char *s); 

%}

%{
    
	int baseBlock ( int token_id, const char* name) {
	    
	    Token *t = new Token();
	    
	    t->key = token_id;
	    t->name = name;
	    t->value = strdup(yytext);
	    t->line = lines;
	    t->column = column;
	    
	    column += yyleng;
	    
	    yylval.token = t;
	    
	    return token_id;
	}
	
%}
%x COMMENT2

LINE            \n

HEXA_VALUE		0x[0-9A-F]+
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

/*KEYWORD*/
PROG            prog                
INIT           inicio              
END             fim
OF              de 
VAR             var
LABEL           rotulo
STRUCT          registro
TYPE            tipo
CONST           const
RETURN          retorne
JUMP            pule
FOR             para
WHILE           enquanto
DO              faca
REPEAT          repita
UNTIL           ate 
IF              se 
THEN            entao 
ELSE            senao 
CASE            caso
BE              seja 
FUNC            func
PROC            proc
BREAK           pare
CONTINUE        continue
REF             ref

/*ARITHMETIC OPERATOR*/
SUM             "+"
MINOR           "-"
MULT            "*"
DIVISION        "/"

/*LOGIC OPERATOR*/
NOT             "!"
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
DOT       "."

ASSIGN          "="
CASSIGN         ":="

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

"(*"                 BEGIN(COMMENT2);
<COMMENT2>[^)*\n]+
<COMMENT2>\n         ++lines;
<COMMENT2><<EOF>>    yyerror((char *)"EOF in comment");
<COMMENT2>"*)"       BEGIN(INITIAL);
<COMMENT2>[*)] 

{SCOMMENT}  /* eat up one-line comments */


%{
    //======================================
    
    //TYPE
    
    //======================================
%}

{INT}       { return baseBlock ( INT, "INT" ); }

{REAL}      { return baseBlock ( REAL, "REAL" ); }
            
{BOOL}      { return baseBlock ( BOOL, "BOOL" ); }

{STRING}    { return baseBlock ( STRING, "STRING" ); }	

{VECTOR}	{ return baseBlock ( VECTOR, "VECTOR" ); }    

%{
    //======================================
    
    //VALUE
    
    //======================================
%}

{HEXA_VALUE}	{ return baseBlock ( HEXA_VALUE, "HEXA_VALUE" ); } 

{INT_VALUE}		{ return baseBlock ( INT_VALUE, "INT_VALUE" ); }    

{REAL_VALUE}	{ return baseBlock ( REAL_VALUE, "REAL_VALUE" ); }    

{BOOL_VALUE}	{ return baseBlock ( BOOL_VALUE, "BOOL_VALUE" ); }    

{STRING_VALUE}	{ return baseBlock ( STRING_VALUE, "STRING_VALUE" ); }
    
%{
    //======================================
    
    //KEYWORD
    
    //======================================
%}

{PROG}      { return baseBlock ( PROG, "PROG" ); }    

{INIT}		{ return baseBlock ( INIT, "INIT"); }    
			
{END}	    { return baseBlock ( END, "END" ); }    

{OF}		{ return baseBlock ( OF, "OF" ); }    
			
{VAR}		{ return baseBlock ( VAR, "VAR" ); }    
			
{LABEL}		{ return baseBlock ( LABEL, "LABEL" ); }    
			
{STRUCT}	{ return baseBlock ( STRUCT, "STRUCT" ); }    
			
{TYPE}		{ return baseBlock ( TYPE, "TYPE" ); }    
			
{CONST}		{ return baseBlock ( CONST, "CONST" ); }    

{REF}	    { return baseBlock ( REF, "REF" ); }    
			
%{
    //======================================
    
    //REPEAT STRUCTURE
    
    //======================================
%}
			
{JUMP}		{ return baseBlock ( JUMP, "JUMP" ); }    
			
{FOR}		{ return baseBlock ( FOR, "FOR" ); }    
			
{WHILE}		{ return baseBlock ( WHILE, "WHILE" ); }    
			
{DO}		{ return baseBlock ( DO, "DO" ); }    
			
{REPEAT}	{ return baseBlock ( REPEAT, "REPEAT" ); }    
			
{UNTIL}		{ return baseBlock ( UNTIL, "ULTIL" ); }    
			
{BREAK}		{ return baseBlock ( BREAK, "BREAK" ); }    
			
{CONTINUE}	{ return baseBlock ( CONTINUE, "CONTINUE" ); }    

%{
    //======================================
    
    //CONDITIONAL STRUCTURE
    
    //======================================
%}
	
			
{IF}		{ return baseBlock ( IF, "IF" ); } 
			
{THEN}		{ return baseBlock ( THEN, "THEN" ); } 
			
{ELSE}		{ return baseBlock ( ELSE, "ELSE" ); } 
			
{CASE}		{ return baseBlock ( CASE, "CASE" ); } 
			
{BE}		{ return baseBlock ( BE, "BE" ); } 

%{
    //======================================
    
    //ABSTRACTION
    
    //======================================
%}
			
{FUNC}		{ return baseBlock ( FUNC, "FUNC" ); } 
			
{PROC}		{ return baseBlock ( PROC, "PROC" ); } 

{RETURN}	{ return baseBlock ( RETURN, "RETURN" ); } 

%{
    //======================================
    
    //ARITHMETIC OPERATOR 
    
    //======================================
%}

{SUM}		{return yytext[0];} 
 
{MINOR}		{return yytext[0];} 
 
{MULT}		{return yytext[0];} 
 
{DIVISION}	{return yytext[0]; } 


%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{ return baseBlock ( AND, "AND" ); } 
 
{OR}	    { return baseBlock ( OR, "OR" ); } 
 
{LESS}		{return yytext[0]; } 

{NOT}	    { return yytext[0];} 
 
{GREATER}	{ return yytext[0];} 
 
{LESSEQ}	{ return baseBlock ( LESSEQ, "LESSEQ" ); } 
 
{GREATEQ}	{ return baseBlock ( GREATEQ, "GREATEQ" ); } 
 
{EQUAL}		{ return baseBlock ( EQUAL, "EQUAL" ); } 
 
{NOTEQ}		{ return baseBlock ( NOTEQ, "NOTEQ" ); } 
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{ return yytext[0];} 
 
{COMMA}		{ return yytext[0]; } 
 
{COLON}		{return yytext[0];} 
 
{LPARENT}	{return yytext[0]; } 
 
{RPARENT}	{return yytext[0];} 

{LBRACKET}	{ return yytext[0]; } 
 
{RBRACKET}	{return yytext[0]; } 
 
{DOUBLEDOT}	{ return baseBlock ( DOUBLEDOT, "DOUBLEDOT" ); } 

{DOT}	    {return yytext[0];} 
 
{ASSIGN}	{ return yytext[0]; } 
 
{CASSIGN}	{ return baseBlock ( CASSIGN, "CASSIGN" ); } 

{ID}        { return baseBlock ( ID, "ID" ); }

[ \t]+      {
                /* eat up whitespace */
                column += yyleng;
            }

<<EOF>>     { return baseBlock ( FINAL, "FINAL" ); }   

.           { 	fprintf(stderr,"lexical error:%d:%d: No expected '%s'\n", lines, column, yytext); exit(1); }

%%

void openFile(int argc, char **argv){
    ++argv, --argc;  /* skip over program name */
	
	if ( argc > 0 ){
	        yyin = fopen( argv[0], "r" );
	}else{
	        yyin = stdin;
    }
}
