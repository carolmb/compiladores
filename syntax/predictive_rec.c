#ifndef _PREDETIVE_REC_
#define _PREDETIVE_REC_

#include <iostream>
#include <string>

const std::string RED("\033[0;31m");
const std::string GREEN("\033[0;32m");
const std::string RESET("\033[0m");

class PredictiveRecursive{
private:
    Token* t = NULL;
public:    
    void error(std::string menssage, std::string function){
        //std::cerr << "In function " << function << std::endl;
        
        if (this->t->key == ERROR){
            std::cout << RED << "LEXICAL ERROR!!! " << RESET << " (LEXEME: " << this->t->value  
                  << " | " << "LINE: " << this->t->line << " | " << "COLUMN: " << this->t->column << ")" << std::endl;
    
        }else{
            std::cout << RED << "SYNTAX ERROR!!!" << RESET << menssage << std::endl;
            std::cerr << "ACTUAL TOKEN (" << "LEXEME: " << this->t->value  
                  << " | " << "LINE: " << this->t->line << " | " << "COLUMN: " << this->t->column << ")" << std::endl;
        }
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
            this->error("EXPECTED TOKEN(S) ( " + name + " )", "eat");
        }    
    }
    

    //range -> atomic '..' atomic 
    void range(){
        switch (t->key){
            case INT_VALUE:
            case REAL_VALUE:
            case BOOL_VALUE:
            case STRING_VALUE:
            case HEXA_VALUE:
            case ID:
                this->atomic();
                this->eat(DOUBLEDOT, "DOUBLEDOT");
                this->atomic();
            break;
            default:
                this->error("EXPECTED TOKEN(S) ( literalint, literalreal, literallogico, literaltexto, literalhexa, id )","range");
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
                this->error("EXPECTED TOKEN(S) ( ']', ',' )", "arraydecaux");
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
                this->error("EXPECTED TOKEN(S) ( literalint, literalreal, literallogico, literaltexto, literalhexa, id )","arraydecaux");
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
                this->error("EXPECTED TOKEN(S) ( ',', ']', ')' )", "expressionlistaux");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '!', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )","expressionlist");
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
                this->error("EXPECTED TOKEN(S) ( '=', ';', 'fim' )", "arraydecaux");
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
                this->error("EXPECTED TOKEN(S) ( 'vetor' )", "arraydec");
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
                this->error("EXPECTED TOKEN(S) ( ',', ')'", "paramslist");
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
                this->error("EXPECTED TOKEN(S) ( id, 'ref' )", "paramsaux");
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
                this->error("EXPECTED TOKEN(S) ( id, 'ref', ')' )", "parameters");
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
                this->error("EXPECTED TOKEN(S) ( 'proc' )", "procdec");
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
                this->error("EXPECTED TOKEN(S) ( 'func' )", "funcdec");
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
                this->error("EXPECTED TOKEN(S) ( 'proc', 'func' )", "abstractiondec");
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
                this->error("EXPECTED TOKEN(S) ( 'const' )", "constdec");
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
                this->error("EXPECTED TOKEN(S) ( 'rotulo' )", "labeldec");
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
                this->error("EXPECTED TOKEN(S) ( 'fim', ';' )", "vardeclistaux");
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
                this->error("EXPECTED TOKEN(S) ( 'var' )", "vardeclist");
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
                this->error("EXPECTED TOKEN(S) ( '..', ';' )", "typedecauxrange");
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
                this->error("EXPECTED TOKEN(S) ( 'registro', id, literalint, literalreal, literallogico, literaltexto, literalhexa, '(' )", "typedecaux");
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
                this->error("EXPECTED TOKEN(S) ( 'vetor', id, '(', 'registro', literalint, literalreal, literallogico, literaltexto, literalhexa )", "typedec");
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
                this->error("EXPECTED TOKEN(S) ( 'tipo' )", "usertype");
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
                this->error("EXPECTED TOKEN(S) ( '..', '*', '/', '+', '-', ';', 'fim', ']', ')', ',', 'ate', 'senao', '||', '&&', '==', '!=', '>', '<', '>=', '<=' )", "optrange");
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
                this->error("EXPECTED TOKEN(S) ( '+', '-' )", "optunary");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "simpleexpr");
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
                this->error("EXPECTED TOKEN(S) ( '*', '/', '+', '-', ';', 'fim', ']', ')', ',', 'ate', 'senao', '||', '&&', '==', '!=', '>', '<', '>=', '<=' )", "exprmulaux");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "exprmul");
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
                this->error("EXPECTED TOKEN(S) ( '+', '-', ';', 'fim', ']', ')', ',', 'ate', 'senao', '||', '&&', '==', '!=', '>', '<', '>=', '<=' )", "exprsum");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "numericexpr");
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
                this->error("EXPECTED TOKEN(S) ( '==', '!=', '<', '>', '<=', '>=', ';', 'fim', ']', ')', ',', 'ate', 'senao', '||', '&&' )", "expreqaux");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "expreq");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '!', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "notfact");
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
                this->error("EXPECTED TOKEN(S) ( '&&', ';', 'fim', ']', ')', ',', 'ate', 'senao', '||' )", "andfactaux");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '!', '+', '-', literalint, literalreal, literallogico, literaltexto, literalhexa )", "andfact");
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
            case ']':
            case ',':
            case UNTIL:
            case ELSE:
            break;
            default:
                this->error("EXPECTED TOKEN(S) ( '||', ';', ']', 'fim', ')', ',', 'ate', 'senao' )", "orfact");
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
                this->error("EXPECTED TOKEN(S) ( id, '(', '!', '+', '-',  literalint, literalreal, literallogico, literaltexto, literalhexa )", "expr");
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
                this->error("EXPECTED TOKEN(S) ( ';', 'fim' )", "decwithassign");
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
                this->error("EXPECTED TOKEN(S) ( 'vetor', id, 'inteiro', 'real', 'logico', 'texto' )", "varconstruction");
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
                this->error("EXPECTED TOKEN(S) ( 'var' )", "vardec");
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
                this->error("EXPECTED TOKEN(S) ( ';', ',', ')', 'ate' )", "commandsaux");
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
                this->error("EXPECTED TOKEN(S) ( id, 'inicio', 'pare', 'continue', 'pule', 'escreva', 'leia', 'retorne', 'para', 'enquanto', 'repita', 'se', caso' )", "commands");
        }
    }
    
    //callidbegin -> ':' callcommand 
    //callidbegin -> '=' expr 
    //callidbegin -> '' 
    void callidbegin(){
        switch (t->key){
            case ':':
                this->eat(':', ":");
                this->callcommand();
            break;
            case '=' :
                this->eat('=', "=");
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
                this->error("EXPECTED TOKEN(S) ( ':', ';', ',', ')', 'ate', 'fim', '=', 'senao' )", "callidbegin");
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
                this->error("EXPECTED TOKEN(S) ( 'pule' )", "calllabel");
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
                this->error("EXPECTED TOKEN(S) ( 'escreva' )", "write");
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
                this->error("EXPECTED TOKEN(S) ( 'leia' )", "read");
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
                this->error("EXPECTED TOKEN(S) ( 'retorne' )", "returns");
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
                this->error("EXPECTED TOKEN(S) ( ',', ';' )", "varassignlistaux");
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
                this->error("EXPECTED TOKEN(S) ( id )", "varassignlist");
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
                this->error("EXPECTED TOKEN(S) ( ';', id )", "prevfor");
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
                this->error("EXPECTED TOKEN(S) ( ',', ')' )", "posforaux2");
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
                this->error("EXPECTED TOKEN(S) ( id, 'inicio', 'pare', 'continue', 'pule', 'escreva', 'leia', 'retorne', 'para', 'enquanto', 'repita', 'se', 'caso' )", "posforaux");
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
                this->error("EXPECTED TOKEN(S) ( ')', id, 'inicio', 'pare', 'continue', 'pule', 'escreva', 'leia', 'retorne', 'para', 'enquanto', 'repita', 'se', 'caso' )", "posfor");
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
                this->error("EXPECTED TOKEN(S) ( id, ';', ')' )", "forstruct");
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
                this->error("EXPECTED TOKEN(S) ( 'para' )", "forloop");
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
                this->error("EXPECTED TOKEN(S) ( 'enquanto' )", "whileloop");
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
                this->error("EXPECTED TOKEN(S) ( 'repita' )", "repeatloop");
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
                this->error("EXPECTED TOKEN(S) ( 'para', 'enquanto', 'repita' )", "loop");
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
                this->error("EXPECTED TOKEN(S) ( 'senao', ';', 'fim', ')', ',', 'ate' )", "ifcondaux");
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
                this->error("EXPECTED TOKEN(S) ( 'se' )", "ifcond");
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
                this->error("EXPECTED TOKEN(S) ( id )", "caseclause");
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
                this->error("EXPECTED TOKEN(S) ( 'senao', ';', 'fim', ')', ',', 'ate' )", "caselistaux2");
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
                this->error("EXPECTED TOKEN(S) ( 'senao', ';', 'fim' )", "caselistaux");
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
                this->error("EXPECTED TOKEN(S) ( id, literalint, literalreal, literallogico, literaltexto, literalhexa )", "caselist");
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
                this->error("EXPECTED TOKEN(S) ( 'fim', 'senao' )", "casecondaux");
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
                this->error("EXPECTED TOKEN(S) ( 'caso' )", "casecond");
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
                this->error("EXPECTED TOKEN(S) ( 'se', 'caso' )", "conditional");
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
                this->error("EXPECTED TOKEN(S) ( id, 'inicio', 'pare', 'continue', 'pule', 'escreva', 'leia', 'retorne', 'para', 'enquanto', 'repita', 'se', 'caso' )", "callcommand");
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
                this->error("EXPECTED TOKEN(S) ( id, 'inicio', 'pare', 'continue', 'pule', 'escreva', 'leia', 'retorne', 'para', 'enquanto', 'repita', 'se', 'caso', 'fim' )", "prevcommand");
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
                error("EXPECTED TOKEN(S) ( 'var', 'tipo', 'rotulo', 'const', 'proc', 'func' )", "declaration");
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
                this->error("EXPECTED TOKEN(S) ( 'inicio' )", "block");
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
                this->error("EXPECTED TOKEN(S) ('inicio', ';', 'var', 'rotulo', 'tipo', 'const', 'proc', 'func' )","prevdec");
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
                this->error("EXPECTED TOKEN(S) ( 'prog' )", "program");
        }
    }
    
    void runRec(){
        
        this->t = getToken();
        this->program();
        
        this->eat(FINAL,"EOF");
		std::cout << GREEN << "PROGRAM FINISHED" << RESET << std::endl;
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
                this->error("EXPECTED TOKEN(S) ( ',', ':' )", "atomiclistaux");
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
                this->error("EXPECTED TOKEN(S) ( id, literalint, literalreal, literallogico, literaltexto, literalhexa )", "atomiclist");
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
            case END:
            case ']':
            case ')':
            case ',':
            case DOUBLEDOT:
            case ':':
            case '=':
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
                this->error("EXPECTED TOKEN(S) ( '[', '.', '(', ';', 'fim', ']', ')', ',', '..', ':', '=', 'ate', 'senao', '||', '&&', '==', '!=', '<', '>', '>=', '<=', '+', '-', '*', '/' )", "idaux");
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
                this->error("EXPECTED TOKEN(S) ( id )", "id");
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
                this->error("EXPECTED TOKEN(S) ( id, literalint, literalreal, literallogico, literaltexto, literalhexa )" , "atomic");
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
                this->error("EXPECTED TOKEN(S) ( literalint, literalreal, literallogico, literaltexto, literalhexa )", "literal");
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
                this->error("EXPECTED TOKEN(S) ( 'inteiro', 'real', 'logico', 'texto' )", "type");
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
                this->error("EXPECTED TOKEN(S) ( ',', ';', ')', ':' )", "idlistaux");
            
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
               this->error("EXPECTED TOKEN(S) ( id )", "idlist"); 
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
                this->error("EXPECTED TOKEN(S) ( '(', id, literalint, literalreal, literallogico, literaltexto, literalhexa )", "optbracket"); 
        }
    }
};    

#endif 