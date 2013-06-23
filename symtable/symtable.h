#ifndef _SYM_TABLE_H_2013_6_5
#define _SYM_TABLE_H_2013_6_5

struct Symbol
{
	char* name;
	void* type;
};
struct SymbolList
{
	struct Symbol* symbol;
	struct SymbolList* next;
};
struct SymbolTable
{
	struct SymbolList* *symbolTable;
	unsigned int size;
	unsigned int (*hashFun)(const char* symbolName,int tableSize);
};
struct SymbolTableList
{
	struct SymbolTable* symbolTable;
	struct SymbolTableList* next;
};
struct SymbolTableStack
{
	struct SymbolTableList* current;
	int currentLevel;
};
/**
 *@brief return the New SymbolTableStack .
 */
extern struct SymbolTableStack* NewSymbolTableStack();
/**
 *@brief the Delete SymbolTableStack,it will delete all the structure created by the
 * SymbolTable Module.
 */
extern void DeleteSymbolTableStack(struct SymbolTableStack* stack);
/**
 *@brief it will create a new SymbolTable and push it into the SymbolTableStack.
 */
extern struct SymbolTableStack* PushSymbolTableStack(
		struct SymbolTableStack* stack);
/**
 *@brief it will pop the top SymbolTable, and delete it.
 */
extern void PopSymbolTableStack(
		struct SymbolTableStack* stack);
/**
 *@return the Symbol looking for.It will search in all the symbol tables in the stack from
 *top to bottom.And return it,if find one.
 *-NULL if it can't find one.
 */
extern struct Symbol* LookUpSymbolTableStack(char* name,
		struct SymbolTableStack* stack);
/**
 *@brief it will be inserted into the topest SymbolTable.
 *@note I will copy the name string,and store the string on the heap.So you can send me the
 *the string from the same buffer every time. But for the void* type,I will just remember the
 *address,and will not delete it after the DeleteSymbolTableStack or PopSymbolTableStack be
 *called.Also return it back when you find it through LookUpSymbolTableStack.
 *@return 
 * -Success return NULL
 * -Falied  return the same name symbol inserted before.
 */
extern struct Symbol* InsertSymbolTableStack(char* name,void* type,
		struct SymbolTableStack* stack);



#endif //_SYM_TABLE_H_2013_6_5
