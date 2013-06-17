/*
 * GlobalState.h
 *
 *  Created on: 2013-6-15
 *      Author: Administrator
 */

#ifndef GLOBALSTATE_H_
#define GLOBALSTATE_H_

#include <string>

namespace cminus
{

struct GlobalState
{
	//stack info.
	int spOffset;
	//global data area info.
	int dataOffset;
	//code.
	std::string code;

	GlobalState()
	{
		spOffset = 0;
		dataOffset = 0;
		code = "";
	}

};
}


#endif /* GLOBALSTATE_H_ */
