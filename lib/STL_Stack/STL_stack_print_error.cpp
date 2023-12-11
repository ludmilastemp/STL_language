#include "STL_stack_print_error.h"

static char errStr[500] = "Error from ";

static void STL_Print (const char* const fmt, ...);

char*
StackPrintErr (const STACK* stk

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               )
{
    char str[100] = " ";

    sprintf (errStr, "\n\n"

#ifdef STACK_DEBUG
             "Error from %s %-3d %s()\n", STL_FSENT_ARGS
#endif
            );

#define StackPrintErrCheck(x)                \
    if (stk->err % (2 * x) >= x)             \
    {                                        \
        sprintf (str, "ERROR! " #x "\n");    \
        strcat (errStr, str);                \
    }

    StackPrintErrCheck (ERR_NOT_DATA_POINTER);
    StackPrintErrCheck (ERR_NOT_MEMORY);
    StackPrintErrCheck (ERR_UNDERFLOW);
    StackPrintErrCheck (ERR_INCORRECT_SIZE);
    StackPrintErrCheck (ERR_INCORRECT_CAPACITY);

#ifdef STACK_CANARY_PROTECTION
    StackPrintErrCheck (ERR_LEFT_CANARY);
    StackPrintErrCheck (ERR_RIGHT_CANARY);
#endif

#ifdef STACK_CANARY_PROTECTION
    StackPrintErrCheck (ERR_LEFT_CANARY_DATA);
    StackPrintErrCheck (ERR_RIGHT_CANARY_DATA);
#endif

#ifdef STACK_HASH_PROTECTION
    StackPrintErrCheck (ERR_HASH_STACK);
    StackPrintErrCheck (ERR_HASH_DATA);
#endif

#undef StackPrintErrCheck

    STL_Print ("%s", errStr);

    return errStr;
}

static void STL_Print (const char* const fmt, ...)
{
    assert   (fmt);
    FILE* fp = fopen (logFile, "a");

    va_list   args = {};

    va_start (args, fmt);

    vfprintf (stderr, fmt, args);
    vfprintf (fp, fmt, args);

    va_end   (args);

    fclose   (fp);
    fp = nullptr;
}
