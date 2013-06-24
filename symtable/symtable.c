#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/**
 *@brief 
 *@param name The name of the symbol,the string will be
 		copyed by this function. And it will be delete 
		after DeleteSymbol called.
 *@param type as the type is void*, so it can be any
 		type. But it will not be copied as name does.
 */
struct Symbol* NewSymbol(const char* name,void* type)
{
	int len;
	struct Symbol* ret = NULL;

	assert(name && type);

	ret = (struct Symbol*)malloc(sizeof(struct Symbol));
	len = strlen(name);
	ret->name = (char*)malloc(len+1);
	strcpy(ret->name,name);
	ret->type = type;

	return ret;
}
void DeleteSymbol(struct Symbol* symbol)
{
	assert(symbol);

	free(symbol->name);
	free(symbol);
}
/**
 *@return Same:return 0
 * Not Same: return others.*/
int CmpSymbol(struct Symbol* left,struct Symbol* right)
{
	assert(left && right);

	return strcmp(left->name,right->name);
}
struct SymbolList* NewSymbolList(struct Symbol* symbol,
		struct SymbolList* symbolList)
{
	struct SymbolList* ret=NULL;
	//symbolList can be NULL.
	assert(symbol);

	ret = (struct SymbolList*)malloc(sizeof(struct SymbolList));
	ret->symbol = symbol;
	ret->next = symbolList;

	return ret;
}
struct SymbolList* DeleteSymbolList(struct SymbolList* symbolList)
{
	struct SymbolList* ret=NULL;

	assert(symbolList);

	ret = symbolList->next;
	DeleteSymbol(symbolList->symbol);
	free(symbolList);

	return ret;
}
/************************************************************************/
/**
*@param symbolList can be NULL.
*/
/************************************************************************/
struct Symbol* LookUpSymbolListAll(const char* name,struct SymbolList* symbolList)
{
	assert(name);
	while(symbolList!=NULL)
	{
		struct Symbol* symbol = NULL;
		symbol = symbolList->symbol;
		if(strcmp(symbol->name,name)==0)
		{
			return symbol;
		}
		symbolList = symbolList->next;
	}
	return NULL;
}
void DeleteSymbolListAll(struct SymbolList* symbolList)
{
	assert(symbolList);
	while((symbolList=DeleteSymbolList(symbolList)));
}
struct SymbolTable* NewSymbolTable(int size,
	unsigned int (*hashFun)(const char* symbolName,int tableSize))
{
	struct SymbolTable* ret=NULL;

	assert(size>0 && hashFun);

	ret = (struct SymbolTable*)malloc(sizeof(struct SymbolTable));
	ret->size = size;
	ret->hashFun = hashFun;

	ret->symbolTable = (struct SymbolList**)malloc(
			sizeof(struct symbolTable*)*(ret->size));
	memset(ret->symbolTable,0,sizeof(struct symbolTable*)*(ret->size));

	return ret;
}
void DeleteSymbolTable(struct SymbolTable* symbolTable)
{
	unsigned int i=0;

	assert(symbolTable);

	for(i=0; i<symbolTable->size; i++)
	{
		struct SymbolList* symbolList = NULL;
		symbolList = symbolTable->symbolTable[i];
		if(symbolList)
		{
			DeleteSymbolListAll(symbolList);
		}
	}
	free(symbolTable->symbolTable);
	free(symbolTable);
}
struct Symbol* LookUpSymbolTable(const char* name,struct SymbolTable* symbolTable)
{
	int index = 0;
	
	assert(name && symbolTable);

	index = (*(symbolTable->hashFun))(name,symbolTable->size);

	return LookUpSymbolListAll(name,symbolTable->symbolTable[index]);
}
struct Symbol* InsertSymbolTable(struct Symbol* symbol,struct SymbolTable*
		symbolTable)
{
	
	int index = 0;
	struct Symbol* symbolRet=NULL;

	assert(symbol && symbolTable);

	index = (*(symbolTable->hashFun))(symbol->name,symbolTable->size);

	symbolRet=LookUpSymbolListAll(symbol->name,symbolTable->symbolTable[index]);
	if(symbolRet!=NULL)
	{
		//Insert the same token twice.
		return symbolRet;
	}
	else
	{
		symbolTable->symbolTable[index]=NewSymbolList(symbol,
				symbolTable->symbolTable[index]);
		return NULL;
	}
}
struct SymbolTableList* NewSymbolTableList(struct SymbolTable* symbolTable,
		struct SymbolTableList* symbolTableList)
{
	struct SymbolTableList* ret = NULL;
	//symbolTableList can be NULL.
	assert(symbolTable);

	ret = (struct SymbolTableList*)malloc(sizeof(struct SymbolTableList));
	ret->symbolTable = symbolTable;
	ret->next = symbolTableList;

	return ret;
}
struct Symbol* InsertSymbolTableList(struct Symbol* symbol,
		struct SymbolTableList* symbolTableList)
{
	assert(symbol && symbolTableList);

	return InsertSymbolTable(symbol,symbolTableList->symbolTable);
}
struct Symbol* LookUpSymbolTableListAll(const char* name,
		struct SymbolTableList* symbolTableList)
{
	assert(symbolTableList);

	while(symbolTableList!=NULL)
	{
		struct Symbol* ret = NULL;
		ret = LookUpSymbolTable(name,symbolTableList->symbolTable);
		if(ret!=NULL)
		{
			return ret;
		}
		symbolTableList = symbolTableList->next;
	}
	return  NULL;
}

struct SymbolTableList* DeleteSymbolTableList(struct SymbolTableList* 
		symbolTableList)
{
	struct SymbolTableList* ret = NULL;
	assert(symbolTableList);

	ret = symbolTableList->next;
	DeleteSymbolTable(symbolTableList->symbolTable);
	free(symbolTableList);

	return ret;
}
void DeleteSymbolTableListAll(
		struct SymbolTableList* symbolTableList)
{
	//assert(symbolTableList);
	if(symbolTableList)
	{
		while((symbolTableList=DeleteSymbolTableList(symbolTableList)));
	}
}
struct SymbolTableStack* NewSymbolTableStack()
{
	struct SymbolTableStack* ret = NULL;
	
	ret = (struct SymbolTableStack*)malloc(sizeof(struct SymbolTableStack));

	ret->current = NULL;
	ret->currentLevel = 0;

	return ret;
}
void DeleteSymbolTableStack(struct SymbolTableStack* stack)
{
	assert(stack);

	DeleteSymbolTableListAll(stack->current);
	free(stack);
}

unsigned int HashFun1(const char* symbolName,int tableSize)
{
	int sum = 0;
	int i=0;

	assert(symbolName && tableSize>0);

	while(symbolName[i]!='\0')
	{
		sum+=symbolName[i]*9;
		i++;
	}
	return sum%tableSize;
}

struct SymbolTableStack* PushSymbolTableStack(struct SymbolTableStack* stack)
{
	assert(stack);

	stack->current = NewSymbolTableList(NewSymbolTable(1331,HashFun1),
			stack->current);
	stack->currentLevel++;

	return stack;
}
void PopSymbolTableStack(struct SymbolTableStack* stack)
{
	assert(stack);
	assert(stack->current);

	stack->current = DeleteSymbolTableList(stack->current);
	stack->currentLevel--;
}
struct Symbol* LookUpSymbolTableStack(const char* name,
		struct SymbolTableStack* stack)
{
	assert(name && stack);
	return LookUpSymbolTableListAll(name,stack->current);
}
struct Symbol* InsertSymbolTableStack(const char* name,void* type,
		struct SymbolTableStack* stack)
{
	struct Symbol* symbol = NULL;
	struct Symbol* ret = NULL;
	symbol = NewSymbol(name,type);
	ret = InsertSymbolTableList(symbol,stack->current);
	if(!ret)
	{
		//success.
		return NULL;
	}
	else
	{
		//failed.
		DeleteSymbol(symbol);
		return ret;
	}
}
