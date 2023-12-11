#ifndef STL_stack_list_of_errors_
#define STL_stack_list_of_errors_

typedef int StackErrorType;

enum StackErrors
{
    ERR_NOT_STACK_POINTER  = 1 << 18,
    ERR_NOT_DATA_POINTER   = 1 << 0,
    ERR_INCORRECT_SIZE     = 1 << 1,
    ERR_INCORRECT_CAPACITY = 1 << 2,
    ERR_NOT_MEMORY         = 1 << 3,
    ERR_UNDERFLOW          = 1 << 4,

#ifdef STACK_CANARY_PROTECTION
    ERR_LEFT_CANARY        = 1 << 5,
    ERR_RIGHT_CANARY       = 1 << 6,
#endif

#ifdef STACK_CANARY_PROTECTION
    ERR_LEFT_CANARY_DATA   = 1 << 7,
    ERR_RIGHT_CANARY_DATA  = 1 << 8,
#endif

#ifdef STACK_HASH_PROTECTION
    ERR_HASH_STACK         = 1 << 9,
    ERR_HASH_DATA          = 1 << 10,
#endif
};

#endif /* STL_stack_list_of_errors_ */
