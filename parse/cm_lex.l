%{
#include "cm_yacc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef TEST
YYSTYPE yylval;
#define TEST_OUT printf("\"%s\"", yytext);
#else
#define TEST_OUT ;
#endif

%}

%option yylineno

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
		while ( (c=input()) != '*');
		while ( (c=input()) == '*' );
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

%%

int yywrap() {
	return 1;
}

#ifdef TEST
int main() {

	while (! (yylex() <= 0) );
}
#endif
