#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED
#include <string>
using namespace std;

typedef enum {ProK,PheadK,DecK,TypeK,VarK,ProcDecK,StmLK,StmtK,ExpK} NodeKind;

typedef enum {ArrayK,CharK,IntegerK,RecordK,IdK,BoolK}  DecKind;

typedef enum {IfK,WhileK,AssignK,ReadK,WriteK,CallK,ReturnK} StmtKind;

typedef enum {OpK,ConstK,VariK} ExpKind;/**表达式的类型*/

typedef enum {IdV,ArrayMembV,FieldMembV} VarKind;/**记录变量的类型*/

typedef enum {valparamType,varparamType} ParamType;

typedef enum {Void,Integer,Boolean} ExpType;/**表达式计算结果的类型*/



struct TreeNode
{
    TreeNode* child[3];
    TreeNode* sibling;
    unsigned int lineno;
    NodeKind nodekind;
    union
    {
        DecKind dec;
        StmtKind stmt;
        ExpKind exp;
    }kind;
    unsigned int idnum;
    std::string name[10];
    struct SymbolTable *table[10];
    struct
    {
        struct
        {
            unsigned int low=0;
            unsigned int up=0;
            DecKind childType;
        }ArrayAttr;
        struct
        {
            ParamType paramt;
        }ProcAttr;
        struct
        {
            char op=' ';
            int val=0;
            VarKind varkind;
            ExpType type;
        }ExpAttr;

        std::string type_name="";
    }attr;
};
#endif // TREENODE_H_INCLUDED
