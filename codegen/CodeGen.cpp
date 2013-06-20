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

	void Node_program::generate_code(void)
	{
		//----------------------------------------------------------------------------
		begin_code_ <<".text\n"
				<<".globl "<<FUNCTION_PREFIX<<string("main\n");

		//----------------------------------------------------------------------------
		g_globalState.code<<begin_code_.str();
		//g_globalState.code<<child->get_code();


	}


	void Node_var_declaration::generate_code(void)
	{
		if(g_symbolTableStack.currentLevel==1)
		{
			//This is the topest block.
			variable_attribute_->position.StoreInMemory(VariablePositionAttribute::MemoryArea::
					GLOBAL_DATA_AREA,g_globalState.dataOffset);
			g_globalState.dataOffset+=4;
		}
		else
		{
			g_globalState.spOffset-=4;
			variable_attribute_->position.StoreInMemory(VariablePositionAttribute::STACK,
					g_globalState.spOffset);
		}
	}
	void Node_fun_declaration::generate_code_preorder(void)
	{
		g_globalState.spOffset=-8;
	}
	void Node_fun_declaration::generate_code(void)
	{
		//--------------------------------------------------------------------------
		begin_code_<<FUNCTION_PREFIX<<(char*&)id_<<":";
		begin_code_<<"sw $fp,-4($sp)\n";
		begin_code_<<"addi $fp,$sp,-4\n";
		begin_code_<<"sw $ra,-4($fp)\n";
		begin_code_<<"addi $sp,$fp,"<<g_globalState.spOffset<<"\n";
		//--------------------------------------------------------------------------
		end_code_<<"lw $ra,-4($fp)\n";
		end_code_<<"addi $sp,$fp,4\n";
		end_code_<<"lw $fp,-4($sp)\n";
		end_code_<<"jr $ra\n";
		//--------------------------------------------------------------------------
		g_globalState.code<<begin_code_.str()
				<<end_code_.str();
	}

}




