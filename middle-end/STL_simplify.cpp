#include "STL_simplify.h"

static int
CountNodes (NodeBinTree* node);

NodeBinTree*
STL_SimplifyEval (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr) return nullptr;

    int initialnNode = CountNodes (node);
    int nNode        = initialnNode + 1;
    int newNode      = initialnNode;
    int nOldNode     = nNode;
    int nSimplify    = 0;

    printf ("\n\nБыло узлов: %d\n\n", initialnNode);

    while (newNode < nOldNode)
    {
        printf ("AAAA");
        nOldNode = nNode;

        nNode = newNode;
        STL_FoldConst (node, fp);
        newNode = CountNodes (node);

        if (newNode < nNode)
        {
            printf ("Проведено STL_FoldConst\n");
            printf ("Стало узлов: %d\n\n", newNode);
            nSimplify++;
        }

//        nNode   = newNode;
//        node    = STL_DeleteNeutralNode (node, fp);
//        newNode = CountNodes (node);
//
//        if (newNode < nNode)
//        {
//            printf ("Проведено STL_DeleteNeutralNode\n");
//            printf ("Стало узлов: %d\n\n", newNode);
//            nSimplify++;
//        }
    }

    printf ("\nПроведено упрощений %d\n", nSimplify);
    printf ("Количество узлов уменьшиловь на %d\n\n", initialnNode - newNode);

    return node;
}

int
STL_FoldConst (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr) return 0;

    if (node->data->type == NodeBinTreeData::T_VALUE)
        return 0;
    if (node->data->type == NodeBinTreeData::T_VARIABLE)
        return 1;
    if (node->data->type == NodeBinTreeData::T_FUNCTION)
    {
        int left  = STL_FoldConst (node->left,  fp);
        int right = STL_FoldConst (node->right, fp);
        return 1;
    }

    int left  = STL_FoldConst (node->left,  fp);
    int right = STL_FoldConst (node->right, fp);

    if (left == 0 && right == 0)
    {
        switch (node->data->opCode)
        {
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case POW:
            case SIN:
            case COS:
            case LN:

                break;

            default:

                return 1;
        }

        if (node->left == nullptr ||
            node->left->data->type != NodeBinTreeData::T_VALUE)
            return 1;

        if (node->right == nullptr ||
            node->right->data->type != NodeBinTreeData::T_VALUE)
            return 1;

        printf (" %s\n", keyWords[node->data->opCode]);

        node->data->value  = Eval (node, fp);
        printf (" val = %d\n", node->data->value);

        node->data->opCode = NodeBinTreeData::OPCODE_POISON;
        node->data->type   = NodeBinTreeData::T_VALUE;

        node->left  = NodeBinTreeDtor (node->left);
        node->right = NodeBinTreeDtor (node->right);

        return 0;
    }

    return 1;
}

#define POISON      NodeBinTreeData::VALUE_POISON
#define T_VAL       NodeBinTreeData::T_VALUE
#define T_OP        NodeBinTreeData::T_OPCODE
#define dataL       node->left->data->value
#define dataR       node->right->data->value
#define _CONST(x)   NodeBinTreeCtor (NodeBinTreeDataCtor (T_VAL, x))
#define _MUL(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '*'), x, y)

NodeBinTree*
STL_DeleteNeutralNode (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr) return 0;
    if (node->data->type != NodeBinTreeData::T_OPCODE) return node;

    node->left  = STL_DeleteNeutralNode (node->left,  fp);
    node->right = STL_DeleteNeutralNode (node->right, fp);

    NodeBinTree* node1 = nullptr;

    switch (node->data->opCode)
    {
        case '+':

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    node1 = node->right;
            }

            if (IsConstData (node->right))
            {
                if (dataR == 0)
                    node1 = node->left;
            }

            break;

        case '-':

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    node1 = _MUL (node->right, _CONST (-1));
            }

            if (IsConstData (node->right))
            {
                if (dataR == 0)
                    node1 = node->left;
            }

            break;

        case '*':

            if (IsConstData (node->left))
            {
                if (dataL == 1)
                    node1 = node->right;

                if (dataL == 0)
                    node1 = _CONST (0);
            }

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    node1 = node->left;

                if (dataR == 0)
                    node1 = _CONST (0);

                if (dataR == -1)
                    node1 = _MUL (_CONST (-1), node->left);
            }

            break;

        case '/':

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    node1 = _CONST (0);
            }

            break;

        case '^':

            if (IsConstData (node->left))
            {
                if (dataL == 1)
                    return _CONST (1);

                if (dataL == 0)
                    node1 = _CONST (0);
            }

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    return node->left;

                if (dataR == 0)
                    node1 = _CONST (1);
            }

            break;

        case LN:

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    node1 = _CONST (0);
            }

            break;

        case SIN:
        case COS:

            break;

        default:

            break;
    }

    if (node1 != nullptr)
    {
        return node1;
    }

    return node;
}

#undef POISON
#undef T_VAL
#undef T_OP
#undef dataL
#undef dataR
#undef _CONST
#undef _MUL

static int
CountNodes (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    return CountNodes (node->left) + CountNodes (node->right) + 1;
}
