#ifndef STL_back_end_
#define STL_back_end_

#include "../lib/STL_BinTree/STL_bintree_struct.h"

enum ErrorInFunction
{
    ERROR_IN_CompileMultipleOperations  = 1,
    ERROR_IN_CompileOperation           = 2,
    ERROR_IN_CompileFunction            = 3,
    ERROR_IN_CompileFuncReturn          = 4,
    ERROR_IN_CompilePrintf              = 5,
    ERROR_IN_CompileAssign              = 6,
    ERROR_IN_CompileIf                  = 7,
    ERROR_IN_CompileCreate              = 8,
};

struct BackEndCtx
{
    NodeBinTree* node;
    FILE* fp;
    int nIf;
    int nWhile;
};

int CompileProgram (BackEndCtx* ctx);

#endif /* STL_back_end_ */
