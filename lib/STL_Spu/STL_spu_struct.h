#ifndef STL_spu_struct_
#define STL_spu_struct_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "STACK_FILE_TYPE_int.h"
#include "../STL_stack/generic.h"
#include "include/STL_spu_struct_const.h"
#include "include/STL_spu_const.h"

struct SPU_Struct
{
    struct Stack_int stk;
    SPU_DATA_TYPE registers[nRegisters];
    int err;
};

#define SpuStructCtor(stk)                                          \
    STL_SpuStructCtor ((stk))

#define SpuStructDtor(stk)                                          \
    STL_SpuStructDtor ((stk))

#define SpuStructVerificator(stk)                                   \
    STL_SpuStructVerificator ((stk))   // גûחמג dump

#define SpuStructDump(stk)                                          \
    STL_SpuStructDump ((stk))

int
STL_SpuStructCtor (SPU_Struct* stk);

int
STL_SpuStructDtor (SPU_Struct* stk);

int
STL_SpuStructVerificator (SPU_Struct* stk);

void
STL_SpuStructDump (const SPU_Struct* stk);

#endif /* STL_stack_ */
