/*
 * RegisterFile.cpp
 *
 *  Created on: 2013-6-14
 *      Author: Administrator
 */

#include "RegisterFile.h"
#include <assert.h>
#include <string>
namespace cminus
{

RegisterFile::RegisterFile()
{
	// TODO Auto-generated constructor stub
	//init the registers.
	std::string registerName[] =
	{ "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1",
			"$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2",
			"$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1",
			"$gp", "$sp", "$fp", "$ra" };
	for (int i = 0; i < REGISTER_COUNT; i++)
	{
		Register registerA(registerName[i],i);
		registers.push_back(registerA);
	}

	registerAllocPoint = S0_INDEX;
}

RegisterFile::~RegisterFile()
{
	// TODO Auto-generated destructor stub
}

} /* namespace cminus */
