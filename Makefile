main:
	flex lex/lex_analyzer.lex
	mv lex.yy.c lex/lex.yy.c
	g++ -Wall compiler.c -o compiler.o -lfl