#include "back-end.h"

static int GetOperation  (NodeBinTree* node, FILE* fp);
static int GetPrintf     (NodeBinTree* node, FILE* fp);
static int GetAssign     (NodeBinTree* node, FILE* fp);
static int GetCreate     (NodeBinTree* node, FILE* fp);

int GetMultipleOperations (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr || fp == nullptr) return 1;

    while (node != nullptr)
    {
        if (node->data->opCode != END_STR)
        {
            printf ("ERROR in GetMultipleOperations!!!\n\n");
            return 1;
        }

        GetOperation (node->left, fp);
        node = node->right;
    }

    return 0;
}

static int GetOperation (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr || fp == nullptr) return 1;

    int check = 1;
    check = check && GetPrintf (node, fp);
    check = check && GetAssign (node, fp);
    check = check && GetCreate (node, fp);

    if (check == 1)
    {
        printf ("\nERROR in GetOperation!!!\n\n");

        printf ("\n\ttype     = %d\
                 \n\tvalue    = %d\
                 \n\topCode   = %d\
                 \n\tvariable = %d",
                 node->data->type,
                 node->data->value,
                 node->data->opCode,
                 node->data->variable);
        return 1;
    }

    return 0;
}

static int GetPrintf (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr || fp == nullptr) return 1;

    if (node->data->opCode   != PRINTF) return 1;
    node = node->left;

    if (node->data->variable != 0) return 1;

    fprintf (fp, "\n\t\tpush rax\
                  \n\t\tOUT");

    return 0;
}

static int GetAssign (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr || fp == nullptr) return 1;

    if (node       ->data->opCode   != ASSING) return 1;
    if (node->right->data->type     != NodeBinTreeData::T_VALUE) return 1;
    if (node->left ->data->variable != 0) return 1;

    fprintf (fp, "\n\t\tpush %d\
                  \n\t\tpop rax", node->right->data->value);

    return 0;
}

static int GetCreate (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr || fp == nullptr) return 1;

    if (node->data->opCode != T_INT) return 1;

    return 0;
}
