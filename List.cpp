
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "List.h"

void List::inserttail(token item)
{
	if(head==nullptr)
	{
		head=new listNode(item,nullptr);
		tail=head;
	}
	else
	{
		tail->next=new listNode(item,nullptr);
		tail=tail->next;
	}
}


void List::show()
{

	if (isempty())
	{
		cout<<"the source file is empty"<<endl;
	}
	else
	{
	    char scannerOutputFile[30]="";
	    cout<<"please input the scannerOutputFile:"<<endl;
	    cin>>scannerOutputFile;

		ofstream outfile;
		outfile.open(scannerOutputFile,ios::out);
		listNode *p=head;
		while (p)
		{
		//	cout<<setw(10)<<setiosflags(ios::left)<<p->data.linenum<<"   ";
			outfile<<setw(10)<<setiosflags(ios::left)<<p->data.linenum<<"   ";
		//	cout<<setw(10)<<setiosflags(ios::left)<<p->data.Ttype<<"   ";
			outfile<<setw(10)<<setiosflags(ios::left)<<p->data.Ttype<<"   ";
		//	cout<<setw(10)<<setiosflags(ios::left)<<p->data.value<<endl;
			outfile<<setw(10)<<setiosflags(ios::left)<<p->data.value<<endl;

			p=p->next;
		}
		outfile.close();
	}
}
