#include <string>
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include "treeNode.h"
#include "symbolTable.h"
#include "middleCodeGen.h"

#define listing stdout

static int currentTempVariable=0;
int tempVariableId=0;
static int lab=0;
CodeListNode *lastCode=NULL;
CodeListNode *firstCode=NULL;


static ArgRecord *newTemp(AccessKind access);
static ArgRecord *argValue(int value);
static int newLabel();
static ArgRecord *argLabel(int label);
static ArgRecord *argAddr(std::string iaName,int level,int off,AccessKind access);
void printMidCode(CodeListNode *);
static CodeListNode *genCode(CodeKind codekind,ArgRecord *arg1,ArgRecord *arg2,ArgRecord *result);

void genMidCode(TreeNode *);
static void genProcDec(TreeNode *);

static ArgRecord *genVar(TreeNode *);
static ArgRecord *genArray(ArgRecord *,TreeNode *,int low,int sizeMem);
static ArgRecord *genField(ArgRecord *,TreeNode *,FieldChain *head);
static ArgRecord *genExp(TreeNode *);

static void genBody(TreeNode *);
//static void genStatement(TreeNode *);
static void genAssignStatement(TreeNode *);
static void genCallStatement(TreeNode *);
static void genReadStatement(TreeNode *);
static void genWriteStatement(TreeNode *);
static void genIfStatement(TreeNode *);
static void genWhileStatement(TreeNode *);

static ArgRecord *newTemp(AccessKind acc)
{

    ArgRecord *arg = new ArgRecord();

    arg->form=addrForm;
    arg->More.addrArg.dataLevel=-1;
    arg->More.addrArg.dataOff=currentTempVariable;
    arg->More.addrArg.idName="temp";
    arg->More.addrArg.access=acc;
    currentTempVariable++;
    return arg;
}

static ArgRecord *argValue(int val)
{
    ArgRecord *arg=new ArgRecord();

    arg->form=valueForm;
    arg->More.valueArg.value=val;

    return arg;
}

static int newLabel()
{
    lab=lab+1;
    return lab;
}

static ArgRecord *argLabel(int lab)
{
    ArgRecord *arg=new ArgRecord();

    arg->form=labelForm;
    arg->More.labelArg.label=lab;

    return arg;
}

static ArgRecord *argAddr(std::string id,int level,int offset,AccessKind acc)
{
    ArgRecord *arg = new ArgRecord();

    arg->form=addrForm;
    arg->More.addrArg.idName=id;
    arg->More.addrArg.dataLevel=level;
    arg->More.addrArg.dataOff=offset;
    arg->More.addrArg.access=acc;

    return arg;
}


static CodeListNode *genCode(CodeKind codekind,ArgRecord *arg1,ArgRecord *arg2,ArgRecord *result)
{
    MidCode *mid = new MidCode();
    CodeListNode *codelistnode=new CodeListNode();

    codelistnode->midCode=mid;
    codelistnode->midCode->codeKind=codekind;
    codelistnode->midCode->operand1=arg1;
    codelistnode->midCode->operand2=arg2;
    codelistnode->midCode->result=result;

    codelistnode->next=NULL;
    codelistnode->pre=NULL;

    if(firstCode!=NULL)
    {
        lastCode=(lastCode->next=codelistnode);
    }
    else
    {
        firstCode=(lastCode=codelistnode);
    }

    return codelistnode;
}

void genMidCode(TreeNode *t)
{
    TreeNode *temp = t->child[1]->sibling->sibling->child[0];
    while(temp!=NULL)
    {
        genProcDec(temp);
        temp = temp->sibling;
    }

    int offMainPro = t->child[0]->table[0]->attrIR.More.ProcAttr.nOff;//主函数sp指针到display表的偏移
    ArgRecord *argOffMainPro = argValue(offMainPro);
    CodeListNode *cln = genCode(mainEntry,NULL,NULL,argOffMainPro);

    currentTempVariable = t->child[0]->table[0]->attrIR.More.ProcAttr.mOff;//过程活动记录的大小,就是临时变量的初始编号

    printf("mainARsize=%d\n",currentTempVariable);
    genBody(t->child[2]->child[0]);
    printf("mainARsize=%d\n",currentTempVariable);

    t->child[0]->table[0]->attrIR.More.ProcAttr.mOff=currentTempVariable;

    ArgRecord *arg2=argValue(currentTempVariable);
    cln->midCode->operand2=arg2;

    genCode(endProc,NULL,NULL,NULL);

}

static void genProcDec(TreeNode *t)
{
    int procEntryLable=newLabel();
    t->table[0]->attrIR.More.ProcAttr.codeEntry=procEntryLable;
    ArgRecord *operand1=argValue(procEntryLable);//过程入口标号的arg结构
    ArgRecord *operand3=argValue(t->table[0]->attrIR.More.ProcAttr.level);//过程层数的arg结构


    TreeNode *p = t->child[1]->sibling->sibling->child[0];
    while(p != NULL)
    {
        genProcDec(p);
        p=p->sibling;
    }

    CodeListNode *cln=genCode(pentry,operand1,NULL,operand3);//过程入口的中间代码入口标号，活动记录大小，层数

    currentTempVariable=t->table[0]->attrIR.More.ProcAttr.mOff;//初始化此过程临时变量的开始标号为过程活动记录中第一个临时变量的偏移

    printf("ARsize=%d\n",currentTempVariable);
    genBody(t->child[2]->child[0]);
    printf("ARsize=%d\n",currentTempVariable);

    ArgRecord *arg2 = argValue(currentTempVariable);//活动记录大小的arg结构
    cln->midCode->operand2=arg2;
    genCode(endProc,NULL,NULL,NULL);
}

static ArgRecord *genVar(TreeNode *t)
{
    std::string id=t->name[0];
    int level=t->table[0]->attrIR.More.VarAttr.level;
    int offset=t->table[0]->attrIR.More.VarAttr.off;
    AccessKind access=t->table[0]->attrIR.More.VarAttr.access;

    ArgRecord *arg = argAddr(id,level,offset,access);

    if(t->attr.ExpAttr.varkind==ArrayMembV)
    {
        int low=t->table[0]->attrIR.idType->More.ArrayAttr.low;
        int sizeMember = t->table[0]->attrIR.idType->More.ArrayAttr.elemTy->typeSize;
        ArgRecord *argArray=genArray(arg,t->child[0],low,sizeMember);
        return argArray;
    }
    else if(t->attr.ExpAttr.varkind==FieldMembV)
    {
        FieldChain *head=t->table[0]->attrIR.idType->More.body;
        ArgRecord *argField=genField(arg,t->child[0],head);
        return argField;
    }
    else
    {
        return arg;
    }
}

static ArgRecord *genArray(ArgRecord *arg,TreeNode *t,int low,int sizeMem)
{
    ArgRecord *Earg = genExp(t);
    ArgRecord *lowArg = argValue(low);
    ArgRecord *sizeArg = argValue(sizeMem);

    ArgRecord *temp1 = newTemp(direct);
    ArgRecord *temp2 = newTemp(direct);
    ArgRecord *temp3 = newTemp(direct);

    genCode(sub,Earg,lowArg,temp1);
    genCode(mult,temp1,sizeArg,temp2);
    genCode(aadd,arg,temp2,temp3);

    return temp3;
}

static ArgRecord *genField(ArgRecord *arg,TreeNode *t,FieldChain *head)
{
    while( operator!=(t->name[0],head->idName) )
    {
        head=head->next;
    }
    int offset=head->offset;
    ArgRecord *offArg = argValue(offset);
    ArgRecord *temp1 = newTemp(t->table[0]->attrIR.More.VarAttr.access);

    ArgRecord *fieldArg = NULL;
    genCode(aadd,arg,offArg,temp1);

    if(head->unitType->kind==arrayTy)
    {
        int low = head->unitType->More.ArrayAttr.low;
        int sizeMem = head->unitType->typeSize;
        fieldArg=genArray(arg,t->child[0],low,sizeMem);
    }
    else
    {
        fieldArg=temp1;
    }
    return fieldArg;

}

static ArgRecord *genExp(TreeNode *t)
{
    ArgRecord *arg=NULL;
    if(t->kind.exp==VariK)
    //if((t->table[0]->attrIR.kind)==varKind)
    {
        arg = genVar(t);
    }
    else if(t->kind.exp==ConstK)
    //if((t->table[0]->attrIR.kind)==constKind)
    {
        arg = argValue(t->attr.ExpAttr.val);
    }
    else if(t->kind.exp==OpK)
    {
        ArgRecord *Larg = genExp(t->child[0]);
        ArgRecord *Rarg = genExp(t->child[1]);
        ArgRecord *temp = newTemp(direct);

        switch(t->attr.ExpAttr.op)
        {
        case '+':
            genCode(add,Larg,Rarg,temp);
            break;
        case '-':
            genCode(sub,Larg,Rarg,temp);
            break;
        case '*':
            genCode(mult,Larg,Rarg,temp);
            break;
        case '/':
            genCode(divi,Larg,Rarg,temp);
            break;
        case '<':
            genCode(ltc,Larg,Rarg,temp);
            break;
        case '=':
            genCode(eqc,Larg,Rarg,temp);
        default:
            break;
        }
        return temp;

    }
    return arg;
}

static void genBody(TreeNode *t)
{
    while(t!=NULL)
    {
        if((t->nodekind)==StmtK)
        {
            switch(t->kind.stmt)
            {
            case AssignK:
                genAssignStatement(t);
                break;
            case CallK:
                genCallStatement(t);
                break;
            case ReadK:
                genReadStatement(t);
                break;
            case WriteK:
                genWriteStatement(t);
                break;
            case IfK:
                genIfStatement(t);
                break;
            case WhileK:
                genWhileStatement(t);
                break;
            default:
                //fprintf(listing,"no this statement\n");
                break;
            }
        }
        else
        {
            //fprintf(listing,"this node is not statement node\n");
        }
        t=t->sibling;
    }
}

static void genAssignStatement(TreeNode *t)
{
    ArgRecord *Larg=genVar(t->child[0]);
    ArgRecord *Rarg=genExp(t->child[1]);
    genCode(assig,Rarg,NULL,Larg);
}

static void genCallStatement(TreeNode *t)
{
    ParamTable *param = t->child[0]->table[0]->attrIR.More.ProcAttr.param;

    TreeNode *temp=t->child[1];
    while( temp != NULL)
    {
        //AccessKind access = temp->table[0]->attrIR.More.VarAttr.access;
        //int level = temp->table[0]->attrIR.More.VarAttr.level;
        //int off = temp->table[0]->attrIR.More.VarAttr.off;

        ArgRecord *arg = genExp(temp);
        ArgRecord *offsetArg = argValue(param->entry->attrIR.More.VarAttr.off);
        ArgRecord *sizeArg = argValue(param->entry->attrIR.idType->typeSize);
        if(param->entry->attrIR.More.VarAttr.access==direct)
        {
            genCode(valAct,arg,offsetArg,sizeArg);
        }
        else if(param->entry->attrIR.More.VarAttr.access==indirect)
        {
            genCode(varAct,arg,offsetArg,sizeArg);
        }

        param=param->next;
        temp=temp->sibling;
    }

    int label = t->child[0]->table[0]->attrIR.More.ProcAttr.codeEntry;
    ArgRecord *arglab = argLabel(label);

    int val = t->child[0]->table[0]->attrIR.More.ProcAttr.nOff;
    ArgRecord *offsetDisplay = argValue(val);

    genCode(call,arglab,NULL,offsetDisplay);
}

static void genReadStatement(TreeNode *t)
{
    std::string idName = t->name[0];
    int level = t->table[0]->attrIR.More.VarAttr.level;
    int off = t->table[0]->attrIR.More.VarAttr.off;
    AccessKind ak = t->table[0]->attrIR.More.VarAttr.access;
    ArgRecord *arg = argAddr(idName,level,off,ak);

    genCode(readc,arg,NULL,NULL);
}

static void genWriteStatement(TreeNode *t)
{
    ArgRecord *arg = genExp(t->child[0]);
    genCode(writec,arg,NULL,NULL);
}

static void genIfStatement(TreeNode *t)
{
    ArgRecord *elseArg = argLabel(newLabel());
    ArgRecord *outArg = argLabel(newLabel());

    ArgRecord *ifExp = genExp(t->child[0]);
    genCode(jump0,ifExp,elseArg,NULL);

    genBody(t->child[1]);
    genCode(jump,outArg,NULL,NULL);

    genCode(label,elseArg,NULL,NULL);
    genBody(t->child[2]);
    genCode(label,outArg,NULL,NULL);
}

static void genWhileStatement(TreeNode *t)
{
    ArgRecord *inArg = argLabel(newLabel());
    ArgRecord *outArg = argLabel(newLabel());

    genCode(label,inArg,NULL,NULL);
    ArgRecord *arg = genExp(t->child[0]);

    genCode(jump0,arg,outArg,NULL);
    genBody(t->child[1]);
    genCode(jump,inArg,NULL,NULL);
    genCode(label,outArg,NULL,NULL);
}

void printMidCode(CodeListNode *head)
{
    //CodeListNode *head=temp;
    while(head!=NULL)
    {
        switch(head->midCode->codeKind)
        {
        case add:
            fprintf(listing,"add ");
            break;
        case sub:
            fprintf(listing,"sub ");
            break;
        case mult:
            fprintf(listing,"mult ");
            break;
        case divi:
            fprintf(listing,"divi ");
            break;
        case eqc:
            fprintf(listing,"eqc ");
            break;
        case ltc:
            fprintf(listing,"ltc ");
            break;
        case readc:
            fprintf(listing,"readc ");
            break;
        case writec:
            fprintf(listing,"writec ");
            break;
        case returnc:
            fprintf(listing,"returnc ");
            break;
        case assig:
            fprintf(listing,"assig ");
            break;
        case aadd:
            fprintf(listing,"aadd ");
            break;
        case label:
            fprintf(listing,"label ");
            break;
        case jump:
            fprintf(listing,"jump ");
            break;
        case jump0:
            fprintf(listing,"jump0 ");
            break;
        case call:
            fprintf(listing,"call ");
            break;
        case varAct:
            fprintf(listing,"varAct ");
            break;
        case valAct:
            fprintf(listing,"valAct ");
            break;
        case pentry:
            fprintf(listing,"pentry ");
            break;
        case endProc:
            fprintf(listing,"endProc ");
            break;
        case mainEntry:
            fprintf(listing,"mainEntry ");
            break;
        default:
            break;
        }

        if(head->midCode->operand1!=NULL)
        {
            switch(head->midCode->operand1->form)
            {
            case labelForm:
                fprintf(listing,"   %d,",head->midCode->operand1->More.labelArg.label);
                break;
            case valueForm:
                fprintf(listing,"   %d,",head->midCode->operand1->More.valueArg.value);
                break;
            case addrForm:
                //fprintf(listing,"   %s",head->midCode->operand1->More.addrArg.idName);
                cout<<"   "<<head->midCode->operand1->More.addrArg.idName<<",";
            default:
                break;
            }
        }
        else
        {
            fprintf(listing,"   ----,");
        }
        if(head->midCode->operand2!=NULL)
        {
            switch(head->midCode->operand2->form)
            {
            case labelForm:
                fprintf(listing,"   %d,",head->midCode->operand2->More.labelArg.label);
                break;
            case valueForm:
                fprintf(listing,"   %d,",head->midCode->operand2->More.valueArg.value);
                break;
            case addrForm:
                //fprintf(listing,"   %s",head->midCode->operand2->More.addrArg.idName);
                cout<<"   "<<head->midCode->operand2->More.addrArg.idName<<",";
            default:
                break;
            }
        }
        else
        {
            fprintf(listing,"----,");
        }


        if(head->midCode->result!=NULL)
        {
            switch(head->midCode->result->form)
            {
            case labelForm:
                fprintf(listing,"   %d",head->midCode->result->More.labelArg.label);
                break;
            case valueForm:
                fprintf(listing,"   %d",head->midCode->result->More.valueArg.value);
                break;
            case addrForm:
                //fprintf(listing,"   %s",head->midCode->result->More.addrArg.idName);
                cout<<"   "<<head->midCode->result->More.addrArg.idName;
            default:
                break;
            }
        }
        else
        {
            fprintf(listing,"----");
        }

        fprintf(listing,"\n");

        head=head->next;
    }
}
