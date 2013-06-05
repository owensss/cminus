%{
#include <stdio.h>
extern int yylineno;
%}
%union {
	int num;
	char* str;
	char op;
}

%token ELSE IF VOID WHILE
%token RETURN
%token INT
%token<num> NUM
%token<str> ID
%token GE LE UEQ EQU
%start program
%%
program: declaration_list
			;

declaration_list: declaration_list declaration
			| declaration
			;

declaration: var_declaration
			| fun_declaration
			;

var_declaration: type_specifier ID ';'
			| type_specifier ID '[' NUM ']' ';'
			;

type_specifier: INT
			| VOID
			;

fun_declaration: type_specifier ID '(' params ')' compound_stmt
			;

params: param_list
			| VOID
			;

param_list: param_list ',' param
			| param
			;

param: type_specifier ID
			| type_specifier ID '[' ']'
			;

compound_stmt:
		'{' local_declarations statement_list '}'
		;

local_declarations: local_declarations var_declaration
			| 
			;

statement_list:statement_list statement
			| 
			;

statement: expression_stmt
			| compound_stmt
			| selection_stmt
			| iteration_stmt
			| return_stmt
			;
	
expression_stmt: expression ';'
			| ';'
			;

selection_stmt: IF '(' expression ')' statement
			| IF '(' expression ')' ELSE statement
			;

iteration_stmt: WHILE '(' expression ')' statement
			;

return_stmt: RETURN ';'
			| RETURN expression ';'
			;

expression: var '=' expression
			| simple_expression
			;

var: 		ID
			| ID '[' expression ']' 
			;

simple_expression:additive_expression RELOP additive_expression
			| additive_expression
			;

RELOP: GE | '<' | '>' | LE | EQU | UEQ
			;

additive_expression: additive_expression addop term 
			| term
			;

addop: '+' | '-'
			;

term: term mulop factor
			| factor
			;

mulop: '*' | '/'
			;

factor: '(' expression ')'
			| var
			| call 
			| NUM
			;

call: ID '(' args ')'
		;

args: arg_list
		| 
		;

arg_list: arg_list ',' expression 
		| expression
		;
%%
int main () {
	return yyparse();
}

void yyerror(const char* what) {
	printf("%d:error:%s", yylineno, what);
}
