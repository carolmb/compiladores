#include "lex/lex.yy.c"
#include "syntax/predictive_rec.c"
#include "syntax/predictive_table.c"
#include "semantics/sem.c"

int main( int argc, char **argv ) {
    
    // std::cout << "NOT: " << NOT << " PROC: " << PROC << std::endl; 

    openFile(argc, argv);
    
    ++argv, --argc;  /* skip over program name */
    // chose which syntax to use
    if(strcmp(argv[1], "-rec") == 0){
        PredictiveRecursive comp;
        comp.runRec();
    }else if (strcmp(argv[1], "-table") == 0){
        // runTable();
        map<int, map<int, vector<int> > > mtx = readMatrix("syntax/spt.csv");
        runTable(mtx);
    }else{
        printf("Defina o tipo de analisador sintático\n");
    }
    
    return 0;
}        
