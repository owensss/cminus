%{
// #define TEST

#ifdef TEST
#define YYDEBUG 1
#endif
#include <cstdio>
#include "include/cm_base.h"
#include "AST/Node.hpp"
extern int yylineno;
extern int colnum;
int yylex(void);  
void yyerror(const char* what);
int yyparse(void);
int yywrap();
using namespace cminus;
%}

%debug
%error-verbose

%union {
	Node_program*               node_program;
	Node_declaration_list*      node_declaration_list;
	Node_declaration*           node_declaration;
	Node_var_declaration*       node_var_declaration;
	Node_fun_declaration*       node_fun_declaration;
	Node_params*                node_params;
	Node_param_list*            node_param_list;
	Node_param*                 node_param;
	Node_compound_stmt*         node_compound_stmt;
	Node_local_declarations*    node_local_declarations;
	Node_statement_list*        node_statement_list;
	Node_statement*             node_statement;
	Node_expression_stmt*       node_expression_stmt;
	Node_selection_stmt*        node_selection_stmt;
	Node_iteration_stmt*        node_iteraton_stmt;
	Node_return_stmt*           node_return_stmt;
	Node_expression*			node_expression;
	Node_var*                   node_var;
	Node_simple_expression*     node_simple_expression;
	Node_additive_expression*   node_additive_expression;
	Node_term*                  node_term;
	Node_factor*                node_factor;
	Node_call*                  node_call;
	Node_args*                  node_args;
	Node_arg_list*              node_arg_list;             
	cm_type                     type;
	cm_ops                      op;
	cm_relops                   relop;
	cm_int_type                 num;
	char*                       str;
}

%token ELSE IF VOID WHILE
%token RETURN
%token INT
%token<num> NUM
%token<str> ID
%token GE LE UEQ EQU
%start program
%type<type>                     type_specifier;
%type<node_program>             program;
%type<node_declaration_list>    declaration_list;
%type<node_declaration>         declaration;
%type<node_var_declaration>     var_declaration;
%type<node_fun_declaration>     fun_declaration; 
%type<node_params>              params;
%type<node_param_list>          param_list;
%type<node_param>               param;
%type<node_compound_stmt>       compound_stmt;
%type<node_local_declarations>  local_declarations;
%type<node_statement_list>      statement_list;
%type<node_statement>           statement;
%type<node_expression_stmt>     expression_stmt;
%type<node_selection_stmt>      selection_stmt;
%type<node_iteraton_stmt>       iteration_stmt;
%type<node_return_stmt>         return_stmt;
%type<node_expression>          expression;
%type<node_var>                 var;
%type<node_simple_expression>   simple_expression;
%type<node_additive_expression> additive_expression;
%type<node_term>                term;
%type<node_factor>              factor;
%type<node_call>                call;
%type<node_args>                args;
%type<node_arg_list>            arg_list;            
%type<op>                       addop mulop;
%type<relop>                    RELOP;

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
program: declaration_list {$$ = new Node_program($1); $$->generate();}
			;

declaration_list: declaration_list declaration { $$ = new Node_declaration_list($1, $2); }
			| declaration { $$ = new Node_declaration_list($1); }
			;

declaration: var_declaration {$$ = $1;}
			| fun_declaration {$$ = $1;}
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

fun_declaration: type_specifier ID '(' params ')' compound_stmt {
					printf("fun\n");
					$$ = new Node_fun_declaration($1, $2, $4, $6);
				 }
			;

params: param_list {$$ = new Node_params($1);}
			| VOID {$$ = new Node_params(NULL);}
			;

param_list: param_list ',' param {$$ = new Node_param_list($1, $3);}
			| param {$$ = new Node_param_list($1);}
			;

param: type_specifier ID {$$ = new Node_param($1, $2);}
			| type_specifier ID '[' ']' {$$ = new Node_param(CM_INT_ARRAY, $2);}
			;

compound_stmt:
		'{' local_declarations statement_list '}' {$$ = new Node_compound_stmt($2, $3);}
		;

local_declarations: local_declarations var_declaration {$$ = new Node_local_declarations($1, $2);}
			|  /* empty */ {$$ = new Node_local_declarations();}
			;

statement_list:statement_list statement {$$ = new Node_statement_list($1, $2);}
			|  {$$ = new Node_statement_list();}/* empty */
			;

statement: expression_stmt {$$ = $1;}
			| compound_stmt {$$ = $1;}
			| selection_stmt {$$ = $1;}
			| iteration_stmt {$$ = $1;}
			| return_stmt {$$ = $1;}
			;
	
expression_stmt: expression ';' {$$ = new Node_expression_stmt($1);}
			| ';' {$$ = new Node_expression_stmt(NULL);}
			;

selection_stmt: IF '(' expression ')' statement %prec LOWER_THAN_ELSE {$$ = new Node_selection_stmt($3, $5, false);}
			| IF '(' expression ')' statement ELSE statement {$$ = new Node_selection_stmt($3, $5, $7, true);}
			;

iteration_stmt: WHILE '(' expression ')' statement {$$ = new Node_iteration_stmt($3, $5);}
			;

return_stmt: RETURN ';' {$$ = new Node_return_stmt();}
			| RETURN expression ';' {$$ = new Node_return_stmt($2);}
			;

expression: var '=' expression {$$ = new Node_expression($1, $3);}
			| simple_expression {$$ = new Node_expression($1);}
			;

var: 		ID {$$ = new Node_var($1);}
			| ID '[' expression ']' {$$ = new Node_var($1, $3);}
			;

simple_expression:additive_expression RELOP additive_expression {$$ = new Node_simple_expression($1, $2, $3);}
			| additive_expression {$$ = new Node_simple_expression($1);}
			;

RELOP: GE   {$$ = cm_ge;}
     | '<'  {$$ = cm_lt;}
	 | '>'  {$$ = cm_gt;}
	 | LE   {$$ = cm_le;}
	 | EQU  {$$ = cm_equ;}
	 | UEQ  {$$ = cm_ueq;}
			;

additive_expression: additive_expression addop term {$$ = new Node_additive_expression($1, $2, $3);}
			| term {$$ = new Node_additive_expression($1);}
			;

addop: '+' {$$ = cm_plus;}
     | '-' {$$ = cm_minus;}
			;

term: term mulop factor {$$ = new Node_term($1, $2, $3);}
			| factor {$$ = new Node_term($1);}
			;

mulop: '*' {$$ = cm_multi;}
     | '/' {$$ = cm_div;}
			;

factor: '(' expression ')' {$$ = new Node_factor($2);}
			| var {$$ = new Node_factor($1);}
			| call {$$ = new Node_factor($1);}
			| NUM {$$ = new Node_factor($1);}
			;

call: ID '(' args ')' {$$ = new Node_call($1, $3);}
		;

args: arg_list {$$ = new Node_args($1);}
		|  {$$ = new Node_args(NULL);}
		;

arg_list: arg_list ',' expression  {$$ = new Node_arg_list($1, $3);}
		| expression {$$ = new Node_arg_list($1);}
		;
%%
extern FILE* yyin;
int main (int argc,char** argv) {
#ifdef TEST
	yydebug=1;
#endif
	if(argc>1)
	{
		if(!(yyin=fopen(argv[1],"r")))
		{
			printf("Error:Can't open file %s\n",argv[1]);
		}
	}
	else
	{
		if(!(yyin=fopen(".\\parse\\yacc_test.txt","r")))
		{
			printf("Error:Can't open file %s\n",argv[1]);
		}
	}
	if( yyparse() ) {
		printf("Okey\n");
	}
}

void yyerror(const char* what) {
	printf("%d:%d:error:%s\n", yylineno, colnum, what);
}
