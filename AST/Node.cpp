#include "Node.hpp"
#include <cstring>
#include <fstream>

extern void yyerror(const char* what);

//!!!!!!!! Node_program
namespace cminus {
	using std::endl;
	std::fstream xml_dump;
	struct _ {
		_() {
			xml_dump.open("dump.xml", std::ios_base::out);
		}
	}_;

	void dump_type(raw_type<cm_type> type_) {
		xml_dump << "<type row=\""<<type_.row() << "\" col=\"" << type_.col() << "\">\n";
		switch (type_ ) {
			case CM_INT:
				xml_dump << "int";
				break;
			case CM_VOID:
				xml_dump << "void";
				break;
			case CM_INT_ARRAY:
				xml_dump << "array";
				break;
		}
		xml_dump << "</type>\n";
	}

	void dump_id(raw_type<char*> id) {
		xml_dump << "<id row=\"" << id.row() << "\" col=\"" << id.col() << "\">" << id << "</id>\n";
	}

	void dump_array(raw_type<cm_size_type> size) {
		xml_dump << "<size>" << size << "</size>\n";
	}

	void dump_relop(raw_type<cm_relops> relop) {
		xml_dump << "<relop>";
		switch (relop) {
			case cm_gt:
				xml_dump << ">";
				break;
			case cm_lt:
				xml_dump << "<";
				break;
			case cm_ge:
				xml_dump << ">=";
				break;
			case cm_le:
				xml_dump << "<=";
				break;
			case cm_equ:
				xml_dump << "==";
				break;
			case cm_ueq:
				xml_dump << "!=";
				break;
		}
		xml_dump << "</relop>\n";
	}

	void dump_ops(raw_type<cm_ops> op) {
		xml_dump << "<op>";
		switch (op) {
			case cm_plus:
				xml_dump << "+";
				break;
			case cm_minus:
				xml_dump << "-";
				break;
			case cm_multi:
				xml_dump << "*";
				break;
			case cm_div:
				xml_dump << "/";
				break;
		}
		xml_dump << "</op>\n";
	}

	void dump_num(raw_type<cm_int_type> num) {
		xml_dump << "<num>" << num << "</num>\n";
	}

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

	// generate the intermedia code
	void Node_program::generate(void) {
		// call child
		xml_dump << "<Node_program>\n";
		child->generate();
		xml_dump << "</Node_program>\n";
	}

	Node_program::~Node_program(void) {
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

	void Node_declaration_list::Node_declaration_list::generate(void) {
		xml_dump << "<Node_declaration_list>\n";
		if (first != NULL) {
			for (Node_declaration* iter = first; iter != last; iter=iter->next()) {
				iter->generate();
			}
		}
		if (last != NULL && first != last) 
			last->generate();
		xml_dump << "</Node_declaration_list>\n";
	}
	//!!!!!!!! Node_declaration
	void Node_declaration::setNext(Node_declaration* Next) {
		next_ = Next;
	}

	void Node_declaration::generate(void) {
		xml_dump << "<Node_declaration>\n";
		xml_dump << "should be invalid\n";
		xml_dump << "</Node_declaration>\n";
	}
	//!!!!!!!! Node_var_declaration
	Node_var_declaration::Node_var_declaration(raw_type<cm_type> type, raw_type<char*> id)
		:type_(type), array_size_(1) {
		id_ = id;
		id_.value = strdup_(id.value);
	}
	Node_var_declaration::Node_var_declaration(raw_type<cm_type> type, raw_type<char*> id, raw_type<cm_size_type> array_size)
		:type_(CM_INT_ARRAY), array_size_(array_size) {
		id_ = id;
		id_.value = strdup_(id.value);
	}
	
	void Node_var_declaration::generate() {
		xml_dump << "<Node_var_declaration>\n";
		dump_type(type_);
		dump_id(id_);
		if (type_ == CM_INT_ARRAY) {
			dump_array(array_size_);
		}
		xml_dump << "</Node_var_declaration>\n";
	}
	//!!!!!!!! Node_fun_declaration
	Node_fun_declaration::Node_fun_declaration(raw_type<cm_type> type, raw_type<char*> id, Node_params* params, Node_compound_stmt* compound)
		:type_(type), params_(params), compound_(compound)
	{
		id_ = id;
		id_.value = strdup_(id.value);
	}

	void Node_fun_declaration::generate() {
		xml_dump << "<Node_fun_declaration>\n";
		dump_type(type_);
		dump_id(id_);
		params_->generate();
		compound_->generate();
		xml_dump << "</Node_fun_declaration>\n";
	}
	//!!!!!!!! Node_params
	Node_params::Node_params(void) :list_(NULL){}
	Node_params::Node_params(Node_param_list* list) :list_(list){
	}
	
	void Node_params::generate(void) {
		xml_dump << "<Node_params>\n";
		if (list_) list_->generate();
		xml_dump << "</Node_params>\n";
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

	void Node_param_list::generate() {
		xml_dump << "<Node_param_list>\n";
		if (first != NULL) {
			for (Node_param* iter = first; iter != last; iter=iter->next()) {
				iter->generate();
			}
		}
		if (last != NULL && first != last) 
			last->generate();
		xml_dump << "</Node_param_list>\n";
	}
	//!!!!!!!! Node_param
	Node_param::Node_param(raw_type<cm_type> type, raw_type<char*> id)
		:next_(NULL), type_(type) {
			id_ = id;
			id_.value = strdup_(id.value);
	}

	void Node_param::generate() {
		xml_dump << "<Node_param>\n";
		dump_type(type_);
		dump_id(id_);
		xml_dump << "</Node_param>\n";
	}
	//!!!!!!!! Node_compound_stmt
	Node_compound_stmt::Node_compound_stmt(Node_local_declarations* local_dec, Node_statement_list* stmt)
		:local_dec_(local_dec), stmt_(stmt) {

	}

	void Node_compound_stmt::generate(void) {
		xml_dump << "<Node_compound_stmt>\n";
		if (local_dec_ != NULL) local_dec_->generate();
		if (stmt_ != NULL) stmt_->generate();
		xml_dump << "</Node_compound_stmt>\n";
	}
	//!!!!!!!!! Node_local_declaratin
	Node_local_declarations::Node_local_declarations(Node_local_declarations* local, Node_var_declaration* var) {
		list_ = std::move(local->list_);
		list_.push_back(var);
	}
	
	void Node_local_declarations::generate() {
		typedef std::vector<Node_var_declaration*> list_type;
		xml_dump << "<Node_local_declarations>\n";
		for (list_type::iterator iter = list_.begin(); iter != list_.end(); ++iter) {
			(*iter)->generate();
		}
		xml_dump << "</Node_local_declarations>\n";
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

	void Node_statement_list::generate(void) {
		xml_dump << "<Node_statement_list>\n";
		if (first != NULL) {
			for (Node_statement* iter = first; iter != last; iter=iter->next()) {
				iter->generate();
			}
		}
		if (last != NULL && first != last) 
			last->generate();
		xml_dump << "</Node_statement_list>\n";
	}
	//!!!!!!!!! Node_statement
	Node_statement::Node_statement(void)
		:next_(NULL) {

	}

	void Node_statement::setNext(Node_statement* next) {
		next_ = next;
	}

	void Node_statement::generate() {
		xml_dump << "<Node_statement>\n";
		xml_dump << "should be invalid\n";
		xml_dump << "</Node_statement>\n";
	}
	//!!!!!!!!! Node_expression_stmt
	Node_expression_stmt::Node_expression_stmt(Node_expression* expr)
		:expr_(expr) {
	}

	void Node_expression_stmt::generate(void) {
		xml_dump << "<Node_expression_stmt>\n";
		expr_->generate();
		xml_dump << "</Node_expression_stmt>\n";
	}

	//!!!!!!!!! Node_selection_stmt
	Node_selection_stmt::Node_selection_stmt(Node_expression* expr, Node_statement* stmt1, bool else__)
		:expr_(expr), stmt1_(stmt1), stmt2_(NULL), else_(else__) {
	}

	Node_selection_stmt::Node_selection_stmt(Node_expression* expr, Node_statement* stmt1, Node_statement* stmt2, bool else__) 
		:expr_(expr), stmt1_(stmt1), stmt2_(stmt2), else_(else__) 
	{
	}

	void Node_selection_stmt::generate() {
		xml_dump << "<Node_selection_stmt>\n";
		expr_->generate();
		stmt1_->generate();
		if (else_) stmt2_->generate();
		xml_dump << "</Node_selection_stmt>\n";
	}
	//!!!!!!!!! Node_iteration_stmt
	Node_iteration_stmt::Node_iteration_stmt(Node_expression* expr, Node_statement* stmt)
		:expr_(expr), stmt_(stmt) {
	}

	void Node_iteration_stmt::generate() {
		xml_dump << "<Node_iteration_stmt>\n";
		expr_->generate();
		stmt_->generate();
		xml_dump << "</Node_iteration_stmt>\n";
	}
	//!!!!!!!!! Node_return_stmt
	Node_return_stmt::Node_return_stmt(Node_expression* expr)
		:expr_(expr) {
	}

	void Node_return_stmt::generate() {
		xml_dump << "<Node_return_stmt>\n";
		if (expr_) expr_->generate();
		xml_dump << "</Node_return_stmt>\n";
	}
	//!!!!!!!!! Node_expression
	void Node_expression::generate() {
		xml_dump << "<Node_expression>\n";
		if (var_) var_->generate();
		if (expr_) expr_->generate();
		if (sim_expr_) sim_expr_->generate();
		xml_dump << "</Node_expression>\n";
	}

	//!!!!!!!!! Node_var
	void Node_var::generate() {
		xml_dump << "<Node_var>\n";
		dump_id(id_);
		if (expr_) expr_->generate();
		xml_dump << "</Node_var>\n";
	}
	//!!!!!!!!! Node_simple_expression
	void Node_simple_expression::generate() {
		xml_dump << "<Node_simple_expression>\n";
		add1_->generate();
		if (add2_ != NULL) {
			dump_relop(relop_);
			add2_->generate();
		}
		xml_dump << "</Node_simple_expression>\n";
	}
	//!!!!!!!!! Node_additive_expression
	void Node_additive_expression::generate() {
		xml_dump << "<Node_additive_expression>\n";
		if (add_) {
			add_->generate();
			dump_ops(op_);
		}
		term_->generate();
		xml_dump << "</Node_additive_expression>\n";
	}
	//!!!!!!!!! Node_term
	void Node_term::generate() {
		xml_dump << "<Node_term>\n";
		if (term_ != NULL) {
			term_->generate();
			dump_ops(op_);
		}
		factor_->generate();
		xml_dump << "</Node_term>\n";
	}
	//!!!!!!!!! Node_factor
	void Node_factor::generate() {
		xml_dump << "<Node_factor>\n";
		switch (type) {
			case t_expression:
				u.expr_->generate();
				break;
			case t_var:
				u.var_->generate();
				break;
			case t_call:
				u.call_->generate();
				break;
			case t_num:
				dump_num(*u.num_);
				break;
		}
		xml_dump << "</Node_factor>\n";
	}

	//!!!!!!!!! Node_call
	void Node_call::generate() {
		xml_dump << "<Node_call>\n";
		dump_id(id_);
		args_->generate();
		xml_dump << "</Node_call>\n";
	}
	//!!!!!!!!! Node_args
	void Node_args::generate() {
		xml_dump << "<Node_args>\n";
		if (arg_list_) 
			arg_list_->generate();
		xml_dump << "</Node_args>\n";
	}
	//!!!!!!!!! Node_arg_list
	void Node_arg_list::generate() {
		typedef std::vector<Node_expression*> list_type;
		xml_dump << "<Node_arg_list>\n";
		for (list_type::iterator iter = vector_expr_.begin(); iter != vector_expr_.end(); ++ iter) {
			(*iter)->generate();
		}
		xml_dump << "</Node_arg_list>\n";
	}
} // namespace cminus

