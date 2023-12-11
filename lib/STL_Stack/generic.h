
#ifndef STACK
#define CONCAT(arg1, arg2) arg1##arg2
#define STACK_TYPED_WITH(type) CONCAT (Stack_, type)
#define STACK STACK_TYPED_WITH(STACK_T)
#endif

#include "STL_stack_list_of_errors.h"
#include "STL_stack_struct.h"
#include "STL_stack_functions.h"
#include "STL_stack_print_error.h"

#undef STACK
#undef STACK_T
#undef STACK_DATA_PRINT_SPECIFIER


