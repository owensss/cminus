%{
#define TEST

#ifdef TEST
#endif
#include <cstdio>
#include "include/cm_base.h"
#include "AST/Node.hpp"
extern int yylineno;
int yylex(void);  
void yyerror(const char* what);
int yyparse(void);
int yywrap();
using namespace cminus;
%}

%debug
%error-verbose
%union {
	int num;
	char* str;
	char op;
	Node_program* node_program;
	Node_declaration_list* node_declaration_list;
	Node_declaration* node_declaration;
	Node_var_declaration* node_var_declaration;
	cm_type type;
}

%token ELSE IF VOID WHILE
%token RETURN
%token INT
%token<num> NUM
%token<str> ID
%token GE LE UEQ EQU
%start program

%type<type> type_specifier;
%type<node_program> program
%type<node_declaration_list> declaration_list;
%type<node_declaration> declaration;
%type<node_var_declaration> var_declaration;
%%
program: declaration_list {$$ = new Node_program($1);}
			;

declaration_list: declaration_list declaration { $$ = new Node_declaration_list($1, $2); }
			| declaration { $$ = new Node_declaration_list($1); }
			;

declaration: var_declaration {$$ = $1;}
			| fun_declaration
			;

var_declaration: type_specifier ID ';' {
					printf("%d, %s\n", $1, $2);
					$$ = new Node_var_declaration($1, $2);
				 }
			| type_specifier ID '[' NUM ']' ';' {
					$$ = new Node_var_declaration($1, $2, $4);
				}
			;

type_specifier: INT {printf("int\n"); $$ = CM_INT;}
			| VOID {$$ = CM_VOID;}
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
	printf("%d:error:%s\n", yylineno, what);
}
