#ifndef STL_back_end_
#define STL_back_end_

#include "gen.h"
#include "../lib/STL_BinTree/STL_bintree_struct.h"

enum ErrorInFunction
{
    ERROR_IN_CompileMultipleOperations  = 1,
    ERROR_IN_CompileOperation           = 2,
    ERROR_IN_CompileArguments           = 3,
    ERROR_IN_CompileFunction            = 4,
    ERROR_IN_CompileFuncReturn          = 5,
    ERROR_IN_CompilePrintf              = 6,
    ERROR_IN_CompileAssign              = 7,
    ERROR_IN_CompileIf                  = 8,
    ERROR_IN_CompileCreate              = 9,
};

const int variableSize = 8;

struct BackEndCtx
{
    NodeBinTree* node;
    FILE* fp;
    Stack_Variable* var;
    int nIf;
    int nWhile;
    int tempVar;
    int nVarInFunc;
    char* buffer;
    char* funcIp[100];
    char* fixup[100];
    int fixupNFunc[100];
    int fixupSize;
};

int CompileProgram (BackEndCtx* ctx);

#endif /* STL_back_end_ */
