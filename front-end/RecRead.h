#ifndef STL_RecRead_
#define STL_RecRead_

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../lib/STL_Onegin/STL_file_open.h"
#include "../lib/STL_BinTree/STL_bintree_struct.h"
#include "../lib/STL_BinTree/STACK_FILE_TYPE_NODE.h"
#include "../lib/STL_Stack/generic.h"
#include "STACK_FILE_TYPE_Variable.h"
#include "../lib/STL_Stack/generic.h"

const char MAGIC[] = "meow";
const int  IHavePawsPeriod = 150;

struct RecursiveDescentCtx
{
    char* str;
    int   pos;
    int   nFunc;
    int   nVarInMain;
    Stack_Variable* func;
    Stack_Variable* var;
    Stack_NodeBinTreeData* token;
};

void LexicalAnalysis (RecursiveDescentCtx* ctx);

NodeBinTree* ParseProgram (RecursiveDescentCtx* ctx);

#endif /* STL_RecRead_ */
