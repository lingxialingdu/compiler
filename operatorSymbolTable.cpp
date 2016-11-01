#include <string>
#include <iostream>
using namespace std;
#include<stdio.h>

#include "symbolTable.h"
#include "operatorSymbolTable.h"


#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE


#define listing stdout

unsigned int Off=0;
//unsigned int mainOff=0;
//unsigned int savedOff=0;
//unsigned int storeOff=0;
int Level = -1;
const unsigned int INITOFF=7;

extern SymbolTable *scope[];


void printSymbolTable(void);
static void printTy(TypeIR *);
static void printVar(SymbolTable * entry);
static void printProc(SymbolTable * entry);
static void printTab(unsigned int);

/**创建一个新的符号表项目*/
SymbolTable * newTableItem(void)
{
	//SymbTable * table = (SymbTable *) malloc(sizeof(SymbTable));
	SymbolTable * table = new SymbolTable();

    if (table==NULL)
	{
		//fprintf(listing,"Out of memory error !");
		//Error = TRUE;
	}
	table->next = NULL;
	table->attrIR.idType = NULL;

	table->attrIR.More.VarAttr.isParam = false;

	return table;
}

/**为一个局部化区域专门新建一个符号表*/
void creatSymbolTable(void)
{
	Level = Level +1;                 /*层数加一*/
	scope[Level] = NULL;	          /*申请了新的一层scope栈的空间*/
	Off = INITOFF;			          /*偏移初始化*/
}


/**离开局部化区域时销毁一张表*/
void  destroySymbolTable(void)
{
	/*如果语义分析跟踪标志为TURE，则将语法分析产生的符号表显示出来*/
	/*if ((TraceTable)&&(Error==FALSE))
	{
		printTable();
		getchar();
	}*/
	Level = Level - 1;
}


int  enter(std::string id, AttributeIR *attribP, SymbolTable **entry)
{
	int present = FALSE;
	SymbolTable * curentry = scope[Level];
	SymbolTable * prentry = scope[Level];

	if(scope[Level]==NULL)
	{
		curentry = newTableItem();
		scope[Level] = curentry;
	}
	else
	{
		while (curentry != NULL)
		{
			prentry = curentry;
			if(operator==(id,curentry->idname))
            {
                present=TRUE;
            }
			else
				curentry = (prentry->next);
		}

		if(present==FALSE)
		{
			curentry = newTableItem();
			prentry->next = curentry;
		}
	}
		curentry->idname=id;
		curentry->attrIR.idType = attribP->idType;
		curentry->attrIR.kind = attribP->kind;

		switch( attribP->kind)
		{
		  case  typeKind: break;
		  case  varKind:
			   curentry->attrIR.More.VarAttr.level =attribP->More.VarAttr.level;
			   curentry->attrIR.More.VarAttr.off=attribP->More.VarAttr.off;
			   curentry->attrIR.More.VarAttr.access=attribP->More.VarAttr.access;
			   break;
		  case  routKind:
			   curentry->attrIR.More.ProcAttr.level=attribP->More.ProcAttr.level;
			   curentry->attrIR.More.ProcAttr.param=attribP->More.ProcAttr.param;
			   break;
		  default :break;
		}
		(* entry) = curentry;

	return present;
}


bool findEntry(std::string id , SymbolTable ** entry)
{
	int present = FALSE;    /*返回值*/
	int lev = Level;		/*临时记录层数的变量*/

	SymbolTable *  findentry = scope[lev];

	while((lev!=-1)&&(present!=TRUE))
	{
		while ((findentry!=NULL)&&(present!=TRUE))
		{
            if(operator==(id,findentry->idname))
            {
                present=TRUE;
            }
			else
				findentry = findentry->next;
							/*如果没找到，则继续链表中的查找*/
		}

		if(present == FALSE)
		{
			lev = lev-1;
			findentry = scope[lev];
		}/*如果在本层中没有查到，则转到上一个局部化区域中继续查找*/
	}

    if (present == FALSE)
		(* entry) = NULL;
	else
		(* entry) = findentry;

	return present;
}


void printSymbolTable(void)
{
	SymbolTable * table = NULL;
	TypeIR * tp = NULL;

    fprintf(listing,"\n symbol table:\n\n");

    Level=0;
	while( scope[Level]!=NULL )
	{
		fprintf(listing,"\n------  level: %d  ------\n",Level);
		table = scope[Level];
		while (table!=NULL)
		{
			switch(table->attrIR.kind)
			{
			case typeKind:
				fprintf(listing,"typeDec:\n");
				printTab(5);
				//fprintf(listing,"name->%s;\n",table->idName);
				cout<<"name->"<<table->idname<<";\n";
				printTab(5);
				tp = table->attrIR.idType;
				if(tp!=NULL)
					printTy(tp);
				else
					fprintf(listing,"type error!\n");
				break;

			case varKind:
				fprintf(listing,"varDec:\n");
				printTab(5);
				printVar(table);
				break;
			case routKind:
				fprintf(listing,"procDec:\n");
				printTab(5);
				printProc(table);
				break;
            case constKind:
            case fieldKind:
                break;
			}
			table = table->next;
		}
		Level = Level+1;
	}
}


/***********************************************************/
/* 函数名  printTy                                         */
/* 功  能  把类型内部表示在输出文件中显示出来              */
/* 说  明  显示其类型名，大小，及其他相关信息              */
/***********************************************************/
static void printTy(TypeIR *ty)
{
	switch(ty->kind)
	{
	case intTy:
		fprintf(listing,"kind->intTy;\n");
		printTab(5);
		fprintf(listing,"size->%d;\n",ty->typeSize);
		break;
	case charTy:
		fprintf(listing,"kind->charTy;\n");
		printTab(5);
		fprintf(listing,"size->%d;\n",ty->typeSize);
		break;
	case arrayTy:
		fprintf(listing,"kind->arrayTy;\n");
		printTab(5);
		fprintf(listing,"size->%d;\n",ty->typeSize);
		printTab(5);

		if(ty->More.ArrayAttr.indexTy->kind==intTy)
		    fprintf(listing,"indextype->intTy;\n");
		else
			fprintf(listing,"indextype->charTy;\n");

		printTab(5);
		if(ty->More.ArrayAttr.elemTy->kind==intTy)
		    fprintf(listing,"elemtype->intTy;\n");
		else
			fprintf(listing,"elemtype->charTy;\n");
		break;
	case recordTy:
		fprintf(listing,"kind->recordTy;\n");
		printTab(5);
		fprintf(listing,"size->%d;\n",ty->typeSize);
		FieldChain * tyBody = ty->More.body;
		fprintf(listing,"\n");
		printTab(3);
		fprintf(listing,"field:\n");
		while(tyBody!=NULL)
		{
			printTab(5);
			//fprintf(listing,"id->%s;\n",tyBody->id);
			cout<<"id->"<<tyBody->idName<<";\n";
			printTab(5);
			fprintf(listing,"unit");
			printTy(tyBody->unitType);
			printTab(5);
			fprintf(listing,"off->%d;\n",tyBody->offset);
			tyBody = tyBody->next;
			fprintf(listing,"\n");
		}
		break;
	}
}

/***********************************************************/
/* 函数名  printVar                                        */
/* 功  能  把变量内部表示在输出文件中显示出来              */
/* 说  明  显示其变量名称，偏移，类型名及其他              */
/***********************************************************/
static void printVar(SymbolTable * entry)
{
    //fprintf(listing,"name->%s;\n",entry->idName);
    cout<<"name->"<<entry->idname<<";\n";

	AttributeIR  attr = entry->attrIR;
	printTab(5);
	fprintf(listing,"level->%d;\n",attr.More.VarAttr.level);
	printTab(5);
	fprintf(listing,"off->%d;\n",attr.More.VarAttr.off);
	printTab(5);
	if(attr.More.VarAttr.isParam==true)
		fprintf(listing,"param\n");
	else
		fprintf(listing,"not param\n");
	printTab(5);
	if(attr.More.VarAttr.access==direct)
		fprintf(listing,"access->direct;\n");
	else
		fprintf(listing,"access->indirect;\n");

	printTab(5);
	fprintf(listing,"\n");

	if(entry->attrIR.idType!=NULL)
    {
		printTab(5);
		printTy(entry->attrIR.idType);
    }
}

/***********************************************************/
/* 函数名  printProc                                       */
/* 功  能  把过程内部表示在输出文件中显示出来              */
/* 说  明  显示过程名，层数                                */
/***********************************************************/
static void printProc(SymbolTable * entry)
{

	//SymbolTable * entry0 = NULL;
	//fprintf(listing,"name->%s;\n",entry->idName);
	cout<<"name->"<<entry->idname<<";\n";

	AttributeIR  attr = entry->attrIR;
	printTab(5);
	fprintf(listing,"level->%d;\n",attr.More.ProcAttr.level);
	//ParamTable * ptable = attr.More.ProcAttr.param;
	printTab(5);

	fprintf(listing,"noff->%d; \n",entry->attrIR.More.ProcAttr.nOff);
	printTab(5);
	fprintf(listing,"moff->%d; \n",entry->attrIR.More.ProcAttr.mOff);
}

static void printTab(unsigned int i)
{
    for(unsigned int _count=0;_count<i;_count++)
        cout<<"\t";
}
