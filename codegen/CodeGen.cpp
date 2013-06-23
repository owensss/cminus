/*
 * CodeGen.cpp
 *
 *  Created on: 2013-6-19
 *      Author: Administrator
 */
#include "GlobalState.h"
#include "RegisterFile.h"
#include "AST/Node.hpp"
#include "attribute/IdentifierAttribute.h"
#include "symtable/symtable.h"
#include <string>
#include <iostream>
#include <cassert>
using std::string;


namespace cminus
{

	//---------------------Code Generator global states-----------------
	GlobalState g_globalState(std::cout);
	RegisterFile g_registerFile;
	extern SymbolTableStack g_symbolTableStack;
	//----------------------Some constant strings.-------------------------
	const static string FUNCTION_PREFIX="fun_";
	//const static string IF_PREFIX="if_";

	void Node_program::generate_code_preorder(void)
	{
		begin_code_<<".data\n";
		begin_code_<<"input_msg:   .asciiz \"\\ninput a number:\"\n";
		begin_code_<<"output_msg:   .asciiz \"\\noutput a number:\"\n";
		//----------------------------------------------------------------------------
		begin_code_ <<".text\n";
		begin_code_	<<".globl "<<"main\n";
		//_start code.
		begin_code_<<"main:\n";
		begin_code_<<"jal "<<FUNCTION_PREFIX<<"main\n";
		begin_code_<<"addi $v0,$zero,10\n";
		begin_code_<<"syscall\n";
		//input code.
		begin_code_<<FUNCTION_PREFIX<<"input:\n";
		begin_code_<<"addi $v0,$zero,4\n";
		begin_code_<<"la $a0, input_msg\n";
		begin_code_<<"syscall\n";
		begin_code_<<"addi $v0,$zero,5\n";
		begin_code_<<"syscall\n";
		begin_code_<<"jr $ra\n";
		//output code.
		begin_code_<<FUNCTION_PREFIX<<"output:\n";
		begin_code_<<"addi $v0,$zero,4\n";
		begin_code_<<"la $a0, output_msg\n";
		begin_code_<<"syscall\n";
		begin_code_<<"addi $v0,$zero,1\n";
		begin_code_<<"lw $a0,0($sp)\n";
		begin_code_<<"syscall\n";
		begin_code_<<"jr $ra\n";

		//----------------------------------------------------------------------------
		g_globalState.code<<begin_code_.str();
	}


	void Node_var_declaration::generate_code_preorder(void)
	{
		if(g_symbolTableStack.currentLevel==1)
		{
			//This is the topest block.
			if(variable_attribute_->type.type==CM_INT_ARRAY)
			{
				variable_attribute_->position.StoreInMemory(
						VariablePositionAttribute::MemoryArea::GLOBAL_DATA_AREA,
						g_globalState.dataOffset);
				variable_attribute_->position.isAddress=false;
				variable_attribute_->position.userDefined=true;
				g_globalState.dataOffset+=4*(variable_attribute_->type.arrayNum);
			}
			else
			{
				variable_attribute_->position.StoreInMemory(
						VariablePositionAttribute::MemoryArea::GLOBAL_DATA_AREA,
						g_globalState.dataOffset);
				variable_attribute_->position.isAddress=false;
				variable_attribute_->position.userDefined=true;
				g_globalState.dataOffset+=4;
			}
		}
		else
		{
			if(variable_attribute_->type.type==CM_INT_ARRAY)
			{
				g_globalState.spOffset-=4*(variable_attribute_->type.arrayNum);
				variable_attribute_->position.StoreInMemory(VariablePositionAttribute::STACK,
						g_globalState.spOffset);
				variable_attribute_->position.isAddress=false;
				variable_attribute_->position.userDefined=true;
			}
			else
			{
				g_globalState.spOffset-=4;
				variable_attribute_->position.StoreInMemory(VariablePositionAttribute::STACK,
					g_globalState.spOffset);
				variable_attribute_->position.isAddress=false;
				variable_attribute_->position.userDefined=true;
			}
		}
		g_globalState.code<<"#Variable "<<variable_attribute_->name<<"="<<
			variable_attribute_->position.AddressCode()<<",IsAddress:"<<
			variable_attribute_->position.isAddress<<"\n";
	}
	void Node_fun_declaration::generate_code_preorder(void)
	{


		begin_code_<<FUNCTION_PREFIX<<(char*&)id_<<":";
		begin_code_<<"sw $fp,-4($sp)\n";
		begin_code_<<"addi $fp,$sp,-4\n";
		begin_code_<<"sw $ra,-4($fp)\n";
		for(int i=0; i<8; i++)
		{
			begin_code_<<"sw $s"<<i<<","<<-8-4*i<<"($fp)\n";
		}
		g_globalState.spOffset=-8-4*7;
		//begin_code_<<"addi $sp,$fp,"<<g_globalState.spOffset<<"\n";
		g_globalState.code<<begin_code_.str();
		//--------------------------------------------------------------------------


	}
	void Node_fun_declaration::generate_code_postorder(void)
	{
		end_code_<<FUNCTION_PREFIX<<this->function_attribute_->name<<"_return:\n";
		for(int i=0; i<8; i++)
		{
			end_code_<<"lw $s"<<i<<","<<-8-4*i<<"($fp)\n";
		}
		end_code_<<"lw $ra,-4($fp)\n";
		end_code_<<"addi $sp,$fp,4\n";
		end_code_<<"lw $fp,-4($sp)\n";
		end_code_<<"jr $ra\n";

		g_globalState.code<<end_code_.str();
	}

	void Node_param::generate_code_preorder()
	{
		assert(param_index>=0);
		assert(this->variable_attribute_);
		if(variable_attribute_->type.type==CM_INT_ARRAY)
		{
			variable_attribute_->position.StoreInMemory(VariablePositionAttribute::STACK,
					4+4*param_index);
			variable_attribute_->position.isAddress=true;
			variable_attribute_->position.userDefined=true;
		}
		else
		{
			variable_attribute_->position.StoreInMemory(VariablePositionAttribute::STACK,
								4+4*param_index);
			variable_attribute_->position.isAddress=false;
			variable_attribute_->position.userDefined=true;
		}

		g_globalState.code<<"#Variable "<<variable_attribute_->name<<"="<<
				variable_attribute_->position.AddressCode()<<",IsAddress:"<<
				variable_attribute_->position.isAddress<<"\n";
	}
	void Node_compound_stmt::generate_code_postorder()
	{
		g_registerFile.Clear();
	}
	void Node_expression_stmt::generate_code_preorder()
	{
		begin_code_<<"#Line:"<<this->row()<<",Type:expression\n";
		g_globalState.code<<begin_code_.str();
	}
	void Node_expression_stmt::generate_code_postorder()
	{
		g_registerFile.Clear();
	}
	void Node_selection_stmt::generate_code_preorder()
	{
		g_globalState.labelID++;
		labelID=g_globalState.labelID;
		begin_code_<<"#Line:"<<expr_->row()<<",Type:if\n";
		g_globalState.code<<begin_code_.str();
	}
	void Node_selection_stmt::generate_code_if()
	{
		VariableAttribute* va = expr_->get_variable_attribute();
		assert(va);
		if_code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
		if_code_<<"beq $zero,"<<g_registerFile.GetRegName(&(va->position))<<",";
		if(else_)
		{
			if_code_<<"else_"<<labelID;
		}
		else
		{
			if_code_<<"end_if_"<<labelID;
		}
		if_code_<<"\n";
		g_globalState.code<<if_code_.str();
		g_registerFile.Clear();
	}
	void Node_selection_stmt::generate_code_else()
	{
		else_code_<<"j end_if_"<<labelID<<"\n";
		else_code_<<"else_"<<labelID<<":\n";

		g_globalState.code<<else_code_.str();
	}
	void Node_selection_stmt::generate_code_postorder()
	{
		end_code_<<"end_if_"<<labelID<<":\n";
		g_registerFile.Clear();

		g_globalState.code<<end_code_.str();
	}
	void Node_iteration_stmt::generate_code_preorder()
	{
		g_globalState.labelID++;
		labelID=g_globalState.labelID;
		begin_code_<<"#Line:"<<expr_->row()<<",Type:while\n";
		begin_code_<<"while_"<<labelID<<":\n";

		g_globalState.code<<begin_code_.str();
	}
	void Node_iteration_stmt::generate_code_inorder()
	{
		VariableAttribute* va = expr_->get_variable_attribute();
		assert(va);
		while_code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
		while_code_<<"beq $zero,"<<g_registerFile.GetRegName(&(va->position))<<","
				<<"end_while_"<<labelID<<"\n";
		g_globalState.code<<while_code_.str();
		g_registerFile.Clear();
	}
	void Node_iteration_stmt::generate_code_postorder()
	{
		end_code_<<"j while_"<<labelID<<"\n";
		end_code_<<"end_while_"<<labelID<<":\n";
		g_globalState.code<<end_code_.str();
	}
	void Node_return_stmt::generate_code_preorder()
	{
		begin_code_<<"#Line:"<<this->row()<<",Type:return\n";
		g_globalState.code<<begin_code_.str();
	}
	void Node_return_stmt::generate_code_postorder()
	{
		if(expr_)
		{
			VariableAttribute* va = expr_->get_variable_attribute();
			assert(va);
			code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
			code_<<"add $v0,$zero,"<<g_registerFile.GetRegName(&(va->position))<<"\n";
		}
		code_<<"j "<<FUNCTION_PREFIX<<this->function_attribute_->name<<"_return\n";

		g_globalState.code<<code_.str();
	}
	void Node_expression::generate_code_postorder()
	{
		if(expr_)
		{
			std::string exprReg;
			{
				VariableAttribute* va = expr_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				exprReg=g_registerFile.GetRegName(&(va->position));

			}
			{
				if(var_->get_variable_attribute()->position.isAddress)
				{
					VariableAttribute* va = var_->get_variable_attribute();
					assert(va);
					code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
					code_<<"sw "<<exprReg<<",";
					code_<<"0("<<g_registerFile.GetRegName(&(va->position))<<")\n";
				}
				else
				{
					code_<<"sw "<<exprReg<<",";
					code_<<var_->get_variable_attribute()->position.AddressCode()<<
							" #"<<var_->get_variable_attribute()->name<<"\n";
				}
			}
			g_globalState.code<<code_.str();
		}

	}
	void Node_var::generate_code_postorder()
	{
		if(expr_)
		{
			VariableAttribute* va = expr_->get_variable_attribute();
			assert(va);
			code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
			std::string offset=g_registerFile.GetRegName(&(va->position));

			this->variable_attribute_->position.isAddress=true;
			code_<<g_registerFile.Insert(g_globalState.spOffset,
					&this->variable_attribute_->position);
			std::string dest = g_registerFile.GetRegName(&(variable_attribute_->position));

			Symbol* ret = LookUpSymbolTableStack((char*&) id_, &g_symbolTableStack);
			VariableAttribute* varAttr = (VariableAttribute*) ret->type;
			assert(varAttr);
			assert(offset!=dest);

			code_<<"sll "<<dest<<","<<offset<<",2\n";
			if(varAttr->position.isAddress)
			{
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(varAttr->position));
				std::string base=g_registerFile.GetRegName(&(varAttr->position));

				assert(offset!=base && base!=dest);
				code_<<"add "<<dest<<","<<dest<<","<<base<<"\n";
			}
			else
			{
				std::string base;
				if(varAttr->position.memoryPosition.where==
						VariablePositionAttribute::GLOBAL_DATA_AREA)
				{
					base="$gp";
				}
				else
				{
					base="$fp";
				}
				code_<<"add "<<dest<<","<<dest<<","<<base<<"\n";
				code_<<"addi "<<dest<<","<<dest<<","<<varAttr->position.memoryPosition.offset<<"\n";
			}

			g_globalState.code<<code_.str();

		}
	}
	void Node_simple_expression::generate_code_postorder()
	{
		if(add2_)
		{
			std::string add1Reg,add2Reg,dest;
			{
				VariableAttribute* va = add1_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				add1Reg=g_registerFile.GetRegName(&(va->position));
			}
			{
				VariableAttribute* va = add2_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				add2Reg=g_registerFile.GetRegName(&(va->position));
			}
			{
				code_<<g_registerFile.Insert(g_globalState.spOffset,
						&this->variable_attribute_->position);
				dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			}
			assert(add1Reg!=add2Reg && add1Reg!=dest && add2Reg!=dest);
			switch ((cm_relops&) relop_)
			{
				case cm_gt:
				{
					code_<<"slt "<<dest<<","<<add2Reg<<","<<add1Reg<<"\n";
					break;
				}
				case cm_lt:
				{
					code_<<"slt "<<dest<<","<<add1Reg<<","<<add2Reg<<"\n";
					break;
				}
				case cm_ge:
				{
					code_<<"slt "<<dest<<","<<add1Reg<<","<<add2Reg<<"\n";
					code_<<"xori "<<dest<<","<<dest<<",1\n";
					break;
				}
				case cm_le:
				{
					code_<<"slt "<<dest<<","<<add2Reg<<","<<add1Reg<<"\n";
					code_<<"xori "<<dest<<","<<dest<<",1\n";
					break;
					break;
				}
				case cm_equ:
				{
					//code_<<"sub "<<dest<<","<<add1Reg<<","<<add2Reg<<"\n";
					g_globalState.labelID++;
					code_<<"add "<<dest<<",$zero,$zero\n";
					code_<<"bne "<<add1Reg<<","<<add2Reg<<",equ_test_"<<g_globalState.labelID<<"\n";
					code_<<"addi "<<dest<<",$zero,1\n";
					code_<<"equ_test_"<<g_globalState.labelID<<":\n";

					break;
				}
				case cm_ueq:
				{
					g_globalState.labelID++;
					code_<<"add "<<dest<<",$zero,$zero\n";
					code_<<"beq "<<add1Reg<<","<<add2Reg<<",ueq_test_"<<g_globalState.labelID<<"\n";
					code_<<"addi "<<dest<<",$zero,1\n";
					code_<<"ueq_test_"<<g_globalState.labelID<<":\n";
					break;
				}
			}
			g_globalState.code<<code_.str();
		}
	}
	void Node_additive_expression::generate_code_postorder()
	{
		if(add_)
		{
			std::string addReg,termReg,dest;

			{
				VariableAttribute* va = add_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				addReg=g_registerFile.GetRegName(&(va->position));
			}
			{
				VariableAttribute* va = term_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				termReg=g_registerFile.GetRegName(&(va->position));
			}
			{
				code_<<g_registerFile.Insert(g_globalState.spOffset,
						&this->variable_attribute_->position);
				dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			}
			assert(addReg!=termReg && addReg!=dest && termReg!=dest);
			switch((cm_ops&)op_)
			{
				case cm_plus:
				{
					code_<<"add "<<dest<<","<<addReg<<","<<termReg<<"\n";
					break;
				}
				case cm_minus:
				{
					code_<<"sub "<<dest<<","<<addReg<<","<<termReg<<"\n";
					break;
				}
				default:
					assert(0);
					break;
			}
			g_globalState.code<<code_.str();
		}
	}
	void Node_term::generate_code_postorder()
	{
		if(term_)
		{
			std::string termReg,factorReg,dest;
			{
				VariableAttribute* va = term_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				termReg=g_registerFile.GetRegName(&(va->position));
			}
			{
				VariableAttribute* va = factor_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				factorReg=g_registerFile.GetRegName(&(va->position));
			}
			{
				code_<<g_registerFile.Insert(g_globalState.spOffset,
						&this->variable_attribute_->position);
				dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			}
			assert(termReg!=factorReg && termReg!=dest && factorReg!=dest);
			switch((cm_ops&)op_)
			{
				case cm_multi:
				{
					code_<<"mult "<<termReg<<","<<factorReg<<"\n";
					code_<<"mflo "<<dest<<"\n";
					break;
				}
				case cm_div:
				{
					code_<<"div "<<termReg<<","<<factorReg<<"\n";
					code_<<"mflo "<<dest<<"\n";
					break;
				}
				default:
					assert(0);
					break;

			}
			g_globalState.code<<code_.str();
		}
	}
	void Node_factor::generate_code_postorder()
	{
		if(type==t_num)
		{
			unsigned int  num = (unsigned int)*u.num_;

			code_<<g_registerFile.Insert(g_globalState.spOffset,
					&this->variable_attribute_->position);
			std::string dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			if((num&0xffff0000)!=0)
			{
				code_<<"lui "<<dest<<","<<((num&0xffff0000)>>16)<<"\n";
				code_<<"ori "<<dest<<","<<dest<<","<<(num&0xffff)<<"\n";
			}
			else
			{
				code_<<"ori "<<dest<<",$zero,"<<(num&0xffff)<<"\n";
			}

		}
		if(type==t_var && u.var_->get_variable_attribute()->position.isAddress==true &&
				u.var_->get_variable_attribute()->type.type==CM_INT)
		{
			std::string addressReg,dest;
			{
				VariableAttribute* va = u.var_->get_variable_attribute();
				assert(va);
				code_<<g_registerFile.LoadWord(g_globalState.spOffset,&(va->position));
				addressReg=g_registerFile.GetRegName(&(va->position));
			}

			{
				this->variable_attribute_ = new VariableAttribute("temp");
				this->variable_attribute_->type.arrayNum = 0;
				this->variable_attribute_->type.type = CM_INT;
				this->variable_attribute_->position.isAddress=false;
				this->variable_attribute_->position.userDefined=false;

				code_<<g_registerFile.Insert(g_globalState.spOffset,
						&this->variable_attribute_->position);
				dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			}
			assert(addressReg!=dest);
			code_<<"lw "<<dest<<",0("<<addressReg<<")\n";
		}

		g_globalState.code<<code_.str();
	}
	void Node_call::generate_code_preorder()
	{

	}
	void Node_call::generate_code_postorder()
	{
		code_<<"jal "<<FUNCTION_PREFIX<<function_attribute_->name<<"\n";

		if(this->variable_attribute_->type.type==CM_INT)
		{
			code_<<g_registerFile.Insert(g_globalState.spOffset,
					&this->variable_attribute_->position);
			std::string dest = g_registerFile.GetRegName(&(variable_attribute_->position));
			code_<<"add "<<dest<<",$zero,$v0\n";
		}
		g_globalState.code<<code_.str();
	}
	void Node_arg_list::generate_code_postorder()
	{
		g_registerFile.StoreTReg(g_globalState.spOffset);
		for(int i=(int)(vector_expr_.size()-1);i>=0; i--)
		{
			g_globalState.spOffset-=4;
			VariableAttribute* va  = vector_expr_[i]->get_variable_attribute();
			if(va->type.type==CM_INT)
			{
				assert(!va->position.isAddress);
				if(va->position.InRegister())
				{
					code_<<"sw "<<g_registerFile.GetRegName(&va->position)
							<<","<<g_globalState.spOffset<<"($fp)\n";
				}
				else
				{
					assert(va->position.InMemory());
					code_<<"lw $v0,"<<va->position.AddressCode()<<"\n";
					code_<<"sw $v0,"<<g_globalState.spOffset<<"($fp)\n";
				}
			}
			if(va->type.type==CM_INT_ARRAY)
			{

				//assert(va->position.InMemory());
				if(va->position.isAddress)
				{
					if(va->position.InRegister())
					{
						code_<<"sw "<<g_registerFile.GetRegName(&va->position)
								<<","<<g_globalState.spOffset<<"($fp)\n";
					}
					else
					{
						assert(va->position.InMemory());
						code_<<"lw $v0,"<<va->position.AddressCode()<<"\n";
						code_<<"sw $v0,"<<g_globalState.spOffset<<"($fp)\n";
					}
				}
				else
				{
					assert(va->position.InMemory());
					code_<<"addi $v0,"<<va->position.RegName()<<","
							<<va->position.memoryPosition.offset<<"\n";
					code_<<"sw $v0,"<<g_globalState.spOffset<<"($fp)\n";
				}
			}
		}
		code_<<"addi $sp,$fp,"<<g_globalState.spOffset<<"\n";

		g_globalState.code<<code_.str();
	}
}





