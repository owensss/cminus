#ifndef CM_NODE_HPP
#define CM_NODE_HPP
#include "include/cm_base.h"

namespace cminus {

class Node;
class Node_program;
class Node_declaration_list;
class Node_declaration;
class Node_var_declaration;

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
		~Node_declaration(void) noexcept(true) {}
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
}

#endif
