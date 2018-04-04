#include "lex/lex.yy.c"
#include "syntax/predictive_rec.c"
#include "syntax/predictive_table.c"
#include "semantics/sem.c"

int main( int argc, char **argv ) {
    
    openFile(argc, argv);
     
    Token *t = getToken();

    printf("( %s, %d, %d ) \n", t->value, t->line, t->column );  
    program2(t);         
}