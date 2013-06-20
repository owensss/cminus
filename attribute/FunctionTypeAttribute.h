/*
 * FunctionTypeAttribute.h
 *
 *  Created on: 2013-6-15
 *      Author: Administrator
 */

#ifndef FUNCTIONTYPEATTRIBUTE_H_
#define FUNCTIONTYPEATTRIBUTE_H_

#include "include/cm_base.h"
#include "VariableTypeAttribute.h"
#include <vector>
namespace cminus
{
	struct FunctionTypeAttribute
	{
		VariableTypeAttribute returntype;
		std::vector<VariableTypeAttribute*>* args;
	};
}


#endif /* FUNCTIONTYPEATTRIBUTE_H_ */
