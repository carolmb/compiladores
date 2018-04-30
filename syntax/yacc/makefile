calc: lex.yy.c y.tab.c
	gcc -g lex.yy.c y.tab.c -o yacc_syn -lfl

lex.yy.c: y.tab.c ../../lex/yacc_analyzer.lex
	lex ../../lex/yacc_analyzer.lex

y.tab.c: yacc_syntax.y
	yacc -d yacc_syntax.y

clean: 
	rm -rf lex.yy.c y.tab.c y.tab.h yacc_syn yacc_syn.dSYM

