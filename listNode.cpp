#include "listNode.h"
#include "token.h"

listNode*  newListNode(const Token item)
{
    listNode* l = (listNode*)malloc(sizeof(listNode));
    if(l)
    {
        l->data = item;
        l->next = NULL;
    }
    return l;

}
