main:
	flex lex/lex_analyzer.lex
	mv lex.yy.c lex/lex.yy.c
	g++ -Wall compiler.c constants.c -o compiler.o -lfl
	
yacc:
	cd syntax/yacc/ && make
	

