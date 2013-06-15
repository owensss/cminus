CC=g++
CFLAGS=-Wall -ly -std=c++0x -std=c++0x -lfl
INCLUDE=-I..
PATH+=..

yacc_test: cm_lex.cpp cm_yacc.cpp ../AST/Node.cpp ../AST/Node.hpp
	$(CC) -o yacc_test cm_yacc.cpp cm_lex.cpp ../AST/Node.cpp $(CFLAGS) $(INCLUDE)

lex_test: cm_lex.cpp cm_yacc.cpp
	$(CC) -o lex_test cm_lex.cpp $(CFLAGS) $(INCLUDE)

cm_yacc.cpp: cm_yacc.y
	bison -o cm_yacc.cpp -t -d cm_yacc.y

cm_lex.cpp: cm_lex.l cm_yacc.y
	flex -o cm_lex.cpp cm_lex.l

clean:
	rm *.cpp *.hpp
