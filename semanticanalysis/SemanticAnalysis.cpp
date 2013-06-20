/*
 * SemanticAnalysis.cpp
 *
 *  Created on: 2013-6-19
 *      Author: Administrator
 */
#include "codegen/GlobalState.h"
#include "codegen/RegisterFile.h"
#include "AST/Node.hpp"
#include "attribute/IdentifierAttribute.h"
#include "symtable/symtable.h"
#include <iostream>
#include <fstream>
namespace cminus
{
#define ERROR_FORMAT "Error:"<<row()<<"-"<<col()<<":"
extern std::fstream xml_dump;
SymbolTableStack g_symbolTableStack;
int g_semanticErrorCount = 0;

void Node_program::semantic_analysis_preorder(void)
{
	PushSymbolTableStack(&g_symbolTableStack);
}
void Node_program::semantic_analysis_postorder(void)
{
	PopSymbolTableStack(&g_symbolTableStack);
}

void Node_var_declaration::semantic_analysis_preorder(void)
{
	//std::string name = std::string((char*&)this->id_);
	variable_attribute_ = new VariableAttribute((char*&) this->id_);
	variable_attribute_->type.type = (cm_type&) type_;
	variable_attribute_->type.arrayNum = (cm_size_type&) array_size_;

	Symbol* ret = InsertSymbolTableStack((char*&) this->id_,
			(void*) variable_attribute_, &g_symbolTableStack);
	if (ret)
	{
		std::cerr << ERROR_FORMAT << "The same name had been defined before.\n";
		g_semanticErrorCount++;
	}

	if (variable_attribute_->type.type == CM_VOID)
	{
		std::cerr << ERROR_FORMAT << "The type of " << variable_attribute_->name
				<< "can not be void\n";
		g_semanticErrorCount++;
	}
}
void Node_fun_declaration::semantic_analysis_preorder(void)
{
	PushSymbolTableStack(&g_symbolTableStack);
	function_attribute_ = new FunctionAttribute((char*&) this->id_);
	function_attribute_->type.returntype.type = (cm_type&) type_;
	function_attribute_->type.returntype.arrayNum = 0;

	Symbol* ret = InsertSymbolTableStack((char*&) this->id_,
			(void*) function_attribute_, &g_symbolTableStack);
	if (ret)
	{
		std::cerr << ERROR_FORMAT << "The same name had been defined before.\n";
		g_semanticErrorCount++;
	}
}
void Node_fun_declaration::semantic_analysis_inorder(void)
{
	this->function_attribute_->type.args = this->params_->get_args();
	this->compound_->set_function_attribute(function_attribute_);
}
void Node_fun_declaration::semantic_analysis_postorder(void)
{
	PopSymbolTableStack(&g_symbolTableStack);
}
void Node_params::semantic_analysis_postorder()
{
	if (list_)
	{
		this->args_ = list_->get_args();
	}
}
void Node_param_list::semantic_analysis_preorder(void)
{
	args_ = new std::vector<VariableTypeAttribute*>();
}
void Node_param_list::semantic_analysis_interleave_order(Node_param* iter)
{
	args_->push_back(iter->get_type());

}
void Node_param::semantic_analysis_preorder()
{
	variable_attribute_ = new VariableAttribute((char*&) id_);
	variable_attribute_->type.type = (cm_type&) type_;
	variable_attribute_->type.arrayNum = 0;

	if (variable_attribute_->type.type == CM_VOID)
	{
		std::cerr << ERROR_FORMAT <<
		"The type in the function declare shouldn't be void\n";
		g_semanticErrorCount++;
	}

	Symbol* ret = InsertSymbolTableStack((char*&) this->id_,
			(void*) variable_attribute_, &g_symbolTableStack);
	if (ret)
	{
		std::cerr << ERROR_FORMAT << "The same name had been defined before.\n";
		g_semanticErrorCount++;
	}

}
void Node_compound_stmt::semantic_analysis_preorder()
{
	PushSymbolTableStack(&g_symbolTableStack);
	if (stmt_ != NULL)
		stmt_->set_function_attribute(this->function_attribute_);
}
void Node_compound_stmt::semantic_analysis_postorder()
{
	PopSymbolTableStack(&g_symbolTableStack);
}
void Node_statement_list::semantic_analysis_interleave_order(
		Node_statement* iter)
{
	iter->set_function_attribute(function_attribute_);
}
void Node_selection_stmt::semantic_analysis_preorder()
{
	if(stmt1_)
	{
		stmt1_->set_function_attribute(this->function_attribute_);
	}
	if(stmt2_)
	{
		stmt2_->set_function_attribute(this->function_attribute_);
	}
}
void Node_iteration_stmt::semantic_analysis_preorder()
{
	if(stmt_)
	{
		stmt_->set_function_attribute(this->function_attribute_);
	}
}
void Node_return_stmt::semantic_analysis_preorder()
{

}
void Node_return_stmt::semantic_analysis_postorder()
{
	if (this->function_attribute_->type.returntype.type
			== CM_VOID&& expr_!=NULL)
	{
		std::cerr << ERROR_FORMAT << "The return type of function "
				<< function_attribute_->name
				<< " is void, but you want to return int.\n";
		g_semanticErrorCount++;
	}
	if (this->function_attribute_->type.returntype.type == CM_INT&& expr_==NULL)
	{
		std::cerr << ERROR_FORMAT << "The return type of function "
				<< function_attribute_->name
				<< " is int, but you want to return nothing.\n";
		g_semanticErrorCount++;
	}
	if (expr_ != NULL
			&& expr_->get_variable_attribute()->type.type == CM_INT_ARRAY)
	{
		std::cerr << ERROR_FORMAT << "You can't return an array.\n";
		g_semanticErrorCount++;
	}
}
void Node_expression_base::dump()
{
	std::string typeStr = "";
	switch (this->variable_attribute_->type.type)
	{
	case CM_INT:
		typeStr = "CM_INT";
		break;
	case CM_INT_ARRAY:
		typeStr = "CM_INT_ARRAY";
		break;
	case CM_VOID:
		typeStr = "CM_VOID";
		break;
	default:
		typeStr = "default";
		break;
	}
	xml_dump << "type:" << typeStr << "\n";
	xml_dump << "arrayNum:" << this->variable_attribute_->type.arrayNum << "\n";
}
void Node_expression::semantic_analysis_postorder()
{

	if (sim_expr_)
	{
		this->variable_attribute_ = sim_expr_->get_variable_attribute();
	}
	if (expr_)
	{
		if (expr_->get_variable_attribute()->type.type != CM_INT)
		{
			std::cerr << ERROR_FORMAT
					<< "The right hand side of the expression is not int.\n";
			g_semanticErrorCount++;
		}
		else
		{
			this->variable_attribute_ = expr_->get_variable_attribute();
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
	}
	dump();
}
void Node_var::semantic_analysis_postorder()
{
	if (expr_)
	{
		if (expr_->get_variable_attribute()->type.type != CM_INT)
		{
			std::cerr << ERROR_FORMAT
					<< "The right hand side of the expression is not int.\n";
			g_semanticErrorCount++;
		}
	}
	Symbol* ret = LookUpSymbolTableStack((char*&) id_, &g_symbolTableStack);
	if (!ret)
	{
		std::cerr << ERROR_FORMAT << "Can't find symbol " << (char*&) id_
				<< ".\n";
		g_semanticErrorCount++;
	}
	else
	{
		IdentifierAttribute* idAttr = (IdentifierAttribute*) ret->type;
		if (idAttr->GetIdentifierType()
				!= IdentifierAttribute::IDENTIFIER_VARIABLE)
		{
			std::cerr << ERROR_FORMAT << "The symbol " << (char*&) id_
					<< "is not the variable type.\n";
			g_semanticErrorCount++;
		}
		else
		{
			VariableAttribute* varAttr = (VariableAttribute*) idAttr;
			if (expr_ == NULL/* && varAttr->type.type!=CM_INT*/)
			{
				//std::cerr<<ERROR_FORMAT<<"The symbol "<<(char*&)id_<<"is not the int type.\n";
				//g_semanticErrorCount++;
				this->variable_attribute_ = varAttr;
			}
			else if (/*expr_!=NULL && */varAttr->type.type == CM_INT_ARRAY)
			{
				this->variable_attribute_ = new VariableAttribute((char*&) id_);
				this->variable_attribute_->type.arrayNum = 0;
				this->variable_attribute_->type.type = CM_INT;
			}
			else
			{
				std::cerr << ERROR_FORMAT << "The symbol " << (char*&) id_
						<< "is not the array type.\n";
				g_semanticErrorCount++;

			}
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute((char*&) id_);
		this->variable_attribute_->type.arrayNum = 0;
		this->variable_attribute_->type.type = CM_INT;
	}
	dump();
}
void Node_simple_expression::semantic_analysis_postorder()
{
	if (add2_ == NULL)
	{
		this->variable_attribute_ = add1_->get_variable_attribute();
	}
	else
	{
		if (add2_->get_variable_attribute()->type.type != CM_INT
				|| add1_->get_variable_attribute()->type.type != CM_INT)
		{
			std::cerr << ERROR_FORMAT
					<< "The right hand side of the expression is not int.\n";
			g_semanticErrorCount++;
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
	}
	dump();
}
void Node_additive_expression::semantic_analysis_postorder()
{
	if (add_ == NULL)
	{
		this->variable_attribute_ = term_->get_variable_attribute();
	}
	else
	{
		if (term_->get_variable_attribute()->type.type != CM_INT
				|| add_->get_variable_attribute()->type.type != CM_INT)
		{
			std::cerr << ERROR_FORMAT
					<< "The right hand side of the expression is not int.\n";
			g_semanticErrorCount++;
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
	}
	dump();
}
void Node_term::semantic_analysis_postorder()
{
	if (term_ == NULL)
	{
		this->variable_attribute_ = factor_->get_variable_attribute();
	}
	else
	{
		if (term_->get_variable_attribute()->type.type != CM_INT
				|| factor_->get_variable_attribute()->type.type != CM_INT)
		{
			std::cerr << ERROR_FORMAT
					<< "The right hand side of the expression is not int.\n";
			g_semanticErrorCount++;
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
	}
	dump();
}
void Node_factor::semantic_analysis_postorder()
{
	switch (type)
	{
	case t_expression:
		//u.expr_->generate();
		this->variable_attribute_ = u.expr_->get_variable_attribute();
		break;
	case t_var:
		//u.var_->generate();
		this->variable_attribute_ = u.var_->get_variable_attribute();
		break;
	case t_call:
		//u.call_->generate();
		this->variable_attribute_ = u.call_->get_variable_attribute();
		break;
	case t_num:
		//dump_num(*u.num_);
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
		break;
	}
	dump();
}
void Node_call::semantic_analysis_preorder()
{
	Symbol* ret = LookUpSymbolTableStack((char*&) id_, &g_symbolTableStack);
	if (!ret)
	{
		std::cerr << ERROR_FORMAT << "Can't find symbol " << (char*&) id_
				<< ".\n";
		g_semanticErrorCount++;
	}
	else
	{
		IdentifierAttribute* idAttr = (IdentifierAttribute*) ret->type;
		if (idAttr->GetIdentifierType()
				!= IdentifierAttribute::IDENTIFIER_FUNCTION)
		{
			std::cerr << ERROR_FORMAT << "The symbol " << (char*&) id_
					<< "is not function.\n";
			g_semanticErrorCount++;
		}
		else
		{
			this->function_attribute_ = (FunctionAttribute*) idAttr;
		}
	}
	if (!this->variable_attribute_)
	{
		this->variable_attribute_ = new VariableAttribute("temp");
		this->variable_attribute_->type.type = CM_INT;
		this->variable_attribute_->type.arrayNum = 0;
	}
}
void Node_call::semantic_analysis_postorder()
{
	if (this->function_attribute_)
	{
		std::vector<VariableTypeAttribute*>* realVT = args_->get_args();
		std::vector<VariableTypeAttribute*>* formalVT =
				(this->function_attribute_->type.args);
		if (realVT != NULL && formalVT != NULL)
		{

			if (realVT->size() == formalVT->size())
			{
				for (size_t i = 0; i < realVT->size(); i++)
				{
					if ((*realVT)[i]->type != (*formalVT)[i]->type)
					{
						std::cerr << ERROR_FORMAT << "The " << i << "th "
								<< "real parameter type is not same as formal one.\n";
						g_semanticErrorCount++;
					}
				}
			}
			else
			{
				std::cerr << ERROR_FORMAT
						<< "The real parameter count is not the same as formal one.\n";
				g_semanticErrorCount++;
			}
		}
		else if (!(realVT == NULL && formalVT == NULL))
		{
			std::cerr << ERROR_FORMAT
					<< "The real parameter count is not the same as formal one.\n";
			g_semanticErrorCount++;
		}
	}
}

void Node_args::semantic_analysis_postorder()
{
	if (arg_list_)
	{
		args_ = arg_list_->get_args();
	}
}
void Node_arg_list::semantic_analysis_preorder()
{
	this->args_ = new std::vector<VariableTypeAttribute*>();
}
void Node_arg_list::semantic_analysis_inorder(Node_expression* exp)
{
	args_->push_back(&(exp->get_variable_attribute()->type));
}
}

