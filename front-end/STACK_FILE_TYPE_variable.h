#ifndef STACK_FILE_TYPE_Variable_
#define STACK_FILE_TYPE_Variable_

#include <stdio.h>

struct Variable
{
    char* name;
    size_t len; 
};

#define STACK_T Variable

#endif /* STACK_FILE_TYPE_Variable_ */
