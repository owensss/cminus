#ifndef CM_NODE_HPP
#define CM_NODE_HPP
#include "include/cm_base.h"
#include <vector>

// Working on fun_decl
// liangchan xing class generator
namespace cminus {

// forward declaration
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

class Node_compound_stmt : public Node {
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
		Node_statement_list* stmt_list_;
		Node_statement* stmt_;
};

class Node_statement : public Node {
	public:
		void generate() {}
		virtual ~Node_statement() {}
		~Node_statement(void);
		void setNext(Node_statement* next) ;
	private:
		Node_statement* next_;
};

class Node_expression_stmt : public Node_statement {
	public:
		Node_expression_stmt(Node_expression_stmt* expression_stmt);
		~Node_expression_stmt() {}
	private:
		Node_expression_stmt* expression_stmt_;
};

class Node_selection_statement : public Node_statement {
	public:
		Node_selection_statement(Node_expression* expression,Node_statement* stmt);
		~Node_selection_statement() {}
	private:
		Node_expression* expression_;
		Node_statement* stmt_;
};

class Node_iteration_stmt : public Node_statement {
	public:
		Node_iteration_stmt(Node_expression* expression,Node_statement* stmt);
		~Node_iteration_stmt() {}
	private:
		Node_expression* expression_;
		Node_statement* stmt_;
};

class Node_return_stmt : public Node_statement {
	public:
		Node_return_stmt(Node_expression* expression);
		Node_return_stmt() {}
	private:
		Node_expression* expression_;
};

class Node_expression : public Node {
	public:
		Node_expression(Node_var* var,Node_expression* expression);
		~Node_expression() {}
	private:
		Node_var* var_;
		Node_expression* expression_;
};

class Node_var : public Node {
	public:
		Node_var(char* id);
		Node_var(char* id,Node_expression* expression);
		~Node_var() {}
	private:
		char* id;
		Node_expression* expression_;
};

class Node_simple_expression : public Node {
	public:
		Node_simple_expression(Node_additive_expression* add1,Node_additive_expression* add2);
		~Node_simple_expression() {}
	private:
		Node_additive_expression* add1_;
		Node_additive_expression* add2_;
};

class Node_additive_expression : public Node {
	public:
		Node_additive_expression(Node_additive_expression* add_expression,char* addop,Node_term* term);
		Node_additive_expression(Node_term* term);
		~Node_additive_expression() {}
	private:
		Node_additive_expression* add_;
		char* addop_;
		Node_term* term_;
};

class Node_term : public Node {
	public:
		Node_term(Node_term* term,char* mulop,Node_factor* factor);
		Node_term(Node_factor* factor);
		virtual ~Node_term() {}
	private:
		Node_term* term_;
		char* mulop_;
		Node_factor* factor_;
};

class Node_factor : public Node {
	public:
		Node_factor(Node_expression* expression);
		Node_factor(Node_var* var);
		Node_factor(Node_call* call);
		Node_factor(char* num);
		virtual ~Node_factor() {}
	private:
		Node_expression* expression_;
		Node_var* var_;
		Node_call* call_;
		char* num_;
};

class Node_call : public Node {
	public:
		Node_call(char* id,Node_args* args);
		~Node_call() {}
	private:
		char* id;
		Node_args* args_;
};

class Node_args : public Node {
	public:
		Node_args(Node_arg_list* arg_list);
		~Node_args() {}
	private:
		Node_arg_list* arg_list_;
};

class Node_arg_list : public Node {
	public:
		Node_arg_list(Node_arg_list* arg_list,Node_expression* expression);
		Node_arg_list(Node_expression* expression);
		~Node_arg_list() {}
	private:
		Node_arg_list* arg_list_;
		Node_expression* expression_;
};
} // namespace cminus
#endif
