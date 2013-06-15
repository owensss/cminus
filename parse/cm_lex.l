%{
#define YYDEBUG 1
#include "include/cm_base.h"
#include "AST/Node.hpp"
using namespace cminus;
#include "cm_yacc.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
int colnum = 0; // column number
static int nextcolnum = 0;
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
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return NUM;
 }

{key_else} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return ELSE;
}

{key_if} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return IF;
}

{key_int} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return INT;
}

{key_return} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return RETURN;
}

{key_void} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return VOID;
}

{key_while} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return WHILE;
}

{id} {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	yylval.str = strdup(yytext);
	return ID;
}

[+\-\*/<>=;,()\[\]{}] {
	TEST_OUT
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return *yytext;
}

"/*" { // skip comments
	int done = 0; // false
	do {
		nextcolnum+=2; // "/*"
		char c;
		// find *
		while ( (c=yyinput()) != '*') nextcolnum+=1;
		++nextcolnum; // for the last *
		while ( (c=yyinput()) == '*' ) nextcolnum +=1;
		++nextcolnum;
		if (c == '/') done = 1;
	} while (! done);
}

"!=" {
	TEST_OUT;
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return UEQ;
}

"<=" {
	TEST_OUT;
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return LE;
}

">=" {
	TEST_OUT;
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return GE;
}

"==" {
	TEST_OUT;
	colnum = nextcolnum+1;
	nextcolnum += yyleng;
	return EQU;
}


"\n" {nextcolnum = 0; colnum = 0; /* reset colnum */ }

. {nextcolnum += 1;}

%%


#ifdef TEST
int main() {

	while (! (yylex() <= 0) );
}
#endif

