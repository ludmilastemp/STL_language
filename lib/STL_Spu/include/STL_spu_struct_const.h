#ifndef STL_spu_struct_const_
#define STL_spu_struct_const_

#include <string.h>
#include <stdio.h>

char*
STL_SpuErrPrint (int err);

typedef int SPU_DATA_TYPE;   /// не забудь поменять тип в стеке
typedef unsigned char OPCODE_T;

const int nRegisters = 8;

struct NewString
{
    char* str;
    int ip;
};

enum ArgType
{
    T_ARG_REG = 0x20,  /// 1 << 5     32
    T_ARG_RAM = 0x40,  /// 1 << 6     64
    T_ARG_IMM = 0x80,  /// 1 << 7     128
};

enum SPU_Error
{
    ERROR_EXIT                  = 1 << 0,
    ERROR_NOT_MEMORY            = 1 << 1,
    ERROR_FILE_FORMAT           = 1 << 2,
    ERROR_INCORRECT_FUNC        = 1 << 3,
    ERROR_INCORRECT_VALUE       = 1 << 4,
};

#endif /* STL_spu_struct_const_ */
