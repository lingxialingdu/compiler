#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED



struct SymbolTable;
struct TypeIR;
struct ParamTable;

typedef enum {constKind,varKind,typeKind,routKind,fieldKind} IdKind;

typedef enum {direct,indirect} AccessKind;

typedef enum {intTy,boolTy,charTy,realTy,arrayTy,recordTy} TypeKind;

struct FieldChain
{
    std::string idName;
    TypeKind typeK;
    TypeIR *unitType;
    unsigned int offset;
    FieldChain *next;
};

struct TypeIR
{
    unsigned int typeSize;
    TypeKind kind;
    union
    {
        struct
        {
            struct TypeIR *indexTy;  /**指向数组下标的内部表示*/
            struct TypeIR *elemTy;   /**指向数组元素的内部表示*/
            unsigned int up;
            unsigned int low;
        }ArrayAttr;

        FieldChain *body;            /**域变量的内部表示*/

    }More;
};

struct AttributeIR
{
    struct TypeIR *idType;
    IdKind  kind;
    union
    {
        struct
        {
            AccessKind access;
            unsigned int level;
            unsigned int off;
            bool isParam;
        }VarAttr;
        struct
        {
            int level;         /**过程的层数*/
            ParamTable *param;
            unsigned int mOff; /**过程活动记录的大小,没有加上临时变量*/
            unsigned int nOff; /**sp到display表的偏移*/
            int codeEntry;     /**过程的入口标号，用于中间代码生成阶段*/
            int procEntry;     /**过程入口地址*/
        }ProcAttr;
    }More;
};

struct SymbolTable
{
    std::string idname;
    AttributeIR attrIR;
    struct SymbolTable *next;
};

struct ParamTable
{
    SymbolTable *entry;
    ParamTable *next;
};

#endif // SYMBOLTABLE_H_INCLUDED
