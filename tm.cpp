#include <stdio.h>
#include "tm.h"
#include "objectCodeGen.h"

//extern int ac,ac1,ac2,displayOFF,top,sp,pc;
extern int instructionCount;
extern Instruction instructionMemory[];
extern int dataMemory[];

static int reg[8]={0,0,0,0,0,0,0,0};
int r=0,s=0,t=0;

#define listing stdout

static stepResult stepTM(void);


stepResult stepTM (void)
{

    while(i<=instructionCount&&i>=0)
    {
        r=instructionMemory[i].operand1;
        s=instructionMemory[i].operand2;
        t=instructionMemory[i].operand3;

        switch(instructionMemory[i].op)
        {
        case halt:
            fprintf(listing,"halt    0,   0,   0");
            return srHALT;
            break;
        case in:
            scanf("%d",&reg[r]);
            break;
        case out:
            break;
        case addtm:
            instructionMemory[i].operand1= instructionMemory[i].operand2+instructionMemory[i].operand3;
            break;
        case subtm:
            instructionMemory[i].operand1=instructionMemory[i].operand2-instructionMemory[i].operand3;
            break;
        case multm:
            instructionMemory[i].operand1=(instructionMemory[i].operand2)*(instructionMemory[i].operand3);
            break;
        case divitm:
            instructionMemory[i].operand1=(instructionMemory[i].operand2)/(instructionMemory[i].operand3);
            break;
        case ld:
            instructionMemory[i].operand1=dataMemory[instructionMemory[i].operand2+instructionMemory[i].operand3];
            break;
        case st:
            dataMemory[instructionMemory[i].operand2+instructionMemory[i].operand3]=instructionMemory[i].operand1;
            break;
        case lda:
            break;
        case ldc:
            break;
        case jlt:
            break;
        case jle:
            break;
        case jgt:
            break;
        case jge:
            break;
        case jeq:
            break;
        case jne:
            break;
        default:
            break;
        }
        pc++;
    }
}
