#include <string>
#include <iostream>
using namespace std;

#include <stdlib.h>

#include "treeNode.h"
#include "symbolTable.h"
#include "operatorSymbolTable.h"


#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#define SCOPESIZE 1000
SymbolTable *scope[SCOPESIZE];

//extern unsigned int mainOff;
//extern unsigned int savedOff;
//extern unsigned int storeOff;

unsigned int mainOff=0;
unsigned int savedOff=0;
unsigned int storeOff=0;

extern unsigned int Off;
extern int Level;



TypeIR *intPtr=NULL;
TypeIR *charPtr=NULL;
TypeIR *boolPtr=NULL;



static TypeIR * newTy(TypeKind  kind);
static FieldChain * newBody(void);
static ParamTable * newParam(void);
//static AttributeIR findAttr(SymbolTable *entry);
static int compat(TypeIR *tp1,TypeIR *tp2);

static void initialize(void);
static TypeIR *nameType(TreeNode *t);
static TypeIR *arrayType(TreeNode *t);
static TypeIR *recordType(TreeNode *t);
static TypeIR *typeProcess(TreeNode *t,DecKind decKind);

static void typeDecPart(TreeNode *t);
static void varDecPart(TreeNode *t);
static void procDecPart(TreeNode *t);
static SymbolTable *headProcess(TreeNode *t);
static ParamTable *paraDecList(TreeNode *t);

static void body(TreeNode *t);
static void statement(TreeNode *t);
static TypeIR *expr(TreeNode *t,AccessKind *eKind);
static TypeIR *arrayVar(TreeNode *t);
static TypeIR *recordVar(TreeNode *t);
static void assignStatement(TreeNode *t);
static void callStatement(TreeNode *t);
static void ifStatement(TreeNode *t);
static void whileStatement(TreeNode *t);
static void readStatement(TreeNode *t);
static void writeStatement(TreeNode *t);
static void returnStatement(TreeNode *t);


static TypeIR * newTy(TypeKind  kind)
{
	//TypeIR * table = (TypeIR *) malloc(sizeof(TypeIR));
	TypeIR *table = new TypeIR();
    if (table==NULL)
	{
		//fprintf(listing,"Out of memory error !");
		//Error = TRUE;
	}
	/* 类型内部表示类型指针table不是NULL,内存单元已经成功分配 */

	else
		switch(kind)
		{
			case intTy:
				table->kind = intTy;
				table->typeSize = 1;
				break;
			case charTy:
				table->kind = charTy;
				table->typeSize = 1;
				break;
			case boolTy:
				table->kind = boolTy;
				table->typeSize = 1;
				break;
			case arrayTy:
				table->kind = arrayTy;
				table->More.ArrayAttr.indexTy = NULL;
				table->More.ArrayAttr.elemTy = NULL;
				break;
			case recordTy:
				table->kind = recordTy;
				table->More.body = NULL;
				break;
            default:
                return NULL;
		}
	return table;
}

static FieldChain * newBody()
{
	//FieldChain * ptr = (FieldChain *) malloc(sizeof(FieldChain));

    FieldChain *ptr=(FieldChain *)new FieldChain();
	if (ptr==NULL)
	{
		//fprintf(listing,"Out of memory error !");
		//Error = TRUE;
	}
	else
	{
	    ptr->idName="";
	    ptr->unitType=NULL;
	    ptr->offset=0;
	    ptr->next=NULL;
	}
	return ptr;
}

static ParamTable * newParam(void)
{
	ParamTable * ptr = (ParamTable *) malloc(sizeof(ParamTable));

    if (ptr==NULL)
	{
		//fprintf(listing,"Out of memory error !");
		//Error = TRUE;
	}
	else
	{
		ptr->entry = NULL;
		ptr->next = NULL;
	}

	return ptr;
}

/**static AttributeIR findAttr(SymbolTable *entry)
{
	AttributeIR attrIr = entry->attrIR;
	return attrIr;
}*/

static int compat(TypeIR *tp1,TypeIR *tp2)
{
	int  present;
	if (tp1!=tp2)
		present = FALSE;  /*类型不等*/
	else
		present = TRUE;   /*类型等价*/
	return present;
}

static void initialize(void)
{
    intPtr=newTy(intTy);
    charPtr=newTy(charTy);
    boolPtr=newTy(boolTy);

    for(int i=0;i<SCOPESIZE;i++)
        scope[i]=NULL;
}

static TypeIR *typeProcess(TreeNode *t,DecKind decKind)
{
    TypeIR *ptr=NULL;
    switch(decKind)
    {
        case IdK:
            ptr=nameType(t);break;
        case ArrayK:
            ptr=arrayType(t);break;
        case RecordK:
            ptr=recordType(t);break;
        case IntegerK:
            ptr=intPtr;break;
        case CharK:
            ptr=charPtr;break;
        case BoolK:
            ptr=boolPtr;break;
        default:
            return NULL;
    }
    return ptr;
}

static TypeIR *nameType(TreeNode *t)
{
    TypeIR *ptrNameType=NULL;
    SymbolTable *entry=NULL;
    int present=FALSE;

    present=findEntry(t->attr.type_name,&entry);

    if(present==TRUE)
    {
        if(entry->attrIR.kind!=typeKind);
        else ptrNameType=entry->attrIR.idType;
    }
    else
    {
        cout<<"error"<<endl;
    }
    return ptrNameType;

}

static TypeIR *arrayType(TreeNode *t)
{

    TypeIR *ptrArray=NULL;
    TypeIR *ptrIndex=NULL;
    TypeIR *ptrElement=NULL;
    if((t->attr.ArrayAttr.low)>(t->attr.ArrayAttr.up))
    {
        cout<<"下标越界"<<endl;
    }

    ptrIndex=typeProcess(t,IntegerK);
    ptrElement=typeProcess(t,t->attr.ArrayAttr.childType);
    ptrArray=newTy(arrayTy);

    ptrArray->typeSize=((t->attr.ArrayAttr.up)-(t->attr.ArrayAttr.low)+1)*sizeof(ptrElement->typeSize);
    ptrArray->More.ArrayAttr.indexTy=ptrIndex;
    ptrArray->More.ArrayAttr.elemTy=ptrElement;
    ptrArray->More.ArrayAttr.up=t->attr.ArrayAttr.up;
    ptrArray->More.ArrayAttr.low=t->attr.ArrayAttr.low;

    return ptrArray;
}

static TypeIR *recordType(TreeNode *t)
{
    TypeIR *ptrRecord=newTy(recordTy);

    t=t->child[0];
    FieldChain *ptrBody=NULL;
    FieldChain *ptrNow=NULL;
    FieldChain *ptrNext=NULL;
    while(t!=NULL)
    {
        for(unsigned int i=0;i<(t->idnum);i++)
        {
            ptrNow=newBody();

            if(ptrBody==NULL)
                ptrBody=ptrNext=ptrNow;

                ptrNow->idName=t->name[i];

                switch(t->kind.dec)
                {
                    case IntegerK:
                        ptrNow->typeK=intTy;ptrNow->unitType=intPtr;break;
                    case CharK:
                        ptrNow->typeK=charTy;ptrNow->unitType=charPtr;break;
                    case ArrayK:
                        ptrNow->typeK=arrayTy;ptrNow->unitType=arrayType(t);break;
                    case BoolK:
                        ptrNow->typeK=boolTy;ptrNow->unitType=boolPtr;break;
                    default:
                        break;
                }

                ptrNext=ptrNow->next;
                ptrNext->offset=(ptrNow->offset)+(ptrNow->unitType->typeSize);
                ptrNow=ptrNext;
        }
        t=t->sibling;
    }

    ptrRecord->typeSize=ptrNext->offset;
    ptrRecord->kind=recordTy;
    ptrRecord->More.body=ptrBody;

    return ptrRecord;
}

static void typeDecPart(TreeNode *t)
{
    int present=FALSE;
    AttributeIR attrIR;
    SymbolTable *entry=NULL;

    while(t!=NULL)
    {
        attrIR.idType=typeProcess(t,t->kind.dec);
        attrIR.kind=typeKind;
        present=enter(t->name[0],&attrIR,&entry);
        if(present==TRUE)
        {
        }
        else
        {
            t->table[0]= entry;
        }
        t=t->sibling;
    }
}

static void varDecPart(TreeNode *t)
{
	AttributeIR  attrIR;
	int present = FALSE;

	SymbolTable * entry = NULL;
	while(t!=NULL)
	{
		attrIR.kind = varKind;
		for(unsigned int i=0; i<(t->idnum) ; i++)
		{
			attrIR.idType = typeProcess(t,t->kind.dec);

			if(t->attr.ProcAttr.paramt == varparamType)
			{
                attrIR.More.VarAttr.access=indirect;
                attrIR.More.VarAttr.level = Level;
                attrIR.More.VarAttr.off = Off;
                Off = Off+1;
			}

			else if(t->attr.ProcAttr.paramt==valparamType)
			{
				attrIR.More.VarAttr.access=direct;
				attrIR.More.VarAttr.level = Level;
				if(attrIR.idType!=NULL)
				{
					attrIR.More.VarAttr.off = Off;
					Off = Off + (attrIR.idType->typeSize);
				}
			}

			present = enter(t->name[i],&attrIR,&entry);

			if(present==TRUE)
			{
				//ErrorPrompt(t->lineno,t->name[i]," is defined repetation!\n");
				cout<<t->name[i]<<" is defined repetation!\n"<<endl;
			}
			else
            {
   				t->table[i] = entry;
            }
		}
		if(t!=NULL)
			t = t->sibling;
	}

	/**如果是主程序，则记录此时偏移，用于目标代码生成时的displayOff*/
	if(Level==1)
	{
		mainOff = Off;
		/**存储主程序AR的display表的偏移到全局变量*/
		storeOff = Off;
	}

	/**如果不是主程序，则记录此时偏移，用于下面填写过程信息表的noff信息*/

	/**nOff sp到display表的偏移*/
	else
		savedOff = Off;
}

/**store the info of the procedure as the previous symboltable item*/
static void  procDecPart(TreeNode * t)
{
    /**pay attention to the position of t in the program tree*/
    TreeNode *p=t;
    SymbolTable * entry = headProcess(t);

    p = t->child[1];
    /**if there is declare part in the procedure ,we should handle the declare first*/
    while (p!=NULL)
    {
        switch ( p->nodekind )
        {
            case TypeK:
                typeDecPart(p->child[0]);
                break ;
            case VarK :
                varDecPart(p->child[0]);
                break ;
            case ProcDecK:
                break;
            default:
                /**ErrorPrompt(t->line no,"","no this node kind in syntax tree!");*/
                cout<<"no this node in syntax tree"<<endl;
                break;
        }
        if( p->nodekind == ProcDecK )
            break;
        else
            p=p->sibling ;
    }

    entry->attrIR.More.ProcAttr.nOff = savedOff;/**nOff指sp到display表的偏移*/
    entry->attrIR.More.ProcAttr.mOff = entry->attrIR.More.ProcAttr.nOff + entry->attrIR.More.ProcAttr.level+1;

    /**mOff指的是活动记录的大小*/
    /**过程活动记录的长度等于 nOff 加上 display 表的长度*/
    /**diplay表的长度等于过程所在层数加一*/

    p=p->child[0];
    /**处理程序的声明部分*/
    while(p!=NULL)
    {
        procDecPart(p);
        p=p->sibling;
    }

    body(t->child[2]);/**handle  block*/

    /**函数部分结束,删除进入形参时,新建立的符号表*/
    if ( Level!=-1 && t!=NULL)
        destroySymbolTable();/**结束当前scope*/
}

static SymbolTable *headProcess(TreeNode * t)
{
	AttributeIR attrIR ;
	int present = FALSE;
	SymbolTable  * entry = NULL;

	/**fill out the black attribute for the identifier*/
	attrIR.kind = routKind;
	attrIR.idType = NULL;
    attrIR.More.ProcAttr.level = Level;

	if(t!=NULL)
	{
		/**登记函数的符号表项*/
		present = enter(t->name[0],&attrIR,&entry);
		t->table[0] = entry;
	}

	/**处理形参声明表*/
	entry->attrIR.More.ProcAttr.param = paraDecList(t);
	return entry;
}

/**函数参数部分的符号信息登记在本层符号表中，与本层内的局部变量保存在一起*/
static ParamTable  * paraDecList(TreeNode * t)
{
	TreeNode * p = NULL;
	ParamTable  * Ptr1 = NULL;
	ParamTable  * Ptr2 = NULL;
	ParamTable  * head = NULL;

	if(t!=NULL)
	{
		if(t->child[0]!=NULL)
			p = t->child[0];   	/**程序声明节点的第一个儿子节点*/

		creatSymbolTable();			/**进入新的局部化区*/

		//Off = 7;                /**子程序中的变量初始偏移设为7*/

		varDecPart(p);			/**变量声明部分*/

		SymbolTable *Ptr0 = scope[Level];

		while(Ptr0 != NULL)         /**只要不为空，就访问其兄弟节点*/
		{
			/**构造形参符号表，并使其连接至符号表的param项*/
			Ptr2 = newParam();
			if(head == NULL)
				head = Ptr1 = Ptr2;
			Ptr0->attrIR.More.VarAttr.isParam = true;
			Ptr0->attrIR.More.VarAttr.access=direct;
			Ptr2->entry = Ptr0;
			Ptr2->next = NULL;
			if(Ptr2 != Ptr1)
			{
				Ptr1->next = Ptr2;
				Ptr1 = Ptr2;
			}
			Ptr0 = Ptr0->next;
		}
	}
	return head;/*返回形参符号表的头指针*/
}

/**函数体变量使用检查，看看是否有未声明就使用，或者用法不当的...一系列错误*/
static void body(TreeNode *t)
{
    if(t==NULL)
    {

    }
    else if(t->nodekind == StmLK)
    {
        TreeNode* p = t->child[0];
        while(p!=NULL)
        {
            statement(p);
            p=p->sibling;
        }
    }
}

static void statement(TreeNode *t)
{
    if(t!=NULL)
    {
        switch(t->kind.stmt)
        {
            case IfK:ifStatement(t);break;
            case WhileK:whileStatement(t);break;
            case AssignK:assignStatement(t);break;
            case ReadK:readStatement(t);break;
            case WriteK:writeStatement(t);break;
            case CallK:callStatement(t);break;
            case ReturnK:returnStatement(t);break;
            default:
                cout<<"it is not a statement.."<<endl;
                break;
        }
    }
}

/************************************************************/
/* 函数名  expr                                             */
/* 功  能  该函数处理表达式的分析                           */
/* 说  明  表达式语义分析的重点是检查运算分量的类型相容性， */
/*         求表达式的类型。其中参数Ekind用来表示实参是变参  */
/*         还是值参。    	                                */
/************************************************************/
static TypeIR * expr(TreeNode * t,AccessKind  *eKind)
{
	int present = FALSE;
	SymbolTable * entry = NULL;

	TypeIR * eptr0 = NULL;
	TypeIR * eptr1 = NULL;
	TypeIR * eptr = NULL;
	if(t!=NULL)
    switch(t->kind.exp)
	{
		case ConstK:
			eptr = typeProcess (t,IntegerK);
		    eptr->kind = intTy;
			if(eKind!=NULL)
				(*eKind ) = direct;   /*直接变量*/
			break;
		case VariK:
		    /*Var = id的情形*/
			if(t->child[0] == NULL)
			{
				/*在符号表中查找此标识符*/
				present = findEntry(t->name[0],&entry);
				t->table[0] = entry;

				if(present==TRUE)
				{   /*id不是变量*/
					if (entry->attrIR.kind != varKind)
					{
					 //ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
					 cout<<t->name[0]<<"is not variable error!"<<endl;
					 eptr = NULL;
					}
					else
					{
						eptr = entry->attrIR.idType;
						if (eKind != NULL)
							(*eKind) = indirect;  /*间接变量*/

					}
				}
				else /*标识符无声明*/
				{
					//ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
				}

			}
		    else if(t->attr.ExpAttr.varkind == ArrayMembV)
					eptr = arrayVar(t);/*Var = Var0[E]的情形*/
            else if(t->attr.ExpAttr.varkind == FieldMembV)
						eptr = recordVar(t);/*Var = Var0.id的情形*/

			break;

		case OpK:
			/*递归调用儿子节点*/
			eptr0 = expr(t->child[0],NULL);
			if(eptr0==NULL)
				return NULL;
			eptr1 = expr(t->child[1],NULL);
			if(eptr1==NULL)
				return NULL;

			/*类型判别*/
			present = compat(eptr0,eptr1);
			if ( present ==TRUE )
			{
				switch(t->attr.ExpAttr.op)
				{
					case '<':
					case '=':
						eptr = boolPtr;
						break;  /*条件表达式*/
					case '+':
					case '-':
					case '*':
					case '/':
						eptr = intPtr;
						break;  /*算数表达式*/
				}
				if(eKind != NULL)
                {
                    (*eKind) = direct; /*直接变量*/
                }
			}
			else
            {
				//ErrorPrompt(t->lineno,"","operator is not compat!\n");
            }
			break;
	}
	else
    {

    }
	return eptr;
}


/************************************************************/
/* 函数名  arrayVar                                         */
/* 功  能  该函数处理数组变量的下标分析                     */
/* 说  明  检查var := var0[E]中var0是不是数组类型变量，E是不*/
/*         是和数组的下标变量类型匹配。                     */
/************************************************************/
static TypeIR *arrayVar(TreeNode *t)
{
    int present = FALSE;
	SymbolTable * entry = NULL;

	TypeIR * eptr0 = NULL;
	TypeIR * eptr1 = NULL;

	TypeIR * eptr = NULL;


	/*在符号表中查找此标识符*/
	present = findEntry(t->name[0],&entry);
	t->table[0] = entry;

	/*找到*/
	if(present==TRUE)
	{
		/*Var0不是变量*/
		if (entry->attrIR.kind!=varKind)
		{
			//ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
			cout<<t->name[0]<<"不是变量类型"<<endl;
			eptr = NULL;
		}
		else if(entry->attrIR.idType!=NULL)/*Var0不是数组类型变量*/
		{
             if(entry->attrIR.idType->kind!=arrayTy)
				{
					//ErrorPrompt(t->lineno,t->name[0],"is not array variable error !\n");
					cout<<t->name[0]<<" is not array variable error !"<<endl;
					eptr = NULL;
				}
				else
				{
					/*检查E的类型是否与下标类型相符*/
					eptr0 = entry->attrIR.idType->More.ArrayAttr.indexTy;
					if(eptr0==NULL)
                    {
                        return NULL;
                    }

					eptr1 = expr(t->child[0],NULL);//intPtr;
					if(eptr1==NULL)
                    {
                        return NULL;
                    }

					present = compat(eptr0,eptr1);
					if(present!=TRUE)
					{
						//ErrorPrompt(t->lineno,"","type is not matched with the array member error !\n");
						cout<<t->name[0]<<" type is not matched with the array member error !"<<endl;
						eptr = NULL;
					}
					else
                    {
                        eptr = entry->attrIR.idType->More.ArrayAttr.elemTy;
                    }
				}
		}
	}
	else/*标识符无声明*/
    {
        cout<<t->name[0]<<"未声明就使用"<<endl;
		//ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
    }
	return eptr;
}

static TypeIR *recordVar(TreeNode *t)
{
    int present = FALSE;
    int result = TRUE;
    FieldChain *currentP = NULL;

	SymbolTable * entry = NULL;

	TypeIR * eptr0 = NULL;
	//TypeIR * eptr1 = NULL;
	TypeIR * eptr = NULL;


	/**在符号表中查找此标识符*/

	present = findEntry(t->name[0],&entry);
	t->table[0] = entry;

	/**找到*/
	if(present==TRUE)
	{
		/*Var0不是变量*/
		if (entry->attrIR.kind!=varKind)
		{
			//ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
			cout<<t->name[0]<<"不是变量类型"<<endl;
			eptr = NULL;
		}
		else if(entry->attrIR.idType!=NULL)/*Var0不是数组类型变量*/
		{
		    if(entry->attrIR.idType->kind!=recordTy)
				{
					//ErrorPrompt(t->lineno,t->name[0],"is not array variable error !\n");
					cout<<t->name[0]<<" is not array variable error !"<<endl;
					eptr = NULL;
				}
				else
				{
				    eptr0 = entry->attrIR.idType;
				    currentP = eptr0->More.body;
				    while((currentP!=NULL)&&(result!=FALSE))
                        {
                            //result = strcmp(t->child[0]->name[0],currentP->id);
                            result = ( operator==(t->child[0]->name[0],currentP->idName) );

                            /** if equal*/
                            if( result== 0)
                                eptr = currentP->unitType;
                            else
                                currentP = currentP->next;
                        }
                    if(currentP==NULL)
                        {
                                if(result!=FALSE)
                                {
                                    //ErrorPrompt(t->child[0]->lineno,t->child[0]->name[0],"is not field type!\n");
                                    eptr = NULL;
					            }
					            else if(t->child[0]->child[0]!=NULL)/*如果id是数组变量*/
                                    {
                                        eptr = arrayVar(t->child[0]);
                                    }
                        }
				}
		}
	}
	else/*标识符无声明*/
    {
        cout<<t->name[0]<<"未声明就使用"<<endl;
		//ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
    }
	return eptr;
}


static void ifStatement(TreeNode *t)
{
    AccessKind *eKind = NULL;
    TypeIR * eptr = expr(t->child[0],eKind);
    if(eptr != NULL)
    {
        if(eptr->kind!=boolTy)
        {
          cout<<t->lineno<<"condition expressrion error!"<<endl;
        }
        else
        {
            TreeNode *p=t->child[1];
            while( p!=NULL )
            {
                statement(p);
                p=p->sibling;
            }

            t=t->child[2];
            while(t!=NULL)
            {
                statement(t);
                t=t->sibling;
            }
        }
    }
}

static void whileStatement(TreeNode *t)
{
    TypeIR *epoint = expr(t->child[0],NULL);

    if(epoint!=NULL)
    {
        if(epoint->kind != boolTy)
        {
            cout<<t->lineno<<"condition expression is error"<<endl;
        }
        else
        {
            TreeNode *p=t->child[1];

            while(p!=NULL)
            {
                statement(p);
                p=p->sibling;
            }
        }

    }
}

static void assignStatement(TreeNode *t)
{
    SymbolTable *entry=NULL;

    int present = FALSE;
    TypeIR *ptr = NULL;
    TypeIR *eptr = NULL;

    TreeNode *child1 = NULL;
    TreeNode *child2 = NULL;

    child1 = t->child[0];
    child2 = t->child[1];

    if(child1->child[0]==NULL)
    {
        present = findEntry(child1->name[0],&entry);
        if(present ==TRUE)
        {
            if(entry->attrIR.kind!=varKind)
            {
                cout<<child1->lineno<<"赋值语句左表达式不是变量"<<endl;
                eptr = NULL;
            }
            else
            {
                eptr = entry->attrIR.idType;
                child1->table[0] = entry;
            }
        }
        else
            cout<<child1->name[0]<<"变量未声明便使用"<<endl;
    }
    else
    {
        if(child1->attr.ExpAttr.varkind==ArrayMembV)
            eptr=arrayVar(child1);
        else
        {
            if(child1->attr.ExpAttr.varkind==FieldMembV)
                eptr=recordVar(child1);
        }
    }
    if(eptr != NULL)
    {
        if((t->nodekind==StmtK)&&(t->kind.stmt==AssignK))
        {
            ptr = expr(child2,NULL);
            if(compat(ptr,eptr))
            {

            }
            else
            {
                cout<<"类型不匹配"<<endl;
            }
        }
    }
}

static void readStatement(TreeNode *t)
{
    SymbolTable *entry = NULL;
    int present = FALSE;
    present = findEntry(t->name[0],&entry);

    t->table[0] = entry;
    if(present==FALSE)
    {
        cout<<t->name[0]<<"变量未声明就使用"<<endl;
    }
    else if(entry->attrIR.kind!=varKind)
    {
        cout<<"read语句参数不是变量，出错"<<endl;
    }

}

static void writeStatement(TreeNode *t)
{
    TypeIR *point = expr(t->child[0],NULL);

    if(point==NULL)
    {

    }
    else
    {
        if(point->kind==boolTy)
        {
            cout<<"write语句的类型是bool类型，报错"<<endl;
        }
        else
        {

        }
    }
}

static void callStatement(TreeNode *t)
{
    AccessKind eKind;
    int present=FALSE;
    SymbolTable *entry=NULL;
    TreeNode *p=NULL;

    present = findEntry(t->child[0]->name[0],&entry);
    t->child[0]->table[0] = entry;
    if(present==FALSE)
    {
        cout<<t->child[0]->name[0]<<"函数名未声明便使用"<<endl;
    }
    else
    {
        if(entry->attrIR.kind!=routKind)
            cout<<"标识符不是函数名"<<endl;
        else
        {
            p=t->child[1];

            ParamTable *paramP=entry->attrIR.More.ProcAttr.param;
            while((p!=NULL)&&(paramP!=NULL))
            {
                SymbolTable *paraEntry=paramP->entry;
                TypeIR *expPtr = expr(p,&eKind);

                if( paraEntry->attrIR.More.VarAttr.access!=eKind )
                {
                    cout<<"实参形参不匹配(一个是值参一个是变参)"<<endl;
                }
                else if( paraEntry->attrIR.idType != expPtr )
                {
                    cout<<"实参形参类型不匹配,一个需要整型,另外一个却给数组..."<<endl;
                }

                p=p->sibling;
                paramP=paramP->next;
            }
        }
    }
}

static void returnStatement(TreeNode *t)
{
    if(Level==0)
    {
        cout<<"返回语句不应该出现在主程序中"<<endl;
    }
}

void analysis(TreeNode *t)
{
    TreeNode *p = NULL;

    creatSymbolTable();
    initialize();

    SymbolTable *entry = headProcess(t->child[0]);


    entry->attrIR.More.ProcAttr.nOff = mainOff;/**nOff指sp到display表的偏移*/
    entry->attrIR.More.ProcAttr.mOff = entry->attrIR.More.ProcAttr.nOff + entry->attrIR.More.ProcAttr.level+1;
    /**moff is the size of AR*/
    p = t->child[1];

    while(p!=NULL)
    {
        switch(p->nodekind)
        {
        case TypeK:
            typeDecPart(p->child[0]);
            break;
        case VarK:
            varDecPart(p->child[0]);
            break;
        case ProcDecK:
            break;
        default:
            break;
        }
        if(p->nodekind==ProcDecK)
            break;
        else
            p=p->sibling;
    }

    if(p->nodekind==ProcDecK)
    {
        p=p->child[0];
        while(p!=NULL)
        {
            procDecPart(p);
            p=p->sibling;
        }
    }

    t->child[0]->table[0]->attrIR.More.ProcAttr.nOff=mainOff;
    t = t->child[2];
    if(t->nodekind ==StmLK)
        body(t);

    if(Level!=-1)
        destroySymbolTable();
}
