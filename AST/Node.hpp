#ifndef CM_NODE_HPP
#define CM_NODE_HPP
#include "include/cm_base.h"
#include <vector>

// Working on fun_decl
// liangchan xing class generator
namespace cminus {

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

class Node {
	public:
		// generate the intermedia code
		virtual void generate(void) = 0;
		// next node
		virtual ~Node() {}
		explicit Node() {}
	private:
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
		Node_program() = delete;
		explicit Node_program(Node_declaration_list* dec_list);
		void generate(void);
		~Node_program() noexcept(true);
	private:
		Node_declaration_list* child;
};

/*
// rule related
declaration_list: declaration_list declaration
			| declaration
*/
class Node_declaration_list : public Node {
	public:
		Node_declaration_list() = delete;
		explicit Node_declaration_list(Node_declaration_list* list, Node_declaration* append);
		explicit Node_declaration_list(Node_declaration* node);
		Node_declaration* next(void) {return last;}
		void generate(void) {}
		~Node_declaration_list() noexcept(true) {}
	private:
		Node_declaration* first;
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
		void generate(void) {}
		
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
		Node_var_declaration() = delete;
		explicit Node_var_declaration(cm_type type, const char* id);
		explicit Node_var_declaration(cm_type type, const char* id, cm_size_type array_size);
	private:
		cm_type type_;
		char* id_;
		cm_size_type array_size_;
};

class Node_fun_declaration : public Node_declaration {
	public:
		Node_fun_declaration() = delete;
		explicit Node_fun_declaration(cm_type type, const char* id, Node_params* params, Node_compound_stmt* compound);
		void generate() {}
		~Node_fun_declaration() = default;
	private:
		cm_type type_;
		const char* id_;
		Node_params* params_;
		Node_compound_stmt* compound_;
};

class Node_params : public Node {
	public:
		explicit Node_params(void); // no params
		explicit Node_params(Node_param_list* list);
		void generate(void) {}
		~Node_params() = default;
	private:
		Node_param_list* list_;
};

class Node_param_list : public Node {
	public:
		explicit Node_param_list(Node_param* param);
		explicit Node_param_list(Node_param_list* list, Node_param* append);
		Node_param* next();
		void generate() {}
		~Node_param_list() = default;
	private:
		Node_param* first;
		Node_param* last;

};

class Node_param : public Node {
	public:
		explicit Node_param(cm_type type, const char* id);
		void setNext(Node_param* next) {next_ = next;}
		void generate() {}
		~Node_param() = default;
	private:
		Node_param* next_;
		cm_type type_;
		char* id_;
};

// pre requ for compound_stmt
class Node_statement : public Node {
	public:
		Node_statement();
		void generate() {}
		virtual ~Node_statement() {}
		void setNext(Node_statement* next) ;
	private:
		Node_statement* next_;
};

class Node_compound_stmt : public Node_statement {
	public:
		explicit Node_compound_stmt(Node_local_declarations* , Node_statement_list*);
		void generate() {}
		~Node_compound_stmt() = default;
	private:
		Node_local_declarations* local_dec_;
		Node_statement_list* stmt_;
};

class Node_local_declarations : public Node {
	public:
		Node_local_declarations(Node_local_declarations*, Node_var_declaration*);
		Node_local_declarations() {}
		void generate() {}
		virtual ~Node_local_declarations() = default;
	private:
		std::vector<Node_var_declaration*> list_;
};

class Node_statement_list : public Node {
	public:
		Node_statement_list(Node_statement_list* state_list,Node_statement* state);
		Node_statement_list();
		~Node_statement_list() = default;
		void generate() {}
	private:
		Node_statement* first;
		Node_statement* last;
};

class Node_expression_stmt : public Node_statement {
	public:
		Node_expression_stmt(Node_expression* expr);
		~Node_expression_stmt() {}
	private:
		Node_expression* expr_;
};

class Node_selection_stmt : public Node_statement {
	public:
		Node_selection_stmt(Node_expression* expr,Node_statement* stmt1, bool else_);
		Node_selection_stmt(Node_expression* expr,Node_statement* stmt1, Node_statement* stmt2, bool else_);
		~Node_selection_stmt() {}
	private:
		Node_expression* expr_;
		Node_statement* stmt1_;
		Node_statement* stmt2_;
		bool else_;
};

class Node_iteration_stmt : public Node_statement {
	public:
		Node_iteration_stmt(Node_expression* expr_,Node_statement* stmt);
		~Node_iteration_stmt() {}
	private:
		Node_expression* expr_;
		Node_statement* stmt_;
};

class Node_return_stmt : public Node_statement {
	public:
		Node_return_stmt(Node_expression* expression);
		Node_return_stmt() :expr_(NULL) {}
	private:
		Node_expression* expr_;
};

class Node_expression : public Node {
	public:
		Node_expression(Node_var* var,Node_expression* expr) :var_(var), expr_(expr), sim_expr_(NULL) {}
		Node_expression(Node_simple_expression* sim_expr)
			:expr_(NULL), sim_expr_(sim_expr) {
			}
		~Node_expression() {}
		void generate() {}
	private:
		Node_var* var_;
		Node_expression* expr_;
		Node_simple_expression* sim_expr_;
};

class Node_var : public Node {
	public:
		Node_var(const char* id) :id_(strdup_(id)), expr_(NULL) {}
		Node_var(const char* id,Node_expression* expr) :id_(strdup_(id)), expr_(expr) {}
		void generate() {}
		~Node_var() {}
	private:
		char* id_;
		Node_expression* expr_;
};

class Node_simple_expression : public Node {
	public:
		Node_simple_expression(Node_additive_expression* add1, cm_relops op, Node_additive_expression* add2) 
			:add1_(add1), relop_(op), add2_(add2) {}

		Node_simple_expression(Node_additive_expression* add) 
			:add1_(add), add2_(NULL) {}

		~Node_simple_expression() {}
		void generate() {}
	private:
		Node_additive_expression* add1_;
		cm_relops relop_;
		Node_additive_expression* add2_;
};

class Node_additive_expression : public Node {
	public:
		Node_additive_expression(Node_additive_expression* add, cm_ops op, Node_term* term) 
			:add_(add), op_(op), term_(term) {}
		
		Node_additive_expression(Node_term* term) :add_(NULL), term_(term) {}

		~Node_additive_expression() {}

		void generate() {}
	private:
		Node_additive_expression* add_;
		cm_ops op_;
		Node_term* term_;
};

class Node_term : public Node {
	public:
		Node_term(Node_term* term, cm_ops op, Node_factor* factor)
			:term_(term), op_(op), factor_(factor) {}
		Node_term(Node_factor* factor) :term_(NULL), factor_(factor) {}
		virtual ~Node_term() {}
		void generate() {}
	private:
		Node_term* term_;
		cm_ops op_;
		Node_factor* factor_;
};

class Node_factor : public Node {
	public:
		Node_factor(Node_expression* expr)
			:type(t_expression) { u.expr_= expr; }
		Node_factor(Node_var* var)
			:type(t_var) { u.var_ = var; }
		Node_factor(Node_call* call)
			:type(t_call) { u.call_ = call; }
		Node_factor(cm_int_type num)
			:type(t_num) { u.num_ = num ;}
		virtual ~Node_factor() {}
		void generate() {}
	private:
		enum {t_expression, t_var, t_call, t_num} type;
		union {
			Node_expression* expr_;
			Node_var* var_;
			Node_call* call_;
			cm_int_type num_;
		} u;
};

class Node_call : public Node {
	public:
		Node_call(const char* id, Node_args* args)
			:id_(strdup_(id)), args_(args) {}
		~Node_call() {}
		void generate() {}
	private:
		char* id_;
		Node_args* args_;
};

class Node_args : public Node {
	public:
		Node_args(Node_arg_list* arg_list) :arg_list_(arg_list) {}
		~Node_args() {}
		void generate() {}
	private:
		Node_arg_list* arg_list_;
};

class Node_arg_list : public Node {
	public:
		Node_arg_list(Node_arg_list* arg_list,Node_expression* expression) {
			vector_expr_ = std::move(arg_list->vector_expr_);
			vector_expr_.push_back(expression);
		}

		Node_arg_list(Node_expression* expression) {
			vector_expr_.push_back(expression);
		}

		void generate() {}
		~Node_arg_list() {}
	private:
		std::vector<Node_expression*> vector_expr_;
};

} // namespace cminus
#endif
