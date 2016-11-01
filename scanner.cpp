#include <string>
#include <iostream>
using namespace std;
#include <stdio.h>
#include "scanner.h"

std::string key[21]={"program","type","var","procedure","begin",
"end","array","of","record","if","then","else","fi","while",
"do","endwh","read","write","return","integer","char"};

int Iskey(const std::string str)
{

	for(int i=0;i<21;i++)
	{
		if(operator==(str,key[i]))
			return 1;
	}
	return 0;
}

int Isletter(char c)
{
    if(((c<='z')&&(c>='a'))||((c<='Z')&&(c>='A')))
		return 1;
	else
		return 0;
}

int Isdigit(char c)
{
    if(c>='0'&& c<='9')
		return 1;
	else
		return 0;
}

List scanner(List t)
{
    std::string arr="";
	char ch=' ';
	int line=1;
	char scannerInputFile[30]="";
	cout<<"please input the *.txt file:"<<endl;
	cin>>scannerInputFile;
	FILE* fd=fopen(scannerInputFile,"r");
	ch=fgetc(fd);

	while(ch!=EOF)
	{
		arr="";
		if(ch==' '){
                while((ch=fgetc(fd))==' ');
		}else if(ch=='\n'){
				line++;
				while((ch=fgetc(fd))=='\n');
            }else{
              if(Isletter(ch)){
					while(Isletter(ch)||Isdigit(ch))
					{
						if((ch <= 'Z')&&(ch >= 'A'))
							ch=ch+32;
						arr=arr+ch;
						ch=fgetc(fd);
					}
             if(Iskey(arr))
					{
						token Tkeyword;
						Tkeyword.linenum=line;
						Tkeyword.Ttype=arr;
						Tkeyword.value=arr;
						t.inserttail(Tkeyword);
					}
					else
					{
						token Tid;
						Tid.linenum=line;
						Tid.Ttype="id";
						Tid.value=arr;
						t.inserttail(Tid);
					}

				}
              else if(Isdigit(ch)){
						while(Isdigit(ch))
                            {
                                arr=arr+ch;
                                ch=fgetc(fd);
                            }
                            token Tnumber;
                            Tnumber.linenum=line;
                            Tnumber.Ttype="intc";
                            Tnumber.value=arr;
                            t.inserttail(Tnumber);
					}
              else{
                switch(ch){
				case'+':
				case'-':
				case'*':
				case'=':
				case'/':{
					token Tcal;
					Tcal.linenum=line;
					Tcal.Ttype=ch;
					Tcal.value=ch;
					t.inserttail(Tcal);
					ch=fgetc(fd);
					break;
                }
				case'(':
                case')':
				case'[':
				case']':
				case';':
				case',':{
						token Tborder;
						Tborder.linenum=line;
						Tborder.Ttype=ch;
						Tborder.value=ch;
						t.inserttail(Tborder);
						ch=fgetc(fd);
						break;
					}
				case'{':{
					    ch=fgetc(fd);
						while(ch!='}')
						{
						    ch=fgetc(fd);
						}
						break;

					}
				case'}':{
				        ch=fgetc(fd);
						break;
					}
				case':':{
					ch=fgetc(fd);
					if(ch=='=')
					{
						token Tcal;
						Tcal.linenum=line;
						Tcal.Ttype=":=";
						Tcal.value=":=";
						t.inserttail(Tcal);
						ch=fgetc(fd);
					}
					else
					{
						token Tcal;
						Tcal.linenum=line;
						Tcal.Ttype=":";
						Tcal.value=":";
						t.inserttail(Tcal);

					}
					break;
                }
				case'.':{
				    ch=fgetc(fd);
					if(ch==EOF)
					{
						token Tcal;
						Tcal.linenum=line;
						Tcal.Ttype=".";
						Tcal.value=".";
						t.inserttail(Tcal);
					}
					else
					{
						if(ch=='.')
						{
							token Tcal;
							Tcal.linenum=line;
							Tcal.Ttype="..";
							Tcal.value="..";
							t.inserttail(Tcal);
							ch=fgetc(fd);
						}
						else
						{
							token Tcal;
							Tcal.linenum=line;
							Tcal.Ttype=".";
							Tcal.value=".";
							t.inserttail(Tcal);
						}

					}
					break;
                }
				case'>':{
					ch=fgetc(fd);
					if(ch=='=')
					{
						token Tcal;
						Tcal.linenum=line;
						Tcal.Ttype=">=";
                        Tcal.value=">=";
                        t.inserttail(Tcal);
						ch=fgetc(fd);
					}
					else
					{
						token Tcal;
						Tcal.linenum=line;
                        Tcal.Ttype=">";
                        Tcal.value=">";
                        t.inserttail(Tcal);
					}

					break;
                }
                case'<':{
					ch=fgetc(fd);
					if(ch=='=')
					{
						token Tcal;
						Tcal.linenum=line;
                        Tcal.Ttype="<=";
                        Tcal.value="<=";
                        t.inserttail(Tcal);
						ch=fgetc(fd);
					}
					else if(ch=='>')
					{
						token Tcal;
						Tcal.linenum=line;
                        Tcal.Ttype="<>";
                        Tcal.value="<>";
                        t.inserttail(Tcal);
						ch=fgetc(fd);
					}
					else
					{
						token Tcal;
						Tcal.linenum=line;
                        Tcal.Ttype="<";
                        Tcal.value="<";
                        t.inserttail(Tcal);
					}
                break;
                }
				default:{
				    ch=fgetc(fd);
				}
              }
            }
        }
	}
	fclose(fd);
	return t;
}

