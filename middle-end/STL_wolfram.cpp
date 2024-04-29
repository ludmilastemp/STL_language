#include "STL_wolfram.h"

double
Eval (NodeBinTree* node, FILE* fp)
{
    if (node == nullptr) return 0;

    switch (node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            return node->data->value;

        case NodeBinTreeData::T_VARIABLE:
        case NodeBinTreeData::T_FUNCTION:

            return NAN;

        case NodeBinTreeData::T_OPCODE:

            break;

        default:

            printf ("ERROR INCORRECT TYPE!!! \n"
                    "\t type = %d\n\n", node->data->type);
    }

    double left   = node->left->data->value;
    double right  = node->right->data->value;
    double answer = 0;

    printf ("    left  = %d\n", left);
    printf ("    right = %d\n", right);

    switch (node->data->opCode)
    {
        case '+':

            answer = left + right;

            break;

        case '-':

            answer = left - right;

            break;

        case '*':

            answer = left * right;

            break;

        case '/':

            if (right == 0) assert (right);
            answer = left / right;

            break;

        case '^':

            answer = pow (left, right);

            break;

        case SIN:

            answer = sin (right);

            break;

        case COS:

            answer = cos (right);

            break;

        case LN:

            if (right == 0) assert (right);

            if (node->left == nullptr) answer = log (right);
            else answer = log (right) / log (left);

            break;

        default:

            break;
    }

    switch (node->data->opCode)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case SIN:
        case COS:
        case LN:

            return answer;

        default:

            break;
    }

    return 0;
}

#define POISON      NodeBinTreeData::VALUE_POISON
#define T_VAL       NodeBinTreeData::T_VALUE
#define T_OP        NodeBinTreeData::T_OPCODE
#define T_VAR       NodeBinTreeData::T_VARIABLE
#define dL          PartialDerivative (node->left,  variable, nVariable, fp)
#define dR          PartialDerivative (node->right, variable, nVariable, fp)
#define cN          BinTreeCopy     (node)
#define cL          BinTreeCopy     (node->left)
#define cR          BinTreeCopy     (node->right)
#define constL      NodeBinTreeCtor (NodeBinTreeDataCtor (node->left->data->type,  node->left->data->value))
#define constR      NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->type, node->right->data->value))
#define _VAR(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (T_VAR,                                 \
                                                          NodeBinTreeData::VALUE_POISON,         \
                                                          NodeBinTreeData::OPCODE_POISON,        \
                                                          x), nullptr, nullptr)
#define _CONST(x)   NodeBinTreeCtor (NodeBinTreeDataCtor (T_VAL, x))
#define _ADD(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '+'), x, y)
#define _SUB(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '-'), x, y)
#define _MUL(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '*'), x, y)
#define _DIV(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '/'), x, y)
#define _POW(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, '^'), x, y)
#define _SIN(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, SIN), nullptr, x)
#define _COS(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, COS), nullptr, x)
#define _LN(x)      NodeBinTreeCtor (NodeBinTreeDataCtor (T_OP, POISON, LN),  nullptr, x)
                                  /*
BinTree*
Differentiate (BinTree* tree, FILE* fp)
{
    if (tree == nullptr) return nullptr;

    NodeBinTree* node = tree->root;
    if (fp) fprintf (fp, "\n\n\nЛегко и просто возьмем производную по %s\\\\\n\n", tree->variable->data[0].name);
    node = cN;
    node = PartialDerivative (node, 0, fp);

//    if (tree->variable->size != 1)
//    {
//        node = _POW (node, _CONST (2));
//
//        for (int i = 1; i < tree->variable->size; i++)
//        {
//            if (fp) fprintf (fp, "\n\nЛегко и просто возьмем производную по %s\\\\\n\n", tree->variable->data[i].name);
//            node = _ADD (node, _POW (PartialDerivative (node, tree->variable, i, fp), _CONST (2)));
//        }
//
//        node = _POW (node, _CONST (0.5));
//    }

    BinTree* dtree = BinTreeCtor (node);
    dtree->buf = tree->buf;
    CopyStack (tree, dtree);

    return dtree;
}

NodeBinTree*
PartialDerivative (NodeBinTree* node, int nVariable, FILE* fp)
{
    if (node == nullptr) return nullptr;

    NodeBinTree* node1 = nullptr;

    switch (node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            if (fp) fprintf (fp, "\nЛегким движением руки $$%lg' = 0$$\n", node->data->value);
            return _CONST (0);

        case NodeBinTreeData::T_VARIABLE:

            if (nVariable != node->data->variable)
            {
                if (fp) fprintf (fp, "\nЛегким движением руки $$%s' = 0$$\n", variable->data[node->data->variable].name);
                return _CONST (0);
            }

            if (fp) fprintf (fp, "\nЛегким движением руки $$%s' = 1$$\n", variable->data[node->data->variable].name);
            return _CONST (1);

        case NodeBinTreeData::T_OPCODE:

            break;

        default:

            printf ("ERROR INCORRECT TYPE2!!! \n"
                    "\t type = %d\n\n", node->data->type);
    }

    switch (node->data->opCode)
    {
        case '+':

            node1 =
            _ADD (dL, dR);

            break;

        case '-':

            node1 =
            _SUB (dL, dR);

            break;

        case '*':

            node1 =
            _ADD (_MUL (dL, cR),
                  _MUL (cL, dR));

            break;

        case '/':

            node1 =
            _DIV (_SUB (_MUL (dL, cR),
                        _MUL (cL, dR)),
                  _POW (cR, _CONST (2)));

            break;

        case '^':
                /// a ^ b
            if (IsConstData (node->left) &&
                IsConstData (node->right))
            {
                node1 = _CONST (0);
            }
                /// f(x) ^ a
            else if (IsConstData (node->right))
            {
                node1 =
                _MUL (_MUL (constR,
                            _POW (cL, _CONST (node->right->data->value - 1))),
                      dL);
            }
                /// a ^ f(x)
            else if (IsConstData (node->left))
            {
                node1 =
                _MUL (_MUL (cN,
                            _LN (constL)),
                      dR);
            }
                /// f(x) ^ g(x)
            else
                node1 =
                _MUL (cN,
                      _ADD (_MUL (dR,
                                  _LN (node->left)),
                            _MUL (_DIV (cR, cL),
                                  dL)));
            break;

        case LN:
                /// ln f(x)
            if (node->left == nullptr)
            {
                node1 =
                _DIV (dR, cR);
            }
                /// log_a  b
            else if (IsConstData (node->left) &&
                     IsConstData (node->right))
            {
                node1 = _CONST (0);
            }
                /// log_a  f(x)
            else if (IsConstData (node->left))
            {
                node1 =
                _DIV (dR,
                      _MUL (cR,
                            _LN (constL)));
            }
                /// log_f(x)  a
            else if (IsConstData (node->right))
            {
                node1 =
                _DIV (_MUL (_MUL (_CONST (-1),
                                  _LN (constR)),
                            dR),
                      _MUL (cL,
                            _POW (_LN (constR),
                                  _CONST (2))));
            }
            // log_f(x)  g(x)

        case SIN:

            node1 = _MUL (_COS (node->right),
                         dR);

            break;

        case COS:

            node1 = _MUL (_MUL (_CONST (-1),
                               _SIN (node->right)),
                         dR);

            break;

        default:

            printf ("INCORRECT OPCODE!!!");
            return nullptr;
    }

    if (fp)
    {
        fprintf (fp, "\nПроизводная от\n");
        STL_Latex (node, variable, fp);
        fprintf (fp, "\nравна");
        STL_Latex (node1, variable, fp);
        fprintf (fp, "\n\n");
    }

    return node1;
}

BinTree*
Taylor (BinTree* tree, int accuracy, double coordinate, FILE* fp)
{
    if (tree == nullptr) return nullptr;

    double oldVar = tree->variable->data[0].value;
    tree->variable->data[0].value = coordinate;

    NodeBinTree* node = _ADD (_CONST ( Eval (tree->root, tree->variable, fp)), nullptr);
    tree->variable->data[0].value = oldVar;

    NodeBinTree* dnode = BinTreeCopy (tree->root);
    NodeBinTree* node1 = node;

    int factorial = 1;
    for (int i = 1; i < accuracy; i++)
    {
        factorial *= i;

        if (fp) fprintf (fp, "\nПроизводная степени %d:\\\\\n\n", i);
        dnode = PartialDerivative (dnode, tree->variable, 0, fp);

        dnode = STL_SimplifyEval (dnode, tree->variable, fp);
        if (fp) fprintf (fp, "\n\nУпрощенный вид производной стпени %d:\n\n", i);
        if (fp) STL_Latex (dnode, tree->variable, fp);

        node1->right = _ADD (_MUL ( _DIV (dnode, _CONST (factorial)),
                                    _POW ( _SUB (_VAR(0), _CONST (coordinate)),
                                           _CONST (i))),
                             _CONST (0));

        node1 = node1->right;
    }

    BinTree* ttree = BinTreeCtor (node);
    ttree->buf = tree->buf;
    CopyStack (tree, ttree);

    return ttree;
}

#undef POISON
#undef dL
#undef dR
#undef cL
#undef cR
#undef constL
#undef constR
#undef _VAR
#undef _CONST
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
#undef _POW
#undef _SIN
#undef _COS
#undef _LN
                                */

int
IsConstData (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    if (node->data->type == NodeBinTreeData::T_VALUE)
    {
        return 1;
    }

    return 0;
}
