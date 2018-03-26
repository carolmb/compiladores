%{
	//LIBRARY
	#include "constants.c"

	//STRUCTS
	struct Token {
	    LEXEMA key;
	    const char* name;
	    const char* value;
	    int line;
	    int column;
	};
    
    //DEFINE
    #define YY_DECL Token* yylex(void)
    
	//VARIABLES
    
    int lines = 1;
    int column = 1;
%}

%{
	//FUNCTIONS AND PROCEDURES
    
	Token* baseBlock ( LEXEMA token_id, const char* name) {
	    
	    Token *t = new Token;
	    
	    t->key = token_id;
	    t->name = name;
	    t->value = yytext;
	    t->line = lines;
	    t->column = column;
	    
	    column += yyleng;
	    
	    return t;
        
	}
	
%}


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

/*KEYWORDS*/
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
MOD             "%"

/*LOGIC OPERATOR*/
NOT             "~"
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

{INT}       { return baseBlock ( LEXEMA::INT, "INT" ); }

{REAL}      { return baseBlock ( LEXEMA::REAL, "REAL" ); }
            
{BOOL}      { return baseBlock ( LEXEMA::BOOL, "BOOL" ); }

{STRING}    { return baseBlock ( LEXEMA::STRING, "STRING" ); }	

{VECTOR}	{ return baseBlock ( LEXEMA::VECTOR, "VECTOR" ); }    

%{
    //======================================
    
    //VALUE
    
    //======================================
%}

{HEXA_VALUE}	{ return baseBlock ( LEXEMA::HEXA_VALUE, "HEXA_VALUE" ); } 

{INT_VALUE}		{ return baseBlock ( LEXEMA::INT_VALUE, "INT_VALUE" ); }    

{REAL_VALUE}	{ return baseBlock ( LEXEMA::REAL_VALUE, "REAL_VALUE" ); }    

{BOOL_VALUE}	{ return baseBlock ( LEXEMA::BOOL_VALUE, "BOOL_VALUE" ); }    

{STRING_VALUE}	{ return baseBlock ( LEXEMA::STRING_VALUE, "STRING_VALUE" ); }
    
%{
    //======================================
    
    //KEYWORDS
    
    //======================================
%}

{PROG}      { return baseBlock ( LEXEMA::PROG, "PROG" ); }    

{INIT}		{ return baseBlock ( LEXEMA::INIT, "INIT"); }    
			
{END}	    { return baseBlock ( LEXEMA::END, "END" ); }    

{OF}		{ return baseBlock ( LEXEMA::OF, "OF" ); }    
			
{VAR}		{ return baseBlock ( LEXEMA::VAR, "VAR" ); }    
			
{LABEL}		{ return baseBlock ( LEXEMA::LABEL, "LABEL" ); }    
			
{STRUCT}	{ return baseBlock ( LEXEMA::STRUCT, "STRUCT" ); }    
			
{TYPE}		{ return baseBlock ( LEXEMA::TYPE, "TYPE" ); }    
			
{CONST}		{ return baseBlock ( LEXEMA::CONST, "CONST" ); }    

{REF}	    { return baseBlock ( LEXEMA::REF, "REF" ); }    
			
%{
    //======================================
    
    //REPEAT STRUCTURE
    
    //======================================
%}
			
{JUMP}		{ return baseBlock ( LEXEMA::JUMP, "JUMP" ); }    
			
{FOR}		{ return baseBlock ( LEXEMA::FOR, "FOR" ); }    
			
{WHILE}		{ return baseBlock ( LEXEMA::WHILE, "WHILE" ); }    
			
{DO}		{ return baseBlock ( LEXEMA::DO, "DO" ); }    
			
{REPEAT}	{ return baseBlock ( LEXEMA::REPEAT, "REPEAT" ); }    
			
{UNTIL}		{ return baseBlock ( LEXEMA::UNTIL, "ULTIL" ); }    
			
{BREAK}		{ return baseBlock ( LEXEMA::BREAK, "BREAK" ); }    
			
{CONTINUE}	{ return baseBlock ( LEXEMA::CONTINUE, "CONTINUE" ); }    

%{
    //======================================
    
    //CONDITIONAL STRUCTURE
    
    //======================================
%}
	
			
{IF}		{ return baseBlock ( LEXEMA::IF, "IF" ); } 
			
{THEN}		{ return baseBlock ( LEXEMA::THEN, "THEN" ); } 
			
{ELSE}		{ return baseBlock ( LEXEMA::ELSE, "ELSE" ); } 
			
{CASE}		{ return baseBlock ( LEXEMA::CASE, "CASE" ); } 
			
{BE}		{ return baseBlock ( LEXEMA::BE, "BE" ); } 

%{
    //======================================
    
    //ABSTRACTION
    
    //======================================
%}
			
{FUNC}		{ return baseBlock ( LEXEMA::FUNC, "FUNC" ); } 
			
{PROC}		{ return baseBlock ( LEXEMA::PROC, "PROC" ); } 

{RETURN}	{ return baseBlock ( LEXEMA::RETURN, "RETURN" ); } 

%{
    //======================================
    
    //ARITHMETIC OPERATOR 
    
    //======================================
%}

{SUM}		{ return baseBlock ( LEXEMA::SUM, "SUM" ); } 
 
{MINOR}		{ return baseBlock ( LEXEMA::MINOR, "MINOR" ); } 
 
{MULT}		{ return baseBlock ( LEXEMA::MULT, "MULT" ); } 
 
{DIVISION}	{ return baseBlock ( LEXEMA::DIVISION, "DIVISION" ); } 
 
{MOD}		{ return baseBlock ( LEXEMA::MOD, "MOD" ); } 

%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{ return baseBlock ( LEXEMA::AND, "AND" ); } 
 
{OR}	    { return baseBlock ( LEXEMA::OR, "OR" ); } 
 
{LESS}		{ return baseBlock ( LEXEMA::LESS, "LESS" ); } 

{NOT}	    { return baseBlock ( LEXEMA::NOT, "NOT" ); } 
 
{GREATER}	{ return baseBlock ( LEXEMA::GREATER, "GREATER" ); } 
 
{LESSEQ}	{ return baseBlock ( LEXEMA::LESSEQ, "LESSEQ" ); } 
 
{GREATEQ}	{ return baseBlock ( LEXEMA::GREATEQ, "GREATEQ" ); } 
 
{EQUAL}		{ return baseBlock ( LEXEMA::EQUAL, "EQUAL" ); } 
 
{NOTEQ}		{ return baseBlock ( LEXEMA::NOTEQ, "NOTEQ" ); } 
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{ return baseBlock ( LEXEMA::SEMICOMMA, "SEMICOMMA" ); } 
 
{COMMA}		{ return baseBlock ( LEXEMA::COMMA, "COMMA" ); } 
 
{COLON}		{ return baseBlock ( LEXEMA::COLON,"COLON" ); } 
 
{LPARENT}	{ return baseBlock ( LEXEMA::LPARENT, "LPARENT" ); } 
 
{RPARENT}	{ return baseBlock ( LEXEMA::RPARENT, "RPARENT" ); } 

{LBRACKET}	{ return baseBlock ( LEXEMA::LBRACKET, "LBRACKET" ); } 
 
{RBRACKET}	{ return baseBlock ( LEXEMA::RBRACKET, "RBRACKET" ); } 
 
{DOUBLEDOT}	{ return baseBlock ( LEXEMA::DOUBLEDOT, "DOUBLEDOT" ); } 
 
{QUOTE}		{ return baseBlock ( LEXEMA::QUOTE, "QUOTE" ); } 
 
{ASSIGN}	{ return baseBlock ( LEXEMA::ASSIGN, "ASSIGN" ); } 
 
{CASSIGN}	{ return baseBlock ( LEXEMA::CASSIGN, "CASSIGN" ); } 

{ID}        { return baseBlock ( LEXEMA::ID, "ID" ); }

[ \t]+      {
                /* eat up whitespace */
                column += yyleng;
            }

<<EOF>>     { return baseBlock ( LEXEMA::FINAL, "FINAL" ); }   

.           { return baseBlock ( LEXEMA::ERROR, "ERROR" ); }

%%

int main( int argc, char **argv ) {
	++argv, --argc;  /* skip over program name */
	
	if ( argc > 0 ){
	        yyin = fopen( argv[0], "r" );
	}else{
	        yyin = stdin;
    }
    
    Token* t = yylex();
    
    while(t->key!=LEXEMA::FINAL){
        
        switch(t->key){
            case LEXEMA::ERROR:
                printf( "( (ERROR) Unrecognized character: %s, %d, %d )\n", t->value, lines, column);
            break;
            case LEXEMA::SUM:
            case LEXEMA::MINOR:
            case LEXEMA::MULT:
            case LEXEMA::DIVISION:
            case LEXEMA::MOD:
            case LEXEMA::SEMICOMMA:
            case LEXEMA::COMMA:
            case LEXEMA::COLON:
            case LEXEMA::LPARENT:
            case LEXEMA::RPARENT:
            case LEXEMA::LBRACKET:
            case LEXEMA::RBRACKET:
            case LEXEMA::DOUBLEDOT:
            case LEXEMA::QUOTE:
                 printf("( %s, %d, %d ) \n", t->value, t->line, t->column );   
            break;  
            default:
                printf("( %s, %s, %d, %d ) \n", t->name, t->value, t->line, t->column ); 
        }
        
        delete t;
        
        t = yylex();
        
    }
    
}

