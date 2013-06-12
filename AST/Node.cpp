#include "Node.hpp"
#include <cstring>
#include <iostream>


//!!!!!!!! Node_program
namespace cminus {
	char* strdup_(const char* str) {
		size_t size = strlen(str);
		char* dup;
		dup = new char[size+1](); // one space for \0
		strncpy(dup, str, size);
		dup[size] = '\0';
		return dup;
	}

	Node_program::Node_program(Node_declaration_list* dec_list) 
		:child(dec_list) {

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
	
	//!!!!!!!! Node_fun_declaration
	Node_fun_declaration::Node_fun_declaration(cm_type type, const char* id, Node_params* params, Node_compound_stmt* compound)
		:type_(type), params_(params), compound_(compound)
	{
		id_ = strdup_(id);
	}

	//!!!!!!!! Node_params
	Node_params::Node_params(void) :list_(NULL){}
	Node_params::Node_params(Node_param_list* list) :list_(list){
	}

	//!!!!!!!! Node_param_list
	Node_param_list::Node_param_list(Node_param* param)
		:first(param), last(param) {
			param->setNext(NULL);
	}

	Node_param_list::Node_param_list(Node_param_list* list, Node_param* append)
		:first(list->first), last(append) {
			list->last->setNext(append);
			append->setNext(NULL);
	}

	//!!!!!!!! Node_param
	Node_param::Node_param(cm_type type, const char* id)
		:next_(NULL), type_(type) {
			id_ = strdup_(id);
	}

	//!!!!!!!! Node_compound_stmt
	Node_compound_stmt::Node_compound_stmt(Node_local_declarations* local_dec, Node_statement_list* stmt)
		:local_dec_(local_dec), stmt_(stmt) {

	}

	//!!!!!!!!! Node_local_declaratin
	Node_local_declarations::Node_local_declarations(Node_local_declarations* local, Node_var_declaration* var) {
		list_ = std::move(local->list_);
		list_.push_back(var);
	}

	//!!!!!!!!! Node_statement_list
	Node_statement_list::Node_statement_list(Node_statement_list* state_list, Node_statement* state) {
		if (state_list->first == NULL && state_list->last == NULL) {
			state_list->first=state;
			state_list->last=state;
		}
		first = state_list->first;
		last = state;
		std::cout << this << "\t" << state_list << "\t" << state << std::endl;
		state_list->last->setNext(last);
		last->setNext(NULL);
	}

	Node_statement_list::Node_statement_list(void)
		:first(NULL), last(NULL)
	{

	}

	//!!!!!!!!! Node_statement
	Node_statement::Node_statement(void)
		:next_(NULL) {

	}

	void Node_statement::setNext(Node_statement* next) {
		next_ = next;
	}

	//!!!!!!!! Node_expression_stmt
	Node_expression_stmt::Node_expression_stmt(Node_expression* expr)
		:expr_(expr) {
	}

	//!!!!!!!! Node_selection_stmt
	Node_selection_stmt::Node_selection_stmt(Node_expression* expr, Node_statement* stmt1, bool else__)
		:expr_(expr), stmt1_(stmt1), stmt2_(NULL), else_(else__) {
	}

	Node_selection_stmt::Node_selection_stmt(Node_expression* expr, Node_statement* stmt1, Node_statement* stmt2, bool else__) 
		:expr_(expr), stmt1_(stmt1), stmt2_(stmt2), else_(else__) 
	{
	}

	//!!!!!!!! Node_iteration_stmt
	Node_iteration_stmt::Node_iteration_stmt(Node_expression* expr, Node_statement* stmt)
		:expr_(expr), stmt_(stmt) {
	}

	//!!!!!!!! Node_return_stmt
	Node_return_stmt::Node_return_stmt(Node_expression* expr)
		:expr_(expr) {
	}

	//!!!!!!!! Node_expression
	
} // namespace cminus

