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
	raw_token<cm_type>          type;
	raw_token<cm_ops>           op;
	raw_token<cm_relops>        relop;
	raw_token<cm_int_type>      num;
	raw_token<char*>            str;
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
program: declaration_list {$$ = new Node_program($1); $$->setLocation(yylineno, colnum);; $$->generate();}
			;

declaration_list: declaration_list declaration { $$ = new Node_declaration_list($1, $2); $$->setLocation(yylineno, colnum); }
			| declaration { $$ = new Node_declaration_list($1); $$->setLocation(yylineno, colnum); }
			;

declaration: var_declaration {$$ = $1;$$->setLocation(yylineno, colnum); }
			| fun_declaration {$$ = $1;$$->setLocation(yylineno, colnum); }
			;

var_declaration: type_specifier ID ';' {
					$$ = new Node_var_declaration($1, $2);
					$$->setLocation(yylineno, colnum); 
				 }
			| type_specifier ID '[' NUM ']' ';' {
					$$ = new Node_var_declaration($1, $2, $4);
					$$->setLocation(yylineno, colnum); 
				}
			;

type_specifier: INT { $$.value = CM_INT; $$.setLocation(yylineno, colnum);}
			| VOID {$$.value = CM_VOID; $$.setLocation(yylineno, colnum);}
			;

fun_declaration: type_specifier ID '(' params ')' compound_stmt {
					$$ = new Node_fun_declaration($1, $2, $4, $6);
					$$->setLocation(yylineno, colnum); 
				 }
			;

params: param_list {$$ = new Node_params($1);$$->setLocation(yylineno, colnum); }
			| VOID {$$ = new Node_params(NULL);$$->setLocation(yylineno, colnum); }
			;

param_list: param_list ',' param {$$ = new Node_param_list($1, $3);$$->setLocation(yylineno, colnum); }
			| param {$$ = new Node_param_list($1);$$->setLocation(yylineno, colnum); }
			;

param: type_specifier ID {$$ = new Node_param($1, $2);$$->setLocation(yylineno, colnum); }
			| type_specifier ID '[' ']' {$$ = new Node_param(raw_token<cm_type>{CM_INT_ARRAY, $1.row, $1.col}, $2);$$->setLocation(yylineno, colnum); }
			;

compound_stmt:
		'{' local_declarations statement_list '}' {$$ = new Node_compound_stmt($2, $3);$$->setLocation(yylineno, colnum); }
		;

local_declarations: local_declarations var_declaration {$$ = new Node_local_declarations($1, $2);}
			|  /* empty */ {$$ = new Node_local_declarations();$$->setLocation(yylineno, colnum); }
			;

statement_list:statement_list statement {$$ = new Node_statement_list($1, $2);$$->setLocation(yylineno, colnum); }
			|  {$$ = new Node_statement_list();$$->setLocation(yylineno, colnum); }/* empty */
			;

statement: expression_stmt {$$ = $1;$$->setLocation(yylineno, colnum); }
			| compound_stmt {$$ = $1;$$->setLocation(yylineno, colnum); }
			| selection_stmt {$$ = $1;$$->setLocation(yylineno, colnum); }
			| iteration_stmt {$$ = $1;$$->setLocation(yylineno, colnum); }
			| return_stmt {$$ = $1;$$->setLocation(yylineno, colnum); }
			;
	
expression_stmt: expression ';' {$$ = new Node_expression_stmt($1);$$->setLocation(yylineno, colnum); }
			| ';' {$$ = new Node_expression_stmt(NULL);$$->setLocation(yylineno, colnum); }
			;

selection_stmt: IF '(' expression ')' statement %prec LOWER_THAN_ELSE {$$ = new Node_selection_stmt($3, $5, false);$$->setLocation(yylineno, colnum); }
			| IF '(' expression ')' statement ELSE statement {$$ = new Node_selection_stmt($3, $5, $7, true);$$->setLocation(yylineno, colnum); }
			;

iteration_stmt: WHILE '(' expression ')' statement {$$ = new Node_iteration_stmt($3, $5);$$->setLocation(yylineno, colnum); }
			;

return_stmt: RETURN ';' {$$ = new Node_return_stmt();$$->setLocation(yylineno, colnum); }
			| RETURN expression ';' {$$ = new Node_return_stmt($2);$$->setLocation(yylineno, colnum); }
			;

expression: var '=' expression {$$ = new Node_expression($1, $3);$$->setLocation(yylineno, colnum); }
			| simple_expression {$$ = new Node_expression($1);$$->setLocation(yylineno, colnum); }
			;

var: 		ID {$$ = new Node_var($1);$$->setLocation(yylineno, colnum); }
			| ID '[' expression ']' {$$ = new Node_var($1, $3);$$->setLocation(yylineno, colnum); }
			;

simple_expression:additive_expression RELOP additive_expression {$$ = new Node_simple_expression($1, $2, $3);$$->setLocation(yylineno, colnum); }
			| additive_expression {$$ = new Node_simple_expression($1);$$->setLocation(yylineno, colnum); }
			;

RELOP: GE   {$$.value = cm_ge;$$.setLocation(yylineno, colnum);}
     | '<'  {$$.value = cm_lt;$$.setLocation(yylineno, colnum);}
	 | '>'  {$$.value = cm_gt;$$.setLocation(yylineno, colnum);}
	 | LE   {$$.value = cm_le;$$.setLocation(yylineno, colnum);}
	 | EQU  {$$.value = cm_equ;$$.setLocation(yylineno, colnum);}
	 | UEQ  {$$.value = cm_ueq;$$.setLocation(yylineno, colnum);}
			;

additive_expression: additive_expression addop term {$$ = new Node_additive_expression($1, $2, $3);$$->setLocation(yylineno, colnum); }
			| term {$$ = new Node_additive_expression($1);$$->setLocation(yylineno, colnum); }
			;


addop: '+' {$$.value = cm_plus;$$.setLocation(yylineno, colnum);}
     | '-' {$$.value = cm_minus;$$.setLocation(yylineno, colnum);}
			;

term: term mulop factor {$$ = new Node_term($1, $2, $3);$$->setLocation(yylineno, colnum); }
			| factor {$$ = new Node_term($1);$$->setLocation(yylineno, colnum); }
			;

mulop: '*' {$$.value = cm_multi;$$.setLocation(yylineno, colnum);}
     | '/' {$$.value = cm_div;$$.setLocation(yylineno, colnum);}
			;

factor: '(' expression ')' {$$ = new Node_factor($2);$$->setLocation(yylineno, colnum); }
			| var {$$ = new Node_factor($1);$$->setLocation(yylineno, colnum); }
			| call {$$ = new Node_factor($1);$$->setLocation(yylineno, colnum); }
			| NUM {$$ = new Node_factor($1);$$->setLocation(yylineno, colnum); }
			;

call: ID '(' args ')' {$$ = new Node_call($1, $3);$$->setLocation(yylineno, colnum); }
		;

args: arg_list {$$ = new Node_args($1);$$->setLocation(yylineno, colnum); }
		|  {$$ = new Node_args(NULL);$$->setLocation(yylineno, colnum); }
		;

arg_list: arg_list ',' expression  {$$ = new Node_arg_list($1, $3);$$->setLocation(yylineno, colnum); }
		| expression {$$ = new Node_arg_list($1);$$->setLocation(yylineno, colnum); }
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
#ifdef LOGICWORLD // note: not work in Linux
		if(!(yyin=fopen(".\\parse\\yacc_test.txt","r")))
		{
			printf("Error:Can't open file %s\n",argv[1]);
		}
#endif
	}
	yyparse();
}

void yyerror(const char* what) {
	printf("%d:%d:error:%s\n", yylineno, colnum, what);
}
