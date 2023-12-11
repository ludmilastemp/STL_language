#include "STL_spu_struct_const.h"

char*
STL_SpuErrPrint (int err)
{
    printf ("\n\n");

#define SpuPrintErrCheck(x)                     \
    if (err % (2 * x) >= x)                     \
    {                                           \
        printf ("ERROR! " #x "\n");             \
    }

    SpuPrintErrCheck (ERROR_INCORRECT_FUNC);
    SpuPrintErrCheck (ERROR_INCORRECT_VALUE);
    SpuPrintErrCheck (ERROR_FILE_FORMAT);

#undef SpuPrintErrCheck

    printf ("\n");

    return 0;
}
