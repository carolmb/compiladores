#ifndef _CONSTANTS_
#define _CONSTANTS_


typedef enum {
	NOT = '!',  
	HEXA_VALUE,		
    INT_VALUE,       
    REAL_VALUE, 
	FINAL,
	BOOL_VALUE,
	CONTINUE,
	LPARENT = '(',         
    RPARENT = ')',
    MULT = '*', 
    SUM = '+', 
    COMMA = ',',             
    MINOR = '-',           
    DOT = '.', //ADD NO DOC
    DIVISION = '/',         
    ID,
    INT,
    REAL,
    BOOL,
    STRING,
    VECTOR,      
    PROG,                   
    INIT,              
    END,
    OF,   
    COLON = ':',
    SEMICOMMA = ';',        
    LESS = '<',
    ASSIGN = '=',            
    GREATER = '>',         
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
    REF,    
    AND,             
    OR,                 
    LESSEQ,          
    GREATEQ,         
    EQUAL,           
    NOTEQ,  
    DOUBLEDOT,    
    LBRACKET = '[', 
    STRING_VALUE,
    RBRACKET = ']',                   
               
    CASSIGN,
    WRITE,
    READ,
    ERROR
    
}KEYWORD;

// namespace non_terminals{

typedef enum {
	PROGRAM_ = ERROR+1,
	BLOCK_,
	PREVDEC_,
	DECLARATION_,
	ARRAYDEC_,
	ARRAYDECAUX_,
	RANGELIST_,
	RANGELISTAUX_,
	RANGE_,
	VARDEC_,
	VARCONSTRUCTION_,
	DECWITHASSIGN_,
	USERTYPE_,
	TYPEDEC_,
	TYPEDECAUX_,
	TYPEDECAUXRANGE_,
	VARDECLIST_,
	VARDECLISTAUX_,
	LABELDEC_,
	CONSTDEC_,
	ABSTRACTIONDEC_,
	PROCDEC_,
	FUNCDEC_,
	PARAMETERS_,
	PARAMSAUX_,
	PARAMSLIST_,
	PREVCOMMAND_,
	CALLCOMMAND_,
	COMMANDS_,
	COMMANDSAUX_,
	CALLIDBEGIN_,
	CALLLABEL_,
	WRITE_,
	READ_,
	RETURN_,
	LOOP_,
	FORLOOP_,
	FORSTRUCT_,
	PREVFOR_,
	VARASSIGNLIST_,
	VARASSIGNLISTAUX_,
	POSFOR_,
	POSFORAUX_,
	POSFORAUX2_,
	WHILELOOP_,
	REPEATLOOP_,
	CONDITIONAL_,
	IFCOND_,
	IFCONDAUX_,
	CASECOND_,
	CASECONDAUX_,
	CASELIST_,
	CASELISTAUX_,
	CASELISTAUX2_,
	CASECLAUSE_,
	EXPRESSIONLIST_,
	EXPRESSIONLISTAUX_,
	EXPR_,
	ORFACT_,
	ANDFACT_,
	ANDFACTAUX_,
	NOTFACT_,
	EXPREQ_,
	EXPREQAUX_,
	NUMERICEXPR_,
	EXPRSUM_,
	EXPRMUL_,
	EXPRMULAUX_,
	SIMPLEEXPR_,
	OPTRANGE_,
	OPTUNARY_,
	OPTBRACKET_,
	IDLIST_,
	IDLISTAUX_,
	TYPE_,
	LITERAL_,
	ATOMIC_,
	ID_,
	IDAUX_,
	ATOMICLIST_,
	ATOMICLISTAUX_
} NONTERMINALS;

bool is_terminal(int value){
	if(value == 0) 
		return true;
	
	if(value >= NOT && value <= ERROR)
		return true;
	return false;
}

bool is_non_terminal(int value){
	if(value >= PROGRAM_ && value <= ATOMICLISTAUX_)
		return true;
	return false;
}

int idx(int value){
	return value - NOT;
}

#endif