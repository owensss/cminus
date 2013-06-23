/*
 * GlobalState.h
 *
 *  Created on: 2013-6-15
 *      Author: Administrator
 */

#ifndef GLOBALSTATE_H_
#define GLOBALSTATE_H_

#include <ostream>

namespace cminus
{

struct GlobalState
{
	//stack info.
	int spOffset;
	//global data area info.
	int dataOffset;
	//label id.
	int labelID;

	//code.
	std::ostream& code;

	GlobalState(std::ostream& codeOutStream)
	:code(codeOutStream)
	{
		spOffset = 0;
		dataOffset = 0;
		labelID = 0;
	}

};
}


#endif /* GLOBALSTATE_H_ */
