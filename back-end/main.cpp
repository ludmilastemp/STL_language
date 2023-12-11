#include <stdio.h>

#include "back-end.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int /* argc */, const char** /* argv */)
{
    File file = { .name = "examples/1tree.cpp" };

    BinTree* tree = BinTreeReadPostorderWithoutNil (&file);
    if (tree == nullptr) printf ("TREE NULLPTR!!!\n\n");

    STL_GraphvizBinTree (tree, 2);

    FILE* fp = fopen ("examples/1asm.cpp", "w");
    assert (fp);

    GetMultipleOperations (tree->root, fp);

    fclose (fp);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
