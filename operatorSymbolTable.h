#ifndef OPERATORSYMBOLTABLE_H_INCLUDED
#define OPERATORSYMBOLTABLE_H_INCLUDED

#include "symbolTable.h"


void creatSymbolTable(void);
SymbolTable *newTableItem(void);
void destroySymbolTable();
int enter(std::string id,AttributeIR *,SymbolTable **entry);
bool findEntry(std::string id,SymbolTable **entry);
bool findField(std::string id,FieldChain *head,FieldChain **entry);


void printSymbolTable(void);
/**void printTable(void);
void printTy(TypeIR *);
void printVar(SymbolTable * entry);
void printProc(SymbolTable * entry);*/

#endif // OPERATORSYMBOLTABLE_H_INCLUDED
