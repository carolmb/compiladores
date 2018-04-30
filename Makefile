main:
	flex lex/lex_analyzer.lex
	mv lex.yy.c lex/lex.yy.c
	g++ -Wall compiler.c constants.c -o compiler.o -lfl
	
# yacc:
# 	lex lex/yacc_analyzer.lex 
# 	mv lex.yy.c syntax/yacc/
# 	cd syntax/yacc/ && yacc -d syntax/yacc/yacc_syntax.y
# 	cd syntax/yacc/ && gcc -g lex.yy.c y.tab.c -o yacc_syn -lfl
