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
class Node_expression_statement;
class Node_selection_statement;
class Node_iteration_stmt;
class Node_return_stmt;
class Node_var;
class Node_simple_expression;
class Node_additive_expression;
class Node_term;
class Node_factor;
class Node_call;
class Node_arg_list;

class Node {
	public:
		// generate the intermedia code
		virtual void generate(void) = 0;
		// next node
		virtual ~Node() = default;
		explicit Node() = default;
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
		Node_declaration() = default;
		void setNext(Node_declaration* Next);
		virtual ~Node_declaration(void) noexcept(true) {}
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
		explicit Node_fun_declaration(Node_type_spcifier* type, const char* id, Node_params* params, Node_compound_stmt* compound);
	private:
		Node_type_spcifier* type_;
		const char* id_;
		Node_params* params_;
		Node_compound_stmt* compound_;
};

class Node_params : public Node {
	public:
		explicit Node_params(void); // no params
		explicit Node_params(Node_param_list* list);
		void generate(void);
	private:
		Node_param_list* list_;
};

class Node_param_list : public Node {
	public:
		explicit Node_param_list(Node_param* param);
		explicit Node_param_list(Node_param_list* list, Node_param* append);
		Node_param* next();
	private:
		Node_param* first;
		Node_param* last;

};

class Node_param  : public Node {
	public:
		explicit Node_param(cm_type type, const char* id);
		void setNext(Node_param* next);
	private:
		Node_param* next;
		cm_type type_;
		char* id_;
};

class Node_compound_stmt : public Node {
	public:
		explicit Node_compound_stmt(Node_local_declarations* , Node_statement_list);
	private:
		Node_local_declarations* local_dec_;
		Node_statement_list* stmt_;
};

class Node_local_declarations : public Node {
	public:
		Node_local_declarations(Node_local_declarations*, Node_var_declaration*);
	private:
		std::vector<Node_var_declaration*> list_;
};

} // namespace cminus
#endif
