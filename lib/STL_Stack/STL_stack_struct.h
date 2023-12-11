#ifndef STL_stack_struct_

#ifdef STACK_FILE_TYPE
    #include STACK_FILE_TYPE
    #define  STL_stack_struct_

    #include "STL_stack_list_of_errors.h"
#endif

#ifndef DBG_E

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* DBG_E */

#ifdef STACK_CANARY_PROTECTION
    typedef long long CanaryType;
    #define CANARY_PRINT_SPECIFIER "%d"
#endif

/// структура стека
struct STACK
{
#ifdef STACK_CANARY_PROTECTION
    CanaryType leftCanary;
#endif

#ifdef STACK_DEBUG
    size_t       CREATE_LINE;
    const char*  CREATE_FILE;
    const char*  CREATE_FUNC;
    const char*  CREATE_NAME;
#endif

    STACK_T* data;
    long long size;
    long long capacity;
    StackErrorType err;

#ifdef STACK_HASH_PROTECTION
    long long hashStack;
    long long hashData;
#endif

#ifdef STACK_CANARY_PROTECTION
    CanaryType rightCanary;
#endif
};

#endif /* STL_stack_struct_ */
