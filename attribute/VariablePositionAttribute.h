/*
 * VariableAttribute.h
 *
 *  Created on: 2013-6-14
 *      Author: Administrator
 */

#ifndef VARIABLEATTRIBUTE_H_
#define VARIABLEATTRIBUTE_H_


#include <assert.h>
#include <string>
#include <stdio.h>

namespace cminus
{
struct VariablePositionAttribute
{

	bool userDefined;
	struct
	{
		bool inRegister;
		int registerIndex;
	}registerPosition;
	enum MemoryArea
	{
		GLOBAL_DATA_AREA,
		STACK
	};
	struct
	{
		bool inMemory;
		MemoryArea where;
		int offset;
	}memoryPosition;
	VariablePositionAttribute()
	{
		userDefined = false;
		registerPosition.inRegister=false;
		registerPosition.registerIndex = 0;
		memoryPosition.inMemory=false;
		memoryPosition.where = GLOBAL_DATA_AREA;
		memoryPosition.offset = 0;
	}
	void StoreInRegister(int index)
	{
		assert(index>=0 && index<32);
		assert(!registerPosition.inRegister);

		registerPosition.inRegister=true;
		registerPosition.registerIndex = index;
	}
	void StoreInMemory(MemoryArea where,int offset)
	{
		assert(!this->memoryPosition.inMemory);

		this->memoryPosition.inMemory = true;
		this->memoryPosition.where = where;
		this->memoryPosition.offset = offset;
	}
	void KickOutOfRegister()
	{
		assert(registerPosition.inRegister);

		registerPosition.inRegister = false;
		registerPosition.registerIndex=0;
	}
	bool InRegister()
	{
		return registerPosition.inRegister;
	}
	bool InMemory()
	{
		return memoryPosition.inMemory;
	}
	std::string AddressCode()
	{
		assert(InMemory());

		if(memoryPosition.where==GLOBAL_DATA_AREA)
		{
			char code[256];
			::sprintf(code,"%d($gp)",memoryPosition.offset);
			return std::string(code);
		}
		else
		{
			char code[256];
			::sprintf(code,"%d($fp)",memoryPosition.offset);
			return std::string(code);
		}
	}
};
}


#endif /* VARIABLEATTRIBUTE_H_ */
