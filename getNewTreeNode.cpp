#include <string>
using namespace std;

#include "treeNode.h"
#include "getNewTreeNode.h"

/********************************************************
 *********以下是创建语法树所用的各类节点的申请***********
 ********************************************************/

/********************************************************/
/* 函数名 newRootNode									*/
/* 功  能 创建语法树根节点函数			        		*/
/* 说  明 该函数为语法树创建一个新的根结点      		*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newRootNode(void)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
      //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i] = NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型ProK */
	t->nodekind = ProK;

	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

	t->idnum=0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i] = " ";
		t->table[i] = NULL;
	}

  }
  /* 函数返回语法树根节点指针t */
  return t;
}


/********************************************************/
/* 函数名 newPheadNode									*/
/* 功  能 创建程序头类型语法树节点函数					*/
/* 说  明 该函数为语法树创建一个新的程序头类型结点		*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newPheadNode(void)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode * t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	   //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++) t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型PheadK */
	t->nodekind = PheadK;

	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

	t->idnum = 0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i] = "";
		t->table[i] = NULL;
	}
  }
  /* 函数返回程序头类型语法树节点指针t */
  return t;
}


/********************************************************/
/* 函数名 newDecANode									*/
/* 功  能 创建声明语法树节点函数,没有指明具体的节点声明 */
/*        类型,在语法树的第二层			                */
/* 说  明 该函数为语法树创建一个新的结点      	     	*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newDecANode(NodeKind kind)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode * t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	  //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++) t->child[0]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为参数kind */
	t->nodekind = kind;

	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

	t->idnum=0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}
  }
  /* 函数返回语法树根节点指针t */
  return t;
}



//尝试


/********************************************************/
/* 函数名 newTypeNode									*/
/* 功  能 类型标志语法树节点创建函数					*/
/* 说  明 该函数为语法树创建一个新的类型标志结点，      */
/*        表示在它之下的声明都为类型声明，        		*/
/*        并将语法树节点的成员初始化					*/
/********************************************************/
TreeNode * newTypeNode()

{
  /* 内存中动态申请分配单元，返回指向该单元的语法树节点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));

  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元		*
   * 将出错信息及行号lineno写入列表文件listing		*/
  if (t==NULL);
    //fprintf(listing,"Out of memory error at line %d\n",lineno);

  /* 语法树节点指针t不是NULL,内存单元已经成功分配 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL(空) */
   	t->sibling = NULL;

	/* 指定新语法树节点t成员: 结点类型nodekind为表达式类型ExpK */
	t->nodekind = TypeK;


	/* 指定新语法树节点t成员: 源代码行号lineno为全局变量lineno */
    //t->lineno = lineno;
    t->lineno=0;

	t->idnum = 0;
	/*初始化符号表地址指针*/
	for (int i=0;i<10;i++)
	   t->table[i] = NULL;

  }

  /* 函数返回表达式类型语法树结点指针t */
  return t;
}

/********************************************************/
/* 函数名 newVarNode									*/
/* 功  能 变量声明标志语法树节点创建函数				*/
/* 说  明 该函数为语法树创建一个新的变量声明标志结点    */
/*         表示在它之下的声明都为变量声明，           	*/
/*        并将语法树节点的成员初始化					*/
/********************************************************/
TreeNode * newVarNode()

{
  /* 内存中动态申请分配单元，返回指向该单元的语法树节点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元		*
   * 将出错信息及行号lineno写入列表文件listing		*/
  if (t==NULL);
    //fprintf(listing,"Out of memory error at line %d\n",lineno);

  /* 语法树节点指针t不是NULL,内存单元已经成功分配 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL(空) */
   	t->sibling = NULL;

	/* 指定新语法树节点t成员: 结点类型nodekind为表达式类型ExpK */
	t->nodekind = VarK;

	/* 指定新语法树节点t成员: 源代码行号lineno为全局变量lineno */
    //t->lineno = lineno;
    t->lineno=0;

    t->idnum = 0;
	/*初始化符号表地址指针*/
	for (int i=0;i<10;i++)
	   t->table[i] = NULL;
  }

  /* 函数返回表达式类型语法树结点指针t */
  return t;
}

//试试，如果行的话，就把它们合成一个！！！！


/********************************************************/
/* 函数名 newDecNode									*/
/* 功  能 创建声明类型语法树节点函数					*/
/* 说  明 该函数为语法树创建一个新的声明类型结点		*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newDecNode(void)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	  //Error = TRUE;
  }

  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++) t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型DecK*/
	t->nodekind = DecK;

	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

	t->idnum=0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}

  }
  /* 函数返回声明类型语法树节点指针t */
  return t;
}


/********************************************************/
/* 函数名 newProcNode									*/
/* 功  能 创建函数类型语法树节点函数					*/
/* 说  明 该函数为语法树创建一个新的函数类型结点		*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newProcNode(void)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
      //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型ProcDecK */
	t->nodekind = ProcDecK;


	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

    t->idnum = 0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}

  }
  /* 函数返回语句类型语法树节点指针t */
  return t;
}

/********************************************************/
/* 函数名 newStmlNode									*/
/* 功  能 创建语句标志类型语法树节点函数				*/
/* 说  明 该函数为语法树创建一个新的语句标志类型结点	*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newStmlNode(void)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	  //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型StmLK */
	t->nodekind = StmLK;

    /* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

    t->idnum = 0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}

  }
   /*函数返回语句类型语法树节点指针t*/
  return t;
}

/********************************************************/
/* 函数名 newStmtNode									*/
/* 功  能 创建语句类型语法树节点函数					*/
/* 说  明 该函数为语法树创建一个新的语句类型结点		*/
/*        并将语法树节点成员初始化						*/
/********************************************************/
TreeNode * newStmtNode(StmtKind kind)

{
  /* 在内存中动态申请分配单元，返回指向该单元的语法树结点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元 *
   * 将出错信息及行号lineno写入列表文件listing  */
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	  //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,成功分配内存单元 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++) t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL */
	t->sibling = NULL;

	/* 指定新语法树节点t成员:结点类型nodekind为语句类型StmtK */
	t->nodekind = StmtK;

	/* 指定新语法树节点t成员:语句类型kind.stmt为函数给定参数kind */
    t->kind.stmt = kind;

	/* 指定新语法树节点t成员:源代码行号lineno为全局变量lineno */
	//t->lineno = lineno;
	t->lineno=0;

	t->idnum = 0;

	for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}
  }
  /* 函数返回语句类型语法树节点指针t */
  return t;
}


/********************************************************/
/* 函数名 newExpNode									*/
/* 功  能 表达式类型语法树节点创建函数					*/
/* 说  明 该函数为语法树创建一个新的表达式类型结点		*/
/*        并将语法树节点的成员初始化					*/
/********************************************************/
TreeNode * newExpNode(ExpKind kind)

{
  /* 内存中动态申请分配单元，返回指向该单元的语法树节点类型指针t */
  //TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  TreeNode *t = new TreeNode();

  int i;

  /* 语法树节点指针t为NULL,未能成功分配内存单元		*
   * 将出错信息及行号lineno写入列表文件listing		*/
  if (t==NULL)
  {
	  //fprintf(listing,"Out of memory error at line %d\n",lineno);
	  //Error = TRUE;
  }
  /* 语法树节点指针t不是NULL,内存单元已经成功分配 */
  else {

	/* 初始化新语法树节点t各子节点child[i]为NULL */
    //for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    for(i=0;i<3;i++)  t->child[i]=NULL;

	/* 初始化新语法树节点t兄弟节点sibling为NULL(空) */
   	t->sibling = NULL;

	/* 指定新语法树节点t成员: 结点类型nodekind为表达式类型ExpK */
	t->nodekind = ExpK;

	/* 指定新语法树节点t成员: 表达式类型kind.exp为函数给定参数kind */
  	t->kind.exp = kind;

	/* 指定新语法树节点t成员: 源代码行号lineno为全局变量lineno */
    //t->lineno = lineno;
    t->lineno=0;

	/* 指定新语法树节点t成员: 表达式为变量类型时的变量类型varkind *
	/* 为IdV.*/
	t->attr.ExpAttr.varkind = IdV;

	/* 指定新语法树节点t成员: 类型检查类型type为Void */
 	t->attr.ExpAttr.type = Void;

    for(i=0;i<10;i++)
	{
		//strcpy(t->name[i],"\0");
		t->name[i]="";
		t->table[i] = NULL;
	}
  }

  /* 函数返回表达式类型语法树结点指针t */
  return t;
}
