#ifndef _CONSTANTS_
#define _CONSTANTS_

enum class LEXEMA {
    HEXA_VALUE,		
    INT_VALUE,       
    REAL_VALUE,      
    BOOL_VALUE,       
    STRING_VALUE,
        
    ID,

    /*PRIMITIVE TYPES*/
    INT,
    REAL,
    BOOL,
    STRING,
    VECTOR,      

    /*KEYWORDS*/
    PROG,                
    INIT,              
    END,
    OF,
    VAR,
    LABEL,           
    STRUCT,         
    TYPE,            
    CONST,           
    RETURN,          
    JUMP,            
    FOR,             
    WHILE,           
    DO,              
    REPEAT,          
    UNTIL,            
    IF,              
    THEN,            
    ELSE,            
    CASE,            
    BE,             
    FUNC,            
    PROC,            
    BREAK,          
    CONTINUE,        
    REF,             
    
    /*ARITHMETIC OPERATOR*/
    SUM,             
    MINOR,           
    MULT,            
    DIVISION,         
    MOD,             
    
    /*LOGIC OPERATOR*/
    NOT,             
    AND,             
    OR,              
    LESS,            
    GREATER,         
    LESSEQ,          
    GREATEQ,         
    EQUAL,           
    NOTEQ,           
    
    /*OTHER OPERATOR*/
    SEMICOMMA,       
    COMMA,           
    COLON,           
    LPARENT,         
    RPARENT,         
    LBRACKET,        
    RBRACKET,        
    DOUBLEDOT,      
    QUOTE,           
    
    ASSIGN,          
    CASSIGN,
    
    /*ADDITIONS*/
    FINAL,
    ERROR
    
};

#endif