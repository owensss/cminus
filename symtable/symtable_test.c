#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	struct SymbolTableStack* stack;
	int i;
	int type[5000];
	for(i=0; i<sizeof(type)/sizeof(int); i++)
	{
		type[i]=i;
	}

	stack = NewSymbolTableStack(); 
	printf("NewSymbolTableStack\n"); 

	PushSymbolTableStack(stack);
	printf("PushSymbolTableStack\n");

	/*------------------------------------TESt1----------------------------------*/
	printf("Test1\n");

	assert(InsertSymbolTableStack("abc","abctype",stack)==NULL);
	printf("InsertSymbolTableStack\n");

	assert(strcmp((char*)(InsertSymbolTableStack("abc","abctype",stack)->type),"abctype")==0);
	printf("InsertSymbolTableStack Same one twice.\n");
	assert(strcmp((char*)(LookUpSymbolTableStack("abc",stack)->type),"abctype")==0);
	printf("LookUpSymbolTableStack\n");

	/*------------------------------------TESt2----------------------------------*/
	printf("Test2\n");
	
	for(i=0; i<1000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}
	/*------------------------------------TESt3----------------------------------*/
	printf("Test3\n");
	PushSymbolTableStack(stack);
	for(i=1000; i<2000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}
	for(i=0; i<1000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		//assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		//assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}

	/*------------------------------------TESt4----------------------------------*/
	printf("Test4\n");

	PushSymbolTableStack(stack);
	for(i=2000; i<3000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}
	for(i=1000; i<2000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		//assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		//assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}
	for(i=0; i<1000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		//assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		//assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(*((int*)(LookUpSymbolTableStack(buf,stack)->type))==i);
	}

	PopSymbolTableStack(stack);
	for(i=2000; i<3000; i++)
	{
		char buf[256];
		sprintf(buf,"%d",i);
		//assert(InsertSymbolTableStack(buf,&type[i],stack)==NULL);
		//assert(*((int*)(InsertSymbolTableStack(buf,&type[i],stack)->type))==i);
		assert(LookUpSymbolTableStack(buf,stack)==NULL);
	}

	//---------------------------------CleanUp-----------------------------------------
	printf("CleanUp!\n");
	PopSymbolTableStack(stack);
	printf("PopSymbolTableStack\n");

	DeleteSymbolTableStack(stack);
	printf("DeleteSymbolTableStack\n");
	stack = NULL;
	
	return 0;
}
