
#include "scanner.h"
#include "List.h"
#include "rd.h"
#include "tree.h"
#include "interfaceMidCode.h"
#include "interfaceObjectCode.h"
#include "semanticAnalysis.h"
#include "operatorSymbolTable.h"

extern int Level;
extern SymbolTable *scope[];

int main()
{
    List newlist;
    scanner(newlist).show(); /**验证词法分析的正确性**/

    parse();     /**进行语法分析*/
    extern Tree Gtree;
    printTree(Gtree.root);

    analysis(Gtree.root);
    printSymbolTable();

    genMidCode(Gtree.root);
    extern CodeListNode *firstCode;
    printMidCode(firstCode);

    genFinalCode(firstCode);
    printObjectCode();
    stepTM();
}




