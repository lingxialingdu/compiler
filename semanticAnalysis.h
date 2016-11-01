#ifndef SEMANTICANALYSIS_H_INCLUDED
#define SEMANTICANALYSIS_H_INCLUDED

#include "treeNode.h"
#include "symbolTable.h"

void analysis(TreeNode*);

/**static TypeIR * newTy(TypeKind  kind);
static FieldChain * newBody(void);
static ParamTable * newParam(void);
static AttributeIR findAttr(SymbolTable *entry);
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
static void returnStatement(TreeNode *t);*/

#endif // SEMANTICANALYSIS_H_INCLUDED
