#ifndef MIDDLECODEGEN_H_INCLUDED
#define MIDDLECODEGEN_H_INCLUDED

#include <string>
using namespace std;

#include "symbolTable.h"

enum Form{valueForm,labelForm,addrForm};

struct ArgRecord
{
    Form form;
    struct    /**这里为何不能使用union类型*/
    {
        struct
        {
            int value;
        }valueArg;
        struct
        {
            unsigned int label;
        }labelArg;
        struct
        {
            std::string idName="";
            int dataLevel;
            int dataOff;
            AccessKind access;
        }addrArg;
    } More;
};

enum CodeKind {add,sub,mult,divi,
               eqc,ltc,
               readc,writec,returnc,assig,aadd,label,jump,jump0,call,varAct,valAct,
               pentry,endProc,mainEntry};

struct MidCode
{
    CodeKind codeKind;
    ArgRecord *operand1=NULL;
    ArgRecord *operand2=NULL;
    ArgRecord *result=NULL;
};

struct CodeListNode
{
    CodeListNode *pre=NULL;
    CodeListNode *next=NULL;
    MidCode *midCode=NULL;
};

#endif // MIDDLECODEGEN_H_INCLUDED
