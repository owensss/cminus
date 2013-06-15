/*
 * IdentifierAttribute.h
 *
 *  Created on: 2013-6-15
 *      Author: Administrator
 */

#ifndef IDENTIFIERATTRIBUTE_H_
#define IDENTIFIERATTRIBUTE_H_

#include "FunctionTypeAttribute.h"
#include "VariablePositionAttribute.h"
#include "VariableTypeAttribute.h"
#include <string>

namespace cminus
{

class IdentifierAttribute
{

public:
	enum IdentifierType
	{
		IDENTIFIER_FUNCTION, IDENTIFIER_VARIABLE
	};
	IdentifierAttribute(IdentifierType identifierType)
	{
		this->identifierType = identifierType;
	}
	IdentifierType GetIdentifierType()
	{
		return identifierType;
	}

private:
	IdentifierType identifierType;
};
struct VariableAttribute: public IdentifierAttribute
{
	std::string name;
	VariablePositionAttribute position;
	VariableTypeAttribute type;
	VariableAttribute(std::string& name) :
		IdentifierAttribute(IdentifierAttribute::IDENTIFIER_VARIABLE)
	{
		this->name = name;
	}
};
struct FunctionAttribute: public IdentifierAttribute
{
	std::string name;
	FunctionTypeAttribute type;
	FunctionAttribute(std::string& name) :
			IdentifierAttribute(IdentifierAttribute::IDENTIFIER_FUNCTION)
	{
		this->name = name;
	}
};

}

#endif /* IDENTIFIERATTRIBUTE_H_ */
