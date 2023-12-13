#include <stdio.h>

#include "RecRead.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int argc , const char**  argv )
{
    File file = { .name = "examples/1.cpp" }; // gj evjkxfyb. argv

    STL_Fread (&file); //?? ub if

    BinTree* tree = BinTreeCtor ();        //
    tree->buf = file.text;

    Stack_NodeBinTreeData token = { 0 };
    StackCtor (&token);

    Stack_Variable var = { 0 };
    StackCtor (&var);

    RecursDescent recDescent = {.str   = tree->buf,
                                .pos   = 0,
                                .error = 0,
                                .var   = &var,
                                .token = &token};

    LexicalAnalysis (&recDescent);

//    printf ("COUNT TOKENS = %d\n", (int)token.size); // warning without (int)

//    for (int i = 0; i < (int)token.size; i++)
//    {
//        printf ("\ni = %d"
//                "\n\ttype     = %d"
//                "\n\tvalue    = %d"
//                "\n\topCode   = %d"
//                "\n\tvariable = %d",
//                 i,
//                 token.data[i].type,
//                 token.data[i].value,
//                 token.data[i].opCode,
//                 token.data[i].variable);
//    }

    recDescent.pos = 0;   // in get mul op
    tree->root = GetMultipleOperations (&recDescent);    // check \0

    STL_GraphvizBinTree (tree, 1);
    BinTreePrintPostorderWithoutNil (tree, "examples/1tree.cpp");

    StackDtor (&token);

    printf ("\n\nОК!!!\n\n");

    return 0;
}
