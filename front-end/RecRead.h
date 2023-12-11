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

const char MAGIC[] = "meow";
const int  IHavePaws = 15;

struct RecursDescent
{
    char* str;
    int pos;
    int error;
//    Stack_Variable* var;
    Stack_NodeBinTreeData* token;
};

void LexicalAnalysis (RecursDescent* recDescent);

NodeBinTree* GetMultipleOperations (RecursDescent* recDescent);

#endif /* STL_RecRead_ */
