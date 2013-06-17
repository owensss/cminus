/*
 * RegisterFile_test.cpp
 *
 *  Created on: 2013-6-15
 *      Author: Administrator
 */
#include "RegisterFile.h"
#include "GlobalState.h"
#include <iostream>
#include <string>

using namespace cminus;
using namespace std;

GlobalState g_globalState;
int main()
{

	RegisterFile registerFile;
	for(int i=0; i<32; i++)
	{
		VariablePositionAttribute* vpa = new VariablePositionAttribute();
		string code = registerFile.Insert(g_globalState.spOffset,vpa);

		cout<<"spOffset:"<<g_globalState.spOffset<<",vpa->registerIndex:"<<
				vpa->registerPosition.registerIndex<<endl;
		cout<<"code:"<<code<<endl;
	}

	registerFile.Clear();

	for(int i=0; i<32; i++)
	{
		VariablePositionAttribute* vpa = new VariablePositionAttribute();
		string code = registerFile.Insert(g_globalState.spOffset,vpa);

		cout<<"spOffset:"<<g_globalState.spOffset<<",vpa->registerIndex:"<<
				vpa->registerPosition.registerIndex<<endl;
		cout<<"code:"<<code<<endl;
	}
	cout<<"*************************Test_LoadWord*****************************************"<<endl;
	for(int i=0; i<32; i++)
	{
		VariablePositionAttribute* vpa = new VariablePositionAttribute();
		vpa->StoreInMemory(VariablePositionAttribute::GLOBAL_DATA_AREA,i*4);

		string code = registerFile.LoadWord(g_globalState.spOffset,vpa);

		cout<<"spOffset:"<<g_globalState.spOffset<<",vpa->registerIndex:"<<
				vpa->registerPosition.registerIndex<<endl;
		cout<<"code:"<<code<<endl;
	}
	//registerFile.Clear();
	cout<<"StoreTReg_1:\n"
		<<registerFile.StoreTReg(g_globalState.spOffset);
	for(int i=0; i<32; i++)
	{
		VariablePositionAttribute* vpa = new VariablePositionAttribute();
		string code = registerFile.Insert(g_globalState.spOffset,vpa);

		cout<<"spOffset:"<<g_globalState.spOffset<<",vpa->registerIndex:"<<
				vpa->registerPosition.registerIndex<<endl;
		cout<<"code:"<<code<<endl;
	}
	cout<<"******************************test StoreTReg******************************************"<<endl;
	cout<<"StoreTReg_2:\n"
		<<registerFile.StoreTReg(g_globalState.spOffset);

	return 0;
}


