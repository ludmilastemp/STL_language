#include <stdio.h>

#include "RecRead.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/1.cpp";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/1tree.cpp";
    else argv2 = argv[2];

    File file = { .name = argv1 };
    STL_Fread (&file);

    BinTree* tree = BinTreeCtor ();
    if (tree == nullptr) return 0;

    tree->buf = file.text;

    Stack_NodeBinTreeData token = { 0 };
    StackCtor (&token);

    Stack_Variable func = { 0 };
    StackCtor (&func);

    Stack_Variable var = { 0 };
    StackCtor (&var);

    RecursiveDescentCtx ctx = {.str   = tree->buf,
                               .pos   = 0,
                               .func  = &func,
                               .var   = &var,
                               .token = &token};

    LexicalAnalysis (&ctx);

//    printf ("COUNT TOKENS = %d\n", (int)token.size); // warning without (int)

    for (int i = 0; i < (int)token.size; i++)
    {
        printf ("\ni = %d"
                "\n\ttype     = %d"
                "\n\tvalue    = %d"
                "\n\topCode   = %d"
                "\n\tvariable = %d"
                "\n\tfunction = %d\n",
                 i,
                 token.data[i].type,
                 token.data[i].value,
                 token.data[i].opCode,
                 token.data[i].variable,
                 token.data[i].function);
    }

    tree->root = ParseProgram (&ctx);

    STL_GraphvizBinTree (tree, after_front_end);
    BinTreePrintPostorderWithoutNil (tree, argv2);

    StackDtor (&token);
    StackDtor (&func);
    StackDtor (&var);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
