#include "listNode.h"


#ifndef _LIST_H_
#define _LIST_H_

struct  List
{
	listNode * head;
	listNode * tail;
	listNode * current;

	List()
	{
	    head=nullptr;
	    tail=nullptr;
	    current=nullptr;
    }

	void inserttail(token item);
	bool isempty()
	{
	    return head==nullptr;
    }
	void show();
};

#endif



