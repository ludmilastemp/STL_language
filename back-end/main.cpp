#include <stdio.h>

#include "back-end.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/1tree.cpp";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/1asm.cpp";
    else argv2 = argv[2];

    File file = { .name = argv1 };

    BinTree* tree = BinTreeReadPostorderWithoutNil (&file);
    if (tree == nullptr)
    {
        printf ("TREE NULLPTR!!!\n\n");
        return 0;
    }

    STL_GraphvizBinTree (tree, after_back_end);

    FILE* fp = fopen (argv2, "w");
    assert (fp);

    BackEndCtx ctx = { .node = tree->root,
                       .fp = fp,
                       .nIf = 1,
                       .nWhile = 1 };

    CompileProgram (&ctx);

    /**
     *  Bug in asm
     */
    fprintf (fp, "\n\n:aaa");

    fclose (fp);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
