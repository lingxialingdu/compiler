#ifndef _LISTNODE_H_
#define _LISTNODE_H_

#include "token.h"

struct listNode
{
	token data;
	listNode * next;
	listNode(listNode * nextnode=NULL){next=nextnode;}
	listNode(const token item,listNode * nextnode=NULL)
	{
	    data=item;
	    next=nextnode;
    }
};

#endif
