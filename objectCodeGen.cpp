#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "objectCodeGen.h"
#include "middleCodeGen.h"

extern CodeListNode *firstCode;

static Label *head=NULL;
static Label *tail=NULL;
static Label *obtainLabel=NULL;

static int instructionCount=0;
static int reg[8]={0,0,0,0,0,0,0,0};

#define ac 0
#define ac1 1
#define ac2 2
#define displayOFF 4
#define top 5
#define sp 6
#define pc 7

Instruction instructionMemory[1024];
int dataMemory[1024];


static void genOperand(ArgRecord *);
static void findAddr(ArgRecord *);
static void findSP(int level);
void genFinalCode(CodeListNode *);
static void genCode(CodeListNode *);
static void genArithmatic(CodeListNode *);
static void geneqc(CodeListNode *);
static void genltc(CodeListNode *);
static void genAADD(CodeListNode *);
static void genRead(CodeListNode *);
static void genWrite(CodeListNode *);
static void genReturn(CodeListNode *);
static void genAssign(CodeListNode *);
static void genLabel(CodeListNode *);
static void genJump(CodeListNode *);
static void genJump0(CodeListNode *);
static void genValact(CodeListNode *);
static void genVaract(CodeListNode *);
static void genCall(CodeListNode *);
static void genPentry(CodeListNode *);
static void genEndProc(CodeListNode *);
static void genMainEntry(CodeListNode *);
//static bool findLabel(int label);
static void addLabel(int,int,labelType);

void stepTM(void);


void genOperand(ArgRecord *arg)
{
    switch(arg->form)
    {
    case valueForm:
        instructionMemory[instructionCount].op=ldc;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=arg->More.valueArg.value;
        instructionMemory[instructionCount].operand3=0;
        instructionCount++;
        break;
    case labelForm:
        instructionMemory[instructionCount].op=ldc;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=arg->More.labelArg.label;
        instructionMemory[instructionCount].operand3=0;
        instructionCount++;
        break;

    case addrForm:
        findAddr(arg);
        if(arg->More.addrArg.access==direct)
        {
            instructionMemory[instructionCount].op=ld;
            instructionMemory[instructionCount].operand1=ac;
            instructionMemory[instructionCount].operand2=0;
            instructionMemory[instructionCount].operand3=ac;
            instructionCount++;
        }
        else if(arg->More.addrArg.access==indirect)
        {
            instructionMemory[instructionCount].op=ld;
            instructionMemory[instructionCount].operand1=ac1;
            instructionMemory[instructionCount].operand2=0;
            instructionMemory[instructionCount].operand3=ac;
            instructionCount++;

            instructionMemory[instructionCount].op=ld;
            instructionMemory[instructionCount].operand1=ac;
            instructionMemory[instructionCount].operand2=0;
            instructionMemory[instructionCount].operand3=ac1;
            instructionCount++;
        }
        break;
    default:
        break;
    }
}

void findAddr(ArgRecord *arg)
{
    int varLevel = arg->More.addrArg.dataLevel;
    int varOFF = arg->More.addrArg.dataOff;

    if(!operator==(arg->More.addrArg.idName,"temp"))
    {
        findSP(varLevel);
        instructionMemory[instructionCount].op=ldc;
        instructionMemory[instructionCount].operand1=ac1;
        instructionMemory[instructionCount].operand2=varOFF;
        instructionMemory[instructionCount].operand3=0;
        instructionCount++;

        instructionMemory[instructionCount].op=addtm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=ac;
        instructionMemory[instructionCount].operand3=ac1;
        instructionCount++;
    }
    else if(operator==(arg->More.addrArg.idName,"temp"))
    {
        instructionMemory[instructionCount].op=ldc;
        instructionMemory[instructionCount].operand1=ac1;
        instructionMemory[instructionCount].operand2=varOFF;
        instructionMemory[instructionCount].operand3=0;
        instructionCount++;

        instructionMemory[instructionCount].op=addtm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=sp;
        instructionMemory[instructionCount].operand3=ac1;
        instructionCount++;
    }
}

void findSP(int level)
{
    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=level;
    instructionMemory[instructionCount].operand3=displayOFF;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=ac;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

}

void genFinalCode(CodeListNode *cln)
{
    memset(dataMemory,0,sizeof(dataMemory));
    memset(instructionMemory,0,sizeof(instructionMemory));

    CodeListNode *currentMidNode=firstCode;
    int saveLoc = 0;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=0;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    saveLoc=instructionCount;
    instructionCount++;

    while(currentMidNode != NULL)
    {
        if(currentMidNode->midCode->codeKind==mainEntry)
        {
            instructionMemory[saveLoc].op=ldc;
            instructionMemory[saveLoc].operand1=pc;
            instructionMemory[saveLoc].operand2=instructionCount;
            instructionMemory[saveLoc].operand3=0;
        }

        genCode(currentMidNode);

        currentMidNode=currentMidNode->next;
    }

    instructionMemory[instructionCount].op=halt;
    instructionMemory[instructionCount].operand1=0;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
}

void genCode(CodeListNode *cln)
{
    switch(cln->midCode->codeKind)
    {
    case add:
    case sub:
    case mult:
    case divi:
        genArithmatic(cln);
        break;

    case eqc:
        geneqc(cln);
        break;

    case ltc:
        genltc(cln);
        break;

    case aadd:
        genAADD(cln);
        break;
    case readc:
        genRead(cln);
        break;
    case writec:
        genWrite(cln);
        break;
    case returnc:
        genReturn(cln);
        break;
    case assig:
        genAssign(cln);
        break;
    case label:
        genLabel(cln);
        break;
    case jump:
        genJump(cln);
        break;
    case jump0:
        genJump0(cln);
        break;
    case valAct:
        genValact(cln);
        break;
    case varAct:
        genVaract(cln);
        break;
    case call:
        genCall(cln);
        break;
    case pentry:
        genPentry(cln);
        break;
    case endProc:
        genEndProc(cln);
        break;
    case mainEntry:
        genMainEntry(cln);
        break;
    default:
        break;
    }
}

void genArithmatic(CodeListNode *cln)
{
    genOperand(cln->midCode->operand1);
    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    genOperand(cln->midCode->operand2);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;

    switch(cln->midCode->codeKind)
    {

    case add:
        instructionMemory[instructionCount].op=addtm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=ac1;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
        break;

    case sub:
        instructionMemory[instructionCount].op=subtm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=ac1;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
        break;

    case mult:
        instructionMemory[instructionCount].op=multm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=ac1;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
        break;

    case divi:
        instructionMemory[instructionCount].op=divitm;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=ac1;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
        break;
    }


    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    findAddr(cln->midCode->result);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

}

void geneqc(CodeListNode *cln)
{
    genOperand(cln->midCode->operand1);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    genOperand(cln->midCode->operand2);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;

    instructionMemory[instructionCount].op=subtm;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=ac1;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    instructionMemory[instructionCount].op=jeq;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=2;
    instructionMemory[instructionCount].operand3=pc;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=pc;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=pc;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;
}

void genltc(CodeListNode *cln)
{
    genOperand(cln->midCode->operand1);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    genOperand(cln->midCode->operand2);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;

    instructionMemory[instructionCount].op=subtm;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=ac1;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    instructionMemory[instructionCount].op=jlt;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=2;
    instructionMemory[instructionCount].operand3=pc;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=pc;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=pc;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;
}

void genAADD(CodeListNode *cln)
{
    if(cln->midCode->operand1->More.addrArg.access=direct)
    {
        findAddr(cln->midCode->operand1);
    }
    else if(cln->midCode->operand1->More.addrArg.access==indirect)
    {
        findAddr(cln->midCode->operand1);

        instructionMemory[instructionCount].op=ld;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
    }

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    genOperand(cln->midCode->operand2);

    instructionMemory[instructionCount].op=addtm;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=ac2;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    findAddr(cln->midCode->result);

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;
}

void genRead(CodeListNode *cln)
{
    instructionMemory[instructionCount].op=in;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    findAddr(cln->midCode->operand1);

    if(cln->midCode->operand1->More.addrArg.access==direct)
    {
        instructionMemory[instructionCount].op=st;
        instructionMemory[instructionCount].operand1=ac2;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
    }
    else if(cln->midCode->operand1->More.addrArg.access==indirect)
    {
        instructionMemory[instructionCount].op=ld;
        instructionMemory[instructionCount].operand1=ac1;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;

        instructionMemory[instructionCount].op=st;
        instructionMemory[instructionCount].operand1=ac2;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac1;
        instructionCount++;
    }
}

void genWrite(CodeListNode *cln)
{
    genOperand(cln->midCode->operand1);

    instructionMemory[instructionCount].op=out;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;
}

void genReturn(CodeListNode *cln)
{
    genEndProc(cln);
}

void genAssign(CodeListNode *cln)
{
    findAddr(cln->midCode->result);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    genOperand(cln->midCode->operand1);

    if(cln->midCode->result->More.addrArg.access==direct)
    {
        instructionMemory[instructionCount].op=st;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac2;
        instructionCount++;
    }
    else if(cln->midCode->result->More.addrArg.access==indirect)
    {
        instructionMemory[instructionCount].op=ld;
        instructionMemory[instructionCount].operand1=ac1;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac2;
        instructionCount++;

        instructionMemory[instructionCount].op=st;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac1;
        instructionCount++;
    }
}

void genLabel(CodeListNode *cln)
{

    int label = cln->midCode->operand1->More.labelArg.label;
    int currentLoc=instructionCount;
    bool flag=false;
    obtainLabel=head;

    while(obtainLabel!=NULL)
    {
        if(obtainLabel->lab==label && obtainLabel->flag==jumpLabel)
        {
            flag=true;
            instructionMemory[obtainLabel->destnationAddr].op=ldc;
            instructionMemory[obtainLabel->destnationAddr].operand1=pc;
            instructionMemory[obtainLabel->destnationAddr].operand2=0;
            instructionMemory[obtainLabel->destnationAddr].operand3=currentLoc;
        }

        obtainLabel=obtainLabel->next;
    }
    if(flag==false)
    {
        addLabel(label,currentLoc,labelLabel);
    }
}

void genJump(CodeListNode *cln)
{
    int label=cln->midCode->operand1->More.labelArg.label;
    bool flag=false;
    obtainLabel=head;

    while(obtainLabel)
    {
        if(obtainLabel->lab==label && obtainLabel->flag==labelLabel)
        {
            flag=true;
            instructionMemory[instructionCount].op=ldc;
            instructionMemory[instructionCount].operand1=pc;
            instructionMemory[instructionCount].operand2=obtainLabel->destnationAddr;
            instructionMemory[instructionCount].operand3=0;
            instructionCount++;
            break;
        }
    }

    if(flag==false)
    {
        instructionCount++;/**预留回填指令的空间*/
        addLabel(label,instructionCount,jumpLabel);
    }
}

void genJump0(CodeListNode *cln)
{
    int saveLoc=0,currentLoc=0;

    genOperand(cln->midCode->operand1);

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    saveLoc=instructionCount;
    instructionCount++;

    genJump(cln);
    currentLoc=instructionCount;

    instructionMemory[saveLoc].op=jne;     /**这条指令是回填指令*/
    instructionMemory[saveLoc].operand1=ac2;
    instructionMemory[saveLoc].operand2=currentLoc;
    instructionMemory[saveLoc].operand3=0;
}

void genValact(CodeListNode *cln)
{
    int paramOFF = cln->midCode->operand2->More.valueArg.value;

    genOperand(cln->midCode->operand1);

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=paramOFF;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;
}

void genVaract(CodeListNode *cln)
{
    int paramOFF  = cln->midCode->operand2->More.valueArg.value;
    if(cln->midCode->operand1->More.addrArg.access==direct)
    {
        findAddr(cln->midCode->operand1);
    }
    else if(cln->midCode->operand1->More.addrArg.access==indirect)
    {
        findAddr(cln->midCode->operand1);

        instructionMemory[instructionCount].op=ld;
        instructionMemory[instructionCount].operand1=ac;
        instructionMemory[instructionCount].operand2=0;
        instructionMemory[instructionCount].operand3=ac;
        instructionCount++;
    }

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=paramOFF;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;
}

void genCall(CodeListNode *cln)
{
    int currentLoc = 0;/**return address is not right*/

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=displayOFF;
    instructionMemory[instructionCount].operand2=6;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    int Noff = cln->midCode->result->More.valueArg.value;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=displayOFF;
    instructionMemory[instructionCount].operand2=Noff;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=currentLoc;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    genJump(cln);
}

void genPentry(CodeListNode *cln)
{
    genLabel(cln);
    int ARsize = cln->midCode->operand2->More.valueArg.value;
    int procLevel = cln->midCode->result->More.valueArg.value;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=sp;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=3;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=4;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=5;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=procLevel;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=2;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    int i=0;
    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=i;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    /**复制调用者的AR空间到新的AR中*/
    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=6;
    instructionMemory[instructionCount].operand3=sp;/**加载调用者display表偏移*/
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=2;
    instructionMemory[instructionCount].operand3=sp;/***加载调用者层数*/
    instructionCount++;

    instructionMemory[instructionCount].op=addtm;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=ac;
    instructionMemory[instructionCount].operand3=sp;/***调用者display表首地址*/
    instructionCount++;

    /**跳到这里最合适*/
    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=ac2;
    instructionMemory[instructionCount].operand3=ac;/**每一次循环旧的AR就向前走动一个display表项*/
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=ac2;
    instructionMemory[instructionCount].operand3=displayOFF;
    instructionCount++;

    instructionMemory[instructionCount].op=addtm;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=ac2;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;      /**每一次循环新的AR的display表项往前移动一个*/

    instructionMemory[instructionCount].op=addtm;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=ac2;
    instructionCount++;

    instructionMemory[instructionCount].op=subtm;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=ac1;
    instructionMemory[instructionCount].operand3=1;
    instructionCount++;

    instructionMemory[instructionCount].op=jlt;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=pc-7;
    instructionCount++;/**循环移动display表项*/

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=procLevel;
    instructionMemory[instructionCount].operand3=displayOFF;
    instructionCount++;

    instructionMemory[instructionCount].op=addtm;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=top;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    instructionMemory[instructionCount].op=st;
    instructionMemory[instructionCount].operand1=top;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=ac;
    instructionCount++;

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=sp;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=top;
    instructionMemory[instructionCount].operand2=ARsize;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;
}

void genEndProc(CodeListNode *cln)
{
    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=3;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=4;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=5;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=displayOFF;
    instructionMemory[instructionCount].operand2=6;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=lda;
    instructionMemory[instructionCount].operand1=top;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=sp;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=sp;
    instructionCount++;

    instructionMemory[instructionCount].op=ld;
    instructionMemory[instructionCount].operand1=pc;
    instructionMemory[instructionCount].operand2=1;
    instructionMemory[instructionCount].operand3=top;
    instructionCount++;
}

void genMainEntry(CodeListNode *cln)
{

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac1;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;

    instructionMemory[instructionCount].op=ldc;
    instructionMemory[instructionCount].operand1=ac2;
    instructionMemory[instructionCount].operand2=0;
    instructionMemory[instructionCount].operand3=0;
    instructionCount++;


    reg[sp]=0;
    reg[top]=cln->midCode->operand2->More.valueArg.value;
    reg[displayOFF]=cln->midCode->result->More.valueArg.value;
}

void addLabel(int label,int addrPC,labelType flag)
{
    Label *lab=new Label();
    lab->destnationAddr=addrPC;
    lab->flag=flag;
    lab->lab=label;
    lab->next=NULL;

    if(head==NULL)
    {
        head=tail=lab;
    }
    else
    {
        tail=(tail->next=lab);
    }
}



void stepTM (void)
{
    int i=0,r=0,s=0,t=0;
    reg[pc]=0;

    reg[0],reg[1],reg[2],reg[3],reg[4],reg[5],reg[6],reg[7];

    while(reg[pc]<=instructionCount-2 && reg[pc]>=0)
    {
        r=instructionMemory[reg[pc]].operand1;
        s=instructionMemory[reg[pc]].operand2;
        t=instructionMemory[reg[pc]].operand3;

        switch(instructionMemory[reg[pc]].op)
        {
        case halt:
            printf("halt    0,   0,   0\n");
            reg[pc]++;
            break;

        case in:
            printf("need read a number:\n");
            scanf("%d",&reg[r]);
            reg[pc]++;
            break;

        case out:
            printf("%d\n",reg[r]);
            reg[pc]++;
            break;

        case addtm:
            reg[r]=reg[s]+reg[t];
            reg[pc]++;
            break;

        case subtm:
            reg[r]=reg[s]-reg[t];
            reg[pc]++;
            break;

        case multm:
            reg[r]=reg[s]*reg[t];
            reg[pc]++;
            break;

        case divitm:
            reg[r]=reg[s]/reg[t];
            reg[pc]++;
            break;

        case ld:
            reg[r]=dataMemory[s+reg[t]];
            reg[pc]++;
            break;

        case st:
            dataMemory[s+reg[t]]=reg[r];
            reg[pc]++;
            break;

        case lda:
            reg[r]=s+reg[t];
            if(r==pc);
            else
                reg[pc]++;
            break;

        case ldc:
            reg[r]=s;
            if(r==pc);
            else
                reg[pc]++;
            break;

        case jlt:
            if(reg[r]<0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        case jle:
            if(reg[r]<=0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        case jgt:
            if(reg[r]>0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        case jge:
            if(reg[r]>=0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        case jeq:
            if(reg[r]==0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        case jne:
            if(reg[r]!=0)
            {
                reg[pc]=s+reg[t];
            }
            else
            {
                reg[pc]++;
            }
            break;
        default:
            break;
        }
    }
}

void printObjectCode(void)
{
    FILE *listing = fopen("3finalCode.txt","w");

    int count=0;
    fprintf(listing,"%d\n",instructionCount);

    for(int i=0;i<instructionCount;i++)
    {
        fprintf(listing,"%d ",count++);
        switch(instructionMemory[i].op)
        {
        case halt:
            fprintf(listing,"halt    0,    0,    0\n");
            break;

        case in:
            fprintf(listing,"in    reg[%d],    0,     0\n",instructionMemory[i].operand1);
            break;

        case out:
            fprintf(listing,"out    reg[%d],    0    0\n",instructionMemory[i].operand1);
            break;

        case addtm:
            fprintf(listing,"addtm  reg[%d],    reg[%d],    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case subtm:
            fprintf(listing,"subtm  reg[%d],    reg[%d],    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case multm:
            fprintf(listing,"multm  reg[%d],    reg[%d],    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case divitm:
            fprintf(listing,"divitm  reg[%d],    reg[%d],    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case ld:
            fprintf(listing,"ld  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case st:
            fprintf(listing,"st  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case lda:
            fprintf(listing,"lda  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case ldc:
            fprintf(listing,"ldc  reg[%d],    %d,    %d\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;

        case jlt:
            fprintf(listing,"jlt  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        case jle:
            fprintf(listing,"jle  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        case jgt:
            fprintf(listing,"jgt  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        case jge:
            fprintf(listing,"jge  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        case jeq:
            fprintf(listing,"jeq  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        case jne:
            fprintf(listing,"jne  reg[%d],    %d,    reg[%d]\n",instructionMemory[i].operand1,instructionMemory[i].operand2,instructionMemory[i].operand3);
            break;
        default:
            break;
        }
    }
    //fflush(listing);
}
