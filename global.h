#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED


#define SCOPESIZE 1000

unsigned int Off=0;
unsigned int mainOff=0;
unsigned int savedOff=0;
unsigned int storeOff=0;
int Level = -1;

const unsigned int INITOFF=7;
SymbolTable *scope[SCOPESIZE];

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#define listing stdout

#endif // GLOBAL_H_INCLUDED
