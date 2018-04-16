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
    

    //range -> atomic '..' atomic 
    void range(){
        switch (t->key){
            case INT_VALUE:
            case REAL_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
            case ID:
                this->atomic();
                this->eat(DOUBLEDOT, "DOUBLEDOT");
                this->atomic();
            break;
            default:
                this->error("Expected Token INT_VALUE, REAL_VALUE, BOOL_VALUE, STRING_VALUE, ID ","range");
        }
    }
    
    //rangelistaux -> "," range rangelistaux |  LAMBDA   
    void rangelistaux(){
        switch (t->key){
            case ']' :
            break;
            case ',' :
                this->eat(',', ",");
                this->range();
                this->rangelistaux();
            break;
            default:
                this->error("Expected Token ']', ','", "arraydecaux");
        }
    }
    
    //rangelist -> range rangelistaux 
    void rangelist(){
        switch (t->key){
            case INT_VALUE:
            case REAL_VALUE:
            case BOOL_VALUE:
            case HEXA_VALUE:
            case STRING_VALUE:
            case ID:
                this->range();
                this->rangelistaux();
            break;
            default:
                this->error("Expected Token INT_VALUE, HEXA_VALUE, REAL_VALUE, BOOL_VALUE, STRING_VALUE, ID ","arraydecaux");
        }
    }
    
    //expressionlistaux -> ',' expr expressionlistaux 
    //expressionlistaux -> ''
    void expressionlistaux(){
        switch (t->key){
            case ',' :
                this->eat(',', ",");
                this->expr();
                this->expressionlistaux();
            break;
            case ']' :
            case ')' :
            break;
            default:
                this->error("Expected Token ',', ']', ')'", "expressionlistaux");
        }
    }
    
    //expressionlist -> expr expressionlistaux 
    void expressionlist(){
        switch (t->key){
            case ID:
            case '(' :
            case '!' :
            case '+' :
            case '-' :
            case INT_VALUE:
            case REAL_VALUE:
            case HEXA_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
                this->expr();
                this->expressionlistaux();
            break;
            default:
                this->error("Expected Token ID, '(', '!', '+', '-', INT_VALUE, HEXA_VALUE, REAL_VALUE, BOOL_VALUE, STRING_VALUE ","expressionlist");
        }
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
    
    //paramslist -> ',' paramsaux 
    //paramslist -> '' 
    void paramslist(){
        switch(t->key){
            case ',':
                this->eat(',',",");
                this->paramsaux();
            break;
            case ')' :
            break;
            default:
                this->error("Expected Token ',', ')'", "paramslist");
        }
    }
    
    //paramsaux -> 'id' ':' type paramslist 
    //paramsaux -> 'ref' 'id' ':' type paramslist 
    void paramsaux(){
        switch(t->key){
            case ID:
                this->eat(ID,"ID");
                this->eat(':',":");
                this->type();
                this->paramslist();
            break;
            case REF :
                this->eat(REF,"REF");
                this->eat(ID,"ID");
                this->eat(':',":");
                this->type();
                this->paramslist();
            break;
            default:
                this->error("Expected Token ID, REF", "paramsaux");
        }
    }
    
    //parameters -> paramsaux 
    //parameters -> '' 
    void parameters(){
        switch (t->key){
            case ID:
            case REF:
                this->paramsaux();
            break;
            case ')':
            break;
            default:
                this->error("Expected Token ID, REF, ')'", "parameters");
        }
    }
    
    //procdec -> 'proc' 'id' '(' parameters ')' prevdec block 
    void procdec(){
        switch (t->key){
            case PROC:
                this->eat(PROC, "PROC");
                this->eat(ID, "ID");
                this->eat('(', "(");
                this->parameters();
                this->eat(')', ")");
                this->prevdec();
                this->block();
            break;
            default:
                this->error("Expected Token PROC", "procdec");
        }
    }
    
    //funcdec -> 'func' 'id' '(' parameters ')' ':' type prevdec block 
    void funcdec(){
        switch (t->key){
            case FUNC:
                this->eat(FUNC, "FUNC");
                this->eat(ID, "ID");
                this->eat('(', "(");
                this->parameters();
                this->eat(')', ")");
                this->eat(':', ":");
                this->type();
                this->prevdec();
                this->block();
            break;
            default:
                this->error("Expected Token FUNC", "funcdec");
        }
    }
    
    //abstractiondec -> procdec 
    //abstractiondec -> funcdec 
    void abstractiondec(){
        switch (t->key){
            case PROC:
                this->procdec();
            break;
            case FUNC:
                this->funcdec();
            break;
            default:
                this->error("Expected Token PROC, FUNC", "abstractiondec");
        }
    }
    
    //constdec -> 'const' idlist ':' type
    void constdec(){
        switch (t->key){
            case CONST:
                this->eat(CONST, "CONST");
                this->idlist();
                this->eat(':', ":");
                this->type();
            break;
            default:
                this->error("Expected Token CONST", "constdec");
        }
    }
    
    //labeldec -> 'rotulo' idlist
    void labeldec(){
        switch (t->key){
            case LABEL:
                this->eat(LABEL, "LABEL");
                this->idlist();
            break;
            default:
                this->error("Expected Token LABEL", "labeldec");
        }
    }
    
    //vardeclistaux -> ';' vardec vardeclistaux 
    //vardeclistaux -> '' 
    void vardeclistaux(){
        switch (t->key){
            case ';':
                this->eat(';', ";");
                this->vardec();
                this->vardeclistaux();
            break;
            case END:
            break;
            default:
                this->error("Expected Token END, ';'", "vardeclistaux");
        }
    }
    
    //vardeclist -> vardec vardeclistaux 
    void vardeclist(){
        switch (t->key){
            case VAR:
                this->vardec();
                this->vardeclistaux();
            break;
            default:
                this->error("Expected Token VAR", "vardeclist");
        }
    }
    
    //typedecauxrange -> '..' atomic
    //typedecauxrange -> ''
    void typedecauxrange(){
        switch (t->key){
            case DOUBLEDOT:
                this->eat(DOUBLEDOT, "DOUBLEDOT");
                this->atomic();
            break;
            case ';':
            break;
            default:
                this->error("Expected Token DOUBLEDOT, ';'", "typedecauxrange");
        }
    }
    
    //typedecaux  -> 'registro' vardeclist 'fim'
    //typedecaux  -> atomic typedecauxrange
    //typedecaux  -> '(' idlist ')'
    void typedecaux(){
        switch (t->key){
            case STRUCT:
                this->eat(STRUCT, "STRUCT");
                this->vardeclist();
                this->eat(END, "END");
            break;
            case ID:
            case INT_VALUE:
            case REAL_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
            case HEXA_VALUE:
                this->atomic();
                this->typedecauxrange();
            break;
            case '(':
                this->eat('(', "(");
                this->idlist();
                this->eat(')', ")");
            break;
            default:
                this->error("Expected Token STRUCT, ID, INT_VALUE, REAL_VALUE, BOOL_VALUE, STRING_VALUE, HEXA_VALUE, '('", "typedecaux");
        }
    }
    
    //typedec -> arraydec 
    //typedec -> typedecaux 
    void typedec(){
        switch (t->key){
            case VECTOR:
                this->arraydec();
            break;
            case ID:
            case '(':
            case STRUCT :
            case INT_VALUE:
            case REAL_VALUE:
            case HEXA_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
                this->typedecaux();
            break;
            default:
                this->error("Expected Token VECTOR, ID, '(', STRUCT, INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "typedec");
        }
    }
    
    //usertype -> 'tipo' 'id' ':=' typedec 
    void usertype(){
        switch (t->key){
            case TYPE:
                this->eat(TYPE, "TYPE");
                this->eat(ID, "ID");
                this->eat(CASSIGN, "CASSIGN");
                this->typedec();
            break;
            default:
                this->error("Expected Token TYPE", "usertype");
        }
    }
    
    //optrange -> '..' atomic 
    //optrange -> '' 
    void optrange(){
        switch (t->key){
            case DOUBLEDOT :
                this->eat(DOUBLEDOT, "DOUBLEDOT");
                this->atomic();
            break;
            case ';' :
            case END :
            case ']' :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            case OR :
            case AND :
            case EQUAL :
            case NOTEQ :
            case '>' :
            case '<' :
            case GREATEQ :
            case LESSEQ :
            case '+' :
            case '-' :
            case '*' :
            case '/' :
            break;
            default:
                this->error("Expected Token DOUBLEDOT, '*', '/', '+', '-', ';', END, ']', ')', ',', UNTIL, ELSE, OR, AND, EQUAL, NOTEQ, '>', '<', GREATEQ, LESSEQ", "optrange");
        }  
    }
    
    //optunary -> '-' 
    //optunary -> '+'
    void optunary(){
        switch (t->key){
            case '+' :
                this->eat('+', "+");
            break;
            case '-' :
                this->eat('-', "-");
            break;
            default:
                this->error("Expected Token '+', '-'", "optunary");
        }  
    }
    
    //simpleexpr -> atomic optrange 
    //simpleexpr -> optunary optbracket 
    //simpleexpr -> '(' expr ')' 
    void simpleexpr(){
        switch (t->key){
            case ID :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->atomic();
                this->optrange();
            break;
            case '+' :
            case '-' :
                this->optunary();
                this->optbracket();
            break;
            case '(' :
                this->eat('(', "(");
                this->expr();
                this->eat(')', ")");
            break;
            default:
                this->error("Expected Token ID, '(', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "simpleexpr");
        }   
    }
    
    //exprmulaux -> '*' simpleexpr exprmulaux 
    //exprmulaux -> '/' simpleexpr exprmulaux 
    //exprmulaux -> '' 
    void exprmulaux(){
        switch (t->key){
            case '*' :
                this->eat('*', "*");
                this->simpleexpr();
                this->exprmulaux();
            break;
            case '/' :
                this->eat('/', "/");
                this->simpleexpr();
                this->exprmulaux();
            break;
            case ';' :
            case END :
            case ']' :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            case OR :
            case AND :
            case EQUAL :
            case NOTEQ :
            case '>' :
            case '<' :
            case GREATEQ :
            case LESSEQ :
            case '+' :
            case '-' :
            break;
            default:
                this->error("Expected Token '*', '/', '+', '-', ';', END, ']', ')', ',', UNTIL, ELSE, OR, AND, EQUAL, NOTEQ, '>', '<', GREATEQ, LESSEQ", "exprmulaux");
        }  
    }
    
    //exprmul -> simpleexpr exprmulaux 
    void exprmul(){
        switch (t->key){
            case ID :
            case '(' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->simpleexpr();
                this->exprmulaux();
            break;
            default:
                this->error("Expected Token ID, '(', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "exprmul");
        }   
    }
    
    //exprsum -> '+' exprmul exprsum 
    //exprsum -> '-' exprmul exprsum 
    //exprsum -> '' 
    void exprsum(){
        switch (t->key){
            case '+' :
                this->eat ('+', "+");
                this->exprmul();
                this->exprsum();
            break;
            case '-' :
                this->eat ('-', "-");
                this->exprmul();
                this->exprsum();
            break;
            case ';' :
            case END :
            case ']' :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            case OR :
            case AND :
            case EQUAL :
            case NOTEQ :
            case '>' :
            case '<' :
            case GREATEQ :
            case LESSEQ :
            break;
            default:
                this->error("Expected Token '+', '-', ';', END, ']', ')', ',', UNTIL, ELSE, OR, AND, EQUAL, NOTEQ, '>', '<', GREATEQ, LESSEQ", "exprsum");
        }
    }
    
    //numericexpr -> exprmul exprsum 
    void numericexpr(){
        switch (t->key){
            case ID :
            case '(' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->exprmul();
                this->exprsum();
            break;
            default:
                this->error("Expected Token ID, '(', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "numericexpr");
        }
    }
    
    //expreqaux -> '==' numericexpr expreqaux 
    //expreqaux -> '!=' numericexpr expreqaux 
    //expreqaux -> '>' numericexpr expreqaux 
    //expreqaux -> '<' numericexpr expreqaux 
    //expreqaux -> '>=' numericexpr expreqaux 
    //expreqaux -> '<=' numericexpr expreqaux 
    //expreqaux -> ''
    void expreqaux(){
        switch (t->key){
            case EQUAL :
                this->eat(EQUAL, "EQUAL");
                this->numericexpr();
                this->expreqaux();
            break;
            case NOTEQ :
                this->eat(NOTEQ, "NOTEQ");
                this->numericexpr();
                this->expreqaux();
            break;
            case '>' :
                this->eat('>', ">");
                this->numericexpr();
                this->expreqaux();
            break;
            case '<' :
                this->eat('<', "<");
                this->numericexpr();
                this->expreqaux();
            break;
            case GREATEQ :
                this->eat(GREATEQ, "GREATEQ");
                this->numericexpr();
                this->expreqaux();
            break;
            case LESSEQ :
                this->eat(LESSEQ, "LESSEQ");
                this->numericexpr();
                this->expreqaux();
            break;
            case ';' :
            case END :
            case ']' :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            case OR :
            case AND :
            break;
            default:
                this->error("Expected Token EQUAL, NOTEQ, '<', '>', GREATEQ, LESSEQ, ';', END, ']', ')', ',', UNTIL, ELSE, OR, AND", "expreqaux");
        }
    }
    
    //expreq -> numericexpr expreqaux 
    void expreq(){
        switch (t->key){
            case ID :
            case '(' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->numericexpr();
                this->expreqaux();
            break;
            default:
                this->error("Expected Token ID, '(', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "expreq");
        }
    }
    
    //notfact -> '!' expreq 
    //notfact -> expreq 
    void notfact(){
        switch (t->key){
            case '!' :
                this->eat('!', "!");
                this->expreq();
            break;
            case ID :
            case '(' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->expreq();
            break;
            default:
                this->error("Expected Token ID, '(', '!', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "notfact");
        }
    }
    
    //andfactaux -> 'and' notfact andfactaux 
    //andfactaux -> ''
    void andfactaux(){
        switch (t->key){
            case AND :
                this->eat(AND, "AND");
                this->notfact();
                this->andfactaux();
            break;
            case ';' :
            case END :
            case ']' :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            case OR :
            break;
            default:
                this->error("Expected Token AND, ';', END, ']', ')', ',', UNTIL, ELSE, OR", "andfactaux");
        }
    }
    
    //andfact -> notfact andfactaux 
    void andfact(){
        switch (t->key){
            case ID :
            case '(' :
            case '!' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->notfact();
                this->andfactaux();
            break;
            default:
                this->error("Expected Token ID, '(', '!', '+', '-', INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "andfact");
        }
    }
    
    //orfact -> 'or' andfact orfact 
    //orfact -> '' 
    void orfact(){
        switch (t->key){
            case OR :
                this->eat(OR, "OR");
                this->andfact();
                this->orfact();
            break;
            case ';':
            case END:
            case ')':
            case ',':
            case UNTIL:
            case ELSE:
            break;
            default:
                this->error("Expected Token OR, ';', END, ')', ',', UNTIL, ELSE", "orfact");
        }
    }
    
    //expr -> andfact orfact
    void expr(){
        switch (t->key){
            case ID :
            case '(' :
            case '!' :
            case '+' :
            case '-' :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->andfact();
                this->orfact();
            break;
            default:
                this->error("Expected Token ID, '(', '!', '+', '-',  INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "expr");
        }
    }
    
    //decwithassign -> '=' expr  ;
    //decwithassign -> '' ;
    void decwithassign() {
        switch (t->key){
            case ';':
            case END:
            break;
            case '=':
                this->eat('=', "=");
                this->expr();
            break;
            default:
                this->error("Expected Token ';', END", "decwithassign");
        }
    }
    
    //varconstruction ->  type decwithassign ;
    //varconstruction -> arraydec;
    void varconstruction(){
        switch (t->key){
            case VECTOR:
                this->arraydec();
            break;
            case ID:
            case INT:
            case REAL:
            case BOOL:
            case STRING:
                this->type();
                this->decwithassign();
            break;
            default:
                this->error("Expected Token VECTOR, ID, INT, REAL, BOOL, STRING", "varconstruction");
        }
    }
    
    //vardec -> 'var' idlist ':' varconstruction;
    void vardec(){
        switch (t->key){
            case VAR:
                this->eat(VAR, "VAR");
                this->idlist();
                this->eat(':', ":");
                this->varconstruction();
            break;
            default:
                this->error("Expected Token VAR", "vardec");
        }
    }
    
    //commandsaux -> ';' commands 
    //commandsaux -> '' 
    void commandsaux(){
        switch (t->key){
            case ';':
                this->eat(';', ";");
                this->commands();
            break;
            case END :
            case ')' :
            case ',' :
            case UNTIL :
            break;
            default:
                this->error("Expected Token ';', ',', ')', UNTIL", "commandsaux");
        }
    }
    
    //commands -> callcommand commandsaux 
    void commands(){
        switch(t->key){
            case ID:
            case INIT:
            case BREAK :
            case CONTINUE :
            case JUMP :
            case WRITE :
            case READ :
            case RETURN :
            case FOR :
            case WHILE :
            case REPEAT :
            case IF :
            case CASE :
                this->callcommand();
                this->commandsaux();
            break;
            default:
                this->error("Expected Token ID, INIT, BREAK, CONTINUE, JUMP, WRITE, READ, RETURN, FOR, WHILE, REPEAT, IF, CASE", "commands");
        }
    }
    
    //callidbegin -> ':' callcommand 
    //callidbegin -> ':=' expr 
    //callidbegin -> '' 
    void callidbegin(){
        switch (t->key){
            case ':':
                this->eat(':', ":");
                this->callcommand();
            break;
            case CASSIGN :
                this->eat(CASSIGN, "CASSIGN");
                this->expr();
            break;
            case ';' :
            case END :
            case ')' :
            case ',' :
            case UNTIL :
            case ELSE :
            break;
            default:
                this->error("Expected Token ':', ';', ',', ')', UNTIL, END, CASSIGN, ELSE", "callidbegin");
        }
    }
    
    //calllabel -> 'pule' 'id'
    void calllabel(){
        switch (t->key){
            case JUMP:
                this->eat(JUMP, "JUMP");
                this->eat(ID, "ID");
            break;
            default:
                this->error("Expected Token JUMP", "calllabel");
        }
    }
    
    //write -> 'escreva' '(' expressionlist ')' 
    void write(){
        switch (t->key){
            case WRITE:
                this->eat(WRITE, "WRITE");
                this->eat('(', "(");
                this->expressionlist();
                this->eat(')', ")");
            break;
            default:
                this->error("Expected Token WRITE", "write");
        }
    }
    
    //read -> 'leia' '(' expressionlist ')' 
    void read(){
        switch (t->key){
            case READ:
                this->eat(READ, "READ");
                this->eat('(', "(");
                this->expressionlist();
                this->eat(')', ")");
            break;
            default:
                this->error("Expected Token READ", "read");
        }
    }
    
    //returns -> 'retorne' expr 
    void returns(){
        switch (t->key){
            case RETURN:
                this->eat(RETURN, "RETURN");
                this->expr();
            break;
            default:
                this->error("Expected Token RETURN", "returns");
        }
    }
    
    //varassignlistaux -> ',' 'id' '=' expr varassignlistaux 
    //varassignlistaux -> '' 
    void varassignlistaux(){
        switch (t->key){
            case ',' :
                this->eat(',', ",");
                this->eat(ID, "ID");
                this->eat('=', "=");
                this->expr();
                this->varassignlistaux();
            break;
            case ';' :
            break;
            default:
                this->error("Expected Token ',', ';'", "varassignlistaux");
        }
    }
    
    //varassignlist -> 'id' '=' expr varassignlistaux 
    void varassignlist(){
        switch (t->key){
            case ID :
                this->eat(ID, "ID");
                this->eat('=', "=");
                this->expr();
                this->varassignlistaux();
            break;
            default:
                this->error("Expected Token ID", "varassignlist");
        }
    }
    
    //prevfor -> varassignlist 
    //prevfor -> '' 
    void prevfor(){
        switch (t->key){
            case ID :
                this->varassignlist();
            break;
            case ';' :
            break;
            default:
                this->error("Expected Token ';', ID", "prevfor");
        }
    }
    
    //posforaux2 -> ',' commands posforaux2 
    //posforaux2 -> '' 
    void posforaux2(){
        switch (t->key){
            case ',' :
                this->eat(',', ",");
                this->commands();
                this->posforaux2();
            break;
            case ')' :
            break;
            default:
                this->error("Expected Token ',', ')'", "posforaux2");
        }
    }
    
    //posforaux -> commands posforaux2 
    void posforaux(){
        switch(t->key){
            case ID:
            case INIT:
            case BREAK :
            case CONTINUE :
            case JUMP :
            case WRITE :
            case READ :
            case RETURN :
            case FOR :
            case WHILE :
            case REPEAT :
            case IF :
            case CASE :
                this->commands();
                this->posforaux2();
            break;
            default:
                this->error("Expected Token ID, INIT, BREAK, CONTINUE, JUMP, WRITE, READ, RETURN, FOR, WHILE, REPEAT, IF, CASE", "posforaux");
        }
    }
    
    //posfor -> posforaux 
    //posfor -> '' 
    void posfor(){
        switch (t->key){
            case ID :
            case INIT :
            case BREAK :
            case CONTINUE :
            case JUMP :
            case WRITE :
            case READ :
            case RETURN :
            case FOR :
            case WHILE :
            case REPEAT :
            case IF :
            case CASE :
                this->posforaux();
            break;
            case ')' :
            break;
            default:
                this->error("Expected Token ')', ID, INIT, BREAK, CONTINUE, JUMP, WRITE, READ, RETURN, FOR,WHILE, REPEAT, IF, CASE", "posfor");
        }
    }
    
    //forstruct -> prevfor ';' expr ';' posfor 
    void forstruct(){
        switch (t->key){
            case ID :
            case ';' :
            case ')' :
                this->prevfor();
                this->eat(';', ";");
                this->expr();
                this->eat(';', ";");
                this->posfor();
            break;
            default:
                this->error("Expected Token ID, ';', ')'", "forstruct");
        }
    }
    
    //forloop -> 'para' '(' forstruct ')' 'faca' callcommand 
    void forloop(){
        switch (t->key){
            case FOR:
                this->eat(FOR, "FOR");
                this->eat('(', "(");
                this->forstruct();
                this->eat(')', ")");
                this->eat(DO, "DO");
                this->callcommand();
            break;
            default:
                this->error("Expected Token FOR", "forloop");
        }
    }
    
    //whileloop -> 'enquanto' '(' expr ')' 'faca' callcommand 
    void whileloop(){
        switch (t->key){
            case WHILE:
                this->eat(WHILE, "WHILE");
                this->eat('(', "(");
                this->expr();
                this->eat(')', ")");
                this->eat(DO, "DO");
                this->callcommand();
            break;
            default:
                this->error("Expected Token WHILE", "whileloop");
        }
    }
    
    //repeatloop -> 'repita' commands 'ate' expr 
    void repeatloop(){
        switch (t->key){
            case REPEAT:
                this->eat(REPEAT, "REPEAT");
                this->commands();
                this->eat(UNTIL, "UNTIL");
                this->expr();
            break;
            default:
                this->error("Expected Token REPEAT", "repeatloop");
        }
    }
    
    //loop -> forloop 
    //loop -> whileloop 
    //loop -> repeatloop 
    void loop(){
        switch (t->key){
            case FOR :
                this->forloop();
            break;
            case WHILE :
                this->whileloop();
            break;
            case REPEAT :
                this->repeatloop();
            break;
            default:
                this->error("Expected Token FOR, WHILE, REPEAT", "loop");
        }
    }
    
    //ifcondaux -> 'senao' callcommand 
    //ifcondaux -> '' 
    void ifcondaux(){
        switch (t->key){
            case ELSE :
                this->eat(ELSE, "ELSE");
                this->callcommand();
            break;
            case ';' :
            case END :
            case ')' :
            case ',' :
            case UNTIL :
            break;
            default:
                this->error("Expected Token ELSE, ';', END, ')', ',', UNTIL", "ifcondaux");
        }
    }
    
    //ifcond -> 'se' '(' expr ')' 'entao' callcommand ifcondaux 
    void ifcond(){
        switch (t->key){
            case IF:
                this->eat(IF, "IF");
                 this->eat('(', "(");
                this->expr();
                this->eat(')', ")");
                this->eat(THEN, "THEN");
                this->callcommand();
                this->ifcondaux();
            break;
            default:
                this->error("Expected Token IF", "ifcond");
        }
    }
    
    //caseclause -> atomiclist ':' callcommand 
    void caseclause(){
        switch (t->key){
            case ID :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->atomiclist();
                this->eat(':', ":");
                this->callcommand();
            break;
            default:
                this->error("Expected Token ID", "caseclause");
        }
    }
    
    //caselistaux2 -> caseclause caselistaux 
    //caselistaux2 -> '' 
    void caselistaux2(){
        switch (t->key){
            case ID :
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->caseclause();
                this->caselistaux();
            break;
            case END :
            case ELSE :
            break;
            default:
                this->error("Expected Token ELSE, ';', END, ')', ',', UNTIL", "caselistaux2");
        }
    }
    
    //caselistaux -> ';' caselistaux2 
    //caselistaux -> ''
    void caselistaux(){
        switch (t->key){
            case ';' :
                this->eat(';', ";");
                this->caselistaux2();
            break;
            case END :
            case ELSE :
            break;
            default:
                this->error("Expected Token ELSE, ';', END", "caselistaux");
        }
    }
    
    //caselist -> caseclause caselistaux 
    void caselist(){
        switch (t->key){
            case ID:
            case INT_VALUE :
            case REAL_VALUE :
            case HEXA_VALUE :
            case BOOL_VALUE :
            case STRING_VALUE :
                this->caseclause();
                this->caselistaux();
            break;
            default:
                this->error("Expected Token ID, INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "caselist");
        }
    }
    
    //casecondaux -> 'fim' 
    //casecondaux -> 'senao' commands 'fim'
    void casecondaux(){
        switch (t->key){
            case END :
                this->eat(END, "END");
            break;
            case ELSE :
                this->eat(ELSE, "ELSE");
                this->commands();
                this->eat(END, "END");
            break;
            default:
                this->error("Expected Token END, ELSE", "casecondaux");
        }
    }
    
    //casecond -> 'caso' '(' expr ')' 'seja' caselist casecondaux 
    void casecond(){
        switch (t->key){
            case CASE:
                this->eat(CASE, "CASE");
                this->eat('(', "(");
                this->expr();
                this->eat(')', ")");
                this->eat(BE, "BE");
                this->caselist();
                this->casecondaux();
            break;
            default:
                this->error("Expected Token CASE", "casecond");
        }
    }
    
    //conditional -> ifcond 
    //conditional -> casecond 
    void conditional(){
        switch (t->key){
            case IF :
                this->ifcond();
            break;
            case CASE :
                this->casecond();
            break;
            default:
                this->error("Expected Token IF, CASE", "conditional");
        }
    }
    
    //callcommand -> 'pare' 
    //callcommand -> write 
    //callcommand -> read 
    //callcommand -> loop   
    //callcommand -> block 
    //callcommand -> returns 
    //callcommand -> 'continue' 
    //callcommand -> id callidbegin 
    //callcommand -> calllabel 
    //callcommand -> conditional
    void callcommand(){
        switch(t->key){
            case ID:
                this->id();
                this->callidbegin();
            break;
            case INIT:
                this->block();
            break;
            case BREAK :
                this->eat(BREAK, "BREAK");
            break;
            case CONTINUE :
                this->eat(CONTINUE, "CONTINUE");
            break;
            case JUMP :
                this->calllabel();
            break;
            case WRITE :
                this->write();
            break;
            case READ :
                this->read();
            break;
            case RETURN :
                this->returns();
            break;
            case FOR :
            case WHILE :
            case REPEAT :
                this->loop();
            break;
            case IF :
            case CASE :
                this->conditional();
            break;
            default:
                this->error("Expected Token ID, INIT, BREAK, CONTINUE, JUMP, WRITE, READ, RETURN, FOR, WHILE, REPEAT, IF, CASE", "callcommand");
        }
    }
    
    //prevcommand -> commands 
    //prevcommand -> ''
    void prevcommand(){
        switch(t->key){
            case ID:
            case INIT:
            case BREAK :
            case CONTINUE :
            case JUMP :
            case WRITE :
            case READ :
            case RETURN :
            case FOR :
            case WHILE :
            case REPEAT :
            case IF :
            case CASE :
                this->commands();
            break;
            case END :
            break;
            default:
                this->error("Expected Token ID, INIT, BREAK, CONTINUE, JUMP, WRITE, READ, RETURN, FOR, WHILE, REPEAT, IF, CASE, END", "prevcommand");
        }
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
            case TYPE:
            case LABEL:
            case CONST:
            case PROC:
            case FUNC:
                this->declaration();
                this->eat(';',";");
                this->prevdec();
            break;
            default:
                this->error("Expected Token ';' , VAR, LABEL, TYPE, CONST, PROC, FUNC","prevdec");
        }
    }
    
    //program -> "prog" "id" ";" prevdec block 
    void program(){ 
        switch (t->key){
            case PROG:
                this->eat(PROG, "PROG");
                this->eat(ID, "ID");
                this->eat(';',";");
                prevdec();
                this->block();
            break;
            default:
                this->error("Expected Token PROG", "program");
        }
    }
    
    void runRec(){
        
        this->t = getToken();
        this->program();
        
        this->eat(FINAL,"EOF");
    }
    
    
    //by Neto
    
    //atomiclistaux -> ',' atomic atomiclistaux 
    //atomiclistaux -> '' 
    void atomiclistaux(){
        switch(t->key){
            case ',':
                this->eat(',',",");
                this->atomic();
                this->atomiclistaux();
            break;
            case ':' :
                //DO NOTHING
            break;
            default :
                this->error("Expected Token ',' e ':' ", "atomiclistaux");
        }
        
    }
    
    //atomiclist -> atomic atomiclistaux 
    void atomiclist(){
        switch(t->key){
            case ID:
            case INT_VALUE:
            case REAL_VALUE:
            case HEXA_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
                this->atomic();
                this->atomiclistaux();
            break;
            default :
                this->error("Expected Token ID, INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE ", "atomiclist");
        }
        
    }
    
    //idaux -> '[' expressionlist ']' 
    //idaux -> '.' id 
    //idaux -> '(' expressionlist ')' 
    //idaux -> ''
    void idaux(){
        switch(t->key){
            case '[':
                this->eat('[',"[");
                this->expressionlist();
                this->eat(']',"]");
            break;
            case '.':
                this->eat('.', ".");
                this->id();
            break;
            case '(':
                this->eat('(', "(");
                this->expressionlist();
                this->eat(')',")");
            break;
            case ';':
            case FINAL:
            case ']':
            case ')':
            case ',':
            case DOUBLEDOT:
            case ':':
            case CASSIGN:
            case UNTIL:
            case ELSE:
            case OR:
            case AND:
            case EQUAL:
            case NOTEQ:
            case '>':
            case '<':
            case GREATEQ:
            case LESSEQ:
            case '+':
            case '-':
            case '*':
            case '/':
                //DO NOTHING
            break;
            default :
                this->error("Expected Token '[', '.', '(', ';', FINAL, ']', ')', ',', DOUBLEDOT, ':', CASSIGN, UNTIL, ELSE, OR, AND, EQUAL, NOTEQ, '<', '>', GREATEQ, LESSEQ, '+', '-', '*', '/' ", "idaux");
        }    
    }
    
    //id -> 'id' idaux 
    void id(){
        switch(t->key){
            case ID:
                this->eat(ID, "ID");
                this->idaux();
            break;
            default:
                this->error("Expected Token ID", "id");
        }
    }
    
    //atomic -> literal 
    //atomic -> id 
    void atomic(){
        switch(t->key){
            case ID :
                this->id();
            break;
            case INT_VALUE:
            case REAL_VALUE:
            case HEXA_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
                this->literal();
            break;    
            default:
                this->error("Expected Token ID, INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE" , "atomic");
        }
    }
    
    //literal -> 'literalint' 
    //literal -> 'literalreal'
    //literal -> 'literalhexa'
    //literal -> 'literallogico' 
    //literal -> 'literaltexto' 
    void literal(){
        switch(t->key){
            case INT_VALUE:
                this->eat(INT_VALUE, "INT_VALUE");
            break;
            case REAL_VALUE:
                this->eat(REAL_VALUE, "REAL_VALUE");
            break;
            case HEXA_VALUE:
                this->eat(HEXA_VALUE, "HEXA_VALUE");
            break;
            case BOOL_VALUE:
                this->eat(BOOL_VALUE, "BOOL_VALUE");
            break;
            case STRING_VALUE:
                this->eat(STRING_VALUE, "STRING_VALUE");
            break;
            default:
                this->error("Expected Token INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "literal");
        }
    }
    
    //type -> 'inteiro' 
    //type -> 'real' 
    //type -> 'logico' 
    //type -> 'texto' 
    //type -> 'id' 
    void type(){
        switch(t->key){
            case INT:
                this->eat(INT, "INT");
            break;
            case REAL:
                this->eat(REAL, "REAL");
            break;
            case BOOL:
                this->eat(BOOL, "BOOL");
            break;
            case STRING:
                this->eat(STRING, "STRING");
            break;
            case ID:
                this->eat(ID, "ID");
            break;    
            default:
                this->error("Expected Token INT, REAL, HEXA, BOOL, STRING", "type");
        }
    }
    
    //idlistaux -> ',' 'id' idlistaux 
    //idlistaux -> ''
    void idlistaux(){
        switch(t->key){
            case ',':
                this->eat(',',",");
                this->eat(ID, "ID");
                this->idlistaux();
            break;
            case ';':
            case ')':
            case ':':
                //DO NOTHING
            break;
            default:
                this->error("Expected Token ',', ';', ')', ':' ", "idlistaux");
            
        }
    }
    
    //idlist -> 'id' idlistaux
    void idlist(){
        switch(t->key){
            case ID:
                this->eat(ID, "ID");
                this->idlistaux();
            break;
            default:
               this->error("Expected Token ID", "idlist"); 
        }
    }
    
    //optbracket -> '(' expr ')' 
    //optbracket -> atomic
    void optbracket(){
        switch(t->key){
            case '(':
                this->eat('(', "(");
                this->expr();
                this->eat(')',")");
            break;
            case ID:
            case INT_VALUE:
            case REAL_VALUE:
            case HEXA_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
                this->atomic();
            break;
            default:
                this->error("Expected Token '(', ID, INT_VALUE, REAL_VALUE, HEXA_VALUE, BOOL_VALUE, STRING_VALUE", "optbracket"); 
        }
    }
};    

#endif 