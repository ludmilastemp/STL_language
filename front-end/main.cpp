#include <stdio.h>

#include "RecRead.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

int main (const int argc, const char** argv)
{                 //          EEEEEEEELLLLLSSSSSEEEEEEE
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
    printf ("Read OK\n");

    tree->buf = file.text;

    Stack_NodeBinTreeData token = { 0 };
    StackCtor (&token);

    Stack_Variable func = { 0 };
    StackCtor (&func);

    Stack_Variable var = { 0 };
    StackCtor (&var);

    RecursiveDescentCtx ctx = {.str   = tree->buf,
                               .pos   = 0,
                               .nFunc = 0,
                               .nVarInMain = 0,
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

//#include <stdio.h>
//
//int* p = 0;
//
//int Func (int n)
//{
//    int k = 0;
//
//    printf ("k = [%p]\n", &k);
//
//    if (n > 1)
//    {
//        k = Func (n - 1);
//    }
//    else k = 1;
//
//    printf ("k = [%p]\n", &k);
//    printf ("k = %d\n", k);
//    printf ("p = [%p]\n", p);
//    printf ("p = %d\n\n", *p);
//    printf ("\n");
//    p = &k;
//
//    return k;
//
//}
//
//
//int main ()
//{
//    int n = 4;
//
//    p = &n;
//
//    Func (n);
//
//    return 0;
//}
