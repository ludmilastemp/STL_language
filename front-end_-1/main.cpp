#include <stdio.h>

#include "back-end.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/1tree.cpp";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/1front_-1.cpp";
    else argv2 = argv[2];

    File file = { .name = argv1 };

    Stack_Variable var = { 0 };
    StackCtor (&var);

    BinTree* tree = BinTreeReadPostorderWithoutNil (&file, &var);
    if (tree == nullptr)
    {
        printf ("TREE NULLPTR!!!\n\n");
        return 0;
    }

    FILE* fp = fopen (argv2, "w");
    assert (fp);

    BackEndCtx ctx = { .node       = tree->root,
                       .fp         = fp,
                       .var        = &var,
                       .nIf        = 1,
                       .nWhile     = 1,
                       .nVarInFunc = 0,
                       .nVarBefore = 0 };

    CompileProgram (&ctx);

    fclose (fp);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
