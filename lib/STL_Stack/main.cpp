#include "STACK_FILE_TYPE_int.h"
#include "generic.h"

int main ()
{
    int temp_int = 0;
    Stack_int stk_int = { };
    StackCtor (&stk_int);
    StackPush (&stk_int, 57);
    StackPush (&stk_int, 68);
    StackPush (&stk_int, 79);
    StackPop  (&stk_int, &temp_int);
    StackDump (&stk_int);
    StackDtor (&stk_int);
}
