#include "STL_spu_struct.h"

static void STL_Print (const char* const fmt, ...);

int
STL_SpuStructCtor (SPU_Struct* spu)
{
    assert (spu);

    StackCtor (&(spu->stk));

    for (int i = 0; i < nRegisters; ++i)
    {
        spu->registers[i] = 0;
    }

    spu->err = 0;

    return 0;
}

int
STL_SpuStructDtor (SPU_Struct* spu)
{
    assert (spu);

    StackDtor (&(spu->stk));

    for (int i = 0; i < nRegisters; ++i)
    {
        spu->registers[i] = 0;
    }

    spu->err = 0;

    spu = nullptr;

    return 0;
}

int
STL_SpuStructVerificator (SPU_Struct* spu)
{
    assert (spu);

    StackVerificator (&(spu->stk));

    return spu->stk.err + spu->err;
}

void
STL_SpuStructDump (const SPU_Struct* spu)
{
    StackDump (&(spu->stk));

    for (int i = 0; i < nRegisters; ++i)
    {
        STL_Print ("r%d = %d\n", i, spu->registers[i]);
    }
}

static void
STL_Print (const char* const fmt, ...)
{
    assert   (fmt);

    va_list   args = {};

    va_start (args, fmt);

    vfprintf (stderr, fmt, args);

    va_end   (args);
}


#undef Verificator
