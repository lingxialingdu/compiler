#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "TreeNode.h"

typedef struct Tree
{
public:
    TreeNode* root;
    Tree()
    {
        root=NULL;
    }
}tree;

void printTree(TreeNode*);

#endif // TREE_H_INCLUDED
