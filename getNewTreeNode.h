#ifndef GETNEWTREENODE_H_INCLUDED
#define GETNEWTREENODE_H_INCLUDED

#include "treeNode.h"
/*******************************/

TreeNode * newTypeNode();

TreeNode * newVarNode();

TreeNode * newRootNode(void);

TreeNode * newPheadNode(void);

TreeNode * newDecANode(NodeKind kind);

TreeNode * newDecNode(void);

TreeNode * newProcNode(void);

TreeNode * newStmlNode(void);

TreeNode * newStmtNode(StmtKind kind);

TreeNode * newExpNode(ExpKind kind);


void  printTree(TreeNode  *tree);



/*********************************/

#endif // GETNEWTREENODE_H_INCLUDED
