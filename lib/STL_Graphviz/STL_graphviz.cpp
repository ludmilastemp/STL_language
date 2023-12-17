#include "STL_graphviz.h"

#define pr(...) fprintf (fp, __VA_ARGS__)

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num);//, Stack_Variable* stk);

void STL_GraphvizBinTree (BinTree* tree, const int file)
{
    if (tree == nullptr) return;

    FILE* fp = fopen ("temp/STL_graphviz_png.dot", "w");
    assert (fp);

    pr ("digraph STL\n{\n"
        "rankdir = TB\n"
        "graph [ splines = ortho, splines = true ]\n"
        "node  [ shape = record, style = filled, fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n"
        "edge  [ arrowhead = none ]\n\n");

    int num = 0;

    PrintSubtree (fp, tree->root, &num);//, tree->variable);

    pr ("\n}\n");

    fclose (fp);

    if (file == after_front_end) system ("dot -Tpng .\\temp/STL_graphviz_png.dot -o temp/STL1.png");
    if (file == after_back_end)  system ("dot -Tpng .\\temp/STL_graphviz_png.dot -o temp/STL2.png");
}

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num)//, Stack_Variable* stk)
{
    fprintf (fp, "\nf%d", *num);

    if (node == nullptr)
    {
        pr (" [ label = nil, \
        fillcolor = \"#FFE0E0\", color = \"#FFA0A0\" ]\n");

        (*num)++;

        return;
    }

    int curNum = *num;

    switch (node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            pr (" [ label = " BIN_TREE_DATA_VALUE_PRINT_SPECIFIER ", \
            fillcolor = \"#E0FFE0\", color = \"#ACFFAC\" ]\n", node->data->value);

            break;

        case NodeBinTreeData::T_OPCODE:

            pr (" [ label = \" %s \", \
            fillcolor = \"#E0FFE0\", color = \"#ACFFAC\" ]\n", operation[node->data->opCode]);

            break;

        case NodeBinTreeData::T_VARIABLE:

            pr (" [ label = \"var%d\", \
            fillcolor = \"#FFE0FF\", color = \"#FFA0FF\" ]\n", node->data->variable + 1);

            break;

        case NodeBinTreeData::T_FUNCTION:

            pr (" [ label = \"func%d\", \
            fillcolor = \"#FFE0DD\", color = \"#FFA0DD\" ]\n", node->data->function + 1);

            break;

        default:
            printf ("\nGraphviz error\nnum = %d\ttype = %d", *num, node->data->type);
            printf ("\
                 \n\ttype     = %d\
                 \n\tvalue    = %d\
                 \n\topCode   = %d\
                 \n\tvariable = %d",
                 node->data->type,
                 node->data->value,
                 node->data->opCode,
                 node->data->variable);
            break;
    }

    (*num)++;

//    if (node->left != nullptr)
    if (node != nullptr)
    {
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->left, num);//, stk);
    }

//    if (node->right != nullptr)
    if (node != nullptr)
    {
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->right, num);//, stk);
    }

    (*num)++;
}

#undef pr

