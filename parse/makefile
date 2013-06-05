yacc_test: cm_lex.c cm_yacc.c
	gcc -o yacc_test cm_yacc.c cm_lex.c -ly

lex_test: cm_lex.c cm_yacc.c
	gcc -o lex_test cm_lex.c -ly

cm_yacc.c: cm_yacc.y
	yacc -o cm_yacc.c -d cm_yacc.y

cm_lex.c: cm_lex.l
	lex -o cm_lex.c cm_lex.l

