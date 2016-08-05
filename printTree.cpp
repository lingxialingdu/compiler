#include <iostream>
using namespace std;

#include "tree.h"

/** 静态变量indentno在函数printTree中	*/
/** 用于存储当前子树缩进格数,初始为0   */
static unsigned int indentno = 0;

/** 增量/减量缩进宏定义 **/
#define INDENT indentno+=4
#define UNINDENT indentno-=4



/********************************************************/
/* 函数名 printSpaces									*/
/* 功  能 空格打印函数									*/
/* 说  明 该函数打印指定数量空格,用于表示子树缩进		*/
/********************************************************/
static void printSpaces(void)

{
  /* 按给定缩进量indentno打印空格进行缩进	*
   * 其中缩进量indentno总能保持非负			*/
  for (unsigned int i=0;i<indentno;i++)
      //fprintf(listing," ");
      cout<<" ";

}

static void printTab(unsigned int i)
{
    for(unsigned int _count=0;_count<i;_count++)
        cout<<"\t";
}

/******************************************************/
/* 函数名 printTree                                   */
/* 功  能 把语法树输出，显示在listing文件中           */
/* 说  明 该函数运用了宏来定义增量减量的缩进          */
/******************************************************/
void  printTree(TreeNode  *tree)
{
    int i;
    /* 增量缩进宏,每次进入语法树节点都进行增量缩进 */
    INDENT;

    /* 函数参数给定语法树节点指针tree非NULL(空) */
    while (tree != NULL)
        {
            /*打印行号*/
            /**if(tree->lineno==0);
            //printTab(9);
            else
            switch((int)(tree->lineno / 10))
            {
			case 0:
				//fprintf(listing,"line:%d",tree->lineno);
				cout<<"line:"<<tree->lineno;
				//printTab(3);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				//fprintf(listing,"line:%d",tree->lineno);
				cout<<"line:"<<tree->lineno;
				//printTab(2);
				break;
			default:
				//fprintf(listing,"line:%d",tree->lineno);
				cout<<"line:"<<tree->lineno;
				//printTab(1);
		}**/

    /* 调用函数printSpaces,打印相应的空格,进行缩进 */
    printSpaces();

  switch ( tree->nodekind)
  {
     case ProK :
		   //fprintf(listing,"%s  ","ProK"); break;
		   cout<<"ProK ";break;
      case PheadK:
		  {
		      //fprintf(listing,"%s  ","PheadK");
		      //fprintf(listing,"%s  ",tree->name[0]);
		      cout<<"PheadK  ";
		      cout<<tree->name[0]<<"  ";
		  }
		   break;
	  case DecK:
		{
		      //fprintf(listing,"%s  ","DecK");
		      cout<<"DecK  ";
		      if (tree->attr.ProcAttr.paramt==varparamType )
                //fprintf(listing,"%s  ","var param:");
                cout<<"var param  ";
              if (tree->attr.ProcAttr.paramt==valparamType)
                //fprintf(listing,"%s  ","value param:");
                cout<<"value param  ";
              switch(tree->kind.dec)
              {
                   case  ArrayK:
                       {
                           //fprintf(listing,"%s  ","ArrayK");
                           cout<<"ArrayK  ";
                           //fprintf(listing,"%d  ",tree->attr.ArrayAttr.up);
                           cout<<tree->attr.ArrayAttr.up<<"  ";
                           //fprintf(listing,"%d  ",tree->attr.ArrayAttr.low);
                           cout<<tree->attr.ArrayAttr.low<<"  ";
                           if (tree->attr.ArrayAttr.childType == CharK)
                            //fprintf(listing,"%s  ","Chark");
                            cout<<"CharK  ";
                           else if( tree->attr.ArrayAttr.childType == IntegerK)
                           //fprintf(listing,"%s  ","IntegerK");
                           cout<<"IntegerK  ";
                        };break;

                    case  CharK:
                        //fprintf(listing,"%s  ","CharK");break;
                        cout<<"CharK  ";break;
                    case  IntegerK:
                        //fprintf(listing,"%s  ","IntegerK");break;
                        cout<<"IntegerK  ";break;
                    case  RecordK:
                        //fprintf(listing,"%s  ","RecordK");break;
                        cout<<"RecordK  ";break;
                    case  IdK:
                        //fprintf(listing,"%s  ","IdK");
                        cout<<"IdK ";
                        //fprintf(listing,"%s  ",tree->attr.type_name);
                        cout<<tree->attr.type_name<<"  ";
                    break;
                    default:
                        //fprintf(listing,"error1!");
                        //Error = TRUE;
                        cout<<"error!";
                };
                if (tree->idnum !=0)
                    for (unsigned int i=0 ; i <= (tree->idnum);i++)
                    {
                        //fprintf(listing,"%s  ",tree->name[i]);
                        cout<<tree->name[i]<<"  ";
                    }
                else
                    {
                        //fprintf(listing,"wrong!no var!\n");
                        //Error = TRUE;
                        cout<<"wrong!no var!";
                    }
                } break;
	  case TypeK:
		   //fprintf(listing,"%s  ","TypeK");break;
		   cout<<"TypeK  ";break;

	  case VarK:
		   //fprintf(listing,"%s  ","VarK");
		   cout<<"VarK  ";
		   if(tree->table[0]!=NULL);
			   //fprintf(listing,"%d  %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);
		   break;

	  case ProcDecK:
			//fprintf(listing,"%s  ","ProcDecK");
			cout<<"ProcDecK  ";
		    //fprintf(listing,"%s  ",tree->name[0]);
		    cout<<tree->name[0];
			if(tree->table[0]!=NULL);
				//fprintf(listing,"%d %d %d  ",tree->table[0]->attrIR.More.ProcAttr.mOff,tree->table[0]->attrIR.More.ProcAttr.nOff,tree->table[0]->attrIR.More.ProcAttr.level);
            break;

	  case StmLK:
		   //fprintf(listing,"%s  ","StmLk");break;
		   cout<<"StmLk  ";break;

	  case StmtK:
		  {
		      //fprintf(listing,"%s  ","StmtK");
		      cout<<"StmtK  ";
		      switch (tree->kind.stmt)
		      {
		           case IfK:
		               //fprintf(listing,"%s  ","If");break;
		               cout<<"If  ";break;
                   case WhileK:
                       //fprintf(listing,"%s  ","While");break;
                       cout<<"While  ";break;
                   case AssignK:
                       //fprintf(listing,"%s  ","Assign");
                       cout<<"Assign  ";
				   break;

				   case ReadK:
				       //fprintf(listing,"%s  ","Read");
				       cout<<"Read  ";
				       //fprintf(listing,"%s  " , tree->name[0]);
				       cout<<tree->name[0];
				   if(tree->table[0]!=NULL);
					   //fprintf(listing,"%d   %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);
				   break;

				   case WriteK:
				       //fprintf(listing,"%s  ","Write");break;
				       cout<<"Write  ";break;
                   case CallK:
                       //fprintf(listing,"%s  ","Call");
                       cout<<"Call  ";
                       //fprintf(listing,"%s  ", tree->name[0]);
                       cout<<tree->name[0];
				   break;

				   case ReturnK:
				       //fprintf(listing,"%s  ","Return");break;
				       cout<<"Return ";break;
                   default:
                       //fprintf(listing,"error2!");
                       //Error = TRUE;
                       cout<<"error2!";
                       }
                    };break;
	  case ExpK:
		  {
		      //fprintf(listing,"%s  ","ExpK");
		      cout<<"ExpK  ";
		      switch (tree->kind.exp)
		      {
		          case OpK:
		              {
		                  //fprintf(listing,"%s  ","Op");
		                  cout<<"Op  ";
		                  switch(tree->attr.ExpAttr.op)
		                  {
		                      //case EQ:   fprintf(listing,"%s  " , "="); break;
		                      //case LT:   fprintf(listing,"%s  " , "<"); break;
		                      //case PLUS: fprintf(listing,"%s  " , "+"); break;
		                      //case MINUS:fprintf(listing,"%s  " , "-"); break;
		                      //case TIMES:fprintf(listing,"%s  " , "*"); break;
		                      //case OVER: fprintf(listing,"%s  " , "/"); break;
		                      case '=':  cout<<"=  ";break;
		                      case '<':  cout<<"<  ";break;
		                      case '+':  cout<<"+  ";break;
		                      case '-':  cout<<"-  ";break;
		                      case '*':  cout<<"*  ";break;
		                      case '/':  cout<<"/  ";break;
		                      default:
		                          //fprintf(listing,"error3!");
		                          //Error = TRUE;
		                          cout<<"error3!";
                            }
                            if(tree->attr.ExpAttr.varkind==ArrayMembV)
                                {
                                    //fprintf(listing,"ArrayMember  ");
                                    //fprintf(listing,"%s  ",tree->name[0]);
                                    cout<<"ArrayMember  ";
                                    cout<<tree->name[0];
                                }
                            };break;
                case ConstK:
                     //fprintf(listing,"%s  ","Const");
                     cout<<"Const  ";
                     switch(tree->attr.ExpAttr.varkind)
                     {
                         case IdV:
                             //fprintf(listing,"Id  ");
                             //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"Id  ";
                             cout<<tree->name[0]<<"  ";
                             break;
						case FieldMembV:
						     //fprintf(listing,"FieldMember  ");
						     //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"FieldMember  ";
                             cout<<tree->name[0]<<"  ";
						break;

						case ArrayMembV:
						     //fprintf(listing,"ArrayMember  ");
						     //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"ArrayMember  ";
                             cout<<tree->name[0]<<"  ";
						break;

						default:
							//fprintf(listing,"var type error!");
							//Error = TRUE;
							cout<<"var type error!";
					}
					//fprintf(listing,"%d  ",tree->attr.ExpAttr.val);
					cout<<tree->attr.ExpAttr.val<<"  ";
					break;
                case VariK:
                      //fprintf(listing,"%s  ","Vari");
                      cout<<"Vari  ";
                      switch(tree->attr.ExpAttr.varkind)
                      {
                          case IdV:
                              //fprintf(listing,"Id  ");
                              //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"Id  ";
                             cout<<tree->name[0]<<"  ";
                              break;
                          case FieldMembV:
                              //fprintf(listing,"FieldMember  ");
                              //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"FieldMember  ";
                             cout<<tree->name[0]<<"  ";
                              break;
                          case ArrayMembV:
                              //fprintf(listing,"ArrayMember  ");
                              //fprintf(listing,"%s  ",tree->name[0]);
                             cout<<"ArrayMember  ";
                             cout<<tree->name[0]<<"  ";
                              break;
                          default:
                              //fprintf(listing,"var type error!");
                              //Error = TRUE;
                              cout<<"var type error!";
				   }
				   if(tree->table[0]!=NULL);
					   //fprintf(listing,"%d   %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);

				   break;
              default:
				  //fprintf(listing,"error4!");
				  //Error = TRUE;
				  cout<<"error4!";
				  }
		  };break;
   default:
	   //fprintf(listing,"error5!");
	   //Error = TRUE;
	   cout<<"error5!";
   }

   //fprintf(listing,"\n");
   cout<<"\n";

    /* 对语法树结点tree的各子结点递归调用printTree过程 *
	 * 缩进写入列表文件listing						   */
    for (i=0;i<3;i++)
         printTree(tree->child[i]);

	/* 对语法树结点tree的兄弟节点递归调用printTree过程 *
	 * 缩进写入列表文件listing						   */
	tree = tree->sibling;
  }

  /* 减量缩进宏,每次退出语法树节点时减量缩进 */
  UNINDENT;
}


