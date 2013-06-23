CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -Wall -std=c++0x -ly -lfl

CFLAGS = -Wall -std=c++0x -g

OBJS = ./AST/Node.o ./parse/cm_yacc.o ./parse/cm_lex.o ./semanticanalysis/SemanticAnalysis.o \
	./symtable/symtable.o ./codegen/RegisterFile.o ./codegen/CodeGen.o

LIBS = 

TARGET = cminus 

INCLUDE = -I.

ATTRIBUTE_HEADER = ./attribute/FunctionTypeAttribute.h ./attribute/VariablePositionAttribute.h \
	./attribute/VariableTypeAttribute.h ./attribute/IdentifierAttribute.h

##################################################################################
all:	$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

##################################################################################
./AST/Node.o: ./AST/Node.cpp ./AST/Node.hpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)

./parse/cm_yacc.cpp: ./parse/cm_yacc.y
	bison -o $@ -t -d $<
	
./parse/cm_yacc.o: ./parse/cm_yacc.cpp ./parse/cm_yacc.hpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)

./parse/cm_lex.cpp: ./parse/cm_lex.l 
	flex -o $@ $<

./parse/cm_lex.o: ./parse/cm_lex.cpp ./parse/cm_yacc.hpp ./AST/Node.hpp ./include/cm_base.h
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)
./codegen/RegisterFile.o: ./codegen/RegisterFile.cpp ./codegen/RegisterFile.h ./attribute/VariablePositionAttribute.h
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)
./codegen/CodeGen.o: ./codegen/CodeGen.cpp ./codegen/GlobalState.h ./codegen/RegisterFile.h AST/Node.hpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE) 
./symtable/symtable.o: ./symtable/symtable.c ./symtable/symtable.h
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)
 ./semanticanalysis/SemanticAnalysis.o: ./semanticanalysis/SemanticAnalysis.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)
##################################################################################



########################################TEST#####################################
./parse/yacc_test: ./parse/cm_lex.cpp ./parse/cm_yacc.cpp ./AST/Node.cpp
	$(CXX) -o $@ $< $(CFLAGS) $(INCLUDE)

./parse/lex_test: ./parse/cm_lex.cpp ./parse/cm_yacc.cpp
	$(CXX) -o $@ $< $(CFLAGS) $(INCLUDE)
###############################################
./codegen/RegisterFile_test.o : ./codegen/RegisterFile_test.cpp ./codegen/RegisterFile.h
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)
RegisterFile_test: ./codegen/RegisterFile_test.o ./codegen/RegisterFile.o
	$(CXX) -o $@ $^ 

##################################################################################

clean:
	rm -f $(OBJS) $(TARGET) ./parse/cm_lex.cpp ./parse/cm_yacc.cpp ./parse/cm_yacc.hpp \
	./parse/yacc_test ./parse/lex_test ./codegen/*.o *.exe ./AST/*.o ./symtable/*.o dump.xml