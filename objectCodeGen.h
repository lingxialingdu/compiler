#ifndef OBJECTCODEGEN_H_INCLUDED
#define OBJECTCODEGEN_H_INCLUDED

#include "middleCodeGen.h"

enum Operation{
               halt,in,out,addtm,subtm,multm,divitm,
               ld,st,
               lda,ldc,jlt,jle,jgt,jge,jeq,jne};

enum labelType{jumpLabel,labelLabel};

struct Label
{
    int lab;
    int destnationAddr;
    labelType flag;
    Label *next;
};

struct Instruction
{
    Operation op;
    int operand1;
    int operand2;
    int operand3;
};

#endif // TM_H_INCLUDED
