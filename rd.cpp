#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "rd.h"
#include "token.h"
#include "treeNode.h"
#include "token.h"
#include "tree.h"
#include "getNewTreeNode.h"

//List tokenList=GetTokenList();
listNode* CurrentListNode=NULL;
Tree Gtree;
std::string temp_name="";

static List GetTokenList();
static TreeNode * program(void);
static TreeNode * programHead(void);
static TreeNode * declarePart(void);
static TreeNode * typeDec(void);
static TreeNode * typeDeclaration(void);
static TreeNode * typeDecList(void);
static TreeNode * typeDecMore(void);
static void  typeId(TreeNode * t);
static void  typeDef(TreeNode * t);
static void  baseType(TreeNode * t);
static void  structureType(TreeNode * t);
static void  arrayType(TreeNode * t);
static void  recType(TreeNode * t);
static TreeNode * fieldDecList(void);
static TreeNode * fieldDecMore(void);
static void  idList(TreeNode * t);
static void  idMore(TreeNode * t);
static TreeNode * varDec(void);
static TreeNode * varDeclaration(void);
static TreeNode * varDecList(void);
static TreeNode * varDecMore(void);
static void  varIdList(TreeNode * t);
static void  varIdMore(TreeNode * t);
static TreeNode * procDec(void);
static TreeNode * procDeclaration(void);
static void  paramList(TreeNode * t);
static TreeNode * paramDecList(void);
static TreeNode * param(void);
static TreeNode * paramMore(void);
static void  formList(TreeNode * t);
static void  fidMore(TreeNode * t);
static TreeNode * procDecPart(void);
static TreeNode * procBody(void);
static TreeNode * programBody(void);
static TreeNode * stmList(void);
static TreeNode * stmMore(void);
static TreeNode * stm(void);
static TreeNode * assCall(void);
static TreeNode * assignmentRest(void);
static TreeNode * conditionalStm(void);
static TreeNode * loopStm(void);
static TreeNode * inputStm(void);
static TreeNode * outputStm(void);
static TreeNode * returnStm(void);
static TreeNode * callStmRest(void);
static TreeNode * actParamList(void);
static TreeNode * actParamMore(void);

static TreeNode * relExp(void);
static TreeNode * otherRelE(TreeNode * t);
static TreeNode * exp(void);
//static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * otherTerm(TreeNode*);
static TreeNode * factor(void);
static TreeNode * otherFactor(TreeNode*);
static TreeNode * variable(void);
static void variMore(TreeNode * t);
static TreeNode * fieldVar(void);
static void fieldVarMore(TreeNode * t );
static void match(const std::string );


static void error()
{
    cout<<CurrentListNode->data.linenum<<" lines is error"<<endl;
}

static int stringToInt(std::string str)
{
    int sum=0;
    for(unsigned int i=0;i<str.length();i++)
        sum=sum*10+(str[i]-'0');
    return sum;
}

/**将token序列文件输入链表*/
List GetTokenList()
{
	List templist;
	cout<<"please input  *next.txt file:"<<endl;
	char  name[30];
	cin>>name;
	ifstream infile;
	infile.open(name,ios::in);
	token token;
	while(1)
	{
		if(infile.eof())
			break;
		else
			infile>>token.linenum;
		if(infile.eof())
			break;
		else
			infile>>token.Ttype;
		if(infile.eof())
			break;
		else
			infile>>token.value;
		templist.inserttail(token);
	}
	infile.close();
	return templist;
}

static void match(std::string str)
{
    if(CurrentListNode!=nullptr)
    {
        if(operator==(str,CurrentListNode->data.Ttype))
        {
            CurrentListNode=CurrentListNode->next;
        }
        else
        {
            cout<<"there is excepted:"<<str<<endl;
            CurrentListNode=CurrentListNode->next;
        }
    }
    else
    {
        cout<<"parse is over"<<endl;
    }
}

void parse(void)
{
    CurrentListNode=GetTokenList().head;
    Gtree.root = program();
    if(CurrentListNode!=nullptr)
    {
        cout<<"Code ends before file"<<endl;
        cout<<CurrentListNode->data.Ttype<<endl;
        cout<<CurrentListNode->data.value<<endl;
        cout<<CurrentListNode->data.linenum<<endl;
    }


}

static TreeNode* program(void)
{
    TreeNode* t=programHead();
    TreeNode* q=declarePart();
    TreeNode* s=programBody();

    TreeNode* root = newRootNode();
    if(root!=nullptr)
    {
        if(t!=nullptr)
            root->child[0]=t;
        else
            error();

        if(q!=nullptr)
            root->child[1] = q;
        else
            error();

        if(s!=nullptr)
            root->child[2] = s;
        else
            error();
    }
    match(".");

    return root;
}

static TreeNode * programHead(void)
{
    TreeNode* t = newPheadNode();
    match("program");

    if(CurrentListNode==nullptr);
    else if(CurrentListNode->data.Ttype=="id")
    {
        if(t!=NULL)
        {
            t->nodekind=PheadK;
            t->name[t->idnum]=CurrentListNode->data.value;
            (t->idnum)++;
        }
        match("id");
    }
    return t;
}

static TreeNode * declarePart(void)
{
    TreeNode *typeP=newDecANode(TypeK);
    TreeNode *pp=typeP;
    if(typeP != nullptr)
    {
        typeP->child[0]=typeDec();
    }


    TreeNode *varP=newDecANode(VarK);
    if(varP!=nullptr)
    {
        varP->child[0]=varDec();
    }


    TreeNode *procP=newProcNode();
    if(procP)
    {
        procP->child[0]=procDec();
    }


    if(varP==nullptr)
    {
        varP=procP;
    }
    if(typeP==nullptr)
    {
        pp=typeP=varP;
    }
    if(varP!=typeP)
    {
        typeP->sibling=varP;
    }
    if(varP!=procP)
    {
        varP->sibling=procP;
    }
    return pp;
}

static TreeNode * typeDec(void)
{
    TreeNode *t = NULL;

    if(CurrentListNode->data.Ttype=="var"||CurrentListNode->data.Ttype=="procedure"
       ||CurrentListNode->data.Ttype=="begin")
    {

    }
    else if(CurrentListNode->data.Ttype=="type")
    {
        t=typeDeclaration();
    }
    else
    {

    }

    return t;
}

static TreeNode * typeDeclaration(void)
{
    match("type");

    TreeNode *t=typeDecList();

    return t;
}

static TreeNode * typeDecList(void)
{
    TreeNode *t=newDecNode();
    TreeNode *p=NULL;

    if(t!=nullptr)
    {
        typeId(t);
        match("=");
        typeDef(t);
        match(";");
        p=typeDecMore();
        if(p!=nullptr)
        {
            t->sibling=p;
        }
    }
    return t;
}

static TreeNode * typeDecMore(void)
{
    TreeNode *p = NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"var")||
            operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"begin"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        p=typeDecList();
    }
    return p;
}

static void  typeId(TreeNode * t)
{
    if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        if(t!=nullptr)
        {
            //t->lineno=CurrentListNode->data.linenum;
            t->name[t->idnum]=CurrentListNode->data.value;
            (t->idnum)++;
            match("id");
        }
    }
}

static void  typeDef(TreeNode * t)/**就是参考书里面的typeName()函数*/
{

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
       operator==(CurrentListNode->data.Ttype,"char"))
       {
           baseType(t);
       }
    else if(operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record"))
    {
        structureType(t);
    }
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {                           /**遇到id,名字信息要保存起来*/
        if(t!=nullptr)
        {
        t->kind.dec=IdK;
        t->attr.type_name=CurrentListNode->data.value;
        }
        match("id");
    }
}

static void  baseType(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer"))
    {
        if(t!=nullptr)
        t->kind.dec=IntegerK;

        match("integer");
    }
    else if(operator==(CurrentListNode->data.Ttype,"char"))
    {
        if(t!=nullptr)
        t->kind.dec=CharK;

        match("char");
    }
    else
    {

    }
}

static void  structureType(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"array"))
    {
        arrayType(t);
    }
    else if(operator==(CurrentListNode->data.Ttype,"record"))
    {
        if(t!=nullptr)
        t->kind.dec=RecordK;

        recType(t);/**里面可能需要创造新节点*/
    }
    else
    {

    }
}

static void  arrayType(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"array"))
    {
        match("array");
        match("[");
        if(operator==(CurrentListNode->data.Ttype,"intc"))
        {               /**这个终结符是有用信息,要保留*/
            t->attr.ArrayAttr.low=stringToInt(CurrentListNode->data.value);
            match("intc");
        }
        match("..");
        if(operator==(CurrentListNode->data.Ttype,"intc"))
        {               /**这个终结符是有用信息,要保留*/
            t->attr.ArrayAttr.up=stringToInt(CurrentListNode->data.value);
            match("intc");
        }
        match("]");
        match("of");
        baseType(t);

        /**childType中保留数组元素的类型*/
        t->attr.ArrayAttr.childType=t->kind.dec;

        /**记录该节点是数组类型*/
        t->kind.dec=ArrayK;
    }
}

static void  recType(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"record"))
    {
        match("record"); /**保留字信息无用，不保留*/
        TreeNode *p=fieldDecList();/**这里需要造新的树节点*/
        if(p)
        {
            t->child[0]=p;
        }
    }
    match("end");
}

static TreeNode * fieldDecList(void)
{
    TreeNode *t=newDecNode();
    TreeNode *p=NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char"))
    {
        baseType(t);
        idList(t);
        match(";");   /**这个终结符没用，不保留在节点中*/
        p=fieldDecMore();

        if(p!=NULL)
            t->sibling=p;
    }
    else if(operator==(CurrentListNode->data.Ttype,"array"))
    {
        arrayType(t);
        idList(t);
        p=fieldDecMore();

        if(p!=NULL)
            t->sibling=p;
    }
    return t;
}

static TreeNode * fieldDecMore(void)
{
    TreeNode *t=NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"end"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"array"))
    {
        t=fieldDecList();
    }
    return t;
}

static void  idList(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {      /**变量的名字保存起来*/
        t->name[t->idnum]=CurrentListNode->data.value;
        (t->idnum)++;
        match("id");
        idMore(t);
    }
}

static void  idMore(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,";"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,","))
    {
        idList(t);
    }
}

static TreeNode * varDec(void)
{
    TreeNode *t=NULL;   /**这是造新节点的起始位置*/
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"procedure")||
            operator==(CurrentListNode->data.Ttype,"begin"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"var"))
    {
        t=varDeclaration();
    }
    return t;
}

static TreeNode * varDeclaration(void)
{
    TreeNode *t = NULL; /**接着varDec(void)继续造该节点*/
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"var"))
    {
        match("var");
        t=varDecList();
    }
    else
    {

    }
    return t;
}

static TreeNode * varDecList(void)
{
    TreeNode *t = newDecNode();/**这个函数是实质性的创造新树节点的函数*/
    TreeNode *p = NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        /**就是参考书中的typeName(TreeNode*)函数*/
        /**这俩个函数都在继续完善这个新的树节点的信息*/
        /**最终的结果是var integer a,b,c;这句话中*/

        /**var独自一个节点,且这个节点不是在这里造的**/
        /**integer a,b,c;一个节点,在这个函数里造*/

        typeDef(t);
        varIdList(t);
        match(";");
        p=varDecMore();

        if(p!=NULL)
            t->sibling=p;
    }
    else
    {

    }
    return t;
}

static TreeNode * varDecMore(void)
{
    TreeNode *t = NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"procedure")||
            operator==(CurrentListNode->data.Ttype,"begin"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        t=varDecList();
    }
    return t;

}

static void  varIdList(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        t->name[t->idnum]=CurrentListNode->data.value;
        (t->idnum)++;
        match("id");
        varIdMore(t);
    }
    else
    {

    }
}

static void  varIdMore(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,","))
    {
        match(","); /** ","无用不保留 */
        varIdList(t);
    }
    else if(operator==(CurrentListNode->data.Ttype,";"))
    {

    }
    else
    {

    }

}

static TreeNode * procDec(void)
{
    TreeNode *t = NULL;    /**打算造函数声明节点*/
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"begin"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"procedure"))
    {
        t=procDeclaration();
    }
    else
    {

    }

    return t;
}

static TreeNode * procDeclaration(void)
{
    TreeNode *t=newProcNode();  /**实质性的开始造函数声明节点*/

    TreeNode *procDecP = NULL;
    TreeNode *procB = NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"procedure"))
    {
        match("procedure");
        if(operator==(CurrentListNode->data.Ttype,"id"))
        {
            /**函数的名字很重要的信息，要保留在节点中*/
            t->name[0]=CurrentListNode->data.value;
        }
        match("id");
        match("(");
        paramList(t);
        match(")");
        match(";");

        //procDecP=procDecPart();
        procDecP=declarePart();
        if(procDecP!=nullptr)
            t->child[1]=procDecP;

        procB=procBody();
        if(procB!=nullptr)
            t->child[2]=procB;

        t->sibling=procDeclaration();

    }
    else
    {
        t=NULL;
    }
    return t;
}

static void  paramList(TreeNode * t)
{
    TreeNode *p = NULL;/**打算为过程参数表造节点*/

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        p=paramDecList();
        if( p!=nullptr )
            t->child[0]=p;
    }
    else if(operator==(CurrentListNode->data.Ttype,")"))
    {
        t->child[0]=NULL;
    }
}

static TreeNode * paramDecList(void)
{
    TreeNode *t = NULL;/**继续打算为过程参数表造节点*/
    TreeNode *p = NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record")||
            operator==(CurrentListNode->data.Ttype,"id")||
            operator==(CurrentListNode->data.Ttype,"var"))
    {
        t=param();
        p=paramMore();

        if(p!=nullptr)
            t->sibling=p;
    }
    else
    {

    }
    return t;
}

static TreeNode * param(void)
{
    TreeNode *t = newDecNode();/**实质性的为参数表构造新节点*/

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"integer")||
            operator==(CurrentListNode->data.Ttype,"char")||
            operator==(CurrentListNode->data.Ttype,"array")||
            operator==(CurrentListNode->data.Ttype,"record"))
    {
        /**记录参数是值参类型*/
        t->attr.ProcAttr.paramt=valparamType;
        typeDef(t);
        formList(t);
    }
    else if(operator==(CurrentListNode->data.Ttype,"var"))
    {
        /**记录参数是变参类型*/
        match("var");
        t->attr.ProcAttr.paramt=varparamType;
        typeDef(t);
        formList(t);

    }
    else
    {

    }
    return t;
}

static TreeNode * paramMore(void)
{
    TreeNode *t = NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,";"))
    {
        match(";");
        t=paramDecList();
    }
    else if(operator==(CurrentListNode->data.Ttype,")"))
    {

    }
    else
    {

    }
    return t;
}

static void  formList(TreeNode *t)   /**formList(TreeNode*)与*/
{                                    /**varIdList(TreeNode*)一样*/
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        t->name[t->idnum]=CurrentListNode->data.value;
        (t->idnum)++;
        match("id");
        fidMore(t);
    }
    else
    {

    }
}

static void  fidMore(TreeNode *t)/**varIdMore(TreeNode*)*/
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,";")||
            operator==(CurrentListNode->data.Ttype,")"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,","))
    {
        match(",");
        formList(t);
    }
}

static TreeNode * procDecPart(void)
{
    TreeNode *t = NULL;
    t=declarePart();
    return t;
}

static TreeNode * procBody(void)
{
    TreeNode *t = NULL; /**打算为函数体造新节点*/
    t=programBody();
    if(t==nullptr)
    {

    }
    return t;
}

static TreeNode * programBody(void)
{
    TreeNode *t=newStmlNode();/**实质性的工作*/

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"begin"))
    {
        match("begin");
        t->child[0]=stmList();
        match("end");
    }

    return t;
}

static TreeNode * stmList(void)
{
    TreeNode *t = NULL;
    TreeNode *p = NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"if")||
            operator==(CurrentListNode->data.Ttype,"while")||
            operator==(CurrentListNode->data.Ttype,"read")||
            operator==(CurrentListNode->data.Ttype,"write")||
            operator==(CurrentListNode->data.Ttype,"return")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        t=stm();
        p=stmMore();

        if(p!=nullptr)
            t->sibling=p;
    }
    else
    {

    }
    return t;
}

static TreeNode * stmMore(void)
{
    TreeNode *t = NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,";"))
    {
        match(";");
        t=stmList();
    }
    else if(operator==(CurrentListNode->data.Ttype,"end"))
    {

    }
    return t;
}

static TreeNode * stm(void)
{
    TreeNode *t = NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"if"))
    {
        t=conditionalStm();
    }
    else if(operator==(CurrentListNode->data.Ttype,"while"))
    {
        t=loopStm();
    }
    else if(operator==(CurrentListNode->data.Ttype,"read"))
    {
        t=inputStm();
    }
    else if(operator==(CurrentListNode->data.Ttype,"write"))
    {
        t=outputStm();
    }
    else if(operator==(CurrentListNode->data.Ttype,"return"))
    {
        t=returnStm();
    }
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        temp_name=CurrentListNode->data.value;
        match("id");
        t=assCall();
    }
    else
    {

    }
    return t;
}

static TreeNode * assCall(void)
{
    TreeNode *t=NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"[")||
            operator==(CurrentListNode->data.Ttype,":=")||
            operator==(CurrentListNode->data.Ttype,"."))
    {
        t=assignmentRest();
    }
    else if(operator==(CurrentListNode->data.Ttype,"("))
    {
        t=callStmRest();
    }
    else
    {

    }
    return t;
}

static TreeNode * assignmentRest(void)
{
    //a[0]:=1+2*3;a.b:=2;a:=3;

    TreeNode *t=newStmtNode(AssignK);

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"[")||
            operator==(CurrentListNode->data.Ttype,":=")||
            operator==(CurrentListNode->data.Ttype,"."))
    {
        TreeNode *p=newExpNode(VariK);
        if(p!=nullptr)
        {
            p->name[p->idnum]=temp_name;
            (p->idnum)++;
            variMore(p);
            t->child[0]=p;
        }

        match(":=");
        t->child[1]=exp();
    }

    return t;
}

static TreeNode * conditionalStm(void)
{
    TreeNode *t=newStmtNode(IfK);

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"if"))
    {
        match("if");
        if(t!=NULL)
            t->child[0]=relExp();
        match("then");
        if(t!=NULL)
            t->child[1]=stmList();
        match("else");
        if(t!=NULL)
            t->child[2]=stmList();
        match("fi");
    }
    else
    {

    }
    return t;
}

static TreeNode * loopStm(void)
{
    TreeNode *t=newStmtNode(WhileK);

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"while"))
    {
        match("while");

        if(t!=NULL)
            t->child[0]=relExp();

        match("do");

        if(t!=NULL)
            t->child[1]=stmList();

        match("endwh");
    }
    else
    {

    }
    return t;
}

static TreeNode * inputStm(void)
{
    TreeNode *t=newStmtNode(ReadK);
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"read"))
    {
        match("read");
        match("(");

        if(t!=NULL)
        {
            t->name[t->idnum]=CurrentListNode->data.value;
            (t->idnum)++;
        }
        match("id");
        match(")");
    }
    return t;
}

static TreeNode * outputStm(void)
{
    TreeNode *t=newStmtNode(WriteK);
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"write"))
    {
        match("write");
        match("(");
        if(t!=NULL)
            t->child[0]=exp();
        match(")");
    }
    return t;
}

static TreeNode * returnStm(void)
{
    TreeNode *t=newStmtNode(ReturnK);
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"return"))
    {
        match("return");
        if(t!=NULL)
        {
        }

    }
    else
    {

    }
    return t;
}

static TreeNode * callStmRest(void)
{
    TreeNode *t=newStmtNode(CallK);

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"("))
    {
        match("(");
        TreeNode *p=newExpNode(VariK);
        if(p!=NULL)
        {
            p->name[p->idnum]=temp_name;
            (p->idnum)++;
            t->child[0]=p;
        }

        t->child[1]=actParamList();

        match(")");
    }
    else
    {

    }
    return t;
}

static TreeNode * actParamList(void)
{
    TreeNode *t=NULL;
    TreeNode *p=NULL;

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,")"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"id")||
            operator==(CurrentListNode->data.Ttype,"intc"))
    {
        t=exp();
        p=actParamMore();
        if(p!=NULL)
            t->sibling=p;
    }
    return t;
}

static TreeNode * actParamMore(void)
{
    TreeNode *t=NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,")"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,","))
    {
        match(",");
        t=actParamList();
    }
    else
    {

    }
    return t;
}



static TreeNode *relExp(void)
{
    TreeNode *t=NULL;
    if(CurrentListNode==NULL);
    else if(operator==(CurrentListNode->data.Ttype,"(")||
            operator==(CurrentListNode->data.Ttype,"intc")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        t=exp();
        TreeNode * p=otherRelE(t);
        return p;
    }
    else
    {

    }
}

static TreeNode * otherRelE(TreeNode *t)
{
    if(CurrentListNode==NULL);
    else if(operator==(CurrentListNode->data.Ttype,"<")||
            operator==(CurrentListNode->data.Ttype,"="))
    {
        TreeNode *p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.ExpAttr.op=(char)CurrentListNode->data.Ttype[0];
        }

        match(CurrentListNode->data.Ttype);

        TreeNode *q=exp();

        if(p!=NULL)
            p->child[1]=q;

        return p;
    }
    else
    {

    }
}

static TreeNode * exp(void)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"(")||
            operator==(CurrentListNode->data.Ttype,"intc")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        TreeNode *t=term();
        TreeNode *p=otherTerm(t);

        if(p==NULL)
            return t;
        else
            return p;
    }

    else
    {
    }
}

static TreeNode* otherTerm(TreeNode *t)
{
    if(CurrentListNode==NULL);
    else if(operator==(CurrentListNode->data.Ttype,"+")||
            operator==(CurrentListNode->data.Ttype,"-"))
    {
        TreeNode *p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.ExpAttr.op=(char)CurrentListNode->data.Ttype[0];
        }
        match(CurrentListNode->data.Ttype);
        TreeNode *q=exp();
        if(p!=NULL)
        {
            p->child[1]=q;
        }

        return p;
    }
    else if(operator==(CurrentListNode->data.Ttype,"<")||
            operator==(CurrentListNode->data.Ttype,"=")||
            operator==(CurrentListNode->data.Ttype,"]")||
            operator==(CurrentListNode->data.Ttype,"then")||
            operator==(CurrentListNode->data.Ttype,"else")||
            operator==(CurrentListNode->data.Ttype,"fi")||
            operator==(CurrentListNode->data.Ttype,"do")||
            operator==(CurrentListNode->data.Ttype,"endwh")||
            operator==(CurrentListNode->data.Ttype,")")||
            operator==(CurrentListNode->data.Ttype,"end")||
            operator==(CurrentListNode->data.Ttype,";")||
            operator==(CurrentListNode->data.Ttype,","))
    {
        return NULL;
    }
    else
    {

    }
}

static TreeNode * term(void)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"(")||
            operator==(CurrentListNode->data.Ttype,"intc")||
            operator==(CurrentListNode->data.Ttype,"id"))
    {
        TreeNode *t=factor();
        TreeNode *p=otherFactor(t);

        if(p==NULL)
            return t;
        else
            return p;
    }
    else
    {
    }
}

static TreeNode* otherFactor(TreeNode *t)
{
    if(CurrentListNode==NULL);
    else if(operator==(CurrentListNode->data.Ttype,"*")||
            operator==(CurrentListNode->data.Ttype,"/"))
    {
        TreeNode *p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.ExpAttr.op=(char)CurrentListNode->data.Ttype[0];
        }

        match(CurrentListNode->data.Ttype);

        TreeNode *q=term();
        if(p!=NULL)
        {
            p->child[1]=q;
        }

        return p;
    }
    else if(operator==(CurrentListNode->data.Ttype,"+")||
            operator==(CurrentListNode->data.Ttype,"-")||
            operator==(CurrentListNode->data.Ttype,"<")||
            operator==(CurrentListNode->data.Ttype,"=")||
            operator==(CurrentListNode->data.Ttype,"]")||
            operator==(CurrentListNode->data.Ttype,"then")||
            operator==(CurrentListNode->data.Ttype,"else")||
            operator==(CurrentListNode->data.Ttype,"fi")||
            operator==(CurrentListNode->data.Ttype,"do")||
            operator==(CurrentListNode->data.Ttype,"endwh")||
            operator==(CurrentListNode->data.Ttype,")")||
            operator==(CurrentListNode->data.Ttype,"end")||
            operator==(CurrentListNode->data.Ttype,";")||
            operator==(CurrentListNode->data.Ttype,","))
    {
        return NULL;
    }
    else
    {

    }
}

static TreeNode * factor(void)
{
    TreeNode *t=NULL;
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"("))
    {
        match("(");
        t=exp();
        match(")");
    }
    else if(operator==(CurrentListNode->data.Ttype,"intc"))
    {
        t=newExpNode(ConstK);
        t->attr.ExpAttr.val=stringToInt(CurrentListNode->data.value);
        match("intc");
    }
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        t=variable();
    }
    else
    {

    }
    return t;
}

static TreeNode * variable(void)
{
    TreeNode *t=newExpNode(VariK);
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        if(t!=NULL)
        {
            (t->name[t->idnum])=CurrentListNode->data.value;
            (t->idnum)++;
        }
        match("id");
        variMore(t);
    }
    else
    {

    }
    return t;
}

static void variMore(TreeNode * t)
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,":=")||
            operator==(CurrentListNode->data.Ttype,"*")||
            operator==(CurrentListNode->data.Ttype,"=")||
            operator==(CurrentListNode->data.Ttype,"<")||
            operator==(CurrentListNode->data.Ttype,"+")||
            operator==(CurrentListNode->data.Ttype,"-")||
            operator==(CurrentListNode->data.Ttype,"/")||
            operator==(CurrentListNode->data.Ttype,")")||
            operator==(CurrentListNode->data.Ttype,"]")||
            operator==(CurrentListNode->data.Ttype,";")||
            operator==(CurrentListNode->data.Ttype,",")||
            operator==(CurrentListNode->data.Ttype,"then")||
            operator==(CurrentListNode->data.Ttype,"else")||
            operator==(CurrentListNode->data.Ttype,"fi")||
            operator==(CurrentListNode->data.Ttype,"do")||
            operator==(CurrentListNode->data.Ttype,"endwh")||
            operator==(CurrentListNode->data.Ttype,"end"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"["))
    {
        match("[");

        if(t!=NULL)
        (t->child[0])=exp();

        if(t!=NULL)
            t->attr.ExpAttr.varkind=ArrayMembV;

        if((t->child[0])!=NULL)
            t->child[0]->attr.ExpAttr.varkind=IdV;

        match("]");
    }
    else if(operator==(CurrentListNode->data.Ttype,"."))
    {
        match(".");

        if(t!=NULL)
        {
            t->child[0]=fieldVar();
            t->attr.ExpAttr.varkind=FieldMembV;
        }
        if(t->child[0]!=NULL)
        {
            t->child[0]->attr.ExpAttr.varkind=IdV;
        }
    }
    else
    {

    }
}

static TreeNode * fieldVar(void)
{
    TreeNode *t=newExpNode(VariK);

    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,"id"))
    {
        if(t!=NULL)
        {
            (t->name[t->idnum])=CurrentListNode->data.value;
            (t->idnum)++;
        }
        match("id");
        fieldVarMore(t);
    }
    else
    {
    }
    return t;
}

static void fieldVarMore(TreeNode * t )
{
    if(CurrentListNode==nullptr);
    else if(operator==(CurrentListNode->data.Ttype,":=")||
            operator==(CurrentListNode->data.Ttype,"*")||
            operator==(CurrentListNode->data.Ttype,"=")||
            operator==(CurrentListNode->data.Ttype,"<")||
            operator==(CurrentListNode->data.Ttype,"+")||
            operator==(CurrentListNode->data.Ttype,"-")||
            operator==(CurrentListNode->data.Ttype,"/")||
            operator==(CurrentListNode->data.Ttype,")")||
            operator==(CurrentListNode->data.Ttype,"]")||
            operator==(CurrentListNode->data.Ttype,";")||
            operator==(CurrentListNode->data.Ttype,",")||
            operator==(CurrentListNode->data.Ttype,"then")||
            operator==(CurrentListNode->data.Ttype,"else")||
            operator==(CurrentListNode->data.Ttype,"fi")||
            operator==(CurrentListNode->data.Ttype,"do")||
            operator==(CurrentListNode->data.Ttype,"endwh")||
            operator==(CurrentListNode->data.Ttype,"end"))
    {

    }
    else if(operator==(CurrentListNode->data.Ttype,"["))
    {
        match("[");
        if(t!=NULL)
        {
            t->child[0]=exp();
            t->child[0]->attr.ExpAttr.varkind=ArrayMembV;
        }
        match("]");
    }
    else
    {

    }
}
