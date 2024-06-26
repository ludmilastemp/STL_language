#include <stdio.h>

#include "RecRead.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int argc, const char** argv)
{                 //          EEEEEEEELLLLLSSSSSEEEEEEE
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/factorial.stl";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/tree.ast";
    else argv2 = argv[2];

    File file = { .name = argv1 };
    STL_Fread (&file);

    BinTree* tree = BinTreeCtor ();
    if (tree == nullptr) return 0;
    printf ("Read OK\n");

    tree->buf = file.text;

    Stack_NodeBinTreeData token = { 0 };
    StackCtor (&token);

    Stack_Function func = { 0 };
    StackCtor (&func);

    Stack_Variable var = { 0 };
    StackCtor (&var);

    RecursiveDescentCtx ctx = {.str   = tree->buf,
                               .func  = &func,
                               .var   = &var,
                               .token = &token};

    LexicalAnalysis (&ctx);
 
//    printf ("COUNT TOKENS = %d\n", (int)token.size); // warning without (int)

    // for (int i = 0; i < (int)token.size; i++)
    // {
    //     printf ("\ni = %d"
    //             "\n\ttype     = %d"
    //             "\n\tvalue    = %d"
    //             "\n\topCode   = %d"
    //             "\n\tfunction = %d"
    //             "\n\tvariable = %d\n",
    //              i,
    //              token.data[i].type,
    //              token.data[i].value,
    //              token.data[i].opCode,
    //              token.data[i].function,
    //              token.data[i].variable);
    // }

    tree->root = ParseProgram (&ctx);
    tree->var  = &var;

    STL_GraphvizBinTree (tree, after_front_end);
    BinTreePrintPostorderWithoutNil (tree, argv2);

    BinTreeDtor (tree);
    
    StackDtor (&token);
    StackDtor (&func);
    StackDtor (&var);

    STL_Fclose (&file);

    printf ("\n\nEND!!!\n\n");

    return 0;
}
