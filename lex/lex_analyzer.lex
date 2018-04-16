%{
	//LIBRARY
    #include "../token.c"
    
    //DEFINE
    #define YY_DECL Token* yylex(void)
    
	//VARIABLES
    int lines = 1;
    int column = 1;
%}

%{
	//FUNCTIONS AND PROCEDURES
    
	Token* baseBlock ( KEYWORD token_id, const char* name) {
	    
	    //Token *t = malloc(sizeof(Token));
	    
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

{SUM}		{ return baseBlock ( SUM, "SUM" ); } 
 
{MINOR}		{ return baseBlock ( MINOR, "MINOR" ); } 
 
{MULT}		{ return baseBlock ( MULT, "MULT" ); } 
 
{DIVISION}	{ return baseBlock ( DIVISION, "DIVISION" ); } 


%{
    //======================================
    
    //LOGIC OPERATOR
    
    //======================================
%}
 
{AND}		{ return baseBlock ( AND, "AND" ); } 
 
{OR}	    { return baseBlock ( OR, "OR" ); } 
 
{LESS}		{ return baseBlock ( LESS, "LESS" ); } 

{NOT}	    { return baseBlock ( NOT, "NOT" ); } 
 
{GREATER}	{ return baseBlock ( GREATER, "GREATER" ); } 
 
{LESSEQ}	{ return baseBlock ( LESSEQ, "LESSEQ" ); } 
 
{GREATEQ}	{ return baseBlock ( GREATEQ, "GREATEQ" ); } 
 
{EQUAL}		{ return baseBlock ( EQUAL, "EQUAL" ); } 
 
{NOTEQ}		{ return baseBlock ( NOTEQ, "NOTEQ" ); } 
			
%{
    //======================================
    
    //OTHER OPERATOR
    
    //======================================
%}
 
{SEMICOMMA}	{ return baseBlock ( SEMICOMMA, "SEMICOMMA" ); } 
 
{COMMA}		{ return baseBlock ( COMMA, "COMMA" ); } 
 
{COLON}		{ return baseBlock ( COLON,"COLON" ); } 
 
{LPARENT}	{ return baseBlock ( LPARENT, "LPARENT" ); } 
 
{RPARENT}	{ return baseBlock ( RPARENT, "RPARENT" ); } 

{LBRACKET}	{ return baseBlock ( LBRACKET, "LBRACKET" ); } 
 
{RBRACKET}	{ return baseBlock ( RBRACKET, "RBRACKET" ); } 
 
{DOUBLEDOT}	{ return baseBlock ( DOUBLEDOT, "DOUBLEDOT" ); } 

{DOT}	    { return baseBlock ( DOT, "DOT" ); } 
 
{ASSIGN}	{ return baseBlock ( ASSIGN, "ASSIGN" ); } 
 
{CASSIGN}	{ return baseBlock ( CASSIGN, "CASSIGN" ); } 

{ID}        { return baseBlock ( ID, "ID" ); }

[ \t]+      {
                /* eat up whitespace */
                column += yyleng;
            }

<<EOF>>     { return baseBlock ( FINAL, "FINAL" ); }   

.           { return baseBlock ( ERROR, "ERROR" ); }

%%

Token* getToken(){
    return yylex();
}

void openFile(int argc, char **argv){
    ++argv, --argc;  /* skip over program name */
	
	if ( argc > 0 ){
	        yyin = fopen( argv[0], "r" );
	}else{
	        yyin = stdin;
    }
}
