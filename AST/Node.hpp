#ifndef CM_NODE_HPP
#define CM_NODE_HPP
#include "include/cm_base.h"
#include "attribute/IdentifierAttribute.h"
#include <vector>

#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#endif
/* TODO: dealllocator generator */
// liangchan xing class generator
namespace cminus {

//extern xml_dump so that it can be used in the CodeGen.
extern std::fstream xml_dump;

// forward declaration
char* strdup_(const char* str);

class Node;
class Node_program;
class Node_declaration_list;
class Node_declaration;
class Node_var_declaration;
class Node_fun_declaration;
class Node_type_spcifier;
class Node_params;
class Node_param_list;
class Node_param;
class Node_compound_stmt;
class Node_local_declarations;
class Node_statement_list;
class Node_statement;
class Node_expression_stmt;
class Node_selection_stmt;
class Node_iteration_stmt;
class Node_return_stmt;
class Node_expression; // +
class Node_var;
class Node_simple_expression;
class Node_additive_expression;
class Node_term;
class Node_factor;
class Node_call;
class Node_args; // +
class Node_arg_list;

template <typename T>
struct raw_token {
	T value;
	int row;
	int col;

	void setLocation(int row__, int col__) {row = row__; col = col__;}
};

template <typename T>
class raw_type {
	public:
		raw_type(const raw_token<T>& token) {
			value = token.value;
			col_ = token.col;
			row_ = token.row;
		}
		raw_type() {col_=0;row_=0;}

		explicit raw_type(T value_) {value = value_; col_=0;row_=0;}
		operator T&() {return value;}
		// T& operator=(const T& value) {value_ = value; return value_;}
		raw_type<T>& operator=(const raw_type<T>& v) {value = v.value; row_ = v.row_; col_ = v.col_; return *this;}
		void setLocation(int row__, int col__) {row_ = row__; col_ = col__;}
		int row() const {return row_;}
		int col() const {return col_;}
	public:
		T value;
	private:
		int row_;
		int col_;
};

class Node {
	public:
		// generate the intermedia code
		virtual void generate(void) = 0;
		virtual void generate_code_preorder(void){}
		virtual void generate_code_postorder(void){}
		virtual void semantic_analysis_preorder(void){}
		virtual void semantic_analysis_postorder(void){}
		//
		void setLocation(int row__, int col__) {row_ = row__; col_ = col__;}
		int row() const {return row_;}
		int col() const {return col_;}
		// next node
		virtual ~Node() {}
		explicit Node() {col_=0;row_=0;}
	private:
		int row_;
		int col_;
		Node& operator = (const Node&);
};

/*
// rule related
program: declaration_list
			;

@desc: the root of the tree
 */
class Node_program : public Node {
	public:
		explicit Node_program(Node_declaration_list* dec_list);
		virtual void generate(void);
		virtual void generate_code_preorder(void);
		virtual void semantic_analysis_preorder(void);
		virtual void semantic_analysis_postorder(void);

		~Node_program() ;
	private:
		Node_program() ; // delete
		Node_declaration_list* child;
		std::ostringstream begin_code_;
};

/*
// rule related
declaration_list: declaration_list declaration
			| declaration
*/
class Node_declaration_list : public Node {
	public:
		explicit Node_declaration_list(Node_declaration_list* list, Node_declaration* append);
		explicit Node_declaration_list(Node_declaration* node);
		Node_declaration* next(void) {return last;}
		void generate(void);
		~Node_declaration_list() {}
	private:
		Node_declaration_list() ;// = delete;
		// point to first element
		Node_declaration* first;
		// point to last element
		Node_declaration* last;
};

/*
// rule related
declaration: var_declaration
			| fun_declaration
			;
 */
class Node_declaration : public Node {
	public:
		Node_declaration() :next_(NULL) {}
		void setNext(Node_declaration* Next);
		virtual ~Node_declaration(void) {}
		Node_declaration* next(void) {return next_;}
		virtual void generate(void);
		
	private:
		Node_declaration* next_;
};

/*
// rule related
var_declaration: type_specifier ID ';'
			| type_specifier ID '[' NUM ']' ';'
			;
*/
class Node_var_declaration : public Node_declaration {
	public:
		explicit Node_var_declaration(raw_type<cm_type> type, raw_type<char*> id);
		explicit Node_var_declaration(raw_type<cm_type> type, raw_type<char*> id, raw_type<cm_size_type> array_size);
		void generate();
		virtual void generate_code_preorder(void);
		virtual void semantic_analysis_preorder(void);
	private:
		raw_type<cm_type> type_;
		Node_var_declaration() ; // = delete;
		raw_type<char*> id_;
		raw_type<cm_size_type> array_size_;

		VariableAttribute* variable_attribute_;
		std::ostringstream code_;
};
/*
//rule related
fun_declaration: type_specifier ID '(' params ')' compound_stmt
*/
class Node_fun_declaration : public Node_declaration {
	public:
		explicit Node_fun_declaration(raw_type<cm_type> type, raw_type<char*> id, Node_params* params, Node_compound_stmt* compound);
		void generate();
		~Node_fun_declaration() {}
		//virtual void generate_code(void);
		virtual void generate_code_preorder(void);
		virtual void generate_code_postorder(void);
		virtual void semantic_analysis_preorder(void);
		void semantic_analysis_inorder(void);
		virtual void semantic_analysis_postorder(void);
	private:
		Node_fun_declaration() ; // = delete;
		raw_type<cm_type> type_;
		raw_type<char*> id_;
		Node_params* params_;
		Node_compound_stmt* compound_;

		FunctionAttribute* function_attribute_;
		std::ostringstream begin_code_;
		std::ostringstream end_code_;
};
/*
rule related
params: param_list
			| VOID
 */
class Node_params : public Node {
	public:
		explicit Node_params(void); // no params
		explicit Node_params(Node_param_list* list);
		void generate(void);
		~Node_params() {} // = default;
		std::vector<VariableTypeAttribute*>* get_args(){return args_;}
		virtual void semantic_analysis_postorder();
	private:
		Node_param_list* list_;

		std::vector<VariableTypeAttribute*>* args_;
};
/*
param_list: param_list ',' param
			| param
*/
class Node_param_list : public Node {
	public:
		explicit Node_param_list(Node_param* param);
		explicit Node_param_list(Node_param_list* list, Node_param* append);
		Node_param* next();
		void generate();
		~Node_param_list() {} // = default;
		std::vector<VariableTypeAttribute*>* get_args(){return args_;}
		virtual void semantic_analysis_preorder(void);
		void semantic_analysis_interleave_order(Node_param* iter);
	private:
		Node_param* first;
		Node_param* last;

		std::vector<VariableTypeAttribute*>* args_;
};
/*
param: type_specifier ID
			| type_specifier ID '[' ']'
*/
class Node_param : public Node {
	public:
		explicit Node_param(raw_type<cm_type> type, raw_type<char*> id);
		void setNext(Node_param* next) {next_ = next;}
		Node_param* next(void) {return next_;}
		void generate();
		virtual void semantic_analysis_preorder();
		virtual void generate_code_preorder();
		~Node_param() {} // = default;
		VariableTypeAttribute* get_type(){return &(variable_attribute_->type);}
		void set_param_index(int index){param_index=index;}
	private:
		Node_param* next_;
		raw_type<cm_type> type_;
		raw_type<char*> id_;

		VariableAttribute* variable_attribute_;
		int param_index;
};

// pre requ for compound_stmt
class Node_statement : public Node {
	public:
		Node_statement();
		virtual ~Node_statement() {}
		Node_statement* next() {return next_;}
		void setNext(Node_statement* next) ;
		void generate() = 0;
		void set_function_attribute(FunctionAttribute* fa){function_attribute_=fa;}
	private:
		Node_statement* next_;
	protected:
		FunctionAttribute* function_attribute_;
};
/*
compound_stmt:
		'{' local_declarations statement_list '}'
 */
class Node_compound_stmt : public Node_statement {
	public:
		explicit Node_compound_stmt(Node_local_declarations* , Node_statement_list*);
		~Node_compound_stmt() {} // = default;
		void generate();

		virtual void semantic_analysis_preorder();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		Node_local_declarations* local_dec_;
		Node_statement_list* stmt_;


};
/*
local_declarations: local_declarations var_declaration
			|   empty
*/
class Node_local_declarations : public Node {
	public:
		Node_local_declarations(Node_local_declarations*, Node_var_declaration*);
		Node_local_declarations() {}
		virtual ~Node_local_declarations() {} // = default;
		void generate();
	private:
		std::vector<Node_var_declaration*> list_;
};
/*
statement_list:statement_list statement
			|  empty
*/
class Node_statement_list : public Node {
	public:
		Node_statement_list(Node_statement_list* state_list,Node_statement* state);
		Node_statement_list();
		~Node_statement_list() {} // = default;
		void generate();
		void set_function_attribute(FunctionAttribute* fa){function_attribute_=fa;}
		void semantic_analysis_interleave_order(Node_statement* iter);
	private:
		Node_statement* first;
		Node_statement* last;

		FunctionAttribute* function_attribute_;
};
/*
expression_stmt: expression ';'
			| ';'
*/
class Node_expression_stmt : public Node_statement {
	public:
		Node_expression_stmt(Node_expression* expr);
		~Node_expression_stmt() {}
		void generate();
		virtual void generate_code_preorder();
		virtual void generate_code_postorder();
	private:
		Node_expression* expr_;

		std::ostringstream begin_code_;
};
/*
selection_stmt: IF '(' expression ')' statement %prec LOWER_THAN_ELSE
			| IF '(' expression ')' statement ELSE statement
*/
class Node_selection_stmt : public Node_statement {
	public:
		Node_selection_stmt(Node_expression* expr,Node_statement* stmt1, bool else_);
		Node_selection_stmt(Node_expression* expr,Node_statement* stmt1, Node_statement* stmt2, bool else_);
		~Node_selection_stmt() {}
		virtual void semantic_analysis_preorder();
		void semantic_analysis_inorder();
		virtual void generate_code_preorder();
		virtual void generate_code_postorder();
		void generate_code_if();
		void generate_code_else();
		void generate();
	private:
		Node_expression* expr_;
		Node_statement* stmt1_;
		Node_statement* stmt2_;
		bool else_;

		std::ostringstream begin_code_;
		std::ostringstream if_code_;
		std::ostringstream else_code_;
		std::ostringstream end_code_;
		int labelID;
};
/*
iteration_stmt: WHILE '(' expression ')' statement
*/
class Node_iteration_stmt : public Node_statement {
	public:
		Node_iteration_stmt(Node_expression* expr_,Node_statement* stmt);
		~Node_iteration_stmt() {}
		void generate();
		virtual void semantic_analysis_preorder();
		void semantic_analysis_inorder();
		virtual void generate_code_preorder();
		virtual void generate_code_inorder();
		virtual void generate_code_postorder();
	private:
		Node_expression* expr_;
		Node_statement* stmt_;

		std::ostringstream begin_code_;
		std::ostringstream while_code_;
		std::ostringstream end_code_;
		int labelID;

};
/*
return_stmt: RETURN ';' {$$ = new Node_return_stmt();}
			| RETURN expression ';'
*/
class Node_return_stmt : public Node_statement {
	public:
		Node_return_stmt(Node_expression* expression);
		Node_return_stmt() :expr_(NULL) {}
		void generate();
		virtual void semantic_analysis_preorder();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_preorder();
		virtual void generate_code_postorder();
	private:
		Node_expression* expr_;

		std::ostringstream begin_code_;
		std::ostringstream code_;
};
class Node_expression_base : public Node
{
public:
	VariableAttribute* get_variable_attribute(){return variable_attribute_;}
protected:
	Node_expression_base(){
		variable_attribute_=NULL;
	}
	//void set_variable_attribute(VariableAttribute* va){variable_attribute_=va;}
	VariableAttribute* variable_attribute_;
	void dump();
};
/*
expression: var '=' expression {$$ = new Node_expression($1, $3);}
			| simple_expression
*/
class Node_expression : public Node_expression_base {
	public:
		Node_expression(Node_var* var,Node_expression* expr) :var_(var), expr_(expr), sim_expr_(NULL) {}
		Node_expression(Node_simple_expression* sim_expr)
			:var_(NULL), expr_(NULL), sim_expr_(sim_expr) {
			}
		~Node_expression() {}
		void generate();
		//virtual void semantic_analysis_preorder();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		Node_var* var_;
		Node_expression* expr_;
		Node_simple_expression* sim_expr_;

		std::ostringstream code_;
};
/*
var: 		ID {$$ = new Node_var($1);}
			| ID '[' expression ']' {$$ = new Node_var($1, $3);}
*/
class Node_var : public Node_expression_base {
	public:
		Node_var(raw_type<char*> id) :expr_(NULL) {id_ = id; id_.value = strdup_(id.value);}
		Node_var(raw_type<char*> id,Node_expression* expr) :expr_(expr) {id_ = id; id_.value = strdup_(id.value);}
		void generate();
		~Node_var() {}
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		raw_type<char*> id_;
		Node_expression* expr_;

		std::ostringstream code_;
};
/*
simple_expression:additive_expression RELOP additive_expression {$$ = new Node_simple_expression($1, $2, $3);}
			| additive_expression {$$ = new Node_simple_expression($1);}
*/
class Node_simple_expression : public Node_expression_base {
	public:
		Node_simple_expression(Node_additive_expression* add1, raw_type<cm_relops> op, Node_additive_expression* add2) 
			:add1_(add1), relop_(op), add2_(add2) {}

		Node_simple_expression(Node_additive_expression* add) 
			:add1_(add), add2_(NULL) {}

		~Node_simple_expression() {}
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
		void generate();
	private:
		Node_additive_expression* add1_;
		raw_type<cm_relops> relop_;
		Node_additive_expression* add2_;

		std::ostringstream code_;
};
/*
additive_expression: additive_expression addop term {$$ = new Node_additive_expression($1, $2, $3);}
			| term {$$ = new Node_additive_expression($1);}
 */
class Node_additive_expression : public Node_expression_base {
	public:
		Node_additive_expression(Node_additive_expression* add, raw_type<cm_ops> op, Node_term* term) 
			:add_(add), op_(op), term_(term) {}
		
		Node_additive_expression(Node_term* term) :add_(NULL), term_(term) {}

		~Node_additive_expression() {}

		void generate();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		Node_additive_expression* add_;
		raw_type<cm_ops> op_;
		Node_term* term_;

		std::ostringstream code_;
};
/*
term: term mulop factor {$$ = new Node_term($1, $2, $3);}
			| factor {$$ = new Node_term($1);}
 */
class Node_term : public Node_expression_base {
	public:
		Node_term(Node_term* term, raw_type<cm_ops> op, Node_factor* factor)
			:term_(term), op_(op), factor_(factor) {}
		Node_term(Node_factor* factor) :term_(NULL), factor_(factor) {}
		virtual ~Node_term() {}
		void generate();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		Node_term* term_;
		raw_type<cm_ops> op_;
		Node_factor* factor_;

		std::ostringstream code_;
};
/*
factor: '(' expression ')' {$$ = new Node_factor($2);}
			| var {$$ = new Node_factor($1);}
			| call {$$ = new Node_factor($1);}
			| NUM {$$ = new Node_factor($1);}
*/
class Node_factor : public Node_expression_base {
	public:
		Node_factor(Node_expression* expr)
			:type(t_expression) { u.expr_= expr; }
		Node_factor(Node_var* var)
			:type(t_var) { u.var_ = var; }
		Node_factor(Node_call* call)
			:type(t_call) { u.call_ = call; }
		Node_factor(raw_type<cm_int_type> num)
			:type(t_num) { u.num_ = new raw_type<cm_int_type>(num);}
		virtual ~Node_factor() {}
		void generate();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_postorder();
	private:
		enum {t_expression, t_var, t_call, t_num} type;
		union {
			Node_expression* expr_;
			Node_var* var_;
			Node_call* call_;
			raw_type<cm_int_type>* num_;
		} u;
		std::ostringstream code_;
};
/*
call: ID '(' args ')' {$$ = new Node_call($1, $3);}
*/
class Node_call : public Node_expression_base {
	public:
		Node_call(raw_type<char*> id, Node_args* args)
			:args_(args) {id_ = id; id_.value = strdup_(id.value);function_attribute_=NULL;}
		~Node_call() {}
		void generate();
		virtual void semantic_analysis_preorder();
		virtual void semantic_analysis_postorder();
		virtual void generate_code_preorder();
		virtual void generate_code_postorder();
	private:
		raw_type<char*> id_;
		Node_args* args_;
		FunctionAttribute* function_attribute_;

		std::ostringstream code_;
};
/*
args: arg_list {$$ = new Node_args($1);}
		|  {$$ = new Node_args(NULL);}
*/
class Node_args : public Node {
	public:
		Node_args(Node_arg_list* arg_list) :arg_list_(arg_list) {args_=NULL;}
		~Node_args() {}
		void generate();
		std::vector<VariableTypeAttribute*>* get_args(){return args_;}
		virtual void semantic_analysis_postorder();
	private:
		Node_arg_list* arg_list_;

		std::vector<VariableTypeAttribute*>* args_;
};
/*
arg_list: arg_list ',' expression  {$$ = new Node_arg_list($1, $3);}
		| expression {$$ = new Node_arg_list($1);}
*/
class Node_arg_list : public Node {
	public:
		Node_arg_list(Node_arg_list* arg_list,Node_expression* expression) {
			vector_expr_ = std::move(arg_list->vector_expr_);
			vector_expr_.push_back(expression);
			args_=NULL;
		}

		Node_arg_list(Node_expression* expression) {
			vector_expr_.push_back(expression);
			args_=NULL;
		}

		void generate();
		virtual void semantic_analysis_preorder();
		void semantic_analysis_inorder(Node_expression* exp);
		virtual void generate_code_postorder();
		~Node_arg_list() {}
		std::vector<VariableTypeAttribute*>* get_args(){return args_;}
	private:
		std::vector<Node_expression*> vector_expr_;

		std::vector<VariableTypeAttribute*>* args_;

		std::ostringstream code_;
};

} // namespace cminus
#endif
