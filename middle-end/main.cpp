#include <stdio.h>

#include "STL_simplify.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/1tree.cpp";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/1tree.cpp";
    else argv2 = argv[2];

    File file = { .name = argv1 };

    BinTree* tree = BinTreeReadPostorderWithoutNil (&file);
    if (tree == nullptr)
    {
        printf ("TREE NULLPTR!!!\n\n");
        return 0;
    }

    tree->root = STL_SimplifyEval (tree->root, nullptr);

    BinTreePrintPostorderWithoutNil (tree, argv2);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
