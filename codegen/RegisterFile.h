/*
 * RegisterFile.h
 *
 *  Created on: 2013-6-14
 *      Author: logicworldzju
 */

#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

#include <string>
#include "attribute/VariablePositionAttribute.h"
#include <vector>
#include <stdio.h>

namespace cminus
{

class RegisterFile
{
	class Register
	{
	public:
		Register(const std::string& name, int index) :
				m_name(name), m_index(index), used(false), variableAttribute(NULL)
		{
		}
		bool IsUsed() const
		{
			return this->used;
		}
		VariablePositionAttribute* GetVariableAttribute()
		{
			return variableAttribute;
		}

		void UseRegister(VariablePositionAttribute* variableAttribute)
		{
			assert(variableAttribute);
			assert(!used);
			used = true;
			this->variableAttribute = variableAttribute;
		}
		void UnuseRegister()
		{
			assert(used);
			used = false;
			variableAttribute = NULL;
		}
		void Clear()
		{
			variableAttribute->KickOutOfRegister();
			this->UnuseRegister();
		}
		std::string LeakRegisterCode(int& spOffset)
		{
			assert(used);
			if(variableAttribute->InMemory())
			{
				return std::string("");
			}
			else
			{
				spOffset-=4;
				variableAttribute->StoreInMemory(VariablePositionAttribute::STACK,spOffset);
				char code[256];
				::sprintf(code,"sw %s,%d($fp) #LeakRegisterCode\n",m_name.c_str(),spOffset);
				return std::string(code);
			}
		}
		std::string LoadRegisterCode()
		{
			assert(used);

			char code[256];
			::sprintf(code,"lw %s,%s #LoadRegisterCode\n",
					m_name.c_str(),variableAttribute->AddressCode().c_str());
			return std::string(code);
		}
		std::string GetRegName()
		{
			return m_name;
		}
	private:
		const std::string m_name;
		const int m_index;
		bool used;
		struct VariablePositionAttribute* variableAttribute;

	};
public:
	RegisterFile();
	virtual ~RegisterFile();

	std::string Insert(int& spOffset,VariablePositionAttribute* variableAttribute)
	{
		std::string code = "";
		if(variableAttribute->InRegister())
		{
			return code;
		}
		int unusedIndex = FindOneUnused();
		if(unusedIndex>=0)
		{
			registers[unusedIndex].UseRegister(variableAttribute);
			variableAttribute->StoreInRegister(unusedIndex);
			//code = registers[unusedIndex].LoadRegisterCode();
		}
		else
		{
			std::string leakCode = registers[registerAllocPoint].LeakRegisterCode(spOffset);
			registers[registerAllocPoint].Clear();
			registers[registerAllocPoint].UseRegister(variableAttribute);
			variableAttribute->StoreInRegister(registerAllocPoint);
			registerAllocPoint = ((registerAllocPoint-T0_INDEX+1)%(S7_INDEX-T0_INDEX+1))+T0_INDEX;
			//return code;
			code = leakCode;//+registers[registerAllocPoint].LoadRegisterCode();
		}

		return code;
	}
	std::string LoadWord(int& spOffset,VariablePositionAttribute* variableAttribute)
	{
		if(!variableAttribute->InRegister())
		{
			std::string code = Insert(spOffset,variableAttribute);
			return code+registers[variableAttribute->registerPosition.registerIndex].LoadRegisterCode();
		}
		else
		{
			return std::string("");
		}
	}
	std::string GetRegName(VariablePositionAttribute* variableAttribute)
	{
		return registers[variableAttribute->registerPosition.registerIndex].GetRegName();
	}
	//SReg means $t0,$t1,...,$t7
	std::string StoreTReg(int& spOffset)
	{
		std::string code;
		for(int i=T0_INDEX; i<=T7_INDEX; i++)
		{
			if(registers[i].IsUsed())
			{
				code+=registers[i].LeakRegisterCode(spOffset);
				registers[i].Clear();
			}
		}
		return code;
	}
	void Clear()
	{
		for(int i=T0_INDEX; i<=S7_INDEX; i++)
		{
			if(registers[i].IsUsed())
			{
				registers[i].Clear();
			}
		}
		registerAllocPoint=S0_INDEX;
	}
	void ClearIfTemp(VariablePositionAttribute* variableAttribute)
	{
		assert(variableAttribute->registerPosition.inRegister);

		if(!variableAttribute->userDefined)
		{
			Clear(variableAttribute->registerPosition.registerIndex);
		}
	}
private:
	int FindOneUnused()
	{
		int unusedIndex = registerAllocPoint;
		while(registers[unusedIndex].IsUsed())
		{
			unusedIndex = ((unusedIndex-T0_INDEX+1)%(S7_INDEX-T0_INDEX+1))+T0_INDEX;
			if(unusedIndex==registerAllocPoint)
				break;
		}
		if(registers[unusedIndex].IsUsed())
		{
			return -1;
		}
		else
		{
			registerAllocPoint = ((unusedIndex-T0_INDEX+1)%(S7_INDEX-T0_INDEX+1))+T0_INDEX;
			return unusedIndex;
		}
	}
	void Clear(int index)
	{
		registers[index].Clear();
	}


private:
	static const int REGISTER_COUNT = 32;
	static const int T0_INDEX=8;
	static const int T7_INDEX=15;
	static const int S0_INDEX=16;
	static const int S7_INDEX=23;

	std::vector<Register> registers;
	int registerAllocPoint;
};

} /* namespace cminus */
#endif /* REGISTERFILE_H_ */
