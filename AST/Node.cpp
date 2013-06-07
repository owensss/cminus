#include "Node.hpp"
#include <cstring>

namespace { // strdup
	char* strdup_(const char* str) {
		size_t size = strlen(str);
		char* dup;
		dup = new char[size+1](); // one space for \0
		strncpy(dup, str, size);
		dup[size] = '\0';
		return dup;
	}
}

//!!!!!!!! Node_program
namespace cminus {
	Node_program::Node_program(Node_declaration_list* dec_list) :child(dec_list) {

	}

	// genrate the intermedia code
	void Node_program::generate(void) {
		// call child
		if (child) child->generate();
	}

	Node_program::~Node_program(void) noexcept(true) {
		if (child) delete child;
	}

	//!!!!!!!! Node_declaration_list
	Node_declaration_list::Node_declaration_list(Node_declaration_list* list, Node_declaration* append) {
		first = list->first;
		last = append;
		list->last->setNext(last);
		last->setNext(NULL);
	}

	Node_declaration_list::Node_declaration_list(Node_declaration* node) {
		first = node;
		last = node;
		node->setNext(NULL) ;
	}

	//!!!!!!!! Node_declaration
	void Node_declaration::setNext(Node_declaration* Next) {
		next_ = Next;
	}

	//!!!!!!!! Node_var_declaration
	Node_var_declaration::Node_var_declaration(cm_type type, const char* id)
		:type_(type), array_size_(1) {
		id_ = strdup_(id);
	}
	Node_var_declaration::Node_var_declaration(cm_type type, const char* id, cm_size_type array_size)
		:type_(CM_INT_ARRAY), array_size_(array_size) {
		id_ = strdup_(id);
	}
} // namespace cminus

