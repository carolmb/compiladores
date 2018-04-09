#ifndef _PREDETIVE_REC_
#define _PREDETIVE_REC_

#include <iostream>
#include <string>

class PredictiveRecursive{
private:
    Token* t = NULL;
public:    
    void error(std::string menssage, std::string function){
        std::cerr << "In function " << function << std::endl;
        std::cerr << "ERROR : " << menssage << std::endl;
        std::cerr << "Actual Token ( NAME : " << this->t->name << " VALUE : " << this->t->value  
                  << " LINE : " << this->t->line << " COLUMN : " << this->t->column << ")" << std::endl;
    
        exit(-1);
    }
    
    void advance(){
        delete t;
        t = getToken();
    }
    
    void eat(int key, std::string name){
        if(this->t!=NULL && this->t->key==key){
            this->advance();
        }else{
            this->error("Expected Token " + name, "eat");
        }    
    }
    
    void type(){
        
    }
    
    void range(){
        
    }
    
    void rangelistaux(){
        
    }
    
    //rangelist -> range rangelistaux 
    void rangelist(){
        switch (t->key){
            case INT_VALUE:
            case REAL_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
            case ID:
                this->range();
                this->rangelistaux();
            break;
            default:
                this->error("Expected Token '=', ';', END", "arraydecaux");
        }
    }
    
    void expressionlist(){
        
    }
    
    //arraydecaux -> "=" "(" expressionlist ")" | LAMBDA
    void arraydecaux(){
        switch (t->key){
            case '=' :
                this->eat('=',"=");
                this->eat('(',"(");
                this->expressionlist();
                this->eat(')',")");
            break;
            case ';':
            case END:
            break;
            default:
                this->error("Expected Token '=', ';', END", "arraydecaux");
        }
    }
    
    //arraydec -> "vetor" "[" rangelist "]" "de" type arraydecaux 
    void arraydec(){
        switch(t->key){
            case VECTOR:
                this->eat(VECTOR,"VECTOR");
                this->eat('[',"[");
                this->rangelist();
                this->eat(']',"]");
                this->eat(OF,"OF");
                this->type();
                this->arraydecaux();
            break;
            default:
                this->error("Expected Token VECTOR", "arraydec");
        }
    }
    
    void abstractiondec(){
        
    }
    
    void constdec(){
        
    }
    
    void labeldec(){
        
    }
    
    void usertype(){
        
    }
    
    void vardec(){
        
    }
    
    void prevcommand(){
        
    }
    
    //declaration -> vardec | usertype | labeldec | constdec | abstractiondec 
    void declaration(){
        switch(t->key){
            case VAR:
                this->vardec();
            break;
            case TYPE:
                this->usertype();
            break;
            case LABEL:
                this->labeldec();
            break;
            case CONST:
                this->constdec();
            break;
            case PROC:
            case FUNC:
                this->abstractiondec();
            break;
            default:
                error("Expected Token VAR, TYPE, LABEL, CONST, PROC, FUNC", "declaration");
        }
    }
    
    //block -> "inicio" prevcommand "fim"
    void block(){
        switch (t->key){
            case INIT:
                this->eat(INIT, "INIT");
                this->prevcommand();
                this->eat(END,"END");
            break;
            default:
                this->error("Expected Token INIT", "block");
        }
    }
    
    //prevdec -> declaration ";" prevdec | LAMBDA
    void prevdec(){
        
        switch (t->key){
            case INIT:
            case ';':
                
            break;
            case VAR:
            case LABEL:
            case TYPE:
            case CONST:
            case PROC:
            case FUNC:
                this->declaration();
                this->eat(';',";");
                this->prevdec();
            break;
            default:
                this->error("Expected Token INIT, ';' , VAR, LABEL, TYPE, CONST, PROC, FUNC","prevdec");
        }
    }
    
    //program -> "prog" "id" ";" prevdec block 
    void program(){ 
        
        this->t = getToken();
        
        switch (t->key){
            case PROG:
                this->eat(PROG, "PROG");
                this->eat(ID, "ID");
                this->eat(';',";");
                prevdec();
                block();
            break;
            default:
                this->error("Expected Token PROG", "program");
        }
    }
    
    void runRec(){
        this->program();
        
        this->eat(FINAL,"EOF");
    }
};    
/*


rangelist -> range rangelistaux 
rangelistaux -> "," range rangelistaux |  LAMBDA   
range -> atomic ".." atomic 
vardec -> "var" idlist ":" varconstruction
varconstruction ->  type decwithassign | arraydec
 
decwithassign -> "=" expression | LAMBDA
usertype -> "tipo" "id" typedec
typedec -> arraydec | ":=" typedecaux
typedecaux -> "registro" vardeclist "fim" | "value" | "(" idlist ")" | range

vardeclist -> vardec vardeclistaux 
vardeclistaux -> ";" vardec vardeclistaux | LAMBDA
labeldec -> "rotulo" idlist
constdec -> "const" "id" "=" expression
abstractiondec -> procdec | funcdec
procdec -> "proc" "id" "(" parameters ")" prevdec ";" block
funcdec -> "func" "id" "(" parameters ")" ":" type ";" prevdec block
parameters -> paramsaux | LAMBDA
paramsaux -> "id" ":" type paramslist   | "ref" "id" ":" type paramslist 
paramslist -> "," paramslist | LAMBDA
prevcommand -> commands | LAMBDA
callcommand ->  "pare" | write | read  | loop | block | return |  "continue" | id callidbegin | calllabel | conditional | block
commands -> callcommand commandsaux

commandsaux -> ";"  commands | LAMBDA



callidbegin  -> ":" callcommand  | ":=" expression | LAMBDA

calllabel -> "pule" "id"
write -> "escreva" "(" expressionlist ")"
read -> "leia" "(" expressionlist ")"
return -> "retorne" expression
loop -> forloop | whileloop | repeatloop
forloop -> "para" "(" forstruct ")" "faca" callcommand
forstruct -> prevfor ";" expression ";" posfor
prevfor -> varassignlist | LAMBDA
varassignlist -> assign varassignlistaux
  
varassignlistaux -> "," assign varassignlistaux | LAMBDA
posfor -> posforaux | LAMBDA
posforaux ->  commands posforaux2
posforaux2 -> "," commands posforaux2 | LAMBDA
whileloop -> "enquanto" "(" expression ")" "faca" callcommand
repeatloop -> "repita" commands "ate" expression
conditional -> ifcond | casecond
ifcond ->  "se" "(" expression ")" "entao" callcommand ifcondaux 
ifcondaux -> "senao" callcommand | LAMBDA

casecond -> "caso" "(" expression ")" "seja" caselist casecondaux
   
casecondaux -> "fim"| "senao" commands "fim"
   
caselist -> caseclause caselistaux
caselistaux -> ";" caselistaux2   | LAMBDA
caselistaux2 ->  caseclause caselistaux | LAMBDA
caseclause -> atomiclist ":" callcommand 

expressionlist -> expression expressionlistaux
expressionlistaux -> "," expression expressionlistaux
expression -> logicexpr 
logicexpr -> logicexprinter bla
bla -> exprand | expreq 
exprand -> "and" expreq exprand  | "or" expreq exprand  | LAMBDA
expreq -> "==" logicexprinter expreq | "!=" logicexprinter expreq | LAMBDA
   
exprleq -> ">" logicexprinter   | "<" logicexprinter   | ">=" logicexprinter  | "<=" logicexprinter   | LAMBDA
logicexprinter -> numericexpr exprleq |  "~" numericexpr exprleq 

numericexpr -> numericexprinter exprsum
exprsum -> "+" numericexprinter exprsum  | "-" numericexprinter exprsum | LAMBDA

numericexprinter -> simpleexpr exprmul
exprmul -> "*" simpleexpr exprmul | "/" simpleexpr exprmul  | LAMBDA
simpleexpr -> atomic optrange | optminus optbracket |  "(" logicexpr ")"
optrange -> ".." atomic | LAMBDA
optminus -> "-" | "+"
optbracket -> "(" logicexpr ")" | atomic

idlist -> "id" idlistaux 

idlistaux -> "," "id" idlistaux |LAMBDA
type -> "inteiro" | "real" | "logico" | "texto" | "id" 
literal -> "literalint" | "literalreal" | "literallogico" | "literaltexto"  
atomic -> literal | id
   
id -> "id" idaux
   
idaux -> "[" expressionlist "]" |  "." id |  "(" expressionlist ")" | LAMBDA
   

atomiclist -> atomic atomiclistaux
atomiclistaux ->  "," atomic atomiclistaux | LAMBDA


*/

#endif 