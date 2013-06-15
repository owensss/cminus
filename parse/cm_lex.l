%{
#define YYDEBUG 1
#include "include/cm_base.h"
#include "AST/Node.hpp"
using namespace cminus;
#include "cm_yacc.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef TEST
YYSTYPE yylval;
#define TEST_OUT printf("\"%s\"", yytext);
#else
#define TEST_OUT ;
#endif

//Add for Mingw doesn't have such a function.
//Also it is not a c/c++ standard lib function.
#ifdef __MINGW32__
	static char* strdup(const char* str)
	{
		int len = strlen(str);
		char* retStr = (char*)malloc(len+1);
		strcpy(retStr,str);
		return retStr;
	}
#endif

%}
%option never-interactive
%option yylineno
%option noyywrap

digit [0-9]
num {digit}+
letter [a-zA-Z]
id {letter}+
key_else "else"
key_if "if"
key_int "int"
key_return "return"
key_void "void"
key_while "while"

%%
{num} {
	TEST_OUT
	yylval.num = atoi(yytext);
	return NUM;
 }

{key_else} {
	TEST_OUT
	return ELSE;
}

{key_if} {
	TEST_OUT
	return IF;
}

{key_int} {
	TEST_OUT
	return INT;
}

{key_return} {
	TEST_OUT
	return RETURN;
}

{key_void} {
	TEST_OUT
	return VOID;
}

{key_while} {
	TEST_OUT
	return WHILE;
}

{id} {
	TEST_OUT
	yylval.str = strdup(yytext);
	return ID;
}

[+\-\*/<>=;,()\[\]{}] {
	TEST_OUT
	return *yytext;
}

"/*" { // skip comments
	int done = 0; // false
	do {
		char c;
		// find *
		while ( (c=yyinput()) != '*');
		while ( (c=yyinput()) == '*' );
		if (c == '/') done = 1;
	} while (! done);
}

"!=" {
	TEST_OUT;
	return UEQ;
}

"<=" {
	TEST_OUT;
	return LE;
}

">=" {
	TEST_OUT;
	return GE;
}

"==" {
	TEST_OUT;
	return EQU;
}


"\n" {}

. {;}

%%


#ifdef TEST
int main() {

	while (! (yylex() <= 0) );
}
#endif
